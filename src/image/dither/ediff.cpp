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
 * ed.c: generic error diffusion functions
 */

#include "config.h"

#include "pipi.h"
#include "pipi-internals.h"

/* Perform a generic error diffusion dithering. The first non-zero
 * element in ker is treated as the current pixel. All other non-zero
 * elements are the error diffusion coefficients.
 * Making the matrix generic is not terribly slower: the performance
 * hit is around 4% for Floyd-Steinberg and 13% for JaJuNi, with the
 * benefit of a lot less code. */
pipi_image_t *pipi_dither_ediff(pipi_image_t *img, pipi_image_t *ker,
                                pipi_scan_t scan)
{
    pipi_image_t *dst;
    pipi_pixels_t *dstp, *kerp;
    float *dstdata, *kerdata;
    int x, y, w, h, i, j, kx, kw, kh;

    w = img->w;
    h = img->h;
    kw = ker->w;
    kh = ker->h;

    dst = pipi_copy(img);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_Y_F32);
    dstdata = (float *)dstp->pixels;

    kerp = pipi_get_pixels(ker, PIPI_PIXELS_Y_F32);
    kerdata = (float *)kerp->pixels;
    for(kx = 0; kx < kw; kx++)
        if(kerdata[kx] > 0)
            break;

    for(y = 0; y < h; y++)
    {
        int reverse = (y & 1) && (scan == PIPI_SCAN_SERPENTINE);

        for(x = 0; x < w; x++)
        {
            float p, q, e;
            int x2 = reverse ? w - 1 - x : x;
            int s = reverse ? -1 : 1;

            p = dstdata[y * w + x2];
            q = p < 0.5 ? 0. : 1.;
            dstdata[y * w + x2] = q;

            e = (p - q);

            for(j = 0; j < kh && y < h - j; j++)
                for(i = 0; i < kw; i++)
                {
                    if(j == 0 && i <= kx)
                        continue;

                    if(x + i - kx < 0 || x + i - kx >= w)
                        continue;

                    dstdata[(y + j) * w + x2 + (i - kx) * s]
                       += e * kerdata[j * kw + i];
                }
        }
    }

    return dst;
}

