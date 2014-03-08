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
class TestEpsilon
{
private:
    static float g_test_epsilon;
public:
    static inline float Get() { return g_test_epsilon; }
    static inline void  Set(float epsilon=.0001f) { g_test_epsilon = epsilon; }
};

static inline bool TestAABBVsAABB(box2 const &b1, box2 const &b2)
{
    vec2 dist = 0.5f * (b1.A - b2.A + b1.B - b2.B);
    vec2 e1 = 0.5f * (b1.B - b1.A);
    vec2 e2 = 0.5f * (b2.B - b2.A);

    return abs(dist.x) <= abs(e1.x) + abs(e2.x)
        && abs(dist.y) <= abs(e1.y) + abs(e2.y);
}
static inline bool TestAABBVsPoint(box2 const &b1, vec2 const &p)
{
    return TestAABBVsAABB(b1, box2(p, p));
}

static inline bool TestAABBVsAABB(box3 const &b1, box3 const &b2)
{
    vec3 dist = 0.5f * (b1.A - b2.A + b1.B - b2.B);
    vec3 e1 = 0.5f * (b1.B - b1.A);
    vec3 e2 = 0.5f * (b2.B - b2.A);

    return abs(dist.x) <= abs(e1.x) + abs(e2.x)
        && abs(dist.y) <= abs(e1.y) + abs(e2.y)
        && abs(dist.z) <= abs(e1.z) + abs(e2.z);
}
static inline bool TestAABBVsPoint(box3 const &b1, vec3 const &p)
{
    return TestAABBVsAABB(b1, box3(p, p));
}

bool TestTriangleVsTriangle(vec3 const &v00, vec3 const &v01, vec3 const &v02,
                           vec3 const &v10, vec3 const &v11, vec3 const &v12,
                           vec3 &ip00, vec3 &ip10);
bool TestRayVsTriangleSide(vec3 const &v0, vec3 const &v1, vec3 const &v2,
                          vec3 const &ip0, vec3 const &ip1,
                          vec3 &iV0, int &iIdx0, vec3 &iV1, int &iIdx1);
bool TestRayVsTriangle(vec3 const &ray_point, vec3 const &ray_dir,
                      vec3 const &tri_p0, vec3 const &tri_p1, vec3 const &tri_p2,
                      vec3 &vi);

struct RayIntersect
{
    enum Value
    {
        Nothing = 0,
        All,
        None,
        P0,
        P1,

        MAX
    }
    m_value;

    inline RayIntersect() : m_value(Nothing) {}
    inline RayIntersect(Value v) : m_value(v) {}
    inline RayIntersect(int v) : m_value((Value)v) {}
    inline operator Value() { return m_value; }
};
#define RAY_ISECT_NOTHING   0
#define RAY_ISECT_ALL       1
#define RAY_ISECT_NONE      2
#define RAY_ISECT_P0        3
#define RAY_ISECT_P1        4
int TestRayVsRay(vec3 const &ray_p00, vec3 const &ray_p01,
                 vec3 const &ray_p10, vec3 const &ray_p11,
                 vec3 &isec_point);
bool TestRayVsPlane(vec3 const &ray_p0, vec3 const &ray_p1,
                   vec3 const &plane_point, vec3 const &plane_normal,
                   vec3 &isec_point, bool test_line_only = false);
bool TestPointVsFrustum(const vec3& point, const mat4& frustum, vec3* result_point=nullptr);

//Project points functions
//Plane
vec3 ProjectPointOnPlane(vec3 const &proj_point, vec3 const &plane_point, vec3 const &plane_normal);
//Line
vec3 ProjectPointOnRay(vec3 const &proj_point, vec3 const &ray_point, vec3 const &ray_dir);
//Point dist to plane
float PointDistToPlane(vec3 const &proj_point, vec3 const &plane_point, vec3 const &plane_normal);
} /* namespace lol */

#endif // __LOL_MATH_GEOMETRY_H__

