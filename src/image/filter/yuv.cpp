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
 * YUV conversion functions
 */

namespace lol
{

image image::YUVToRGB() const
{
    image ret = *this;
    int count = size().x * size().y;

    vec4 *pixels = ret.lock<PixelFormat::RGBA_F32>();
    for (int n = 0; n < count; ++n)
        pixels[n] = Color::YUVToRGB(pixels[n]);
    ret.unlock(pixels);

    return ret;
}

image image::RGBToYUV() const
{
    image ret = *this;
    int count = size().x * size().y;

    vec4 *pixels = ret.lock<PixelFormat::RGBA_F32>();
    for (int n = 0; n < count; ++n)
        pixels[n] = Color::RGBToYUV(pixels[n]);
    ret.unlock(pixels);

    return ret;
}

} /* namespace lol */

