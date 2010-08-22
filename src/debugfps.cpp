//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "core.h"
#include "debugfps.h"

/*
 * DebugFps implementation class
 */

class DebugFpsData
{
    friend class DebugFps;

private:
    int fontid;
    int frame;
};

/*
 * Public DebugFps class
 */

DebugFps::DebugFps()
{
    data = new DebugFpsData();

    data->fontid = Forge::Register("gfx/font/ascii.png");
    data->frame = 0;
}

Entity::Group DebugFps::GetGroup()
{
    return GROUP_AFTER;
}

void DebugFps::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    data->frame++;

    char buf[1024];
    Font *font = Forge::GetFont(data->fontid);

    sprintf(buf, "%2.2f fps (%i)",
            1e3f / Profiler::GetAvg(Profiler::STAT_TICK_FRAME), data->frame);
    font->PrintBold(10, 10, buf);

    sprintf(buf, "Game  % 7.2f % 7.2f",
            Profiler::GetAvg(Profiler::STAT_TICK_GAME),
            Profiler::GetMax(Profiler::STAT_TICK_GAME));
    font->PrintBold(10, 34, buf);

    sprintf(buf, "Draw  % 7.2f % 7.2f",
            Profiler::GetAvg(Profiler::STAT_TICK_DRAW),
            Profiler::GetMax(Profiler::STAT_TICK_DRAW));
    font->PrintBold(10, 50, buf);

    sprintf(buf, "Blit  % 7.2f % 7.2f",
            Profiler::GetAvg(Profiler::STAT_TICK_BLIT),
            Profiler::GetMax(Profiler::STAT_TICK_BLIT));
    font->PrintBold(10, 66, buf);

    sprintf(buf, "Frame % 7.2f % 7.2f",
            Profiler::GetAvg(Profiler::STAT_TICK_FRAME),
            Profiler::GetMax(Profiler::STAT_TICK_FRAME));
    font->PrintBold(10, 82, buf);
}

DebugFps::~DebugFps()
{
    Forge::Deregister(data->fontid);
    delete data;
}

