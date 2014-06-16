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
 * Noise rendering functions
 */

namespace lol
{

bool Image::RenderRandom(ivec2 size)
{
    unsigned int ctx = 1;

    SetSize(size);
    vec4 *pixels = Lock<PixelFormat::RGBA_F32>();
    int count = size.x * size.y;

    for (int n = 0; n < count; ++n)
    {
        for (int t : { 0, 1, 2 })
        {
            long hi, lo;

            hi = ctx / 12773L;
            lo = ctx % 12773L;
            ctx = 16807L * lo - 2836L * hi;
            if(ctx <= 0)
                ctx += 0x7fffffffL;

            pixels[n][t] = (float)((ctx % 65536) / 65535.);
        }
        pixels[n][3] = 1.0f;
    }

    Unlock(pixels);

    return true;
}

} /* namespace lol */

