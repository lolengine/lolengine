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
 * YUV conversion functions
 */

namespace lol
{

Image Image::YUVToRGB() const
{
    Image ret = *this;
    int count = GetSize().x * GetSize().y;

    vec4 *pixels = ret.Lock<PixelFormat::RGBA_F32>();
    for (int n = 0; n < count; ++n)
        pixels[n] = Color::YUVToRGB(pixels[n]);
    ret.Unlock(pixels);

    return ret;
}

Image Image::RGBToYUV() const
{
    Image ret = *this;
    int count = GetSize().x * GetSize().y;

    vec4 *pixels = ret.Lock<PixelFormat::RGBA_F32>();
    for (int n = 0; n < count; ++n)
        pixels[n] = Color::RGBToYUV(pixels[n]);
    ret.Unlock(pixels);

    return ret;
}

} /* namespace lol */

