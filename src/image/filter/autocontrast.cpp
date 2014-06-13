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
 * autocontrast.c: autocontrast functions
 * TODO: the current approach is naive; we should use the histogram in order
 * to decide how to change the contrast.
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_autocontrast(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    float min = 1.0, max = 0.0, t;
    int x, y, w, h, gray;

    w = src->w;
    h = src->h;

    gray = (src->last_modified == PIPI_PIXELS_Y_F32);

    srcp = gray ? pipi_get_pixels(src, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            if(gray)
            {
                if(srcdata[y * w + x] < min)
                    min = srcdata[y * w + x];
                if(srcdata[y * w + x] > max)
                    max = srcdata[y * w + x];
            }
            else
            {
                if(srcdata[4 * (y * w + x)] < min)
                    min = srcdata[4 * (y * w + x)];
                if(srcdata[4 * (y * w + x)] > max)
                    max = srcdata[4 * (y * w + x)];
                if(srcdata[4 * (y * w + x) + 1] < min)
                    min = srcdata[4 * (y * w + x) + 1];
                if(srcdata[4 * (y * w + x) + 1] > max)
                    max = srcdata[4 * (y * w + x) + 1];
                if(srcdata[4 * (y * w + x) + 2] < min)
                    min = srcdata[4 * (y * w + x) + 2];
                if(srcdata[4 * (y * w + x) + 2] > max)
                    max = srcdata[4 * (y * w + x) + 2];
            }
        }
    }

    if(min >= max)
        return pipi_copy(src);

    t = 1. / (max - min);

    dst = pipi_new(w, h);
    dstp = gray ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            if(gray)
            {
                dstdata[y * w + x] = (srcdata[y * w + x] - min) * t;
            }
            else
            {
                dstdata[4 * (y * w + x)]
                    = (srcdata[4 * (y * w + x)] - min) * t;
                dstdata[4 * (y * w + x) + 1]
                    = (srcdata[4 * (y * w + x) + 1] - min) * t;
                dstdata[4 * (y * w + x) + 2]
                    = (srcdata[4 * (y * w + x) + 2] - min) * t;
                dstdata[4 * (y * w + x) + 3]
                    = srcdata[4 * (y * w + x) + 3];
            }
        }
    }

    return dst;
}

