//
//  Lol Engine
//
//  Copyright © 2010—2016 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if LOL_USE_SDL_IMAGE

#if HAVE_SDL_SDL_H
#   include <SDL/SDL.h>
#   include <SDL/SDL_image.h>
#elif HAVE_SDL2_SDL_H
#   include <SDL2/SDL.h>
#   include <SDL2/SDL_image.h>
#else
#   include <SDL.h>
#   include <SDL_image.h>
#endif

#include "../../image/resource-private.h"

namespace lol
{

/*
 * Image implementation class
 */

class SdlImageCodec : public ResourceCodec
{
public:
    virtual char const *GetName() { return "<SdlImageCodec>"; }
    virtual ResourceCodecData* Load(char const *path);
    virtual bool Save(char const *path, ResourceCodecData* data);

    static SDL_Surface *Create32BppSurface(ivec2 size);
};

DECLARE_IMAGE_CODEC(SdlImageCodec, 50)

ResourceCodecData* SdlImageCodec::Load(char const *path)
{
    SDL_Surface *surface = nullptr;

    for (auto candidate : sys::get_path_list(path))
    {
        surface = IMG_Load(candidate.C());
        if (surface)
            break;
    }

    if (!surface)
    {
#if !LOL_BUILD_RELEASE
        msg::error("could not load image %s\n", path);
#endif
        return nullptr;
    }

    ivec2 size(surface->w, surface->h);

    if (surface->format->BytesPerPixel != 4)
    {
        SDL_Surface *tmp = Create32BppSurface(size);
        SDL_BlitSurface(surface, nullptr, tmp, nullptr);
        SDL_FreeSurface(surface);
        surface = tmp;
    }

    auto data = new ResourceImageData(new Image(size));
    auto image = data->m_image;
    u8vec4 *pixel_data = image->Lock<PixelFormat::RGBA_8>();
    memcpy(pixel_data, surface->pixels, 4 * size.x * size.y);
    image->Unlock(pixel_data);

    SDL_FreeSurface(surface);

    return data;
}

bool SdlImageCodec::Save(char const *path, ResourceCodecData* data)
{
    auto data_image = dynamic_cast<ResourceImageData*>(data);
    if (data_image == nullptr)
        return false;

    auto image = data_image->m_image;
    ivec2 size = image->GetSize();
    SDL_Surface *surface = Create32BppSurface(size);

    u8vec4 *pixel_data = image->Lock<PixelFormat::RGBA_8>();
    memcpy(surface->pixels, pixel_data, 4 * size.x * size.y);
    image->Unlock(pixel_data);

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

#endif /* LOL_USE_SDL_IMAGE */

