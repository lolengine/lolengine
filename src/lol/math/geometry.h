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

struct Box2D
{
    inline Box2D()
      : A(0.f, 0.f),
        B(0.f, 0.f)
    {}

    inline Box2D(vec2 a, vec2 b)
      : A(a),
        B(b)
    {}

    vec2 A, B;
};

struct Box3D
{
    inline Box3D()
      : A(0.f, 0.f, 0.f),
        B(0.f, 0.f, 0.f)
    {}

    inline Box3D(vec3 a, vec3 b)
      : A(a),
        B(b)
    {}

    vec3 A, B;
};

static inline bool BoxIsectBox(Box2D const &b1, Box2D const &b2)
{
    vec2 c1 = 0.5f * (b1.A + b1.B);
    vec2 c2 = 0.5f * (b2.A + b2.B);
    vec2 e1 = 0.5f * (b1.B - b1.A);
    vec2 e2 = 0.5f * (b2.B - b2.A);

    return abs(c1.x - c2.x) < abs(e1.x) + abs(e2.x)
        || abs(c1.y - c2.y) < abs(e1.y) + abs(e2.y);
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

} /* namespace lol */

#endif // __LOL_MATH_GEOMETRY_H__

