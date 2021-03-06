//
//  Lol Engine — Unit tests for quaternions
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/unit_test>
#include <lol/vector>    // vec_t
#include <lol/transform> // quat_t
#include <lol/math>      // rand(), radians()

#include <cstring> // memset
#include <tuple>   // std::tuple
#include <vector>  // std::vector
#include <array>   // std::array

namespace lol
{

lolunit_declare_fixture(quaternion_test)
{
    void setup()
    {
        /* Generate identity quaternions */
        m_vectorpairs.push_back({ vec3::axis_x, vec3::axis_x });
        m_vectorpairs.push_back({ 2.f * vec3::axis_x, 3.f * vec3::axis_x });

        /* Generate 90-degree rotations */
        m_vectorpairs.push_back({ vec3::axis_x, vec3::axis_y });
        m_vectorpairs.push_back({ 2.f * vec3::axis_x, 3.f * vec3::axis_y });

        /* Generate 180-degree rotations */
        m_vectorpairs.push_back({ vec3::axis_x, -vec3::axis_x });
        m_vectorpairs.push_back({ 2.f * vec3::axis_x, -3.f * vec3::axis_x });

        /* Fill array with random test values */
        for (int i = 0; i < 10000; ++i)
        {
            vec3 v1 = pow(10.f, rand(-5.f, 5.f))
                    * vec3(rand(-1.f, 1.f), rand(-1.f, 1.f), rand(-1.f, 1.f));
            vec3 v2 = pow(10.f, rand(-5.f, 5.f))
                    * vec3(rand(-1.f, 1.f), rand(-1.f, 1.f), rand(-1.f, 1.f));
            m_vectorpairs.push_back({ v1, v2 });
        }
    }

    lolunit_declare_test(equality)
    {
        quat a4(1.f, 2.f, 3.f, 4.f);
        quat b4(0.f, 2.f, 3.f, 4.f);
        quat c4(1.f, 0.f, 3.f, 4.f);
        quat d4(1.f, 2.f, 0.f, 4.f);
        quat e4(1.f, 2.f, 3.f, 0.f);

        lolunit_assert_equal(a4, a4);
        lolunit_refute_different(a4, a4);

        lolunit_assert_different(a4, b4);
        lolunit_refute_equal(a4, b4);
        lolunit_assert_different(a4, c4);
        lolunit_refute_equal(a4, c4);
        lolunit_assert_different(a4, d4);
        lolunit_refute_equal(a4, d4);
        lolunit_assert_different(a4, e4);
        lolunit_refute_equal(a4, e4);
    }

    lolunit_declare_test(unary_minus)
    {
        quat a(1.f, 3.f, 2.f, 4.f);
        quat b(-1.f, -3.f, -2.f, -4.f);

        lolunit_assert_equal(a, -b);
        lolunit_assert_equal(-a, b);
    }

    lolunit_declare_test(conjugate)
    {
        quat a(1.f, 3.f, 2.f, 4.f);
        quat b(1.f, -3.f, -2.f, -4.f);

        lolunit_assert_equal(a, ~b);
        lolunit_assert_equal(~a, b);
    }

    lolunit_declare_test(quaternion_norm)
    {
        quat a(2.f, -2.f, -8.f, 3.f);

        lolunit_assert_equal(norm(a), 9.f);

        quat b = a * ~a;
        quat c(norm(a) * norm(a), 0.f, 0.f, 0.f);

        lolunit_assert_equal(b, c);

        quat d(2.f, 5.f, -4.f, -2.f);

        lolunit_assert_equal(norm(a * d), norm(a) * norm(d));
    }

    lolunit_declare_test(quaternion_dot)
    {
        quat a(-1.f, 2.f, -3.f, 4.f);
        quat b(8.f, 7.f, 6.f, 5.f);

        lolunit_assert_equal(dot(a, b), 8.f);
    }

    lolunit_declare_test(base)
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

    lolunit_declare_test(quaternion_normalize)
    {
        quat a(2.f, -2.f, -8.f, 3.f);
        quat b = normalize(a);

        lolunit_assert_doubles_equal(norm(b), 1.0, 1e-5);
    }

    lolunit_declare_test(quaternion_inverse)
    {
        quat a(2.f, -2.f, -8.f, 3.f);
        quat b = inverse(a);
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

    lolunit_declare_test(rotation)
    {
        /* Check that rotating 1 radian twice means rotating 2 radians */
        quat a = quat::rotate(1.f, vec3::axis_x);
        quat b = quat::rotate(2.f, vec3::axis_x);
        quat c = a * a;

        lolunit_assert_doubles_equal(c.w, b.w, 1e-5);
        lolunit_assert_doubles_equal(c.x, b.x, 1e-5);
        lolunit_assert_doubles_equal(c.y, b.y, 1e-5);
        lolunit_assert_doubles_equal(c.z, b.z, 1e-5);

        /* Check that rotating 1 radian then 2 is the same as 2 then 1 */
        quat d = a * b;
        quat e = b * a;

        lolunit_assert_doubles_equal(e.w, d.w, 1e-5);
        lolunit_assert_doubles_equal(e.x, d.x, 1e-5);
        lolunit_assert_doubles_equal(e.y, d.y, 1e-5);
        lolunit_assert_doubles_equal(e.z, d.z, 1e-5);
    }

    lolunit_declare_test(to_axis_angle)
    {
        quat q = quat::rotate(0.3f, vec3::axis_x);
        vec3 axis = q.axis();
        float angle = q.angle();

        lolunit_assert_doubles_equal(1.0, axis.x, 1e-6);
        lolunit_assert_doubles_equal(0.0, axis.y, 1e-6);
        lolunit_assert_doubles_equal(0.0, axis.z, 1e-6);

        lolunit_assert_doubles_equal(0.3f, (double)angle, 1e-6);
    }

    lolunit_declare_test(from_two_vectors)
    {
        for (auto pair : m_vectorpairs)
        {
            vec3 a0 = pair[0];
            vec3 b0 = pair[1];
            vec3 a = normalize(a0);
            vec3 b = normalize(b0);

            quat q = quat::rotate(a0, b0);

            auto ctx = a0.tostring() + " " + b0.tostring();
            lolunit_set_context(ctx);

            /* Check that q is a unit quaternion */
            lolunit_assert_doubles_equal(1.0, (double)norm(q), 1e-5);

            /* Check that q transforms a into b */
            vec3 c = q.transform(a);

            lolunit_assert_doubles_equal(c.x, b.x, 2e-5);
            lolunit_assert_doubles_equal(c.y, b.y, 2e-5);
            lolunit_assert_doubles_equal(c.z, b.z, 2e-5);

            /* Check that ~q transforms b into a */
            vec3 d = (~q).transform(b);

            lolunit_assert_doubles_equal(d.x, a.x, 2e-5);
            lolunit_assert_doubles_equal(d.y, a.y, 2e-5);
            lolunit_assert_doubles_equal(d.z, a.z, 2e-5);

            if (distance(a, b) > 1e-6f)
            {
                /* If a and b differ, check that the rotation axis is normal to both
                 * vectors, which is only true if the rotation uses the shortest path. */
                vec3 axis = q.axis();
                lolunit_assert_doubles_equal(0.0, (double)dot(axis, a), 1e-5);
                lolunit_assert_doubles_equal(0.0, (double)dot(axis, b), 1e-5);
            }
            else
            {
                /* If a and b are roughly the same, check that the rotation angle
                 * is zero. */
                lolunit_assert_doubles_equal(0.0, (double)q.angle(), 1e-5);
            }
        }
    }

    lolunit_declare_test(from_euler_norm)
    {
        for (int i = 0; i < 100; ++i)
        {
            vec3 angles(rand(100.f), rand(100.f), rand(100.f));

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

    lolunit_declare_test(first_two_euler_angles)
    {
        for (int i = 0; i < 100; ++i)
        {
            /* We check that fromeuler_xyx and fromeuler_xyz give the
             * same result if the 3rd angle is zero. */
            vec3 angles(rand(radians(360.f)), rand(radians(360.f)), 0.f);
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

    lolunit_declare_test(last_two_euler_angles)
    {
        for (int i = 0; i < 100; ++i)
        {
            /* We check that fromeuler_zyz and fromeuler_xyz give the
             * same result if the 1st angle is zero. */
            vec3 angles(0.f, rand(radians(360.f)), rand(radians(360.f)));
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

    lolunit_declare_test(tait_bryan_angles)
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

    lolunit_declare_test(euler_angles)
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
    std::vector<std::array<vec3,2>> m_vectorpairs;
};

} /* namespace lol */

