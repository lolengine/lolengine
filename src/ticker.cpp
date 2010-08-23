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
        todolist(0), autolist(0),
        nentities(0),
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
    Entity *list[Entity::GROUP_COUNT];
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
    entity->next = data->todolist;
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
                if (prev)
                    prev->autonext = e->autonext;
                else
                    data->autolist = e->autonext;
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
    for (int i = 0; i < Entity::GROUP_COUNT; i++)
    {
        fprintf(stderr, "Group %i\n", i);

        for (Entity *e = data->list[i]; e; e = e->next)
            fprintf(stderr, "  \\-- %s (ref %i, destroy %i)\n", e->GetName(), e->ref, e->destroy);
    }
#endif

    data->frame++;

    data->deltams = data->timer.GetMs();
    data->bias += data->deltams;

    /* Garbage collect objects that can be destroyed. We can do this
     * before inserting awaiting objects, because there is no way these
     * are already marked for destruction. */
    for (int i = 0; i < Entity::GROUP_COUNT; i++)
        for (Entity *e = data->list[i], *prev = NULL; e; )
        {
            if (e->destroy)
            {
                if (prev)
                    prev->next = e->next;
                else
                    data->list[i] = e->next;

                Entity *tmp = e;
                e = e->next;
                delete tmp;

                data->nentities--;
            }
            else
            {
                if (e->ref <= 0)
                    e->destroy = 1;
                prev = e;
                e = e->next;
            }
        }

    /* Insert waiting objects into the appropriate lists */
    while (data->todolist)
    {
        Entity *e = data->todolist;
        data->todolist = e->next;

        int i = e->GetGroup();
        e->next = data->list[i];
        data->list[i] = e;
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

