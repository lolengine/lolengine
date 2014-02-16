//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2010-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// Various geometry functions
// --------------------------
//

#if !defined __LOL_MATH_GEOMETRY_H__
#define __LOL_MATH_GEOMETRY_H__

#include <cmath>
#include <cstdio>
#include <algorithm>

#include <stdint.h>

#define CSG_EPSILON     0.0001

namespace lol
{

#define LOL_BOX_TYPEDEFS(tname, suffix) \
    template <typename T> struct tname; \
    typedef tname<float> suffix; \
    typedef tname<double> d##suffix; \
    typedef tname<int32_t> i##suffix; \
    typedef tname<uint32_t> u##suffix;

LOL_BOX_TYPEDEFS(Box2, box2)
LOL_BOX_TYPEDEFS(Box3, box3)

#undef LOL_BOX_TYPEDEFS

/*
 * 2D boxes
 */

template <typename T> struct Box2
{
    inline Box2()
      : A(T(0)),
        B(T(0))
    {}

    inline Box2(Vec2<T> const &a, Vec2<T> const &b)
      : A(a),
        B(b)
    {}

    inline Box2(T const &ax, T const &ay, T const &bx, T const &by)
      : A(ax, ay),
        B(bx, by)
    {}

    Box2<T> operator +(Vec2<T> const &v) const
    {
        return Box2<T>(A + v, B + v);
    }

    Box2<T> &operator +=(Vec2<T> const &v)
    {
        return *this = *this + v;
    }

    Box2<T> operator -(Vec2<T> const &v) const
    {
        return Box2<T>(A - v, B - v);
    }

    Box2<T> &operator -=(Vec2<T> const &v)
    {
        return *this = *this - v;
    }

    bool operator ==(Box2<T> const &box)
    {
        return A == box.A && B == box.B;
    }

    bool operator !=(Box2<T> const &box)
    {
        return A != box.A || B != box.B;
    }

    Vec2<T> A, B;
};

/*
 * 3D boxes
 */

template <typename T> struct Box3
{
    inline Box3()
      : A(T(0)),
        B(T(0))
    {}

    inline Box3(Vec3<T> const &a, Vec3<T> const &b)
      : A(a),
        B(b)
    {}

    inline Box3(T const &ax, T const &ay, T const &az,
                T const &bx, T const &by, T const &bz)
      : A(ax, ay, az),
        B(bx, by, bz)
    {}

    Box3<T> operator +(Vec3<T> const &v) const
    {
        return Box3<T>(A + v, B + v);
    }

    Box3<T> &operator +=(Vec3<T> const &v)
    {
        return *this = *this + v;
    }

    Box3<T> operator -(Vec3<T> const &v) const
    {
        return Box3<T>(A - v, B - v);
    }

    Box3<T> &operator -=(Vec3<T> const &v)
    {
        return *this = *this - v;
    }

    bool operator ==(Box3<T> const &box)
    {
        return A == box.A && B == box.B;
    }

    bool operator !=(Box3<T> const &box)
    {
        return A != box.A || B != box.B;
    }

    Vec3<T> A, B;
};

/*
 * Helper geometry functions
 */

static inline bool BoxIsectBox(box2 const &b1, box2 const &b2)
{
    vec2 dist = 0.5f * (b1.A - b2.A + b1.B - b2.B);
    vec2 e1 = 0.5f * (b1.B - b1.A);
    vec2 e2 = 0.5f * (b2.B - b2.A);

    return abs(dist.x) < abs(e1.x) + abs(e2.x)
        && abs(dist.y) < abs(e1.y) + abs(e2.y);
}

bool TriangleIsectTriangle(vec3 const &v00, vec3 const &v01, vec3 const &v02,
                           vec3 const &v10, vec3 const &v11, vec3 const &v12,
                           vec3 &iP00, vec3 &iP10);
bool RayIsectTriangleSide(vec3 const &v0, vec3 const &v1, vec3 const &v2,
                          vec3 const &iP0, vec3 const &iP1,
                          vec3 &iV0, int &iIdx0, vec3 &iV1, int &iIdx1);
bool RayIsectTriangle(vec3 const &rayP, vec3 const &rayD,
                      vec3 const &triV0, vec3 const &triV1, vec3 const &triV2,
                      vec3 &vi);
#define RAY_ISECT_NOTHING   0
#define RAY_ISECT_ALL       1
#define RAY_ISECT_NONE      2
#define RAY_ISECT_P0        3
#define RAY_ISECT_P1        4
int RayIsectRay(vec3 const &rayP00, vec3 const &rayP01,
                vec3 const &rayP10, vec3 const &rayP11,
                vec3 &vIsec);
vec3 ProjPointOnPlane(vec3 const &point, vec3 const &planeP, vec3 const &planeN);
bool RayIsectPlane(vec3 const &rayP0, vec3 const &rayP1,
                   vec3 const &planeP, vec3 const &planeN,
                   vec3 &vIsec, bool test_line_only = false);
bool TestPointInFrustum(const vec3& point, const mat4& frustum, vec3* result_point=nullptr);
} /* namespace lol */

#endif // __LOL_MATH_GEOMETRY_H__

