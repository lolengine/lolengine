//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

#if defined USE_SDL_MIXER
#   include <SDL.h>
#   include <SDL_mixer.h>
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
    char *name, *path;
#if defined USE_SDL_MIXER
    Mix_Chunk *chunk;
#endif
};

/*
 * Public Sample class
 */

Sample::Sample(char const *path)
  : data(new SampleData())
{
    data->name = (char *)malloc(9 + strlen(path) + 1);
    data->path = data->name + 9;
    sprintf(data->name, "<sample> %s", path);

#if defined USE_SDL_MIXER
    data->chunk = Mix_LoadWAV(path);
    if (!data->chunk)
    {
#if !LOL_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        SDL_Quit();
        exit(1);
    }
#endif
}

Sample::~Sample()
{
#if defined USE_SDL_MIXER
    Mix_FreeChunk(data->chunk);
#endif
    free(data->name);
    delete data;
}

void Sample::TickGame(float seconds)
{
    Entity::TickGame(seconds);
}

char const *Sample::GetName()
{
    return data->name;
}

void Sample::Play()
{
#if defined USE_SDL_MIXER
    Mix_PlayChannel(-1, data->chunk, 0);
#endif
}

} /* namespace lol */

