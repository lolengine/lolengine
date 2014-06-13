/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2008 Sam Hocevar <sam@zoy.org>
 *                2008 Jean-Yves Lamoureux <jylam@lnxscene.org>
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
 * accessors.c: accessors for various informations about images
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

int pipi_get_image_width(pipi_image_t *img)
{
    return img->w;
}
int pipi_get_image_height(pipi_image_t *img)
{
    return img->h;
}
int pipi_get_image_pitch(pipi_image_t *img)
{
    return img->pitch;
}
int pipi_get_image_last_modified(pipi_image_t *img)
{
    return img->last_modified;
}



char formats[][100] =
{
    "Unknow",
    "RGBA_C",
    "BGR_C",
    "RGBA_F",
    "Y_F",
    "MASK_C",
    "LOL",
};

const char* pipi_get_format_name(int format)
{
    if(format>PIPI_PIXELS_MAX) return "Invalid";
    else return formats[format];
}
