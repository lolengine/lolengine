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
 * ostromoukhov.c: Ostromoukhov dithering functions
 *
 * This module implements Ostromoukhov's simple error diffusion algorithm,
 * as introduced in the paper "A Simple and Efficient Error-Diffusion
 * Algorithm", Proceedings of SIGGRAPH 2001, in ACM Computer Graphics,
 * Annual Conference Series, pp. 567--572, 2001.
 *
 * TODO: the table is actually a piecewise linear function, so it should
 * be easy to change it into something that works perfectly with floats.
 */

#include "config.h"

#include "pipi.h"
#include "pipi-internals.h"

static int const table[][3] =
{
     {13, 0, 5}, {13, 0, 5}, {21, 0, 10}, {7, 0, 4},
     {8, 0, 5}, {47, 3, 28}, {23, 3, 13}, {15, 3, 8},
     {22, 6, 11}, {43, 15, 20}, {7, 3, 3}, {501, 224, 211},
     {249, 116, 103}, {165, 80, 67}, {123, 62, 49}, {489, 256, 191},
     {81, 44, 31}, {483, 272, 181}, {60, 35, 22}, {53, 32, 19},
     {237, 148, 83}, {471, 304, 161}, {3, 2, 1}, {481, 314, 185},
     {354, 226, 155}, {1389, 866, 685}, {227, 138, 125}, {267, 158, 163},
     {327, 188, 220}, {61, 34, 45}, {627, 338, 505}, {1227, 638, 1075},
     {20, 10, 19}, {1937, 1000, 1767}, {977, 520, 855}, {657, 360, 551},
     {71, 40, 57}, {2005, 1160, 1539}, {337, 200, 247}, {2039, 1240, 1425},
     {257, 160, 171}, {691, 440, 437}, {1045, 680, 627}, {301, 200, 171},
     {177, 120, 95}, {2141, 1480, 1083}, {1079, 760, 513}, {725, 520, 323},
     {137, 100, 57}, {2209, 1640, 855}, {53, 40, 19}, {2243, 1720, 741},
     {565, 440, 171}, {759, 600, 209}, {1147, 920, 285}, {2311, 1880, 513},
     {97, 80, 19}, {335, 280, 57}, {1181, 1000, 171}, {793, 680, 95},
     {599, 520, 57}, {2413, 2120, 171}, {405, 360, 19}, {2447, 2200, 57},
     {11, 10, 0}, {158, 151, 3}, {178, 179, 7}, {1030, 1091, 63},
     {248, 277, 21}, {318, 375, 35}, {458, 571, 63}, {878, 1159, 147},
     {5, 7, 1}, {172, 181, 37}, {97, 76, 22}, {72, 41, 17},
     {119, 47, 29}, {4, 1, 1}, {4, 1, 1}, {4, 1, 1},
     {4, 1, 1}, {4, 1, 1}, {4, 1, 1}, {4, 1, 1},
     {4, 1, 1}, {4, 1, 1}, {65, 18, 17}, {95, 29, 26},
     {185, 62, 53}, {30, 11, 9}, {35, 14, 11}, {85, 37, 28},
     {55, 26, 19}, {80, 41, 29}, {155, 86, 59}, {5, 3, 2},
     {5, 3, 2}, {5, 3, 2}, {5, 3, 2}, {5, 3, 2},
     {5, 3, 2}, {5, 3, 2}, {5, 3, 2}, {5, 3, 2},
     {5, 3, 2}, {5, 3, 2}, {5, 3, 2}, {5, 3, 2},
     {305, 176, 119}, {155, 86, 59}, {105, 56, 39}, {80, 41, 29},
     {65, 32, 23}, {55, 26, 19}, {335, 152, 113}, {85, 37, 28},
     {115, 48, 37}, {35, 14, 11}, {355, 136, 109}, {30, 11, 9},
     {365, 128, 107}, {185, 62, 53}, {25, 8, 7}, {95, 29, 26},
     {385, 112, 103}, {65, 18, 17}, {395, 104, 101}, {4, 1, 1}
};

pipi_image_t *pipi_dither_ostromoukhov(pipi_image_t *img, pipi_scan_t scan)
{
    pipi_image_t *dst;
    pipi_pixels_t *dstp;
    float *dstdata;
    int x, y, w, h;

    w = img->w;
    h = img->h;

    dst = pipi_copy(img);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_Y_F32);
    dstdata = (float *)dstp->pixels;

    for(y = 0; y < h; y++)
    {
        int reverse = (y & 1) && (scan == PIPI_SCAN_SERPENTINE);

        for(x = 0; x < w; x++)
        {
            float p, q, e;
            int x2, s, i;

            x2 = reverse ? w - 1 - x : x;
            s = reverse ? -1 : 1;

            p = dstdata[y * w + x2];
            q = p < 0.5 ? 0. : 1.;
            dstdata[y * w + x2] = q;

            e = p - q;
            i = p * 255.9999;
            if(i > 127) i = 255 - i;
            if(i < 0) i = 0; /* XXX: no "else" here */
            e /= table[i][0] + table[i][1] + table[i][2];

            if(x < w - 1)
                dstdata[y * w + x2 + s] += e * table[i][0];
            if(y < h - 1)
            {
                if(x > 0)
                    dstdata[(y + 1) * w + x2 - s] += e * table[i][1];
                dstdata[(y + 1) * w + x2] += e * table[i][2];
            }
        }
    }

    return dst;
}

