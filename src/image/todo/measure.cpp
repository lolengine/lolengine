/*
 *  Lol Engine
 *
 *  Copyright © 2004—2008 Sam Hocevar <sam@hocevar.net>
 *            © 2008 Jean-Yves Lamoureux <jylam@lnxscene.org>
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What the Fuck You Want
 *  to Public License, Version 2, as published by the WTFPL Task Force.
 *  See http://www.wtfpl.net/ for more details.
 */

/*
 * measure.c: distance functions
 */

#include "config.h"

#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

double pipi_measure_rmsd(pipi_image_t *i1, pipi_image_t *i2)
{
    return sqrt(pipi_measure_msd(i1, i2));
}

double pipi_measure_msd(pipi_image_t *i1, pipi_image_t *i2)
{
    pipi_format_t f1, f2;
    double ret = 0.0;
    float *p1, *p2;
    int x, y, w, h, gray;

    w = i1->w < i2->w ? i1->w : i2->w;
    h = i1->h < i2->h ? i1->h : i2->h;

    f1 = i1->last_modified;
    f2 = i2->last_modified;

    gray = f1 == PIPI_PIXELS_Y_F32 && f2 == PIPI_PIXELS_Y_F32;

    /* FIXME: this is not right */
    if(gray)
    {
        p1 = (float *)i1->p[PIPI_PIXELS_Y_F32].pixels;
        p2 = (float *)i2->p[PIPI_PIXELS_Y_F32].pixels;
    }
    else
    {
        pipi_get_pixels(i1, PIPI_PIXELS_RGBA_F32);
        pipi_get_pixels(i2, PIPI_PIXELS_RGBA_F32);
        p1 = (float *)i1->p[PIPI_PIXELS_RGBA_F32].pixels;
        p2 = (float *)i2->p[PIPI_PIXELS_RGBA_F32].pixels;
    }

    if(gray)
    {
        for(y = 0; y < h; y++)
            for(x = 0; x < w; x++)
            {
                float a = p1[y * i1->w + x];
                float b = p2[y * i2->w + x];
                ret += (a - b) * (a - b);
            }
    }
    else
    {
        for(y = 0; y < h; y++)
            for(x = 0; x < w; x++)
            {
                float a, b, sum = 0.0;

                a = p1[(y * i1->w + x) * 4];
                b = p2[(y * i2->w + x) * 4];
                sum += (a - b) * (a - b);

                a = p1[(y * i1->w + x) * 4 + 1];
                b = p2[(y * i2->w + x) * 4 + 1];
                sum += (a - b) * (a - b);

                a = p1[(y * i1->w + x) * 4 + 2];
                b = p2[(y * i2->w + x) * 4 + 2];
                sum += (a - b) * (a - b);

                ret += sum / 3;
            }
    }

    /* TODO: free pixels if they were allocated */

    /* Restore original image formats */
    i1->last_modified = f1;
    i2->last_modified = f2;

    return ret / (w * h);
}

