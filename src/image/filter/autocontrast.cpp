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
 * Autocontrast functions
 * TODO: the current approach is naive; we should use the histogram in order
 * to decide how to change the contrast.
 */

namespace lol
{

Image Image::AutoContrast() const
{
    Image ret = *this;

    float min_val = 1.f, max_val = 0.f;
    int count = GetSize().x * GetSize().y;

    if (GetFormat() == PixelFormat::Y_8 || GetFormat() == PixelFormat::Y_F32)
    {
        float *pixels = ret.Lock<PixelFormat::Y_F32>();
        for (int n = 0; n < count; ++n)
        {
            min_val = lol::min(min_val, pixels[n]);
            max_val = lol::max(max_val, pixels[n]);
        }

        float t = max_val > min_val ? 1.f / (max_val - min_val) : 1.f;
        for (int n = 0; n < count; ++n)
            pixels[n] = (pixels[n] - min_val) * t;

        ret.Unlock(pixels);
    }
    else
    {
        vec4 *pixels = ret.Lock<PixelFormat::RGBA_F32>();
        for (int n = 0; n < count; ++n)
        {
            min_val = lol::min(min_val, pixels[n].r);
            min_val = lol::min(min_val, pixels[n].g);
            min_val = lol::min(min_val, pixels[n].b);
            max_val = lol::max(max_val, pixels[n].r);
            max_val = lol::max(max_val, pixels[n].g);
            max_val = lol::max(max_val, pixels[n].b);
        }

        float t = max_val > min_val ? 1.f / (max_val - min_val) : 1.f;
        for (int n = 0; n < count; ++n)
            pixels[n] = vec4((pixels[n].r - min_val) * t,
                             (pixels[n].g - min_val) * t,
                             (pixels[n].b - min_val) * t,
                             pixels[n].a);;

        ret.Unlock(pixels);
    }

    return ret;
}

} /* namespace lol */

