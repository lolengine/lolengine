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
 * context.c: processing stack handling routines
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_context_t *pipi_create_context()
{
    pipi_context_t *ret;

    ret = malloc(sizeof(pipi_context_t));
    memset(ret, 0, sizeof(pipi_context_t));

    return ret;
}

void pipi_destroy_context(pipi_context_t *ctx)
{
    free(ctx);
}

pipi_command_t const *pipi_get_command_list(void)
{
    static pipi_command_t const list[] =
    {
        { "load", 1 },
        { "save", 1 },

        { "dup", 0 },
        { "swap", 0 },
        { "roll", 1 },

        { "gamma", 1 },
        { "scale", 1 },
        { "crop", 1 },
        { "geometry", 1 },
        { "tile", 1 },
        { "dither", 1 },
        { "blur", 1 },
        { "boxblur", 1 },
        { "median", 1 },
        { "gray", 0 },
        { "brightness", 1 },
        { "contrast", 1 },
        { "autocontrast", 0 },
        { "order", 0 },
        { "hflip", 0 },
        { "vflip", 0 },
        { "rotate90", 0 },
        { "rotate180", 0 },
        { "rotate270", 0 },
        { "rotate", 1 },
        { "invert", 0 },
        { "threshold", 1 },
        { "dilate", 0 },
        { "erode", 0 },
        { "wrap", 0 },
        { "combine", 0 },
        { "split", 0 },
        { "blit", 1 },
        { "mean", 0 },
        { "merge", 1 },
        { "min", 0 },
        { "max", 0 },
        { "add", 0 },
        { "sub", 0 },
        { "difference", 0 },
        { "multiply", 0 },
        { "divide", 0 },
        { "screen", 0 },
        { "overlay", 0 },
        { "line", 1 },
        { "sine", 1 },
        { "wave", 1 },
        { "rgb2yuv", 0 },
        { "yuv2rgb", 0 },

        /* End marker */
        { NULL, 0 }
    };

    return list;
}

int pipi_command(pipi_context_t *ctx, char const *cmd, ...)
{
    if(!strcmp(cmd, "load"))
    {
        char const *file;
        va_list ap;

        va_start(ap, cmd);
        file = va_arg(ap, char const *);
        va_end(ap);
        ctx->images[ctx->nimages] = pipi_load(file);
        if(ctx->images[ctx->nimages] == NULL)
            return -1;
        ctx->nimages++;
    }
    else if(!strcmp(cmd, "save"))
    {
        char const *file;
        va_list ap;

        if(ctx->nimages < 1)
            return -1;
        ctx->nimages--;
        va_start(ap, cmd);
        file = va_arg(ap, char const *);
        va_end(ap);
        pipi_save(ctx->images[ctx->nimages], file);
        pipi_free(ctx->images[ctx->nimages]);
    }
    else if(!strcmp(cmd, "gamma"))
    {
        char const *val;
        va_list ap;

        va_start(ap, cmd);
        val = va_arg(ap, char const *);
        va_end(ap);

        pipi_set_gamma(atof(val));
    }
    else if(!strcmp(cmd, "dither"))
    {
        pipi_image_t *src, *dst;
        char const *method;
        va_list ap;

        if(ctx->nimages < 1)
            return -1;
        va_start(ap, cmd);
        method = va_arg(ap, char const *);
        va_end(ap);
        src = ctx->images[ctx->nimages - 1];
        dst = NULL;
        if(!strcmp(method, "ost"))
            dst = pipi_dither_ostromoukhov(src, 0);
        else if(!strcmp(method, "sost"))
            dst = pipi_dither_ostromoukhov(src, 1);
        else if(!strcmp(method, "ediff"))
        {
            if(ctx->nimages < 2)
                return -1;
            dst = pipi_dither_ediff(ctx->images[ctx->nimages - 2], src, 0);
            pipi_free(ctx->images[ctx->nimages - 2]);
            ctx->nimages--;
        }
        else if(!strcmp(method, "sediff"))
        {
            if(ctx->nimages < 2)
                return -1;
            dst = pipi_dither_ediff(ctx->images[ctx->nimages - 2], src, 1);
            pipi_free(ctx->images[ctx->nimages - 2]);
            ctx->nimages--;
        }
        else if(!strncmp(method, "ordered", 7))
        {
            double scale = 1., angle = .0;
            if(ctx->nimages < 2)
                return -1;
            method = strchr(method, ':');
            if(method)
            {
                scale = atof(method + 1);
                method = strchr(method + 1, ':');
                if(method)
                    angle = atof(method + 1);
            }
            if(scale <= 0.)
                scale = 1.;
            dst = pipi_dither_ordered_ext(ctx->images[ctx->nimages - 2], src,
                                          scale, angle);
            pipi_free(ctx->images[ctx->nimages - 2]);
            ctx->nimages--;
        }
        else if(!strncmp(method, "halftone", 8))
        {
            double r, angle = .0;
            method = strchr(method, ':');
            if(!method)
                return -1;
            r = atof(method + 1);
            method = strchr(method + 1, ':');
            if(method)
                angle = atof(method + 1);
            if(r < 1.)
                r = 1.;
            dst = pipi_dither_halftone(src, r, angle);
        }
        else if(!strcmp(method, "random"))
            dst = pipi_dither_random(src);
        else if(!strcmp(method, "dbs"))
            dst = pipi_dither_dbs(src);
        if(dst == NULL)
            return -1;
        pipi_free(src);
        ctx->images[ctx->nimages - 1] = dst;
    }
    else if(!strcmp(cmd, "blur"))
    {
        pipi_image_t *src, *dst;
        char const *arg;
        va_list ap;
        double w, h, a = 0.0;

        if(ctx->nimages < 1)
            return -1;
        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);
        w = h = atof(arg);
        arg = strchr(arg, 'x');
        if(arg)
        {
            h = atof(arg + 1);
            arg = strchr(arg, 'r');
            if(arg)
                a = atof(arg + 1);
        }
        src = ctx->images[ctx->nimages - 1];
        dst = pipi_gaussian_blur_ext(src, w, h, a, 0.0, 0.0);
        if(dst == NULL)
            return -1;
        pipi_free(src);
        ctx->images[ctx->nimages - 1] = dst;
    }
    else if(!strcmp(cmd, "boxblur") || !strcmp(cmd, "median"))
    {
        pipi_image_t *src, *dst = NULL;
        char const *arg;
        va_list ap;
        double w, h;

        if(ctx->nimages < 1)
            return -1;
        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);
        w = h = atof(arg);
        arg = strchr(arg, 'x');
        if(arg)
            h = atof(arg + 1);
        src = ctx->images[ctx->nimages - 1];
        switch(cmd[0])
        {
            case 'b': dst = pipi_box_blur_ext(src, w, h); break;
            case 'm': dst = pipi_median_ext(src, w, h); break;
        }
        if(dst == NULL)
            return -1;
        pipi_free(src);
        ctx->images[ctx->nimages - 1] = dst;
    }
    else if(!strcmp(cmd, "geometry") || !strcmp(cmd, "tile"))
    {
        pipi_image_t *src, *dst = NULL;
        char const *arg;
        va_list ap;
        int w, h;

        if(ctx->nimages < 1)
            return -1;
        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);
        w = atoi(arg);
        arg = strchr(arg, 'x');
        if(!arg)
            return -1;
        h = atoi(arg + 1);
        if(w <= 0 || h <= 0)
            return -1;
        src = ctx->images[ctx->nimages - 1];
        switch(cmd[0])
        {
            case 'g': dst = pipi_resize_bicubic(src, w, h); break;
            case 't': dst = pipi_tile(src, w, h); break;
        }
        if(dst == NULL)
            return -1;
        pipi_free(src);
        ctx->images[ctx->nimages - 1] = dst;
    }
    else if(!strcmp(cmd, "scale"))
    {
        pipi_image_t *src, *dst;
        char const *arg;
        va_list ap;
        double scale;
        int w, h;

        if(ctx->nimages < 1)
            return -1;
        src = ctx->images[ctx->nimages - 1];
        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);
        scale = atof(arg);
        w = (int)(scale * src->w + 0.5);
        h = (int)(scale * src->h + 0.5);
        if(w <= 0 || h <= 0)
            return -1;
        dst = pipi_resize_bicubic(src, w, h);
        if(dst == NULL)
            return -1;
        pipi_free(src);
        ctx->images[ctx->nimages - 1] = dst;
    }
    else if(!strcmp(cmd, "crop"))
    {
        pipi_image_t *tmp;
        char const *arg;
        va_list ap;
        int w, h, x = 0, y = 0;
        int ret;

        if(ctx->nimages < 1)
            return -1;

        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);

        ret = sscanf(arg, "%dx%d+%d+%d", &w, &h, &x, &y);
        if(ret < 2)
            return -1;

        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_crop(tmp, w, h, x, y);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "brightness") || !strcmp(cmd, "contrast")
             || !strcmp(cmd, "threshold") || !strcmp(cmd, "rotate"))
    {
        pipi_image_t *src, *dst = NULL;
        char const *arg;
        va_list ap;
        double val;

        if(ctx->nimages < 1)
            return -1;
        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);
        val = atof(arg);
        src = ctx->images[ctx->nimages - 1];
        switch(cmd[0])
        {
            case 'b': dst = pipi_brightness(src, val); break;
            case 'c': dst = pipi_contrast(src, val); break;
            case 'r': dst = pipi_rotate(src, val); break;
            case 't': dst = pipi_threshold(src, val); break;
        }
        if(dst == NULL)
            return -1;
        pipi_free(src);
        ctx->images[ctx->nimages - 1] = dst;
    }
    else if(!strcmp(cmd, "hflip"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_hflip(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "vflip"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_vflip(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "rotate90"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_rotate90(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "rotate180"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_rotate180(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "rotate270"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_rotate270(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "order"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_order(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "split"))
    {
        pipi_image_t *src;

        if(ctx->nimages < 1)
            return -1;
        src = ctx->images[ctx->nimages - 1];
        ctx->nimages += 2;
        ctx->images[ctx->nimages - 3] = pipi_red(src);
        ctx->images[ctx->nimages - 2] = pipi_green(src);
        ctx->images[ctx->nimages - 1] = pipi_blue(src);
        pipi_free(src);
    }
    else if(!strcmp(cmd, "combine"))
    {
        pipi_image_t *dst;

        if(ctx->nimages < 3)
            return -1;
        dst = pipi_rgb(ctx->images[ctx->nimages - 3],
                       ctx->images[ctx->nimages - 2],
                       ctx->images[ctx->nimages - 1]);
        if(dst == NULL)
            return -1;
        pipi_free(ctx->images[ctx->nimages - 3]);
        pipi_free(ctx->images[ctx->nimages - 2]);
        pipi_free(ctx->images[ctx->nimages - 1]);
        ctx->images[ctx->nimages - 3] = dst;
        ctx->nimages -= 2;
    }
    else if(!strcmp(cmd, "blit"))
    {
        pipi_image_t *dst;
        char const *arg;
        va_list ap;
        int x, y;

        if(ctx->nimages < 2)
            return -1;
        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);
        x = atoi(arg);
        arg = strchr(arg, 'x');
        if(!arg)
            return -1;
        y = atoi(arg + 1);

        dst = pipi_blit(ctx->images[ctx->nimages - 2],
                        ctx->images[ctx->nimages - 1], x, y);
        if(dst == NULL)
            return -1;
        pipi_free(ctx->images[ctx->nimages - 2]);
        pipi_free(ctx->images[ctx->nimages - 1]);
        ctx->images[ctx->nimages - 2] = dst;
        ctx->nimages--;
    }
    else if(!strcmp(cmd, "merge"))
    {
        pipi_image_t *dst;
        char const *arg;
        va_list ap;
        double val;

        if(ctx->nimages < 2)
            return -1;

        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);
        val = atof(arg);

        dst = pipi_merge(ctx->images[ctx->nimages - 2],
                         ctx->images[ctx->nimages - 1], val);
        if(dst == NULL)
            return -1;
        pipi_free(ctx->images[ctx->nimages - 2]);
        pipi_free(ctx->images[ctx->nimages - 1]);
        ctx->images[ctx->nimages - 2] = dst;
        ctx->nimages--;
    }
    else if(!strcmp(cmd, "mean") || !strcmp(cmd, "min") || !strcmp(cmd, "max")
             || !strcmp(cmd, "add") || !strcmp(cmd, "sub")
             || !strcmp(cmd, "difference") || !strcmp(cmd, "multiply")
             || !strcmp(cmd, "divide") || !strcmp(cmd, "screen")
             || !strcmp(cmd, "overlay"))
    {
        pipi_image_t *dst = NULL;

        if(ctx->nimages < 2)
            return -1;
        switch(cmd[2])
        {
            case 'a': dst = pipi_mean(ctx->images[ctx->nimages - 2],
                                      ctx->images[ctx->nimages - 1]);
                      break;
            case 'n': dst = pipi_min(ctx->images[ctx->nimages - 2],
                                     ctx->images[ctx->nimages - 1]);
                      break;
            case 'x': dst = pipi_max(ctx->images[ctx->nimages - 2],
                                     ctx->images[ctx->nimages - 1]);
                      break;
            case 'd': dst = pipi_add(ctx->images[ctx->nimages - 2],
                                     ctx->images[ctx->nimages - 1]);
                      break;
            case 'b': dst = pipi_sub(ctx->images[ctx->nimages - 2],
                                     ctx->images[ctx->nimages - 1]);
                      break;
            case 'f': dst = pipi_difference(ctx->images[ctx->nimages - 2],
                                            ctx->images[ctx->nimages - 1]);
                      break;
            case 'l': dst = pipi_multiply(ctx->images[ctx->nimages - 2],
                                          ctx->images[ctx->nimages - 1]);
                      break;
            case 'v': dst = pipi_divide(ctx->images[ctx->nimages - 2],
                                        ctx->images[ctx->nimages - 1]);
                      break;
            case 'r': dst = pipi_screen(ctx->images[ctx->nimages - 2],
                                        ctx->images[ctx->nimages - 1]);
                      break;
            case 'e': dst = pipi_overlay(ctx->images[ctx->nimages - 2],
                                         ctx->images[ctx->nimages - 1]);
                      break;
        }
        if(dst == NULL)
            return -1;
        pipi_free(ctx->images[ctx->nimages - 2]);
        pipi_free(ctx->images[ctx->nimages - 1]);
        ctx->images[ctx->nimages - 2] = dst;
        ctx->nimages--;
    }
    else if(!strcmp(cmd, "wrap"))
    {
        if(ctx->nimages < 1)
            return -1;
        ctx->images[ctx->nimages - 1]->wrap = 1;
    }
    else if(!strcmp(cmd, "autocontrast"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_autocontrast(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "invert"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_invert(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "dilate"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_dilate(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "erode"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 1)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = pipi_erode(tmp);
        pipi_free(tmp);
    }
    else if(!strcmp(cmd, "gray"))
    {
        if(ctx->nimages < 1)
            return -1;
        pipi_get_pixels(ctx->images[ctx->nimages - 1], PIPI_PIXELS_Y_F32);
    }
    else if(!strcmp(cmd, "free"))
    {
        if(ctx->nimages < 1)
            return -1;
        ctx->nimages--;
        pipi_free(ctx->images[ctx->nimages]);
    }
    else if(!strcmp(cmd, "dup"))
    {
        if(ctx->nimages < 1)
            return -1;
        ctx->images[ctx->nimages] = pipi_copy(ctx->images[ctx->nimages - 1]);
        ctx->nimages++;
    }
    else if(!strcmp(cmd, "swap"))
    {
        pipi_image_t *tmp;
        if(ctx->nimages < 2)
            return -1;
        tmp = ctx->images[ctx->nimages - 1];
        ctx->images[ctx->nimages - 1] = ctx->images[ctx->nimages - 2];
        ctx->images[ctx->nimages - 2] = tmp;
    }
    else if(!strcmp(cmd, "roll"))
    {
        pipi_image_t *tmp;
        char const *arg;
        va_list ap;
        int val;

        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);
        val = atoi(arg);
        if(val <= 0 || ctx->nimages < val)
            return -1;
        if(val == 1)
            return 0;
        tmp = ctx->images[ctx->nimages - val];
        memmove(ctx->images + ctx->nimages - val,
                ctx->images + ctx->nimages - val + 1,
                (val - 1) * sizeof(*ctx->images));
        ctx->images[ctx->nimages - 1] = tmp;
    }
    else if(!strcmp(cmd, "line"))
    {
        char const *arg;
        va_list ap;
        int x1, y1, x2, y2, aa = 0, ret;
        uint32_t color = 0;

        if(ctx->nimages < 1)
            return -1;

        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);

        ret = sscanf(arg, "%d,%d,%d,%d,%08x,%d",
               &x1, &y1, &x2, &y2, &color, &aa);
        if(ret < 5) return -1;

        ctx->images[ctx->nimages] = pipi_copy(ctx->images[ctx->nimages - 1]);
        pipi_draw_line(ctx->images[ctx->nimages],
                       x1,  y1,  x2,  y2, color, aa);
        ctx->nimages++;
    }
    else if(!strcmp(cmd, "sine") || !strcmp(cmd, "wave"))
    {
        pipi_image_t *src, *dst = NULL;
        char const *arg;
        va_list ap;
        float dw, dh, d = 0.0, a = 0.0;
        int ret;

        if(ctx->nimages < 1)
            return -1;

        va_start(ap, cmd);
        arg = va_arg(ap, char const *);
        va_end(ap);

        ret = sscanf(arg, "%gx%g+%gr%g", &dw, &dh, &d, &a);
        if(ret < 2)
            return -1;

        src = ctx->images[ctx->nimages - 1];
        switch(cmd[0])
        {
            case 's': dst = pipi_sine(src, dw, dh, d, a); break;
            case 'w': dst = pipi_wave(src, dw, dh, d, a); break;
        }
        if(dst == NULL)
            return -1;
        pipi_free(src);
        ctx->images[ctx->nimages - 1] = dst;
    }
    else if(!strcmp(cmd, "rgb2yuv") || !strcmp(cmd, "yuv2rgb"))
    {
        pipi_image_t *src, *dst = NULL;
        if(ctx->nimages < 1)
            return -1;
        src = ctx->images[ctx->nimages - 1];
        switch (cmd[0])
        {
            case 'r': dst = pipi_rgb2yuv(src); break;
            case 'y': dst = pipi_yuv2rgb(src); break;
        }
        if(dst == NULL)
            return -1;
        pipi_free(src);
        ctx->images[ctx->nimages - 1] = dst;
    }
    else
    {
        return -1;
    }

    return 0;
}

