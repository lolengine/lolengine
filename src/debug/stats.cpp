//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
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

    m_gamegroup = tickable::group::game::stats;
}

void DebugStats::tick_game(float seconds)
{
    Entity::tick_game(seconds);

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

