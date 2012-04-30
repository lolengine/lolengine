//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
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
        /* Check that rotation is CCW */
        mat2 m90 = mat2::rotate(90.f);

        vec2 a(2.f, 3.f);
        vec2 b = m90 * a;

        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.x, -a.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(b.y,  a.x, 1e-5);
    }

    LOLUNIT_TEST(Compose2D)
    {
        /* Check that rotating 20 degrees twice means rotating 40 degrees */
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
        /* Check that rotation is CCW around each axis */
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
    }

    LOLUNIT_TEST(Compose3D)
    {
        /* Check that rotating 20 degrees twice means rotating 40 degrees */
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
        /* Check that rotating using a quaternion is the same as rotating
         * using a matrix */
        mat3 m20 = mat3::rotate(20.f, 1.f, 2.f, 3.f);
        quat q20 = quat::rotate(20.f, 1.f, 2.f, 3.f);
        vec3 a(-2.f, 4.f, 3.f);

        vec3 b = m20 * a;
        vec3 c = q20.transform(a);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.x, b.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.y, b.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(c.z, b.z, 1e-5);
    }

    LOLUNIT_TEST(QuaternionFromMatrix)
    {
        quat q1 = quat::rotate(20.f, 1.f, 2.f, 3.f);
        quat q2 = quat(mat3::rotate(20.f, 1.f, 2.f, 3.f));

        LOLUNIT_ASSERT_DOUBLES_EQUAL(q2.w, q1.w, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(q2.x, q1.x, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(q2.y, q1.y, 1e-5);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(q2.z, q1.z, 1e-5);
    }
};

} /* namespace lol */

