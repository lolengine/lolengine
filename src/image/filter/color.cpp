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
 * Colour manipulation functions
 */

namespace lol
{

Image Image::Brightness(float val) const
{
    Image ret = *this;
    int count = GetSize().x * GetSize().y;

    if (GetFormat() == PixelFormat::Y_8 || GetFormat() == PixelFormat::Y_F32)
    {
        float *pixels = ret.Lock<PixelFormat::Y_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = lol::clamp(pixels[n] + val, 0.f, 1.f);
        ret.Unlock(pixels);
    }
    else
    {
        vec4 *pixels = ret.Lock<PixelFormat::RGBA_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = vec4(lol::clamp(pixels[n].rgb + vec3(val), 0.f, 1.f),
                             pixels[n].a);
        ret.Unlock(pixels);
    }

    return ret;
}

Image Image::Contrast(float val) const
{
    Image ret = *this;
    int count = GetSize().x * GetSize().y;

    if (val >= 0.f)
    {
        if (val > 0.99999f)
            val = 0.99999f;

        val = 1.f / (1.f - val);
    }
    else
    {
        val = lol::clamp(1.f + val, 0.f, 1.f);
    }

    if (GetFormat() == PixelFormat::Y_8 || GetFormat() == PixelFormat::Y_F32)
    {
        float add = -0.5f * val + 0.5f;
        float *pixels = ret.Lock<PixelFormat::Y_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = lol::clamp(pixels[n] * val + add, 0.f, 1.f);
        ret.Unlock(pixels);
    }
    else
    {
        vec3 add = vec3(-0.5f * val + 0.5f);
        vec4 *pixels = ret.Lock<PixelFormat::RGBA_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = vec4(lol::clamp(pixels[n].rgb * val + add, 0.f, 1.f),
                             pixels[n].a);
        ret.Unlock(pixels);
    }

    return ret;
}

/*
 * TODO: the current approach is naive; we should use the histogram in order
 * to decide how to change the contrast.
 */
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

Image Image::Invert() const
{
    Image ret = *this;
    int count = GetSize().x * GetSize().y;

    if (GetFormat() == PixelFormat::Y_8 || GetFormat() == PixelFormat::Y_F32)
    {
        float *pixels = ret.Lock<PixelFormat::Y_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = 1.f - pixels[n];
        ret.Unlock(pixels);
    }
    else
    {
        vec4 *pixels = ret.Lock<PixelFormat::RGBA_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = vec4(vec3(1.f) -pixels[n].rgb, pixels[n].a);
        ret.Unlock(pixels);
    }

    return ret;
}

Image Image::Threshold(float val) const
{
    Image ret = *this;
    int count = GetSize().x * GetSize().y;

    float *pixels = ret.Lock<PixelFormat::Y_F32>();
    for (int n = 0; n < count; ++n)
        pixels[n] = pixels[n] > val ? 1.f : 0.f;
    ret.Unlock(pixels);

    return ret;
}

Image Image::Threshold(vec3 val) const
{
    Image ret = *this;
    int count = GetSize().x * GetSize().y;

    vec4 *pixels = ret.Lock<PixelFormat::RGBA_F32>();
    for (int n = 0; n < count; ++n)
        pixels[n] = vec4(pixels[n].r > val.r ? 1.f : 0.f,
                         pixels[n].g > val.g ? 1.f : 0.f,
                         pixels[n].b > val.b ? 1.f : 0.f,
                         pixels[n].a);
    ret.Unlock(pixels);

    return ret;
}

} /* namespace lol */

