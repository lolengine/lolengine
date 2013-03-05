//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cstdio>
#include <cstring>

#if defined USE_SDL_MIXER
#   if defined HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#   if defined HAVE_SDL_SDL_MIXER_H
#      include <SDL/SDL_mixer.h>
#   else
#      include <SDL_mixer.h>
#   endif
#endif

#include "core.h"

using namespace std;

namespace lol
{

/*
 * Sample implementation class
 */

class SampleData
{
    friend class Sample;

private:
    String m_name;
#if defined USE_SDL_MIXER
    Mix_Chunk *m_chunk;
    int m_channel;
#endif
};

/*
 * Public Sample class
 */

Sample::Sample(char const *path)
  : data(new SampleData())
{
    data->m_name = String("<sample> ") + path;

#if defined USE_SDL_MIXER
    Array<String> pathlist = System::GetPathList(path);
    for (int i = 0; i < pathlist.Count(); ++i)
    {
        data->m_chunk = Mix_LoadWAV(pathlist[i].C());
        if (data->m_chunk)
            break;
    }
    if (!data->m_chunk)
    {
#if !LOL_RELEASE
        Log::Error("could not load sample %s\n", path);
#endif
        SDL_Quit();
        exit(1);
    }
    data->m_channel = -1;
#endif
}

Sample::~Sample()
{
#if defined USE_SDL_MIXER
    Mix_FreeChunk(data->m_chunk);
#endif
    delete data;
}

void Sample::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

char const *Sample::GetName()
{
    return data->m_name.C();
}

void Sample::Play()
{
#if defined USE_SDL_MIXER
    data->m_channel = Mix_PlayChannel(-1, data->m_chunk, 0);
#endif
}

void Sample::Loop()
{
#if defined USE_SDL_MIXER
    data->m_channel = Mix_PlayChannel(-1, data->m_chunk, -1);
#endif
}

void Sample::Stop()
{
#if defined USE_SDL_MIXER
    if (data->m_channel >= 0)
        Mix_HaltChannel(data->m_channel);
    data->m_channel = -1;
#endif
}

} /* namespace lol */

