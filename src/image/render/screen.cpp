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
 * screen.c: halftoning screen functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_render_bayer(int w, int h)
{
    pipi_image_t *ret;
    pipi_pixels_t *pix;
    float *data;
    int i, j, n;

    if(w <= 0 || h <= 0)
        return NULL;

    for(n = 1; n < w || n < h; n *= 2)
        ;

    ret = pipi_new(w, h);
    pix = pipi_get_pixels(ret, PIPI_PIXELS_Y_F32);
    data = (float *)pix->pixels;

    for(j = 0; j < h; j++)
        for(i = 0; i < w; i++)
        {
            int k, l, x = 0;

            for(k = 1, l = n * n / 4; k < n; k *= 2, l /= 4)
            {
                if((i & k) && (j & k))
                    x += l;
                else if(i & k)
                    x += 3 * l;
                else if(j & k)
                    x += 2 * l;
            }

            data[j * w + i] = (double)(x + 1) / (n * n + 1);
        }

    return ret;
}

typedef struct
{
    int x, y;
    double dist;
}
dot_t;

static int cmpdot(const void *p1, const void *p2)
{
    return ((dot_t const *)p1)->dist > ((dot_t const *)p2)->dist;
}

pipi_image_t *pipi_render_halftone(int w, int h)
{
    pipi_image_t *ret;
    pipi_pixels_t *pix;
    float *data;
    dot_t *circle;
    int x, y, n;

    if(w <= 0 || h <= 0)
        return NULL;

    circle = malloc(w * h * sizeof(dot_t));

    for(y = 0; y < h; y++)
        for(x = 0; x < w; x++)
        {
            double dy = ((double)y + .07) / h - .5;
            double dx = (double)x / w - .5;
            /* Using dx²+dy² here creates another interesting halftone. */
            double r = - cos(M_PI * (dx - dy)) - cos(M_PI * (dx + dy));
            circle[y * w + x].x = x;
            circle[y * w + x].y = y;
            circle[y * w + x].dist = r;
        }
    qsort(circle, w * h, sizeof(dot_t), cmpdot);

    ret = pipi_new(w * 2, h * 2);
    pix = pipi_get_pixels(ret, PIPI_PIXELS_Y_F32);
    data = (float *)pix->pixels;

    for(n = 0; n < w * h; n++)
    {
        x = circle[n].x;
        y = circle[n].y;

        data[y * (2 * w) + x] = (float)(2 * n + 1) / (w * h * 4 + 1);
        data[(y + h) * (2 * w) + x + w] = (float)(2 * n + 2) / (w * h * 4 + 1);
        data[(y + h) * (2 * w) + x] = 1. - (float)(2 * n + 1) / (w * h * 4 + 1);
        data[y * (2 * w) + x + w] = 1. - (float)(2 * n + 2) / (w * h * 4 + 1);
    }

    free(circle);

    return ret;
}

