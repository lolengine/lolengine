//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Profiler class
// -------------------
// The Profiler is a static class that collects statistic counters.
//

#include <stdint.h>

namespace lol
{

class Profiler
{
public:
    enum
    {
        STAT_TICK_FRAME = 0,
        STAT_TICK_GAME,
        STAT_TICK_DRAW,
        STAT_TICK_BLIT,
        STAT_USER_00,
        STAT_USER_01,
        STAT_USER_02,
        STAT_USER_03,
        STAT_USER_04,
        STAT_USER_05,
        STAT_USER_06,
        STAT_USER_07,
        STAT_USER_08,
        STAT_USER_09,
        STAT_COUNT
    };

    static void Start(int id);
    static void Stop(int id);
    static float GetAvg(int id);
    static float GetMax(int id);

private:
    Profiler() {}
};

} /* namespace lol */

