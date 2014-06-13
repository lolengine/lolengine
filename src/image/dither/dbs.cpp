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
 * dbs.c: Direct Binary Search dithering functions
 */

#include "config.h"

#include <string.h>
#include <stdlib.h>

#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

#define CELL 16

#define N 7
#define NN ((N * 2 + 1))

/* FIXME: though the algorithm is supposed to stop, we do not have a real,
 * guaranteed stop condition here. */

pipi_image_t *pipi_dither_dbs(pipi_image_t *img)
{
    double kernel[NN * NN];
    double t = 0.;
    pipi_image_t *src, *dst, *tmp1, *tmp2;
    pipi_pixels_t *dstp, *tmp1p, *tmp2p;
    int *changelist;
    float *dstdata, *tmp1data, *tmp2data;
    int i, j, x, y, w, h, cw, ch;

    /* Build our human visual system kernel. */
    for(j = 0; j < NN; j++)
        for(i = 0; i < NN; i++)
        {
            double a = (double)(i - N);
            double b = (double)(j - N);
            kernel[j * NN + i] =
                    exp(-(a * a + b * b) / (2. * 1.6 * 1.6))
                  + exp(-(a * a + b * b) / (2. * 0.6 * 0.6));
            t += kernel[j * NN + i];
        }

    for(j = 0; j < NN; j++)
        for(i = 0; i < NN; i++)
            kernel[j * NN + i] /= t;

    w = img->w;
    h = img->h;

    cw = (w + CELL - 1) / CELL;
    ch = (h + CELL - 1) / CELL;
    changelist = malloc(cw * ch * sizeof(int));
    memset(changelist, 0, cw * ch * sizeof(int));

    src = pipi_copy(img);
    pipi_get_pixels(src, PIPI_PIXELS_Y_F32);

    tmp1 = pipi_convolution(src, NN, NN, kernel);
    tmp1p = pipi_get_pixels(tmp1, PIPI_PIXELS_Y_F32);
    tmp1data = (float *)tmp1p->pixels;

    dst = pipi_dither_random(src);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_Y_F32);
    dstdata = (float *)dstp->pixels;

    pipi_free(src);

    tmp2 = pipi_convolution(dst, NN, NN, kernel);
    tmp2p = pipi_get_pixels(tmp2, PIPI_PIXELS_Y_F32);
    tmp2data = (float *)tmp2p->pixels;

    for(;;)
    {
        int cx, cy, n;
        int allchanges = 0;

        for(cy = 0; cy < ch; cy++)
        for(cx = 0; cx < cw; cx++)
        {
            int changes = 0;

            if(changelist[cy * cw + cx] >= 2)
                continue;

            for(y = cy * CELL; y < (cy + 1) * CELL; y++)
            for(x = cx * CELL; x < (cx + 1) * CELL; x++)
            {
                double d, d2, e, best = 0.;
                int opx = -1, opy = -1;

                d = dstdata[y * w + x];

                /* Compute the effect of a toggle */
                e = 0.;
                for(j = -N; j < N + 1; j++)
                {
                    if(y + j < 0 || y + j >= h)
                        continue;

                    for(i = -N; i < N + 1; i++)
                    {
                        double m, p, q1, q2;

                        if(x + i < 0 || x + i >= w)
                            continue;

                        m = kernel[(j + N) * NN + i + N];
                        p = tmp1data[(y + j) * w + x + i];
                        q1 = tmp2data[(y + j) * w + x + i];
                        q2 = q1 - m * d + m * (1. - d);
                        e += (q1 - p) * (q1 - p) - (q2 - p) * (q2 - p);
                    }
                }
                if(e > best)
                {
                    best = e;
                    opx = opy = 0;
                }

                /* Compute the effect of swaps */
                for(n = 0; n < 8; n++)
                {
                    static int const step[] =
                      { 0, 1, 0, -1, -1, 0, 1, 0, -1, -1, -1, 1, 1, -1, 1, 1 };
                    int idx = step[n * 2], idy = step[n * 2 + 1];
                    if(y + idy < 0 || y + idy >= h
                         || x + idx < 0 || x + idx >= w)
                        continue;
                    d2 = dstdata[(y + idy) * w + x + idx];
                    if(d2 == d)
                        continue;
                    e = 0.;
                    for(j = -N; j < N + 1; j++)
                    {
                        if(y + j < 0 || y + j >= h)
                            continue;
                        if(j - idy + N < 0 || j - idy + N >= NN)
                            continue;
                        for(i = -N; i < N + 1; i++)
                        {
                            double ma, mb, p, q1, q2;
                            if(x + i < 0 || x + i >= w)
                                continue;
                            if(i - idx + N < 0 || i - idx + N >= NN)
                                continue;
                            ma = kernel[(j + N) * NN + i + N];
                            mb = kernel[(j - idy + N) * NN + i - idx + N];
                            p = tmp1data[(y + j) * w + x + i];
                            q1 = tmp2data[(y + j) * w + x + i];
                            q2 = q1 - ma * d + ma * d2 - mb * d2 + mb * d;
                            e += (q1 - p) * (q1 - p) - (q2 - p) * (q2 - p);
                        }
                    }
                    if(e > best)
                    {
                        best = e;
                        opx = idx;
                        opy = idy;
                    }
                }

                /* Apply the change if interesting */
                if(best <= 0.)
                    continue;
                if(opx || opy)
                {
                    d2 = dstdata[(y + opy) * w + x + opx];
                    dstdata[(y + opy) * w + x + opx] = d;
                }
                else
                    d2 = 1. - d;
                dstdata[y * w + x] = d2;
                for(j = -N; j < N + 1; j++)
                for(i = -N; i < N + 1; i++)
                {
                    double m = kernel[(j + N) * NN + i + N];
                    if(y + j >= 0 && y + j < h
                        && x + i >= 0 && x + i < w)
                    {
                        t = tmp2data[(y + j) * w + x + i];
                        tmp2data[(y + j) * w + x + i] = t + m * (d2 - d);
                    }
                    if((opx || opy) && y + opy + j >= 0 && y + opy + j < h
                                    && x + opx + i >= 0 && x + opx + i < w)
                    {
                        t = tmp2data[(y + opy + j) * w + x + opx + i];
                        tmp2data[(y + opy + j) * w + x + opx + i]
                                = t + m * (d - d2);
                    }
                }

                changes++;
            }

            if(changes == 0)
                changelist[cy * cw + cx]++;

            allchanges += changes;
        }

        if(allchanges == 0)
            break;
    }

    free(changelist);

    pipi_free(tmp1);
    pipi_free(tmp2);

    return dst;
}

