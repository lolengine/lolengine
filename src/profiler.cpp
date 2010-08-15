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
#include "timer.h"

/*
 * Profiler implementation class
 */

static class ProfilerData
{
    friend class Profiler;

    static int const HISTORY = 30;

public:
    ProfilerData()
    {
        for (int i = 0; i < HISTORY; i++)
            history[i] = 0.0f;
        frame = 0;
        mean = max = 0.0f;
    }

private:
    float history[HISTORY];
    Timer timer;
    int frame;
    float mean, max;
}
data[Profiler::STAT_COUNT];

/*
 * Profiler public class
 */

void Profiler::Start(int id)
{
    data[id].timer.GetSeconds();
}

void Profiler::Stop(int id)
{
    float delta_time = data[id].timer.GetSeconds();

    data[id].history[data->frame % ProfilerData::HISTORY] = delta_time;
    data[id].frame++;
    data[id].mean = 0.0f;
    data[id].max = 0.0f;

    for (int i = 0; i < ProfilerData::HISTORY; i++)
    {
        data[id].mean += data[id].history[i];
        if (data[id].history[i] > data[id].max)
            data[id].max = data[id].history[i];
    }
    data[id].mean /= ProfilerData::HISTORY;
}

float Profiler::GetMean(int id)
{
    return data[id].mean;
}

float Profiler::GetMax(int id)
{
    return data[id].max;
}

