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
 * convolution.c: generic convolution functions
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

#if !defined TEMPLATE_FILE /* This file uses the template system */
#define TEMPLATE_FLAGS SET_FLAG_GRAY | SET_FLAG_WRAP
#define TEMPLATE_FILE "filter/convolution.c"
#include "pipi-template.h"

pipi_image_t *pipi_convolution(pipi_image_t *src, int m, int n, double mat[])
{
    pipi_image_t *ret;
    double tmp;
    double *hvec, *vvec;
    int i, j, besti = -1, bestj = -1;

    /* Find the cell with the largest value */
    tmp = 0.0;
    for(i = 0; i < m * n; i++)
        if(mat[i] * mat[i] > tmp)
        {
            tmp = mat[i] * mat[i];
            besti = i % m;
            bestj = i / m;
        }

    /* If the kernel is empty, return an empty picture */
    if(tmp == 0.0)
        return pipi_new(src->w, src->h);

    /* Check whether the matrix rank is 1 */
    for(j = 0; j < n; j++)
    {
        if(j == bestj)
            continue;

        for(i = 0; i < m; i++)
        {
            double p, q;

            if(i == besti)
                continue;

            p = mat[j * m + i] * mat[bestj * m + besti];
            q = mat[bestj * m + i] * mat[j * m + besti];

            if(fabs(p - q) > 0.0001 * 0.0001)
            {
                if(src->last_modified == PIPI_PIXELS_Y_F32)
                {
                    if(src->wrap)
                        return conv_gray_wrap(src, m, n, mat);
                    return conv_gray(src, m, n, mat);
                }
                else
                {
                    if(src->wrap)
                        return conv_wrap(src, m, n, mat);
                    return conv(src, m, n, mat);
                }
            }
        }
    }

    /* Matrix rank is 1! Separate the filter */
    hvec = malloc(m * sizeof(double));
    vvec = malloc(n * sizeof(double));

    tmp = sqrt(fabs(mat[bestj * m + besti]));
    for(i = 0; i < m; i++)
        hvec[i] = mat[bestj * m + i] / tmp;
    for(j = 0; j < n; j++)
        vvec[j] = mat[j * m + besti] / tmp;

    if(src->last_modified == PIPI_PIXELS_Y_F32)
        ret = src->wrap ? sepconv_gray_wrap(src, m, hvec, n, vvec)
                        : sepconv_gray(src, m, hvec, n, vvec);
    else
        ret = src->wrap ? sepconv_wrap(src, m, hvec, n, vvec)
                        : sepconv(src, m, hvec, n, vvec);

    free(hvec);
    free(vvec);

    return ret;
}

#else /* XXX: the following functions use the template system */

static pipi_image_t *T(conv)(pipi_image_t *src, int m, int n, double mat[])
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    int x, y, i, j, w, h;

    w = src->w;
    h = src->h;

    srcp = FLAG_GRAY ? pipi_get_pixels(src, PIPI_PIXELS_Y_F32)
                     : pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = FLAG_GRAY ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
                     : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double R = 0., G = 0., B = 0., A = 0.;
            double Y = 0.;
            int x2, y2, off = 4 * (y * w + x);

            for(j = 0; j < n; j++)
            {
                y2 = y + j - n / 2;
                if(y2 < 0) y2 = FLAG_WRAP ? h - 1 - ((-y2 - 1) % h) : 0;
                else if(y2 >= h) y2 = FLAG_WRAP ? y2 % h : h - 1;

                for(i = 0; i < m; i++)
                {
                    double f = mat[j * m + i];

                    x2 = x + i - m / 2;
                    if(x2 < 0) x2 = FLAG_WRAP ? w - 1 - ((-x2 - 1) % w) : 0;
                    else if(x2 >= w) x2 = FLAG_WRAP ? x2 % w : w - 1;

                    if(FLAG_GRAY)
                        Y += f * srcdata[y2 * w + x2];
                    else
                    {
                        R += f * srcdata[(y2 * w + x2) * 4];
                        G += f * srcdata[(y2 * w + x2) * 4 + 1];
                        B += f * srcdata[(y2 * w + x2) * 4 + 2];
                        A += f * srcdata[(y2 * w + x2) * 4 + 3];
                    }
                }
            }

            if(FLAG_GRAY)
                dstdata[y * w + x] = Y < 0.0 ? 0.0 : Y > 1.0 ? 1.0 : Y;
            else
            {
                dstdata[off] = R < 0.0 ? 0.0 : R > 1.0 ? 1.0 : R;
                dstdata[off + 1] = G < 0.0 ? 0.0 : G > 1.0 ? 1.0 : G;
                dstdata[off + 2] = B < 0.0 ? 0.0 : B > 1.0 ? 1.0 : B;
                dstdata[off + 3] = A < 0.0 ? 0.0 : A > 1.0 ? 1.0 : A;
            }
        }
    }

    return dst;
}

static pipi_image_t *T(sepconv)(pipi_image_t *src,
                                int m, double hvec[], int n, double vvec[])
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    double *buffer;
    int x, y, i, j, w, h;

    w = src->w;
    h = src->h;

    srcp = FLAG_GRAY ? pipi_get_pixels(src, PIPI_PIXELS_Y_F32)
                     : pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = FLAG_GRAY ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
                     : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    buffer = malloc(w * h * (FLAG_GRAY ? 1 : 4) * sizeof(double));

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double R = 0., G = 0., B = 0., A = 0.;
            double Y = 0.;
            int x2, off = 4 * (y * w + x);

            for(i = 0; i < m; i++)
            {
                double f = hvec[i];

                x2 = x + i - m / 2;
                if(x2 < 0) x2 = FLAG_WRAP ? w - 1 - ((-x2 - 1) % w) : 0;
                else if(x2 >= w) x2 = FLAG_WRAP ? x2 % w : w - 1;

                if(FLAG_GRAY)
                    Y += f * srcdata[y * w + x2];
                else
                {
                    R += f * srcdata[(y * w + x2) * 4];
                    G += f * srcdata[(y * w + x2) * 4 + 1];
                    B += f * srcdata[(y * w + x2) * 4 + 2];
                    A += f * srcdata[(y * w + x2) * 4 + 3];
                }
            }

            if(FLAG_GRAY)
                buffer[y * w + x] = Y;
            else
            {
                buffer[off] = R;
                buffer[off + 1] = G;
                buffer[off + 2] = B;
                buffer[off + 3] = A;
            }
        }
    }

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double R = 0., G = 0., B = 0., A = 0.;
            double Y = 0.;
            int y2, off = 4 * (y * w + x);

            for(j = 0; j < n; j++)
            {
                double f = vvec[j];

                y2 = y + j - n / 2;
                if(y2 < 0) y2 = FLAG_WRAP ? h - 1 - ((-y2 - 1) % h) : 0;
                else if(y2 >= h) y2 = FLAG_WRAP ? y2 % h : h - 1;

                if(FLAG_GRAY)
                    Y += f * buffer[y2 * w + x];
                else
                {
                    R += f * buffer[(y2 * w + x) * 4];
                    G += f * buffer[(y2 * w + x) * 4 + 1];
                    B += f * buffer[(y2 * w + x) * 4 + 2];
                    A += f * buffer[(y2 * w + x) * 4 + 3];
                }
            }

            if(FLAG_GRAY)
                dstdata[y * w + x] = Y < 0.0 ? 0.0 : Y > 1.0 ? 1.0 : Y;
            else
            {
                dstdata[off] = R < 0.0 ? 0.0 : R > 1.0 ? 1.0 : R;
                dstdata[off + 1] = G < 0.0 ? 0.0 : G > 1.0 ? 1.0 : G;
                dstdata[off + 2] = B < 0.0 ? 0.0 : B > 1.0 ? 1.0 : B;
                dstdata[off + 3] = A < 0.0 ? 0.0 : A > 1.0 ? 1.0 : A;
            }
        }
    }

    free(buffer);

    return dst;
}

#endif

