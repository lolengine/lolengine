/*
 *  Lol Engine
 *
 *  Copyright © 2004—2008 Sam Hocevar <sam@hocevar.net>
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What the Fuck You Want
 *  to Public License, Version 2, as published by the WTFPL Task Force.
 *  See http://www.wtfpl.net/ for more details.
 */

/*
 * transform.c: basic transformation functions
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_hflip(pipi_image_t *src)
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
                dstdata[y * w + x] = srcdata[y * w + w - 1 - x];
            }
            else
            {
                dstdata[4 * (y * w + x)]
                                = srcdata[4 * (y * w + w - 1 - x)];
                dstdata[4 * (y * w + x) + 1]
                                = srcdata[4 * (y * w + w - 1 - x) + 1];
                dstdata[4 * (y * w + x) + 2]
                                = srcdata[4 * (y * w + w - 1 - x) + 2];
                dstdata[4 * (y * w + x) + 3]
                                = srcdata[4 * (y * w + w - 1 - x) + 3];
            }
        }
    }

    return dst;
}

pipi_image_t *pipi_vflip(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int y, w, h, gray;

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
        if(gray)
        {
            memcpy(dstdata + y * w,
                   srcdata + (h - 1 - y) * w,
                   w * sizeof(*dstdata));
        }
        else
        {
            memcpy(dstdata + 4 * y * w,
                   srcdata + 4 * (h - 1 - y) * w,
                   4 * w * sizeof(*dstdata));
        }
    }

    return dst;
}

pipi_image_t *pipi_rotate90(pipi_image_t *src)
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

    dst = pipi_new(h, w);
    dstp = gray ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            if(gray)
            {
                dstdata[x * h + y] = srcdata[y * w + w - 1 - x];
            }
            else
            {
                dstdata[4 * (x * h + y)]
                    = srcdata[4 * (y * w + w - 1 - x)];
                dstdata[4 * (x * h + y) + 1]
                    = srcdata[4 * (y * w + w - 1 - x) + 1];
                dstdata[4 * (x * h + y) + 2]
                    = srcdata[4 * (y * w + w - 1 - x) + 2];
                dstdata[4 * (x * h + y) + 3]
                    = srcdata[4 * (y * w + w - 1 - x) + 3];
            }
        }
    }

    return dst;
}

pipi_image_t *pipi_rotate180(pipi_image_t *src)
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
                dstdata[y * w + x] = srcdata[(h - 1 - y) * w + (w - 1 - x)];
            }
            else
            {
                dstdata[4 * (y * w + x)]
                     = srcdata[4 * ((h - 1 - y) * w + (w - 1 - x))];
                dstdata[4 * (y * w + x) + 1]
                     = srcdata[4 * ((h - 1 - y) * w + (w - 1 - x)) + 1];
                dstdata[4 * (y * w + x) + 2]
                     = srcdata[4 * ((h - 1 - y) * w + (w - 1 - x)) + 2];
                dstdata[4 * (y * w + x) + 3]
                     = srcdata[4 * ((h - 1 - y) * w + (w - 1 - x)) + 3];
            }
        }
    }

    return dst;
}

pipi_image_t *pipi_rotate270(pipi_image_t *src)
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

    dst = pipi_new(h, w);
    dstp = gray ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            if(gray)
            {
                dstdata[x * h + h - 1 - y] = srcdata[y * w + x];
            }
            else
            {
                dstdata[4 * (x * h + h - 1 - y)]
                    = srcdata[4 * (y * w + x)];
                dstdata[4 * (x * h + h - 1 - y) + 1]
                    = srcdata[4 * (y * w + x) + 1];
                dstdata[4 * (x * h + h - 1 - y) + 2]
                    = srcdata[4 * (y * w + x) + 2];
                dstdata[4 * (x * h + h - 1 - y) + 3]
                    = srcdata[4 * (y * w + x) + 3];
            }
        }
    }

    return dst;
}

