//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "core.h"
#include "debugstats.h"

/*
 * DebugStats implementation class
 */

class DebugStatsData
{
    friend class DebugStats;

private:
    FILE *fp;
};

/*
 * Public DebugStats class
 */

DebugStats::DebugStats(char const *path)
{
    data = new DebugStatsData();
    data->fp = fopen(path, "w+");
}

Entity::Group DebugStats::GetGroup()
{
    return GROUP_AFTER;
}

void DebugStats::TickGame(float deltams)
{
    Entity::TickGame(deltams);

    fprintf(data->fp, "%i %f %f %f %f\n",
            Ticker::GetFrameNum(),
            Profiler::GetAvg(Profiler::STAT_TICK_GAME),
            Profiler::GetAvg(Profiler::STAT_TICK_DRAW),
            Profiler::GetAvg(Profiler::STAT_TICK_BLIT),
            Profiler::GetAvg(Profiler::STAT_TICK_FRAME));
}

DebugStats::~DebugStats()
{
    fclose(data->fp);
    delete data;
}

