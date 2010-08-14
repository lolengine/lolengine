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

    static int const HISTORY = 30;

private:
    float history[HISTORY];
    Font *font;
    int frame;
};

/*
 * Public DebugFps class
 */

DebugFps::DebugFps()
{
    data = new DebugFpsData();

    for (int i = 0; i < DebugFpsData::HISTORY; i++)
        data->history[i] = 0.0f;
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

    data->history[data->frame % DebugFpsData::HISTORY] = delta_time;
    data->frame++;

    float mean = 0.0f, max = 0.0f;
    for (int i = 0; i < DebugFpsData::HISTORY; i++)
    {
        mean += data->history[i];
        if (data->history[i] > max)
            max = data->history[i];
    }
    mean /= DebugFpsData::HISTORY;

    char buf[1024];
    sprintf(buf, "%3.2f ms (%3.2f fps) -- max %3.2f ms -- #%i",
            mean, 1000.0f / mean, max, data->frame);
    data->font->Print(10, 10, buf);
    data->font->Print(11, 10, buf);
    data->font->Print(10, 11, buf);
}

DebugFps::~DebugFps()
{
    Forge::ReleaseFont(data->font);
    delete data;
}

