//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cstring>

#include <pipi.h>

#include "debugrecord.h"
#include "video.h"

/*
 * DebugRecord implementation class
 */

class DebugRecordData
{
    friend class DebugRecord;

private:
    char const *path;
    int width, height;
    pipi_sequence_t *sequence;
};

/*
 * Public DebugRecord class
 */

DebugRecord::DebugRecord(char const *path)
{
    data = new DebugRecordData();
    data->path = strdup(path);
    data->width = 0;
    data->height = 0;
    data->sequence = NULL;
}

Asset::Group DebugRecord::GetGroup()
{
    return GROUP_RENDER_CAPTURE;
}

void DebugRecord::TickGame(float delta_time)
{
    Asset::TickGame(delta_time);
}

void DebugRecord::TickRender(float delta_time)
{
    Asset::TickRender(delta_time);

    int width = Video::GetWidth();
    int height = Video::GetHeight();

    if (data->width != width || data->height != height)
    {
        data->width = width;
        data->height = height;

        if (data->sequence)
            pipi_close_sequence(data->sequence);

        data->sequence = pipi_open_sequence(data->path, width, height, 30);
    }

    if (data->sequence)
    {
        uint32_t *buffer = new uint32_t[width * height];
        Video::Capture(buffer);
        pipi_feed_sequence(data->sequence, (uint8_t *)buffer, width, height);
        delete[] buffer;
    }
}

DebugRecord::~DebugRecord()
{
    delete data;
}

