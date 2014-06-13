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
 * colorstring.c: color from string functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"


struct color_table
{
    char name[255];
    float a,r,g,b;
};

struct color_table color_table[] =
{
    { "black"  , 1,   0,    0,    0 },
    { "white"  , 1,   1,    1,    1 },
    { "red"    , 1,   1,    0,    0 },
    { "green"  , 1,   0,    1,    0 },
    { "blue"   , 1,   0,    0,    1 },
    { "yellow" , 1,   1,    1,    0 },
    { "cyan"   , 1,   0,    1,    1 },
    { "magenta", 1,   1,    0,    1 },
    { "grey"   , 1,   0.5,  0.5,  0.5 },
    { "gray"   , 1,   0.5,  0.5,  0.5 },
    { "grey50" , 1,   0.5,  0.5,  0.5 },
    { "gray50" , 1,   0.5,  0.5,  0.5 },
    { "grey25" , 1,   0.25, 0.25, 0.25 },
    { "gray25" , 1,   0.25, 0.25, 0.25 },
};



pipi_pixel_t *pipi_get_color_from_string(const char* s)
{
    pipi_pixel_t *color;

    if(!s) return NULL;

    color = malloc(sizeof(pipi_pixel_t));

    if(s[0] == '#')
    {
        uint32_t c = 0;
        sscanf(s, "%x", &c);

        color->pixel_float.a = ((c&0xFF000000)>>24) / 255.0f;
        color->pixel_float.r = ((c&0x00FF0000)>>16) / 255.0f;
        color->pixel_float.g = ((c&0x0000FF00)>>8)  / 255.0f;
        color->pixel_float.b = ((c&0x000000FF)>>0)  / 255.0f;
    }
    else if(!strncmp(s, "rgb(", 4))
    {
        uint32_t r ,g ,b;
        sscanf(s, "rgb(%u,%u,%u)", &r, &g, &b);
        color->pixel_float.r = r / 255.0f;
        color->pixel_float.g = g / 255.0f;
        color->pixel_float.b = b / 255.0f;
    }
    else if(!strncmp(s, "frgb(", 5))
    {
        float r ,g ,b;
        sscanf(s, "frgb(%f,%f,%f)", &r, &g, &b);
        color->pixel_float.r = r;
        color->pixel_float.g = g;
        color->pixel_float.b = b;
    }
    else if(!strncmp(s, "argb(", 4))
    {
        uint32_t a, r ,g ,b;
        sscanf(s, "argb(%u,%u,%u,%u)", &a, &r, &g, &b);
        color->pixel_float.a = a / 255.0f;
        color->pixel_float.r = r / 255.0f;
        color->pixel_float.g = g / 255.0f;
        color->pixel_float.b = b / 255.0f;
    }
    else if(!strncmp(s, "fargb(", 5))
    {
        float a, r ,g ,b;
        sscanf(s, "fargb(%f, %f,%f,%f)", &a, &r, &g, &b);
        color->pixel_float.a = a;
        color->pixel_float.r = r;
        color->pixel_float.g = g;
        color->pixel_float.b = b;
    }
    else
    {
        unsigned int i;
        for(i=0; i<sizeof(color_table); i++)
        {
            if(!strcasecmp(color_table[i].name,s))
            {
                color->pixel_float.a = color_table[i].a;
                color->pixel_float.r = color_table[i].r;
                color->pixel_float.g = color_table[i].g;
                color->pixel_float.b = color_table[i].b;
                break;
            }
        }


    }
    return color;
}
