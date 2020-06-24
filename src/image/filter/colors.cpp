//
//  Lol Engine
//
//  Copyright © 2004—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

/*
 * Colour manipulation functions
 */

namespace lol
{

old_image old_image::Brightness(float val) const
{
    old_image ret = *this;
    int count = size().x * size().y;

    if (format() == PixelFormat::Y_8 || format() == PixelFormat::Y_F32)
    {
        float *pixels = ret.lock<PixelFormat::Y_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = lol::clamp(pixels[n] + val, 0.f, 1.f);
        ret.unlock(pixels);
    }
    else
    {
        vec4 *pixels = ret.lock<PixelFormat::RGBA_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = vec4(lol::clamp(pixels[n].rgb + vec3(val), 0.f, 1.f),
                             pixels[n].a);
        ret.unlock(pixels);
    }

    return ret;
}

old_image old_image::Contrast(float val) const
{
    old_image ret = *this;
    int count = size().x * size().y;

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

    if (format() == PixelFormat::Y_8 || format() == PixelFormat::Y_F32)
    {
        float add = -0.5f * val + 0.5f;
        float *pixels = ret.lock<PixelFormat::Y_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = lol::clamp(pixels[n] * val + add, 0.f, 1.f);
        ret.unlock(pixels);
    }
    else
    {
        vec3 add = vec3(-0.5f * val + 0.5f);
        vec4 *pixels = ret.lock<PixelFormat::RGBA_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = vec4(lol::clamp(pixels[n].rgb * val + add, 0.f, 1.f),
                             pixels[n].a);
        ret.unlock(pixels);
    }

    return ret;
}

/*
 * TODO: the current approach is naive; we should use the histogram in order
 * to decide how to change the contrast.
 */
old_image old_image::AutoContrast() const
{
    old_image ret = *this;

    float min_val = 1.f, max_val = 0.f;
    int count = size().x * size().y;

    if (format() == PixelFormat::Y_8 || format() == PixelFormat::Y_F32)
    {
        float *pixels = ret.lock<PixelFormat::Y_F32>();
        for (int n = 0; n < count; ++n)
        {
            min_val = lol::min(min_val, pixels[n]);
            max_val = lol::max(max_val, pixels[n]);
        }

        float t = max_val > min_val ? 1.f / (max_val - min_val) : 1.f;
        for (int n = 0; n < count; ++n)
            pixels[n] = (pixels[n] - min_val) * t;

        ret.unlock(pixels);
    }
    else
    {
        vec4 *pixels = ret.lock<PixelFormat::RGBA_F32>();
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

        ret.unlock(pixels);
    }

    return ret;
}

old_image old_image::Invert() const
{
    old_image ret = *this;
    int count = size().x * size().y;

    if (format() == PixelFormat::Y_8 || format() == PixelFormat::Y_F32)
    {
        float *pixels = ret.lock<PixelFormat::Y_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = 1.f - pixels[n];
        ret.unlock(pixels);
    }
    else
    {
        vec4 *pixels = ret.lock<PixelFormat::RGBA_F32>();
        for (int n = 0; n < count; ++n)
            pixels[n] = vec4(vec3(1.f) -pixels[n].rgb, pixels[n].a);
        ret.unlock(pixels);
    }

    return ret;
}

old_image old_image::Threshold(float val) const
{
    old_image ret = *this;
    int count = size().x * size().y;

    float *pixels = ret.lock<PixelFormat::Y_F32>();
    for (int n = 0; n < count; ++n)
        pixels[n] = pixels[n] > val ? 1.f : 0.f;
    ret.unlock(pixels);

    return ret;
}

old_image old_image::Threshold(vec3 val) const
{
    old_image ret = *this;
    int count = size().x * size().y;

    vec4 *pixels = ret.lock<PixelFormat::RGBA_F32>();
    for (int n = 0; n < count; ++n)
        pixels[n] = vec4(pixels[n].r > val.r ? 1.f : 0.f,
                         pixels[n].g > val.g ? 1.f : 0.f,
                         pixels[n].b > val.b ? 1.f : 0.f,
                         pixels[n].a);
    ret.unlock(pixels);

    return ret;
}

} /* namespace lol */

