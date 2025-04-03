//
//  Lol Engine
//
//  Copyright © 2010–2025 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine/audio>
#include <memory>

#include "../3rdparty/qoa/qoa.h"

namespace lol::audio
{

// The QOA decoder (https://qoaformat.org/)

class qoa_decoder : public stream<int16_t>
{
public:
    qoa_decoder(uint8_t const *data, size_t size)
      : stream<int16_t>(1, 0),
        m_frame_count(0),
        m_frame_pos(0)
    {
        qoa_desc qoa;
        if (auto qoa_data = qoa_decode(data, (int)size, &qoa); qoa_data)
        {
            m_channels = qoa.channels;
            m_frequency = qoa.samplerate;
            // Should use QOA_FREE() instead of free() but it is private to the implementation.
            m_qoa_data = std::shared_ptr<int16_t>(qoa_data, [](int16_t *p) { free(p); });
            m_frame_count = qoa.samples;
        }
    }

    virtual size_t get(int16_t *buf, size_t frames) override
    {
        size_t todo = std::min(frames, m_frame_count - m_frame_pos);
        std::copy_n(m_qoa_data.get() + m_frame_pos * m_channels, todo * m_channels, buf);
        m_frame_pos += todo;
        return todo;
    }

    virtual std::optional<size_t> size() const override
    {
        return m_frame_count;
    }

    virtual std::optional<size_t> pos() const override
    {
        return m_frame_pos;
    }

    virtual bool seek(size_t pos) override
    {
        m_frame_pos = std::min(pos, m_frame_count);
        return true;
    }

protected:
    std::shared_ptr<int16_t> m_qoa_data;
    size_t m_frame_count;
    size_t m_frame_pos;
};

std::shared_ptr<stream<int16_t>> make_qoa_decoder(uint8_t const *data, size_t size)
{
    return std::make_shared<qoa_decoder>(data, size);
}

} // namespace lol::audio
