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

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(QuaternionTest)
{
    void SetUp()
    {
        /* Generate identity quaternions */
        m_vectorpairs.Push(vec3::axis_x, vec3::axis_x);
        m_vectorpairs.Push(2.f * vec3::axis_x, 3.f * vec3::axis_x);

        /* Generate 90-degree rotations */
        m_vectorpairs.Push(vec3::axis_x, vec3::axis_y);
        m_vectorpairs.Push(2.f * vec3::axis_x, 3.f * vec3::axis_y);

        /* Generate 180-degree rotations */
        m_vectorpairs.Push(vec3::axis_x, -vec3::axis_x);
        m_vectorpairs.Push(2.f * vec3::axis_x, -3.f * vec3::axis_x);

        /* Fill array with random test values */
        for (int i = 0; i < 10000; ++i)
        {
            vec3 v1 = lol::pow(10.f, rand(-5.f, 5.f))
                    * vec3(rand(-1.f, 1.f), rand(-1.f, 1.f), rand(-1.f, 1.f));
            vec3 v2 = lol::pow(10.f, rand(-5.f, 5.f))
                    * vec3(rand(-1.f, 1.f), rand(-1.f, 1.f), rand(-1.f, 1.f));
            m_vectorpairs.Push(v1, v2);
        }
    }

    void TearDown() {}

    LOLUNIT_TEST(Equality)
    {
        quat a4(1.f, 2.f, 3.f, 4.f);
        quat b4(0.f, 2.f, 3.f, 4.f);
        quat c4(1.f, 0.f, 3.f, 4.f);
        quat d4(1.f, 2.f, 0.f, 4.f);
        quat e4(1.f, 2.f, 3.f, 0.f);

        LOLUNIT_ASSERT_EQUAL(a4, a4);
        LOLUNIT_ASSERT_NOT_DIFFERENT(a4, a4);

        LOLUNIT_ASSERT_DIFFERENT(a4, b4);
        LOLUNIT_ASSERT_NOT_EQUAL(a4, b4);
        LOLUNIT_ASSERT_DIFFERENT(a4, c4);
        LOLUNIT_ASSERT_NOT_EQUAL(a4, c4);
        LOLUNIT_ASSERT_DIFFERENT(a4, d4);
        LOLUNIT_ASSERT_NOT_EQUAL(a4, d4);
        LOLUNIT_ASSERT_DIFFERENT(a4, e4);
        LOLUNIT_ASSERT_NOT_EQUAL(a4, e4);
    }

    LOLUNIT_TEST(UnaryMinus)
    {
        quat a(1.f, 3.f, 2.f, 4.f);
        quat b(-1.f, -3.f, -2.f, -4.f);

        LOLUNIT_ASSERT_EQUAL(a, -b);
        LOLUNIT_ASSERT_EQUAL(-a, b);
    }

    LOLUNIT_TEST(Conjugate)
    {
        quat a(1.f, 3.f, 2.f, 4.f);
        quat b(1.f, -3.f, -2.f, -4.f);

        LOLUNIT_ASSERT_EQUAL(a, ~b);
        LOLUNIT_ASSERT_EQUAL(~a, b);
    }

    LOLUNIT_TEST(Norm)
    {
        quat a(2.f, -2.f, -8.f, 3.f);

        LOLUNIT_ASSERT_EQUAL(norm(a), 9.f);

        quat b = a * ~a;
        quat c(norm(a) * norm(a), 0.f, 0.f, 0.f);

        LOLUNIT_ASSERT_EQUAL(b, c);

        quat d(2.f, 5.f, -4.f, -2.f);

        LOLUNIT_ASSERT_EQUAL(norm(a * d), norm(a) * norm(d));
    }

    LOLUNIT_TEST(Dot)
    {
        quat a(-1.f, 2.f, -3.f, 4.f);
        quat b(8.f, 7.f, 6.f, 5.f);

        LOLUNIT_ASSERT_EQUAL(dot(a, b), 8.f);
    }

    LOLUNIT_TEST(Base)
    {
        quat one(1.f, 0.f, 0.f, 0.f);
        quat i(0.f, 1.f, 0.f, 0.f);
        quat j(0.f, 0.f, 1.f, 0.f);
        quat k(0.f, 0.f, 0.f, 1.f);

        LOLUNIT_ASSERT_EQUAL(norm(one), 1.f);
        LOLUNIT_ASSERT_EQUAL(norm(i), 1.f);
        LOLUNIT_ASSERT_EQUAL(norm(j), 1.f);
        LOLUNIT_ASSERT_EQUAL(norm(k), 1.f);

        LOLUNIT_ASSERT_EQUAL(i * i, -one);
        LOLUNIT_ASSERT_EQUAL(j * j, -one);
        LOLUNIT_ASSERT_EQUAL(k * k, -one);
        LOLUNIT_ASSERT_EQUAL(i * j * k, -one);

        LOLUNIT_ASSERT_EQUAL(i * j, k);
        LOLUNIT_ASSERT_EQUAL(j * i, -k);
        LOLUNIT_ASSERT_EQUAL(j * k, i);
        LOLUNIT_ASSERT_EQUAL(k * j, -i);
        LOLUNIT_ASSERT_EQUAL(k * i, j);
        LOLUNIT_ASSERT_EQUAL(i * k, -j);
    }

    LOLUNIT_TEST(Normalize)
    {
        quat a(2.f, -2.f, -8.f, 3.f);
        quat b = normalize(a);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(b), 1.0, 1e-5);
    }

    LOLUNIT_TEST(Reciprocal)
    {
        quat a(2.f, -2.f, -8.f, 3.f);
        quat b = re(a);
        quat c = 1.f / a;

        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.w, c.w, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.x, c.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.y, c.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.z, c.z, 1e-5);

        quat m1 = a * b;
        quat m2 = b * a;
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.w, m2.w, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.x, m2.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.y, m2.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.z, m2.z, 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.w, 1.0, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.x, 0.0, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.y, 0.0, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.z, 0.0, 1e-5);
    }

    LOLUNIT_TEST(Rotation)
    {
        /* Check that rotating 10 degrees twice means rotating 20 degrees */
        quat a = quat::rotate(10.f, vec3::axis_x);
        quat b = quat::rotate(20.f, vec3::axis_x);
        quat c = a * a;

        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.w, b.w, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.x, b.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.y, b.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.z, b.z, 1e-5);

        /* Check that rotating 10 degrees then 20 is the same as 20 then 10 */
        quat d = a * b;
        quat e = b * a;

        LOLUNIT_ASSERT_DOUBLES_EQUAL(e.w, d.w, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(e.x, d.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(e.y, d.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(e.z, d.z, 1e-5);
    }

    LOLUNIT_TEST(ToAxisAngle)
    {
        quat q = quat::rotate(10.f, vec3::axis_x);
        vec3 axis = q.axis();
        float angle = q.angle();

        LOLUNIT_ASSERT_DOUBLES_EQUAL(1.0, axis.x, 1e-6);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(0.0, axis.y, 1e-6);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(0.0, axis.z, 1e-6);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(10.0, (double)degrees(angle), 1e-6);
    }

    LOLUNIT_TEST(FromTwoVectors)
    {
        for (auto pair : m_vectorpairs)
        {
            vec3 a = pair.m1;
            vec3 b = pair.m2;
            vec3 da = normalize(a);
            vec3 db = normalize(b);

            quat q = quat::rotate(a, b);

            /* Check that q is a unit quaternion */
            LOLUNIT_ASSERT_DOUBLES_EQUAL(1.0, (double)norm(q), 1e-5);

            /* Check that q transforms da into db */
            vec3 c = q.transform(da);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(c.x, db.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(c.y, db.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(c.z, db.z, 1e-5);

            /* Check that ~q transforms db into da */
            vec3 d = (~q).transform(db);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(d.x, da.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(d.y, da.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(d.z, da.z, 1e-5);

            if (distance(da, db) > 1e-6f)
            {
                /* If da and db differ, check that the rotation axis is normal to both
                 * vectors, which is only true if the rotation uses the shortest path. */
                vec3 axis = q.axis();
                LOLUNIT_ASSERT_DOUBLES_EQUAL(0.0, (double)dot(axis, da), 1e-5);
                LOLUNIT_ASSERT_DOUBLES_EQUAL(0.0, (double)dot(axis, db), 1e-5);
            }
            else
            {
                /* If da and db are roughly the same, check that the rotation angle
                 * is zero. */
                LOLUNIT_ASSERT_DOUBLES_EQUAL(0.0, (double)q.angle(), 1e-5);
            }
        }
    }

    LOLUNIT_TEST(FromEulerNorm)
    {
        for (int i = 0; i < 100; ++i)
        {
            vec3 angles(rand(360.f), rand(360.f), rand(360.f));

            /* Tait-Bryan */
            quat q1 = quat::fromeuler_xyz(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q1), 1.f, 1e-5);

            quat q2 = quat::fromeuler_yzx(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q2), 1.f, 1e-5);

            quat q3 = quat::fromeuler_zxy(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q3), 1.f, 1e-5);

            quat q4 = quat::fromeuler_xzy(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q4), 1.f, 1e-5);

            quat q5 = quat::fromeuler_zyx(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q5), 1.f, 1e-5);

            quat q6 = quat::fromeuler_yxz(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q6), 1.f, 1e-5);

            /* Euler */
            quat q7 = quat::fromeuler_xyx(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q7), 1.f, 1e-5);

            quat q8 = quat::fromeuler_yzy(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q8), 1.f, 1e-5);

            quat q9 = quat::fromeuler_zxz(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q9), 1.f, 1e-5);

            quat q10 = quat::fromeuler_xzx(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q10), 1.f, 1e-5);

            quat q11 = quat::fromeuler_zyz(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q11), 1.f, 1e-5);

            quat q12 = quat::fromeuler_yxy(angles);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(q12), 1.f, 1e-5);
        }
    }

    LOLUNIT_TEST(FirstTwoEulerAngles)
    {
        for (int i = 0; i < 100; ++i)
        {
            /* We check that fromeuler_xyx and fromeuler_xyz give the
             * same result if the 3rd angle is zero. */
            vec3 angles(rand(360.f), rand(360.f), 0.f);
            quat q1, q2;

            q1 = quat::fromeuler_xyz(angles);
            q2 = quat::fromeuler_xyx(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_yzx(angles);
            q2 = quat::fromeuler_yzy(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_zxy(angles);
            q2 = quat::fromeuler_zxz(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_xzy(angles);
            q2 = quat::fromeuler_xzx(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_zyx(angles);
            q2 = quat::fromeuler_zyz(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_yxz(angles);
            q2 = quat::fromeuler_yxy(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);
        }
    }

    LOLUNIT_TEST(LastTwoEulerAngles)
    {
        for (int i = 0; i < 100; ++i)
        {
            /* We check that fromeuler_zyz and fromeuler_xyz give the
             * same result if the 1st angle is zero. */
            vec3 angles(0.f, rand(360.f), rand(360.f));
            quat q1, q2;

            q1 = quat::fromeuler_xyz(angles);
            q2 = quat::fromeuler_zyz(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_yzx(angles);
            q2 = quat::fromeuler_xzx(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_zxy(angles);
            q2 = quat::fromeuler_yxy(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_xzy(angles);
            q2 = quat::fromeuler_yzy(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_zyx(angles);
            q2 = quat::fromeuler_xyx(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_yxz(angles);
            q2 = quat::fromeuler_zxz(angles);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.w, q2.w, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.x, q2.x, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.y, q2.y, 1e-5);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(q1.z, q2.z, 1e-5);
        }
    }

    LOLUNIT_TEST(TaitBryanAngles)
    {
        for (int i = 0; i < 100; ++i)
        {
            /* Pick a random point and a random quaternion. We want
             * to check whether going to Tait-Bryan angles and back to
             * quaternion creates the same transform. */
            vec3 p(rand(1.f, 2.f), rand(1.f, 2.f), rand(1.f, 2.f));
            quat q0 = normalize(quat(rand(-1.f, 1.f), rand(-1.f, 1.f),
                                     rand(-1.f, 1.f), rand(-1.f, 1.f)));
            vec3 p0 = q0.transform(p);

            /* x-y-z */
            quat q1 = quat::fromeuler_xyz(vec3::toeuler_xyz(q0));
            vec3 p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* y-z-x */
            q1 = quat::fromeuler_yzx(vec3::toeuler_yzx(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* z-x-y */
            q1 = quat::fromeuler_zxy(vec3::toeuler_zxy(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* x-z-y */
            q1 = quat::fromeuler_xzy(vec3::toeuler_xzy(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* z-y-x */
            q1 = quat::fromeuler_zyx(vec3::toeuler_zyx(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* y-x-z */
            q1 = quat::fromeuler_yxz(vec3::toeuler_yxz(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);
        }
    }

    LOLUNIT_TEST(EulerAngles)
    {
        for (int i = 0; i < 100; ++i)
        {
            /* Pick a random point and a random quaternion. We want
             * to check whether going to Euler angles and back to
             * quaternion creates the same transform. */
            vec3 p(rand(1.f, 2.f), rand(1.f, 2.f), rand(1.f, 2.f));
            quat q0 = normalize(quat(rand(-1.f, 1.f), rand(-1.f, 1.f),
                                     rand(-1.f, 1.f), rand(-1.f, 1.f)));
            vec3 p0 = q0.transform(p);

            /* x-y-x */
            quat q1 = quat::fromeuler_xyx(vec3::toeuler_xyx(q0));
            vec3 p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* y-z-y */
            q1 = quat::fromeuler_yzy(vec3::toeuler_yzy(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* z-x-z */
            q1 = quat::fromeuler_zxz(vec3::toeuler_zxz(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* x-z-x */
            q1 = quat::fromeuler_xzx(vec3::toeuler_xzx(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* z-y-z */
            q1 = quat::fromeuler_zyz(vec3::toeuler_zyz(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);

            /* y-x-y */
            q1 = quat::fromeuler_yxy(vec3::toeuler_yxy(q0));
            p1 = q1.transform(p);

            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p0.x, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p0.y, 1e-4);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p0.z, 1e-4);
        }
    }

private:
    array<vec3, vec3> m_vectorpairs;
};

} /* namespace lol */

