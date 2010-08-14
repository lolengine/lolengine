//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include "debugfps.h"
#include "forge.h"

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

Asset::Group DebugFps::GetGroup()
{
    return GROUP_AFTER;
}

void DebugFps::TickRender(float delta_time)
{
    Asset::TickGame(delta_time);

    char buf[1024];
    sprintf(buf, "%3.2f fps (%i)", 1000.0f / delta_time, data->frame++);
    data->font->Print(10, 10, buf);
    data->font->Print(11, 10, buf);
    data->font->Print(10, 11, buf);
}

DebugFps::~DebugFps()
{
    Forge::ReleaseFont(data->font);
    delete data;
}

