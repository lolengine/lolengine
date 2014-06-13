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
 * noise.c: noise rendering functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_render_random(int w, int h)
{
    pipi_image_t *ret;
    pipi_pixels_t *pix;
    float *data;
    unsigned int ctx = 1;
    int x, y, t;

    ret = pipi_new(w, h);
    pix = pipi_get_pixels(ret, PIPI_PIXELS_RGBA_F32);
    data = (float *)pix->pixels;

    for(y = 0; y < h; y++)
        for(x = 0; x < w; x++)
        {
            for(t = 0; t < 3; t++)
            {
                long hi, lo;

                hi = ctx / 12773L;
                lo = ctx % 12773L;
                ctx = 16807L * lo - 2836L * hi;
                if(ctx <= 0)
                    ctx += 0x7fffffffL;

                data[4 * (y * w + x) + t]
                    = (double)((ctx % 65536) / 65535.);
            }
            data[4 * (y * w + x) + 3] = 1.0;
        }

    return ret;
}

