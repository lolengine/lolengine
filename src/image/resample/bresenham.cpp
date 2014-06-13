/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2009 Sam Hocevar <sam@hocevar.net>
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
 * bresenham.c: Bresenham image resizing functions
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

/* This is Bresenham resizing. I rediscovered it independently but it was
 * actually first described in 1995 by Tim Kientzle in "Scaling Bitmaps
 * with Bresenham". */

/* FIXME: the algorithm does not handle alpha components properly. Resulting
 * alpha should be the mean alpha value of the neightbouring pixels, but
 * the colour components should be weighted with the alpha value. */
pipi_image_t *pipi_resize_bresenham(pipi_image_t *src, int w, int h)
{
    float *srcdata, *dstdata, *aline, *line;
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    int x, y, x0, y0, sw, dw, sh, dh, remy;
    float invswsh;

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    sw = src->w; sh = src->h;
    dw = dst->w; dh = dst->h;
    invswsh = 1.0f / (sw * sh);

    aline = malloc(4 * dw * sizeof(float));
    line = malloc(4 * dw * sizeof(float));

    memset(line, 0, 4 * dw * sizeof(float));
    remy = 0;

    for(y = 0, y0 = 0; y < dh; y++)
    {
        int toty = 0, ny;

        memset(aline, 0, 4 * dw * sizeof(float));

        while(toty < sh)
        {
            if(remy == 0)
            {
                float r = 0, g = 0, b = 0, a = 0;
                int remx = 0;

                for(x = 0, x0 = 0; x < dw; x++)
                {
                    float ar = 0, ag = 0, ab = 0, aa = 0;
                    int totx = 0, nx;

                    while(totx < sw)
                    {
                        if(remx == 0)
                        {
                            r = srcdata[(y0 * sw + x0) * 4];
                            g = srcdata[(y0 * sw + x0) * 4 + 1];
                            b = srcdata[(y0 * sw + x0) * 4 + 2];
                            a = srcdata[(y0 * sw + x0) * 4 + 3];
                            x0++;
                            remx = dw;
                        }

                        nx = (totx + remx <= sw) ? remx : sw - totx;
                        ar += nx * r; ag += nx * g; ab += nx * b; aa += nx * a;
                        totx += nx;
                        remx -= nx;
                    }

                    line[4 * x] = ar;
                    line[4 * x + 1] = ag;
                    line[4 * x + 2] = ab;
                    line[4 * x + 3] = aa;
                }

                y0++;
                remy = dh;
            }

            ny = (toty + remy <= sh) ? remy : sh - toty;
            for(x = 0; x < dw; x++)
            {
                aline[4 * x] += ny * line[4 * x];
                aline[4 * x + 1] += ny * line[4 * x + 1];
                aline[4 * x + 2] += ny * line[4 * x + 2];
                aline[4 * x + 3] += ny * line[4 * x + 3];
            }
            toty += ny;
            remy -= ny;
        }

        for(x = 0; x < dw; x++)
        {
            dstdata[(y * dw + x) * 4] = aline[4 * x] * invswsh;
            dstdata[(y * dw + x) * 4 + 1] = aline[4 * x + 1] * invswsh;
            dstdata[(y * dw + x) * 4 + 2] = aline[4 * x + 2] * invswsh;
            dstdata[(y * dw + x) * 4 + 3] = aline[4 * x + 3] * invswsh;
        }
    }

    free(aline);
    free(line);

    return dst;
}

