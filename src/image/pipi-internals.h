/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2009 Sam Hocevar <sam@hocevar.net>
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
 *  pipi-internals.h: internal types
 */

#ifndef __PIPI_INTERNALS_H__
#define __PIPI_INTERNALS_H__

#include "pipi-stubs.h"

#define SET_FLAG_GRAY 0x00000001
#define SET_FLAG_WRAP 0x00000002
#define SET_FLAG_8BIT 0x00000004

struct pipi_histogram
{
    int r_present, g_present, b_present, y_present;
    unsigned int a[256];
    unsigned int r[256];
    unsigned int g[256];
    unsigned int b[256];
    unsigned int y[256];
};

#ifdef USE_TILES
#define TILE_SIZE 128

struct pipi_tile
{
    int x, y;
    int zoom;

    int refcount;

    pipi_format_t fmt;
    int plane;
    union { uint8_t *u8; float *f; double *d; } data;
    union { uint8_t u8[1]; float f[1]; double d[1]; } align;
};
#endif /* USE_TILES */

/* pipi_image_t: the image structure. This is probably going to be the most
 * complex structure in the library, but right now it only has fairly normal
 * stuff, like width and height and pointers to pixel areas. */
struct pipi_image
{
    int w, h, pitch;

#ifdef USE_TILES
    pipi_tile_t **tiles;
    int ntiles;
#endif /* USE_TILES */

    /* A list of internal image flags.
     *  wrap: should filters wrap around at edges?
     *  u8: are the image samples still 8-bit per channel? */
    int wrap, u8;

    /* Translation vectors for wrap around and tiling. */
    int wrapx1, wrapy1, wrapx2, wrapy2;

    /* List of all possible pixel formats and the last active one. */
    pipi_pixels_t p[PIPI_PIXELS_MAX];
    pipi_format_t last_modified;

    /* Private data used by the codec */
    pipi_format_t codec_format;
    void *codec_priv;
    int (*codec_free)(pipi_image_t *);
};

struct pipi_sequence
{
    int w, h, fps;
    uint8_t *convert_buf;

    void *codec_priv;
};

struct pipi_context
{
    int nimages;
    pipi_image_t *images[1024]; /* FIXME: do dynamic allocation */
};

#ifdef USE_IMLIB2
pipi_image_t *pipi_load_imlib2(const char *name);
int pipi_save_imlib2(pipi_image_t *img, const char *name);
#endif

#ifdef USE_OPENCV
pipi_image_t *pipi_load_opencv(const char *name);
int pipi_save_opencv(pipi_image_t *img, const char *name);
#endif

#ifdef USE_SDL
pipi_image_t *pipi_load_sdl(const char *name);
int pipi_save_sdl(pipi_image_t *img, const char *name);
#endif

#ifdef USE_GDIPLUS
pipi_image_t *pipi_load_gdiplus(const char *name);
int pipi_save_gdiplus(pipi_image_t *img, const char *name);
#endif

#ifdef USE_GDI
pipi_image_t *pipi_load_gdi(const char *name);
int pipi_save_gdi(pipi_image_t *img, const char *name);
#endif

#ifdef USE_COCOA
pipi_image_t *pipi_load_coreimage(const char *name);
int pipi_save_coreimage(pipi_image_t *img, const char *name);
#endif

#ifdef USE_JPEG
pipi_image_t *pipi_load_jpeg(const char *name);
int pipi_save_jpeg(pipi_image_t *img, const char *name);
#endif

pipi_image_t *pipi_load_oric(const char *name);
int pipi_save_oric(pipi_image_t *img, const char *name);

#endif /* __PIPI_INTERNALS_H__ */

