//
//  Lol Engine
//
//  Copyright © 2010—2016 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

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
    array<uint8_t> m_empty; // SDL keeps a reference to this
    Mix_Chunk *m_chunk;
#endif
};

array<audio_streamer *> g_streamers;

 /*
 * Public audio class
 */

void audio::init()
{
#if defined LOL_USE_SDL_MIXER
    Mix_OpenAudio(22050, AUDIO_S16, 2, 1024);
    set_channels(8);
#endif
}

void audio::set_channels(int channels)
{
#if defined LOL_USE_SDL_MIXER
    Mix_AllocateChannels(channels);
#else
    UNUSED(channels);
#endif
}

void audio::set_volume(int channel, int volume)
{
#if defined LOL_USE_SDL_MIXER
    Mix_Volume(channel, volume);
#else
    UNUSED(channel, volume);
#endif
}

void audio::mute_all()
{
#if defined LOL_USE_SDL_MIXER
    Mix_Volume(-1,0);
#endif
}

void audio::unmute_all()
{
#if defined LOL_USE_SDL_MIXER
    Mix_Volume(-1,MIX_MAX_VOLUME);
#endif
}

int audio::start_streaming(std::function<void(void *, int)> const &f)
{
#if defined LOL_USE_SDL_MIXER
    static auto trampoline = [](int, void *stream, int bytes, void *udata)
    {
        auto s = (audio_streamer *)udata;
        s->m_callback(stream, bytes);
    };

    audio_streamer *s = new audio_streamer();
    g_streamers.push(s);

    s->m_empty.resize(1024);
    s->m_chunk = Mix_QuickLoad_RAW(s->m_empty.data(), s->m_empty.bytes());
    s->m_channel = Mix_PlayChannel(-1, s->m_chunk, -1);
    s->m_callback = f;
    Mix_RegisterEffect(s->m_channel, trampoline, nullptr, s);

    return s->m_channel;
#else
    UNUSED(f);
    return -1;
#endif
}

void audio::stop_streaming(int channel)
{
#if defined LOL_USE_SDL_MIXER
    for (int i = 0; i < g_streamers.count(); ++i)
    {
        if (g_streamers[i]->m_channel == channel)
        {
            Mix_HaltChannel(channel);
            delete g_streamers[i];
            g_streamers.remove(i);
            return;
        }
    }
#else
    UNUSED(channel);
#endif
}

} /* namespace lol */

