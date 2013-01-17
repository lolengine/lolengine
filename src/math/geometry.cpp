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

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined _XBOX
#   define _USE_MATH_DEFINES /* for M_PI */
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#elif defined _WIN32
#   define _USE_MATH_DEFINES /* for M_PI */
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

#include <cstdlib> /* free() */
#include <cstring> /* strdup() */

#include <ostream> /* std::ostream */

#include "core.h"

using namespace std;

namespace lol
{
    //Projects Point on Plane : Normal must be given normalized. returns point on plane.
    vec3 ProjPointOnPlane(vec3 const &point, vec3 const &planeP, vec3 const &planeN)
    {
        vec3 o2p = point - planeP;
        float d = -dot(o2p, planeN);
        return point + d * planeN;
    }

    //gets the dist from a Point to a Plane : Normal must be given normalized. returns distance.
    float PointDistToPlane(vec3 const &point, vec3 const &planeP, vec3 const &planeN)
    {
        vec3 o2p = point - planeP;
        return abs(dot(o2p, planeN));
    }

    // Line/triangle : sets vIsec as the intersection point & return true if ok.
    bool RayIsectTriangle(vec3 const &rayP, vec3 const &rayD, 
                                 vec3 const &triV0, vec3 const &triV1, vec3 const &triV2,
                                 vec3 &vIsec)
    {
        vec3 v01, v02, h, v0P, q;
        float a, f, triU, triV;
    
        //
        v01 = triV1 - triV0;
        v02 = triV2 - triV0;

        h = cross(rayD, v02);
        a = dot(v01, h);

        //rayDir is coplanar to the triangle, exit.
        if (a > -CSG_EPSILON && a < CSG_EPSILON)
            return false;

        f = 1 / a;
        v0P = rayP - triV0;
        triU = f * (dot(v0P, h));

        //point is supposed to have an U on the segment v01
        if (triU < -CSG_EPSILON || triU > 1.0)
            return false;

        q = cross(v0P, v01);
        triV = f * dot(rayD, q);

        //point is not in the triangle
        if (triV < -CSG_EPSILON || triU + triV > 1.0)
            return false;

        // at this stage we can compute t to find out where
        // the intersection point is on the line
        float t = f * dot(v02, q);

        if (t > CSG_EPSILON) // ray intersection
        {
            vIsec = triV0 + v01 * triU + v02 * triV;
            return true;
        }
        else // this means that there is a line intersection
             // but not a ray intersection
             return false;
    }

    // Triangle/Triangle
    bool TriangleIsectTriangle(vec3 const &v00, vec3 const &v01, vec3 const &v02, //triangle 0
                                      vec3 const &v10, vec3 const &v11, vec3 const &v12, //triangle 1
                                      vec3 &iP00, vec3 &iP10) //triangle intersection, iPx means gives the actual intersection points.
    {
        vec3 isec[2] = { vec3(0, 0, 0), vec3(0, 0, 0) };
        vec3 triV[6] = { v00, v01, v02,
                         v10, v11, v12 };
        vec3 triD[6] = { v01 - v00, v02 - v01, v00 - v02,
                         v11 - v10, v12 - v11, v10 - v12 };
        int isecIdx = 0;
        vec3 vIsec(0);

        //Check the normal before doing any other calculations
        vec3 plane_norm[2] = { cross(normalize(triD[0]), normalize(triD[1])),
                               cross(normalize(triD[3]), normalize(triD[4])) };
        if (abs(dot(plane_norm[0], plane_norm[1])) == 1.0f)
            return false;

#if 0
        //Precheck to verify if two point of one of the tri-A are on tri-B, and vice versa.
        int zero_dist[2] = { 0, 0 };
        for (int i = 0; i < 3; i++)
        {
            if (PointDistToPlane(triV[i], triV[3], plane_norm[1]) < CSG_EPSILON)
                zero_dist[0]++;
            if (PointDistToPlane(triV[i + 3], triV[0], plane_norm[0]) < CSG_EPSILON)
                zero_dist[1]++;
        }

        if (zero_dist[0] >= 2 || zero_dist[0] >= 2)
            return false;
#endif

        //We first need to find two points intersecting, no matter on which triangle.
        for (int i = 0; i < 2 && isecIdx < 2; i++)
        {
            int found_isec = -1;
            for (int j = 0; j < 3 && isecIdx < 2; j++)
            {
                int pIdx = j + i * 3;
                int tIdx = (1 - i) * 3;
                if (RayIsectTriangle(triV[pIdx], triD[pIdx],
                                     triV[tIdx + 0], triV[tIdx + 1], triV[tIdx + 2],
                                     isec[isecIdx]))
                {
#if 1 //if the point is near one the given entry, consider it as being the same point.
                    for (int k = 0; k < 6; k++)
                    {
                        if (length(isec[isecIdx] - triV[k]) < CSG_EPSILON)
                        {
                            isec[isecIdx] = triV[k];
                            break;
                        }
                    }
#endif

                    //Automatic pass on the first intersection
                    if (isecIdx == 0 ||
                        //If we have already found an intersection, pass if it's on this triangle.
                        /*found_isec == i ||*/
                        //if it's the second intersection, we need it to be different from the first one.
                        (isecIdx == 1 && length(isec[0] - isec[1]) > CSG_EPSILON))
                    {
                        found_isec = i;
                        isecIdx++;
                    }
                }
            }
        }

        if (isecIdx >= 2)
        {
            iP00 = isec[0];
            iP10 = isec[1];
            return true;
        }
        return false;
    }

    //Ray/Line : returns one of the RAY_ISECT_* defines.
    int RayIsectRay(vec3 const &rayP00, vec3 const &rayP01, 
                           vec3 const &rayP10, vec3 const &rayP11,
                           vec3 &vIsec)
    {
        vec3 rayD0 = rayP01 - rayP00;
        float rayS0 = length(rayD0);
        vec3 rayD0N = normalize(rayD0);

        vec3 rayD1 = rayP11 - rayP10;
        float rayS1 = length(rayD1);
        vec3 rayD1N = normalize(rayD1);

        vec3 rayP0P1 = rayP10 - rayP00;
        vec3 c01 = cross(rayD0N, rayD1N);
        float crs01S = length(c01);

        if (crs01S > -CSG_EPSILON && crs01S < CSG_EPSILON)
            return 0;

        mat3 m0 = mat3(rayP0P1, rayD1N, c01);
        float t0 = determinant(m0) / (crs01S * crs01S);
        vec3 isec0 = rayP00 + rayD0N * t0;

        mat3 m1 = mat3(rayP0P1, rayD0N, c01);
        float t1 = determinant(m1) / (crs01S * crs01S);
        vec3 isec1 = rayP10 + rayD1N * t1;

        if (sqlength(isec0 - isec1) < CSG_EPSILON) //ray intersection
        {
            vIsec = (isec0 + isec0) * .5f;
            float d0 = (length(rayP01 - vIsec) < CSG_EPSILON || length(rayP00 - vIsec) < CSG_EPSILON)?
                        (-1.0f):
                        (dot(rayP00 - vIsec, rayP01 - vIsec));
            float d1 = (length(rayP10 - vIsec) < CSG_EPSILON || length(rayP11 - vIsec) < CSG_EPSILON)?
                        (-1.0f):
                        (dot(rayP10 - vIsec, rayP11 - vIsec));

            //if the dot is negative, your point is in each ray, so say OK.
            if (d0 < .0f && d1 < .0f)
                return RAY_ISECT_ALL;
            if (d0 < .0f)
                return RAY_ISECT_P0;
            if (d1 < .0f)
                return RAY_ISECT_P1;
            return RAY_ISECT_NONE;
        }
        else // this means that there is a line intersection
             // but not a ray intersection
             return RAY_ISECT_NOTHING;
    }

    //Ray/Plane : Normal must be given normalized. returns 1 if succeeded.
    bool RayIsectPlane(vec3 const &rayP0, vec3 const &rayP1,
                              vec3 const &planeP, vec3 const &planeN,
                              vec3 &vIsec, bool test_line_only)
    {
        vec3 ray_dir = rayP1 - rayP0;
        float d = dot(ray_dir, planeN);

        if (d > -CSG_EPSILON && d < CSG_EPSILON)
            return false;

        vec3 o2p1 = rayP1 - planeP;
        vec3 o2p0 = rayP0 - planeP;

        if (!test_line_only)
        {
            d = dot(o2p1, planeN);
            d *= dot(o2p0, planeN);

            //point are on the same side, so ray can intersect.
            if (d > .0f)
                return false;
        }

        float t = (dot(ProjPointOnPlane(rayP0, planeP, planeN) - rayP0, planeN)) / dot(ray_dir, planeN);

        if (!test_line_only && (t < -CSG_EPSILON || t > 1.0f))
            return false;

        vIsec = rayP0 + t * ray_dir;
        return true;
    }

    //used to find the intersecting points between a triangle side and a coplanar line.
    bool RayIsectTriangleSide(vec3 const &v0, vec3 const &v1, vec3 const &v2,
                                     vec3 const &iP0, vec3 const &iP1,
                                     vec3 &iV0, int &iIdx0, vec3 &iV1, int &iIdx1)
    {
        vec3 isecV[2] = { vec3(.0f), vec3(.0f) };
        int isecI[2] = { -1, -1 };

        vec3 triV[3] = { v0, v1, v2 };
        int isecIdx = 0;
        vec3 vIsec(0);

        //Two points given, so we test each triangle side to find the intersect
        isecIdx = 0;
        for (int j = 0; j < 3 && isecIdx < 2; j++)
        {
            int Result = RayIsectRay(triV[j], triV[(j + 1) % 3], iP0, iP1, vIsec);
            if (Result == RAY_ISECT_P0 || Result == RAY_ISECT_ALL)
            {
                isecV[isecIdx] = vIsec;
                isecI[isecIdx] = j;
                isecIdx++;
            }
        }

        if (isecIdx < 2)
            return false;

        //We send the infos back to the parent.
        //TODO : that can be better than this horrible thing.
        iV0 = isecV[0];
        iV1 = isecV[1];
        iIdx0 = isecI[0];
        iIdx1 = isecI[1];
        return true;
    }

} /* namespace lol */

