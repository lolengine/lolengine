//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
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

LOLUNIT_FIXTURE(RotationTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(Rotate2D)
    {
        /* Rotations must be CCW */
        mat2 m90 = mat2::rotate(90.f);

        vec2 a(2.f, 3.f);
        vec2 b = m90 * a;

        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.x, -a.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.y,  a.x, 1e-5);

        float d = determinant(m90);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(d, 1.0, 1e-5);
    }

    LOLUNIT_TEST(Compose2D)
    {
        /* Rotating 20 degrees twice must equal rotating 40 degrees */
        mat2 m20 = mat2::rotate(20.f);
        mat2 m40 = mat2::rotate(40.f);
        mat2 m20x20 = m20 * m20;

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[0][0], m40[0][0], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[1][0], m40[1][0], 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[0][1], m40[0][1], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[1][1], m40[1][1], 1e-5);
    }

    LOLUNIT_TEST(Rotate3D)
    {
        /* Rotations must be CCW along each axis */
        mat3 m90x = mat3::rotate(90.f, 1.f, 0.f, 0.f);
        mat3 m90y = mat3::rotate(90.f, 0.f, 1.f, 0.f);
        mat3 m90z = mat3::rotate(90.f, 0.f, 0.f, 1.f);

        vec3 a(2.f, 3.f, 4.f);
        vec3 b = m90x * a;
        vec3 c = m90y * a;
        vec3 d = m90z * a;

        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.x,  a.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.y, -a.z, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.z,  a.y, 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.x,  a.z, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.y,  a.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.z, -a.x, 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(d.x, -a.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(d.y,  a.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(d.z,  a.z, 1e-5);

        float dx = determinant(m90x);
        float dy = determinant(m90y);
        float dz = determinant(m90z);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(dx, 1.0, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(dy, 1.0, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(dz, 1.0, 1e-5);
    }

    LOLUNIT_TEST(Compose3D)
    {
        /* Rotating 20 degrees twice must equal rotating 40 degrees */
        mat3 m20 = mat3::rotate(20.f, 1.f, 2.f, 3.f);
        mat3 m40 = mat3::rotate(40.f, 1.f, 2.f, 3.f);
        mat3 m20x20 = m20 * m20;

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[0][0], m40[0][0], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[1][0], m40[1][0], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[2][0], m40[2][0], 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[0][1], m40[0][1], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[1][1], m40[1][1], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[2][1], m40[2][1], 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[0][2], m40[0][2], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[1][2], m40[1][2], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m20x20[2][2], m40[2][2], 1e-5);
    }

    LOLUNIT_TEST(QuaternionTransform)
    {
        /* Rotating using a quaternion must equal rotating using a matrix */
        mat3 m20 = mat3::rotate(20.f, 1.f, 2.f, 3.f);
        quat q20 = quat::rotate(20.f, 1.f, 2.f, 3.f);
        vec3 a(-2.f, 4.f, 3.f);

        vec3 b = m20 * a;
        vec3 c = q20.transform(a);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.x, b.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.y, b.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.z, b.z, 1e-5);

        float n = norm(q20);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(n, 1.0, 1e-5);
    }

    LOLUNIT_TEST(QuaternionFromMatrix)
    {
        /* A rotation matrix converted to a quaternion should match the
         * quaternion built with the same parameters */
        quat q1 = quat::rotate(20.f, 1.f, 2.f, 3.f);
        quat q2 = quat(mat3::rotate(20.f, 1.f, 2.f, 3.f));

        LOLUNIT_ASSERT_DOUBLES_EQUAL(q2.w, q1.w, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(q2.x, q1.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(q2.y, q1.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(q2.z, q1.z, 1e-5);

        float n1 = norm(q1);
        float n2 = norm(q2);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(n1, 1.0, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(n2, 1.0, 1e-5);
    }

    LOLUNIT_TEST(MatrixFromQuaternion)
    {
        /* A quaternion converted to a rotation matrix should match the
         * rotation matrix built with the same parameters */
        mat3 m1 = mat3::rotate(60.f, 1.f, -2.f, 3.f);
        mat3 m2 = mat3(quat::rotate(60.f, 1.f, -2.f, 3.f));

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m2[0][0], m1[0][0], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m2[1][0], m1[1][0], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m2[2][0], m1[2][0], 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m2[0][1], m1[0][1], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m2[1][1], m1[1][1], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m2[2][1], m1[2][1], 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m2[0][2], m1[0][2], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m2[1][2], m1[1][2], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m2[2][2], m1[2][2], 1e-5);

        float d1 = determinant(m1);
        float d2 = determinant(m2);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(d1, 1.0, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(d2, 1.0, 1e-5);
    }

    LOLUNIT_TEST(MatrixCompositionThroughQuaternions)
    {
        /* Combining two rotation matrices should match the matrix created
         * from the combination of the two equivalent quaternions */
        mat3 m1 = mat3::rotate(60.f, 1.f, -2.f, 3.f);
        mat3 m2 = mat3::rotate(20.f, -3.f, 1.f, -3.f);

        mat3 m3 = m2 * m1;
        mat3 m4(quat(m2) * quat(m1));

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m4[0][0], m3[0][0], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m4[1][0], m3[1][0], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m4[2][0], m3[2][0], 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m4[0][1], m3[0][1], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m4[1][1], m3[1][1], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m4[2][1], m3[2][1], 1e-5);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m4[0][2], m3[0][2], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m4[1][2], m3[1][2], 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m4[2][2], m3[2][2], 1e-5);
    }

    LOLUNIT_TEST(QuaternionCompositionThroughMatrices)
    {
        /* Combining two quaternions should match the quaternion created
         * from the combination of the two equivalent rotation matrices */
        quat q1 = quat::rotate(60.f, 1.f, -2.f, 3.f);
        quat q2 = quat::rotate(20.f, -3.f, 1.f, -2.f);

        quat q3 = q2 * q1;
        quat q4(mat3(q2) * mat3(q1));

        LOLUNIT_ASSERT_DOUBLES_EQUAL(q4.w, q3.w, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(q4.x, q3.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(q4.y, q3.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(q4.z, q3.z, 1e-5);
    }
};

} /* namespace lol */

