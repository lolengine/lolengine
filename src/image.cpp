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
#include <cstdio>

#if defined USE_SDL
#   include <SDL.h>
#   include <SDL_image.h>
#endif

#include "core.h"
#include "lolgl.h"

namespace lol
{

/*
 * Image implementation class
 */

class ImageData
{
    friend class Image;

private:
    vec2i size;
    Image::format_t format;

#if defined USE_SDL
    SDL_Surface *img;
#endif
};

/*
 * Public Image class
 */

Image::Image(char const *path)
  : data(new ImageData())
{
#if defined USE_SDL
    for (char const *name = path; *name; name++)
        if ((data->img = IMG_Load(name)))
            break;

    if (!data->img)
    {
#if !LOL_RELEASE
        fprintf(stderr, "ERROR: could not load %s\n", path);
#endif
        SDL_Quit();
        exit(1);
    }

    data->size = vec2i(data->img->w, data->img->h);
    data->format = data->img->format->Amask ? FORMAT_RGBA : FORMAT_RGB;
#else
    data->size = 0;
    data->format = FORMAT_UNKNOWN;
#endif
}

vec2i Image::GetSize() const
{
    return data->size;
}

Image::format_t Image::GetFormat() const
{
    return data->format;
}

void * Image::GetData() const
{
    return data->img->pixels;
}

Image::~Image()
{
#if defined USE_LOL
    SDL_FreeSurface(data->img);
#endif
    delete data;
}

} /* namespace lol */

