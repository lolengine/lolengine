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

#include <array>
#include <functional>
#include <lol/engine/audio>
#include <lol/msg>
#include <memory>
#include <mutex>
#include <unordered_map>

#if LOL_USE_KORE
#   include <kinc/audio2/audio.h>
#endif

namespace lol::audio
{

static std::shared_ptr<mixer<float>> g_mixer;
static std::mutex g_mutex;

// FIXME: this could be removed by inserting this in g_mixer
std::unordered_map<int, std::shared_ptr<stream<float>>> g_streams;

void init()
{
    g_mixer = std::make_shared<mixer<float>>(2, 48000);

#if LOL_USE_KORE
    kinc_a2_init();
    kinc_a2_set_callback([](kinc_a2_buffer_t* buffer, uint32_t samples, void* userdata)
    {
        size_t channel_count;
        std::vector<float> tmp;

        {
            std::unique_lock<std::mutex> lock(g_mutex);

            channel_count = g_mixer->channels();
            tmp.resize(samples * channel_count);
            g_mixer->get(tmp.data(), samples);
        }

        for (size_t n = 0; n < samples; ++n)
        {
            for (size_t ch = 0; ch < channel_count; ++ch)
                buffer->channels[ch][buffer->write_location] = lol::audio::sample::softclip(tmp[n * channel_count + ch]);

            buffer->write_location = (buffer->write_location + 1) % buffer->data_size;
        }
    }, nullptr);
#endif
}

void shutdown()
{
#if LOL_USE_KORE
    kinc_a2_set_callback(nullptr, nullptr);
#endif
}

template<>
int start_stream(std::shared_ptr<stream<float>> s)
{
    static int idx = 0;
    std::unique_lock<std::mutex> lock(g_mutex);
    g_streams[idx] = s;
    g_mixer->add(s);
    return idx++;
}

void stop_stream(int id)
{
    std::unique_lock<std::mutex> lock(g_mutex);
    auto s = g_streams[id];
    g_streams.erase(id);
    g_mixer->remove(s);
}

} // namespace lol::audio
