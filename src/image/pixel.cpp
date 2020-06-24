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

#include <lol/msg>
#include <cassert>

#include "image-private.h"

namespace lol
{

static float u8tof32(uint8_t pixel)
{
    //return pow((float)pixel / 255.f, global_gamma);
    return pixel / 255.f;
}

static vec4 u8tof32(u8vec4 pixel)
{
    //vec4 ret;
    //ret.r = pow((float)pixel.r / 255.f, global_gamma);
    //ret.g = pow((float)pixel.g / 255.f, global_gamma);
    //ret.b = pow((float)pixel.b / 255.f, global_gamma);
    return (vec4)pixel / 255.f;
}

static vec3 u8tof32(u8vec3 pixel)
{
    //vec3 ret;
    //ret.r = pow((float)pixel.r / 255.f, global_gamma);
    //ret.g = pow((float)pixel.g / 255.f, global_gamma);
    //ret.b = pow((float)pixel.b / 255.f, global_gamma);
    return (vec3)pixel / 255.f;
}

static uint8_t f32tou8(float pixel)
{
    return (uint8_t)(pixel * 255.99f);
}

static u8vec3 f32tou8(vec3 pixel)
{
    return (u8vec3)(pixel * 255.99f);
}

static u8vec4 f32tou8(vec4 pixel)
{
    return (u8vec4)(pixel * 255.99f);
}

/*
 * Pixel-level image manipulation
 */

PixelFormat old_image::format() const
{
    return m_data->m_format;
}

/* Conversion rules matrix
 *
 * From:   To→  1  2  3  4  5  6
 * Y_8       1  .  o  o  x  x  x
 * RGB_8     2  ~  .  o  ~  x  x
 * RGBA_8    3  ~  o  .  ~  x  x
 * Y_F32     4  #  ~  ~  .  o  o
 * RGB_F32   5  ~  #  ~  #  .  o
 * RGBA_F32  6  ~  ~  #  ~  o  .
 *
 * . no conversion necessary
 * ~ intermediate conversion to RGBA_F32 or RGB_F32
 * o easy conversion (add/remove alpha and/or convert gray→color)
 * x lossless conversion (u8 to float)
 * # lossy conversion (dithering and/or convert color→gray)
 */
void old_image::set_format(PixelFormat fmt)
{
    PixelFormat old_fmt = m_data->m_format;

    /* Preliminary intermediate conversions */
    if (old_fmt == PixelFormat::RGBA_8 && fmt == PixelFormat::Y_F32)
        set_format(PixelFormat::RGBA_F32);
    else if (old_fmt == PixelFormat::RGB_8 && fmt == PixelFormat::Y_F32)
        set_format(PixelFormat::RGBA_F32);
    else if (old_fmt == PixelFormat::Y_F32 && fmt == PixelFormat::RGBA_8)
        set_format(PixelFormat::RGBA_F32);
    else if (old_fmt == PixelFormat::Y_F32 && fmt == PixelFormat::RGB_8)
        set_format(PixelFormat::RGBA_F32);
    else if (old_fmt == PixelFormat::RGB_F32 && fmt == PixelFormat::RGBA_8)
        set_format(PixelFormat::RGBA_F32);
    else if (old_fmt == PixelFormat::RGBA_F32 && fmt == PixelFormat::Y_F32)
        set_format(PixelFormat::RGB_F32);
    else if (old_fmt == PixelFormat::RGBA_F32 && fmt == PixelFormat::RGB_8)
        set_format(PixelFormat::RGB_F32);
    else if (old_fmt == PixelFormat::RGB_8 && fmt == PixelFormat::Y_8)
        set_format(PixelFormat::RGB_F32);
    else if (old_fmt == PixelFormat::RGBA_8 && fmt == PixelFormat::Y_8)
        set_format(PixelFormat::RGB_F32);
    else if (old_fmt == PixelFormat::RGB_F32 && fmt == PixelFormat::Y_8)
        set_format(PixelFormat::Y_F32);
    else if (old_fmt == PixelFormat::RGBA_F32 && fmt == PixelFormat::Y_8)
        set_format(PixelFormat::Y_F32);

    old_fmt = m_data->m_format;

    /* Set the new active pixel format */
    m_data->m_format = fmt;

    ivec2 isize = size();
    int count = isize.x * isize.y;

    /* If we never used this format, allocate a new buffer: we will
     * obviously need it. */
    if (m_data->m_pixels[(int)fmt] == nullptr)
    {
        PixelDataBase *data = nullptr;
#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wswitch"
#endif
        switch (fmt)
        {
            case PixelFormat::Y_8:
                data = new PixelData<PixelFormat::Y_8>(isize); break;
            case PixelFormat::RGB_8:
                data = new PixelData<PixelFormat::RGB_8>(isize); break;
            case PixelFormat::RGBA_8:
                data = new PixelData<PixelFormat::RGBA_8>(isize); break;
            case PixelFormat::Y_F32:
                data = new PixelData<PixelFormat::Y_F32>(isize); break;
            case PixelFormat::RGB_F32:
                data = new PixelData<PixelFormat::RGB_F32>(isize); break;
            case PixelFormat::RGBA_F32:
                data = new PixelData<PixelFormat::RGBA_F32>(isize); break;
            case PixelFormat::Unknown:
            default:
                msg::error("invalid pixel type %d", (int)fmt);
                assert(false);
                break;
        }
#if __GNUC__
#pragma GCC diagnostic pop
#endif
        m_data->m_pixels[(int)fmt] = data;
    }

    /* If the requested format is already the current format, or if the
     * current format is invalid, there is nothing to convert. */
    if (fmt == old_fmt || old_fmt == PixelFormat::Unknown)
        return;

    /* Easy conversions: just add or remove channels */
    if (old_fmt == PixelFormat::Y_8 && fmt == PixelFormat::RGB_8)
    {
        uint8_t *src = (uint8_t *)m_data->m_pixels[(int)old_fmt]->data();
        u8vec3 *dest = (u8vec3 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8vec3(src[n]);
    }
    else if (old_fmt == PixelFormat::Y_8 && fmt == PixelFormat::RGBA_8)
    {
        uint8_t *src = (uint8_t *)m_data->m_pixels[(int)old_fmt]->data();
        u8vec4 *dest = (u8vec4 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8vec4(u8vec3(src[n]), 255);
    }
    else if (old_fmt == PixelFormat::RGBA_8 && fmt == PixelFormat::RGB_8)
    {
        u8vec4 *src = (u8vec4 *)m_data->m_pixels[(int)old_fmt]->data();
        u8vec3 *dest = (u8vec3 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = src[n].rgb;
    }
    else if (old_fmt == PixelFormat::RGB_8 && fmt == PixelFormat::RGBA_8)
    {
        u8vec3 *src = (u8vec3 *)m_data->m_pixels[(int)old_fmt]->data();
        u8vec4 *dest = (u8vec4 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8vec4(src[n], 255);
    }
    else if (old_fmt == PixelFormat::RGBA_F32 && fmt == PixelFormat::RGB_F32)
    {
        vec4 *src = (vec4 *)m_data->m_pixels[(int)old_fmt]->data();
        vec3 *dest = (vec3 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = src[n].rgb;
    }
    else if (old_fmt == PixelFormat::RGB_F32 && fmt == PixelFormat::RGBA_F32)
    {
        vec3 *src = (vec3 *)m_data->m_pixels[(int)old_fmt]->data();
        vec4 *dest = (vec4 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = vec4(src[n], 1.f);
    }
    else if (old_fmt == PixelFormat::Y_F32 && fmt == PixelFormat::RGB_F32)
    {
        float *src = (float *)m_data->m_pixels[(int)old_fmt]->data();
        vec3 *dest = (vec3 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = vec3(src[n]);
    }
    else if (old_fmt == PixelFormat::Y_F32 && fmt == PixelFormat::RGBA_F32)
    {
        float *src = (float *)m_data->m_pixels[(int)old_fmt]->data();
        vec4 *dest = (vec4 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = vec4(vec3(src[n]), 1.0f);
    }
    /* Lossless conversions: u8 to float */
    else if (old_fmt == PixelFormat::Y_8 && fmt == PixelFormat::Y_F32)
    {
        uint8_t *src = (uint8_t *)m_data->m_pixels[(int)old_fmt]->data();
        float *dest = (float *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8tof32(src[n]);
    }
    else if (old_fmt == PixelFormat::Y_8 && fmt == PixelFormat::RGB_F32)
    {
        uint8_t *src = (uint8_t *)m_data->m_pixels[(int)old_fmt]->data();
        vec3 *dest = (vec3 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8tof32(u8vec3(src[n]));
    }
    else if (old_fmt == PixelFormat::RGB_8 && fmt == PixelFormat::RGB_F32)
    {
        u8vec3 *src = (u8vec3 *)m_data->m_pixels[(int)old_fmt]->data();
        vec3 *dest = (vec3 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8tof32(src[n]);
    }
    else if (old_fmt == PixelFormat::RGBA_8 && fmt == PixelFormat::RGB_F32)
    {
        u8vec4 *src = (u8vec4 *)m_data->m_pixels[(int)old_fmt]->data();
        vec3 *dest = (vec3 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8tof32(src[n].rgb);
    }
    else if (old_fmt == PixelFormat::Y_8 && fmt == PixelFormat::RGBA_F32)
    {
        uint8_t *src = (uint8_t *)m_data->m_pixels[(int)old_fmt]->data();
        vec4 *dest = (vec4 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8tof32(u8vec4(u8vec3(src[n]), 255));
    }
    else if (old_fmt == PixelFormat::RGB_8 && fmt == PixelFormat::RGBA_F32)
    {
        u8vec3 *src = (u8vec3 *)m_data->m_pixels[(int)old_fmt]->data();
        vec4 *dest = (vec4 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8tof32(u8vec4(src[n], 255));
    }
    else if (old_fmt == PixelFormat::RGBA_8 && fmt == PixelFormat::RGBA_F32)
    {
        u8vec4 *src = (u8vec4 *)m_data->m_pixels[(int)old_fmt]->data();
        vec4 *dest = (vec4 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = u8tof32(src[n]);
    }
    /* Other conversions */
    else if (old_fmt == PixelFormat::RGB_F32 && fmt == PixelFormat::Y_F32)
    {
        vec3 *src = (vec3 *)m_data->m_pixels[(int)old_fmt]->data();
        float *dest = (float *)m_data->m_pixels[(int)fmt]->data();

        vec3 const coeff(0.299f, 0.587f, 0.114f);

        for (int n = 0; n < count; ++n)
            dest[n] = dot(coeff, src[n]);
    }
    else if (old_fmt == PixelFormat::Y_F32 && fmt == PixelFormat::Y_8)
    {
        float *src = (float *)m_data->m_pixels[(int)old_fmt]->data();
        uint8_t *dest = (uint8_t *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = f32tou8(src[n]);
    }
    else if (old_fmt == PixelFormat::RGB_F32 && fmt == PixelFormat::RGB_8)
    {
        vec3 *src = (vec3 *)m_data->m_pixels[(int)old_fmt]->data();
        u8vec3 *dest = (u8vec3 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = f32tou8(src[n]);
    }
    else if (old_fmt == PixelFormat::RGBA_F32 && fmt == PixelFormat::RGBA_8)
    {
        vec4 *src = (vec4 *)m_data->m_pixels[(int)old_fmt]->data();
        u8vec4 *dest = (u8vec4 *)m_data->m_pixels[(int)fmt]->data();

        for (int n = 0; n < count; ++n)
            dest[n] = f32tou8(src[n]);
#if 0
        init_tables();

        for (int y = 0; y < isize.y; y++)
            for (int x = 0; x < isize.x; x++)
                for (i = 0; i < 4; i++)
                {
                    double p, e;
                    uint8_t d;

                    p = src[4 * (y * isize.x + x) + i];

                    if (p < 0.) d = 0.;
                    else if (p > 1.) d = 255;
                    else d = (int)(255.999 * pow(p, 1. / global_gamma));

                    dest[4 * (y * isize.x + x) + i] = d;

                    e = (p - u8tof32(d)) / 16;
                    if (x < isize.x - 1)
                        src[4 * (y * isize.x + x + 1) + i] += e * 7;
                    if (y < isize.y - 1)
                    {
                        if (x > 0)
                            src[4 * ((y + 1) * isize.x + x - 1) + i] += e * 3;
                        src[4 * ((y + 1) * isize.x + x) + i] += e * 5;
                        if (x < isize.x - 1)
                            src[4 * ((y + 1) * isize.x + x + 1) + i] += e;
                    }
                }
#endif
    }
    else
    {
        msg::error("Unable to find old_image conversion from %d to %d",
                   (int)old_fmt, (int)fmt);
    }
}

} /* namespace lol */

