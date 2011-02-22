//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cmath>

#if defined USE_SDL
#   include <SDL.h>
#   include <SDL_mixer.h>
#endif

#include "core.h"

namespace lol
{

/*
 * Public Audio class
 */

void Audio::Setup(int channels)
{
#if defined USE_SDL
    Mix_OpenAudio(22050, AUDIO_S16, channels, 1024);
#endif
}

} /* namespace lol */

