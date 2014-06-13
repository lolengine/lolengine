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
 * random.c: random dithering functions
 */

#include "config.h"

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_dither_random(pipi_image_t *img)
{
    pipi_image_t *dst;
    pipi_pixels_t *dstp;
    float *dstdata;
    unsigned int ctx = 1;
    int x, y, w, h;

    w = img->w;
    h = img->h;

    dst = pipi_copy(img);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_Y_F32);
    dstdata = (float *)dstp->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            long hi, lo;
            float p, q;

            hi = ctx / 12773L;
            lo = ctx % 12773L;
            ctx = 16807L * lo - 2836L * hi;
            if(ctx <= 0)
                ctx += 0x7fffffffL;

            p = dstdata[y * w + x];
            q = p > (double)((ctx % 65536) / 65535.) ? 1. : 0.;
            dstdata[y * w + x] = q;
        }
    }

    return dst;
}

