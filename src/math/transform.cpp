//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{

template<> quat quat::rotate(float degrees, vec3 const &v)
{
    float half_angle = radians(degrees) * 0.5f;

    vec3 tmp = normalize(v) * sin(half_angle);

    return quat(cos(half_angle), tmp.x, tmp.y, tmp.z);
}

template<> quat quat::rotate(float degrees, float x, float y, float z)
{
    return quat::rotate(degrees, vec3(x, y, z));
}

template<> quat quat::rotate(vec3 const &src, vec3 const &dst)
{
    /* Algorithm directly taken from Sam Hocevar's article "Quaternion from
     * two vectors: the final version".
     * http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final */
    float magnitude = lol::sqrt(sqlength(src) * sqlength(dst));
    float real_part = magnitude + dot(src, dst);
    vec3 w;

    if (real_part < 1.e-6f * magnitude)
    {
        /* If src and dst are exactly opposite, rotate 180 degrees
         * around an arbitrary orthogonal axis. Axis normalisation
         * can happen later, when we normalise the quaternion. */
        real_part = 0.0f;
        w = abs(src.x) > abs(src.z) ? vec3(-src.y, src.x, 0.f)
                                    : vec3(0.f, -src.z, src.y);
    }
    else
    {
        /* Otherwise, build quaternion the standard way. */
        w = cross(src, dst);
    }

    return normalize(quat(real_part, w.x, w.y, w.z));
}

template<> quat slerp(quat const &qa, quat const &qb, float f)
{
    float const magnitude = lol::sqrt(sqlength(qa) * sqlength(qb));
    float const product = lol::dot(qa, qb) / magnitude;

    /* If quaternions are equal or opposite, there is no need
     * to slerp anything, just return qa. */
    if (std::abs(product) >= 1.0f)
        return qa;

    float const sign = (product < 0.0f) ? -1.0f : 1.0f;
    float const theta = lol::acos(sign * product);
    float const s1 = lol::sin(sign * f * theta);
    float const s0 = lol::sin((1.0f - f) * theta);

    /* This is the same as 1/sin(theta) */
    float const d = 1.0f / lol::sqrt(1.f - product * product);

    return qa * (s0 * d) + qb * (s1 * d);
}

static inline vec3 quat_toeuler_generic(quat const &q, int i, int j, int k)
{
    float n = norm(q);

    if (!n)
        return vec3::zero;

    /* (2 + i - j) % 3 means x-y-z direct order; otherwise indirect */
    float const sign = ((2 + i - j) % 3) ? 1.f : -1.f;

    vec3 ret;

    /* k == i means X-Y-X style Euler angles; otherwise we’re
     * actually handling X-Y-Z style Tait-Bryan angles. */
    if (k == i)
    {
        k = 3 - i - j;

        ret[0] = atan2(q[1 + i] * q[1 + j] + sign * (q.w * q[1 + k]),
                       q.w * q[1 + j] - sign * (q[1 + i] * q[1 + k]));
        ret[1] = acos(2.f * (sq(q.w) + sq(q[1 + i])) - 1.f);
        ret[2] = atan2(q[1 + i] * q[1 + j] - sign * (q.w * q[1 + k]),
                       q.w * q[1 + j] + sign * (q[1 + i] * q[1 + k]));
    }
    else
    {
        ret[0] = atan2(2.f * (q.w * q[1 + i] - sign * (q[1 + j] * q[1 + k])),
                       1.f - 2.f * (sq(q[1 + i]) + sq(q[1 + j])));
        ret[1] = asin(2.f * (q.w * q[1 + j] + sign * (q[1 + i] * q[1 + k])));
        ret[2] = atan2(2.f * (q.w * q[1 + k] - sign * (q[1 + j] * q[1 + i])),
                       1.f - 2.f * (sq(q[1 + k]) + sq(q[1 + j])));
    }

    return degrees(ret / n);
}

static inline mat3 mat3_fromeuler_generic(vec3 const &v, int i, int j, int k)
{
    mat3 ret;

    vec3 const w = radians(v);
    float const s0 = sin(w[0]), c0 = cos(w[0]);
    float const s1 = sin(w[1]), c1 = cos(w[1]);
    float const s2 = sin(w[2]), c2 = cos(w[2]);

    /* (2 + i - j) % 3 means x-y-z direct order; otherwise indirect */
    float const sign = ((2 + i - j) % 3) ? 1.f : -1.f;

    /* k == i means X-Y-X style Euler angles; otherwise we’re
     * actually handling X-Y-Z style Tait-Bryan angles. */
    if (k == i)
    {
        k = 3 - i - j;

        ret[i][i] =   c1;
        ret[i][j] =   s0 * s1;
        ret[i][k] = - sign * (c0 * s1);

        ret[j][i] =   s1 * s2;
        ret[j][j] =   c0 * c2 - s0 * c1 * s2;
        ret[j][k] =   sign * (s0 * c2 + c0 * c1 * s2);

        ret[k][i] =   sign * (s1 * c2);
        ret[k][j] = - sign * (c0 * s2 + s0 * c1 * c2);
        ret[k][k] = - s0 * s2 + c0 * c1 * c2;
    }
    else
    {
        ret[i][i] =   c1 * c2;
        ret[i][j] =   sign * (c0 * s2) + s0 * s1 * c2;
        ret[i][k] =   s0 * s2 - sign * (c0 * s1 * c2);

        ret[j][i] = - sign * (c1 * s2);
        ret[j][j] =   c0 * c2 - sign * (s0 * s1 * s2);
        ret[j][k] =   sign * (s0 * c2) + c0 * s1 * s2;

        ret[k][i] =   sign * s1;
        ret[k][j] = - sign * (s0 * c1);
        ret[k][k] =   c0 * c1;
    }

    return ret;
}

static inline quat quat_fromeuler_generic(vec3 const &v, int i, int j, int k)
{
    vec3 const half_angles = radians(v * 0.5f);
    float const s0 = sin(half_angles[0]), c0 = cos(half_angles[0]);
    float const s1 = sin(half_angles[1]), c1 = cos(half_angles[1]);
    float const s2 = sin(half_angles[2]), c2 = cos(half_angles[2]);

    quat ret;

    /* (2 + i - j) % 3 means x-y-z direct order; otherwise indirect */
    float const sign = ((2 + i - j) % 3) ? 1.f : -1.f;

    /* k == i means X-Y-X style Euler angles; otherwise we’re
     * actually handling X-Y-Z style Tait-Bryan angles. */
    if (k == i)
    {
        k = 3 - i - j;

        ret[0] =     c1 * (c0 * c2 - s0 * s2);
        ret[1 + i] = c1 * (c0 * s2 + s0 * c2);
        ret[1 + j] = s1 * (c0 * c2 + s0 * s2);
        ret[1 + k] = sign * (s1 * (s0 * c2 - c0 * s2));
    }
    else
    {
        ret[0] =     c0 * c1 * c2 - sign * (s0 * s1 * s2);
        ret[1 + i] = s0 * c1 * c2 + sign * (c0 * s1 * s2);
        ret[1 + j] = c0 * s1 * c2 - sign * (s0 * c1 * s2);
        ret[1 + k] = c0 * c1 * s2 + sign * (s0 * s1 * c2);
    }

    return ret;
}

#define DEFINE_GENERIC_EULER_CONVERSIONS(a1, a2, a3) \
    DEFINE_GENERIC_EULER_CONVERSIONS_INNER(a1, a2, a3, a1##a2##a3) \

#define DEFINE_GENERIC_EULER_CONVERSIONS_INNER(a1, a2, a3, name) \
    /* Create quaternions from Euler angles */ \
    template<> quat quat::fromeuler_##name(vec3 const &v) \
    { \
        int x = 0, y = 1, z = 2; UNUSED(x, y, z); \
        return quat_fromeuler_generic(v, a1, a2, a3); \
    } \
    \
    template<> quat quat::fromeuler_##name(float phi, float theta, float psi) \
    { \
        return quat::fromeuler_##name(vec3(phi, theta, psi)); \
    } \
    \
    /* Create 3×3 matrices from Euler angles */ \
    template<> mat3 mat3::fromeuler_##name(vec3 const &v) \
    { \
        int x = 0, y = 1, z = 2; UNUSED(x, y, z); \
        return mat3_fromeuler_generic(v, a1, a2, a3); \
    } \
    \
    template<> mat3 mat3::fromeuler_##name(float phi, float theta, float psi) \
    { \
        return mat3::fromeuler_##name(vec3(phi, theta, psi)); \
    } \
    \
    /* Create 4×4 matrices from Euler angles */ \
    template<> mat4 mat4::fromeuler_##name(vec3 const &v) \
    { \
        int x = 0, y = 1, z = 2; UNUSED(x, y, z); \
        return mat4(mat3_fromeuler_generic(v, a1, a2, a3), 1.f); \
    } \
    \
    template<> mat4 mat4::fromeuler_##name(float phi, float theta, float psi) \
    { \
        return mat4::fromeuler_##name(vec3(phi, theta, psi)); \
    } \
    \
    /* Retrieve Euler angles from a quaternion */ \
    template<> vec3 vec3::toeuler_##name(quat const &q) \
    { \
        int x = 0, y = 1, z = 2; UNUSED(x, y, z); \
        return quat_toeuler_generic(q, a1, a2, a3); \
    }

DEFINE_GENERIC_EULER_CONVERSIONS(x, y, x)
DEFINE_GENERIC_EULER_CONVERSIONS(x, z, x)
DEFINE_GENERIC_EULER_CONVERSIONS(y, x, y)
DEFINE_GENERIC_EULER_CONVERSIONS(y, z, y)
DEFINE_GENERIC_EULER_CONVERSIONS(z, x, z)
DEFINE_GENERIC_EULER_CONVERSIONS(z, y, z)

DEFINE_GENERIC_EULER_CONVERSIONS(x, y, z)
DEFINE_GENERIC_EULER_CONVERSIONS(x, z, y)
DEFINE_GENERIC_EULER_CONVERSIONS(y, x, z)
DEFINE_GENERIC_EULER_CONVERSIONS(y, z, x)
DEFINE_GENERIC_EULER_CONVERSIONS(z, x, y)
DEFINE_GENERIC_EULER_CONVERSIONS(z, y, x)

#undef DEFINE_GENERIC_EULER_CONVERSIONS
#undef DEFINE_GENERIC_EULER_CONVERSIONS_INNER

} /* namespace lol */

