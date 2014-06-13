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
 * dither.c: dithering functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

/* FIXME: this is not the right place for this... see pixels.c instead */
void pipi_dither_24to16(pipi_image_t *img)
{
    int *error, *nexterror;
    pipi_pixels_t *p;
    uint32_t *p32;
    int x, y;

    error = malloc(sizeof(int) * 3 * (img->w + 2));
    nexterror = malloc(sizeof(int) * 3 * (img->w + 2));
    p = pipi_get_pixels(img, PIPI_PIXELS_RGBA_U8);
    p32 = (uint32_t *)p->pixels;

    memset(error, 0, sizeof(int) * 3 * (img->w + 2));

    for(y = 0; y < img->h; y++)
    {
        int er = 0, eg = 0, eb = 0;

        memset(nexterror, 0, sizeof(int) * 3 * (img->w + 2));

        for(x = 0; x < img->w; x++)
        {
            int r, g, b, a, r2, g2, b2;
            r = p32[y * img->w + x] & 0xff;
            g = (p32[y * img->w + x] >> 8) & 0xff;
            b = (p32[y * img->w + x] >> 16) & 0xff;
            a = (p32[y * img->w + x] >> 24) & 0xff;
            r += er + error[x * 3 + 3];
            g += eg + error[x * 3 + 4];
            b += eb + error[x * 3 + 5];
            r2 = r / 8 * 8; g2 = g / 4 * 4; b2 = b / 8 * 8;
            if(r2 < 0) r2 = 0; if(r2 > 0xf8) r2 = 0xf8;
            if(g2 < 0) g2 = 0; if(g2 > 0xfc) g2 = 0xfc;
            if(b2 < 0) b2 = 0; if(b2 > 0xf8) b2 = 0xf8;
            /* hack */
            if(r2 == 0x88 && g2 == 0x88 && b2 == 0x88) g2 = 0x84;
            /* hack */
            p32[y * img->w + x] = (a << 24) | (b2 << 16) | (g2 << 8)  | r2;

            er = r - (r2 / 8 * 255 / 31);
            eg = g - (g2 / 4 * 255 / 63);
            eb = b - (b2 / 8 * 255 / 31);
            nexterror[x * 3 + 0] += er * 3 / 8;
            nexterror[x * 3 + 1] += eg * 3 / 8;
            nexterror[x * 3 + 2] += eb * 3 / 8;
            nexterror[x * 3 + 3] += er * 5 / 8;
            nexterror[x * 3 + 4] += eg * 5 / 8;
            nexterror[x * 3 + 5] += eb * 5 / 8;
            nexterror[x * 3 + 6] += er * 1 / 8;
            nexterror[x * 3 + 7] += eg * 1 / 8;
            nexterror[x * 3 + 8] += eb * 1 / 8;
            er -= er * 3 / 8 + er * 7 / 8 + er * 1 / 8;
            eg -= eg * 3 / 8 + eg * 7 / 8 + eg * 1 / 8;
            eb -= eb * 3 / 8 + eb * 7 / 8 + eb * 1 / 8;
        }

        memcpy(error, nexterror, sizeof(int) * 3 * (img->w + 2));
    }

    pipi_release_pixels(img, p);

    free(error);
    free(nexterror);
}

