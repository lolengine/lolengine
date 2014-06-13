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
 * color.c: colour manipulation functions
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_brightness(pipi_image_t *src, double val)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h, gray;

    w = src->w;
    h = src->h;

    gray = (src->last_modified == PIPI_PIXELS_Y_F32);

    srcp = gray ? pipi_get_pixels(src, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = gray ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    if(val >= 0.0)
    {
        for(y = 0; y < h; y++)
        {
            for(x = 0; x < w; x++)
            {
                if(gray)
                {
                    double p = srcdata[y * w + x];
                    dstdata[y * w + x] = p < 1. - val ? p + val : 1.;
                }
                else
                {
                    double p;
                    int d = 4 * (y * w + x);

                    p = srcdata[d];
                    dstdata[d] = p < 1. - val ? p + val : 1.;
                    p = srcdata[d + 1];
                    dstdata[d + 1] = p < 1. - val ? p + val : 1.;
                    p = srcdata[d + 2];
                    dstdata[d + 2] = p < 1. - val ? p + val : 1.;
                    p = srcdata[d + 3];
                    dstdata[d + 3] = p;
                }
            }
        }
    }
    else
    {
        for(y = 0; y < h; y++)
        {
            for(x = 0; x < w; x++)
            {
                if(gray)
                {
                    double p = srcdata[y * w + x];
                    dstdata[y * w + x] = p > -val ? p + val : 0.;
                }
                else
                {
                    double p;
                    int d = 4 * (y * w + x);

                    p = srcdata[d];
                    dstdata[d] = p > -val ? p + val : 0.;
                    p = srcdata[d + 1];
                    dstdata[d + 1] = p > -val ? p + val : 0.;
                    p = srcdata[d + 2];
                    dstdata[d + 2] = p > -val ? p + val : 0.;
                    p = srcdata[d + 3];
                    dstdata[d + 3] = p;
                }
            }
        }
    }

    return dst;
}

pipi_image_t *pipi_contrast(pipi_image_t *src, double val)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h, gray;

    w = src->w;
    h = src->h;

    gray = (src->last_modified == PIPI_PIXELS_Y_F32);

    srcp = gray ? pipi_get_pixels(src, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = gray ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    if(val >= 0.0)
    {
        if(val > 0.99999)
            val = 0.99999;

        val = 1. / (1. - val);

        for(y = 0; y < h; y++)
        {
            for(x = 0; x < w; x++)
            {
                if(gray)
                {
                    double p = (srcdata[y * w + x] - 0.5) * val + 0.5;
                    dstdata[y * w + x] = p < 0. ? 0. : p > 1. ? 1. : p;
                }
                else
                {
                    double p;
                    int d = 4 * (y * w + x);

                    p = (srcdata[d] - 0.5) * val + 0.5;
                    dstdata[d] = p < 0. ? 0. : p > 1. ? 1. : p;
                    p = (srcdata[d + 1] - 0.5) * val + 0.5;
                    dstdata[d + 1] = p < 0. ? 0. : p > 1. ? 1. : p;
                    p = (srcdata[d + 2] - 0.5) * val + 0.5;
                    dstdata[d + 2] = p < 0. ? 0. : p > 1. ? 1. : p;
                    p = srcdata[d + 3];
                    dstdata[d + 3] = p;
                }
            }
        }
    }
    else
    {
        if(val < -1.)
            val = -1.;

        val = 1. + val;

        for(y = 0; y < h; y++)
        {
            for(x = 0; x < w; x++)
            {
                if(gray)
                {
                    double p = srcdata[y * w + x];
                    dstdata[y * w + x] = (p - 0.5) * val + 0.5;
                }
                else
                {
                    double p;
                    int d = 4 * (y * w + x);

                    p = srcdata[d];
                    dstdata[d] = (p - 0.5) * val + 0.5;
                    p = srcdata[d + 1];
                    dstdata[d + 1] = (p - 0.5) * val + 0.5;
                    p = srcdata[d + 2];
                    dstdata[d + 2] = (p - 0.5) * val + 0.5;
                    p = srcdata[d + 3];
                    dstdata[d + 3] = p;
                }
            }
        }
    }

    return dst;
}

pipi_image_t *pipi_invert(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h, gray;

    w = src->w;
    h = src->h;

    gray = (src->last_modified == PIPI_PIXELS_Y_F32);

    srcp = gray ? pipi_get_pixels(src, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

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
                dstdata[y * w + x] = 1. - srcdata[y * w + x];
            }
            else
            {
                int d = 4 * (y * w + x);

                dstdata[d] = 1. - srcdata[d];
                dstdata[d + 1] = 1. - srcdata[d + 1];
                dstdata[d + 2] = 1. - srcdata[d + 2];
                dstdata[d + 3] = 1. - srcdata[d + 3];
            }
        }
    }

    return dst;
}

pipi_image_t *pipi_threshold(pipi_image_t *src, double val)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h, gray;

    w = src->w;
    h = src->h;

    gray = (src->last_modified == PIPI_PIXELS_Y_F32);

    srcp = gray ? pipi_get_pixels(src, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

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
                dstdata[y * w + x] = srcdata[y * w + x] < val ? 0. : 1.;
            }
            else
            {
                int d = 4 * (y * w + x);

                dstdata[d] = srcdata[d] < val ? 0. : 1.;
                dstdata[d + 1] = srcdata[d + 1] < val ? 0. : 1.;
                dstdata[d + 2] = srcdata[d + 2] < val ? 0. : 1.;
                dstdata[d + 3] = srcdata[d + 3] < val ? 0. : 1.;
            }
        }
    }

    return dst;
}

