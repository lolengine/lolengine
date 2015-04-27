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
 * bezier.c: bezier curves rendering functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"


int pipi_draw_bezier4(pipi_image_t *img ,
                      int x1, int y1,
                      int x2, int y2,
                      int x3, int y3,
                      int x4, int y4,
                      uint32_t c, int n, int aa)
{
    if(img->last_modified == PIPI_PIXELS_RGBA_U8)
    {
        float t;
        float x= x1, y= y1;
        float lx, ly;
        for(t=0; t<1; t+=(1.0f/n))
        {
            float a = t;
            float b = 1 - t;

            lx = x; ly = y;

            x = (x1*(b*b*b)) + 3*x2*(b*b)*a + 3*x4*b*(a*a) + x3*(a*a*a);
            y = (y1*(b*b*b)) + 3*y2*(b*b)*a + 3*y4*b*(a*a) + y3*(a*a*a);

            pipi_draw_line(img , lx, ly, x, y, c, aa);
        }
        pipi_draw_line(img , x, y, x3, y3, c, aa);
    }

    return 0;
}
