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
 * floodfill.c: flood fill (4 neighbours) functions
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pipi.h"
#include "pipi-internals.h"

#define STACKSIZE (1<<20)

static void  pipi_flood_fill_stack_scanline_u32(pipi_pixels_t *dstp,
                                                int x,    int y,
                                                uint32_t new, uint32_t old);
static void  pipi_flood_fill_stack_scanline_float(pipi_pixels_t *dstp,
                                                  int x,    int y,
                                                  float nr, float ng, float nb, float na,
                                                  float or, float og, float ob, float oa);
static int   pop (int *x,int *y, int h);
static int   push(int x, int y, int h);
static void  clear_stack(int h);
static int   stack[STACKSIZE];
static int   stack_pointer;

static float get_max_color_diff(float r1, float g1, float b1,
                                float r2, float g2, float b2);
static int   validate_pixel_f(float r1, float g1, float b1,
                              float r2, float g2, float b2);

/*
  Stack based flood fill.
  Instead of using system stack (calling recursively functions,
  which can lead to big problems as this stack is a fixed and small sized one),
  we create our own one.
  Additionnaly, we use a scanline trick to speed up the whole thing.

  This method is more or less the one described at
  http://student.kuleuven.be/~m0216922/CG/floodfill.html

*/

/* Public function */
int pipi_flood_fill(pipi_image_t *src,
                    int px, int py,
                    float r, float g, float b, float a)
{
    pipi_image_t  *dst = src;
    pipi_pixels_t *dstp;
    int w, h;

    if(!src) return -1;

    w = src->w;
    h = src->h;

    if((px >= src->w) || (py >= src->h) ||
       (px < 0) || (py < 0)) return -1;


    if(src->last_modified == PIPI_PIXELS_RGBA_U8) {
        uint32_t  *dstdata;
        unsigned char nr, ng, nb, na;

        /* Get ARGB32 pointer */
        dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_U8);
        dstdata = (uint32_t *)dstp->pixels;

        nr = r*255.0f;
        ng = g*255.0f;
        nb = b*255.0f;
        na = a*255.0f;

        dstp->w = w;
        dstp->h = h;
        pipi_flood_fill_stack_scanline_u32(dstp, px, py, (na<<24)|(nr<<16)|(ng<<8)|(nb), dstdata[px+py*w]);

    } else {
        int gray = (dst->last_modified == PIPI_PIXELS_Y_F32);
        float *dstdata;
        float or, og, ob, oa;

        dstp = gray ? pipi_get_pixels(dst, PIPI_PIXELS_Y_F32)
            : pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);

        dstdata = (float *)dstp->pixels;

        or = dstdata[(px+py*w)*4];
        og = dstdata[(px+py*w)*4 + 1];
        ob = dstdata[(px+py*w)*4 + 2];
        oa = dstdata[(px+py*w)*4 + 3];

        dstp->w = w;
        dstp->h = h;

        pipi_flood_fill_stack_scanline_float(dstp, px, py, r, g, b, a, or, og, ob, oa);
    }

    return 0;
}


/* ARGB32 */
void pipi_flood_fill_stack_scanline_u32(pipi_pixels_t *dstp,
                                        int x,    int y,
                                        uint32_t new, uint32_t old)
{
    int yt;
    int left, right;

    uint32_t *dstdata = (uint32_t *)dstp->pixels;

    if(new==old) return;

    clear_stack(dstp->h);

    if(!push(x, y, dstp->h)) return;

    while(pop(&x, &y, dstp->h))
    {
        yt = y;
        while(yt >= 0 && (dstdata[x+yt*dstp->w] == old)) {
            yt--;
        }

        yt++;
        left = right = 0;

        while(yt < dstp->h && (dstdata[x+yt*dstp->w] == old))
        {
            uint32_t *cur_line = &dstdata[(yt*dstp->w)];
            int   xp1 = (x+1);
            int   xm1 = (x-1);
            cur_line[x]     = new;

            if(!left && x > 0 && (cur_line[xm1] == old))
            {
                if(!push(x - 1, yt, dstp->h)) return;
                left = 1;
            }
            else if(left && x > 0 && (cur_line[xm1] != old))
            {
                left = 0;
            }
            if(!right && x < dstp->w - 1 && (cur_line[xp1] == old))
            {
                if(!push(x + 1, yt, dstp->h)) return;
                right = 1;
            }
            else if(right && x < dstp->w - 1 && (cur_line[xp1] != old))
            {
                right = 0;
            }
            yt++;
        }
    }
}

/* Float version. Much slower, but supports HDR and (soon antialiasing) */
static void pipi_flood_fill_stack_scanline_float(pipi_pixels_t *dstp,
                                                 int x,    int y,
                                                 float nr, float ng, float nb, float na,
                                                 float or, float og, float ob, float oa)
{
    int yt;
    int left, right;
    float *dstdata = (float *)dstp->pixels;

    if((nr==or) && (ng==og) && (nb==ob)) return;

    clear_stack(dstp->h);

    if(!push(x, y, dstp->h)) return;

    while(pop(&x, &y, dstp->h))
    {
        yt = y;
        while(yt >= 0 && validate_pixel_f(dstdata[(x+yt*dstp->w)*4] ,
                                          dstdata[(x+yt*dstp->w)*4 + 1] ,
                                          dstdata[(x+yt*dstp->w)*4 + 2] ,
                                          or, og, ob)) {
            yt--;
        }

        yt++;
        left = right = 0;

        while(yt < dstp->h && validate_pixel_f(dstdata[(x+yt*dstp->w)*4] ,
                                               dstdata[(x+yt*dstp->w)*4 + 1] ,
                                               dstdata[(x+yt*dstp->w)*4 + 2] ,
                                               or, og, ob))
        {
            float *cur_line = &dstdata[(yt*dstp->w)*4];
            int   xp1 = (x+1)*4;
            int   xm1 = (x-1)*4;
            cur_line[x*4]     = nr;
            cur_line[x*4 + 1] = ng;
            cur_line[x*4 + 2] = nb;
            cur_line[x*4 + 3] = na;

            if(!left && x > 0 && validate_pixel_f(cur_line[xm1],
                                                  cur_line[xm1 + 1],
                                                  cur_line[xm1 + 2],
                                                  or, og, ob))
            {
                if(!push(x - 1, yt, dstp->h)) return;
                left = 1;
            }
            else if(left && x > 0 && !validate_pixel_f(cur_line[xm1] ,
                                                       cur_line[xm1 + 1] ,
                                                       cur_line[xm1 + 2] ,
                                                       or, og, ob))
            {
                left = 0;
            }
            if(!right && x < dstp->w - 1 && validate_pixel_f(cur_line[xp1] ,
                                                             cur_line[xp1 + 1] ,
                                                             cur_line[xp1 + 2] ,
                                                             or, og, ob))
            {
                if(!push(x + 1, yt, dstp->h)) return;
                right = 1;
            }
            else if(right && x < dstp->w - 1 && !validate_pixel_f(cur_line[xp1] ,
                                                                  cur_line[xp1 + 1] ,
                                                                  cur_line[xp1 + 2],
                                                                  or, og, ob))
            {
                right = 0;
            }
            yt++;
        }
    }
}


/* Following functions are local  */

static int pop(int *x, int *y, int h)
{
    if(stack_pointer > 0)
    {
        int p = stack[stack_pointer];
        *x = p / h;
        *y = p % h;
        stack_pointer--;
        return 1;
    }
    else
    {
        return 0;
    }
}

static int push(int x, int y, int h)
{
    if(stack_pointer < STACKSIZE - 1)
    {
        stack_pointer++;
        stack[stack_pointer] = h * x + y;
        return 1;
    }
    else
    {
        return 0;
    }
}

static void clear_stack(int h)
{
    int x, y;
    while(pop(&x, &y, h));
}

#define MAX(a, b) (b>a?b:a)


/* FIXME : Paves the way to antialiasing, but could be only 3 tests */
static float get_max_color_diff(float r1, float g1, float b1,
                                float r2, float g2, float b2)
{
    float r = abs(r2-r1);
    float g = abs(g2-g1);
    float b = abs(b2-b1);

    return MAX(MAX(r, g), b);
}


static int validate_pixel_f(float r1, float g1, float b1,
                            float r2, float g2, float b2)
{

    if(get_max_color_diff(r1, g1, b1,
                          r2, g2, b2)
       == 0) return 1;
    else
        return 0;
}
