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
        quat a4(1.0f, 2.0f, 3.0f, 4.0f);
        quat b4(0.0f, 2.0f, 3.0f, 4.0f);
        quat c4(1.0f, 0.0f, 3.0f, 4.0f);
        quat d4(1.0f, 2.0f, 0.0f, 4.0f);
        quat e4(1.0f, 2.0f, 3.0f, 0.0f);

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
        quat a(1.0f, 3.0f, 2.0f, 4.0f);
        quat b(-1.0f, -3.0f, -2.0f, -4.0f);

        LOLUNIT_ASSERT_EQUAL(a, -b);
        LOLUNIT_ASSERT_EQUAL(-a, b);
    }

    LOLUNIT_TEST(Conjugate)
    {
        quat a(1.0f, 3.0f, 2.0f, 4.0f);
        quat b(-1.0f, -3.0f, -2.0f, 4.0f);

        LOLUNIT_ASSERT_EQUAL(a, ~b);
        LOLUNIT_ASSERT_EQUAL(~a, b);
    }

    LOLUNIT_TEST(Norm)
    {
        quat a(2.0f, -2.0f, -8.0f, 3.0f);

        LOLUNIT_ASSERT_EQUAL(norm(a), 81.0f);

        quat b = a * ~a;
        quat c(0.0f, 0.0f, 0.0f, norm(a));

        LOLUNIT_ASSERT_EQUAL(b, c);

        quat d(2.0f, 3.0f, -4.0f, -1.0f);

        LOLUNIT_ASSERT_EQUAL(norm(a * d), norm(a) * norm(d));
    }

    LOLUNIT_TEST(Base)
    {
        quat i(1.0f, 0.0f, 0.0f, 0.0f);
        quat j(0.0f, 1.0f, 0.0f, 0.0f);
        quat k(0.0f, 0.0f, 1.0f, 0.0f);
        quat one(0.0f, 0.0f, 0.0f, 1.0f);

        LOLUNIT_ASSERT_EQUAL(norm(i), 1.0f);
        LOLUNIT_ASSERT_EQUAL(norm(j), 1.0f);
        LOLUNIT_ASSERT_EQUAL(norm(k), 1.0f);
        LOLUNIT_ASSERT_EQUAL(norm(one), 1.0f);

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
        quat a(2.0f, -2.0f, -8.0f, 3.0f);
        quat b = normalize(a);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(b), 1.0, 1e-8);
    }

    LOLUNIT_TEST(Reciprocal)
    {
        quat a(2.0f, -2.0f, -8.0f, 3.0f);
        quat b = re(a);

        quat m1 = a * b;
        quat m2 = b * a;
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.x, m2.x, 1e-8);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.y, m2.y, 1e-8);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.z, m2.z, 1e-8);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.w, m2.w, 1e-8);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.x, 0.0, 1e-8);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.y, 0.0, 1e-8);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.z, 0.0, 1e-8);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(m1.w, 1.0, 1e-8);
    }
};

} /* namespace lol */

