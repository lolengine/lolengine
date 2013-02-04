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

