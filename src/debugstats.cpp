//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "core.h"
#include "debugstats.h"

namespace lol
{

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
  : data(new DebugStatsData())
{
    data->fp = fopen(path, "w+");

    gamegroup = GAMEGROUP_AFTER;
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

} /* namespace lol */

