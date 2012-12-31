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
        using std::pow;
        vec3 ret = 12.92 * c;
        if (c.r > 0.0031308)
            ret.r = 1.055 * pow(c.r, 1.0 / 2.4) - 0.055;
        if (c.g > 0.0031308)
            ret.g = 1.055 * pow(c.g, 1.0 / 2.4) - 0.055;
        if (c.b > 0.0031308)
            ret.b = 1.055 * pow(c.b, 1.0 / 2.4) - 0.055;
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
        using std::pow;
        vec3 ret = 1.0 / 12.92 * c;
        if (c.r > 0.04045)
            ret.r = pow(c.r + 0.055, 2.4) / pow(1.055, 2.4);
        if (c.g > 0.04045)
            ret.g = pow(c.g + 0.055, 2.4) / pow(1.055, 2.4);
        if (c.b > 0.04045)
            ret.b = pow(c.b + 0.055, 2.4) / pow(1.055, 2.4);
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
        mat3 m(vec3(3.2406, -0.9689, 0.0557),
               vec3(-1.5372, 1.8758, -0.2040),
               vec3(-0.4986, 0.0415, 1.0570));
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
        mat3 m(vec3(0.4124, 0.2126, 0.0193),
               vec3(0.3576, 0.7152, 0.1192),
               vec3(0.1805, 0.0722, 0.9505));
        return m * c;
    }

    static vec4 CIEXYZToLinearRGB(vec4 c)
    {
        return vec4(CIEXYZToLinearRGB(c.rgb), c.a);
    }
};

} /* namespace lol */

#endif // __LOL_IMAGE_COLOR_H__

