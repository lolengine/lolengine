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
#include <cstdio>
#include <stdint.h>

#include "core.h"

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
        frame(0), deltams(0), bias(0)
    {
        for (int i = 0; i < Entity::ALLGROUP_END; i++)
            list[i] = NULL;
    }

    ~TickerData()
    {
#if !FINAL_RELEASE
        if (nentities)
            fprintf(stderr, "ERROR: still %i entities in ticker\n", nentities);
        if (autolist)
        {
            int count = 0;
            for (Entity *e = autolist; e; e = e->autonext, count++)
                ;
            fprintf(stderr, "ERROR: still %i autoreleased entities\n", count);
        }
        fprintf(stderr, "INFO: %i frames required to quit\n",
                frame - quitframe);
#endif
    }

private:
    /* Entity management */
    Entity *todolist, *autolist;
    Entity *list[Entity::ALLGROUP_END];
    int nentities;

    /* Fixed framerate management */
    int frame, quitframe;
    Timer timer;
    float deltams, bias;
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
#if !FINAL_RELEASE
    if (entity->destroy)
        fprintf(stderr, "ERROR: refing entity scheduled for destruction\n");
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
#if !FINAL_RELEASE
    if (entity->ref <= 0)
        fprintf(stderr, "ERROR: dereferencing unreferenced entity\n");
    if (entity->autorelease)
        fprintf(stderr, "ERROR: dereferencing autoreleased entity\n");
#endif
    return --entity->ref;
}

void Ticker::TickGame()
{
    Profiler::Stop(Profiler::STAT_TICK_FRAME);
    Profiler::Start(Profiler::STAT_TICK_FRAME);

    Profiler::Start(Profiler::STAT_TICK_GAME);

#if 0
    fprintf(stderr, "-------------------------------------\n");
    for (int i = 0; i < Entity::ALLGROUP_END; i++)
    {
        fprintf(stderr, "%s Group %i\n",
                (i < Entity::GAMEGROUP_END) ? "Game" : "Draw", i);

        for (Entity *e = data->list[i]; e; )
        {
            fprintf(stderr, "  \\-- %s (ref %i, destroy %i)\n", e->GetName(), e->ref, e->destroy);
            e = (i < Entity::GAMEGROUP_END) ? e->gamenext : e->drawnext;
        }
    }
#endif

    data->frame++;

    data->deltams = data->timer.GetMs();
    data->bias += data->deltams;

    /* Garbage collect objects that can be destroyed. We can do this
     * before inserting awaiting objects, because only objects already in
     * the tick lists can be marked for destruction. */
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
#if !FINAL_RELEASE
                if (e->state != Entity::STATE_IDLE)
                    fprintf(stderr, "ERROR: entity not idle for game tick\n");
                e->state = Entity::STATE_PRETICK_GAME;
#endif
                e->TickGame(data->deltams);
#if !FINAL_RELEASE
                if (e->state != Entity::STATE_POSTTICK_GAME)
                    fprintf(stderr, "ERROR: entity missed super game tick\n");
                e->state = Entity::STATE_IDLE;
#endif
            }

    Profiler::Stop(Profiler::STAT_TICK_GAME);
}

void Ticker::TickDraw()
{
    Profiler::Start(Profiler::STAT_TICK_DRAW);

    /* Tick objects for the draw loop */
    for (int i = Entity::DRAWGROUP_BEGIN; i < Entity::DRAWGROUP_END; i++)
    {
        switch (i)
        {
        case Entity::DRAWGROUP_HUD:
            Video::SetDepth(false);
            break;
        default:
            Video::SetDepth(true);
            break;
        }

        for (Entity *e = data->list[i]; e; e = e->drawnext)
            if (!e->destroy)
            {
#if !FINAL_RELEASE
                if (e->state != Entity::STATE_IDLE)
                    fprintf(stderr, "ERROR: entity not idle for draw tick\n");
                e->state = Entity::STATE_PRETICK_DRAW;
#endif
                e->TickDraw(data->deltams);
#if !FINAL_RELEASE
                if (e->state != Entity::STATE_POSTTICK_DRAW)
                    fprintf(stderr, "ERROR: entity missed super draw tick\n");
                e->state = Entity::STATE_IDLE;
#endif
            }
    }

    Profiler::Stop(Profiler::STAT_TICK_DRAW);
    Profiler::Start(Profiler::STAT_TICK_BLIT);
}

void Ticker::ClampFps(float deltams)
{
    Profiler::Stop(Profiler::STAT_TICK_BLIT);

    if (deltams > data->bias + 200.0f)
        deltams = data->bias + 200.0f; // Don't go below 5 fps
    if (deltams > data->bias)
        data->timer.WaitMs(deltams - data->bias);
    data->bias -= deltams;
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

    data->quitframe = data->frame;
}

int Ticker::Finished()
{
    return !data->nentities;
}

