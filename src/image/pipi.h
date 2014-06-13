/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2009 Sam Hocevar <sam@hocevar.net>
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
 * pipi.h: the full libpipi public API
 */

#ifndef __PIPI_H__
#define __PIPI_H__

#include <pipi-types.h>

#undef __extern
#if defined(_DOXYGEN_SKIP_ME)
#elif defined(_WIN32) && defined(__LIBPIPI__)
#   define __extern extern __declspec(dllexport)
#else
#   define __extern extern
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* pipi_scan_t: this enum is a list of all possible scanning methods when
 * parsing an image’s pixels. Not all functions support all scanning paths. */
typedef enum
{
    PIPI_SCAN_RASTER = 0,
    PIPI_SCAN_SERPENTINE = 1
}
pipi_scan_t;

/* pipi_format_t: this enum is a list of all possible pixel formats for
 * our internal images. RGBA_U8 is the most usual format when an image has
 * just been loaded, but RGBA_F32 is a lot better for complex operations. */
typedef enum
{
    PIPI_PIXELS_UNINITIALISED = -1,

    PIPI_PIXELS_RGBA_U8 = 0,
    PIPI_PIXELS_BGR_U8 = 1,
    PIPI_PIXELS_RGBA_F32 = 2,
    PIPI_PIXELS_Y_F32 = 3,

    PIPI_PIXELS_MASK_U8 = 4,

    PIPI_PIXELS_MAX = 5
}
pipi_format_t;


typedef enum
{
    PIPI_COLOR_R = 1,
    PIPI_COLOR_G = 2,
    PIPI_COLOR_B = 4,
    PIPI_COLOR_A = 8,
    PIPI_COLOR_Y = 16
}
pipi_color_flag_t;

struct pixel_u32
{
    uint8_t r, g, b, a;
};
struct pixel_float
{
    float r, g, b, a;
};

typedef struct
{
    union
    {
        struct pixel_float pixel_float;
        struct pixel_u32   pixel_u32;
    };
}
pipi_pixel_t;

/* pipi_pixels_t: this structure stores a pixel view of an image. */
typedef struct
{
    void *pixels;
    int w, h, pitch, bpp;
    size_t bytes;
}
pipi_pixels_t;

/* pipi_tile_t: the internal tile type */
typedef struct pipi_tile pipi_tile_t;

/* pipi_image_t: the main image type */
typedef struct pipi_image pipi_image_t;

/* pipi_sequence_t: the image sequence type */
typedef struct pipi_sequence pipi_sequence_t;

/* pipi_context_t: the processing stack */
typedef struct pipi_context pipi_context_t;

/* pipi_histogram_t: the histogram type */
typedef struct pipi_histogram pipi_histogram_t;

/* pipi_command_t: the command type */
typedef struct
{
    char const *name;
    int argc;
}
pipi_command_t;

__extern pipi_pixel_t *pipi_get_color_from_string(const char* s);

__extern char const * pipi_get_version(void);

__extern pipi_context_t *pipi_create_context(void);
__extern void pipi_destroy_context(pipi_context_t *);
__extern pipi_command_t const *pipi_get_command_list(void);
__extern int pipi_command(pipi_context_t *, char const *, ...);

__extern pipi_tile_t *pipi_get_tile(pipi_image_t *, int, int, int,
                                    pipi_format_t, int);
__extern void pipi_release_tile(pipi_image_t *, pipi_tile_t *);
__extern pipi_tile_t *pipi_create_tile(pipi_format_t, int);

__extern pipi_image_t *pipi_load(char const *);
__extern pipi_image_t *pipi_load_stock(char const *);
__extern pipi_image_t *pipi_new(int, int);
__extern pipi_image_t *pipi_copy(pipi_image_t *);
__extern void pipi_free(pipi_image_t *);
__extern int pipi_save(pipi_image_t *, const char *);

__extern void pipi_set_gamma(double);
__extern pipi_pixels_t *pipi_get_pixels(pipi_image_t *, pipi_format_t);
__extern void pipi_release_pixels(pipi_image_t *, pipi_pixels_t *);
__extern void pipi_set_colorspace(pipi_image_t *, pipi_format_t);
__extern int pipi_get_image_width(pipi_image_t *img);
__extern int pipi_get_image_height(pipi_image_t *img);
__extern int pipi_get_image_pitch(pipi_image_t *img);
__extern int pipi_get_image_last_modified(pipi_image_t *img);
__extern const char* pipi_get_format_name(int format);


__extern double pipi_measure_msd(pipi_image_t *, pipi_image_t *);
__extern double pipi_measure_rmsd(pipi_image_t *, pipi_image_t *);

__extern pipi_image_t *pipi_resize_bresenham(pipi_image_t *, int, int);
__extern pipi_image_t *pipi_resize_bicubic(pipi_image_t *, int, int);
__extern pipi_image_t *pipi_crop(pipi_image_t *, int, int, int, int);

__extern pipi_image_t *pipi_render_random(int, int);
__extern pipi_image_t *pipi_render_bayer(int, int);
__extern pipi_image_t *pipi_render_halftone(int, int);

__extern pipi_image_t *pipi_rgb(pipi_image_t *, pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_red(pipi_image_t *);
__extern pipi_image_t *pipi_green(pipi_image_t *);
__extern pipi_image_t *pipi_blue(pipi_image_t *);
__extern pipi_image_t *pipi_blit(pipi_image_t *, pipi_image_t *, int, int);
__extern pipi_image_t *pipi_merge(pipi_image_t *, pipi_image_t *, double);
__extern pipi_image_t *pipi_mean(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_min(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_max(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_add(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_sub(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_difference(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_multiply(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_divide(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_screen(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_overlay(pipi_image_t *, pipi_image_t *);

__extern pipi_image_t *pipi_convolution(pipi_image_t *, int, int, double[]);
__extern pipi_image_t *pipi_gaussian_blur(pipi_image_t *, float);
__extern pipi_image_t *pipi_gaussian_blur_ext(pipi_image_t *,
                                            float, float, float, float, float);
__extern pipi_image_t *pipi_box_blur(pipi_image_t *, int);
__extern pipi_image_t *pipi_box_blur_ext(pipi_image_t *, int, int);
__extern pipi_image_t *pipi_brightness(pipi_image_t *, double);
__extern pipi_image_t *pipi_contrast(pipi_image_t *, double);
__extern pipi_image_t *pipi_autocontrast(pipi_image_t *);
__extern pipi_image_t *pipi_invert(pipi_image_t *);
__extern pipi_image_t *pipi_threshold(pipi_image_t *, double);
__extern pipi_image_t *pipi_hflip(pipi_image_t *);
__extern pipi_image_t *pipi_vflip(pipi_image_t *);
__extern pipi_image_t *pipi_rotate(pipi_image_t *, double);
__extern pipi_image_t *pipi_rotate90(pipi_image_t *);
__extern pipi_image_t *pipi_rotate180(pipi_image_t *);
__extern pipi_image_t *pipi_rotate270(pipi_image_t *);
__extern pipi_image_t *pipi_median(pipi_image_t *, int);
__extern pipi_image_t *pipi_median_ext(pipi_image_t *, int, int);
__extern pipi_image_t *pipi_dilate(pipi_image_t *);
__extern pipi_image_t *pipi_erode(pipi_image_t *);
__extern pipi_image_t *pipi_sine(pipi_image_t *, double, double,
                                 double, double);
__extern pipi_image_t *pipi_wave(pipi_image_t *, double, double,
                                 double, double);
__extern pipi_image_t *pipi_rgb2yuv(pipi_image_t *);
__extern pipi_image_t *pipi_yuv2rgb(pipi_image_t *);

__extern pipi_image_t *pipi_order(pipi_image_t *);

__extern pipi_image_t *pipi_tile(pipi_image_t *, int, int);
__extern int pipi_flood_fill(pipi_image_t *,
                           int, int, float, float, float, float);
__extern int pipi_draw_line(pipi_image_t *, int, int, int, int, uint32_t, int);
__extern int pipi_draw_rectangle(pipi_image_t *, int, int, int, int, uint32_t, int);
__extern int pipi_draw_polyline(pipi_image_t *, int const[], int const[],
                              int , uint32_t, int);
__extern int pipi_draw_bezier4(pipi_image_t *,int, int, int, int, int, int, int, int, uint32_t, int, int);
__extern pipi_image_t *pipi_reduce(pipi_image_t *, int, double const *);

__extern pipi_image_t *pipi_dither_ediff(pipi_image_t *, pipi_image_t *,
                                       pipi_scan_t);
__extern pipi_image_t *pipi_dither_ordered(pipi_image_t *, pipi_image_t *);
__extern pipi_image_t *pipi_dither_ordered_ext(pipi_image_t *, pipi_image_t *,
                                             double, double);
__extern pipi_image_t *pipi_dither_halftone(pipi_image_t *, double, double);
__extern pipi_image_t *pipi_dither_random(pipi_image_t *);
__extern pipi_image_t *pipi_dither_ostromoukhov(pipi_image_t *, pipi_scan_t);
__extern pipi_image_t *pipi_dither_dbs(pipi_image_t *);
__extern void pipi_dither_24to16(pipi_image_t *);

__extern pipi_histogram_t* pipi_new_histogram(void);
__extern int pipi_get_image_histogram(pipi_image_t *, pipi_histogram_t *, int);
__extern int pipi_free_histogram(pipi_histogram_t*);
__extern int pipi_render_histogram(pipi_image_t *, pipi_histogram_t*, int);

__extern pipi_sequence_t *pipi_open_sequence(char const *, int, int, int,
                                             int, int, int, int);
__extern int pipi_feed_sequence(pipi_sequence_t *, uint8_t const *, int, int);
__extern int pipi_close_sequence(pipi_sequence_t *);

#ifdef __cplusplus
}
#endif

#endif /* __PIPI_H__ */

