//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2014 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(QuaternionTest)
{
    void SetUp()
    {
        /* Generate identity quaternions */
        m_vectorpairs.push(vec3::axis_x, vec3::axis_x);
        m_vectorpairs.push(2.f * vec3::axis_x, 3.f * vec3::axis_x);

        /* Generate 90-degree rotations */
        m_vectorpairs.push(vec3::axis_x, vec3::axis_y);
        m_vectorpairs.push(2.f * vec3::axis_x, 3.f * vec3::axis_y);

        /* Generate 180-degree rotations */
        m_vectorpairs.push(vec3::axis_x, -vec3::axis_x);
        m_vectorpairs.push(2.f * vec3::axis_x, -3.f * vec3::axis_x);

        /* Fill array with random test values */
        for (int i = 0; i < 10000; ++i)
        {
            vec3 v1 = lol::pow(10.f, rand(-5.f, 5.f))
                    * vec3(rand(-1.f, 1.f), rand(-1.f, 1.f), rand(-1.f, 1.f));
            vec3 v2 = lol::pow(10.f, rand(-5.f, 5.f))
                    * vec3(rand(-1.f, 1.f), rand(-1.f, 1.f), rand(-1.f, 1.f));
            m_vectorpairs.push(v1, v2);
        }
    }

    void TearDown() {}

    lolunit_declare_test(Equality)
    {
        quat a4(1.f, 2.f, 3.f, 4.f);
        quat b4(0.f, 2.f, 3.f, 4.f);
        quat c4(1.f, 0.f, 3.f, 4.f);
        quat d4(1.f, 2.f, 0.f, 4.f);
        quat e4(1.f, 2.f, 3.f, 0.f);

        lolunit_assert_equal(a4, a4);
        lolunit_assert_not_different(a4, a4);

        lolunit_assert_different(a4, b4);
        lolunit_assert_not_equal(a4, b4);
        lolunit_assert_different(a4, c4);
        lolunit_assert_not_equal(a4, c4);
        lolunit_assert_different(a4, d4);
        lolunit_assert_not_equal(a4, d4);
        lolunit_assert_different(a4, e4);
        lolunit_assert_not_equal(a4, e4);
    }

    lolunit_declare_test(UnaryMinus)
    {
        quat a(1.f, 3.f, 2.f, 4.f);
        quat b(-1.f, -3.f, -2.f, -4.f);

        lolunit_assert_equal(a, -b);
        lolunit_assert_equal(-a, b);
    }

    lolunit_declare_test(Conjugate)
    {
        quat a(1.f, 3.f, 2.f, 4.f);
        quat b(1.f, -3.f, -2.f, -4.f);

        lolunit_assert_equal(a, ~b);
        lolunit_assert_equal(~a, b);
    }

    lolunit_declare_test(Norm)
    {
        quat a(2.f, -2.f, -8.f, 3.f);

        lolunit_assert_equal(norm(a), 9.f);

        quat b = a * ~a;
        quat c(norm(a) * norm(a), 0.f, 0.f, 0.f);

        lolunit_assert_equal(b, c);

        quat d(2.f, 5.f, -4.f, -2.f);

        lolunit_assert_equal(norm(a * d), norm(a) * norm(d));
    }

    lolunit_declare_test(Dot)
    {
        quat a(-1.f, 2.f, -3.f, 4.f);
        quat b(8.f, 7.f, 6.f, 5.f);

        lolunit_assert_equal(dot(a, b), 8.f);
    }

    lolunit_declare_test(Base)
    {
        quat one(1.f, 0.f, 0.f, 0.f);
        quat i(0.f, 1.f, 0.f, 0.f);
        quat j(0.f, 0.f, 1.f, 0.f);
        quat k(0.f, 0.f, 0.f, 1.f);

        lolunit_assert_equal(norm(one), 1.f);
        lolunit_assert_equal(norm(i), 1.f);
        lolunit_assert_equal(norm(j), 1.f);
        lolunit_assert_equal(norm(k), 1.f);

        lolunit_assert_equal(i * i, -one);
        lolunit_assert_equal(j * j, -one);
        lolunit_assert_equal(k * k, -one);
        lolunit_assert_equal(i * j * k, -one);

        lolunit_assert_equal(i * j, k);
        lolunit_assert_equal(j * i, -k);
        lolunit_assert_equal(j * k, i);
        lolunit_assert_equal(k * j, -i);
        lolunit_assert_equal(k * i, j);
        lolunit_assert_equal(i * k, -j);
    }

    lolunit_declare_test(Normalize)
    {
        quat a(2.f, -2.f, -8.f, 3.f);
        quat b = normalize(a);

        lolunit_assert_doubles_equal(norm(b), 1.0, 1e-5);
    }

    lolunit_declare_test(Reciprocal)
    {
        quat a(2.f, -2.f, -8.f, 3.f);
        quat b = re(a);
        quat c = 1.f / a;

        lolunit_assert_doubles_equal(b.w, c.w, 1e-5);
        lolunit_assert_doubles_equal(b.x, c.x, 1e-5);
        lolunit_assert_doubles_equal(b.y, c.y, 1e-5);
        lolunit_assert_doubles_equal(b.z, c.z, 1e-5);

        quat m1 = a * b;
        quat m2 = b * a;
        lolunit_assert_doubles_equal(m1.w, m2.w, 1e-5);
        lolunit_assert_doubles_equal(m1.x, m2.x, 1e-5);
        lolunit_assert_doubles_equal(m1.y, m2.y, 1e-5);
        lolunit_assert_doubles_equal(m1.z, m2.z, 1e-5);

        lolunit_assert_doubles_equal(m1.w, 1.0, 1e-5);
        lolunit_assert_doubles_equal(m1.x, 0.0, 1e-5);
        lolunit_assert_doubles_equal(m1.y, 0.0, 1e-5);
        lolunit_assert_doubles_equal(m1.z, 0.0, 1e-5);
    }

    lolunit_declare_test(Rotation)
    {
        /* Check that rotating 10 degrees twice means rotating 20 degrees */
        quat a = quat::rotate(10.f, vec3::axis_x);
        quat b = quat::rotate(20.f, vec3::axis_x);
        quat c = a * a;

        lolunit_assert_doubles_equal(c.w, b.w, 1e-5);
        lolunit_assert_doubles_equal(c.x, b.x, 1e-5);
        lolunit_assert_doubles_equal(c.y, b.y, 1e-5);
        lolunit_assert_doubles_equal(c.z, b.z, 1e-5);

        /* Check that rotating 10 degrees then 20 is the same as 20 then 10 */
        quat d = a * b;
        quat e = b * a;

        lolunit_assert_doubles_equal(e.w, d.w, 1e-5);
        lolunit_assert_doubles_equal(e.x, d.x, 1e-5);
        lolunit_assert_doubles_equal(e.y, d.y, 1e-5);
        lolunit_assert_doubles_equal(e.z, d.z, 1e-5);
    }

    lolunit_declare_test(ToAxisAngle)
    {
        quat q = quat::rotate(10.f, vec3::axis_x);
        vec3 axis = q.axis();
        float angle = q.angle();

        lolunit_assert_doubles_equal(1.0, axis.x, 1e-6);
        lolunit_assert_doubles_equal(0.0, axis.y, 1e-6);
        lolunit_assert_doubles_equal(0.0, axis.z, 1e-6);

        lolunit_assert_doubles_equal(10.0, (double)degrees(angle), 1e-6);
    }

    lolunit_declare_test(FromTwoVectors)
    {
        for (auto pair : m_vectorpairs)
        {
            vec3 a = pair.m1;
            vec3 b = pair.m2;
            vec3 da = normalize(a);
            vec3 db = normalize(b);

            quat q = quat::rotate(a, b);

            /* Check that q is a unit quaternion */
            lolunit_assert_doubles_equal(1.0, (double)norm(q), 1e-5);

            /* Check that q transforms da into db */
            vec3 c = q.transform(da);

            lolunit_assert_doubles_equal(c.x, db.x, 1e-5);
            lolunit_assert_doubles_equal(c.y, db.y, 1e-5);
            lolunit_assert_doubles_equal(c.z, db.z, 1e-5);

            /* Check that ~q transforms db into da */
            vec3 d = (~q).transform(db);

            lolunit_assert_doubles_equal(d.x, da.x, 1e-5);
            lolunit_assert_doubles_equal(d.y, da.y, 1e-5);
            lolunit_assert_doubles_equal(d.z, da.z, 1e-5);

            if (distance(da, db) > 1e-6f)
            {
                /* If da and db differ, check that the rotation axis is normal to both
                 * vectors, which is only true if the rotation uses the shortest path. */
                vec3 axis = q.axis();
                lolunit_assert_doubles_equal(0.0, (double)dot(axis, da), 1e-5);
                lolunit_assert_doubles_equal(0.0, (double)dot(axis, db), 1e-5);
            }
            else
            {
                /* If da and db are roughly the same, check that the rotation angle
                 * is zero. */
                lolunit_assert_doubles_equal(0.0, (double)q.angle(), 1e-5);
            }
        }
    }

    lolunit_declare_test(FromEulerNorm)
    {
        for (int i = 0; i < 100; ++i)
        {
            vec3 angles(rand(360.f), rand(360.f), rand(360.f));

            /* Tait-Bryan */
            quat q1 = quat::fromeuler_xyz(angles);
            lolunit_assert_doubles_equal(norm(q1), 1.f, 1e-5);

            quat q2 = quat::fromeuler_yzx(angles);
            lolunit_assert_doubles_equal(norm(q2), 1.f, 1e-5);

            quat q3 = quat::fromeuler_zxy(angles);
            lolunit_assert_doubles_equal(norm(q3), 1.f, 1e-5);

            quat q4 = quat::fromeuler_xzy(angles);
            lolunit_assert_doubles_equal(norm(q4), 1.f, 1e-5);

            quat q5 = quat::fromeuler_zyx(angles);
            lolunit_assert_doubles_equal(norm(q5), 1.f, 1e-5);

            quat q6 = quat::fromeuler_yxz(angles);
            lolunit_assert_doubles_equal(norm(q6), 1.f, 1e-5);

            /* Euler */
            quat q7 = quat::fromeuler_xyx(angles);
            lolunit_assert_doubles_equal(norm(q7), 1.f, 1e-5);

            quat q8 = quat::fromeuler_yzy(angles);
            lolunit_assert_doubles_equal(norm(q8), 1.f, 1e-5);

            quat q9 = quat::fromeuler_zxz(angles);
            lolunit_assert_doubles_equal(norm(q9), 1.f, 1e-5);

            quat q10 = quat::fromeuler_xzx(angles);
            lolunit_assert_doubles_equal(norm(q10), 1.f, 1e-5);

            quat q11 = quat::fromeuler_zyz(angles);
            lolunit_assert_doubles_equal(norm(q11), 1.f, 1e-5);

            quat q12 = quat::fromeuler_yxy(angles);
            lolunit_assert_doubles_equal(norm(q12), 1.f, 1e-5);
        }
    }

    lolunit_declare_test(FirstTwoEulerAngles)
    {
        for (int i = 0; i < 100; ++i)
        {
            /* We check that fromeuler_xyx and fromeuler_xyz give the
             * same result if the 3rd angle is zero. */
            vec3 angles(rand(360.f), rand(360.f), 0.f);
            quat q1, q2;

            q1 = quat::fromeuler_xyz(angles);
            q2 = quat::fromeuler_xyx(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_yzx(angles);
            q2 = quat::fromeuler_yzy(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_zxy(angles);
            q2 = quat::fromeuler_zxz(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_xzy(angles);
            q2 = quat::fromeuler_xzx(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_zyx(angles);
            q2 = quat::fromeuler_zyz(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_yxz(angles);
            q2 = quat::fromeuler_yxy(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);
        }
    }

    lolunit_declare_test(LastTwoEulerAngles)
    {
        for (int i = 0; i < 100; ++i)
        {
            /* We check that fromeuler_zyz and fromeuler_xyz give the
             * same result if the 1st angle is zero. */
            vec3 angles(0.f, rand(360.f), rand(360.f));
            quat q1, q2;

            q1 = quat::fromeuler_xyz(angles);
            q2 = quat::fromeuler_zyz(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_yzx(angles);
            q2 = quat::fromeuler_xzx(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_zxy(angles);
            q2 = quat::fromeuler_yxy(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_xzy(angles);
            q2 = quat::fromeuler_yzy(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_zyx(angles);
            q2 = quat::fromeuler_xyx(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);

            q1 = quat::fromeuler_yxz(angles);
            q2 = quat::fromeuler_zxz(angles);

            lolunit_assert_doubles_equal(q1.w, q2.w, 1e-5);
            lolunit_assert_doubles_equal(q1.x, q2.x, 1e-5);
            lolunit_assert_doubles_equal(q1.y, q2.y, 1e-5);
            lolunit_assert_doubles_equal(q1.z, q2.z, 1e-5);
        }
    }

    lolunit_declare_test(TaitBryanAngles)
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

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* y-z-x */
            q1 = quat::fromeuler_yzx(vec3::toeuler_yzx(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* z-x-y */
            q1 = quat::fromeuler_zxy(vec3::toeuler_zxy(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* x-z-y */
            q1 = quat::fromeuler_xzy(vec3::toeuler_xzy(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* z-y-x */
            q1 = quat::fromeuler_zyx(vec3::toeuler_zyx(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* y-x-z */
            q1 = quat::fromeuler_yxz(vec3::toeuler_yxz(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);
        }
    }

    lolunit_declare_test(EulerAngles)
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

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* y-z-y */
            q1 = quat::fromeuler_yzy(vec3::toeuler_yzy(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* z-x-z */
            q1 = quat::fromeuler_zxz(vec3::toeuler_zxz(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* x-z-x */
            q1 = quat::fromeuler_xzx(vec3::toeuler_xzx(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* z-y-z */
            q1 = quat::fromeuler_zyz(vec3::toeuler_zyz(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);

            /* y-x-y */
            q1 = quat::fromeuler_yxy(vec3::toeuler_yxy(q0));
            p1 = q1.transform(p);

            lolunit_assert_doubles_equal(p1.x, p0.x, 1e-4);
            lolunit_assert_doubles_equal(p1.y, p0.y, 1e-4);
            lolunit_assert_doubles_equal(p1.z, p0.z, 1e-4);
        }
    }

private:
    array<vec3, vec3> m_vectorpairs;
};

} /* namespace lol */

