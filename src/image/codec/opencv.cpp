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
 * image.c: image I/O functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cv.h>
#include <highgui.h>

#include "pipi.h"
#include "pipi-internals.h"

/* FIXME: this whole file is broken until we support BGR24 images */

static int pipi_free_opencv(pipi_image_t *);

pipi_image_t *pipi_load_opencv(const char *name)
{
    pipi_image_t *img;

    IplImage *priv = cvLoadImage(name, 1);

    if(!priv)
        return NULL;

    img = pipi_new(priv->width, priv->height);

    img->p[PIPI_PIXELS_BGR_U8].pixels = priv->imageData;
    img->p[PIPI_PIXELS_BGR_U8].w = priv->width;
    img->p[PIPI_PIXELS_BGR_U8].h = priv->height;
    img->p[PIPI_PIXELS_BGR_U8].pitch = priv->widthStep;
    img->p[PIPI_PIXELS_BGR_U8].bpp = 24;
    img->p[PIPI_PIXELS_BGR_U8].bytes = 3 * img->w * img->h;
    img->last_modified = PIPI_PIXELS_BGR_U8;

    img->codec_priv = (void *)priv;
    img->codec_format = PIPI_PIXELS_BGR_U8;
    img->codec_free = pipi_free_opencv;

    img->wrap = 0;
    img->u8 = 1;

    return img;
}

int pipi_save_opencv(pipi_image_t *img, const char *name)
{
    if(!img->codec_priv)
    {
        IplImage *priv = cvCreateImage(cvSize(img->w, img->h),
                                       IPL_DEPTH_8U, 3);

        /* FIXME: check pitch differences here */
        if(img->last_modified == PIPI_PIXELS_BGR_U8)
        {
            memcpy(priv->imageData, img->p[PIPI_PIXELS_BGR_U8].pixels,
                   3 * img->w * img->h);
            free(img->p[PIPI_PIXELS_BGR_U8].pixels);
        }

        img->p[PIPI_PIXELS_BGR_U8].pixels = priv->imageData;
        img->p[PIPI_PIXELS_BGR_U8].w = priv->width;
        img->p[PIPI_PIXELS_BGR_U8].h = priv->height;
        img->p[PIPI_PIXELS_BGR_U8].pitch = priv->widthStep;
        img->p[PIPI_PIXELS_BGR_U8].bpp = 24;
        img->p[PIPI_PIXELS_BGR_U8].bytes = 3 * img->w * img->h;

        img->codec_priv = (void *)priv;
        img->codec_format = PIPI_PIXELS_BGR_U8;
        img->codec_free = pipi_free_opencv;

        img->wrap = 0;
        img->u8 = 1;
    }

    pipi_set_colorspace(img, img->codec_format);
    cvSaveImage(name, img->codec_priv, NULL);

    return 0;
}

/*
 * XXX: The following functions are local.
 */

static int pipi_free_opencv(pipi_image_t *img)
{
    IplImage *iplimg;
    iplimg = (IplImage *)img->codec_priv;
    cvReleaseImage(&iplimg);

    return 0;
}

