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
 * ordered.c: Bayer ordered dithering functions
 */

#include "config.h"

#include <stdlib.h>
#include <math.h>
#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_dither_halftone(pipi_image_t *img, double r, double angle)
{
#define PRECISION 4.
    pipi_image_t *ret, *kernel;
    int k = (r * PRECISION / 2. / sqrt(2.) + .5);

    kernel = pipi_render_halftone(k, k);
    ret = pipi_dither_ordered_ext(img, kernel, 1. / PRECISION, angle + 45.);
    pipi_free(kernel);

    return ret;
}

pipi_image_t *pipi_dither_ordered(pipi_image_t *img, pipi_image_t *kernel)
{
    return pipi_dither_ordered_ext(img, kernel, 1.0, 0.0);
}

pipi_image_t *pipi_dither_ordered_ext(pipi_image_t *img, pipi_image_t *kernel,
                                      double scale, double angle)
{
    double sint, cost;
    pipi_image_t *dst;
    pipi_pixels_t *dstp, *kernelp;
    float *dstdata, *kerneldata;
    int x, y, w, h, kx, ky, kw, kh;

    w = img->w;
    h = img->h;
    kw = kernel->w;
    kh = kernel->h;

    cost = cos(angle * (M_PI / 180));
    sint = sin(angle * (M_PI / 180));

    dst = pipi_copy(img);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_Y_F32);
    dstdata = (float *)dstp->pixels;

    kernelp = pipi_get_pixels(kernel, PIPI_PIXELS_Y_F32);
    kerneldata = (float *)kernelp->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            float p, q;

            kx = (int)((cost * x - sint * y + 2 * w * h) / scale) % kw;
            ky = (int)((cost * y + sint * x + 2 * w * h) / scale) % kh;

            p = dstdata[y * w + x];
            q = p > kerneldata[ky * kw + kx] ? 1. : 0.;
            dstdata[y * w + x] = q;
        }
    }

    return dst;
}

typedef struct
{
    int x, y;
    double val;
}
dot_t;

static int cmpdot(const void *p1, const void *p2)
{
    return ((dot_t const *)p1)->val > ((dot_t const *)p2)->val;
}

pipi_image_t *pipi_order(pipi_image_t *src)
{
    double epsilon;
    pipi_image_t *dst;
    pipi_pixels_t *dstp, *srcp;
    float *dstdata, *srcdata;
    dot_t *circle;
    int x, y, w, h, n;

    w = src->w;
    h = src->h;
    epsilon = 1. / (w * h + 1);

    srcp = pipi_get_pixels(src, PIPI_PIXELS_Y_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_Y_F32);
    dstdata = (float *)dstp->pixels;

    circle = malloc(w * h * sizeof(dot_t));

    for(y = 0; y < h; y++)
        for(x = 0; x < w; x++)
        {
            circle[y * w + x].x = x;
            circle[y * w + x].y = y;
            circle[y * w + x].val = srcdata[y * w + x];
        }
    qsort(circle, w * h, sizeof(dot_t), cmpdot);

    for(n = 0; n < w * h; n++)
    {
        x = circle[n].x;
        y = circle[n].y;
        dstdata[y * w + x] = (float)(n + 1) * epsilon;
    }

    free(circle);

    return dst;
}

