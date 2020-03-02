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
// The color class
// ———————————————
// Provides various color conversion routines.
//

#include <lol/math/vector.h>

namespace lol
{

template<typename T>
class color_t : public vec_t<T,4>
{
    using vec_t<T,4>::vec_t; // inherit ctors

    using vec3_t = vec_t<T,3>;
    using vec4_t = vec_t<T,4>;
    using mat3_t = mat_t<T,3,3>;

public:
    //
    // Some predefined colours
    //
    static const color_t<T> black, red, green, yellow, blue, magenta,
                            cyan, white, dark_gray, gray, light_gray;

    /*
     * Convert linear RGB to sRGB
     */
    static vec3_t rgb_to_srgb(vec3_t src)
    {
        vec3_t ret = 12.92f * src;
        if (src.r > 0.0031308f)
            ret.r = 1.055f * pow(src.r, 1.0f / 2.4f) - 0.055f;
        if (src.g > 0.0031308f)
            ret.g = 1.055f * pow(src.g, 1.0f / 2.4f) - 0.055f;
        if (src.b > 0.0031308f)
            ret.b = 1.055f * pow(src.b, 1.0f / 2.4f) - 0.055f;
        return ret;
    }

    static vec4_t rgb_to_srgb(vec4_t src)
    {
        return vec4_t(rgb_to_srgb(src.rgb), src.a);
    }

    /*
     * Convert sRGB to linear RGB
     */
    static vec3_t srgb_to_rgb(vec3_t src)
    {
        vec3_t ret = 1.0f / 12.92f * src;
        if (src.r > 0.04045f)
            ret.r = pow(src.r + 0.055f, 2.4f) / pow(1.055f, 2.4f);
        if (src.g > 0.04045f)
            ret.g = pow(src.g + 0.055f, 2.4f) / pow(1.055f, 2.4f);
        if (src.b > 0.04045f)
            ret.b = pow(src.b + 0.055f, 2.4f) / pow(1.055f, 2.4f);
        return ret;
    }

    static vec4_t srgb_to_rgb(vec4_t src)
    {
        return vec4_t(srgb_to_rgb(src.rgb), src.a);
    }

    /*
     * Convert linear RGB to YUV
     */
    static vec3_t rgb_to_yuv(vec3_t src)
    {
        mat3_t m(vec3_t(0.299f, -0.14713f,  0.615f),
                 vec3_t(0.587f, -0.28886f, -0.51499f),
                 vec3_t(0.114f,  0.436f,   -0.10001f));
        vec3_t tmp = m * src;
        tmp.r = (tmp.r * 220.f + 16.f) / 255.f;
        tmp.g += 0.5f;
        tmp.b += 0.5f;
        return clamp(tmp, 0.f, 1.f);
    }

    static vec4_t rgb_to_yuv(vec4_t src)
    {
        return vec4_t(rgb_to_yuv(src.rgb), src.a);
    }

    /*
     * Convert YUV to linear RGB
     */
    static vec3_t yuv_to_rgb(vec3_t src)
    {
        src.r = (src.r * 255.f - 16.f) / 220.f;
        src.g -= 0.5f;
        src.b -= 0.5f;
        mat3_t m(vec3_t(1.f,       1.f,      1.f),
                 vec3_t(0.f,      -0.39465f, 2.03211f),
                 vec3_t(1.13983f, -0.58060f, 0.f));
        return clamp(m * src, 0.f, 1.f);
    }

    static vec4_t yuv_to_rgb(vec4_t src)
    {
        return vec4_t(yuv_to_rgb(src.rgb), src.a);
    }

    /*
     * Convert linear HSV to linear RGB
     */
    static vec3_t hsv_to_rgb(vec3_t src)
    {
        vec3_t tmp = vec3_t(-1.f + abs(6.f * src.x - 3.f),
                         2.f - abs(6.f * src.x - 2.f),
                         2.f - abs(6.f * src.x - 4.f));
        return src.z * mix(vec3_t(1.f), clamp(tmp, 0.f, 1.f), src.y);
    }

    static vec4_t hsv_to_rgb(vec4_t src)
    {
        return vec4_t(hsv_to_rgb(src.rgb), src.a);
    }

    /*
     * Convert RGB to HSV
     */
    static vec3_t rgb_to_hsv(vec3_t src)
    {
        float K = 0.f;

        if (src.g < src.b)
            src = src.rbg, K = -1.f;

        if (src.r < src.g)
            src = src.grb, K = -2.f / 6.f - K;

        float chroma = src.r - min(src.g, src.b);
        /* XXX: we use min() here because numerical stability is not
         * guaranteed with -ffast-math, I’ve seen it fail on i386. */
        return vec3_t(min(abs(K + (src.g - src.b) / (6.f * chroma)), 1.f),
                    chroma / (src.r + 1e-20f),
                    src.r);
    }

    static vec4_t rgb_to_hsv(vec4_t src)
    {
        return vec4_t(rgb_to_hsv(src.rgb), src.a);
    }

    /*
     * Convert RGB to HSL
     */
    static vec3_t rgb_to_hsl(vec3_t src)
    {
        float K = 0.f;

        if (src.g < src.b)
            src = src.rbg, K = -1.f;

        if (src.r < src.g)
            src = src.grb, K = -2.f / 6.f - K;

        float chroma = src.r - min(src.g, src.b);
        float luma = src.r + min(src.g, src.b);
        /* XXX: we use min() here because numerical stability is not
         * guaranteed with -ffast-math, I’ve seen it fail on i386. */
        float h = min(abs(K + (src.g - src.b) / (6.f * chroma)), 1.f);
        float s = clamp(chroma / (min(luma, 2.f - luma)), 0.f, 1.f);
        return vec3_t(h, s, 0.5f * luma);
    }

    static vec4_t rgb_to_hsl(vec4_t src)
    {
        return vec4_t(rgb_to_hsl(src.rgb), src.a);
    }

    /*
     * Convert linear HSV to linear HSL
     */
    static vec3_t hsv_to_hsl(vec3_t src)
    {
        float tmp = (2 - src.y) * src.z;
        return vec3_t(src.x,
                      src.y * src.z / (min(tmp, 2.f - tmp) + 1e-20f),
                      0.5f * tmp);
    }

    static vec4_t hsv_to_hsl(vec4_t src)
    {
        return vec4_t(hsv_to_hsl(src.rgb), src.a);
    }

    /*
     * Convert linear HSL to linear HSV
     */
    static vec3_t hsl_to_hsv(vec3_t src)
    {
        float tmp = src.y * (0.5f - abs(0.5f - src.z));
        return vec3_t(src.x, 2.f * tmp / (src.z + tmp + 1e-20f), src.z + tmp);
    }

    static vec4_t hsl_to_hsv(vec4_t src)
    {
        return vec4_t(hsl_to_hsv(src.rgb), src.a);
    }

    /*
     * Convert linear RGB [0 1] to CIE XYZ [0 100]
     */
    static vec3_t rgb_to_ciexyz(vec3_t src)
    {
        mat3_t m(vec3_t(41.24f, 21.26f,  1.93f),
                 vec3_t(35.76f, 71.52f, 11.92f),
                 vec3_t(18.05f,  7.22f, 95.05f));
        return m * src;
    }

    static vec4_t rgb_to_ciexyz(vec4_t src)
    {
        return vec4_t(rgb_to_ciexyz(src.rgb), src.a);
    }

    /*
     * Convert CIE XYZ [0 100] to linear RGB [0 1]
     */
    static vec3_t ciexyz_to_rgb(vec3_t src)
    {
        mat3_t m(vec3_t( 0.032406f, -0.009689f,  0.000557f),
                 vec3_t(-0.015372f,  0.018758f, -0.002040f),
                 vec3_t(-0.004986f,  0.000415f,  0.010570f));
        return m * src;
    }

    static vec4_t ciexyz_to_rgb(vec4_t src)
    {
        return vec4_t(ciexyz_to_rgb(src.rgb), src.a);
    }

    /*
     * Convert CIE XYZ to CIE xyY
     */
    static vec3_t ciexyz_to_ciexyy(vec3_t src)
    {
        float tmp = 1.0f / (src.x + src.y + src.z);
        return vec3_t(src.x * tmp, src.y * tmp, src.y);
    }

    static vec4_t ciexyz_to_ciexyy(vec4_t src)
    {
        return vec4_t(ciexyz_to_ciexyy(src.rgb), src.a);
    }

    /*
     * Convert CIE xyY to CIE XYZ
     */
    static vec3_t ciexyy_to_ciexyz(vec3_t src)
    {
        return src.z * vec3_t(src.x / src.y,
                            1.0f,
                            (1.0f - src.x - src.y) / src.y);
    }

    static vec4_t ciexyy_to_ciexyz(vec4_t src)
    {
        return vec4_t(ciexyy_to_ciexyz(src.rgb), src.a);
    }

    /*
     * Convert CIE XYZ to CIE L*a*b*
     * Note: XYZ values are normalised using a D65 illuminant if
     * no white value is provided.
     */
    static vec3_t ciexyz_to_cielab(vec3_t src, vec3_t white_value)
    {
        float const a = (6.0f * 6.0f * 6.0f) / (29 * 29 * 29);
        float const b = (29.0f * 29.0f) / (3 * 6 * 6);
        float const c = 4.0f / 29;
        float const d = 1.0f / 3;

        src /= white_value;

        vec3_t f = b * src + vec3_t(c);
        if (src.x > a)
            f.x = pow(src.x, d);
        if (src.y > a)
            f.y = pow(src.y, d);
        if (src.z > a)
            f.z = pow(src.z, d);

        return vec3_t(116.f * f.y - 16.f,
                      500.f * (f.x - f.y),
                      200.f * (f.y - f.z));
    }

    static vec3_t ciexyz_to_cielab(vec3_t src)
    {
        return ciexyz_to_cielab(src, vec3_t(95.047f, 100.f, 108.883f));
    }

    static vec4_t ciexyz_to_cielab(vec4_t src, vec4_t white_value)
    {
        return vec4_t(ciexyz_to_cielab(src.rgb, white_value.rgb), src.a);
    }

    static vec4_t ciexyz_to_cielab(vec4_t src)
    {
        return vec4_t(ciexyz_to_cielab(src.rgb), src.a);
    }

    /*
     * Return distance using the CIEDE2000 metric,
     * input should be in CIE L*a*b*.
     */
    static T distance_ciede2k(vec3_t lab1, vec3_t lab2);

    /*
     * Convert a wavelength into an xyY fully saturated colour.
     */
    static vec3_t wavelength_to_ciexyy(float nm);

    /*
    * Convert uint color to vec4_t.
    */
    static vec4_t from_rgba32(uint32_t c);
    static vec4_t from_rgb32(uint32_t c);
    static vec4_t from_rgba16(uint16_t c);
    static vec4_t from_rgb16(uint16_t c);

    /*
    * Convert uint color to vec4_t.
    */
    static uint32_t to_rgba32(vec4_t c);
    static uint32_t to_rgb32(vec4_t c);
    static uint16_t to_rgba16(vec4_t c);
    static uint16_t to_rgb16(vec4_t c);
};

using color = color_t<float>;

} /* namespace lol */

#include "private/color.ipp"
#include "private/cie1931.ipp"
