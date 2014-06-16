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
 * Halftoning screen functions
 */

namespace lol
{

bool Image::RenderBayer(ivec2 size)
{
    if (size.x <= 0 || size.y <= 0)
        return false;

    int n = 1;
    while (n < size.x || n < size.y)
        n *= 2;

    SetSize(size);
    float *pixels = Lock<PixelFormat::Y_F32>();

    for (int j = 0; j < size.y; j++)
        for (int i = 0; i < size.x; i++)
        {
            int x = 0;

            for (int k = 1, l = n * n / 4; k < n; k *= 2, l /= 4)
            {
                if ((i & k) && (j & k))
                    x += l;
                else if (i & k)
                    x += 3 * l;
                else if (j & k)
                    x += 2 * l;
            }

            pixels[j * size.x + i] = (float)(x + 1) / (n * n + 1);
        }

    Unlock(pixels);

    return true;
}

typedef struct
{
    int x, y;
    double dist;
}
dot_t;

static int cmpdot(const void *p1, const void *p2)
{
    return ((dot_t const *)p1)->dist > ((dot_t const *)p2)->dist;
}

bool Image::RenderHalftone(ivec2 size)
{
    if (size.x <= 0 || size.y <= 0)
        return false;

    Array<dot_t> circle;
    circle.Resize(size.x * size.y);
    for (int y = 0; y < size.y; y++)
        for (int x = 0; x < size.x; x++)
        {
            double dy = ((double)y + .07) / size.y - .5;
            double dx = (double)x / size.x - .5;
            /* Using dx²+dy² here creates another interesting halftone. */
            double r = - cos(M_PI * (dx - dy)) - cos(M_PI * (dx + dy));
            circle[y * size.x + x].x = x;
            circle[y * size.x + x].y = y;
            circle[y * size.x + x].dist = r;
        }
    std::qsort(circle.Data(), size.x * size.y, sizeof(dot_t), cmpdot);

    SetSize(2 * size);
    float *pixels = Lock<PixelFormat::Y_F32>();
    float mul = 1.f / (size.x * size.y * 4 + 1);
    for (int n = 0; n < size.x * size.y; n++)
    {
        int x = circle[n].x;
        int y = circle[n].y;

        pixels[y * (2 * size.x) + x] = (float)(2 * n + 1) * mul;
        pixels[(y + size.y) * (2 * size.x) + x + size.x] = (float)(2 * n + 2) * mul;
        pixels[(y + size.y) * (2 * size.x) + x] = 1. - (float)(2 * n + 1) * mul;
        pixels[y * (2 * size.x) + x + size.x] = 1. - (float)(2 * n + 2) * mul;
    }
    Unlock(pixels);

    return true;
}

} /* namespace lol */

