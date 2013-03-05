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

DECLARE_IMAGE_LOADER(SdlImageData, 50)
{
public:
    virtual bool Open(char const *);
    virtual bool Close();

    virtual uint8_t *GetData() const;

    static SDL_Surface *Create32BppSurface(ivec2 size);

private:
    SDL_Surface *m_img;
};

/*
 * Public Image class
 */

bool SdlImageData::Open(char const *path)
{
    Array<String> pathlist = System::GetPathList(path);
    for (int i = 0; i < pathlist.Count(); i++)
    {
        m_img = IMG_Load(pathlist[i].C());
        if (m_img)
            break;
    }

    if (!m_img)
    {
#if !LOL_RELEASE
        Log::Error("could not load image %s\n", path);
#endif
        return false;
    }

    m_size = ivec2(m_img->w, m_img->h);

    if (m_img->format->BytesPerPixel != 4)
    {
        SDL_Surface *tmp = Create32BppSurface(m_size);
        SDL_BlitSurface(m_img, nullptr, tmp, nullptr);
        SDL_FreeSurface(m_img);
        m_img = tmp;
    }

    m_format = m_img->format->Amask ? PixelFormat::RGBA_8
                                    : PixelFormat::RGB_8;

    return true;
}

bool SdlImageData::Close()
{
    SDL_FreeSurface(m_img);

    return true;
}

uint8_t * SdlImageData::GetData() const
{
    return (uint8_t *)m_img->pixels;
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

