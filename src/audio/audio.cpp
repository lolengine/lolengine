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
#define LOL_AUDIO_DEFAULT_FRAMES 1024

#define LOL_AUDIO_DEFAULT_TRACKS 8
#define LOL_AUDIO_DEFAULT_CHANNELS 2
#define LOL_AUDIO_DEFAULT_FORMAT AUDIO_S16
#define LOL_AUDIO_DEFAULT_RATE 22050

namespace lol
{

struct audio_streamer
{
    int m_track = -1;
    std::function<void(void *, int)> m_callback;
    // Buffer where the streaming client will write, and where the SDL
    // audio conversion may happen.
    std::vector<uint8_t> m_convert_buffer;
    // Remaining bytes in the conversion buffer.
    int m_extra_bytes = 0;
    // Buffer used to write the converted audio data for mixing.
    std::vector<uint8_t> m_output_buffer;
#if defined LOL_USE_SDL_MIXER
    SDL_AudioCVT m_convert;
    Mix_Chunk *m_chunk = nullptr;
#endif
};

std::unordered_set<std::shared_ptr<audio_streamer>> audio::m_streamers;

// The global audio format
static int g_frequency, g_channels;
static audio::format g_format;

static audio::format sdl2lol_format(Uint16 sdl_format)
{
    switch (sdl_format)
    {
        case AUDIO_U8: return audio::format::uint8;
        case AUDIO_S8: return audio::format::int8;
        case AUDIO_U16LSB: return audio::format::uint16le;
        case AUDIO_U16MSB: return audio::format::uint16be;
        case AUDIO_S16LSB: return audio::format::int16le;
        case AUDIO_S16MSB: return audio::format::int16be;
        case AUDIO_S32LSB: return audio::format::int32le;
        case AUDIO_S32MSB: return audio::format::int32be;
        case AUDIO_F32LSB: return audio::format::float32le;
        case AUDIO_F32MSB: return audio::format::float32be;
        default: return audio::format::unknown;
    }
}

static int lol2sdl_format(audio::format format)
{
    switch (format)
    {
        case audio::format::uint8: return AUDIO_U8;
        case audio::format::int8:  return AUDIO_S8;
        case audio::format::uint16le:  return AUDIO_U16LSB;
        case audio::format::uint16be:  return AUDIO_U16MSB;
        case audio::format::int16le:   return AUDIO_S16LSB;
        case audio::format::int16be:   return AUDIO_S16MSB;
        case audio::format::int32le:   return AUDIO_S32LSB;
        case audio::format::int32be:   return AUDIO_S32MSB;
        case audio::format::float32le: return AUDIO_F32LSB;
        case audio::format::float32be: return AUDIO_F32MSB;
        default: return 0;
    }
}

 /*
 * Public audio class
 */

#if defined LOL_USE_SDL_MIXER
void audio::init()
{
    if (Mix_OpenAudio(LOL_AUDIO_DEFAULT_RATE, LOL_AUDIO_DEFAULT_FORMAT, LOL_AUDIO_DEFAULT_CHANNELS, LOL_AUDIO_DEFAULT_FRAMES) < 0)
    {
        msg::error("error opening audio: %s\n", Mix_GetError());
        return;
    }

    set_tracks(LOL_AUDIO_DEFAULT_TRACKS);

    Uint16 sdl_format;
    if (Mix_QuerySpec(&g_frequency, &sdl_format, &g_channels) == 0)
    {
        msg::error("error querying audio: %s\n", Mix_GetError());
        return;
    }

    g_format = sdl2lol_format(sdl_format);

    char const *u = (SDL_AUDIO_ISFLOAT(sdl_format) || SDL_AUDIO_ISSIGNED(sdl_format)) ? "" : "u";
    char const *t = SDL_AUDIO_ISFLOAT(sdl_format) ? "float" : "int";
    int b = SDL_AUDIO_BITSIZE(sdl_format);
    char const *e = b <= 8 ? "" : SDL_AUDIO_ISLITTLEENDIAN(sdl_format) ? "le" : "be";

    msg::info("audio initialised: freq=%dHz format=%s%s%d%s channels=%d\n",
              g_frequency, u, t, b, e, g_channels);
}

void audio::set_tracks(int tracks)
{
    Mix_AllocateChannels(tracks);
}

void audio::set_volume(int track, int volume)
{
    Mix_Volume(track, volume);
}

void audio::mute_all()
{
    Mix_Volume(-1, 0);
}

void audio::unmute_all()
{
    Mix_Volume(-1, MIX_MAX_VOLUME);
}

int audio::start_streaming(std::function<void(void *, int)> const &f,
                           enum audio::format format /* = audio::format::uint16le */,
                           int frequency /* = 22050 */,
                           int channels /* = 2 */)
{
    static auto trampoline = [](int, void *stream, int bytes, void *udata)
    {
        auto s = (audio_streamer *)udata;

        // If there were still bytes from a previous conversion, copy them.
        if (s->m_extra_bytes)
        {
            int tocopy = lol::min(bytes, s->m_extra_bytes);
            memcpy(stream, s->m_convert.buf + s->m_convert.len_cvt - s->m_extra_bytes, tocopy);
            s->m_extra_bytes -= tocopy;
            bytes -= tocopy;
            stream = (void *)((uint8_t *)stream + tocopy);
        }

        // Ask the callback for more bytes as long as we need them.
        while (bytes > 0)
        {
            s->m_callback(s->m_convert.buf, s->m_convert.len);
            if (s->m_convert.needed)
                SDL_ConvertAudio(&s->m_convert);
            int tocopy = lol::min(bytes, s->m_convert.len_cvt);
            s->m_extra_bytes = s->m_convert.len_cvt - tocopy;
            memcpy(stream, s->m_convert.buf, tocopy);
            bytes -= tocopy;
            stream = (void *)((uint8_t *)stream + tocopy);
        }
    };

    auto s = std::make_shared<audio_streamer>();
    m_streamers.insert(s);

    // Build an audio converter that converts from a given streaming format
    // to the format SDL was currently initialised with, if necessary.
    Uint16 sdl_format = lol2sdl_format(format);
    Uint16 sdl_g_format = lol2sdl_format(g_format);
    SDL_BuildAudioCVT(&s->m_convert, sdl_format, channels, frequency, sdl_g_format, g_channels, g_frequency);

    // This is how many bytes we will ask the streaming callback
    s->m_convert.len = LOL_AUDIO_DEFAULT_FRAMES * channels * SDL_AUDIO_BITSIZE(sdl_format) / 8;
    s->m_convert_buffer.resize(s->m_convert.len * s->m_convert.len_mult);
    s->m_convert.buf = s->m_convert_buffer.data();

    // This is how many bytes we will send to the SDL mixer
    Uint32 output_bytes = LOL_AUDIO_DEFAULT_FRAMES * g_channels * SDL_AUDIO_BITSIZE(sdl_g_format) / 8;
    s->m_output_buffer.resize(output_bytes);
    Uint8* audio_data = (Uint8*)s->m_output_buffer.data();
    s->m_chunk = Mix_QuickLoad_RAW(audio_data, output_bytes);
    s->m_track = Mix_PlayChannel(-1, s->m_chunk, -1);
    s->m_callback = f;
    Mix_RegisterEffect(s->m_track, trampoline, nullptr, s.get());

    return s->m_track;
}

void audio::stop_streaming(int track)
{
    for (auto streamer : m_streamers)
    {
        if (streamer->m_track == track)
        {
            Mix_HaltChannel(track);
            m_streamers.erase(streamer);
            break;
        }
    }
}

#else
void audio::init() {}
void audio::set_tracks(int) {}
void audio::set_volume(int, int) {}
void audio::mute_all() {}
void audio::unmute_all() {}
int audio::start_streaming(std::function<void(void *, int)> const &) { return -1; }
void audio::stop_streaming(int) {}
#endif

} /* namespace lol */

