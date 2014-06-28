//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <stdint.h>

#include "core.h"

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
               "still %i entities in ticker\n", nentities);
        ASSERT(m_autolist.Count() == 0,
               "still %i autoreleased entities\n", m_autolist.Count());
        Log::Debug("%i frames required to quit\n", frame - quitframe);

#if LOL_FEATURE_THREADS
        gametick.Push(0);
        disktick.Push(0);
        delete gamethread;
        delete diskthread;
#endif
    }

private:
    /* Entity management */
    array<Entity *> m_todolist, m_autolist;
    array<Entity *> m_list[Entity::ALLGROUP_END];
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
    static void *GameThreadMain(void *p);
    static void *DrawThreadMain(void *p); /* unused */
    static void *DiskThreadMain(void *p);
    Thread *gamethread, *drawthread, *diskthread;
    Queue<int> gametick, drawtick, disktick;
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
    data->m_todolist.Push(entity);

    /* Objects are autoreleased by default. Put them in a list. */
    data->m_autolist.Push(entity);
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
        for (int i = data->m_autolist.Count(); i--; )
        {
            if (data->m_autolist[i] == entity)
            {
                data->m_autolist.RemoveSwap(i);
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
void *TickerData::GameThreadMain(void * /* p */)
{
#if LOL_BUILD_DEBUG
    Log::Info("ticker game thread initialised\n");
#endif

    for (;;)
    {
        int tick = data->gametick.Pop();
        if (!tick)
            break;

        GameThreadTick();

        data->drawtick.Push(1);
    }

    data->drawtick.Push(0);

#if LOL_BUILD_DEBUG
    Log::Info("ticker game thread terminated\n");
#endif

    return nullptr;
}
#endif /* LOL_FEATURE_THREADS */

#if LOL_FEATURE_THREADS
void *TickerData::DrawThreadMain(void * /* p */)
{
#if LOL_BUILD_DEBUG
    Log::Info("ticker draw thread initialised\n");
#endif

    for (;;)
    {
        int tick = data->drawtick.Pop();
        if (!tick)
            break;

        DrawThreadTick();

        data->gametick.Push(1);
    }

#if LOL_BUILD_DEBUG
    Log::Info("ticker draw thread terminated\n");
#endif

    return nullptr;
}
#endif /* LOL_FEATURE_THREADS */

#if LOL_FEATURE_THREADS
void *TickerData::DiskThreadMain(void * /* p */)
{
    /* FIXME: temporary hack to avoid crashes on the PS3 */
    data->disktick.Pop();

    return nullptr;
}
#endif /* LOL_FEATURE_THREADS */

void TickerData::GameThreadTick()
{
    Profiler::Stop(Profiler::STAT_TICK_FRAME);
    Profiler::Start(Profiler::STAT_TICK_FRAME);

    Profiler::Start(Profiler::STAT_TICK_GAME);

#if 0
    Log::Debug("-------------------------------------\n");
    for (int g = 0; g < Entity::ALLGROUP_END; ++g)
    {
        Log::Debug("%s Group %i\n",
                   (g < Entity::GAMEGROUP_END) ? "Game" : "Draw", g);

        for (int i = 0; i < data->m_list[g].Count(); ++i)
        {
            Entity *e = data->m_list[g][i];
            Log::Debug("  \\-- [%p] %s (m_ref %i, destroy %i)\n",
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
        Log::Info("ticker keepalive: tick!\n");
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
        for (int i = 0; i < data->m_list[g].Count() && n < data->panic; ++i)
        {
            Entity * e = data->m_list[g][i];
            if (e->m_ref)
            {
#if !LOL_BUILD_RELEASE
                Log::Error("poking %s\n", e->GetName());
#endif
                e->m_ref--;
                n++;
            }
        }

#if !LOL_BUILD_RELEASE
        if (n)
            Log::Error("%i entities stuck after %i frames, poked %i\n",
                       data->nentities, data->quitdelay, n);
#endif

        data->quitdelay = data->quitdelay > 1 ? data->quitdelay / 2 : 1;
    }

    /* Garbage collect objects that can be destroyed. We can do this
     * before inserting awaiting objects, because only objects already
     * in the tick lists can be marked for destruction. */
    for (int g = 0; g < Entity::ALLGROUP_END; ++g)
    {
        for (int i = data->m_list[g].Count(); i--; )
        {
            Entity *e = data->m_list[g][i];

            if (e->m_destroy && g < Entity::GAMEGROUP_END)
            {
                /* If entity is to be destroyed, remove it from the
                 * game tick list. */
                data->m_list[g].RemoveSwap(i);
            }
            else if (e->m_destroy)
            {
                /* If entity is to be destroyed, remove it from the
                 * draw tick list and destroy it. */
                data->m_list[g].RemoveSwap(i);
                delete e;

                data->nentities--;
            }
            else
            {
                if (e->m_ref <= 0 && g >= Entity::DRAWGROUP_BEGIN)
                    e->m_destroy = 1;
            }
        }
    }

    /* Insert waiting objects into the appropriate lists */
    while (data->m_todolist.Count())
    {
        Entity *e = data->m_todolist.Last();

        data->m_todolist.Remove(-1);
        data->m_list[e->m_gamegroup].Push(e);
        data->m_list[e->m_drawgroup].Push(e);

        // Initialize the entity
        e->InitGame();
    }

    /* Tick objects for the game loop */
    for (int g = Entity::GAMEGROUP_BEGIN; g < Entity::GAMEGROUP_END; ++g)
        for (int i = 0; i < data->m_list[g].Count(); ++i)
        {
            Entity *e = data->m_list[g][i];

            if (!e->m_destroy)
            {
#if !LOL_BUILD_RELEASE
                if (e->m_tickstate != Entity::STATE_IDLE)
                    Log::Error("entity %s [%p] not idle for game tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_PRETICK_GAME;
#endif
                e->TickGame(data->deltatime);
#if !LOL_BUILD_RELEASE
                if (e->m_tickstate != Entity::STATE_POSTTICK_GAME)
                    Log::Error("entity %s [%p] missed super game tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_IDLE;
#endif
            }
        }

    Profiler::Stop(Profiler::STAT_TICK_GAME);
}

void TickerData::DrawThreadTick()
{
    Profiler::Start(Profiler::STAT_TICK_DRAW);

    /* Tick objects for the draw loop */
    for (int g = Entity::DRAWGROUP_BEGIN; g < Entity::DRAWGROUP_END; ++g)
    {
        switch (g)
        {
        case Entity::DRAWGROUP_BEGIN:
            g_scene->Reset();
            g_renderer->Clear(ClearMask::All);
            break;
        default:
            break;
        }

        for (int i = 0; i < data->m_list[g].Count(); ++i)
        {
            Entity *e = data->m_list[g][i];

            if (!e->m_destroy)
            {
#if !LOL_BUILD_RELEASE
                if (e->m_tickstate != Entity::STATE_IDLE)
                    Log::Error("entity %s [%p] not idle for draw tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_PRETICK_DRAW;
#endif
                e->TickDraw(data->deltatime, *g_scene);
#if !LOL_BUILD_RELEASE
                if (e->m_tickstate != Entity::STATE_POSTTICK_DRAW)
                    Log::Error("entity %s [%p] missed super draw tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_IDLE;
#endif
            }
        }

        /* Do this render step */
        g_scene->RenderPrimitives();
        g_scene->RenderTiles();
        g_scene->RenderLines(data->deltatime);
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

void Ticker::Setup(float fps)
{
    data->fps = fps;

#if LOL_FEATURE_THREADS
    data->gamethread = new Thread(TickerData::GameThreadMain, nullptr);
    data->gametick.Push(1);

    data->diskthread = new Thread(TickerData::DiskThreadMain, nullptr);
#endif
}

void Ticker::TickDraw()
{
#if LOL_FEATURE_THREADS
    data->drawtick.Pop();
#else
    TickerData::GameThreadTick();
#endif

    TickerData::DrawThreadTick();

    Profiler::Start(Profiler::STAT_TICK_BLIT);

    /* Signal game thread that it can carry on */
#if LOL_FEATURE_THREADS
    data->gametick.Push(1);
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
    while (data->m_autolist.Count())
    {
        data->m_autolist.Last()->m_ref--;
        data->m_autolist.Remove(-1);
    }

    data->quit = 1;
    data->quitframe = data->frame;
}

int Ticker::Finished()
{
    return !data->nentities;
}

} /* namespace lol */

