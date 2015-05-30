//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <ostream> /* std::ostream */

namespace lol
{

#define LOL_PRINTF_TOSTRING(type, ...) \
template<> void type::printf() const        { msg::debug(__VA_ARGS__); } \
template<> String type::tostring() const    { return String::format(__VA_ARGS__); }

LOL_PRINTF_TOSTRING(vec2,   "[ %6.6f %6.6f ]\n", x, y);
LOL_PRINTF_TOSTRING(ivec2,  "[ %i %i ]\n", x, y);
LOL_PRINTF_TOSTRING(cmplx,  "[ %6.6f %6.6f ]\n", x, y);
LOL_PRINTF_TOSTRING(vec3,   "[ %6.6f %6.6f %6.6f ]\n", x, y, z);
LOL_PRINTF_TOSTRING(ivec3,  "[ %i %i %i ]\n", x, y, z);
LOL_PRINTF_TOSTRING(vec4,   "[ %6.6f %6.6f %6.6f %6.6f ]\n", x, y, z, w);
LOL_PRINTF_TOSTRING(ivec4,  "[ %i %i %i %i ]\n", x, y, z, w);
LOL_PRINTF_TOSTRING(quat,   "[ %6.6f %6.6f %6.6f %6.6f ]\n", w, x, y, z);

template<> void mat2::printf() const
{
    mat2 const &p = *this;

    msg::debug("[ %6.6f %6.6f\n", p[0][0], p[1][0]);
    msg::debug("  %6.6f %6.6f ]\n", p[0][1], p[1][1]);
}

template<> String mat2::tostring() const
{
    mat2 const &p = *this;

    return String::format("[ %6.6f %6.6f\n", p[0][0], p[1][0]) +
           String::format("  %6.6f %6.6f ]\n", p[0][1], p[1][1]);
}

template<> void mat3::printf() const
{
    mat3 const &p = *this;

    msg::debug("[ %6.6f %6.6f %6.6f\n", p[0][0], p[1][0], p[2][0]);
    msg::debug("  %6.6f %6.6f %6.6f\n", p[0][1], p[1][1], p[2][1]);
    msg::debug("  %6.6f %6.6f %6.6f ]\n", p[0][2], p[1][2], p[2][2]);
}

template<> String mat3::tostring() const
{
    mat3 const &p = *this;

    return String::format("[ %6.6f %6.6f %6.6f\n", p[0][0], p[1][0], p[2][0]) +
           String::format("  %6.6f %6.6f %6.6f\n", p[0][1], p[1][1], p[2][1]) +
           String::format("  %6.6f %6.6f %6.6f ]\n", p[0][2], p[1][2], p[2][2]);
}

template<> void mat4::printf() const
{
    mat4 const &p = *this;

    msg::debug("[ %6.6f %6.6f %6.6f %6.6f\n",
               p[0][0], p[1][0], p[2][0], p[3][0]);
    msg::debug("  %6.6f %6.6f %6.6f %6.6f\n",
               p[0][1], p[1][1], p[2][1], p[3][1]);
    msg::debug("  %6.6f %6.6f %6.6f %6.6f\n",
               p[0][2], p[1][2], p[2][2], p[3][2]);
    msg::debug("  %6.6f %6.6f %6.6f %6.6f ]\n",
               p[0][3], p[1][3], p[2][3], p[3][3]);
}

template<> String mat4::tostring() const
{
    mat4 const &p = *this;

    return String::format("[ %6.6f %6.6f %6.6f %6.6f\n",
                            p[0][0], p[1][0], p[2][0], p[3][0]) +
           String::format("  %6.6f %6.6f %6.6f %6.6f\n",
                            p[0][1], p[1][1], p[2][1], p[3][1]) +
           String::format("  %6.6f %6.6f %6.6f %6.6f\n",
                            p[0][2], p[1][2], p[2][2], p[3][2]) +
           String::format("  %6.6f %6.6f %6.6f %6.6f ]\n",
                            p[0][3], p[1][3], p[2][3], p[3][3]);
}

template<> std::ostream &operator<<(std::ostream &stream, ivec2 const &v)
{
    return stream << "(" << v.x << ", " << v.y << ")";
}

template<> std::ostream &operator<<(std::ostream &stream, ivec3 const &v)
{
    return stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template<> std::ostream &operator<<(std::ostream &stream, ivec4 const &v)
{
    return stream << "(" << v.x << ", " << v.y << ", "
                         << v.z << ", " << v.w << ")";
}

template<> std::ostream &operator<<(std::ostream &stream, vec2 const &v)
{
    return stream << "(" << v.x << ", " << v.y << ")";
}

template<> std::ostream &operator<<(std::ostream &stream, cmplx const &v)
{
    return stream << "(" << v.x << ", " << v.y << ")";
}

template<> std::ostream &operator<<(std::ostream &stream, vec3 const &v)
{
    return stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template<> std::ostream &operator<<(std::ostream &stream, vec4 const &v)
{
    return stream << "(" << v.x << ", " << v.y << ", "
                         << v.z << ", " << v.w << ")";
}

template<> std::ostream &operator<<(std::ostream &stream, quat const &v)
{
    return stream << "(" << v.x << ", " << v.y << ", "
                         << v.z << ", " << v.w << ")";
}

template<> std::ostream &operator<<(std::ostream &stream, mat4 const &m)
{
    stream << "((" << m[0][0] << ", " << m[1][0]
            << ", " << m[2][0] << ", " << m[3][0] << "), ";
    stream << "(" << m[0][1] << ", " << m[1][1]
           << ", " << m[2][1] << ", " << m[3][1] << "), ";
    stream << "(" << m[0][2] << ", " << m[1][2]
           << ", " << m[2][2] << ", " << m[3][2] << "), ";
    stream << "(" << m[0][3] << ", " << m[1][3]
           << ", " << m[2][3] << ", " << m[3][3] << "))";
    return stream;
}

} /* namespace lol */

