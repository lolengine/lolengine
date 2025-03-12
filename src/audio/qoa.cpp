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
#include <vector>

#include "../3rdparty/qoa/qoa.h"

namespace lol::audio
{

// The QOA decoder (https://qoaformat.org/)

class qoa_decoder : public stream<int16_t>
{
public:
    qoa_decoder(uint8_t const *data, size_t size)
      : stream<int16_t>(1, 0),
        m_sample_pos(0)
    {
        qoa_desc qoa;
        if (auto qoa_data = qoa_decode(data, (int)size, &qoa); qoa_data)
        {
            m_channels = qoa.channels;
            m_frequency = qoa.samplerate;
            // This is possibly a large memory copy but we accept the overhead; the other way
            // to do this would be to duplicate the qoa_decode() code and I don’t like that.
            m_samples.assign(qoa_data, qoa_data + qoa.samples * qoa.channels);
            // Should be QOA_FREE() for consistency but it is private to the implementation.
            free(qoa_data);
        }
    }

    virtual size_t get(int16_t *buf, size_t frames) override
    {
        size_t samples = std::min(frames * m_channels, m_samples.size() - m_sample_pos);
        std::copy_n(std::next(m_samples.begin(), m_sample_pos), samples, buf);
        m_sample_pos += samples;
        return samples / m_channels;
    }

    virtual std::optional<size_t> size() const override
    {
        return m_samples.size() / m_channels;
    }

    virtual std::optional<size_t> pos() const override
    {
        return m_sample_pos / m_channels;
    }

    virtual bool seek(size_t pos) override
    {
        m_sample_pos = std::min(pos * m_channels, m_samples.size());
        return true;
    }

protected:
    std::vector<int16_t> m_samples;
    size_t m_sample_pos;
};

std::shared_ptr<stream<int16_t>> make_qoa_decoder(uint8_t const *data, size_t size)
{
    return std::make_shared<qoa_decoder>(data, size);
}

} // namespace lol::audio
