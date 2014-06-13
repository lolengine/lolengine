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
 * imlib.c: ImLib I/O functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Imlib2.h>

#include "pipi.h"
#include "pipi-internals.h"

static int pipi_free_imlib2(pipi_image_t *);

pipi_image_t *pipi_load_imlib2(const char *name)
{
    pipi_image_t *img;
    Imlib_Image priv = imlib_load_image(name);

    if(!priv)
        return NULL;

    imlib_context_set_image(priv);

    if(!imlib_image_get_data())
    {
        imlib_free_image();
        return NULL;
    }

    img = pipi_new(imlib_image_get_width(), imlib_image_get_height());

    img->p[PIPI_PIXELS_RGBA_U8].pixels = imlib_image_get_data();
    img->p[PIPI_PIXELS_RGBA_U8].w = img->w;
    img->p[PIPI_PIXELS_RGBA_U8].h = img->h;
    img->p[PIPI_PIXELS_RGBA_U8].pitch = 4 * img->w;
    img->p[PIPI_PIXELS_RGBA_U8].bpp = 32;
    img->p[PIPI_PIXELS_RGBA_U8].bytes = 4 * img->w * img->h;
    img->last_modified = PIPI_PIXELS_RGBA_U8;

    img->codec_priv = (void *)priv;
    img->codec_format = PIPI_PIXELS_RGBA_U8;
    img->codec_free = pipi_free_imlib2;

    img->wrap = 0;
    img->u8 = 1;

    return img;
}

int pipi_save_imlib2(pipi_image_t *img, const char *name)
{
    if(!img->codec_priv)
    {
        Imlib_Image priv = imlib_create_image(img->w, img->h);
        void *data;

        imlib_context_set_image(priv);
        imlib_image_set_has_alpha(1);
        data = imlib_image_get_data();

        /* FIXME: check pitch differences here */
        if(img->last_modified == PIPI_PIXELS_RGBA_U8)
        {
            memcpy(data, img->p[PIPI_PIXELS_RGBA_U8].pixels,
                   4 * img->w * img->h);
            free(img->p[PIPI_PIXELS_RGBA_U8].pixels);
        }

        img->p[PIPI_PIXELS_RGBA_U8].pixels = data;
        img->p[PIPI_PIXELS_RGBA_U8].w = imlib_image_get_width();
        img->p[PIPI_PIXELS_RGBA_U8].h = imlib_image_get_height();
        img->p[PIPI_PIXELS_RGBA_U8].pitch = 4 * imlib_image_get_width();
        img->p[PIPI_PIXELS_RGBA_U8].bpp = 32;
        img->p[PIPI_PIXELS_RGBA_U8].bytes = 4 * img->w * img->h;

        img->codec_priv = (void *)priv;
        img->codec_format = PIPI_PIXELS_RGBA_U8;
        img->codec_free = pipi_free_imlib2;

        img->wrap = 0;
        img->u8 = 1;
    }

    pipi_set_colorspace(img, img->codec_format);
    imlib_context_set_image(img->codec_priv);
    imlib_save_image(name);

    return 0;
}

/*
 * XXX: The following functions are local.
 */

static int pipi_free_imlib2(pipi_image_t *img)
{
    imlib_context_set_image(img->codec_priv);
    imlib_free_image();

    return 0;
}

