/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2010 Sam Hocevar <sam@hocevar.net>
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
 * bicubic.c: Bicubic image resizing functions
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

static inline int min_int(int a, int b) { return a < b ? a : b; }
static inline int max_int(int a, int b) { return a > b ? a : b; }

pipi_image_t *pipi_resize_bicubic(pipi_image_t *src, int w, int h)
{
    float *srcdata, *dstdata, *p0, *p1, *p2, *p3;
    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    int x, y, i, sw, dw, sh, dh, i0, i1, i2, i3;
    float scalex, scaley;

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    sw = src->w; sh = src->h;
    dw = dst->w; dh = dst->h;

    scalex = dw > 1 ? (float)(sw - 1) / (dw - 1) : 1.0f;
    scaley = dh > 1 ? (float)(sh - 1) / (dh - 1) : 1.0f;

    for(y = 0; y < dh; y++)
    {
        float yfloat = scaley * y;
        int yint = (int)yfloat;
        float y1 = yfloat - yint;

        p0 = srcdata + 4 * sw * min_int(max_int(0, yint - 1), sh - 1);
        p1 = srcdata + 4 * sw * min_int(max_int(0, yint    ), sh - 1);
        p2 = srcdata + 4 * sw * min_int(max_int(0, yint + 1), sh - 1);
        p3 = srcdata + 4 * sw * min_int(max_int(0, yint + 2), sh - 1);

        for (x = 0; x < dw; x++)
        {
            float xfloat = scalex * x;
            int xint = (int)xfloat;
            float x1 = xfloat - xint;

            i0 = 4 * min_int(max_int(0, xint - 1), sw - 1);
            i1 = 4 * min_int(max_int(0, xint    ), sw - 1);
            i2 = 4 * min_int(max_int(0, xint + 1), sw - 1);
            i3 = 4 * min_int(max_int(0, xint + 2), sw - 1);

            for (i = 0; i < 4; i++, i0++, i1++, i2++, i3++)
            {
                float a00 = p1[i1];
                float a01 = .5f * (p2[i1] - p0[i1]);
                float a02 = p0[i1] - 2.5f * p1[i1]
                            + 2.f * p2[i1] - .5f * p3[i1];
                float a03 = .5f * (p3[i1] - p0[i1]) + 1.5f * (p1[i1] - p2[i1]);

                float a10 = .5f * (p1[i2] - p1[i0]);
                float a11 = .25f * (p0[i0] - p2[i0] - p0[i2] + p2[i2]);
                float a12 = .5f * (p0[i2] - p0[i0]) + 1.25f * (p1[i0] - p1[i2])
                            + .25f * (p3[i0] - p3[i2]) + p2[i2] - p2[i0];
                float a13 = .25f * (p0[i0] - p3[i0] - p0[i2] + p3[i2])
                            + .75f * (p2[i0] - p1[i0] + p1[i2] - p2[i2]);

                float a20 = p1[i0] - 2.5f * p1[i1]
                            + 2.f * p1[i2] - .5f * p1[i3];
                float a21 = .5f * (p2[i0] - p0[i0]) + 1.25f * (p0[i1] - p2[i1])
                            + .25f * (p0[i3] - p2[i3]) - p0[i2] + p2[i2];
                float a22 = p0[i0] - p3[i2] - 2.5f * (p1[i0] + p0[i1])
                            + 2.f * (p2[i0] + p0[i2]) - .5f * (p3[i0] + p0[i3])
                            + 6.25f * p1[i1] - 5.f * (p2[i1] + p1[i2])
                            + 1.25f * (p3[i1] + p1[i3])
                            + 4.f * p2[i2] - p2[i3] + .25f * p3[i3];
                float a23 = 1.5f * (p1[i0] - p2[i0]) + .5f * (p3[i0] - p0[i0])
                            + 1.25f * (p0[i1] - p3[i1])
                            + 3.75f * (p2[i1] - p1[i1]) + p3[i2] - p0[i2]
                            + 3.f * (p1[i2] - p2[i2]) + .25f * (p0[i3] - p3[i3])
                            + .75f * (p2[i3] - p1[i3]);

                float a30 = .5f * (p1[i3] - p1[i0]) + 1.5f * (p1[i1] - p1[i2]);
                float a31 = .25f * (p0[i0] - p2[i0]) + .25f * (p2[i3] - p0[i3])
                            + .75f * (p2[i1] - p0[i1] + p0[i2] - p2[i2]);
                float a32 = -.5f * p0[i0] + 1.25f * p1[i0] - p2[i0]
                            + .25f * p3[i0] + 1.5f * p0[i1] - 3.75f * p1[i1]
                            + 3.f * p2[i1] - .75f * p3[i1] - 1.5f * p0[i2]
                            + 3.75f * p1[i2] - 3.f * p2[i2] + .75f * p3[i2]
                            + .5f * p0[i3] - 1.25f * p1[i3] + p2[i3]
                            - .25f * p3[i3];
                float a33 = .25f * p0[i0] - .75f * p1[i0] + .75f * p2[i0]
                            - .25f * p3[i0] - .75f * p0[i1] + 2.25f * p1[i1]
                            - 2.25f * p2[i1] + .75f * p3[i1] + .75f * p0[i2]
                            - 2.25f * p1[i2] + 2.25f * p2[i2] - .75f * p3[i2]
                            - .25f * p0[i3] + .75f * p1[i3] - .75f * p2[i3]
                            + .25f * p3[i3];

                float y2 = y1 * y1; float y3 = y2 * y1;
                float x2 = x1 * x1; float x3 = x2 * x1;

                float p = a00 + a01 * y1 + a02 * y2 + a03 * y3 +
                   + a10 * x1 + a11 * x1 * y1 + a12 * x1 * y2 + a13 * x1 * y3
                   + a20 * x2 + a21 * x2 * y1 + a22 * x2 * y2 + a23 * x2 * y3
                   + a30 * x3 + a31 * x3 * y1 + a32 * x3 * y2 + a33 * x3 * y3;
                if (p < 0.0f) p = 0.0f;
                if (p > 1.0f) p = 1.0f;

                dstdata[(y * dw + x) * 4 + i] = p;
            }
        }
    }

    return dst;
}

