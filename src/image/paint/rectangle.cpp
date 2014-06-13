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
 * rectangle.c: rectangle rendering functions
 */

/* XXX: this file is a JOKE, don't use it */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

int pipi_draw_rectangle(pipi_image_t *img , int xa, int ya, int xb, int yb, uint32_t c, int aa)
{
    while(ya < yb)
    {
        pipi_draw_line(img, xa, ya, xb, ya, c, aa);
        ya++;
    }

    while(ya > yb)
    {
        pipi_draw_line(img, xa, ya, xb, ya, c, aa);
        ya--;
    }

    return pipi_draw_line(img, xa, yb, xb, yb, c, aa);
}

