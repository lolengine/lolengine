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

    Box2<T> operator *(Vec2<T> const &v) const
    {
        return Box2<T>(A * v, B * v);
    }

    Box2<T> &operator *=(Vec2<T> const &v)
    {
        return *this = *this * v;
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

    Box3<T> operator *(Vec3<T> const &v) const
    {
        return Box3<T>(A * v, B * v);
    }

    Box3<T> &operator *=(Vec3<T> const &v)
    {
        return *this = *this * v;
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
    float           m_epsilon;
    float           m_value;
public:
    TestEpsilon()   { m_value = 0.f; m_epsilon = .0001f; }
    static float Get();
    static void Set(float epsilon=.0001f);
    static const TestEpsilon& F(float value);
private:
    float Minus() const;
    float Plus()  const;
public:
    bool operator==(float value) const;
    bool operator!=(float value) const;
    bool operator<(float value)  const;
    bool operator<=(float value) const;
    bool operator>(float value)  const;
    bool operator>=(float value) const;
};
bool operator==(float value, const TestEpsilon& epsilon);
bool operator!=(float value, const TestEpsilon& epsilon);
bool operator<(float value, const TestEpsilon& epsilon);
bool operator<=(float value, const TestEpsilon& epsilon);
bool operator>(float value, const TestEpsilon& epsilon);
bool operator>=(float value, const TestEpsilon& epsilon);

//--
static inline bool TestAABBVsAABB(box2 const &b1, box2 const &b2)
{
    vec2 c  = 0.5f * ((b1.A + b1.B) - (b2.A + b2.B));
    vec2 e1 = 0.5f *  (b1.B - b1.A);
    vec2 e2 = 0.5f *  (b2.B - b2.A);

    return abs(c.x) <= e1.x + e2.x
        && abs(c.y) <= e1.y + e2.y;
}
static inline bool TestAABBVsPoint(box2 const &b1, vec2 const &p)
{
    return TestAABBVsAABB(b1, box2(p, p));
}

static inline bool TestAABBVsAABB(box3 const &b1, box3 const &b2)
{
    vec3 c =  0.5f * ((b1.A + b1.B) - (b2.A + b2.B));
    vec3 e1 = 0.5f *  (b1.B - b1.A);
    vec3 e2 = 0.5f *  (b2.B - b2.A);

    return abs(c.x) <= e1.x + e2.x
        && abs(c.y) <= e1.y + e2.y
        && abs(c.z) <= e1.z + e2.z;
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
                 vec3 &isec_p);
bool TestPointVsFrustum(const vec3& point, const mat4& frustum, vec3* result_point=nullptr);

//Ray/Plane : Normal must be given normalized. returns 1 if succeeded.
template <typename TV>
bool TestRayVsPlane(const TV &ray_p0,  const TV &ray_p1,
                    const TV &plane_p, const TV &plane_n,
                          TV &isec_p,  bool test_line_only=false)
{
    TV ray_dir = ray_p1 - ray_p0;
    float d = dot(ray_dir, plane_n);

    if (d > -TestEpsilon::Get() && d < TestEpsilon::Get())
        return false;

    TV o2p1 = ray_p1 - plane_p;
    TV o2p0 = ray_p0 - plane_p;

    if (!test_line_only)
    {
        d = dot(o2p1, plane_n);
        d *= dot(o2p0, plane_n);

        //point are on the same side, so ray can intersect.
        if (d > .0f)
            return false;
    }

    float t = (dot(ProjectPointOnPlane(ray_p0, plane_p, plane_n) - ray_p0, plane_n)) / dot(ray_dir, plane_n);

    if (!test_line_only && (t < -TestEpsilon::Get() || t > 1.f + TestEpsilon::Get()))
        return false;

    isec_p = ray_p0 + t * ray_dir;
    return true;
}

/* A safe enum for Primitive edge face. */
struct PlaneIntersection
{
    enum Value
    {
        Invalid=-1,
        Back,
        Front,
        Plane,

        MAX
    }
    m_value;

    inline PlaneIntersection() : m_value(Invalid) {}
    inline PlaneIntersection(Value v) : m_value(v) {}
    inline PlaneIntersection(int v) : m_value((Value)v) {}
    inline operator Value() { return m_value; }
};

//Point/Plane : Normal must be given normalized.
template <typename TV>
PlaneIntersection TestPointVsPlane(const TV &point, const TV &plane_p, const TV &plane_n)
{
    float d = dot(normalize(point - plane_p), plane_n);
    if (d > TestEpsilon::Get())
        return PlaneIntersection::Front;
    else if (d < -TestEpsilon::Get())
        return PlaneIntersection::Back;
    else
        return PlaneIntersection::Plane;
}

/* Project point on plane */
template <typename TV>
TV ProjectPointOnPlane(TV const &p, TV const &origin, TV const &normal)
{
    return p - dot(p - origin, normal) * normal;
}

/* Project point on line */
template <typename TV>
TV ProjectPointOnRay(TV const &p, TV const &origin, TV const &direction)
{
    return origin + direction * dot(p - origin, direction);
}

/* Distance from point to plane */
template <typename TV>
float PointDistToPlane(TV const &p, TV const &origin, TV const &normal)
{
    return abs(dot(p - origin, normal));
}

/* Distance from point to segment */
template <typename TV>
float PointDistToSegment(TV const &p, TV const &a, TV const &b)
{
    float d2 = sqlength(b - a);
    float u = d2 ? dot(p - a, b - a) / d2 : 0.0f;
    return distance(p, mix(a, b, clamp(u, 0.0f, 1.0f)));
}

} /* namespace lol */

#endif // __LOL_MATH_GEOMETRY_H__

