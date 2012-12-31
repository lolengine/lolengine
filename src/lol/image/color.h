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
    static vec3 LinearRGBTosRGB(vec3 c)
    {
        vec3 ret = 12.92f * c;
        if (c.r > 0.0031308f)
            ret.r = 1.055f * pow(c.r, 1.0f / 2.4f) - 0.055f;
        if (c.g > 0.0031308f)
            ret.g = 1.055f * pow(c.g, 1.0f / 2.4f) - 0.055f;
        if (c.b > 0.0031308f)
            ret.b = 1.055f * pow(c.b, 1.0f / 2.4f) - 0.055f;
        return ret;
    }

    static vec4 LinearRGBTosRGB(vec4 c)
    {
        return vec4(LinearRGBTosRGB(c.rgb), c.a);
    }

    /*
     * Convert sRGB to linear RGB
     */
    static vec3 sRGBToLinearRGB(vec3 c)
    {
        vec3 ret = 1.0f / 12.92f * c;
        if (c.r > 0.04045f)
            ret.r = pow(c.r + 0.055f, 2.4f) / pow(1.055f, 2.4f);
        if (c.g > 0.04045f)
            ret.g = pow(c.g + 0.055f, 2.4f) / pow(1.055f, 2.4f);
        if (c.b > 0.04045f)
            ret.b = pow(c.b + 0.055f, 2.4f) / pow(1.055f, 2.4f);
        return ret;
    }

    static vec4 sRGBToLinearRGB(vec4 c)
    {
        return vec4(sRGBToLinearRGB(c.rgb), c.a);
    }

    /*
     * Convert linear RGB to CIE XYZ
     */
    static vec3 LinearRGBToCIEXYZ(vec3 c)
    {
        mat3 m(vec3(3.2406f, -0.9689f, 0.0557f),
               vec3(-1.5372f, 1.8758f, -0.2040f),
               vec3(-0.4986f, 0.0415f, 1.0570f));
        return m * c;
    }

    static vec4 LinearRGBToCIEXYZ(vec4 c)
    {
        return vec4(LinearRGBToCIEXYZ(c.rgb), c.a);
    }

    /*
     * Convert CIE XYZ to linear RGB
     */
    static vec3 CIEXYZToLinearRGB(vec3 c)
    {
        mat3 m(vec3(0.4124f, 0.2126f, 0.0193f),
               vec3(0.3576f, 0.7152f, 0.1192f),
               vec3(0.1805f, 0.0722f, 0.9505f));
        return m * c;
    }

    static vec4 CIEXYZToLinearRGB(vec4 c)
    {
        return vec4(CIEXYZToLinearRGB(c.rgb), c.a);
    }
};

} /* namespace lol */

#endif // __LOL_IMAGE_COLOR_H__

