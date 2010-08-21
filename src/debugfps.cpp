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
    Font *font;
    int frame;
};

/*
 * Public DebugFps class
 */

DebugFps::DebugFps()
{
    data = new DebugFpsData();

    data->font = Forge::GetFont("gfx/font/ascii.png");
    data->frame = 0;
}

Entity::Group DebugFps::GetGroup()
{
    return GROUP_AFTER;
}

void DebugFps::TickRender(float deltams)
{
    Entity::TickRender(deltams);

    data->frame++;

    char buf[1024];

    sprintf(buf, "%2.2f fps (%i)",
            1e3f / Profiler::GetMean(Profiler::STAT_TICK_FRAME), data->frame);
    data->font->PrintBold(10, 10, buf);

    sprintf(buf, "Game   % 7.2f % 7.2f",
            Profiler::GetMean(Profiler::STAT_TICK_GAME),
            Profiler::GetMax(Profiler::STAT_TICK_GAME));
    data->font->PrintBold(10, 34, buf);

    sprintf(buf, "Render % 7.2f % 7.2f",
            Profiler::GetMean(Profiler::STAT_TICK_RENDER),
            Profiler::GetMax(Profiler::STAT_TICK_RENDER));
    data->font->PrintBold(10, 50, buf);

    sprintf(buf, "Blit   % 7.2f % 7.2f",
            Profiler::GetMean(Profiler::STAT_TICK_BLIT),
            Profiler::GetMax(Profiler::STAT_TICK_BLIT));
    data->font->PrintBold(10, 66, buf);

    sprintf(buf, "Frame  % 7.2f % 7.2f",
            Profiler::GetMean(Profiler::STAT_TICK_FRAME),
            Profiler::GetMax(Profiler::STAT_TICK_FRAME));
    data->font->PrintBold(10, 82, buf);
}

DebugFps::~DebugFps()
{
    Forge::ReleaseFont(data->font);
    delete data;
}

