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

#include <cstdlib>
#include <cstdio>
#include <cstring>

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

/* The sample cache */
static entity_dict<sample> sample_cache;

/*
 * sample implementation class
 */

class sample_data
{
    friend class sample;

private:
    std::string m_name;
#if defined LOL_USE_SDL_MIXER
    Mix_Chunk *m_chunk;
    int m_channel;
#endif
};

/*
 * Public sample class
 */

sample *sample::create(std::string const &path)
{
    auto ret = sample_cache.get(path);
    return ret ? ret : sample_cache.set(path, new sample(path));
}

sample *sample::create(void const *samples, size_t len)
{
    return new sample(samples, len);
}

void sample::destroy(sample *s)
{
    // FIXME: decrement!
    sample_cache.erase(s);
}

sample::sample(std::string const &path)
  : data(std::make_unique<sample_data>())
{
    data->m_name = std::string("<sample> ") + path;

#if defined LOL_USE_SDL_MIXER
    for (auto candidate : sys::get_path_list(path))
    {
        data->m_chunk = Mix_LoadWAV(candidate.c_str());
        if (data->m_chunk)
            break;
    }
    if (!data->m_chunk)
    {
        msg::error("could not load sample %s: %s\n", path.c_str(), Mix_GetError());
    }
    data->m_channel = -1;
#endif
}

sample::sample(void const *samples, size_t len)
  : data(std::make_unique<sample_data>())
{
    data->m_name = std::string("<sample>");

#if defined LOL_USE_SDL_MIXER
    data->m_chunk = Mix_QuickLoad_RAW((Uint8 *)samples, (Uint32)len);
    data->m_channel = -1;
#endif
}

sample::~sample()
{
#if defined LOL_USE_SDL_MIXER
    if (data->m_chunk)
        Mix_FreeChunk(data->m_chunk);
#endif
}

void sample::tick_game(float seconds)
{
    entity::tick_game(seconds);
}

std::string sample::GetName() const
{
    return data->m_name;
}

void sample::play()
{
#if defined LOL_USE_SDL_MIXER
    if (data->m_chunk)
        data->m_channel = Mix_PlayChannel(-1, data->m_chunk, 0);
#endif
}

void sample::loop()
{
#if defined LOL_USE_SDL_MIXER
    if (data->m_chunk)
        data->m_channel = Mix_PlayChannel(-1, data->m_chunk, -1);
#endif
}

void sample::stop()
{
#if defined LOL_USE_SDL_MIXER
    if (data->m_channel >= 0)
        Mix_HaltChannel(data->m_channel);
    data->m_channel = -1;
#endif
}

} /* namespace lol */

