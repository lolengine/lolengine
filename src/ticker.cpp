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
#include "asset.h"
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
        nassets(0)
    {
        for (int i = 0; i < Asset::GROUP_COUNT; i++)
            list[i] = NULL;
        bias = 0.0f;
    }

    ~TickerData()
    {
#if !FINAL_RELEASE
        if (nassets)
            fprintf(stderr, "ERROR: still %i assets in ticker\n", nassets);
#endif
    }

private:
    /* Asset management */
    Asset *todo;
    Asset *list[Asset::GROUP_COUNT];
    int nassets;

    /* Fixed framerate management */
    Timer timer;
    float delta_time, bias;
}
tickerdata;

static TickerData * const data = &tickerdata;

/*
 * Ticker public class
 */

void Ticker::Register(Asset *asset)
{
    /* If we are called from its constructor, the object's vtable is not
     * ready yet, so we do not know which group this asset belongs to. Wait
     * until the first tick. */
    asset->next = data->todo;
    data->todo = asset;
}

void Ticker::TickGame()
{
    Profiler::Stop(Profiler::STAT_TICK_FRAME);
    Profiler::Start(Profiler::STAT_TICK_FRAME);

    Profiler::Start(Profiler::STAT_TICK_GAME);

    data->delta_time = data->timer.GetSeconds();
    data->bias += data->delta_time;

    /* Insert waiting objects in the appropriate lists */
    while (data->todo)
    {
        Asset *a = data->todo;
        data->todo = a->next;

        int i = a->GetGroup();
        a->next = data->list[i];
        data->list[i] = a;
        data->nassets++;
    }

    /* Garbage collect objects that can be destroyed */
    for (int i = 0; i < Asset::GROUP_COUNT; i++)
        for (Asset *a = data->list[i], *prev = NULL; a; prev = a, a = a->next)
            if (a->destroy)
            {
                if (prev)
                    prev->next = a->next;
                else
                    data->list[i] = a->next;

                data->nassets--;
                delete a;
            }

    /* Tick objects for the game loop */
    for (int i = 0; i < Asset::GROUP_COUNT; i++)
        for (Asset *a = data->list[i]; a; a = a->next)
            if (!a->destroy)
                a->TickGame(data->delta_time);

    Profiler::Stop(Profiler::STAT_TICK_GAME);
}

void Ticker::TickRender()
{
    Profiler::Start(Profiler::STAT_TICK_RENDER);

    /* Tick objects for the render loop */
    for (int i = 0; i < Asset::GROUP_COUNT; i++)
        for (Asset *a = data->list[i]; a; a = a->next)
            if (!a->destroy)
                a->TickRender(data->delta_time);

    Profiler::Stop(Profiler::STAT_TICK_RENDER);
    Profiler::Start(Profiler::STAT_TICK_BLIT);
}

void Ticker::ClampFps(float fps)
{
    Profiler::Stop(Profiler::STAT_TICK_BLIT);

    float ideal_time = 1.0f / fps;
    if (ideal_time > data->bias)
        data->timer.WaitSeconds(ideal_time - data->bias);
    data->bias -= ideal_time;
}

