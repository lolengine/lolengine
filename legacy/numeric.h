//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Various math utility functions
// ——————————————————————————————
//

#include <lol/math> // lol::lerp, lol::clamp
#include <cstdlib>
#include <stdint.h>

namespace lol
{

/* Next power of two. */
template <typename T> static inline T PotUp(T val)
{
    val = val - 1;
    if (sizeof(val) > 4) val = val | ((uint64_t)val >> 32);
    if (sizeof(val) > 2) val = val | ((uint64_t)val >> 16);
    if (sizeof(val) > 1) val = val | ((uint64_t)val >> 8);
    val = val | ((uint64_t)val >> 4);
    val = val | ((uint64_t)val >> 2);
    val = val | ((uint64_t)val >> 1);
    return val + 1;
}

//Damp for float
template <typename T1, typename T2, typename Tf> static inline T1 damp(const T1 &a, const T2 &b, const Tf &x, const Tf &dt)
{
    if (dt <= .0f)
        return a;
    return lol::lerp(a, b, dt / (dt + x));
}

//SmoothClamp clamps x in [a - sd, b + sd] and returns a value in [a, b] that is smoothed at the borders.
static inline float SmoothClamp(float &x, float a, float b, float sd)
{
    if (b < a)
        Swap(a, b);

    float tx = x;
    float ta = a - sd;
    float tb = b + sd;
    if (sd > 0.f)
    {
        if ((b - a) < 2.f * sd)
            sd = .5f * (b - a);

        if (tx < a + sd && tx > a - sd)
        {
            float t = (tx - a) / sd;
            t = 0.25f * (t + 1.0f) * (t + 1.0f);
            tx = a + t * sd;
        }
        else if (tx < b + sd && tx > b - sd)
        {
            float t = -(tx - b) / sd;
            t = 0.25f * (t + 1.0f) * (t + 1.0f);
            tx = b - t * sd;
        }
    }

    x = lol::clamp(x, ta, tb);
    return lol::clamp(tx, a, b);
}

} /* namespace lol */

