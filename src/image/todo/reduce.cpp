/*
 *  Lol Engine
 *
 *  Copyright © 2004—2008 Sam Hocevar <sam@hocevar.net>
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What the Fuck You Want
 *  to Public License, Version 2, as published by the WTFPL Task Force.
 *  See http://www.wtfpl.net/ for more details.
 */

/*
 * reduce.c: palette reduction routines
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

#include "pipi.h"
#include "pipi-internals.h"

#define R 0
#define G 1
#define B 2
#define X 3
#define Y 4
#define A 5

#define BRIGHT(x) (0.299*(x)[0] + 0.587*(x)[1] + 0.114*(x)[2])

#define MAXCOLORS 16
#define STEPS 1024
#define EPSILON (0.000001)

typedef struct
{
    double pts[STEPS + 1][MAXCOLORS * (MAXCOLORS - 1) / 2][6];
    int hullsize[STEPS + 1];
    double bary[STEPS + 1][3];
}
hull_t;

static double const y[3] = { .299, .587, .114 };
static double u[3], v[3];
static int ylen;

/*
 * Find two base vectors for the chrominance planes.
 */
static void init_uv(void)
{
    double tmp;

    ylen = sqrt(y[R] * y[R] + y[G] * y[G] + y[B] * y[B]);

    u[R] = y[1];
    u[G] = -y[0];
    u[B] = 0;
    tmp = sqrt(u[R] * u[R] + u[G] * u[G] + u[B] * u[B]);
    u[R] /= tmp; u[G] /= tmp; u[B] /= tmp;

    v[R] = y[G] * u[B] - y[B] * u[G];
    v[G] = y[B] * u[R] - y[R] * u[B];
    v[B] = y[R] * u[G] - y[G] * u[R];
    tmp = sqrt(v[R] * v[R] + v[G] * v[G] + v[B] * v[B]);
    v[R] /= tmp; v[G] /= tmp; v[B] /= tmp;
}

/*
 * Compute the convex hull of a given palette.
 */
static hull_t *compute_hull(int ncolors, double const *palette)
{
    double pal[MAXCOLORS][3];
    double gray[3];
    double *dark = NULL, *light = NULL;
    double tmp, min = 1.0, max = 0.0;
    hull_t *ret = malloc(sizeof(hull_t));
    int i, j, n;

    debug("");
    debug("### NEW HULL ###");
    debug("");

    debug("Analysing %i colors", ncolors);

    for(i = 0; i < ncolors; i++)
    {
        pal[i][R] = palette[i * 3];
        pal[i][G] = palette[i * 3 + 1];
        pal[i][B] = palette[i * 3 + 2];
        debug("  [%i] (%g,%g,%g)", i, pal[i][R], pal[i][G], pal[i][B]);
    }

    /*
     * 1. Find the darkest and lightest colours
     */
    for(i = 0; i < ncolors; i++)
    {
        double p = BRIGHT(pal[i]);
        if(p < min)
        {
            dark = pal[i];
            min = p;
        }
        if(p > max)
        {
            light = pal[i];
            max = p;
        }
    }

    gray[R] = light[R] - dark[R];
    gray[G] = light[G] - dark[G];
    gray[B] = light[B] - dark[B];

    debug("  gray axis (%g,%g,%g) - (%g,%g,%g)",
          dark[R], dark[G], dark[B], light[R], light[G], light[B]);

    /*
     * 3. Browse the grey axis and do stuff
     */
    for(n = 0; n <= STEPS; n++)
    {
        double pts[MAXCOLORS * (MAXCOLORS - 1) / 2][5];
        double ptmp[5];
        double p0[3];
#define SWAP(p1,p2) do { memcpy(ptmp, p1, sizeof(ptmp)); \
                         memcpy(p1, p2, sizeof(ptmp)); \
                         memcpy(p2, ptmp, sizeof(ptmp)); } while(0)
        double ctx, cty, weight;
        double t = n * 1.0 / STEPS;
        int npts = 0, left;

        debug("Slice %i/%i", n, STEPS);

        p0[R] = dark[R] + t * gray[R];
        p0[G] = dark[G] + t * gray[G];
        p0[B] = dark[B] + t * gray[B];

        debug("  3D gray (%g,%g,%g)", p0[R], p0[G], p0[B]);

        /*
         * 3.1. Find all edges that intersect the t.y + (u,v) plane
         */
        for(i = 0; i < ncolors; i++)
        {
            double k1[3];
            double yk1;
            k1[R] = pal[i][R] - p0[R];
            k1[G] = pal[i][G] - p0[G];
            k1[B] = pal[i][B] - p0[B];
            tmp = sqrt(k1[R] * k1[R] + k1[G] * k1[G] + k1[B] * k1[B]);

            /* If k1.y > t.y.y, we don't want this point */
            yk1 = y[R] * k1[R] + y[G] * k1[G] + y[B] * k1[B];
            if(yk1 > t * ylen * ylen + EPSILON)
                continue;

            for(j = 0; j < ncolors; j++)
            {
                double k2[3];
                double yk2, s;

                if(i == j)
                    continue;

                k2[R] = pal[j][R] - p0[R];
                k2[G] = pal[j][G] - p0[G];
                k2[B] = pal[j][B] - p0[B];
                tmp = sqrt(k2[R] * k2[R] + k2[G] * k2[G] + k2[B] * k2[B]);

                /* If k2.y < t.y.y, we don't want this point */
                yk2 = y[R] * k2[R] + y[G] * k2[G] + y[B] * k2[B];
                if(yk2 < t * ylen * ylen - EPSILON)
                    continue;

                if(yk2 < yk1)
                    continue;

                s = yk1 == yk2 ? 0.5 : (t * ylen * ylen - yk1) / (yk2 - yk1);

                pts[npts][R] = p0[R] + k1[R] + s * (k2[R] - k1[R]);
                pts[npts][G] = p0[G] + k1[G] + s * (k2[G] - k1[G]);
                pts[npts][B] = p0[B] + k1[B] + s * (k2[B] - k1[B]);
                npts++;
            }
        }

        /*
         * 3.2. Find the barycentre of these points' convex hull. We use
         *      the Graham Scan technique.
         */

        /* Make our problem a 2-D problem. */
        for(i = 0; i < npts; i++)
        {
            pts[i][X] = (pts[i][R] - p0[R]) * u[R]
                           + (pts[i][G] - p0[G]) * u[G]
                           + (pts[i][B] - p0[B]) * u[B];
            pts[i][Y] = (pts[i][R] - p0[R]) * v[R]
                           + (pts[i][G] - p0[G]) * v[G]
                           + (pts[i][B] - p0[B]) * v[B];
        }

        /* Find the leftmost point */
        left = -1;
        tmp = 10.;
        for(i = 0; i < npts; i++)
            if(pts[i][X] < tmp)
            {
                left = i;
                tmp = pts[i][X];
            }
        SWAP(pts[0], pts[left]);

        /* Sort the remaining points radially around pts[0]. Bubble sort
         * is okay for small sizes, I don't care. */
        for(i = 1; i < npts; i++)
            for(j = 1; j < npts - i; j++)
            {
                double k1 = (pts[j][X] - pts[0][X])
                              * (pts[j + 1][Y] - pts[0][Y]);
                double k2 = (pts[j + 1][X] - pts[0][X])
                              * (pts[j][Y] - pts[0][Y]);
                if(k1 < k2 - EPSILON)
                    SWAP(pts[j], pts[j + 1]);
                else if(k1 < k2 + EPSILON)
                {
                    /* Aligned! keep the farthest point */
                    double ax = pts[j][X] - pts[0][X];
                    double ay = pts[j][Y] - pts[0][Y];
                    double bx = pts[j + 1][X] - pts[0][X];
                    double by = pts[j + 1][Y] - pts[0][Y];

                    if(ax * ax + ay * ay > bx * bx + by * by)
                        SWAP(pts[j], pts[j + 1]);
                }
            }

        /* Remove points not in the convex hull */
        for(i = 2; i < npts; /* */)
        {
            double k1, k2;

            if(i < 2)
            {
                i++;
                continue;
            }

            k1 = (pts[i - 1][X] - pts[i - 2][X])
                          * (pts[i][Y] - pts[i - 2][Y]);
            k2 = (pts[i][X] - pts[i - 2][X])
                          * (pts[i - 1][Y] - pts[i - 2][Y]);
            if(k1 <= k2 + EPSILON)
            {
                for(j = i - 1; j < npts - 1; j++)
                    SWAP(pts[j], pts[j + 1]);
                npts--;
            }
            else
                i++;
        }
        /* FIXME: check the last point */

        for(i = 0; i < npts; i++)
            debug("    2D pt[%i] (%g,%g)", i, pts[i][X], pts[i][Y]);

        /* Compute the barycentre coordinates */
        ctx = 0.;
        cty = 0.;
        weight = 0.;
        for(i = 2; i < npts; i++)
        {
            double abx = pts[i - 1][X] - pts[0][X];
            double aby = pts[i - 1][Y] - pts[0][Y];
            double acx = pts[i][X] - pts[0][X];
            double acy = pts[i][Y] - pts[0][Y];
            double area;
            double sqarea = (abx * abx + aby * aby) * (acx * acx + acy * acy)
                          - (abx * acx + aby * acy) * (abx * acx + aby * acy);
            if(sqarea <= 0.)
                continue;

            area = sqrt(sqarea);
            ctx += area * (abx + acx) / 3;
            cty += area * (aby + acy) / 3;
            weight += area;
        }

        if(weight > EPSILON)
        {
            ctx = pts[0][X] + ctx / weight;
            cty = pts[0][Y] + cty / weight;
        }
        else
        {
            int right = -1;
            tmp = -10.;
            for(i = 0; i < npts; i++)
                if(pts[i][X] > tmp)
                {
                    right = i;
                    tmp = pts[i][X];
                }
            ctx = 0.5 * (pts[0][X] + pts[right][X]);
            cty = 0.5 * (pts[0][Y] + pts[right][Y]);
        }

        debug("    2D bary (%g,%g)", ctx, cty);

        /*
         * 3.3. Store the barycentre and convex hull information.
         */

        ret->bary[n][R] = p0[R] + ctx * u[R] + cty * v[R];
        ret->bary[n][G] = p0[G] + ctx * u[G] + cty * v[G];
        ret->bary[n][B] = p0[B] + ctx * u[B] + cty * v[B];

        for(i = 0; i < npts; i++)
        {
            ret->pts[n][i][R] = pts[i][R];
            ret->pts[n][i][G] = pts[i][G];
            ret->pts[n][i][B] = pts[i][B];
            ret->pts[n][i][X] = pts[i][X] - ctx;
            ret->pts[n][i][Y] = pts[i][Y] - cty;
            ret->pts[n][i][A] = atan2(pts[i][Y] - cty, pts[i][X] - ctx);

            debug("  3D pt[%i] (%g,%g,%g) angle %g",
                  i, pts[i][R], pts[i][G], pts[i][B], ret->pts[n][i][A]);
        }
        ret->hullsize[n] = npts;

        debug("  3D bary (%g,%g,%g)",
              ret->bary[n][R], ret->bary[n][G], ret->bary[n][B]);
    }

    return ret;
}


pipi_image_t *pipi_reduce(pipi_image_t *src,
                          int ncolors, double const *palette)
{
    static double const rgbpal[] =
    {
        0, 0, 0,  0, 0, 1,  0, 1, 0,  0, 1, 1,
        1, 0, 0,  1, 0, 1,  1, 1, 0,  1, 1, 1,
    };

    pipi_image_t *dst;
    pipi_pixels_t *srcp, *dstp;
    float *srcdata, *dstdata;
    hull_t *rgbhull, *myhull;
    int i, j, w, h;

    init_uv();

    rgbhull = compute_hull(8, rgbpal);
    myhull = compute_hull(ncolors, palette);

    /*
     * 4. Load image and change its palette.
     */

    debug("");
    debug("### PROCESSING IMAGE ###");
    debug("");

    srcp = pipi_get_pixels(src, PIPI_PIXELS_RGBA_F32);
    srcdata = (float *)srcp->pixels;

    w = srcp->w;
    h = srcp->h;

    dst = pipi_new(w, h);
    dstp = pipi_get_pixels(dst, PIPI_PIXELS_RGBA_F32);
    dstdata = (float *)dstp->pixels;

    for(j = 0; j < h; j++)
        for(i = 0; i < w; i++)
        {
            double p[3];
            double xp, yp, angle, xa, ya, xb, yb, t, s;
            int slice, n, count;

            /* FIXME: Imlib fucks up the RGB order. */
            p[B] = srcdata[4 * (j * w + i)];
            p[G] = srcdata[4 * (j * w + i) + 1];
            p[R] = srcdata[4 * (j * w + i) + 2];

            debug("Pixel +%i+%i (%g,%g,%g)", i, j, p[R], p[G], p[B]);

            slice = (int)(BRIGHT(p) * STEPS + 0.5);

            debug("  slice %i", slice);

            /* Convert to 2D. The origin is the slice's barycentre. */
            xp = (p[R] - rgbhull->bary[slice][R]) * u[R]
               + (p[G] - rgbhull->bary[slice][G]) * u[G]
               + (p[B] - rgbhull->bary[slice][B]) * u[B];
            yp = (p[R] - rgbhull->bary[slice][R]) * v[R]
               + (p[G] - rgbhull->bary[slice][G]) * v[G]
               + (p[B] - rgbhull->bary[slice][B]) * v[B];

            debug("    2D pt (%g,%g)", xp, yp);

            /* 1. find the excentricity in RGB space. There is an easier
             * way to do this, which is to find the intersection of our
             * line with the RGB cube itself, but we'd lose the possibility
             * of having an original colour space other than RGB. */

            /* First, find the relevant triangle. */
            count = rgbhull->hullsize[slice];
            angle = atan2(yp, xp);
            for(n = 0; n < count; n++)
            {
                double a1 = rgbhull->pts[slice][n][A];
                double a2 = rgbhull->pts[slice][(n + 1) % count][A];
                if(a1 > a2)
                {
                    if(angle >= a1)
                        a2 += 2 * M_PI;
                    else
                        a1 -= 2 * M_PI;
                }
                if(angle >= a1 && angle <= a2)
                    break;
            }

            /* Now compute the distance to the triangle's edge. If the edge
             * intersection is M, then t is such as P = t.M (can be zero) */
            xa = rgbhull->pts[slice][n % count][X];
            ya = rgbhull->pts[slice][n % count][Y];
            xb = rgbhull->pts[slice][(n + 1) % count][X];
            yb = rgbhull->pts[slice][(n + 1) % count][Y];
            t = (xp * (yb - ya) - yp * (xb - xa)) / (xa * yb - xb * ya);

            if(t > 1.0)
                t = 1.0;

            debug("    best RGB %g (%g,%g) (%g,%g)", t, xa, ya, xb, yb);

            /* 2. apply the excentricity in reduced space. */

            count = myhull->hullsize[slice];
            for(n = 0; n < count; n++)
            {
                double a1 = myhull->pts[slice][n][A];
                double a2 = myhull->pts[slice][(n + 1) % count][A];
                if(a1 > a2)
                {
                    if(angle >= a1)
                        a2 += 2 * M_PI;
                    else
                        a1 -= 2 * M_PI;
                }
                if(angle >= a1 && angle <= a2)
                    break;
            }

            /* If the edge intersection is M', s is such as P = s.M'. We
             * want P' = t.M' = t.P/s  */
            xa = myhull->pts[slice][n % count][X];
            ya = myhull->pts[slice][n % count][Y];
            xb = myhull->pts[slice][(n + 1) % count][X];
            yb = myhull->pts[slice][(n + 1) % count][Y];
            s = (xp * (yb - ya) - yp * (xb - xa)) / (xa * yb - xb * ya);

            debug("    best custom %g (%g,%g) (%g,%g)", s, xa, ya, xb, yb);

            if(s > 0)
            {
                xp *= t / s;
                yp *= t / s;
            }

            p[R] = myhull->bary[slice][R] + xp * u[R] + yp * v[R];
            p[G] = myhull->bary[slice][G] + xp * u[G] + yp * v[G];
            p[B] = myhull->bary[slice][B] + xp * u[B] + yp * v[B];

            /* Clipping should not be necessary, but the above code
             * is unfortunately not perfect. */
            if(p[R] < 0.0) p[R] = 0.0; else if(p[R] > 1.0) p[R] = 1.0;
            if(p[G] < 0.0) p[G] = 0.0; else if(p[G] > 1.0) p[G] = 1.0;
            if(p[B] < 0.0) p[B] = 0.0; else if(p[B] > 1.0) p[B] = 1.0;

            dstdata[4 * (j * w + i)] = p[B];
            dstdata[4 * (j * w + i) + 1] = p[G];
            dstdata[4 * (j * w + i) + 2] = p[R];
        }

    free(rgbhull);
    free(myhull);

    return dst;
}

