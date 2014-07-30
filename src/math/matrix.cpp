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

#include <lol/main.h>

namespace lol
{

template<> mat3 mat3::scale(float x)
{
    mat3 ret(1.0f);

    ret[0][0] = x;
    ret[1][1] = x;
    ret[2][2] = x;

    return ret;
}

template<> mat3 mat3::scale(float x, float y, float z)
{
    mat3 ret(1.0f);

    ret[0][0] = x;
    ret[1][1] = y;
    ret[2][2] = z;

    return ret;
}

template<> mat3 mat3::scale(vec3 v)
{
    return scale(v.x, v.y, v.z);
}

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

template<> mat2 mat2::rotate(float degrees)
{
    float st = sin(radians(degrees));
    float ct = cos(radians(degrees));

    mat2 ret;

    ret[0][0] = ct;
    ret[0][1] = st;

    ret[1][0] = -st;
    ret[1][1] = ct;

    return ret;
}

template<> mat3 mat3::rotate(float degrees, float x, float y, float z)
{
    float st = sin(radians(degrees));
    float ct = cos(radians(degrees));

    float len = std::sqrt(x * x + y * y + z * z);
    float invlen = len ? 1.0f / len : 0.0f;
    x *= invlen;
    y *= invlen;
    z *= invlen;

    float mtx = (1.0f - ct) * x;
    float mty = (1.0f - ct) * y;
    float mtz = (1.0f - ct) * z;

    mat3 ret;

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

template<> mat3 mat3::rotate(float degrees, vec3 v)
{
    return rotate(degrees, v.x, v.y, v.z);
}

template<> mat3::mat_t(quat const &q)
{
    float n = norm(q);

    if (!n)
    {
        for (int j = 0; j < 3; j++)
            for (int i = 0; i < 3; i++)
                (*this)[i][j] = (i == j) ? 1.f : 0.f;
        return;
    }

    float s = 2.0f / n;

    (*this)[0][0] = 1.0f - s * (q.y * q.y + q.z * q.z);
    (*this)[0][1] = s * (q.x * q.y + q.z * q.w);
    (*this)[0][2] = s * (q.x * q.z - q.y * q.w);

    (*this)[1][0] = s * (q.x * q.y - q.z * q.w);
    (*this)[1][1] = 1.0f - s * (q.z * q.z + q.x * q.x);
    (*this)[1][2] = s * (q.y * q.z + q.x * q.w);

    (*this)[2][0] = s * (q.x * q.z + q.y * q.w);
    (*this)[2][1] = s * (q.y * q.z - q.x * q.w);
    (*this)[2][2] = 1.0f - s * (q.x * q.x + q.y * q.y);
}

template<> mat4::mat_t(quat const &q)
{
    *this = mat4(mat3(q), 1.f);
}

template<> mat4 mat4::lookat(vec3 eye, vec3 center, vec3 up)
{
    vec3 v3 = normalize(eye - center);
    vec3 v1 = normalize(cross(up, v3));
    vec3 v2 = cross(v3, v1);

    return mat4(vec4(v1.x, v2.x, v3.x, 0.f),
                vec4(v1.y, v2.y, v3.y, 0.f),
                vec4(v1.z, v2.z, v3.z, 0.f),
                vec4(-dot(eye, v1), -dot(eye, v2), -dot(eye, v3), 1.f));
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

template<> mat4 mat4::ortho(float width, float height,
                            float near, float far)
{
    return mat4::ortho(-0.5f * width, 0.5f * width,
                       -0.5f * height, 0.5f * height, near, far);
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

//Returns a standard perspective matrix
template<> mat4 mat4::perspective(float fov_y, float width,
                                  float height, float near, float far)
{
    float t2 = lol::tan(radians(fov_y) * 0.5f);
    float t1 = t2 * width / height;

    return frustum(-near * t1, near * t1, -near * t2, near * t2, near, far);
}

//Returns a perspective matrix with the camera location shifted to be on the near plane
template<> mat4 mat4::shifted_perspective(float fov_y, float screen_size,
                                          float screen_ratio_yx, float near, float far)
{
    float tan_y = tanf(radians(fov_y) * .5f);
    ASSERT(tan_y > 0.000001f);
    float dist_scr = (screen_size * screen_ratio_yx * .5f) / tan_y;

    return mat4::perspective(fov_y, screen_size, screen_size * screen_ratio_yx,
                             max(.001f, dist_scr + near),
                             max(.001f, dist_scr + far)) *
           mat4::translate(.0f, .0f, -dist_scr);
}

} /* namespace lol */

