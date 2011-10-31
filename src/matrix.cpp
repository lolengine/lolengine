//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined WIN32 && !defined _XBOX
#   define _USE_MATH_DEFINES /* for M_PI */
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

#include <cmath> /* for M_PI */
#include <cstdlib> /* free() */
#include <cstring> /* strdup() */

#include "core.h"

using namespace std;

namespace lol
{

template<> float dot(vec2 v1, vec2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

template<> float dot(vec3 v1, vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<> float dot(vec4 v1, vec4 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template<> vec3 cross(vec3 v1, vec3 v2)
{
    return vec3(v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
}

template<> vec2 normalize(vec2 v)
{
    float norm = v.len();
    if (!norm)
        return vec2(0);
    return v / norm;
}

template<> vec3 normalize(vec3 v)
{
    float norm = v.len();
    if (!norm)
        return vec3(0);
    return v / norm;
}

template<> vec4 normalize(vec4 v)
{
    float norm = v.len();
    if (!norm)
        return vec4(0);
    return v / norm;
}

static inline float det3(float a, float b, float c,
                         float d, float e, float f,
                         float g, float h, float i)
{
    return a * (e * i - h * f)
         + b * (f * g - i * d)
         + c * (d * h - g * e);
}

static inline float cofact3(mat4 const &mat, int i, int j)
{
    return det3(mat[(i + 1) & 3][(j + 1) & 3],
                mat[(i + 2) & 3][(j + 1) & 3],
                mat[(i + 3) & 3][(j + 1) & 3],
                mat[(i + 1) & 3][(j + 2) & 3],
                mat[(i + 2) & 3][(j + 2) & 3],
                mat[(i + 3) & 3][(j + 2) & 3],
                mat[(i + 1) & 3][(j + 3) & 3],
                mat[(i + 2) & 3][(j + 3) & 3],
                mat[(i + 3) & 3][(j + 3) & 3]) * (((i + j) & 1) ? -1.0f : 1.0f);
}

template<> float mat4::det() const
{
    float ret = 0;
    for (int n = 0; n < 4; n++)
        ret += (*this)[n][0] * cofact3(*this, n, 0);
    return ret;
}

template<> mat4 mat4::invert() const
{
    mat4 ret;
    float d = det();
    if (d)
    {
        d = 1.0f / d;
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
                ret[j][i] = cofact3(*this, i, j) * d;
    }
    return ret;
}

template<> void vec2::printf() const
{
    Log::Debug("[ %6.6f %6.6f ]\n", x, y);
}

template<> void vec3::printf() const
{
    Log::Debug("[ %6.6f %6.6f %6.6f ]\n", x, y, z);
}

template<> void vec4::printf() const
{
    Log::Debug("[ %6.6f %6.6f %6.6f %6.6f ]\n", x, y, z, w);
}

template<> void mat4::printf() const
{
    mat4 const &p = *this;

    Log::Debug("[ %6.6f %6.6f %6.6f %6.6f\n",
               p[0][0], p[1][0], p[2][0], p[3][0]);
    Log::Debug("  %6.6f %6.6f %6.6f %6.6f\n",
               p[0][1], p[1][1], p[2][1], p[3][1]);
    Log::Debug("  %6.6f %6.6f %6.6f %6.6f\n",
               p[0][2], p[1][2], p[2][2], p[3][2]);
    Log::Debug("  %6.6f %6.6f %6.6f %6.6f ]\n",
               p[0][3], p[1][3], p[2][3], p[3][3]);
}

#if !defined __ANDROID__
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

template<> std::ostream &operator<<(std::ostream &stream, vec3 const &v)
{
    return stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template<> std::ostream &operator<<(std::ostream &stream, vec4 const &v)
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
#endif

template<> mat4 mat4::translate(float x, float y, float z)
{
    mat4 ret(1.0f);
    ret[3][0] = x;
    ret[3][1] = y;
    ret[3][2] = z;
    return ret;
}

template<> mat4 mat4::translate(vec3 v)
{
    return translate(v.x, v.y, v.z);
}

template<> mat4 mat4::rotate(float angle, float x, float y, float z)
{
    angle *= (M_PI / 180.0f);

    float st = sinf(angle);
    float ct = cosf(angle);

    float len = sqrtf(x * x + y * y + z * z);
    float invlen = len ? 1.0f / len : 0.0f;
    x *= invlen;
    y *= invlen;
    z *= invlen;

    float mtx = (1.0f - ct) * x;
    float mty = (1.0f - ct) * y;
    float mtz = (1.0f - ct) * z;

    mat4 ret(1.0f);

    ret[0][0] = x * mtx + ct;
    ret[0][1] = x * mty + st * z;
    ret[0][2] = x * mtz - st * y;

    ret[1][0] = y * mtx - st * z;
    ret[1][1] = y * mty + ct;
    ret[1][2] = y * mtz + st * x;

    ret[2][0] = z * mtx + st * y;
    ret[2][1] = z * mty - st * x;
    ret[2][2] = z * mtz + ct;

    return ret;
}

template<> mat4 mat4::rotate(float angle, vec3 v)
{
    return rotate(angle, v.x, v.y, v.z);
}

template<> mat4 mat4::lookat(vec3 eye, vec3 center, vec3 up)
{
    vec3 v3 = normalize(eye - center);
    vec3 v2 = normalize(up);
    vec3 v1 = normalize(cross(v2, v3));
    v2 = cross(v3, v1);

    mat4 orient(1.0f);
    orient[0][0] = v1.x;
    orient[0][1] = v2.x;
    orient[0][2] = v3.x;
    orient[1][0] = v1.y;
    orient[1][1] = v2.y;
    orient[1][2] = v3.y;
    orient[2][0] = v1.z;
    orient[2][1] = v2.z;
    orient[2][2] = v3.z;

    return orient * mat4::translate(-eye);
}

template<> mat4 mat4::ortho(float left, float right, float bottom,
                            float top, float near, float far)
{
    float invrl = (right != left) ? 1.0f / (right - left) : 0.0f;
    float invtb = (top != bottom) ? 1.0f / (top - bottom) : 0.0f;
    float invfn = (far != near) ? 1.0f / (far - near) : 0.0f;

    mat4 ret(0.0f);
    ret[0][0] = 2.0f * invrl;
    ret[1][1] = 2.0f * invtb;
    ret[2][2] = -2.0f * invfn;
    ret[3][0] = - (right + left) * invrl;
    ret[3][1] = - (top + bottom) * invtb;
    ret[3][2] = - (far + near) * invfn;
    ret[3][3] = 1.0f;
    return ret;
}

template<> mat4 mat4::frustum(float left, float right, float bottom,
                              float top, float near, float far)
{
    float invrl = (right != left) ? 1.0f / (right - left) : 0.0f;
    float invtb = (top != bottom) ? 1.0f / (top - bottom) : 0.0f;
    float invfn = (far != near) ? 1.0f / (far - near) : 0.0f;

    mat4 ret(0.0f);
    ret[0][0] = 2.0f * near * invrl;
    ret[1][1] = 2.0f * near * invtb;
    ret[2][0] = (right + left) * invrl;
    ret[2][1] = (top + bottom) * invtb;
    ret[2][2] = - (far + near) * invfn;
    ret[2][3] = -1.0f;
    ret[3][2] = -2.0f * far * near * invfn;
    return ret;
}

template<> mat4 mat4::perspective(float fov_y, float width,
                                  float height, float near, float far)
{
    fov_y *= (M_PI / 180.0f);

    float t2 = tanf(fov_y * 0.5f);
    float t1 = t2 * width / height;

    return frustum(-near * t1, near * t1, -near * t2, near * t2, near, far);
}

} /* namespace lol */

