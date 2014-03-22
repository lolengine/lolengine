//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

namespace lol
{

/*
 * Hardcoded constants for colours
 */
vec4 const Color::black   = vec4(0, 0, 0, 1);
vec4 const Color::red     = vec4(1, 0, 0, 1);
vec4 const Color::green   = vec4(0, 1, 0, 1);
vec4 const Color::yellow  = vec4(1, 1, 0, 1);
vec4 const Color::blue    = vec4(0, 0, 1, 1);
vec4 const Color::magenta = vec4(1, 0, 1, 1);
vec4 const Color::cyan    = vec4(0, 1, 1, 1);
vec4 const Color::white   = vec4(1, 1, 1, 1);

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

} /* namespace lol */

