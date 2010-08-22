//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Profiler class
// -------------------
// The Profiler is a static class that collects statistic counters.
//

#if !defined __DH_PROFILER_H__
#define __DH_PROFILER_H__

#include <stdint.h>

class Profiler
{
public:
    enum
    {
        STAT_TICK_FRAME = 0,
        STAT_TICK_GAME,
        STAT_TICK_DRAW,
        STAT_TICK_BLIT,
        STAT_COUNT
    };

    static void Start(int id);
    static void Stop(int id);
    static float GetAvg(int id);
    static float GetMax(int id);
};

#endif // __DH_PROFILER_H__

