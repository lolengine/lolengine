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

#include <cstring>

#if defined USE_PIPI
#   include <pipi.h>
#endif

#include <lol/main.h>
#include "loldebug.h"

namespace lol
{

/*
 * DebugRecord implementation class
 */

class DebugRecordData
{
    friend class DebugRecord;

private:
    String m_path;
    ivec2 m_size;
    int m_fps;
#if defined USE_PIPI
    pipi_sequence_t *m_sequence;
#endif
};

/*
 * Public DebugRecord class
 */

DebugRecord::DebugRecord(String const &path, float fps)
  : m_data(new DebugRecordData())
{
    Ticker::StartRecording();

    m_data->m_path = path;
    m_data->m_size = ivec2::zero;
    m_data->m_fps = (int)(fps + 0.5f);
#if defined USE_PIPI
    m_data->m_sequence = nullptr;
#endif

    m_drawgroup = DRAWGROUP_CAPTURE;
}

void DebugRecord::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

void DebugRecord::TickDraw(float seconds, Scene &scene)
{
    Entity::TickDraw(seconds, scene);

    ivec2 size = Video::GetSize();

    if (m_data->m_size != size)
    {
        m_data->m_size = size;

#if defined USE_PIPI
        if (m_data->m_sequence)
            pipi_close_sequence(m_data->m_sequence);

        m_data->m_sequence = pipi_open_sequence(m_data->m_path, size.x, size.y,
                                                1 /* RGB */, m_data->m_fps,
                                                1, 1, 60 * 1024 * 1024);
#endif
    }

#if defined USE_PIPI
    if (m_data->m_sequence)
    {
        uint32_t *buffer = new uint32_t[size.x * size.y];
        Video::Capture(buffer);
        pipi_feed_sequence(m_data->m_sequence, (uint8_t *)buffer,
                           size.x, size.y);
        delete[] buffer;
    }
#endif
}

DebugRecord::~DebugRecord()
{
    Ticker::StopRecording();

    delete m_data;
}

} /* namespace lol */

