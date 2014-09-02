//
// Lol Engine
//
// Copyright: (c) 2004-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

/*
 * Random dithering
 */

namespace lol
{

Image Image::DitherRandom() const
{
    Image dst = *this;

    float *pixels = dst.Lock<PixelFormat::Y_F32>();
    int count = GetSize().x * GetSize().y;

    for (int n = 0; n < count; ++n)
    {
        pixels[n] = (pixels[n] > lol::rand(0.5f)) ? 1.f : 0.f;
    }

    dst.Unlock(pixels);

    return dst;
}

} /* namespace lol */

