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
        todolist(0), autolist(0),
        nentities(0),
        frame(0), recording(0), deltatime(0), bias(0), fps(0),
#if LOL_DEBUG
        keepalive(0),
#endif
        quit(0), quitframe(0), quitdelay(20), panic(0)
    {
        for (int i = 0; i < Entity::ALLGROUP_END; i++)
            list[i] = NULL;
    }

    ~TickerData()
    {
        ASSERT(nentities == 0,
               "still %i entities in ticker\n", nentities);
#if !LOL_RELEASE
        if (autolist)
        {
            int count = 0;
            for (Entity *e = autolist; e; e = e->m_autonext, count++)
                ;
            ASSERT(count == 0, "still %i autoreleased entities\n", count);
        }
#endif
        Log::Debug("%i frames required to quit\n",
                frame - quitframe);

        gametick.Push(0);
        disktick.Push(0);
        delete gamethread;
        delete diskthread;
    }

private:
    /* Entity management */
    Entity *todolist, *autolist;
    Entity *list[Entity::ALLGROUP_END];
    int nentities;

    /* Fixed framerate management */
    int frame, recording;
    Timer timer;
    float deltatime, bias, fps;
#if LOL_DEBUG
    float keepalive;
#endif

    /* Background threads */
    static void *GameThreadMain(void *p);
    static void *DrawThreadMain(void *p); /* unused */
    static void *DiskThreadMain(void *p);
    Thread *gamethread, *drawthread, *diskthread;
    Queue<int> gametick, drawtick, disktick;

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
    entity->m_gamenext = data->todolist;
    data->todolist = entity;
    /* Objects are autoreleased by default. Put them in a circular list. */
    entity->m_autorelease = 1;
    entity->m_autonext = data->autolist;
    data->autolist = entity;
    entity->m_ref = 1;

    data->nentities++;
}

void Ticker::Ref(Entity *entity)
{
    ASSERT(entity, "dereferencing NULL entity\n");
    ASSERT(!entity->m_destroy,
           "referencing entity scheduled for destruction %s\n",
           entity->GetName());

    if (entity->m_autorelease)
    {
        /* Get the entity out of the m_autorelease list. This is usually
         * very fast since the first entry in autolist is the last
         * registered entity. */
        for (Entity *e = data->autolist, *prev = NULL; e;
             prev = e, e = e->m_autonext)
        {
            if (e == entity)
            {
                (prev ? prev->m_autonext : data->autolist) = e->m_autonext;
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
    ASSERT(entity, "dereferencing NULL entity\n");
    ASSERT(entity->m_ref > 0, "dereferencing unreferenced entity %s\n",
           entity->GetName())
    ASSERT(!entity->m_autorelease, "dereferencing autoreleased entity %s\n",
           entity->GetName())

    return --entity->m_ref;
}

void *TickerData::GameThreadMain(void * /* p */)
{
#if LOL_DEBUG
    Log::Info("ticker game thread initialised\n");
#endif

    for (;;)
    {
        int tick = data->gametick.Pop();
        if (!tick)
            break;

        Profiler::Stop(Profiler::STAT_TICK_FRAME);
        Profiler::Start(Profiler::STAT_TICK_FRAME);

        Profiler::Start(Profiler::STAT_TICK_GAME);

#if 0
        Log::Debug("-------------------------------------\n");
        for (int i = 0; i < Entity::ALLGROUP_END; i++)
        {
            Log::Debug("%s Group %i\n",
                       (i < Entity::GAMEGROUP_END) ? "Game" : "Draw", i);

            for (Entity *e = data->list[i]; e; )
            {
                Log::Debug("  \\-- %s (m_ref %i, destroy %i)\n", e->GetName(), e->m_ref, e->m_destroy);
                e = (i < Entity::GAMEGROUP_END) ? e->m_gamenext : e->m_drawnext;
            }
        }
#endif

        data->frame++;

        /* Ensure some randomness */
        (void)std::rand();

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

#if LOL_DEBUG
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

            for (int i = 0; i < Entity::ALLGROUP_END && n < data->panic; i++)
            for (Entity *e = data->list[i]; e && n < data->panic; e = e->m_gamenext)
                if (e->m_ref)
                {
#if !LOL_RELEASE
                    Log::Error("poking %s\n", e->GetName());
#endif
                    e->m_ref--;
                    n++;
                }

#if !LOL_RELEASE
            if (n)
                Log::Error("%i entities stuck after %i frames, poked %i\n",
                           data->nentities, data->quitdelay, n);
#endif

            data->quitdelay = data->quitdelay > 1 ? data->quitdelay / 2 : 1;
        }

        /* Garbage collect objects that can be destroyed. We can do this
         * before inserting awaiting objects, because only objects already
         * inthe tick lists can be marked for destruction. */
        for (int i = 0; i < Entity::ALLGROUP_END; i++)
            for (Entity *e = data->list[i], *prev = NULL; e; )
            {
                if (e->m_destroy && i < Entity::GAMEGROUP_END)
                {
                    /* If entity is to be destroyed, remove it from the
                     * game tick list. */
                    (prev ? prev->m_gamenext : data->list[i]) = e->m_gamenext;

                    e = e->m_gamenext;
                }
                else if (e->m_destroy)
                {
                    /* If entity is to be destroyed, remove it from the
                     * draw tick list and destroy it. */
                    (prev ? prev->m_drawnext : data->list[i]) = e->m_drawnext;

                    Entity *tmp = e;
                    e = e->m_drawnext; /* Can only be in a draw group list */
                    delete tmp;

                    data->nentities--;
                }
                else
                {
                    if (e->m_ref <= 0 && i >= Entity::DRAWGROUP_BEGIN)
                        e->m_destroy = 1;
                    prev = e;
                    e = (i < Entity::GAMEGROUP_END) ? e->m_gamenext : e->m_drawnext;
                }
            }

        /* Insert waiting objects into the appropriate lists */
        while (data->todolist)
        {
            Entity *e = data->todolist;
            data->todolist = e->m_gamenext;

            e->m_gamenext = data->list[e->m_gamegroup];
            data->list[e->m_gamegroup] = e;
            e->m_drawnext = data->list[e->m_drawgroup];
            data->list[e->m_drawgroup] = e;
        }

        /* Tick objects for the game loop */
        for (int i = Entity::GAMEGROUP_BEGIN; i < Entity::GAMEGROUP_END; i++)
            for (Entity *e = data->list[i]; e; e = e->m_gamenext)
                if (!e->m_destroy)
                {
#if !LOL_RELEASE
                    if (e->m_tickstate != Entity::STATE_IDLE)
                        Log::Error("entity %s [%p] not idle for game tick\n",
                                   e->GetName(), e);
                    e->m_tickstate = Entity::STATE_PRETICK_GAME;
#endif
                    e->TickGame(data->deltatime);
#if !LOL_RELEASE
                    if (e->m_tickstate != Entity::STATE_POSTTICK_GAME)
                        Log::Error("entity %s [%p] missed super game tick\n",
                                   e->GetName(), e);
                    e->m_tickstate = Entity::STATE_IDLE;
#endif
                }

        Profiler::Stop(Profiler::STAT_TICK_GAME);

        data->drawtick.Push(1);
    }

    data->drawtick.Push(0);

#if LOL_DEBUG
    Log::Info("ticker game thread terminated\n");
#endif

    return NULL;
}

void *TickerData::DrawThreadMain(void * /* p */)
{
    for (;;)
    {
        int tick = data->drawtick.Pop();
        if (!tick)
            break;

        data->gametick.Push(1);
    }

    return NULL;
}

void *TickerData::DiskThreadMain(void * /* p */)
{
    /* FIXME: temporary hack to avoid crashes on the PS3 */
    data->disktick.Pop();

    return NULL;
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

    data->gamethread = new Thread(TickerData::GameThreadMain, NULL);
    data->gametick.Push(1);

    data->diskthread = new Thread(TickerData::DiskThreadMain, NULL);
}

void Ticker::TickDraw()
{
    data->drawtick.Pop();

    Profiler::Start(Profiler::STAT_TICK_DRAW);

    /* Tick objects for the draw loop */
    for (int i = Entity::DRAWGROUP_BEGIN; i < Entity::DRAWGROUP_END; i++)
    {
        switch (i)
        {
        case Entity::DRAWGROUP_BEGIN:
            Scene::GetDefault()->Reset();
            Video::Clear(ClearMask::All);
            break;
        case Entity::DRAWGROUP_HUD:
            Video::SetDepth(false);
            break;
        default:
            Video::SetDepth(true);
            break;
        }

        for (Entity *e = data->list[i]; e; e = e->m_drawnext)
            if (!e->m_destroy)
            {
#if !LOL_RELEASE
                if (e->m_tickstate != Entity::STATE_IDLE)
                    Log::Error("entity %s [%p] not idle for draw tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_PRETICK_DRAW;
#endif
                e->TickDraw(data->deltatime);
#if !LOL_RELEASE
                if (e->m_tickstate != Entity::STATE_POSTTICK_DRAW)
                    Log::Error("entity %s [%p] missed super draw tick\n",
                               e->GetName(), e);
                e->m_tickstate = Entity::STATE_IDLE;
#endif
            }

        /* Do this render step */
        Scene::GetDefault()->Render();
    }

    Profiler::Stop(Profiler::STAT_TICK_DRAW);
    Profiler::Start(Profiler::STAT_TICK_BLIT);

    /* Signal game thread that it can carry on */
    data->gametick.Push(1);

    /* Clamp FPS */
    Profiler::Stop(Profiler::STAT_TICK_BLIT);

    /* If framerate is fixed, force wait time to 1/FPS. Otherwise, set wait
     * time to 0. */
    float frametime = data->fps ? 1.f / data->fps : 0.f;

    if (frametime > data->bias + .2f)
        frametime = data->bias + .2f; // Don't go below 5 fps
    if (frametime > data->bias)
        data->timer.Wait(frametime - data->bias);

    /* If recording, do not try to compensate for lag. */
    if (!data->recording)
        data->bias -= frametime;
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
    while (data->autolist)
    {
        data->autolist->m_ref--;
        data->autolist = data->autolist->m_autonext;
    }

    data->quit = 1;
    data->quitframe = data->frame;
}

int Ticker::Finished()
{
    return !data->nentities;
}

} /* namespace lol */

