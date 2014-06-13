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
 * minmax.c: min and max computation functions
 */

#include "config.h"

#include <stdlib.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_min(pipi_image_t *img1, pipi_image_t *img2)
{
    pipi_image_t *dst;
    pipi_pixels_t *img1p, *img2p, *dstp;
    float *img1data, *img2data, *dstdata;
    int x, y, w, h;

    if(img1->w != img2->w || img1->h != img2->h)
        return NULL;

    w = img1->w;
    h = img1->h;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    img1p = pipi_get_pixels(img1, PIPI_PIXELS_RGBA_F32);
    img1data = (float *)img1p->pixels;
    img2p = pipi_get_pixels(img2, PIPI_PIXELS_RGBA_F32);
    img2data = (float *)img2p->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            float p, q;

            p = img1data[4 * (y * w + x)];
            q = img2data[4 * (y * w + x)];
            dstdata[4 * (y * w + x)] = p < q ? p : q;

            p = img1data[4 * (y * w + x) + 1];
            q = img2data[4 * (y * w + x) + 1];
            dstdata[4 * (y * w + x) + 1] = p < q ? p : q;

            p = img1data[4 * (y * w + x) + 2];
            q = img2data[4 * (y * w + x) + 2];
            dstdata[4 * (y * w + x) + 2] = p < q ? p : q;

            p = img1data[4 * (y * w + x) + 3];
            q = img2data[4 * (y * w + x) + 3];
            dstdata[4 * (y * w + x) + 3] = p < q ? p : q;
        }
    }

    return dst;
}

pipi_image_t *pipi_max(pipi_image_t *img1, pipi_image_t *img2)
{
    pipi_image_t *dst;
    pipi_pixels_t *img1p, *img2p, *dstp;
    float *img1data, *img2data, *dstdata;
    int x, y, w, h;

    if(img1->w != img2->w || img1->h != img2->h)
        return NULL;

    w = img1->w;
    h = img1->h;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    img1p = pipi_get_pixels(img1, PIPI_PIXELS_RGBA_F32);
    img1data = (float *)img1p->pixels;
    img2p = pipi_get_pixels(img2, PIPI_PIXELS_RGBA_F32);
    img2data = (float *)img2p->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            float p, q;

            p = img1data[4 * (y * w + x)];
            q = img2data[4 * (y * w + x)];
            dstdata[4 * (y * w + x)] = p > q ? p : q;

            p = img1data[4 * (y * w + x) + 1];
            q = img2data[4 * (y * w + x) + 1];
            dstdata[4 * (y * w + x) + 1] = p > q ? p : q;

            p = img1data[4 * (y * w + x) + 2];
            q = img2data[4 * (y * w + x) + 2];
            dstdata[4 * (y * w + x) + 2] = p > q ? p : q;

            p = img1data[4 * (y * w + x) + 3];
            q = img2data[4 * (y * w + x) + 3];
            dstdata[4 * (y * w + x) + 3] = p > q ? p : q;
        }
    }

    return dst;
}

