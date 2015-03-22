//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{

/*
 * Hardcoded constants for colours
 */
vec4 const Color::black     = vec4(0, 0, 0, 1);
vec4 const Color::red       = vec4(1, 0, 0, 1);
vec4 const Color::green     = vec4(0, 1, 0, 1);
vec4 const Color::yellow    = vec4(1, 1, 0, 1);
vec4 const Color::blue      = vec4(0, 0, 1, 1);
vec4 const Color::magenta   = vec4(1, 0, 1, 1);
vec4 const Color::cyan      = vec4(0, 1, 1, 1);
vec4 const Color::white     = vec4(1, 1, 1, 1);
vec4 const Color::gray_dark = vec4(.25f, .25f, .25f, 1);
vec4 const Color::gray      = vec4(.5f, .5f, .5f, 1);
vec4 const Color::gray_light= vec4(.75f, .75f, .75f, 1);

/*
 * Conversion from colours to hexadecimal
 */
String Color::HexString4Bpp(vec3 c)
{
    uvec3 c2 = (uvec3)(clamp(c, 0.f, 1.f) * 255.999f);
    return String::Printf("%01x%01x%01x", c2.r, c2.g, c2.b);
}

String Color::HexString4Bpp(vec4 c)
{
    uvec4 c2 = (uvec4)(clamp(c, 0.f, 1.f) * 15.999f);
    return String::Printf("%01x%1x%01x%01x", c2.r, c2.g, c2.b, c2.a);
}

String Color::HexString8Bpp(vec3 c)
{
    uvec3 c2 = (uvec3)(clamp(c, 0.f, 1.f) * 255.999f);
    return String::Printf("%02x%02x%02x", c2.r, c2.g, c2.b);
}

String Color::HexString8Bpp(vec4 c)
{
    uvec4 c2 = (uvec4)(clamp(c, 0.f, 1.f) * 15.999f);
    return String::Printf("%02x%2x%02x%02x", c2.r, c2.g, c2.b, c2.a);
}

/*
* Convert uint color to vec4.
*/
vec4 Color::FromRGBA32(uint32_t c)
{
    ivec4 v(c >> 24, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff);
    return vec4(v) * (1.f / 255.f);
}
vec4 Color::FromRGB32(uint32_t c)
{
    return Color::FromRGBA32((c << 8) | 0x000000ffu);
}
vec4 Color::FromRGBA16(uint16_t c)
{
    return Color::FromRGBA32
        ( 0x11000000u * (c >> 12)
        | 0x00110000u * ((c >> 8) & 0xf)
        | 0x00001100u * ((c >> 4) & 0xf)
        | 0x00000011u * (c & 0xf));
}
vec4 Color::FromRGB16(uint16_t c)
{
    return Color::FromRGBA16((c << 4) | 0xfu);
}

/*
* Convert uint color to vec4.
*/
uint32_t Color::ToRGBA32(vec4 c)
{
    ivec4 v(c * 0xff);
    return (uint32_t)((v.r << 24) | (v.g << 16) | (v.b << 8) | v.a);
}
uint32_t Color::ToRGB32(vec4 c)
{
    ivec4 v(c * 0xff);
    return (uint32_t)((v.r << 16) | (v.g << 8) | v.b);
}
uint16_t Color::ToRGBA16(vec4 c)
{
    ivec4 v(c * 0xf);
    return (uint16_t)((v.r << 12) | (v.g << 8) | (v.b << 4) | v.a);
}
uint16_t Color::ToRGB16(vec4 c)
{
    ivec4 v(c * 0xf);
    return (uint16_t)((v.r << 8) | (v.g << 4) | v.b);
}

/*
* Conversion from colours to hexadecimal
*/
vec4 Color::C8BppHexString(String s)
{
    String c = s[0] == '#' ? &s[1] : s;
    uint32_t tmp = std::strtol(c.C(), nullptr, 16);
    if (c.Count() == 3)
    {
        tmp = 0x11000000u * (tmp >> 8)
            | 0x00110000u * ((tmp >> 4) & 0xf)
            | 0x00001100u * (tmp & 0xf)
            | 0x000000ffu;
    }
    else if (c.Count() == 4)
    {
        tmp = 0x11000000u * (tmp >> 12)
            | 0x00110000u * ((tmp >> 8) & 0xf)
            | 0x00001100u * ((tmp >> 4) & 0xf)
            | 0x00000011u * (tmp & 0xf);
    }
    else if (c.Count() == 6)
    {
        tmp = 0xffu | 0x100u * tmp;
    }
    vec4 color(0.f);
    ivec4 v(tmp >> 24, (tmp >> 16) & 0xff, (tmp >> 8) & 0xff, tmp & 0xff);
    return vec4(v) * (1.f / 255.f);
}

} /* namespace lol */

