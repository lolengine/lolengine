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
 * tile.c: Tiling function
 */

#include "config.h"

#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_tile(pipi_image_t *tile, int w, int h)
{
    pipi_image_t *dst;
    pipi_pixels_t *tilep, *dstp;
    float *tiledata, *dstdata;
    int x, y, tw, th, todo;

    tw = tile->w;
    th = tile->h;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    tilep = pipi_get_pixels(tile, PIPI_PIXELS_RGBA_F32);
    tiledata = (float *)tilep->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x += todo)
        {
            todo = (x + tw > w) ? w - x : tw;
            memcpy(dstdata + 4 * (y * w + x),
                   tiledata + 4 * (y % th) * tw,
                   4 * todo * sizeof(float));
        }
    }

    return dst;
}

