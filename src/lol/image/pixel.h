//
// Lol Engine
//
// Copyright: (c) 2004-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Pixel-related classes
// -------------------------
//

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
        return 1;
    case PixelFormat::RGB_8:
        return 3;
    case PixelFormat::RGBA_8:
        return 4;
    case PixelFormat::Y_F32:
        return 4;
    case PixelFormat::RGB_F32:
        return 12;
    case PixelFormat::RGBA_F32:
        return 16;
    }
    return 0;
#if __GNUC__
#pragma GCC diagnostic pop
#endif
};

} /* namespace lol */

