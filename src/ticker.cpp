//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
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
        frame(0), recording(0), deltams(0), bias(0), fps(0),
        quit(0), quitframe(0), quitdelay(20), panic(0)
    {
        for (int i = 0; i < Entity::ALLGROUP_END; i++)
            list[i] = NULL;
    }

    ~TickerData()
    {
#if !LOL_RELEASE
        if (nentities)
            Log::Error("still %i entities in ticker\n", nentities);
        if (autolist)
        {
            int count = 0;
            for (Entity *e = autolist; e; e = e->autonext, count++)
                ;
            Log::Error("still %i autoreleased entities\n", count);
        }
        Log::Debug("%i frames required to quit\n",
                frame - quitframe);
#endif
        gametick.Push(0);
        delete gamethread;
    }

private:
    /* Entity management */
    Entity *todolist, *autolist;
    Entity *list[Entity::ALLGROUP_END];
    int nentities;

    /* Fixed framerate management */
    int frame, recording;
    Timer timer;
    float deltams, bias, fps;

    /* Background threads */
    static void *GameThreadMain(void *p);
    static void *DrawThreadMain(void *p); /* unused */
    Thread *gamethread, *drawthread;
    Queue gametick, drawtick;

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
    entity->gamenext = data->todolist;
    data->todolist = entity;
    /* Objects are autoreleased by default. Put them in a circular list. */
    entity->autorelease = 1;
    entity->autonext = data->autolist;
    data->autolist = entity;
    entity->ref = 1;

    data->nentities++;
}

void Ticker::Ref(Entity *entity)
{
#if !LOL_RELEASE
    if (!entity)
    {
        Log::Error("referencing NULL entity\n");
        return;
    }
    if (entity->destroy)
        Log::Error("referencing entity scheduled for destruction\n");
#endif
    if (entity->autorelease)
    {
        /* Get the entity out of the autorelease list. This is usually
         * very fast since the first entry in autolist is the last
         * registered entity. */
        for (Entity *e = data->autolist, *prev = NULL; e;
             prev = e, e = e->autonext)
        {
            if (e == entity)
            {
                (prev ? prev->autonext : data->autolist) = e->autonext;
                break;
            }
        }
        entity->autorelease = 0;
    }
    else
        entity->ref++;
}

int Ticker::Unref(Entity *entity)
{
#if !LOL_RELEASE
    if (!entity)
    {
        Log::Error("dereferencing NULL entity\n");
        return 0;
    }
    if (entity->ref <= 0)
        Log::Error("dereferencing unreferenced entity\n");
    if (entity->autorelease)
        Log::Error("dereferencing autoreleased entity\n");
#endif
    return --entity->ref;
}

void *TickerData::GameThreadMain(void *p)
{
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
                Log::Debug("  \\-- %s (ref %i, destroy %i)\n", e->GetName(), e->ref, e->destroy);
                e = (i < Entity::GAMEGROUP_END) ? e->gamenext : e->drawnext;
            }
        }
#endif

        data->frame++;

        /* If recording with fixed framerate, set deltams to a fixed value */
        if (data->recording && data->fps)
        {
            data->deltams = 1000.0f / data->fps;
        }
        else
        {
            data->deltams = data->timer.GetMs();
            data->bias += data->deltams;
        }

        /* If shutdown is stuck, kick the first entity we meet and see
         * whether it makes things better. Note that it is always a bug to
         * have referenced entities after 20 frames, but at least this
         * safeguard makes it possible to exit the program cleanly. */
        if (data->quit && !((data->frame - data->quitframe) % data->quitdelay))
        {
            int n = 0;
            data->panic = 2 * (data->panic + 1);

            for (int i = 0; i < Entity::ALLGROUP_END && n < data->panic; i++)
            for (Entity *e = data->list[i]; e && n < data->panic; e = e->gamenext)
                if (e->ref)
                {
#if !LOL_RELEASE
                    Log::Error("poking %s\n", e->GetName());
#endif
                    e->ref--;
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
                if (e->destroy && i < Entity::GAMEGROUP_END)
                {
                    /* If entity is to be destroyed, remove it from the
                     * game tick list. */
                    (prev ? prev->gamenext : data->list[i]) = e->gamenext;

                    e = e->gamenext;
                }
                else if (e->destroy)
                {
                    /* If entity is to be destroyed, remove it from the
                     * draw tick list and destroy it. */
                    (prev ? prev->drawnext : data->list[i]) = e->drawnext;

                    Entity *tmp = e;
                    e = e->drawnext; /* Can only be in a draw group list */
                    delete tmp;

                    data->nentities--;
                }
                else
                {
                    if (e->ref <= 0 && i >= Entity::DRAWGROUP_BEGIN)
                        e->destroy = 1;
                    prev = e;
                    e = (i < Entity::GAMEGROUP_END) ? e->gamenext : e->drawnext;
                }
            }

        /* Insert waiting objects into the appropriate lists */
        while (data->todolist)
        {
            Entity *e = data->todolist;
            data->todolist = e->gamenext;

            e->gamenext = data->list[e->gamegroup];
            data->list[e->gamegroup] = e;
            e->drawnext = data->list[e->drawgroup];
            data->list[e->drawgroup] = e;
        }

        /* Tick objects for the game loop */
        for (int i = Entity::GAMEGROUP_BEGIN; i < Entity::GAMEGROUP_END; i++)
            for (Entity *e = data->list[i]; e; e = e->gamenext)
                if (!e->destroy)
                {
#if !LOL_RELEASE
                    if (e->state != Entity::STATE_IDLE)
                        Log::Error("entity not idle for game tick\n");
                    e->state = Entity::STATE_PRETICK_GAME;
#endif
                    e->TickGame(data->deltams);
#if !LOL_RELEASE
                    if (e->state != Entity::STATE_POSTTICK_GAME)
                        Log::Error("entity missed super game tick\n");
                    e->state = Entity::STATE_IDLE;
#endif
                }

        Profiler::Stop(Profiler::STAT_TICK_GAME);

        data->drawtick.Push(1);
    }

    data->drawtick.Push(0);

    return NULL;
}

void *TickerData::DrawThreadMain(void *p)
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

void Ticker::SetState(Entity *entity, uint32_t state)
{

}

void Ticker::SetStateWhenMatch(Entity *entity, uint32_t state,
                               Entity *other_entity, uint32_t other_state)
{

}

void Ticker::Setup(float fps)
{
    data->fps = fps;

    data->gamethread = new Thread(TickerData::GameThreadMain, NULL);
    data->gametick.Push(1);
}

void Ticker::TickDraw()
{
    data->drawtick.Pop();

    Profiler::Start(Profiler::STAT_TICK_DRAW);

    Video::Clear();

    Scene::GetDefault();

    /* Tick objects for the draw loop */
    for (int i = Entity::DRAWGROUP_BEGIN; i < Entity::DRAWGROUP_END; i++)
    {
        switch (i)
        {
        case Entity::DRAWGROUP_HUD:
            Scene::GetDefault()->Render();
            Video::SetDepth(false);
            break;
        default:
            Video::SetDepth(true);
            break;
        }

        for (Entity *e = data->list[i]; e; e = e->drawnext)
            if (!e->destroy)
            {
#if !LOL_RELEASE
                if (e->state != Entity::STATE_IDLE)
                    Log::Error("entity not idle for draw tick\n");
                e->state = Entity::STATE_PRETICK_DRAW;
#endif
                e->TickDraw(data->deltams);
#if !LOL_RELEASE
                if (e->state != Entity::STATE_POSTTICK_DRAW)
                    Log::Error("entity missed super draw tick\n");
                e->state = Entity::STATE_IDLE;
#endif
            }
    }

    Scene::GetDefault()->Render();
    Scene::Reset();

    Profiler::Stop(Profiler::STAT_TICK_DRAW);
    Profiler::Start(Profiler::STAT_TICK_BLIT);

    /* Signal game thread that it can carry on */
    data->gametick.Push(1);

    /* Clamp FPS */
    Profiler::Stop(Profiler::STAT_TICK_BLIT);

    /* If framerate is fixed, force wait time to 1/FPS. Otherwise, set wait
     * time to 0. */
    float framems = data->fps ? 1000.0f / data->fps : 0.0f;

    if (framems > data->bias + 200.0f)
        framems = data->bias + 200.0f; // Don't go below 5 fps
    if (framems > data->bias)
        data->timer.WaitMs(framems - data->bias);

    /* If recording, do not try to compensate for lag. */
    if (!data->recording)
        data->bias -= framems;
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
    /* We're bailing out. Release all autorelease objects. */
    while (data->autolist)
    {
        data->autolist->ref--;
        data->autolist = data->autolist->autonext;
    }

    data->quit = 1;
    data->quitframe = data->frame;
}

int Ticker::Finished()
{
    return !data->nentities;
}

} /* namespace lol */

