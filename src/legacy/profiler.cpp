//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdlib>
#include <stdint.h>

namespace lol
{

/*
 * Profiler implementation class
 */

static class profiler_data
{
    friend class Profiler;

    void update()
    {
        history[frame++ % HISTORY] = m_timer.get();
        avg = max = 0.0f;

        for (int i = 0; i < HISTORY; i++)
        {
            avg += history[i];
            if (history[i] > max)
                max = history[i];
        }
        avg /= HISTORY;
    }

    static int const HISTORY = 32;

private:
    float history[HISTORY] = { 0.0f };
    int frame = 0;
    timer m_timer;
    float avg = 0.0f, max = 0.0f;
}
data[Profiler::STAT_COUNT];

/*
 * Profiler public class
 */

void Profiler::Start(int id)
{
    data[id].m_timer.get();
}

void Profiler::Stop(int id)
{
    data[id].update();
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

