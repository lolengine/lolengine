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

pipi_image_t *pipi_rotate(pipi_image_t *src, double a)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    double sina, cosa, cx, cy;
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

    sina = sin(a * M_PI / 180.0);
    cosa = cos(a * M_PI / 180.0);

    cx = (double)w / 2.0;
    cy = (double)h / 2.0;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double dx, dy;
            int x2, y2;

            dx = ((double)x - cx) * cosa - ((double)y - cy) * sina;
            dy = ((double)y - cy) * cosa + ((double)x - cx) * sina;

            x2 = (int)(cx + dx + 0.5);
            y2 = (int)(cy + dy + 0.5);

            if(gray)
            {
                if(x2 < 0 || y2 < 0 || x2 >= w || y2 >= h)
                    ;
                else
                    dstdata[y * w + x] = srcdata[y2 * w + x2];
            }
            else
            {
                if(x2 < 0 || y2 < 0 || x2 >= w || y2 >= h)
                {
                    dstdata[4 * (y * w + x) + 3] = 0.0f;
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
    }

    return dst;
}

