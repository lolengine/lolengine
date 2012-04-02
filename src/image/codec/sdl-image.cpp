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

#if defined USE_SDL_IMAGE

#include <cmath>

#include <SDL.h>
#include <SDL_image.h>

#include "core.h"
#include "../../image/image-private.h"

using namespace std;

namespace lol
{

/*
 * Image implementation class
 */

DECLARE_IMAGE_LOADER(SdlImageData, 50)
{
public:
    virtual bool Open(char const *);
    virtual bool Close();

    virtual void *GetData() const;

    static SDL_Surface *Create32BppSurface(ivec2 size);

private:
    SDL_Surface *img;
};

/*
 * Public Image class
 */

bool SdlImageData::Open(char const *path)
{
    for (char const *name = path; *name; name++)
        if ((img = IMG_Load(name)))
            break;

    if (!img)
    {
#if !LOL_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        return false;
    }

    size = ivec2(img->w, img->h);

    if (img->format->BytesPerPixel != 4)
    {
        SDL_Surface *tmp = Create32BppSurface(size);
        SDL_BlitSurface(img, NULL, tmp, NULL);
        SDL_FreeSurface(img);
        img = tmp;
    }

    format = img->format->Amask ? Image::FORMAT_RGBA : Image::FORMAT_RGB;

    return true;
}

bool SdlImageData::Close()
{
    SDL_FreeSurface(img);

    return true;
}

void * SdlImageData::GetData() const
{
    return img->pixels;
}

SDL_Surface *SdlImageData::Create32BppSurface(ivec2 size)
{
    uint32_t rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    return SDL_CreateRGBSurface(SDL_SWSURFACE, size.x, size.y, 32,
                                rmask, gmask, bmask, amask);
}

} /* namespace lol */

#endif /* defined USE_SDL_IMAGE */

