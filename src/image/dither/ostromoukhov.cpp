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

#include <lol/main.h>

/*
 * Ostromoukhov dithering functions
 *
 * This module implements Ostromoukhov's simple error diffusion algorithm,
 * as introduced in the paper "A Simple and Efficient Error-Diffusion
 * Algorithm", Proceedings of SIGGRAPH 2001, in ACM Computer Graphics,
 * Annual Conference Series, pp. 567--572, 2001.
 *
 * Note: the authors initially provided a large data table, but we
 * manually converted it to a piecewise linear function.
 */

namespace lol
{

static inline vec3 GetDiffusion(float v)
{
    static vec3 const table[] =
    {
        vec3(0.0000f, 0.2777f, 0.0000f),
        vec3(0.0039f, 0.2777f, 0.0000f),
        vec3(0.0117f, 0.3636f, 0.0000f),
        vec3(0.0157f, 0.3846f, 0.0000f),
        vec3(0.0392f, 0.2308f, 0.2308f),
        vec3(0.0863f, 0.1667f, 0.3333f),
        vec3(0.1255f, 0.3878f, 0.2041f),
        vec3(0.2510f, 0.0000f, 0.4762f),
        vec3(0.2824f, 0.0769f, 0.5385f),
        vec3(0.3020f, 0.1667f, 0.1667f),
        vec3(0.3333f, 0.1667f, 0.1667f),
        vec3(0.3725f, 0.2000f, 0.3000f),
        vec3(0.4196f, 0.2000f, 0.3000f),
        vec3(0.4980f, 0.1667f, 0.1667f),
        vec3(1.0000f, 0.1667f, 0.1667f), /* Safe value */
    };

    vec3 ret(0.1667f);

    if (v > 0.5f)
       v = 1.f - v;
    if (v < 0.f)
       v = 0.f;

    for (unsigned int i = 1; i < sizeof(table) / sizeof(table[0]); ++i)
    {
        if (v <= table[i][0])
        {
            ret[1] = lol::mix(table[i - 1][1], table[i][1],
                         (table[i][0] - v) / (table[i][0] - table[i - 1][0]));
            ret[2] = lol::mix(table[i - 1][2], table[i][2],
                         (table[i][0] - v) / (table[i][0] - table[i - 1][0]));
            break;
        }
    }

    ret[0] = 1.f - ret[1] - ret[2];
    return ret;
}

Image Image::DitherOstromoukhov(ScanMode scan) const
{
    Image dst = *this;

    float *pixels = dst.Lock<PixelFormat::Y_F32>();
    int w = dst.GetSize().x;
    int h = dst.GetSize().y;

    for (int y = 0; y < h; y++)
    {
        bool reverse = (y & 1) && (scan == ScanMode::Serpentine);

        for (int x = 0; x < w; x++)
        {
            int x2 = reverse ? w - 1 - x : x;
            int s = reverse ? -1 : 1;

            float p = pixels[y * w + x2];
            float q = p < 0.5f ? 0.f : 1.f;
            pixels[y * w + x2] = q;

            vec3 e = (p - q) * GetDiffusion(p);

            if(x < w - 1)
                pixels[y * w + x2 + s] += e[0];
            if(y < h - 1)
            {
                if(x > 0)
                    pixels[(y + 1) * w + x2 - s] += e[1];
                pixels[(y + 1) * w + x2] += e[2];
            }
        }
    }

    dst.Unlock(pixels);

    return dst;
}

} /* namespace lol */

