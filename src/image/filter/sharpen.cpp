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
 * sharpen.c: sharpen functions
 */

#include "config.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_gaussian_sharpen(pipi_image_t *src, float radius)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    double *kernel, *buffer;
    double K, L;
    int x, y, i, w, h, kr, kw;

    w = src->w;
    h = src->h;

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F);
    dstdata = (float *)dstp->pixels;

    kr = (int)(3. * radius + 0.99999);
    kw = 2 * kr + 1;
    K = 1. / (sqrt(2. * M_PI) * radius);
    L = -1. / (2. * radius * radius);

    kernel = malloc(kw * sizeof(double));
    for(i = -kr; i <= kr; i++)
        kernel[i + kr] = exp(L * i * i) * K;

    buffer = malloc(w * h * 4 * sizeof(double));

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double R = 0., G = 0., B = 0., t = 0.;
            int x2, off = 4 * (y * w + x);

            for(i = -kr; i <= kr; i++)
            {
                double f = kernel[i + kr];

                x2 = x + i;
                if(x2 < 0) x2 = 0;
                else if(x2 >= w) x2 = w - 1;

                R += f * srcdata[(y * w + x2) * 4];
                G += f * srcdata[(y * w + x2) * 4 + 1];
                B += f * srcdata[(y * w + x2) * 4 + 2];
                t += f;
            }

            buffer[off] = R / t;
            buffer[off + 1] = G / t;
            buffer[off + 2] = B / t;
        }
    }

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double R = 0., G = 0., B = 0., t = 0.;
            int y2, off = 4 * (y * w + x);

            for(i = -kr; i <= kr; i++)
            {
                double f = kernel[i + kr];

                y2 = y + i;
                if(y2 < 0) y2 = 0;
                else if(y2 >= h) y2 = h - 1;

                R += f * buffer[(y2 * w + x) * 4];
                G += f * buffer[(y2 * w + x) * 4 + 1];
                B += f * buffer[(y2 * w + x) * 4 + 2];
                t += f;
            }

            dstdata[off] = 2 * srcdata[off] - R / t;
            if(dstdata[off] < 0.0) dstdata[off] = 0.0;
            if(dstdata[off] > 1.0) dstdata[off] = 1.0;
            dstdata[off + 1] = 2 * srcdata[off + 1 ] - G / t;
            if(dstdata[off + 1] < 0.0) dstdata[off + 1] = 0.0;
            if(dstdata[off + 1] > 1.0) dstdata[off + 1] = 1.0;
            dstdata[off + 2] = 2 * srcdata[off + 2 ] - B / t;
            if(dstdata[off + 2] < 0.0) dstdata[off + 2] = 0.0;
            if(dstdata[off + 2] > 1.0) dstdata[off + 2] = 1.0;
        }
    }

    free(buffer);
    free(kernel);

    return dst;
}

