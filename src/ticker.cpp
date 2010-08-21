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

#include "profiler.h"
#include "ticker.h"
#include "entity.h"
#include "timer.h"

/*
 * Ticker implementation class
 */

static class TickerData
{
    friend class Ticker;

public:
    TickerData() :
        todo(0),
        nentities(0)
    {
        for (int i = 0; i < Entity::GROUP_COUNT; i++)
            list[i] = NULL;
        bias = 0.0f;
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
    Timer timer;
    float delta_time, bias;
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

    data->delta_time = data->timer.GetSeconds();
    data->bias += data->delta_time;

    /* Garbage collect objects that can be destroyed. We can do this
     * before inserting awaiting objects, because there is no way these
     * are already marked for destruction. */
    for (int i = 0; i < Entity::GROUP_COUNT; i++)
        for (Entity *a = data->list[i], *prev = NULL; a; prev = a, a = a->next)
            if (a->destroy)
            {
                if (prev)
                    prev->next = a->next;
                else
                    data->list[i] = a->next;

                data->nentities--;
                delete a;
            }

    /* Insert waiting objects into the appropriate lists */
    while (data->todo)
    {
        Entity *a = data->todo;
        data->todo = a->next;

        int i = a->GetGroup();
        a->next = data->list[i];
        data->list[i] = a;
        data->nentities++;
    }

    /* Tick objects for the game loop */
    for (int i = 0; i < Entity::GROUP_COUNT; i++)
        for (Entity *a = data->list[i]; a; a = a->next)
            if (!a->destroy)
            {
#if !FINAL_RELEASE
                if (a->state != Entity::STATE_IDLE)
                    fprintf(stderr, "ERROR: entity not idle for game tick\n");
                a->state = Entity::STATE_PRETICK_GAME;
#endif
                a->TickGame(data->delta_time);
#if !FINAL_RELEASE
                if (a->state != Entity::STATE_POSTTICK_GAME)
                    fprintf(stderr, "ERROR: entity missed super game tick\n");
                a->state = Entity::STATE_IDLE;
#endif
            }

    Profiler::Stop(Profiler::STAT_TICK_GAME);
}

void Ticker::TickRender()
{
    Profiler::Start(Profiler::STAT_TICK_RENDER);

    /* Tick objects for the render loop */
    for (int i = 0; i < Entity::GROUP_COUNT; i++)
        for (Entity *a = data->list[i]; a; a = a->next)
            if (!a->destroy)
            {
#if !FINAL_RELEASE
                if (a->state != Entity::STATE_IDLE)
                    fprintf(stderr, "ERROR: entity not idle for render tick\n");
                a->state = Entity::STATE_PRETICK_RENDER;
#endif
                a->TickRender(data->delta_time);
#if !FINAL_RELEASE
                if (a->state != Entity::STATE_POSTTICK_RENDER)
                    fprintf(stderr, "ERROR: entity missed super render tick\n");
                a->state = Entity::STATE_IDLE;
#endif
            }

    Profiler::Stop(Profiler::STAT_TICK_RENDER);
    Profiler::Start(Profiler::STAT_TICK_BLIT);
}

void Ticker::ClampFps(float delta_time)
{
    Profiler::Stop(Profiler::STAT_TICK_BLIT);

    if (delta_time > data->bias + 0.2f)
        delta_time = data->bias + 0.2f; // Don't go below 5 fps
    if (delta_time > data->bias)
        data->timer.WaitSeconds(delta_time - data->bias);
    data->bias -= delta_time;
}

