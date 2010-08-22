//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
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
        todo(0), nentities(0),
        frame(0), deltams(0), bias(0)
    {
        for (int i = 0; i < Entity::GROUP_COUNT; i++)
            list[i] = NULL;
    }

    ~TickerData()
    {
#if !FINAL_RELEASE
        if (nentities)
            fprintf(stderr, "ERROR: still %i entities in ticker\n", nentities);
#endif
    }

private:
    /* Entity management */
    Entity *todo;
    Entity *list[Entity::GROUP_COUNT];
    int nentities;

    /* Fixed framerate management */
    int frame;
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
    entity->next = data->todo;
    data->todo = entity;
}

void Ticker::TickGame()
{
    Profiler::Stop(Profiler::STAT_TICK_FRAME);
    Profiler::Start(Profiler::STAT_TICK_FRAME);

    Profiler::Start(Profiler::STAT_TICK_GAME);

    data->frame++;

    data->deltams = data->timer.GetMs();
    data->bias += data->deltams;

    /* Garbage collect objects that can be destroyed. We can do this
     * before inserting awaiting objects, because there is no way these
     * are already marked for destruction. */
    for (int i = 0; i < Entity::GROUP_COUNT; i++)
        for (Entity *e = data->list[i], *prev = NULL; e; prev = e, e = e->next)
            if (e->destroy)
            {
                if (prev)
                    prev->next = e->next;
                else
                    data->list[i] = e->next;

                data->nentities--;
                delete e;
            }

    /* Insert waiting objects into the appropriate lists */
    while (data->todo)
    {
        Entity *e = data->todo;
        data->todo = e->next;

        int i = e->GetGroup();
        e->next = data->list[i];
        data->list[i] = e;
        data->nentities++;
    }

    /* Tick objects for the game loop */
    for (int i = 0; i < Entity::GROUP_COUNT; i++)
        for (Entity *e = data->list[i]; e; e = e->next)
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
    for (int i = 0; i < Entity::GROUP_COUNT; i++)
        for (Entity *e = data->list[i]; e; e = e->next)
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

