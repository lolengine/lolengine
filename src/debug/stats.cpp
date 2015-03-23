//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdio>

#include "loldebug.h"

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

    m_gamegroup = GAMEGROUP_STATS;
}

void DebugStats::TickGame(float seconds)
{
    Entity::TickGame(seconds);

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

