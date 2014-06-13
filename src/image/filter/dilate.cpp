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
 * dilate.c: dilate and erode functions
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

/* FIXME: these functions are almost the same, try to merge them
 * somewhat efficiently. */
/* TODO: - dilate by k (Manhattan distance)
 *       - dilate by r (euclidian distance, with non-integer r) */
pipi_image_t *pipi_dilate(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h, i, gray;

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
            double t;
            int x2, y2, x3, y3;

            y2 = y - 1;
            if(y2 < 0) y2 = h - 1;
            y3 = y + 1;
            if(y3 >= h) y3 = 0;

            x2 = x - 1;
            if(x2 < 0) x2 = w - 1;
            x3 = x + 1;
            if(x3 >= w) x3 = 0;

            if(gray)
            {
                t = srcdata[y * w + x];
                if(srcdata[y2 * w + x] > t) t = srcdata[y2 * w + x];
                if(srcdata[y3 * w + x] > t) t = srcdata[y3 * w + x];
                if(srcdata[y * w + x2] > t) t = srcdata[y * w + x2];
                if(srcdata[y * w + x3] > t) t = srcdata[y * w + x3];
                dstdata[y * w + x] = t;
            }
            else
            {
                for(i = 0; i < 4; i++)
                {
                    t = srcdata[4 * (y * w + x) + i];
                    if(srcdata[4 * (y2 * w + x) + i] > t)
                        t = srcdata[4 * (y2 * w + x) + i];
                    if(srcdata[4 * (y3 * w + x) + i] > t)
                        t = srcdata[4 * (y3 * w + x) + i];
                    if(srcdata[4 * (y * w + x2) + i] > t)
                        t = srcdata[4 * (y * w + x2) + i];
                    if(srcdata[4 * (y * w + x3) + i] > t)
                        t = srcdata[4 * (y * w + x3) + i];
                    dstdata[4 * (y * w + x) + i] = t;
                }
            }
        }
    }

    return dst;
}

pipi_image_t *pipi_erode(pipi_image_t *src)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, w, h, i, gray;

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
            double t;
            int x2, y2, x3, y3;

            y2 = y - 1;
            if(y2 < 0) y2 = h - 1;
            y3 = y + 1;
            if(y3 >= h) y3 = 0;

            x2 = x - 1;
            if(x2 < 0) x2 = w - 1;
            x3 = x + 1;
            if(x3 >= w) x3 = 0;

            if(gray)
            {
                t = srcdata[y * w + x];
                if(srcdata[y2 * w + x] < t) t = srcdata[y2 * w + x];
                if(srcdata[y3 * w + x] < t) t = srcdata[y3 * w + x];
                if(srcdata[y * w + x2] < t) t = srcdata[y * w + x2];
                if(srcdata[y * w + x3] < t) t = srcdata[y * w + x3];
                dstdata[y * w + x] = t;
            }
            else
            {
                for(i = 0; i < 4; i++)
                {
                    t = srcdata[4 * (y * w + x) + i];
                    if(srcdata[4 * (y2 * w + x) + i] < t)
                        t = srcdata[4 * (y2 * w + x) + i];
                    if(srcdata[4 * (y3 * w + x) + i] < t)
                        t = srcdata[4 * (y3 * w + x) + i];
                    if(srcdata[4 * (y * w + x2) + i] < t)
                        t = srcdata[4 * (y * w + x2) + i];
                    if(srcdata[4 * (y * w + x3) + i] < t)
                        t = srcdata[4 * (y * w + x3) + i];
                    dstdata[4 * (y * w + x) + i] = t;
                }
            }
        }
    }

    return dst;
}

