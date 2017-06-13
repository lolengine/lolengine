//
//  Lol Engine
//
//  Copyright © 2004—2017 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

/*
 * Random dithering
 */

namespace lol
{

image image::dither_random() const
{
    image dst = *this;

    float *pixels = dst.lock<PixelFormat::Y_F32>();
    int count = size().x * size().y;

    for (int n = 0; n < count; ++n)
    {
        pixels[n] = (pixels[n] > lol::rand(0.5f)) ? 1.f : 0.f;
    }

    dst.unlock(pixels);

    return dst;
}

} /* namespace lol */

