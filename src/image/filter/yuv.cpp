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
 * yuv.c: YUV conversion functions
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_rgb2yuv(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h;

    w = src->w;
    h = src->h;

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double r, g, b, a, yp, u, v;
            int d = 4 * (y * w + x);

            r = srcdata[d];
            g = srcdata[d + 1];
            b = srcdata[d + 2];
            a = srcdata[d + 3];

            yp = 0.299 * r + 0.587 * g + 0.114 * b;
            yp = (yp * 220.0 + 16.0) / 255.0;

            u = 0.5 - 0.14713 * r - 0.28886 * g + 0.436 * b;
            if (u < 0.0) u = 0.0;
            if (u > 1.0) u = 1.0;

            v = 0.5 + 0.615 * r - 0.51499 * g - 0.10001 * b;
            if (v < 0.0) v = 0.0;
            if (v > 1.0) v = 1.0;

            dstdata[d] = yp;
            dstdata[d + 1] = u;
            dstdata[d + 2] = v;
            dstdata[d + 3] = a;
        }
    }

    return dst;
}

pipi_image_t *pipi_yuv2rgb(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h;

    w = src->w;
    h = src->h;

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double r, g, b, a, yp, u, v;
            int d = 4 * (y * w + x);

            yp = (srcdata[d] * 255.0 - 16.0) / 220.0;
            u = srcdata[d + 1] - 0.5;
            v = srcdata[d + 2] - 0.5;
            a = srcdata[d + 3];

            r = yp + 1.13983 * v;
            if (r < 0.0) r = 0.0;
            if (r > 1.0) r = 1.0;
            g = yp - 0.39465 * u - 0.58060 * v;
            if (g < 0.0) g = 0.0;
            if (g > 1.0) g = 1.0;
            b = yp + 2.03211 * u;
            if (b < 0.0) b = 0.0;
            if (b > 1.0) b = 1.0;

            dstdata[d] = r;
            dstdata[d + 1] = g;
            dstdata[d + 2] = b;
            dstdata[d + 3] = a;
        }
    }

    return dst;
}

