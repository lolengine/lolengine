//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cstdlib> /* free() */
#include <cstring> /* strdup() */

#include "core.h"

static inline float det3(float a, float b, float c,
                         float d, float e, float f,
                         float g, float h, float i)
{
    return a * (e * i - h * f)
         + b * (f * g - i * d)
         + c * (d * h - g * e);
}

static inline float cofact3(float4x4 const &mat, int i, int j)
{
    return det3(mat[(i + 1) & 3][(j + 1) & 3],
                mat[(i + 2) & 3][(j + 1) & 3],
                mat[(i + 3) & 3][(j + 1) & 3],
                mat[(i + 1) & 3][(j + 2) & 3],
                mat[(i + 2) & 3][(j + 2) & 3],
                mat[(i + 3) & 3][(j + 2) & 3],
                mat[(i + 1) & 3][(j + 3) & 3],
                mat[(i + 2) & 3][(j + 3) & 3],
                mat[(i + 3) & 3][(j + 3) & 3]) * (((i + j) & 1) ? -1.0f : 1.0f);
}

template<> float float4x4::det() const
{
    float ret = 0;
    for (int n = 0; n < 4; n++)
        ret += (*this)[n][0] * cofact3(*this, n, 0);
    return ret;
}

template<> float4x4 float4x4::invert() const
{
    float4x4 ret;
    float d = det();
    if (d)
    {
        d = 1.0f / d;
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
                ret[j][i] = cofact3(*this, i, j) * d;
    }
    return ret;
}

