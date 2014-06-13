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
 * tiles.c: the tiles system
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

#ifdef USE_TILES
pipi_tile_t *pipi_get_tile(pipi_image_t *img, int x, int y, int zoom,
                           pipi_format_t fmt, int plane)
{
    pipi_tile_t * ret;
    int i;

    /* If the tile already exists, return it. */
    for(i = 0; i < img->ntiles; i++)
    {
        if(img->tiles[i]->x == x && img->tiles[i]->y == y
            && img->tiles[i]->fmt == fmt
            && img->tiles[i]->zoom == zoom
            && img->tiles[i]->plane == plane)
        {
            img->tiles[i]->refcount++;
            return img->tiles[i];
        }
    }

    /* Create a tile. When the image provides a tile creation function,
     * we should use it. */
    ret = pipi_create_tile(fmt, plane);
    ret->x = x;
    ret->y = y;
    ret->refcount = 1;

    /* Insert tile and return it. */
    img->ntiles++;
    img->tiles = realloc(img->tiles, img->ntiles * sizeof(pipi_tile_t *));
    img->tiles[img->ntiles - 1] = ret;

    return ret;
}

void pipi_release_tile(pipi_image_t *img, pipi_tile_t *tile)
{
    int i;

    for(i = 0; i < img->ntiles; i++)
    {
        if(img->tiles[i] == tile)
        {
            img->tiles[i]->refcount--;
            if(img->tiles[i]->refcount <= 0)
            {
                free(img->tiles[i]);
                img->tiles[i] = img->tiles[img->ntiles - 1];
                img->ntiles--;
            }
            return;
        }
    }
}

pipi_tile_t *pipi_create_tile(pipi_format_t fmt, int plane)
{
    pipi_tile_t * ret;
    size_t bytes;

    switch(fmt)
    {
        case PIPI_PIXELS_RGBA_U8:
        case PIPI_PIXELS_BGR_U8:
            bytes = sizeof(uint8_t) * TILE_SIZE * TILE_SIZE;
            break;
        case PIPI_PIXELS_RGBA_F32:
        case PIPI_PIXELS_Y_F32:
            bytes = sizeof(float) * TILE_SIZE * TILE_SIZE;
            break;
        default:
            bytes = 0;
            break;
    }

    ret = malloc(sizeof(pipi_tile_t) + bytes);
    ret->fmt = fmt;
    ret->plane = plane;
    ret->data.u8 = ret->align.u8;

    return ret;
}
#endif /* USE_TILES */

