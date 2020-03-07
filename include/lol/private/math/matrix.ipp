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

#include <cassert>
#include <cmath>     // std::tan
#include <algorithm> // std::max

namespace lol
{

template<>
inline std::string mat2::tostring() const
{
    mat2 const &p = *this;

    return format("[ %6.6f %6.6f\n", p[0][0], p[1][0]) +
           format("  %6.6f %6.6f ]\n", p[0][1], p[1][1]);
}

template<>
inline std::string mat3::tostring() const
{
    mat3 const &p = *this;

    return format("[ %6.6f %6.6f %6.6f\n", p[0][0], p[1][0], p[2][0]) +
           format("  %6.6f %6.6f %6.6f\n", p[0][1], p[1][1], p[2][1]) +
           format("  %6.6f %6.6f %6.6f ]\n", p[0][2], p[1][2], p[2][2]);
}

template<>
inline std::string mat4::tostring() const
{
    mat4 const &p = *this;

    return format("[ %6.6f %6.6f %6.6f %6.6f\n",
                  p[0][0], p[1][0], p[2][0], p[3][0]) +
           format("  %6.6f %6.6f %6.6f %6.6f\n",
                  p[0][1], p[1][1], p[2][1], p[3][1]) +
           format("  %6.6f %6.6f %6.6f %6.6f\n",
                  p[0][2], p[1][2], p[2][2], p[3][2]) +
           format("  %6.6f %6.6f %6.6f %6.6f ]\n",
                  p[0][3], p[1][3], p[2][3], p[3][3]);
}

template<typename T>
mat_t<T,3,3> mat_t<T,3,3>::scale(T x, T y, T z)
{
    mat_t<T,3,3> ret(T(1));
    ret[0][0] = x;
    ret[1][1] = y;
    ret[2][2] = z;
    return ret;
}

template<typename T>
mat_t<T,3,3> mat_t<T,3,3>::scale(T x)
{
    return scale(x, x, x);
}

template<typename T>
mat_t<T,3,3> mat_t<T,3,3>::scale(vec_t<T,3> v)
{
    return scale(v.x, v.y, v.z);
}

template<typename T>
mat_t<T,4,4> mat_t<T,4,4>::translate(T x, T y, T z)
{
    mat_t<T,4,4> ret(T(1));
    ret[3][0] = x;
    ret[3][1] = y;
    ret[3][2] = z;
    return ret;
}

template<typename T>
mat_t<T,4,4> mat_t<T,4,4>::translate(vec_t<T,3> v)
{
    return translate(v.x, v.y, v.z);
}

template<typename T>
mat_t<T,2,2> mat_t<T,2,2>::rotate(T radians)
{
    T st = sin(radians);
    T ct = cos(radians);

    mat_t<T,2,2> ret;

    ret[0][0] = ct;
    ret[0][1] = st;

    ret[1][0] = -st;
    ret[1][1] = ct;

    return ret;
}

template<typename T>
mat_t<T,3,3> mat_t<T,3,3>::rotate(T radians, T x, T y, T z)
{
    T st = sin(radians);
    T ct = cos(radians);

    T len = std::sqrt(x * x + y * y + z * z);
    T invlen = len ? T(1) / len : T(0);
    x *= invlen;
    y *= invlen;
    z *= invlen;

    T mtx = (T(1) - ct) * x;
    T mty = (T(1) - ct) * y;
    T mtz = (T(1) - ct) * z;

    mat_t<T,3,3> ret;

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

template<typename T>
mat_t<T,3,3> mat_t<T,3,3>::rotate(T radians, vec_t<T,3> v)
{
    return rotate(radians, v.x, v.y, v.z);
}

template<typename T>
mat_t<T,3,3>::mat_t(quat_t<T> const &q)
{
    T n = norm(q);

    if (!n)
    {
        for (int j = 0; j < 3; j++)
            for (int i = 0; i < 3; i++)
                (*this)[i][j] = (i == j) ? T(1) : T(0);
        return;
    }

    T s = T(2) / n;

    (*this)[0][0] = T(1) - s * (q.y * q.y + q.z * q.z);
    (*this)[0][1] = s * (q.x * q.y + q.z * q.w);
    (*this)[0][2] = s * (q.x * q.z - q.y * q.w);

    (*this)[1][0] = s * (q.x * q.y - q.z * q.w);
    (*this)[1][1] = T(1) - s * (q.z * q.z + q.x * q.x);
    (*this)[1][2] = s * (q.y * q.z + q.x * q.w);

    (*this)[2][0] = s * (q.x * q.z + q.y * q.w);
    (*this)[2][1] = s * (q.y * q.z - q.x * q.w);
    (*this)[2][2] = T(1) - s * (q.x * q.x + q.y * q.y);
}

template<typename T>
mat_t<T,4,4>::mat_t(quat_t<T> const &q)
{
    *this = mat_t<T,4,4>(mat_t<T,3,3>(q), T(1));
}

// These are only specialised for float type, but could be extended
// to anything else. I’m just not sure it’s worth it.
template<>
inline mat4 mat4::lookat(vec3 eye, vec3 center, vec3 up)
{
    vec3 v3 = normalize(eye - center);
    vec3 v1 = normalize(cross(up, v3));
    vec3 v2 = cross(v3, v1);

    return mat4(vec4(v1.x, v2.x, v3.x, 0.f),
                vec4(v1.y, v2.y, v3.y, 0.f),
                vec4(v1.z, v2.z, v3.z, 0.f),
                vec4(-dot(eye, v1), -dot(eye, v2), -dot(eye, v3), 1.f));
}

template<>
inline mat4 mat4::ortho(float left, float right, float bottom,
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

template<>
inline mat4 mat4::ortho(float width, float height,
                        float near, float far)
{
    return mat4::ortho(-0.5f * width, 0.5f * width,
                       -0.5f * height, 0.5f * height, near, far);
}

template<>
inline mat4 mat4::frustum(float left, float right, float bottom,
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

/*
 * Return a standard perspective matrix
 */

template<>
inline mat4 mat4::perspective(float fov_y, float width,
                              float height, float near, float far)
{
    using std::tan;

    float t2 = tan(fov_y * 0.5f);
    float t1 = t2 * width / height;

    return frustum(-near * t1, near * t1, -near * t2, near * t2, near, far);
}

/*
 * Return a perspective matrix with the camera location shifted to be on
 * the near plane
 */

template<>
inline mat4 mat4::shifted_perspective(float fov_y, float screen_size,
                                      float screen_ratio_yx,
                                      float near, float far)
{
    using std::tan;

    float tan_y = tan(fov_y * .5f);
    assert(tan_y > 0.000001f);
    float dist_scr = (screen_size * screen_ratio_yx * .5f) / tan_y;

    return mat4::perspective(fov_y, screen_size, screen_size * screen_ratio_yx,
                             std::max(.001f, dist_scr + near),
                             std::max(.001f, dist_scr + far)) *
           mat4::translate(.0f, .0f, -dist_scr);
}

} // namespace lol
