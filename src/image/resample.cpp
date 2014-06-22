//
// Lol Engine
//
// Copyright: (c) 2004-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

/*
 * Image resizing functions
 */

namespace lol
{

static Image ResizeBicubic(Image &image, ivec2 size);
static Image ResizeBresenham(Image &image, ivec2 size);

Image Image::Resize(ivec2 size, ResampleAlgorithm algorithm)
{
    switch (algorithm)
    {
        case ResampleAlgorithm::Bicubic:
            return ResizeBicubic(*this, size);
        case ResampleAlgorithm::Bresenham:
        default:
            return ResizeBresenham(*this, size);
    }
}

static Image ResizeBicubic(Image &image, ivec2 size)
{
    Image dst(size);
    ivec2 const oldsize = image.GetSize();

    vec4 const *srcp = image.Lock<PixelFormat::RGBA_F32>();
    vec4 *dstp = dst.Lock<PixelFormat::RGBA_F32>();

    float scalex = size.x > 1 ? (oldsize.x - 1.f) / (size.x - 1) : 1.f;
    float scaley = size.y > 1 ? (oldsize.y - 1.f) / (size.y - 1) : 1.f;

    for (int y = 0; y < size.y; ++y)
    {
        float yfloat = scaley * y;
        int yint = (int)yfloat;
        float y1 = yfloat - yint;

        vec4 const *p0 = srcp + oldsize.x * lol::min(lol::max(0, yint - 1), oldsize.y - 1);
        vec4 const *p1 = srcp + oldsize.x * lol::min(lol::max(0, yint    ), oldsize.y - 1);
        vec4 const *p2 = srcp + oldsize.x * lol::min(lol::max(0, yint + 1), oldsize.y - 1);
        vec4 const *p3 = srcp + oldsize.x * lol::min(lol::max(0, yint + 2), oldsize.y - 1);

        for (int x = 0; x < size.x; ++x)
        {
            float xfloat = scalex * x;
            int xint = (int)xfloat;
            float x1 = xfloat - xint;

            int const i0 = lol::min(lol::max(0, xint - 1), oldsize.x - 1);
            int const i1 = lol::min(lol::max(0, xint    ), oldsize.x - 1);
            int const i2 = lol::min(lol::max(0, xint + 1), oldsize.x - 1);
            int const i3 = lol::min(lol::max(0, xint + 2), oldsize.x - 1);

            vec4 a00 = p1[i1];
            vec4 a01 = .5f * (p2[i1] - p0[i1]);
            vec4 a02 = p0[i1] - 2.5f * p1[i1]
                        + 2.f * p2[i1] - .5f * p3[i1];
            vec4 a03 = .5f * (p3[i1] - p0[i1]) + 1.5f * (p1[i1] - p2[i1]);

            vec4 a10 = .5f * (p1[i2] - p1[i0]);
            vec4 a11 = .25f * (p0[i0] - p2[i0] - p0[i2] + p2[i2]);
            vec4 a12 = .5f * (p0[i2] - p0[i0]) + 1.25f * (p1[i0] - p1[i2])
                        + .25f * (p3[i0] - p3[i2]) + p2[i2] - p2[i0];
            vec4 a13 = .25f * (p0[i0] - p3[i0] - p0[i2] + p3[i2])
                        + .75f * (p2[i0] - p1[i0] + p1[i2] - p2[i2]);

            vec4 a20 = p1[i0] - 2.5f * p1[i1]
                        + 2.f * p1[i2] - .5f * p1[i3];
            vec4 a21 = .5f * (p2[i0] - p0[i0]) + 1.25f * (p0[i1] - p2[i1])
                        + .25f * (p0[i3] - p2[i3]) - p0[i2] + p2[i2];
            vec4 a22 = p0[i0] - p3[i2] - 2.5f * (p1[i0] + p0[i1])
                        + 2.f * (p2[i0] + p0[i2]) - .5f * (p3[i0] + p0[i3])
                        + 6.25f * p1[i1] - 5.f * (p2[i1] + p1[i2])
                        + 1.25f * (p3[i1] + p1[i3])
                        + 4.f * p2[i2] - p2[i3] + .25f * p3[i3];
            vec4 a23 = 1.5f * (p1[i0] - p2[i0]) + .5f * (p3[i0] - p0[i0])
                        + 1.25f * (p0[i1] - p3[i1])
                        + 3.75f * (p2[i1] - p1[i1]) + p3[i2] - p0[i2]
                        + 3.f * (p1[i2] - p2[i2]) + .25f * (p0[i3] - p3[i3])
                        + .75f * (p2[i3] - p1[i3]);

            vec4 a30 = .5f * (p1[i3] - p1[i0]) + 1.5f * (p1[i1] - p1[i2]);
            vec4 a31 = .25f * (p0[i0] - p2[i0]) + .25f * (p2[i3] - p0[i3])
                        + .75f * (p2[i1] - p0[i1] + p0[i2] - p2[i2]);
            vec4 a32 = -.5f * p0[i0] + 1.25f * p1[i0] - p2[i0]
                        + .25f * p3[i0] + 1.5f * p0[i1] - 3.75f * p1[i1]
                        + 3.f * p2[i1] - .75f * p3[i1] - 1.5f * p0[i2]
                        + 3.75f * p1[i2] - 3.f * p2[i2] + .75f * p3[i2]
                        + .5f * p0[i3] - 1.25f * p1[i3] + p2[i3]
                        - .25f * p3[i3];
            vec4 a33 = .25f * p0[i0] - .75f * p1[i0] + .75f * p2[i0]
                        - .25f * p3[i0] - .75f * p0[i1] + 2.25f * p1[i1]
                        - 2.25f * p2[i1] + .75f * p3[i1] + .75f * p0[i2]
                        - 2.25f * p1[i2] + 2.25f * p2[i2] - .75f * p3[i2]
                        - .25f * p0[i3] + .75f * p1[i3] - .75f * p2[i3]
                        + .25f * p3[i3];

            float y2 = y1 * y1; float y3 = y2 * y1;
            float x2 = x1 * x1; float x3 = x2 * x1;

            vec4 p = a00 + a01 * y1 + a02 * y2 + a03 * y3
                   + a10 * x1 + a11 * x1 * y1 + a12 * x1 * y2 + a13 * x1 * y3
                   + a20 * x2 + a21 * x2 * y1 + a22 * x2 * y2 + a23 * x2 * y3
                   + a30 * x3 + a31 * x3 * y1 + a32 * x3 * y2 + a33 * x3 * y3;

            dstp[y * size.x + x] = lol::clamp(p, 0.f, 1.f);
        }
    }

    dst.Unlock(dstp);
    image.Unlock(srcp);

    return dst;
}

/* This is Bresenham resizing. I “rediscovered” it independently but
 * it was actually first described in 1995 by Tim Kientzle in “Scaling
 * Bitmaps with Bresenham”. */

/* FIXME: the algorithm does not handle alpha components properly. Resulting
 * alpha should be the mean alpha value of the neightbouring pixels, but
 * the colour components should be weighted with the alpha value. */
static Image ResizeBresenham(Image &image, ivec2 size)
{
    Image dst(size);
    ivec2 const oldsize = image.GetSize();
    float const invswsh = 1.0f / (oldsize.x * oldsize.y);

    vec4 const *srcp = image.Lock<PixelFormat::RGBA_F32>();
    vec4 *dstp = dst.Lock<PixelFormat::RGBA_F32>();

    Array<vec4> aline, line;
    aline.Resize(size.x);
    line.Resize(size.x);
    memset(line.Data(), 0, line.Bytes());

    int remy = 0;

    for (int y = 0, y0 = 0; y < size.y; y++)
    {
        memset(aline.Data(), 0, aline.Bytes());

        for (int toty = 0; toty < oldsize.y; )
        {
            if (remy == 0)
            {
                vec4 color(0.f);
                int remx = 0;

                for (int x = 0, x0 = 0; x < size.x; x++)
                {
                    vec4 acolor(0.f);

                    for (int totx = 0; totx < oldsize.x; )
                    {
                        if (remx == 0)
                        {
                            color = srcp[y0 * oldsize.x + x0];
                            x0++;
                            remx = size.x;
                        }

                        int nx = lol::min(remx, oldsize.x - totx);
                        acolor += (float)nx * color;
                        totx += nx;
                        remx -= nx;
                    }

                    line[x] = acolor;
                }

                y0++;
                remy = size.y;
            }

            int ny = lol::min(remy, oldsize.y - toty);
            for (int x = 0; x < size.x; x++)
                aline[x] += (float)ny * line[x];
            toty += ny;
            remy -= ny;
        }

        for (int x = 0; x < size.x; x++)
            dstp[y * size.x + x] = aline[x] * invswsh;
    }

    dst.Unlock(dstp);
    image.Unlock(srcp);

    return dst;
}

} /* namespace lol */

