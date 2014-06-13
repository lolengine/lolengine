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
 * median.c: median filter functions
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

static int cmpdouble(void const *i1, void const *i2)
{
    double a = *(double const *)i1;
    double b = *(double const *)i2;

    return (a > b) - (a < b);
}

pipi_image_t *pipi_median(pipi_image_t *src, int radius)
{
    return pipi_median_ext(src, radius, radius);
}

/* FIXME: this is in desperate want of optimisation. Here is what could
 * be done to improve the performance:
 *  - prefetch the neighbourhood; most neighbours are the same as the
 *    previous pixels.
 *  - use a better sort algorithm; bubble sort is ridiculous
 *  - even better, use state-of-the art median selection, ie. O(3n), for
 *    most common combinations (9, 25, 49, 81). */
pipi_image_t *pipi_median_ext(pipi_image_t *src, int rx, int ry)
{
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    double *list;
    int x, y, w, h, i, j, size, gray;

    w = src->w;
    h = src->h;
    size = (2 * rx + 1) * (2 * ry + 1);

    gray = (src->last_modified == PIPI_PIXELS_Y_F32);

    srcp = gray ? pipi_get_pixels(src, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = gray ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
                : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    list = malloc(size * (gray ? 1 : 4) * sizeof(double));

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            double *parser = list;

            /* Make a list of neighbours */
            for(j = -ry; j <= ry; j++)
            {
                int y2 = y + j;
                if(y2 < 0) y2 = h - 1 - ((-y2 - 1) % h);
                else if(y2 > 0) y2 = y2 % h;

                for(i = -rx; i <= rx; i++)
                {
                    int x2 = x + i;
                    if(x2 < 0) x2 = w - 1 - ((-x2 - 1) % w);
                    else if(x2 > 0) x2 = x2 % w;

                    if(gray)
                    {
                        *parser++ = srcdata[y2 * w + x2];
                    }
                    else
                    {
                        parser[0] = srcdata[4 * (y2 * w + x2)];
                        parser[size * 1] = srcdata[4 * (y2 * w + x2) + 1];
                        parser[size * 2] = srcdata[4 * (y2 * w + x2) + 2];
                        parser[size * 3] = srcdata[4 * (y2 * w + x2) + 3];
                        parser++;
                    }
                }
            }

            /* Sort the list */
            qsort(list, size, sizeof(double), cmpdouble);
            if(!gray)
            {
                qsort(list + size, size, sizeof(double), cmpdouble);
                qsort(list + 2 * size, size, sizeof(double), cmpdouble);
                qsort(list + 3 * size, size, sizeof(double), cmpdouble);
            }

            /* Store the median value */
            if(gray)
            {
                dstdata[y * w + x] = list[size / 2];
            }
            else
            {
                dstdata[4 * (y * w + x)] = list[size / 2];
                dstdata[4 * (y * w + x) + 1] = list[size / 2 + size * 1];
                dstdata[4 * (y * w + x) + 2] = list[size / 2 + size * 2];
                dstdata[4 * (y * w + x) + 3] = list[size / 2 + size * 3];
            }
        }
    }

    return dst;
}

