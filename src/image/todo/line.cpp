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
 * line.c: line rendering functions
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

#if !defined TEMPLATE_FILE /* This file uses the template system */

static float fractf(float d) { return (d - floorf(d)); }
static float fractinvf(float d) { return (1 - (d - floorf(d))); }

struct line
{
    int xa, ya;
    int xb, yb;
    void (*draw) (pipi_image_t*, struct line*);
    union {
        uint32_t color32;
        float    colorf[3];
    };

    union {
        uint32_t *buf_u32;
        float    *buf_f;
    };

};

#define TEMPLATE_FLAGS SET_FLAG_GRAY | SET_FLAG_8BIT
#define TEMPLATE_FILE "paint/line.c"
#include "pipi-template.h"

static void clip_line(pipi_image_t*, struct line*);
static uint8_t clip_bits(pipi_image_t*, int, int);

int pipi_draw_line(pipi_image_t *img , int xa, int ya, int xb, int yb, uint32_t c, int aa)
{
    struct line s;
    s.xa = xa;
    s.ya = ya;
    s.xb = xb;
    s.yb = yb;


    /* No Transparency routine for u32 yet, fallback to float version */
    if(img->last_modified == PIPI_PIXELS_RGBA_U8)
    {
        if(!aa)
        {
            uint32_t  *dstdata;
            dstdata = (uint32_t *)pipi_get_pixels(img, PIPI_PIXELS_RGBA_U8)->pixels;
            s.color32 = c;
            s.buf_u32 = dstdata;
            s.draw = line_8bit;
        }
        else
        {
            float  *dstdata;
            dstdata = (float *)pipi_get_pixels(img, PIPI_PIXELS_RGBA_F32)->pixels;
            s.colorf[2] = ((c&0x00FF0000)>>16)/255.0f; /* XXX FIXME */
            s.colorf[1] = ((c&0x0000FF00)>>8)/255.0f;  /* XXX FIXME */
            s.colorf[0] = (c&0x000000FF)/255.0f;       /* XXX FIXME */
            s.buf_f = dstdata;
            s.draw = aaline;
        }
    }
    else if(img->last_modified == PIPI_PIXELS_Y_F32)
    {
        float  *dstdata;
        dstdata = (float *)pipi_get_pixels(img, PIPI_PIXELS_Y_F32)->pixels;
        s.colorf[0] = (c & 0xff) / 255.0f; /* XXX FIXME */
        s.buf_f = dstdata;
        s.draw = aa ? aaline_gray : line_gray;
    }
    else
    {
        float  *dstdata;
        dstdata = (float *)pipi_get_pixels(img, PIPI_PIXELS_RGBA_F32)->pixels;
        s.colorf[2] = ((c&0x00FF0000)>>16)/255.0f; /* XXX FIXME */
        s.colorf[1] = ((c&0x0000FF00)>>8)/255.0f;  /* XXX FIXME */
        s.colorf[0] = (c&0x000000FF)/255.0f;       /* XXX FIXME */
        s.buf_f = dstdata;
        s.draw = aa ? aaline : line;
    }

    clip_line(img, &s);
    return 0;
}

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

int pipi_draw_polyline(pipi_image_t *img, int const x[], int const y[],
                       int n, uint32_t c, int aa)
{
    int i;

    for(i = 0; i < n; i++)
        pipi_draw_line(img, x[i], y[i], x[i + 1], y[i + 1], c, aa);

    return 0;
}

/*
 * XXX: The following functions are local.
 */

/* Generic Cohen-Sutherland line clipping function. */
static void clip_line(pipi_image_t *img, struct line* s)
{
    uint8_t bits1, bits2;

    bits1 = clip_bits(img, s->xa, s->ya);
    bits2 = clip_bits(img, s->xb, s->yb);

    if(bits1 & bits2)
        return;

    if(bits1 == 0)
    {
        if(bits2 == 0)
            s->draw(img, s);
        else
        {
            int tmp;
            tmp = s->xa; s->xa = s->xb; s->xb = tmp;
            tmp = s->ya; s->ya = s->yb; s->yb = tmp;
            clip_line(img, s);
        }
        return;
    }

    if(bits1 & (1<<0))
    {
        s->ya = s->yb - (s->xb - 0) * (s->yb - s->ya) / (s->xb - s->xa);
        s->xa = 0;
    }
    else if(bits1 & (1<<1))
    {
        int xmax = img->w - 1;
        s->ya = s->yb - (s->xb - xmax) * (s->yb - s->ya) / (s->xb - s->xa);
        s->xa = xmax;
    }
    else if(bits1 & (1<<2))
    {
        s->xa = s->xb - (s->yb - 0) * (s->xb - s->xa) / (s->yb - s->ya);
        s->ya = 0;
    }
    else if(bits1 & (1<<3))
    {
        int ymax = img->h - 1;
        s->xa = s->xb - (s->yb - ymax) * (s->xb - s->xa) / (s->yb - s->ya);
        s->ya = ymax;
    }

    clip_line(img, s);
}

/* Helper function for clip_line(). */
static uint8_t clip_bits(pipi_image_t *img, int x, int y)
{
    uint8_t b = 0;

    if(x < 0)
        b |= (1<<0);
    else if(x >= (int)img->w)
        b |= (1<<1);

    if(y < 0)
        b |= (1<<2);
    else if(y >= (int)img->h)
        b |= (1<<3);

    return b;
}

#else /* XXX: the following functions use the template system */

/* Xiaolin Wu's line algorithm, as seen at http://portal.acm.org/citation.cfm?id=122734 */

#define PLOT(x, y, c) \
    if(FLAG_8BIT) \
    { \
        /* TODO */ \
    } \
    else \
    { \
        if(FLAG_GRAY) \
        { \
            s->buf_f[((int)(x))+((int)(y))*img->w] =  \
            (c*s->colorf[0]) + (1-c) * s->buf_f[((int)(x))+((int)(y))*img->w]; \
            if(s->buf_f[((int)(x))+((int)(y))*img->w] > 1.0f) \
                s->buf_f[((int)(x))+((int)(y))*img->w] = 1.0f; \
            if(s->buf_f[((int)(x))+((int)(y))*img->w] < 0.0f) \
                s->buf_f[((int)(x))+((int)(y))*img->w] = 0.0f; \
        } \
        else \
        { \
            int qwer = (((int)(x)*4))+((int)(y))*(img->w*4);\
            int qweg = (1+((int)(x)*4))+((int)(y))*(img->w*4); \
            int qweb = (2+((int)(x)*4))+((int)(y))*(img->w*4); \
            s->buf_f[qwer] = (c*s->colorf[0]) + (1-c) * s->buf_f[qwer]; \
            s->buf_f[qweg] = (c*s->colorf[1]) + (1-c) * s->buf_f[qweg]; \
            s->buf_f[qweb] = (c*s->colorf[2]) + (1-c) * s->buf_f[qweb]; \
            if(s->buf_f[qwer] > 1.0f) s->buf_f[qwer] = 1.0f; \
            if(s->buf_f[qwer] < 0.0f) s->buf_f[qwer] = 0.0f; \
            if(s->buf_f[qweg] > 1.0f) s->buf_f[qweg] = 1.0f; \
            if(s->buf_f[qweg] < 0.0f) s->buf_f[qweg] = 0.0f; \
            if(s->buf_f[qweb] > 1.0f) s->buf_f[qweb] = 1.0f; \
            if(s->buf_f[qweb] < 0.0f) s->buf_f[qweb] = 0.0f; \
        } \
    }

static void T(aaline)(pipi_image_t *img, struct line* s)
{
    float xa = s->xa, ya = s->ya, xb = s->xb, yb = s->yb;
    float g, xd, yd, xgap, xend, yend, xf, yf, val1, val2;
    int x, y, ixa, ixb, iya, iyb;

    xd = xb - xa;
    yd = yb - ya;

    /* "Horizontal" line (X greater than Y)*/
    if (fabsf(xd) > fabsf(yd))
    {
        if (xa > xb)
        {
            float tmp;
            tmp = xa; xa = xb; xb = tmp;
            tmp = ya; ya = yb; yb = tmp;
            xd = (xb-xa);
            yd = (yb-ya);
        }
        g = yd/xd;

        xend = (float)(int)(xa+0.5);
        yend = ya + g*(xend-xa);
        xgap = fractinvf(xa+0.5);
        ixa = (int)xend;
        iya = (int)yend;
        val1 = fractinvf(yend)*xgap;
        val2 = fractf(yend)*xgap;

        PLOT(ixa, iya,   val1);
        PLOT(ixa, (iya+1)<ya?(iya+1):iya, val2);

        yf = yend+g;
        xend = (float)(int)(xb+0.5);
        yend = yb + g*(xend-xb);
        xgap = fractinvf(xb-0.5);
        ixb = (int)xend;
        iyb = (int)yend;
        val1 = fractinvf(yend)*xgap;
        val2 = fractf(yend)*xgap;

        PLOT(ixb, iyb,   val1);
        PLOT(ixb, iyb+1<yb?iyb+1:iyb, val2);

        for (x = (ixa+1); x < ixb; x++)
        {
            float focus;

            val1 = fractinvf(yf);
            val2 = fractf(yf);
            focus = (1.0 - fabsf(val1-val2));
            val1 += 0.3*focus;
            val2 += 0.3*focus;

            PLOT(x, yf, val1);
            PLOT(x, (yf+1)<ya?(yf+1):yf, val2);

            yf = yf + g;
        }
    }
    /* "Vertical" line (Y greater than X)*/
    else
    {
        if (xa > xb)
        {
            float tmp;
            tmp = xa; xa = xb; xb = tmp;
            tmp = ya; ya = yb; yb = tmp;
            xd = (xb-xa);
            yd = (yb-ya);
        }

        g = xd/yd;

        xend = (float)(int)(xa+0.5);
        yend = ya + g*(xend-xa);
        xgap = fractf(xa+0.5);
        ixa = (int)xend;
        iya = (int)yend;
        val1 = fractinvf(yend)*xgap;
        val2 = fractf(yend)*xgap;

        PLOT(ixa, iya, val1);
        PLOT(ixa, (iya+1)<ya?(iya+1):iya, val2);

        xf = xend + g;

        xend = (float)(int)(xb+0.5);
        yend = yb + g*(xend-xb);
        xgap = fractinvf(xb-0.5);
        ixb = (int)xend;
        iyb = (int)yend;
        val1 = fractinvf(yend)*xgap;
        val2 = fractf(yend)*xgap;

        PLOT(ixb, iyb,   val1);
        PLOT(ixb, (iyb+1)<yb?(iyb+1):iyb, val2);


        for (y = (iya+1); y < iyb; y++)
        {
            float focus;
            int   vx = xf;
            val1 = fractinvf(xf);
            val2 = fractf(xf);
            focus = (1.0 - fabsf(val1-val2));
            val1 += 0.3*focus;
            val2 += 0.3*focus;
            PLOT(vx, y, val1);
            vx++;
            PLOT(vx, y, val2);
            xf = xf + g;
        }
    }
}

#undef PLOT

/* Solid line drawing function, using Bresenham's mid-point line
 * scan-conversion algorithm. */
static void T(line)(pipi_image_t *img, struct line* s)
{
    int xa, ya, xb, yb;
    int dx, dy;
    int xinc, yinc;

    xa = s->xa; ya = s->ya; xb = s->xb; yb = s->yb;

    dx = abs(xb - xa);
    dy = abs(yb - ya);

    xinc = (xa > xb) ? -1 : 1;
    yinc = (ya > yb) ? -1 : 1;

    if(dx >= dy)
    {
        int dpr = dy << 1;
        int dpru = dpr - (dx << 1);
        int delta = dpr - dx;

        for(; dx >= 0; dx--)
        {
            if(FLAG_GRAY)
            {
                if(FLAG_8BIT)
                    /* TODO */;
                else
                    s->buf_f[xa + ya * img->w] = s->colorf[0];
            }
            else
            {
                if(FLAG_8BIT)
                    s->buf_u32[xa + ya * img->w] = s->color32;
                else
                {
                    s->buf_f[4 * (ya * img->w + xa)] = s->colorf[0];
                    s->buf_f[4 * (ya * img->w + xa) + 1] = s->colorf[1];
                    s->buf_f[4 * (ya * img->w + xa) + 2] = s->colorf[2];
                }
            }

            if(delta > 0)
            {
                xa += xinc;
                ya += yinc;
                delta += dpru;
            }
            else
            {
                xa += xinc;
                delta += dpr;
            }
        }
    }
    else
    {
        int dpr = dx << 1;
        int dpru = dpr - (dy << 1);
        int delta = dpr - dy;

        for(; dy >= 0; dy--)
        {
            if(FLAG_GRAY)
            {
                if(FLAG_8BIT)
                    /* TODO */;
                else
                    s->buf_f[xa + ya * img->w] = s->colorf[0];
            }
            else
            {
                if(FLAG_8BIT)
                    s->buf_u32[xa + ya * img->w] = s->color32;
                else
                {
                    s->buf_f[4 * (ya * img->w + xa)] = s->colorf[0];
                    s->buf_f[4 * (ya * img->w + xa) + 1] = s->colorf[1];
                    s->buf_f[4 * (ya * img->w + xa) + 2] = s->colorf[2];
                }
            }

            if(delta > 0)
            {
                xa += xinc;
                ya += yinc;
                delta += dpru;
            }
            else
            {
                ya += yinc;
                delta += dpr;
            }
        }
    }
}

#endif

