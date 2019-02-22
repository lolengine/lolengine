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

#include <unordered_set>
#include <functional>

#if LOL_USE_SDL_MIXER
#   if HAVE_SDL2_SDL_H
#       include <SDL2/SDL.h>
#       include <SDL2/SDL_mixer.h>
#   elif HAVE_SDL_H
#       include <SDL.h>
#       include <SDL_mixer.h>
#   endif
#endif

namespace lol
{

struct audio_streamer
{
    int m_channel;
    std::function<void(void *, int)> m_callback;
#if defined LOL_USE_SDL_MIXER
    std::vector<uint8_t> m_empty; // SDL keeps a reference to this
    Mix_Chunk *m_chunk;
#endif
};

std::unordered_set<std::shared_ptr<audio_streamer>> audio::m_streamers;

 /*
 * Public audio class
 */

#if defined LOL_USE_SDL_MIXER
void audio::init()
{
    Mix_OpenAudio(22050, AUDIO_S16, 2, 1024);
    set_channels(8);
}

void audio::set_channels(int channels)
{
    Mix_AllocateChannels(channels);
}

void audio::set_volume(int channel, int volume)
{
    Mix_Volume(channel, volume);
}

void audio::mute_all()
{
    Mix_Volume(-1, 0);
}

void audio::unmute_all()
{
    Mix_Volume(-1, MIX_MAX_VOLUME);
}

int audio::start_streaming(std::function<void(void *, int)> const &f)
{
    static auto trampoline = [](int, void *stream, int bytes, void *udata)
    {
        auto s = (audio_streamer *)udata;
        s->m_callback(stream, bytes);
    };

    std::shared_ptr<audio_streamer> s = std::make_shared<audio_streamer>();
    m_streamers.insert(s);

    s->m_empty.resize(1024);
    s->m_chunk = Mix_QuickLoad_RAW(s->m_empty.data(), s->m_empty.size() * sizeof(uint8_t));
    s->m_channel = Mix_PlayChannel(-1, s->m_chunk, -1);
    s->m_callback = f;
    Mix_RegisterEffect(s->m_channel, trampoline, nullptr, s.get());

    return s->m_channel;
}

void audio::stop_streaming(int stream)
{
    for (auto streamer : m_streamers)
    {
        if (streamer->m_channel == stream)
        {
            Mix_HaltChannel(stream);
            m_streamers.erase(streamer);
            break;
        }
    }
}

#else
void audio::init() {}
void audio::set_channels(int) {}
void audio::set_volume(int, int) {}
void audio::mute_all() {}
void audio::unmute_all() {}
int audio::start_streaming(std::function<void(void *, int)> const &) { return -1; }
void audio::stop_streaming(int) {}
#endif

} /* namespace lol */

