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
    int width, height, fps;
#if defined USE_PIPI
    pipi_sequence_t *sequence;
#endif
};

/*
 * Public DebugRecord class
 */

DebugRecord::DebugRecord(char const *path, float fps)
  : data(new DebugRecordData())
{
    data->path = strdup(path);
    data->width = 0;
    data->height = 0;
    data->fps = (int)(fps + 0.5f);
#if defined USE_PIPI
    data->sequence = NULL;
#endif

    drawgroup = DRAWGROUP_CAPTURE;
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

        data->sequence = pipi_open_sequence(data->path, width, height,
                                            1 /* RGB */, data->fps,
                                            1, 1, 60 * 1024 * 1024);
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
