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
 * rgb.c: RGB combining function
 */

#include "config.h"

#include <stdlib.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_rgb(pipi_image_t *i1, pipi_image_t *i2, pipi_image_t *i3)
{
    pipi_image_t *dst;
    pipi_pixels_t *i1p, *i2p, *i3p, *dstp;
    float *i1data, *i2data, *i3data, *dstdata;
    int x, y, w, h;

    if(i1->w != i2->w || i1->h != i2->h || i1->w != i3->w || i1->h != i3->h)
        return NULL;

    w = i1->w;
    h = i1->h;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    i1p = pipi_get_pixels(i1, PIPI_PIXELS_Y_F32);
    i1data = (float *)i1p->pixels;
    i2p = pipi_get_pixels(i2, PIPI_PIXELS_Y_F32);
    i2data = (float *)i2p->pixels;
    i3p = pipi_get_pixels(i3, PIPI_PIXELS_Y_F32);
    i3data = (float *)i3p->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            dstdata[4 * (y * w + x)] = i1data[y * w + x];
            dstdata[4 * (y * w + x) + 1] = i2data[y * w + x];
            dstdata[4 * (y * w + x) + 2] = i3data[y * w + x];
            dstdata[4 * (y * w + x) + 3] = 1.0;
        }
    }

    return dst;
}

pipi_image_t *pipi_red(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h;

    w = src->w;
    h = src->h;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_Y_F32);
    dstdata = (float *)dstp->pixels;

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    for(y = 0; y < h; y++)
        for(x = 0; x < w; x++)
            dstdata[y * w + x] = srcdata[4 * (y * w + x)];

    return dst;
}

pipi_image_t *pipi_green(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h;

    w = src->w;
    h = src->h;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_Y_F32);
    dstdata = (float *)dstp->pixels;

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    for(y = 0; y < h; y++)
        for(x = 0; x < w; x++)
            dstdata[y * w + x] = srcdata[4 * (y * w + x) + 1];

    return dst;
}

pipi_image_t *pipi_blue(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h;

    w = src->w;
    h = src->h;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_Y_F32);
    dstdata = (float *)dstp->pixels;

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    for(y = 0; y < h; y++)
        for(x = 0; x < w; x++)
            dstdata[y * w + x] = srcdata[4 * (y * w + x) + 2];

    return dst;
}

