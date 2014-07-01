//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
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

#include <lol/main.h>

namespace lol
{

/*
 * Profiler implementation class
 */

static class ProfilerData
{
    friend class Profiler;

    static int const HISTORY = 32;

public:
    ProfilerData()
    {
        for (int i = 0; i < HISTORY; i++)
            history[i] = 0.0f;
        avg = max = 0.0f;
    }

private:
    float history[HISTORY];
    Timer timer;
    float avg, max;
}
data[Profiler::STAT_COUNT];

/*
 * Profiler public class
 */

void Profiler::Start(int id)
{
    data[id].timer.Get();
}

void Profiler::Stop(int id)
{
    float seconds = data[id].timer.Get();

    data[id].history[Ticker::GetFrameNum() % ProfilerData::HISTORY] = seconds;
    data[id].avg = 0.0f;
    data[id].max = 0.0f;

    for (int i = 0; i < ProfilerData::HISTORY; i++)
    {
        data[id].avg += data[id].history[i];
        if (data[id].history[i] > data[id].max)
            data[id].max = data[id].history[i];
    }
    data[id].avg /= ProfilerData::HISTORY;
}

float Profiler::GetAvg(int id)
{
    return data[id].avg;
}

float Profiler::GetMax(int id)
{
    return data[id].max;
}

} /* namespace lol */

