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
 * blur.c: blur functions
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

#if !defined TEMPLATE_FILE /* This file uses the template system */
#define TEMPLATE_FLAGS SET_FLAG_GRAY | SET_FLAG_WRAP
#define TEMPLATE_FILE "filter/blur.c"
#include "pipi-template.h"

pipi_image_t *pipi_box_blur(pipi_image_t *src, int size)
{
    return pipi_box_blur_ext(src, size, size);
}

pipi_image_t *pipi_box_blur_ext(pipi_image_t *src, int m, int n)
{
    if(src->wrap)
    {
        if(src->last_modified == PIPI_PIXELS_Y_F32)
            return boxblur_gray_wrap(src, m, n);

        return boxblur_wrap(src, m, n);
    }
    else
    {
        if(src->last_modified == PIPI_PIXELS_Y_F32)
            return boxblur_gray(src, m, n);

        return boxblur(src, m, n);
    }
}

#else /* XXX: the following functions use the template system */

static pipi_image_t *T(boxblur)(pipi_image_t *src, int m, int n)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    double *acc;
    int x, y, w, h, i, j, i2, j2, size;

    w = src->w;
    h = src->h;
    size = (2 * m + 1) * (2 * n + 1);

    srcp = FLAG_GRAY ? pipi_get_pixels(src, PIPI_PIXELS_Y_F32)
                     : pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = FLAG_GRAY ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
                     : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    acc = malloc(w * (FLAG_GRAY ? 1 : 4) * sizeof(double));

    /* Step 1: fill the accumulator */
    for(x = 0; x < w; x++)
    {
        double r = 0., g = 0., b = 0., a = 0.;
        double t = 0.;

        for(j = -n; j <= n; j++)
        {
            if(FLAG_WRAP)
                j2 = (j < 0) ? h - 1 - ((-j - 1) % h) : j % h;
            else
                j2 = (j < 0) ? 0 : (j >= h) ? h - 1 : j;

            if(FLAG_GRAY)
                t += srcdata[j2 * w + x];
            else
            {
                r += srcdata[4 * (j2 * w + x)];
                g += srcdata[4 * (j2 * w + x) + 1];
                b += srcdata[4 * (j2 * w + x) + 2];
                a += srcdata[4 * (j2 * w + x) + 3];
            }
        }

        if(FLAG_GRAY)
            acc[x] = t;
        else
        {
            acc[4 * x] = r;
            acc[4 * x + 1] = g;
            acc[4 * x + 2] = b;
            acc[4 * x + 3] = a;
        }
    }

    /* Step 2: blur the image, line by line */
    for(y = 0; y < h; y++)
    {
        double r = 0., g = 0., b = 0., a = 0.;
        double t = 0.;

        /* 2.1: compute the first pixel */
        for(i = -m; i <= m; i++)
        {
            if(FLAG_WRAP)
                i2 = (i < 0) ? w - 1 - ((-i - 1) % w) : i % w;
            else
                i2 = (i < 0) ? 0 : (i >= w) ? w - 1 : i;

            if(FLAG_GRAY)
                t += acc[i2];
            else
            {
                r += acc[4 * i2];
                g += acc[4 * i2 + 1];
                b += acc[4 * i2 + 2];
                a += acc[4 * i2 + 3];
            }
        }

        /* 2.2: iterate on the whole line */
        for(x = 0; x < w; x++)
        {
            int u, u2, v, v2;

            if(FLAG_GRAY)
            {
                dstdata[y * w + x] = t / size;
            }
            else
            {
                dstdata[4 * (y * w + x)] = r / size;
                dstdata[4 * (y * w + x) + 1] = g / size;
                dstdata[4 * (y * w + x) + 2] = b / size;
                dstdata[4 * (y * w + x) + 3] = a / size;
            }

            u = x - m;
            if(FLAG_WRAP)
                u2 = (u < 0) ? w - 1 - ((-u - 1) % w) : u % w;
            else
                u2 = (u < 0) ? 0 : (u >= w) ? w - 1 : u;
            v = x + m + 1;
            if(FLAG_WRAP)
                v2 = (v < 0) ? w - 1 - ((-v - 1) % w) : v % w;
            else
                v2 = (v < 0) ? 0 : (v >= w) ? w - 1 : v;
            if(FLAG_GRAY)
            {
                t = t - acc[u2] + acc[v2];
            }
            else
            {
                r = r - acc[4 * u2] + acc[4 * v2];
                g = g - acc[4 * u2 + 1] + acc[4 * v2 + 1];
                b = b - acc[4 * u2 + 2] + acc[4 * v2 + 2];
                a = a - acc[4 * u2 + 3] + acc[4 * v2 + 3];
            }
        }

        /* 2.3: update the accumulator */
        for(x = 0; x < w; x++)
        {
            int u, u2, v, v2;

            u = y - n;
            if(FLAG_WRAP)
                u2 = (u < 0) ? h - 1 - ((-u - 1) % h) : u % h;
            else
                u2 = (u < 0) ? 0 : (u >= h) ? h - 1 : u;
            v = y + n + 1;
            if(FLAG_WRAP)
                v2 = (v < 0) ? h - 1 - ((-v - 1) % h) : v % h;
            else
                v2 = (v < 0) ? 0 : (v >= h) ? h - 1 : v;
            if(FLAG_GRAY)
            {
                acc[x] += srcdata[v2 * w + x] - srcdata[u2 * w + x];
            }
            else
            {
                int uoff = 4 * (u2 * w + x);
                int voff = 4 * (v2 * w + x);

                acc[4 * x] += srcdata[voff] - srcdata[uoff];
                acc[4 * x + 1] += srcdata[voff + 1] - srcdata[uoff + 1];
                acc[4 * x + 2] += srcdata[voff + 2] - srcdata[uoff + 2];
                acc[4 * x + 3] += srcdata[voff + 3] - srcdata[uoff + 3];
            }
        }
    }

    free(acc);

    return dst;
}

#endif

