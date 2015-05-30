//
//  Lol Engine
//
//  Copyright © 2004—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Pixel-related classes
// -------------------------
//

#include <lol/base/types.h>
#include <lol/math/vector.h>

namespace lol
{

/* The pixel formats we know about */
enum class PixelFormat
{
    /* XXX: make sure to update image.cpp and texture.cpp when this changes */
    Unknown,
    Y_8,
    RGB_8,
    RGBA_8,
    Y_F32,
    RGB_F32,
    RGBA_F32,
};

/* Associated storage types for each pixel format */
template <PixelFormat T> struct PixelType { typedef void type; };
template<> struct PixelType<PixelFormat::Y_8> { typedef uint8_t type; };
template<> struct PixelType<PixelFormat::RGB_8> { typedef u8vec3 type; };
template<> struct PixelType<PixelFormat::RGBA_8> { typedef u8vec4 type; };
template<> struct PixelType<PixelFormat::Y_F32> { typedef float type; };
template<> struct PixelType<PixelFormat::RGB_F32> { typedef vec3 type; };
template<> struct PixelType<PixelFormat::RGBA_F32> { typedef vec4 type; };

/* Number of bytes used by each pixel format */
static inline uint8_t BytesPerPixel(PixelFormat format)
{
#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wswitch"
#endif
    switch (format)
    {
    case PixelFormat::Unknown:
        break;
    case PixelFormat::Y_8:
        return sizeof(PixelType<PixelFormat::Y_8>::type);
    case PixelFormat::RGB_8:
        return sizeof(PixelType<PixelFormat::RGB_8>::type);
    case PixelFormat::RGBA_8:
        return sizeof(PixelType<PixelFormat::RGBA_8>::type);
    case PixelFormat::Y_F32:
        return sizeof(PixelType<PixelFormat::Y_F32>::type);
    case PixelFormat::RGB_F32:
        return sizeof(PixelType<PixelFormat::RGB_F32>::type);
    case PixelFormat::RGBA_F32:
        return sizeof(PixelType<PixelFormat::RGBA_F32>::type);
    }
    return 0;
#if __GNUC__
#pragma GCC diagnostic pop
#endif
};

} /* namespace lol */

