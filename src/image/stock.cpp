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
 * stock.c: stock images
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_load_stock(char const *name)
{
    pipi_image_t *ret;
    pipi_pixels_t *pix;

    /* Generate a Bayer dithering pattern. */
    if(!strncmp(name, "bayer:", 6))
    {
        int w, h = 0;

        w = atoi(name + 6);
        name = strchr(name + 6, 'x');
        if(name)
            h = atoi(name + 1);
        if(!h)
            h = w;

        return pipi_render_bayer(w, h);
    }

    /* Generate a clustered dithering pattern. */
    if(!strncmp(name, "halftone:", 9))
    {
        int w, h = 0;

        w = atoi(name + 9);
        name = strchr(name + 9, 'x');
        if(name)
            h = atoi(name + 1);
        if(!h)
            h = w;

        return pipi_render_halftone(w, h);
    }

    /* Generate an error diffusion matrix. */
    if(!strncmp(name, "ediff:", 6))
    {
        float const *ker;
        int w, h;

        if(!strcmp(name + 6, "fs"))
        {
            static float const myker[] =
            {
                   0.,     1.,  7./16,
                3./16,  5./16,  1./16,
            };
            ker = myker; w = 3; h = 2;
        }
        else if(!strcmp(name + 6, "jajuni"))
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  7./48,  5./48,
                3./48,  5./48,  7./48,  5./48,  3./48,
                1./48,  3./48,  5./48,  3./48,  1./48,
            };
            ker = myker; w = 5; h = 3;
        }
        else if(!strcmp(name + 6, "atkinson"))
        {
            static float const myker[] =
            {
                  0.,    1.,  1./8,  1./8,
                1./8,  1./8,  1./8,    0.,
                  0.,  1./8,    0.,    0.,
            };
            ker = myker; w = 4; h = 3;
        }
        else if(!strcmp(name + 6, "fan"))
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  7./16,
                1./16,  3./16,  5./16,     0.,
            };
            ker = myker; w = 4; h = 2;
        }
        else if(!strcmp(name + 6, "shiaufan"))
        {
            static float const myker[] =
            {
                  0.,    0.,    1.,  1./2,
                1./8,  1./8,  1./4,    0.,
            };
            ker = myker; w = 4; h = 2;
        }
        else if(!strcmp(name + 6, "shiaufan2"))
        {
            static float const myker[] =
            {
                   0.,     0.,    0.,    1.,  1./2,
                1./16,  1./16,  1./8,  1./4,    0.,
            };
            ker = myker; w = 5; h = 2;
        }
        else if(!strcmp(name + 6, "stucki"))
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  8./42,  4./42,
                2./42,  4./42,  8./42,  4./42,  2./42,
                1./42,  2./42,  4./42,  2./42,  1./42,
            };
            ker = myker; w = 5; h = 3;
        }
        else if(!strcmp(name + 6, "burkes"))
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  4./16,  2./16,
                1./16,  2./16,  4./16,  2./16,  1./16,
            };
            ker = myker; w = 5; h = 2;
        }
        else if(!strcmp(name + 6, "sierra"))
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  5./32,  3./32,
                2./32,  4./32,  5./32,  4./32,  2./32,
                   0.,  2./32,  3./32,  2./32,     0.,
            };
            ker = myker; w = 5; h = 3;
        }
        else if(!strcmp(name + 6, "sierra2"))
        {
            static float const myker[] =
            {
                   0.,     0.,     1.,  4./16,  3./16,
                1./16,  2./16,  3./16,  2./16,  1./16,
            };
            ker = myker; w = 5; h = 2;
        }
        else if(!strcmp(name + 6, "lite"))
        {
            static float const myker[] =
            {
                  0.,    1.,  1./2,
                1./4,  1./4,    0.,
            };
            ker = myker; w = 3; h = 2;
        }
        else
            return NULL;

        ret = pipi_new(w, h);
        pix = pipi_get_pixels(ret, PIPI_PIXELS_Y_F32);
        memcpy(pix->pixels, ker, w * h * sizeof(float));

        return ret;
    }

    /* Generate a completely random image. */
    if(!strncmp(name, "random:", 7))
    {
        int w, h = 0;

        w = atoi(name + 7);
        name = strchr(name + 7, 'x');
        if(name)
            h = atoi(name + 1);
        if(!h)
            h = w;
        if(w <= 0 || h <= 0)
            return NULL;

        return pipi_render_random(w, h);
    }

    return NULL;
}

