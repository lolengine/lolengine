//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>

#if defined USE_PIPI
#   include <pipi.h>
#endif

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
    std::string m_path;
    ivec2 m_size;
    int m_fps;
#if defined USE_PIPI
    pipi_sequence_t *m_sequence;
#endif
};

/*
 * Public DebugRecord class
 */

DebugRecord::DebugRecord(std::string const &path, float fps)
  : m_data(new DebugRecordData())
{
    Ticker::StartRecording();

    m_data->m_path = path;
    m_data->m_size = ivec2::zero;
    m_data->m_fps = (int)(fps + 0.5f);
#if defined USE_PIPI
    m_data->m_sequence = nullptr;
#endif

    m_drawgroup = tickable::group::draw::capture;
}

void DebugRecord::tick_game(float seconds)
{
    entity::tick_game(seconds);
}

void DebugRecord::tick_draw(float seconds, Scene &scene)
{
    entity::tick_draw(seconds, scene);

    ivec2 size = video::size();

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
        video::capture(buffer);
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

