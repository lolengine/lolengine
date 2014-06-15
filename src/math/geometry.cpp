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

#include <cstdlib> /* free() */
#include <cstring> /* strdup() */

#include <ostream> /* std::ostream */

#include "core.h"

using namespace std;

namespace lol
{
    //Test epsilon stuff
    TestEpsilon g_test_epsilon;
    float TestEpsilon::Get()                                    { return g_test_epsilon.m_epsilon; }
    void  TestEpsilon::Set(float epsilon)                       { g_test_epsilon.m_epsilon = lol::max(epsilon, .0f); }
    const TestEpsilon& TestEpsilon::F(float value)              { g_test_epsilon.m_value = value; return g_test_epsilon; }
    float TestEpsilon::Minus()const                             { return m_value - m_epsilon; }
    float TestEpsilon::Plus() const                             { return m_value + m_epsilon; }
    bool  TestEpsilon::operator==(float value)const             { return (Minus() <= value   && value  <= Plus()); }
    bool  TestEpsilon::operator!=(float value)const             { return (value   <  Minus() || Plus() <  value); }
    bool  TestEpsilon::operator<(float value) const             { return (Plus()  <  value); }
    bool  TestEpsilon::operator<=(float value)const             { return (Minus() <= value); }
    bool  TestEpsilon::operator>(float value) const             { return (Minus() >  value); }
    bool  TestEpsilon::operator>=(float value)const             { return (Plus()  >= value); }
    bool operator==(float value, const TestEpsilon& epsilon)    { return epsilon == value; }
    bool operator!=(float value, const TestEpsilon& epsilon)    { return epsilon != value; }
    bool operator<(float value, const TestEpsilon& epsilon)     { return epsilon >  value; }
    bool operator<=(float value, const TestEpsilon& epsilon)    { return epsilon >= value; }
    bool operator>(float value, const TestEpsilon& epsilon)     { return epsilon <  value; }
    bool operator>=(float value, const TestEpsilon& epsilon)    { return epsilon <= value; }

    // Line/triangle : sets isec_p as the intersection point & return true if ok.
    bool TestRayVsTriangle(vec3 const &ray_point, vec3 const &ray_dir,
                           vec3 const &tri_p0, vec3 const &tri_p1, vec3 const &tri_p2,
                           vec3 &isec_p)
    {
        vec3 v01, v02, h, v0P, q;
        float a, f, triU, triV;

        //
        v01 = tri_p1 - tri_p0;
        v02 = tri_p2 - tri_p0;

        h = cross(ray_dir, v02);
        a = dot(v01, h);

        //rayDir is coplanar to the triangle, exit.
        if (a > -TestEpsilon::Get() && a < TestEpsilon::Get())
            return false;

        f = 1 / a;
        v0P = ray_point - tri_p0;
        triU = f * (dot(v0P, h));

        //point is supposed to have an U on the segment v01
        if (triU < -TestEpsilon::Get() || triU > 1.0)
            return false;

        q = cross(v0P, v01);
        triV = f * dot(ray_dir, q);

        //point is not in the triangle
        if (triV < -TestEpsilon::Get() || triU + triV > 1.0)
            return false;

        // at this stage we can compute t to find out where
        // the intersection point is on the line
        float t = f * dot(v02, q);

        if (t > TestEpsilon::Get()) // ray intersection
        {
            isec_p = tri_p0 + v01 * triU + v02 * triV;
            return true;
        }
        else // this means that there is a line intersection
             // but not a ray intersection
             return false;
    }

    // Triangle/Triangle
    bool TestTriangleVsTriangle(vec3 const &v00, vec3 const &v01, vec3 const &v02, //triangle 0
                                      vec3 const &v10, vec3 const &v11, vec3 const &v12, //triangle 1
                                      vec3 &ip00, vec3 &ip10) //triangle intersection, iPx means gives the actual intersection points.
    {
        vec3 isec[2] = { vec3(0, 0, 0), vec3(0, 0, 0) };
        vec3 triV[6] = { v00, v01, v02,
                         v10, v11, v12 };
        vec3 triD[6] = { v01 - v00, v02 - v01, v00 - v02,
                         v11 - v10, v12 - v11, v10 - v12 };
        int isecIdx = 0;
        vec3 isec_p(0);

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
            if (PointDistToPlane(triV[i], triV[3], plane_norm[1]) < TestEpsilon::Get())
                zero_dist[0]++;
            if (PointDistToPlane(triV[i + 3], triV[0], plane_norm[0]) < TestEpsilon::Get())
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
                if (TestRayVsTriangle(triV[pIdx], triD[pIdx],
                                     triV[tIdx + 0], triV[tIdx + 1], triV[tIdx + 2],
                                     isec[isecIdx]))
                {
#if 1 //if the point is near one the given entry, consider it as being the same point.
                    for (int k = 0; k < 6; k++)
                    {
                        if (length(isec[isecIdx] - triV[k]) < TestEpsilon::Get())
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
                        (isecIdx == 1 && length(isec[0] - isec[1]) > TestEpsilon::Get()))
                    {
                        found_isec = i;
                        isecIdx++;
                    }
                }
            }
        }

        if (isecIdx >= 2)
        {
            ip00 = isec[0];
            ip10 = isec[1];
            return true;
        }
        return false;
    }

    //Ray/Line : returns one of the RAY_ISECT_* defines.
    int TestRayVsRay(vec3 const &ray_p00, vec3 const &ray_p01,
                           vec3 const &ray_p10, vec3 const &ray_p11,
                           vec3 &isec_p)
    {
        vec3 rayD0 = ray_p01 - ray_p00;
        float rayS0 = length(rayD0);
        vec3 rayD0N = normalize(rayD0);

        vec3 rayD1 = ray_p11 - ray_p10;
        float rayS1 = length(rayD1);
        vec3 rayD1N = normalize(rayD1);

        vec3 rayP0P1 = ray_p10 - ray_p00;
        vec3 c01 = cross(rayD0N, rayD1N);
        float crs01S = length(c01);

        if (crs01S > -TestEpsilon::Get() && crs01S < TestEpsilon::Get())
            return 0;

        mat3 m0 = mat3(rayP0P1, rayD1N, c01);
        float t0 = determinant(m0) / (crs01S * crs01S);
        vec3 isec0 = ray_p00 + rayD0N * t0;

        mat3 m1 = mat3(rayP0P1, rayD0N, c01);
        float t1 = determinant(m1) / (crs01S * crs01S);
        vec3 isec1 = ray_p10 + rayD1N * t1;

        if (sqlength(isec0 - isec1) < TestEpsilon::Get()) //ray intersection
        {
            isec_p = (isec0 + isec0) * .5f;
            float d0 = (length(ray_p01 - isec_p) < TestEpsilon::Get() || length(ray_p00 - isec_p) < TestEpsilon::Get())?
                        (-1.0f):
                        (dot(ray_p00 - isec_p, ray_p01 - isec_p));
            float d1 = (length(ray_p10 - isec_p) < TestEpsilon::Get() || length(ray_p11 - isec_p) < TestEpsilon::Get())?
                        (-1.0f):
                        (dot(ray_p10 - isec_p, ray_p11 - isec_p));

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

    //used to find the intersecting points between a triangle side and a coplanar line.
    bool TestRayVsTriangleSide(vec3 const &v0, vec3 const &v1, vec3 const &v2,
                                     vec3 const &ip0, vec3 const &ip1,
                                     vec3 &iV0, int &iIdx0, vec3 &iV1, int &iIdx1)
    {
        vec3 isecV[2] = { vec3(.0f), vec3(.0f) };
        int isecI[2] = { -1, -1 };

        vec3 triV[3] = { v0, v1, v2 };
        int isecIdx = 0;
        vec3 isec_p(0);

        //Two points given, so we test each triangle side to find the intersect
        isecIdx = 0;
        for (int j = 0; j < 3 && isecIdx < 2; j++)
        {
            int Result = TestRayVsRay(triV[j], triV[(j + 1) % 3], ip0, ip1, isec_p);
            if (Result == RAY_ISECT_P0 || Result == RAY_ISECT_ALL)
            {
                isecV[isecIdx] = isec_p;
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

    //--
    bool TestPointVsFrustum(const vec3& point, const mat4& frustum, vec3* result_point)
    {
        vec4 proj_point = frustum * vec4(point, 1.f);
        proj_point /= proj_point.w;

        if (result_point)
            *result_point = proj_point.xyz;
        for (int i = 0; i < 3; i++)
            if (lol::abs(proj_point[i]) > 1.f)
                return false;
        return true;
    }
} /* namespace lol */

