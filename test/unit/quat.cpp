//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
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

LOLUNIT_FIXTURE(QuaternionTest)
{
    void SetUp() {}

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

        LOLUNIT_ASSERT_EQUAL(norm(a), 81.f);

        quat b = a * ~a;
        quat c(norm(a), 0.f, 0.f, 0.f);

        LOLUNIT_ASSERT_EQUAL(b, c);

        quat d(2.f, 3.f, -4.f, -1.f);

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
        quat a = quat::rotate(10.f, vec3(1, 0, 0));
        quat b = quat::rotate(20.f, vec3(1, 0, 0));
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
};

} /* namespace lol */

