//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
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
#   if HAVE_SDL_SDL_H
#       include <SDL/SDL.h>
#       include <SDL/SDL_mixer.h>
#   elif HAVE_SDL2_SDL_H
#       include <SDL2/SDL.h>
#       include <SDL2/SDL_mixer.h>
#   else
#       include <SDL.h>
#       include <SDL_mixer.h>
#   endif
#endif

namespace lol
{

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

sample::sample(char const *path)
  : data(new sample_data())
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
        msg::error("could not load sample %s: %s\n", path, Mix_GetError());
    }
    data->m_channel = -1;
#endif
}

sample::~sample()
{
#if defined LOL_USE_SDL_MIXER
    if (data->m_chunk)
        Mix_FreeChunk(data->m_chunk);
#endif
    delete data;
}

void sample::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

char const *sample::GetName()
{
    return data->m_name.c_str();
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

