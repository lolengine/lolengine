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
 * wave.c: wave and other warping effects
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

#include "pipi.h"
#include "pipi-internals.h"

#define BORDER 64

pipi_image_t *pipi_wave(pipi_image_t *src, double dw, double dh,
                        double d, double a)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    double sina, cosa;
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

    sina = sin(a);
    cosa = cos(a);

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double angle = 2 * M_PI / dw * ((x - w / 2) * cosa
                                             + (y - h / 2) * sina - d);
            double displacement = dh * sin(angle);
            double dx, dy;
            int x2, y2;

            dx = cosa * displacement;
            dy = sina * displacement;

            if(x < BORDER) dx = dx * x / BORDER;
            if(x > w - 1 - BORDER) dx = dx * (w - 1 - x) / BORDER;
            if(y < BORDER) dy = dy * y / BORDER;
            if(y > h - 1 - BORDER) dy = dy * (h - 1 - y) / BORDER;

            x2 = x + dx;
            y2 = y + dy;

            /* Just in case... */
            if(x2 < 0) x2 = 0;
            else if(x2 >= w) x2 = w - 1;

            if(y2 < 0) y2 = 0;
            else if(y2 >= h) y2 = h - 1;

            if(gray)
            {
                dstdata[y * w + x] = srcdata[y2 * w + x2];
            }
            else
            {
                for(i = 0; i < 4; i++)
                {
                    dstdata[4 * (y * w + x) + i]
                        = srcdata[4 * (y2 * w + x2) + i];
                }
            }
        }
    }

    return dst;
}

pipi_image_t *pipi_sine(pipi_image_t *src, double dw, double dh,
                        double d, double a)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    double sina, cosa;
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

    sina = sin(a);
    cosa = cos(a);

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double angle = 2 * M_PI / dw * ((x - w / 2) * cosa
                                             + (y - h / 2) * sina - d);
            double displacement = dh * sin(angle);
            double dx, dy;
            int x2, y2;

            dx = -sina * displacement;
            dy = cosa * displacement;

            if(x < BORDER) dx = dx * x / BORDER;
            if(x > w - 1 - BORDER) dx = dx * (w - 1 - x) / BORDER;
            if(y < BORDER) dy = dy * y / BORDER;
            if(y > h - 1 - BORDER) dy = dy * (h - 1 - y) / BORDER;

            x2 = x + dx;
            y2 = y + dy;

            /* Just in case... */
            if(x2 < 0) x2 = 0;
            else if(x2 >= w) x2 = w - 1;

            if(y2 < 0) y2 = 0;
            else if(y2 >= h) y2 = h - 1;

            if(gray)
            {
                dstdata[y * w + x] = srcdata[y2 * w + x2];
            }
            else
            {
                for(i = 0; i < 4; i++)
                {
                    dstdata[4 * (y * w + x) + i]
                        = srcdata[4 * (y2 * w + x2) + i];
                }
            }
        }
    }

    return dst;
}

