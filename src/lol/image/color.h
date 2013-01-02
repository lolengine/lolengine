//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Color class
// ---------------
// Provides various color conversion routines.
//

#if !defined __LOL_IMAGE_COLOR_H__
#define __LOL_IMAGE_COLOR_H__

#include <lol/math/vector.h>

namespace lol
{

class Color
{
public:
    /*
     * Convert linear RGB to sRGB
     */
    static vec3 LinearRGBTosRGB(vec3 src)
    {
        vec3 ret = 12.92f * src;
        if (src.r > 0.0031308f)
            ret.r = 1.055f * pow(src.r, 1.0f / 2.4f) - 0.055f;
        if (src.g > 0.0031308f)
            ret.g = 1.055f * pow(src.g, 1.0f / 2.4f) - 0.055f;
        if (src.b > 0.0031308f)
            ret.b = 1.055f * pow(src.b, 1.0f / 2.4f) - 0.055f;
        return ret;
    }

    static vec4 LinearRGBTosRGB(vec4 src)
    {
        return vec4(LinearRGBTosRGB(src.rgb), src.a);
    }

    /*
     * Convert sRGB to linear RGB
     */
    static vec3 sRGBToLinearRGB(vec3 src)
    {
        vec3 ret = 1.0f / 12.92f * src;
        if (src.r > 0.04045f)
            ret.r = pow(src.r + 0.055f, 2.4f) / pow(1.055f, 2.4f);
        if (src.g > 0.04045f)
            ret.g = pow(src.g + 0.055f, 2.4f) / pow(1.055f, 2.4f);
        if (src.b > 0.04045f)
            ret.b = pow(src.b + 0.055f, 2.4f) / pow(1.055f, 2.4f);
        return ret;
    }

    static vec4 sRGBToLinearRGB(vec4 src)
    {
        return vec4(sRGBToLinearRGB(src.rgb), src.a);
    }

    /*
     * Convert linear HSV to linear RGB
     */
    static vec3 HSVToLinearRGB(vec3 src)
    {
        vec3 tmp = abs(fract(vec3(src.x) + vec3(3.f, 2.f, 1.f) / 3.f) * 6.f - vec3(3.f));
        return mix(vec3(1.f), clamp(tmp - vec3(1.f), 0.f, 1.f), src.y) * src.z;
    }

    static vec4 HSVToLinearRGB(vec4 src)
    {
        return vec4(HSVToLinearRGB(src.rgb), src.a);
    }

    /*
     * Convert linear HSV to linear HSL
     */
    static vec3 HSVToHSL(vec3 src)
    {
        float tmp = (2 - src.y) * src.z;
        return vec3(src.x, src.y * src.z / (1.f - abs(1.f - tmp)), 0.5f * tmp);
    }

    static vec4 HSVToHSL(vec4 src)
    {
        return vec4(HSVToHSL(src.rgb), src.a);
    }

    /*
     * Convert linear HSL to linear HSV
     */
    static vec3 HSLToHSV(vec3 src)
    {
        float tmp = src.y * (0.5f - abs(0.5f - src.z));
        return vec3(src.x, 2.f * tmp / (src.z + tmp), src.z + tmp);
    }

    static vec4 HSLToHSV(vec4 src)
    {
        return vec4(HSLToHSV(src.rgb), src.a);
    }

    /*
     * Convert linear RGB to CIE XYZ
     */
    static vec3 LinearRGBToCIEXYZ(vec3 src)
    {
        mat3 m(vec3(0.4124f, 0.2126f, 0.0193f),
               vec3(0.3576f, 0.7152f, 0.1192f),
               vec3(0.1805f, 0.0722f, 0.9505f));
        return m * src;
    }

    static vec4 LinearRGBToCIEXYZ(vec4 src)
    {
        return vec4(LinearRGBToCIEXYZ(src.rgb), src.a);
    }

    /*
     * Convert CIE XYZ to linear RGB
     */
    static vec3 CIEXYZToLinearRGB(vec3 src)
    {
        mat3 m(vec3(3.2406f, -0.9689f, 0.0557f),
               vec3(-1.5372f, 1.8758f, -0.2040f),
               vec3(-0.4986f, 0.0415f, 1.0570f));
        return m * src;
    }

    static vec4 CIEXYZToLinearRGB(vec4 src)
    {
        return vec4(CIEXYZToLinearRGB(src.rgb), src.a);
    }

    /*
     * Convert CIE XYZ to CIE xyY
     */
    static vec3 CIEXYZToCIExyY(vec3 src)
    {
        float tmp = 1.0f / (src.x + src.y + src.z);
        return vec3(src.x * tmp, src.y * tmp, src.y);
    }

    static vec4 CIEXYZToCIExyY(vec4 src)
    {
        return vec4(CIEXYZToCIExyY(src.rgb), src.a);
    }

    /*
     * Convert CIE XYZ to CIE L*a*b*
     * Note: XYZ values are normalised using a D65 illuminant.
     */
    static vec3 CIEXYZToCIELab(vec3 src)
    {
        float const a = (6.0 * 6.0 * 6.0) / (29 * 29 * 29);
        float const b = (29.0 * 29.0) / (3 * 6 * 6);
        float const c = 4.0 / 29;
        float const d = 1.0 / 3;

        /* Normalise XYZ using D65 illuminant */
        src *= vec3(1.f / 95.047f, 1.f / 100.f, 1.f / 108.883f);

        vec3 f = b * src + vec3(c);
        if (src.x > a)
            f.x = pow(src.x, d);
        if (src.y > a)
            f.y = pow(src.y, d);
        if (src.z > a)
            f.z = pow(src.z, d);

        return vec3(116.f * f.y - 16.f,
                    500.f * (f.x - f.y),
                    200.f * (f.y - f.z));
    }

    static vec4 CIEXYZToCIELab(vec4 src)
    {
        return vec4(CIEXYZToLinearRGB(src.rgb), src.a);
    }

    /*
     * Return distance using the CIEDE2000 metric,
     * input should be in CIE L*a*b*.
     */
    static float DistanceCIEDE2000(vec3 lab1, vec3 lab2)
    {
        float const deg2rad = 6.28318530718f / 360.f;
        float const rad2deg = 360.f / 6.28318530718f;

        float C1 = length(lab1.yz);
        float C2 = length(lab2.yz);
        float C_ = 0.5f * (C1 + C2);

        float L1 = lab1.x;
        float L2 = lab2.x;
        float dLp = L2 - L1;
        float L_ = 0.5f * (L1 + L2);

        float tmp1 = pow(C_, 7.f);
        float tmp2 = 1.5f - 0.5f * sqrt(tmp1 / (tmp1 + pow(25.f, 7.f)));
        float ap1 = lab1.y * tmp2;
        float ap2 = lab2.y * tmp2;
        float Cp1 = sqrt(ap1 * ap1 + lab1.z * lab1.z);
        float Cp2 = sqrt(ap2 * ap2 + lab2.z * lab2.z);
        float dCp = Cp2 - Cp1;
        float Cp_ = 0.5f * (Cp1 + Cp2);

        float hp1 = fmod(atan2(lab1.z, ap1) * rad2deg, 360.f);
        if (hp1 < 0.f)
            hp1 += 360.f;
        float hp2 = fmod(atan2(lab2.z, ap2) * rad2deg, 360.f);
        if (hp2 < 0.f)
            hp2 += 360.f;
        float dhp;
        if (abs(hp1 - hp2) <= 180.f)
            dhp = hp2 - hp1;
        else if (hp2 <= hp1)
            dhp = hp2 - hp1 + 360.f;
        else
            dhp = hp2 - hp1 - 360.f;
        float dHp = 2.f * sqrt(Cp1 * Cp2) * sin(dhp / 2.f * deg2rad);
        float Hp_;
        if (abs(hp1 - hp2) > 180.f)
            Hp_ = 0.5f * (hp1 + hp2 + 360.f);
        else
            Hp_ = 0.5f * (hp1 + hp2);

        float T = 1.f - 0.17f * cos((Hp_ - 30.f) * deg2rad)
                      + 0.24f * cos(2 * Hp_ * deg2rad)
                      + 0.32f * cos((3.f * Hp_ + 6.f) * deg2rad)
                      - 0.20f * cos((4.f * Hp_ - 63.f) * deg2rad);
        float SL = 1.f + 0.015f * (L_ - 50) * (L_ - 50)
                                / sqrt(20.f + (L_ - 50) * (L_ - 50));
        float SC = 1.f + 0.045f * Cp_;
        float SH = 1.f + 0.015f * Cp_ * T;
        float RT = -2.f * sqrt(pow(Cp_, 7.f) / (pow(Cp_, 7.f) + pow(25.f, 7.f)))
                        * sin(60.f * deg2rad * exp(-pow((Hp_ - 275.f) / 25.f, 2.f)));

        dLp /= SL;
        dCp /= SC;
        dHp /= SH;

        return sqrt(dLp * dLp + dCp * dCp + dHp * dHp + RT * dCp * dHp);
    }
};

} /* namespace lol */

#endif // __LOL_IMAGE_COLOR_H__

