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
 * pixels.c: pixel-level image manipulation
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

static void init_tables(void);

static double global_gamma = 2.2;
static int done = 0;

static float u8tof32_table[256];
static inline float u8tof32(uint8_t p) { return u8tof32_table[(int)p]; }

/* Return a direct pointer to an image's pixels. */
pipi_pixels_t *pipi_get_pixels(pipi_image_t *img, pipi_format_t type)
{
    size_t bytes = 0;
    int x, y, i, bpp = 0;

    if(type < 0 || type >= PIPI_PIXELS_MAX)
        return NULL;

    if(img->last_modified == type)
        return &img->p[type];

    /* Handle special cases */
    if(type == PIPI_PIXELS_MASK_U8)
        return &img->p[type];

    /* Preliminary conversions */
    if(img->last_modified == PIPI_PIXELS_RGBA_U8
                  && type == PIPI_PIXELS_Y_F32)
        pipi_get_pixels(img, PIPI_PIXELS_RGBA_F32);
    else if(img->last_modified == PIPI_PIXELS_BGR_U8
                       && type == PIPI_PIXELS_Y_F32)
        pipi_get_pixels(img, PIPI_PIXELS_RGBA_F32);
    else if(img->last_modified == PIPI_PIXELS_Y_F32
                       && type == PIPI_PIXELS_RGBA_U8)
        pipi_get_pixels(img, PIPI_PIXELS_RGBA_F32);
    else if(img->last_modified == PIPI_PIXELS_Y_F32
                       && type == PIPI_PIXELS_BGR_U8)
        pipi_get_pixels(img, PIPI_PIXELS_RGBA_F32);

    /* Allocate pixels if necessary */
    if(!img->p[type].pixels)
    {
        switch(type)
        {
        case PIPI_PIXELS_RGBA_U8:
            bytes = img->w * img->h * 4 * sizeof(uint8_t);
            bpp = 4 * sizeof(uint8_t);
            break;
        case PIPI_PIXELS_BGR_U8:
            bytes = img->w * img->h * 3 * sizeof(uint8_t);
            bpp = 3 * sizeof(uint8_t);
            break;
        case PIPI_PIXELS_RGBA_F32:
            bytes = img->w * img->h * 4 * sizeof(float);
            bpp = 4 * sizeof(float);
            break;
        case PIPI_PIXELS_Y_F32:
            bytes = img->w * img->h * sizeof(float);
            bpp = sizeof(float);
            break;
        default:
            return NULL;
        }

        img->p[type].pixels = malloc(bytes);
        img->p[type].bytes = bytes;
        img->p[type].bpp = bpp;
        img->p[type].w = img->w;
        img->p[type].h = img->h;
    }

    /* Convert pixels */
    if(img->last_modified == PIPI_PIXELS_RGBA_U8
                  && type == PIPI_PIXELS_RGBA_F32)
    {
        uint8_t *src = (uint8_t *)img->p[PIPI_PIXELS_RGBA_U8].pixels;
        float *dest = (float *)img->p[type].pixels;

        init_tables();

        for(y = 0; y < img->h; y++)
            for(x = 0; x < img->w; x++)
                for(i = 0; i < 4; i++)
                    dest[4 * (y * img->w + x) + i]
                        = u8tof32(src[4 * (y * img->w + x) + i]);
    }
    else if(img->last_modified == PIPI_PIXELS_BGR_U8
                       && type == PIPI_PIXELS_RGBA_F32)
    {
        uint8_t *src = (uint8_t *)img->p[PIPI_PIXELS_BGR_U8].pixels;
        float *dest = (float *)img->p[type].pixels;

        init_tables();

        for(y = 0; y < img->h; y++)
            for(x = 0; x < img->w; x++)
            {
                dest[4 * (y * img->w + x)]
                    = u8tof32(src[3 * (y * img->w + x) + 2]);
                dest[4 * (y * img->w + x) + 1]
                    = u8tof32(src[3 * (y * img->w + x) + 1]);
                dest[4 * (y * img->w + x) + 2]
                    = u8tof32(src[3 * (y * img->w + x)]);
                dest[4 * (y * img->w + x) + 3] = 1.0;
            }
    }
    else if(img->last_modified == PIPI_PIXELS_RGBA_F32
                       && type == PIPI_PIXELS_RGBA_U8)
    {
        float *src = (float *)img->p[PIPI_PIXELS_RGBA_F32].pixels;
        uint8_t *dest = (uint8_t *)img->p[type].pixels;

        init_tables();

        for(y = 0; y < img->h; y++)
            for(x = 0; x < img->w; x++)
                for(i = 0; i < 4; i++)
                {
                    double p, e;
                    uint8_t d;

                    p = src[4 * (y * img->w + x) + i];

                    if(p < 0.) d = 0.;
                    else if(p > 1.) d = 255;
                    else d = (int)(255.999 * pow(p, 1. / global_gamma));

                    dest[4 * (y * img->w + x) + i] = d;

                    e = (p - u8tof32(d)) / 16;
                    if(x < img->w - 1)
                        src[4 * (y * img->w + x + 1) + i] += e * 7;
                    if(y < img->h - 1)
                    {
                        if(x > 0)
                            src[4 * ((y + 1) * img->w + x - 1) + i] += e * 3;
                        src[4 * ((y + 1) * img->w + x) + i] += e * 5;
                        if(x < img->w - 1)
                            src[4 * ((y + 1) * img->w + x + 1) + i] += e;
                    }
                }
    }
    else if(img->last_modified == PIPI_PIXELS_RGBA_F32
                       && type == PIPI_PIXELS_BGR_U8)
    {
        float *src = (float *)img->p[PIPI_PIXELS_RGBA_F32].pixels;
        uint8_t *dest = (uint8_t *)img->p[type].pixels;

        init_tables();

        for(y = 0; y < img->h; y++)
            for(x = 0; x < img->w; x++)
                for(i = 0; i < 3; i++)
                {
                    double p, e;
                    uint8_t d;

                    p = src[4 * (y * img->w + x) + i];

                    if(p < 0.) d = 0.;
                    else if(p > 1.) d = 255;
                    else d = (int)(255.999 * pow(p, 1. / global_gamma));

                    dest[3 * (y * img->w + x) + i] = d;

                    e = (p - u8tof32(d)) / 16;
                    if(x < img->w - 1)
                        src[4 * (y * img->w + x + 1) + i] += e * 7;
                    if(y < img->h - 1)
                    {
                        if(x > 0)
                            src[4 * ((y + 1) * img->w + x - 1) + i] += e * 3;
                        src[4 * ((y + 1) * img->w + x) + i] += e * 5;
                        if(x < img->w - 1)
                            src[4 * ((y + 1) * img->w + x + 1) + i] += e;
                    }
                }
    }
    else if(img->last_modified == PIPI_PIXELS_Y_F32
                       && type == PIPI_PIXELS_RGBA_F32)
    {
        float *src = (float *)img->p[PIPI_PIXELS_Y_F32].pixels;
        float *dest = (float *)img->p[PIPI_PIXELS_RGBA_F32].pixels;

        init_tables();

        for(y = 0; y < img->h; y++)
            for(x = 0; x < img->w; x++)
            {
                float p = src[y * img->w + x];
                dest[4 * (y * img->w + x)] = p;
                dest[4 * (y * img->w + x) + 1] = p;
                dest[4 * (y * img->w + x) + 2] = p;
                dest[4 * (y * img->w + x) + 3] = 1.0;
            }
    }
    else if(img->last_modified == PIPI_PIXELS_RGBA_F32
                       && type == PIPI_PIXELS_Y_F32)
    {
        float *src = (float *)img->p[PIPI_PIXELS_RGBA_F32].pixels;
        float *dest = (float *)img->p[PIPI_PIXELS_Y_F32].pixels;

        init_tables();

        for(y = 0; y < img->h; y++)
            for(x = 0; x < img->w; x++)
            {
                float p = 0.;
                p += 0.299 * src[4 * (y * img->w + x)];
                p += 0.587 * src[4 * (y * img->w + x) + 1];
                p += 0.114 * src[4 * (y * img->w + x) + 2];
                dest[y * img->w + x] = p;
            }
    }
    else
    {
        memset(img->p[type].pixels, 0, bytes);
    }

    img->last_modified = type;

    return &img->p[type];
}

void pipi_release_pixels(pipi_image_t *img, pipi_pixels_t *p)
{
    return;
}

void pipi_set_colorspace(pipi_image_t *img, pipi_format_t fmt)
{
    pipi_pixels_t *p = pipi_get_pixels(img, fmt);
    pipi_release_pixels(img, p);
}

void pipi_set_gamma(double g)
{
    if(g > 0.)
    {
        global_gamma = g;
        done = 0;
    }
}

static void init_tables(void)
{
    int i;

    if(done)
        return;

    for(i = 0; i < 256; i++)
        u8tof32_table[i] = pow((double)i / 255., global_gamma);

    done = 1;
}

