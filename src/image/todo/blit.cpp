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
 * mean.c: Mean computation function
 */

#include "config.h"

#include <stdlib.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_blit(pipi_image_t *img1, pipi_image_t *img2, int x, int y)
{
    pipi_image_t *dst;
    pipi_pixels_t *img1p, *img2p, *dstp;
    float *img1data, *img2data, *dstdata;
    int dx, dy, w1, h1, w2, h2;

    w1 = img1->w;
    h1 = img1->h;
    w2 = img2->w;
    h2 = img2->h;

    dst = pipi_copy(img1);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    img1p = pipi_get_pixels(img1, PIPI_PIXELS_RGBA_F32);
    img1data = (float *)img1p->pixels;
    img2p = pipi_get_pixels(img2, PIPI_PIXELS_RGBA_F32);
    img2data = (float *)img2p->pixels;

    for(dy = 0; dy < h2; dy++)
    {
        if (y + dy < 0)
            continue;

        if (y + dy >= h1)
            break;

        for(dx = 0; dx < w2; dx++)
        {
            float p, q;
            double t1, t2;

            if (x + dx < 0)
                continue;

            if (x + dx >= w1)
                break;

            t1 = img2data[4 * (dy * w2 + dx) + 3];
            t2 = 1.0 - t1;

            p = img1data[4 * ((y + dy) * w1 + (x + dx))];
            q = img2data[4 * (dy * w2 + dx)];
            dstdata[4 * ((y + dy) * w1 + (x + dx))] = t2 * p + t1 * q;

            p = img1data[4 * ((y + dy) * w1 + (x + dx)) + 1];
            q = img2data[4 * (dy * w2 + dx) + 1];
            dstdata[4 * ((y + dy) * w1 + (x + dx)) + 1] = t2 * p + t1 * q;

            p = img1data[4 * ((y + dy) * w1 + (x + dx)) + 2];
            q = img2data[4 * (dy * w2 + dx) + 2];
            dstdata[4 * ((y + dy) * w1 + (x + dx)) + 2] = t2 * p + t1 * q;

            p = img1data[4 * ((y + dy) * w1 + (x + dx)) + 3];
            q = img2data[4 * (dy * w2 + dx) + 3];
            dstdata[4 * ((y + dy) * w1 + (x + dx)) + 3] = t2 * p + t1 * q;
        }
    }

    return dst;
}

