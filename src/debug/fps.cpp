//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "core.h"
#include "loldebug.h"

using namespace std;

namespace lol
{

/*
 * DebugFps implementation class
 */

class DebugFpsData
{
    friend class DebugFps;

private:
    Text *lines[5];
};

/*
 * Public DebugFps class
 */

DebugFps::DebugFps(int x, int y)
  : data(new DebugFpsData())
{
#if 0
    for (int i = 0; i < 5; i ++)
    {
        data->lines[i] = new Text(NULL, "src/data/font/ascii.png");
        data->lines[i]->SetPos(ivec3(x, y + (i ? 8 : 0) + 16 * i, 0));
        Ticker::Ref(data->lines[i]);
    }
#else
    data->lines[0] = new Text(NULL, "src/data/font/ascii.png");
    data->lines[0]->SetPos(ivec3(x, y, 100));
    Ticker::Ref(data->lines[0]);
#endif
}

void DebugFps::TickGame(float seconds)
{
    Entity::TickGame(seconds);

    char buf[1024];

#if 0
    sprintf(buf, "%2.2f fps (%i)",
            1.0f / Profiler::GetAvg(Profiler::STAT_TICK_FRAME),
            Ticker::GetFrameNum());
    data->lines[0]->SetText(buf);

    sprintf(buf, "Game  % 7.2f % 7.2f",
            1e3f * Profiler::GetAvg(Profiler::STAT_TICK_GAME),
            1e3f * Profiler::GetMax(Profiler::STAT_TICK_GAME));
    data->lines[1]->SetText(buf);

    sprintf(buf, "Draw  % 7.2f % 7.2f",
            1e3f * Profiler::GetAvg(Profiler::STAT_TICK_DRAW),
            1e3f * Profiler::GetMax(Profiler::STAT_TICK_DRAW));
    data->lines[2]->SetText(buf);

    sprintf(buf, "Blit  % 7.2f % 7.2f",
            1e3f * Profiler::GetAvg(Profiler::STAT_TICK_BLIT),
            1e3f * Profiler::GetMax(Profiler::STAT_TICK_BLIT));
    data->lines[3]->SetText(buf);

    sprintf(buf, "Frame % 7.2f % 7.2f",
            1e3f * Profiler::GetAvg(Profiler::STAT_TICK_FRAME),
            1e3f * Profiler::GetMax(Profiler::STAT_TICK_FRAME));
    data->lines[4]->SetText(buf);
#else
    sprintf(buf, "%2.2f/%2.2f/%2.2f/%2.2f %2.2f fps (%i)",
            1e3f * Profiler::GetAvg(Profiler::STAT_TICK_GAME),
            1e3f * Profiler::GetAvg(Profiler::STAT_TICK_DRAW),
            1e3f * Profiler::GetAvg(Profiler::STAT_TICK_BLIT),
            1e3f * Profiler::GetAvg(Profiler::STAT_TICK_FRAME),
            1.0f / Profiler::GetAvg(Profiler::STAT_TICK_FRAME),
            Ticker::GetFrameNum());
    data->lines[0]->SetText(buf);
#endif
}

DebugFps::~DebugFps()
{
#if 0
    for (int i = 0; i < 5; i ++)
        Ticker::Unref(data->lines[i]);
#else
    Ticker::Unref(data->lines[0]);
#endif
    delete data;
}

} /* namespace lol */

