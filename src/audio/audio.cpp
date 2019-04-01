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

#include <array>
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

// Buffer size, in samples (https://wiki.libsdl.org/SDL_AudioSpec)
// “[…] refers to the size of the audio buffer in sample frames. A sample frame
// is a chunk of audio data of the size specified in format multiplied by the
// number of channels.”
#define LOL_AUDIO_SAMPLE_FRAMES 1024
#define LOL_AUDIO_CHANNELS 2

namespace lol
{

struct audio_streamer
{
    int m_channel;
    std::function<void(void *, int)> m_callback;
    std::array<uint16_t, LOL_AUDIO_CHANNELS * LOL_AUDIO_SAMPLE_FRAMES> m_empty; // SDL needs a reference to this
#if defined LOL_USE_SDL_MIXER
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
    if (Mix_OpenAudio(22050, AUDIO_S16, LOL_AUDIO_CHANNELS, LOL_AUDIO_SAMPLE_FRAMES) < 0)
    {
        msg::error("error opening audio: %s\n", Mix_GetError());
        return;
    }

    set_channels(8);

    int frequency, channels;
    Uint16 format;
    if (Mix_QuerySpec(&frequency, &format, &channels) == 0)
    {
        msg::error("error querying audio: %s\n", Mix_GetError());
        return;
    }

    char const *sformat = "unknown";
    switch (format)
    {
        case AUDIO_U8: sformat = "uint8"; break;
        case AUDIO_S8: sformat = "int8"; break;
        case AUDIO_U16LSB: sformat = "uint16le"; break;
        case AUDIO_U16MSB: sformat = "uint16be"; break;
        case AUDIO_S16LSB: sformat = "int16le"; break;
        case AUDIO_S16MSB: sformat = "int16be"; break;
        case AUDIO_S32LSB: sformat = "int32le"; break;
        case AUDIO_S32MSB: sformat = "int32be"; break;
        case AUDIO_F32LSB: sformat = "float32le"; break;
        case AUDIO_F32MSB: sformat = "float32be"; break;
    }
    msg::info("audio freq=%dHz format=%s channels=%d\n",
              frequency, sformat, channels);
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

    auto s = std::make_shared<audio_streamer>();
    m_streamers.insert(s);

    Uint8* audio_data = (Uint8*)s->m_empty.data();
    Uint32 audio_size = (Uint32)(s->m_empty.size() * sizeof(s->m_empty[0]));
    memset(audio_data, 17, audio_size);
    s->m_chunk = Mix_QuickLoad_RAW(audio_data, audio_size);
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

