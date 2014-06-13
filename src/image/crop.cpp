/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2009 Sam Hocevar <sam@hocevar.net>
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
 * crop.c: image cropping functions
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_crop(pipi_image_t *src, int w, int h, int dx, int dy)
{
    float *srcdata, *dstdata;
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    int y, off, len;

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    off = dx;
    len = w;

    if(dx < 0)
    {
        len += dx;
        dx = 0;
    }

    if(dx + len > srcp->w)
        len = srcp->w - dx;

    if(len > 0)
    {
        for(y = 0; y < h; y++)
        {
            if(y + dy < 0 || y + dy >= srcp->h)
                continue;

            memcpy(dstdata + y * w * 4,
                   srcdata + ((y + dy) * srcp->w + dx) * 4,
                   len * 4 * sizeof(float));
        }
    }

    return dst;
}

