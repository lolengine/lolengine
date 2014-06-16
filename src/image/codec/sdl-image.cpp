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

#if defined USE_SDL_IMAGE

#if defined HAVE_SDL_SDL_H
#   include <SDL/SDL.h>
#else
#   include <SDL.h>
#endif
#if defined HAVE_SDL_SDL_IMAGE_H
#   include <SDL/SDL_image.h>
#else
#   include <SDL_image.h>
#endif

#include "core.h"
#include "../../image/image-private.h"

using namespace std;

namespace lol
{

/*
 * Image implementation class
 */

class SdlImageCodec : public ImageCodec
{
public:
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);

    static SDL_Surface *Create32BppSurface(ivec2 size);
};

DECLARE_IMAGE_CODEC(SdlImageCodec, 50)

/*
 * Public Image class
 */

bool SdlImageCodec::Load(Image *image, char const *path)
{
    SDL_Surface *surface = nullptr;

    Array<String> pathlist = System::GetPathList(path);
    for (int i = 0; i < pathlist.Count(); i++)
    {
        surface = IMG_Load(pathlist[i].C());
        if (surface)
            break;
    }

    if (!surface)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("could not load image %s\n", path);
#endif
        return false;
    }

    ivec2 size(surface->w, surface->h);

    if (surface->format->BytesPerPixel != 4)
    {
        SDL_Surface *tmp = Create32BppSurface(size);
        SDL_BlitSurface(surface, nullptr, tmp, nullptr);
        SDL_FreeSurface(surface);
        surface = tmp;
    }

    image->SetSize(size);
    u8vec4 *data = image->Lock<PixelFormat::RGBA_8>();
    memcpy(data, surface->pixels, 4 * size.x * size.y);
    image->Unlock();

    SDL_FreeSurface(surface);

    return true;
}

bool SdlImageCodec::Save(Image *image, char const *path)
{
    ivec2 size = image->GetSize();
    SDL_Surface *surface = Create32BppSurface(size);

    u8vec4 *data = image->Lock<PixelFormat::RGBA_8>();
    memcpy(surface->pixels, data, 4 * size.x * size.y);
    image->Unlock();

    int ret = SDL_SaveBMP(surface, path);
    SDL_FreeSurface(surface);

    return ret == 0;
}

SDL_Surface *SdlImageCodec::Create32BppSurface(ivec2 size)
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

