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

#include <cstring>

#if defined USE_PIPI
#   include <pipi.h>
#endif

#include "core.h"
#include "loldebug.h"

using namespace std;

namespace lol
{

/*
 * DebugRecord implementation class
 */

class DebugRecordData
{
    friend class DebugRecord;

private:
    char const *path;
    ivec2 size;
    int fps;
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
    Ticker::StartRecording();

    data->path = strdup(path);
    data->size = 0;
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

    ivec2 size = Video::GetSize();

    if (data->size != size)
    {
        data->size = size;

#if defined USE_PIPI
        if (data->sequence)
            pipi_close_sequence(data->sequence);

        data->sequence = pipi_open_sequence(data->path, size.x, size.y,
                                            1 /* RGB */, data->fps,
                                            1, 1, 60 * 1024 * 1024);
#endif
    }

#if defined USE_PIPI
    if (data->sequence)
    {
        uint32_t *buffer = new uint32_t[size.x * size.y];
        Video::Capture(buffer);
        pipi_feed_sequence(data->sequence, (uint8_t *)buffer, size.x, size.y);
        delete[] buffer;
    }
#endif
}

DebugRecord::~DebugRecord()
{
    Ticker::StopRecording();

    delete data;
}

} /* namespace lol */

