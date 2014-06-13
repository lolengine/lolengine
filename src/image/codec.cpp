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
 * codec.c: image I/O functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_load(char const *name)
{
    pipi_image_t *ret = NULL;

    if(!strncmp(name, "random:", 7) ||
       !strncmp(name, "ediff:", 6) ||
       !strncmp(name, "halftone:", 6) ||
       !strncmp(name, "bayer:", 6))
        ret = pipi_load_stock(name);

    if(!ret)
        ret = pipi_load_oric(name);

#if USE_JPEG
    if(!ret)
        ret = pipi_load_jpeg(name);
#endif
#if USE_IMLIB2
    if(!ret)
        ret = pipi_load_imlib2(name);
#endif
#if USE_OPENCV
    if(!ret)
        ret = pipi_load_opencv(name);
#endif
#if USE_SDL
    if(!ret)
        ret = pipi_load_sdl(name);
#endif
#if USE_GDIPLUS
    if(!ret)
        ret = pipi_load_gdiplus(name);
#endif
#if USE_GDI
    if(!ret)
        ret = pipi_load_gdi(name);
#endif
#if USE_COCOA
    if(!ret)
        ret = pipi_load_coreimage(name);
#endif
    return ret;
}

void pipi_free(pipi_image_t *img)
{
    int i;

    for(i = 0; i < PIPI_PIXELS_MAX; i++)
        if(i != img->codec_format && img->p[i].pixels)
            free(img->p[i].pixels);

    if(img->codec_priv)
        img->codec_free(img);

    free(img);
}

int pipi_save(pipi_image_t *img, const char *name)
{
    int ret = -1;

    if(ret < 0)
        ret = pipi_save_oric(img, name);

#if USE_JPEG
    if(ret < 0)
        ret = pipi_save_jpeg(img, name);
#endif
#if USE_IMLIB2
    if(ret < 0)
        ret = pipi_save_imlib2(img, name);
#endif
#if USE_OPENCV
    if(ret < 0)
        ret = pipi_save_opencv(img, name);
#endif
#if USE_SDL
    if(ret < 0)
        ret = pipi_save_sdl(img, name);
#endif
#if USE_GDIPLUS
    if(ret < 0)
        ret = pipi_save_gdiplus(img, name);
#endif
#if USE_GDI
    if(ret < 0)
        ret = pipi_save_gdi(img, name);
#endif
#if USE_COCOA
    if(ret < 0)
        ret = pipi_save_coreimage(img, name);
#endif

    return ret;
}

