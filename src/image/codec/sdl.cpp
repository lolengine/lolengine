/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2008 Sam Hocevar <sam@zoy.org>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/*
 * sdl.c: SDL_image I/O functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_image.h>

#include "pipi.h"
#include "pipi-internals.h"

static int pipi_free_sdl(pipi_image_t *);
static SDL_Surface *create_32bpp_surface(int w, int h);

pipi_image_t *pipi_load_sdl(const char *name)
{
    pipi_image_t *img;
    SDL_Surface *priv = IMG_Load(name);

    if(!priv)
        return NULL;

    if(priv->format->BytesPerPixel != 4)
    {
        SDL_Surface *tmp = create_32bpp_surface(priv->w, priv->h);
        SDL_BlitSurface(priv, NULL, tmp, NULL);
        SDL_FreeSurface(priv);
        priv = tmp;
    }

    img = pipi_new(priv->w, priv->h);

    img->p[PIPI_PIXELS_RGBA_U8].pixels = priv->pixels;
    img->p[PIPI_PIXELS_RGBA_U8].w = priv->w;
    img->p[PIPI_PIXELS_RGBA_U8].h = priv->h;
    img->p[PIPI_PIXELS_RGBA_U8].pitch = priv->pitch;
    img->p[PIPI_PIXELS_RGBA_U8].bpp = 32;
    img->p[PIPI_PIXELS_RGBA_U8].bytes = 4 * img->w * img->h;
    img->last_modified = PIPI_PIXELS_RGBA_U8;

    img->codec_priv = (void *)priv;
    img->codec_format = PIPI_PIXELS_RGBA_U8;
    img->codec_free = pipi_free_sdl;

    img->wrap = 0;
    img->u8 = 1;

    return img;
}

int pipi_save_sdl(pipi_image_t *img, const char *name)
{
    if(!img->codec_priv)
    {
        SDL_Surface *priv = create_32bpp_surface(img->w, img->h);

        /* FIXME: check pitch differences here */
        if(img->last_modified == PIPI_PIXELS_RGBA_U8)
        {
            memcpy(priv->pixels, img->p[PIPI_PIXELS_RGBA_U8].pixels,
                   priv->pitch * priv->h);
            free(img->p[PIPI_PIXELS_RGBA_U8].pixels);
        }

        img->p[PIPI_PIXELS_RGBA_U8].pixels = priv->pixels;
        img->p[PIPI_PIXELS_RGBA_U8].w = priv->w;
        img->p[PIPI_PIXELS_RGBA_U8].h = priv->h;
        img->p[PIPI_PIXELS_RGBA_U8].pitch = priv->pitch;
        img->p[PIPI_PIXELS_RGBA_U8].bpp = 32;
        img->p[PIPI_PIXELS_RGBA_U8].bytes = 4 * img->w * img->h;

        img->codec_priv = (void *)priv;
        img->codec_format = PIPI_PIXELS_RGBA_U8;
        img->codec_free = pipi_free_sdl;

        img->wrap = 0;
        img->u8 = 1;
    }

    pipi_set_colorspace(img, img->codec_format);
    SDL_SaveBMP(img->codec_priv, name);

    return 0;
}

/*
 * XXX: The following functions are local.
 */

static int pipi_free_sdl(pipi_image_t *img)
{
    SDL_FreeSurface(img->codec_priv);

    return 0;
}

static SDL_Surface *create_32bpp_surface(int w, int h)
{
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif

    return SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
                                rmask, gmask, bmask, amask);
}

