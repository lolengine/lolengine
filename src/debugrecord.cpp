//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cstring>

#if defined USE_PIPI
#   include <pipi.h>
#endif

#include "core.h"
#include "debugrecord.h"

/*
 * DebugRecord implementation class
 */

class DebugRecordData
{
    friend class DebugRecord;

private:
    char const *path;
    int width, height;
#if defined USE_PIPI
    pipi_sequence_t *sequence;
#endif
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
#if defined USE_PIPI
    data->sequence = NULL;
#endif
}

Entity::Group DebugRecord::GetGroup()
{
    return GROUP_DRAW_CAPTURE;
}

void DebugRecord::TickGame(float deltams)
{
    Entity::TickGame(deltams);
}

void DebugRecord::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    int width = Video::GetWidth();
    int height = Video::GetHeight();

    if (data->width != width || data->height != height)
    {
        data->width = width;
        data->height = height;

#if defined USE_PIPI
        if (data->sequence)
            pipi_close_sequence(data->sequence);

        data->sequence = pipi_open_sequence(data->path, width, height, 30);
#endif
    }

#if defined USE_PIPI
    if (data->sequence)
    {
        uint32_t *buffer = new uint32_t[width * height];
        Video::Capture(buffer);
        pipi_feed_sequence(data->sequence, (uint8_t *)buffer, width, height);
        delete[] buffer;
    }
#endif
}

DebugRecord::~DebugRecord()
{
    delete data;
}
