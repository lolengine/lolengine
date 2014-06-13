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
 * pipi.c: core library routines
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

/** \brief Return the \e libpipi version.
 *
 *  Return a read-only string with the \e libpipi version information.
 *
 *  This function never fails.
 *
 *  \return The \e libpipi version information.
 */
char const * pipi_get_version(void)
{
    return PACKAGE_VERSION;
}

/*
static int init = 0;

void _pipi_init(void)
{
    if(init)
        return;

    _pipi_init_pixels();
}
*/

pipi_image_t *pipi_new(int w, int h)
{
    pipi_image_t *img;

    img = malloc(sizeof(pipi_image_t));
    memset(img, 0, sizeof(pipi_image_t));

    img->w = w;
    img->h = h;
    img->last_modified = PIPI_PIXELS_UNINITIALISED;
    img->codec_format = PIPI_PIXELS_UNINITIALISED;

    img->wrap = 0;
    img->u8 = 1;

    return img;
}

pipi_image_t *pipi_copy(pipi_image_t *src)
{
    pipi_image_t *dst = pipi_new(src->w, src->h);

    /* Copy properties */
    dst->wrap = src->wrap;
    dst->u8 = src->u8;

    /* Copy pixels, if any */
    if(src->last_modified != PIPI_PIXELS_UNINITIALISED)
    {
        pipi_pixels_t *srcp, *dstp;

        srcp = &src->p[src->last_modified];
        dstp = &dst->p[src->last_modified];

        memcpy(dstp, srcp, sizeof(pipi_pixels_t));
        dstp->pixels = malloc(dstp->bytes);
        memcpy(dstp->pixels, srcp->pixels, dstp->bytes);

        dst->last_modified = src->last_modified;
    }

    return dst;
}

