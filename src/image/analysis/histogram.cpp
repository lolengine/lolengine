/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2008 Sam Hocevar <sam@zoy.org>
 *                2008 Jean-Yves Lamoureux <jylam@lnxscene.org
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
 * histogram.c: histogram functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"


pipi_histogram_t* pipi_new_histogram(void)
{
    return malloc(sizeof(pipi_histogram_t));
}


int pipi_get_image_histogram(pipi_image_t *img, pipi_histogram_t*h, int flags)
{
    pipi_pixels_t *p;
    uint8_t  *data;
    float n;
    unsigned int max;
    int i;

    if(!h) return -1;

    p = pipi_get_pixels(img, PIPI_PIXELS_RGBA_U8);
    data = (uint8_t *)p->pixels;
    memset(h->a, 0, 256*(sizeof(unsigned int)));
    memset(h->r, 0, 256*(sizeof(unsigned int)));
    memset(h->g, 0, 256*(sizeof(unsigned int)));
    memset(h->b, 0, 256*(sizeof(unsigned int)));
    memset(h->y, 0, 256*(sizeof(unsigned int)));


    for(i=0; i< img->w*img->h*4; i+=4)
    {
        if(flags&PIPI_COLOR_A)
            h->a[data[i+3]]++;
        if(flags&PIPI_COLOR_R)
            h->r[data[i+2]]++;
        if(flags&PIPI_COLOR_G)
            h->g[data[i+1]]++;
        if(flags&PIPI_COLOR_B)
            h->b[data[i]]++;
        if(flags&PIPI_COLOR_Y)
        {
            uint32_t val = 0.;
            val += 0.299 * data[i];
            val += 0.587 * data[i+1];
            val += 0.114 * data[i+2];

            h->y[val>255?255:val]++;
        }
    }

    /* Normalize dataset */
    if(flags&PIPI_COLOR_R)
    {
        max = 0;
        for(i=0; i<256; i++)
            if(h->r[i] > max) max = h->r[i];
        n = 255.0f / max;
        for(i=0; i<256; i++)
            h->r[i]*=n;
    }
    if(flags&PIPI_COLOR_G)
    {
        max = 0;
        for(i=0; i<256; i++)
            if(h->g[i] > max) max = h->g[i];
        n = 255.0f / max;
        for(i=0; i<256; i++)
            h->g[i]*=n;
    }
    if(flags&PIPI_COLOR_B)
    {
        max = 0;
        for(i=0; i<256; i++)
            if(h->b[i] > max) max = h->b[i];
        n = 255.0f / max;
        for(i=0; i<256; i++)
            h->b[i]*=n;
    }
    if(flags&PIPI_COLOR_A)
    {
        max = 0;
        for(i=0; i<256; i++)
            if(h->a[i] > max) max = h->a[i];
        n = 255.0f / max;
        for(i=0; i<256; i++)
            h->a[i]*=n;
    }
    if(flags&PIPI_COLOR_Y)
    {
        max = 0;
        for(i=0; i<256; i++)
            if(h->y[i] > max) max = h->y[i];
        n = 255.0f / max;
        for(i=0; i<256; i++)
            h->y[i]*=n;
    }

    pipi_release_pixels(img, p);

    return 0;
}

int pipi_render_histogram(pipi_image_t *img, pipi_histogram_t*h, int flags)
{
    int x;

    if(!img || !h) return -1;

    for(x=0; x<256; x++)
    {
        if(flags&PIPI_COLOR_R)
            pipi_draw_line(img,
                           x, 255,
                           x, 255 - h->r[x],
                           0x00FF0000,
                           0);
        if(flags&PIPI_COLOR_G)
            pipi_draw_line(img,
                           x, 255,
                           x, 255 - h->g[x],
                           0x0000FF00,
                           0);
        if(flags&PIPI_COLOR_B)
            pipi_draw_line(img,
                           x, 255,
                           x, 255 - h->b[x],
                           0x000000FF,
                           0);
        if(flags&PIPI_COLOR_A)
            pipi_draw_line(img,
                           x, 255,
                           x, 255 - h->a[x],
                           0x00000FFF,
                           0);
        if(flags&PIPI_COLOR_Y)
            pipi_draw_line(img,
                           x, 255,
                           x, 255 - h->y[x],
                           0x00FFFFFF,
                           0);
    }

    return 0;
}


int pipi_free_histogram(pipi_histogram_t* h)
{
    if(h) free(h);
    else  return -1;

    return 0;
}
