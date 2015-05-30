//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdlib>
#include <stdint.h>
#include <functional>

namespace lol
{

/*
 * Ticker implementation class
 */

static class TickerData
{
    friend class Ticker;

public:
    TickerData() :
        nentities(0),
        frame(0), recording(0), deltatime(0), bias(0), fps(0),
#if LOL_BUILD_DEBUG
        keepalive(0),
#endif
        quit(0), quitframe(0), quitdelay(20), panic(0)
    {
    }

    ~TickerData()
    {
        ASSERT(nentities == 0,
               "still %d entities in ticker\n", nentities);
        ASSERT(m_autolist.count() == 0,
               "still %d autoreleased entities\n", m_autolist.count());
        msg::debug("%d frames required to quit\n", frame - quitframe);

#if LOL_FEATURE_THREADS
        gametick.push(0);
        disktick.push(0);
        delete gamethread;
        delete diskthread;
#endif
    }

private:
    /* Entity management */
    array<Entity *> m_todolist, m_autolist;
    array<Entity *> m_list[Entity::ALLGROUP_END];
    array<int> m_scenes[Entity::ALLGROUP_END];
    int nentities;

    /* Fixed framerate management */
    int frame, recording;
    Timer timer;
    float deltatime, bias, fps;
#if LOL_BUILD_DEBUG
    float keepalive;
#endif

    /* The three main functions (for now) */
    static void GameThreadTick();
    static void DrawThreadTick();
    static void DiskThreadTick();

#if LOL_FEATURE_THREADS
    /* The associated background threads */
    void GameThreadMain();
    void DrawThreadMain(); /* unused */
    void DiskThreadMain();
    thread *gamethread, *drawthread, *diskthread;
    queue<int> gametick, drawtick, disktick;
#endif

    /* Shutdown management */
    int quit, quitframe, quitdelay, panic;
}
tickerdata;

static TickerData * const data = &tickerdata;

/*
 * Ticker public class
 */

void Ticker::Register(Entity *entity)
{
    /* If we are called from its constructor, the object's vtable is not
     * ready yet, so we do not know which group this entity belongs to. Wait
     * until the first tick. */
    data->m_todolist.push(entity);

    /* Objects are autoreleased by default. Put them in a list. */
    data->m_autolist.push(entity);
    entity->m_autorelease = 1;
    entity->m_ref = 1;

    data->nentities++;
}

void Ticker::Ref(Entity *entity)
{
    ASSERT(entity, "dereferencing nullptr entity\n");
    ASSERT(!entity->m_destroy,
           "referencing entity scheduled for destruction %s\n",
           entity->GetName());

    if (entity->m_autorelease)
    {
        /* Get the entity out of the m_autorelease list. This is usually
         * very fast since the last entry in autolist is the last
         * registered entity. */
        for (int i = data->m_autolist.count(); i--; )
        {
            if (data->m_autolist[i] == entity)
            {
                data->m_autolist.remove_swap(i);
                break;
            }
        }
        entity->m_autorelease = 0;
    }
    else
        entity->m_ref++;
}

int Ticker::Unref(Entity *entity)
{
    ASSERT(entity, "dereferencing null entity\n");
    ASSERT(entity->m_ref > 0, "dereferencing unreferenced entity %s\n",
           entity->GetName());
    ASSERT(!entity->m_autorelease, "dereferencing autoreleased entity %s\n",
           entity->GetName());

    return --entity->m_ref;
}

#if LOL_FEATURE_THREADS
void TickerData::GameThreadMain()
{
#if LOL_BUILD_DEBUG
    msg::info("ticker game thread initialised\n");
#endif

    for (;;)
    {
        int tick = gametick.pop();
        if (!tick)
            break;

        GameThreadTick();

        drawtick.push(1);
    }

    drawtick.push(0);

#if LOL_BUILD_DEBUG
    msg::info("ticker game thread terminated\n");
#endif
}
#endif /* LOL_FEATURE_THREADS */

#if LOL_FEATURE_THREADS
void TickerData::DrawThreadMain() /* unused */
{
#if LOL_BUILD_DEBUG
    msg::info("ticker draw thread initialised\n");
#endif

    for (;;)
    {
        int tick = drawtick.pop();
        if (!tick)
            break;

        DrawThreadTick();

        gametick.push(1);
    }

#if LOL_BUILD_DEBUG
    msg::info("ticker draw thread terminated\n");
#endif
}
#endif /* LOL_FEATURE_THREADS */

#if LOL_FEATURE_THREADS
void TickerData::DiskThreadMain()
{
    /* FIXME: temporary hack to avoid crashes on the PS3 */
    disktick.pop();
}
#endif /* LOL_FEATURE_THREADS */

//-----------------------------------------------------------------------------
void TickerData::GameThreadTick()
{
    Profiler::Stop(Profiler::STAT_TICK_FRAME);
    Profiler::Start(Profiler::STAT_TICK_FRAME);

    Profiler::Start(Profiler::STAT_TICK_GAME);

#if 0
    msg::debug("-------------------------------------\n");
    for (int g = 0; g < Entity::ALLGROUP_END; ++g)
    {
        msg::debug("%s Group %d\n",
                   (g < Entity::GAMEGROUP_END) ? "Game" : "Draw", g);

        for (int i = 0; i < data->m_list[g].count(); ++i)
        {
            Entity *e = data->m_list[g][i];
            msg::debug("  \\-- [%p] %s (m_ref %d, destroy %d)\n",
                       e, e->GetName(), e->m_ref, e->m_destroy);
        }
    }
#endif

    data->frame++;

    /* Ensure some randomness */
    rand<int>();

    /* If recording with fixed framerate, set deltatime to a fixed value */
    if (data->recording && data->fps)
    {
        data->deltatime = 1.f / data->fps;
    }
    else
    {
        data->deltatime = data->timer.Get();
        data->bias += data->deltatime;
    }

    /* Do not go below 15 fps */
    if (data->deltatime > 1.f / 15.f)
    {
        data->deltatime = 1.f / 15.f;
        data->bias = 0.f;
    }

#if LOL_BUILD_DEBUG
    data->keepalive += data->deltatime;
    if (data->keepalive > 10.f)
    {
        msg::info("ticker keepalive: tick!\n");
        data->keepalive = 0.f;
    }
#endif

    /* If shutdown is stuck, kick the first entity we meet and see
     * whether it makes things better. Note that it is always a bug to
     * have referenced entities after 20 frames, but at least this
     * safeguard makes it possible to exit the program cleanly. */
    if (data->quit && !((data->frame - data->quitframe) % data->quitdelay))
    {
        int n = 0;
        data->panic = 2 * (data->panic + 1);

        for (int g = 0; g < Entity::ALLGROUP_END && n < data->panic; ++g)
        for (int i = 0; i < data->m_list[g].count() && n < data->panic; ++i)
        {
            Entity * e = data->m_list[g][i];
            if (e->m_ref)
            {
#if !LOL_BUILD_RELEASE
                msg::error("poking %s\n", e->GetName());
#endif
                e->m_ref--;
                n++;
            }
        }

#if !LOL_BUILD_RELEASE
        if (n)
            msg::error("%d entities stuck after %d frames, poked %d\n",
                       data->nentities, data->quitdelay, n);
#endif

        data->quitdelay = data->quitdelay > 1 ? data->quitdelay / 2 : 1;
    }

    /* Garbage collect objects that can be destroyed. We can do this
     * before inserting awaiting objects, because only objects already
     * in the tick lists can be marked for destruction. */
    array<Entity*> destroy_list;
    bool do_reserve = true;
    for (int g = 0; g < Entity::ALLGROUP_END; ++g)
    {
        for (int i = data->m_list[g].count(); i--;)
        {
            Entity *e = data->m_list[g][i];

            if (e->m_destroy && g < Entity::GAMEGROUP_END)
            {
                /* Game tick list:
                * If entity is to be destroyed, remove it */
                data->m_list[g].remove_swap(i);
                if (do_reserve)
                {
                    do_reserve = false;
                    destroy_list.reserve(data->nentities); //Should it be less ?
                }
                destroy_list.push_unique(e);
            }
            else if (e->m_destroy)
            {
                /* Draw tick list:
                * If entity is to be destroyed,
                * remove it and store it. */
                data->m_list[g].remove_swap(i);
                int removal_count = 0;
                for (int j = 0; j < Scene::GetCount(); j++)
                {
                    //If entity is concerned by this scene, add it in the list
                    if (Scene::GetScene(j).IsRelevant(e))
                        removal_count++;
                    //Update scene index
                    data->m_scenes[e->m_drawgroup][j] -= removal_count;
                }
                if (do_reserve)
                {
                    do_reserve = false;
                    destroy_list.reserve(data->nentities); //Should it be less ?
                }
                destroy_list.push_unique(e);
            }
            else
            {
                if (e->m_ref <= 0 && g >= Entity::DRAWGROUP_BEGIN)
                    e->m_destroy = 1;
            }
        }
    }
    if (!!destroy_list.count())
    {
        data->nentities -= destroy_list.count();
        for (Entity* e : destroy_list)
            delete e;
    }

    /* Insert waiting objects into the appropriate lists */
    while (data->m_todolist.count())
    {
        Entity *e = data->m_todolist.last();

        //If the entity has no mask, default it
        if (e->m_scene_mask == 0)
        {
            Scene::GetScene().Link(e);
        }

        data->m_todolist.remove(-1);
        data->m_list[e->m_gamegroup].push(e);
        if (e->m_drawgroup != Entity::DRAWGROUP_NONE)
        {
            if (data->m_scenes[e->m_drawgroup].count() < Scene::GetCount())
                data->m_scenes[e->m_drawgroup].resize(Scene::GetCount());

            int added_count = 0;
            for (int i = 0; i < Scene::GetCount(); i++)
            {
                //If entity is concerned by this scene, add it in the list
                if (Scene::GetScene(i).IsRelevant(e))
                {
                    data->m_list[e->m_drawgroup].insert(e, data->m_scenes[e->m_drawgroup][i]);
                    added_count++;
                }
                //Update scene index
                data->m_scenes[e->m_drawgroup][i] += added_count;
            }
        }

        // Initialize the entity
        e->InitGame();
    }

    /* Tick objects for the game loop */
    for (int g = Entity::GAMEGROUP_BEGIN; g < Entity::GAMEGROUP_END && !data->quit /* Stop as soon as required */; ++g)
    {
        for (int i = 0; i < data->m_list[g].count() && !data->quit /* Stop as soon as required */; ++i)
        {
            Entity *e = data->m_list[g][i];
            if (!e->m_destroy)
            {
#if !LOL_BUILD_RELEASE
                if (e->m_tickstate != Entity::STATE_IDLE)
                    msg::error("entity %s [%p] not idle for game tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_PRETICK_GAME;
#endif
                e->TickGame(data->deltatime);
#if !LOL_BUILD_RELEASE
                if (e->m_tickstate != Entity::STATE_POSTTICK_GAME)
                    msg::error("entity %s [%p] missed super game tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_IDLE;
#endif
            }
        }
    }

    Profiler::Stop(Profiler::STAT_TICK_GAME);
}

//-----------------------------------------------------------------------------
void TickerData::DrawThreadTick()
{
    Profiler::Start(Profiler::STAT_TICK_DRAW);

    /* Tick objects for the draw loop */
    for (int g = Entity::DRAWGROUP_BEGIN; g < Entity::DRAWGROUP_END && !data->quit /* Stop as soon as required */; ++g)
    {
        switch (g)
        {
        case Entity::DRAWGROUP_BEGIN:
            for (int i = 0; i < Scene::GetCount(); i++)
                Scene::GetScene(i).Reset();
            break;
        default:
            break;
        }

        int scene_idx = 0;
        for (int i = 0; i < data->m_list[g].count() && !data->quit /* Stop as soon as required */; ++i)
        {
            //We're outside of the range of the current scene, on to the next
            if (i >= data->m_scenes[g][scene_idx])
                ++scene_idx;

            Entity *e = data->m_list[g][i];

            if (!e->m_destroy)
            {
#if !LOL_BUILD_RELEASE
                if (e->m_tickstate != Entity::STATE_IDLE)
                    msg::error("entity %s [%p] not idle for draw tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_PRETICK_DRAW;
#endif
                e->TickDraw(data->deltatime, Scene::GetScene(scene_idx));
#if !LOL_BUILD_RELEASE
                if (e->m_tickstate != Entity::STATE_POSTTICK_DRAW)
                    msg::error("entity %s [%p] missed super draw tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_IDLE;
#endif
            }
        }

    }

    //Do the scene render loop
    for (int idx = 0; idx < Scene::GetCount() && !data->quit /* Stop as soon as required */; ++idx)
    {
        Scene& scene = Scene::GetScene(idx);

        /* Enable display */
        scene.EnableDisplay();

        Renderer::Get(idx)->Clear(ClearMask::All);

        /* Do the render step */
        scene.RenderPrimitives();
        scene.RenderTiles();
        scene.RenderLines(data->deltatime);

        /* Disable display */
        scene.DisableDisplay();

        //break;
    }

    Profiler::Stop(Profiler::STAT_TICK_DRAW);
}

void TickerData::DiskThreadTick()
{
    ;
}

void Ticker::SetState(Entity * /* entity */, uint32_t /* state */)
{

}

void Ticker::SetStateWhenMatch(Entity * /* entity */, uint32_t /* state */,
                               Entity * /* other_entity */, uint32_t /* other_state */)
{

}

//-----------------------------------------------------------------------------
void Ticker::Setup(float fps)
{
    data->fps = fps;

#if LOL_FEATURE_THREADS
    data->gamethread = new thread(std::bind(&TickerData::GameThreadMain, data));
    data->gametick.push(1);

    data->diskthread = new thread(std::bind(&TickerData::DiskThreadMain, data));
#endif
}

void Ticker::TickDraw()
{
#if LOL_FEATURE_THREADS
    data->drawtick.pop();
#else
    TickerData::GameThreadTick();
#endif

    TickerData::DrawThreadTick();

    Profiler::Start(Profiler::STAT_TICK_BLIT);

    /* Signal game thread that it can carry on */
#if LOL_FEATURE_THREADS
    data->gametick.push(1);
#else
    TickerData::DiskThreadTick();
#endif

    /* Clamp FPS */
    Profiler::Stop(Profiler::STAT_TICK_BLIT);

#if !EMSCRIPTEN
    /* If framerate is fixed, force wait time to 1/FPS. Otherwise, set wait
     * time to 0. */
    float frametime = data->fps ? 1.f / data->fps : 0.f;

    if (frametime > data->bias + .2f)
        frametime = data->bias + .2f; /* Don't go below 5 fps */
    if (frametime > data->bias)
        data->timer.Wait(frametime - data->bias);

    /* If recording, do not try to compensate for lag. */
    if (!data->recording)
        data->bias -= frametime;
#endif
}

void Ticker::StartRecording()
{
    data->recording++;
}

void Ticker::StopRecording()
{
    data->recording--;
}

int Ticker::GetFrameNum()
{
    return data->frame;
}

void Ticker::Shutdown()
{
    /* We're bailing out. Release all m_autorelease objects. */
    while (data->m_autolist.count())
    {
        data->m_autolist.last()->m_ref--;
        data->m_autolist.remove(-1);
    }

    data->quit = 1;
    data->quitframe = data->frame;
}

int Ticker::Finished()
{
    return !data->nentities;
}

} /* namespace lol */

