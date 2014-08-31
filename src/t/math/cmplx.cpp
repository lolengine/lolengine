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

#include <lol/main.h>
#include <lolunit.h>

namespace lol
{

LOLUNIT_FIXTURE(ComplexTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(Equality)
    {
        cmplx a2(1.0f, 2.0f);
        cmplx b2(0.0f, 2.0f);
        cmplx c2(1.0f, 0.0f);

        LOLUNIT_ASSERT_EQUAL(a2, a2);
        LOLUNIT_ASSERT_NOT_DIFFERENT(a2, a2);

        LOLUNIT_ASSERT_DIFFERENT(a2, b2);
        LOLUNIT_ASSERT_NOT_EQUAL(a2, b2);
        LOLUNIT_ASSERT_DIFFERENT(a2, c2);
        LOLUNIT_ASSERT_NOT_EQUAL(a2, c2);
    }

    LOLUNIT_TEST(UnaryMinus)
    {
        cmplx a(1.0f, 3.0f);
        cmplx b(-1.0f, -3.0f);

        LOLUNIT_ASSERT_EQUAL(a, -b);
        LOLUNIT_ASSERT_EQUAL(-a, b);
    }

    LOLUNIT_TEST(Conjugate)
    {
        cmplx a(1.0f, 3.0f);
        cmplx b(1.0f, -3.0f);

        LOLUNIT_ASSERT_EQUAL(a, ~b);
        LOLUNIT_ASSERT_EQUAL(~a, b);
    }

    LOLUNIT_TEST(Norm)
    {
        cmplx a(3.0f, -4.0f);

        LOLUNIT_ASSERT_EQUAL(norm(a), 5.0f);

        cmplx b = a * ~a;
        cmplx c = norm(a) * norm(a);

        LOLUNIT_ASSERT_EQUAL(b, c);

        cmplx d(5.0f, 12.0f);

        LOLUNIT_ASSERT_EQUAL(norm(d), 13.0f);
        LOLUNIT_ASSERT_EQUAL(norm(a * d), norm(a) * norm(d));
    }

    LOLUNIT_TEST(Base)
    {
        cmplx one(1.0f, 0.0f);
        cmplx i(0.0f, 1.0f);

        LOLUNIT_ASSERT_EQUAL(norm(one), 1.0f);
        LOLUNIT_ASSERT_EQUAL(norm(i), 1.0f);

        LOLUNIT_ASSERT_EQUAL(i * i, -one);
    }

    LOLUNIT_TEST(Normalize)
    {
        cmplx a(3.0f, -4.0f);
        cmplx b = normalize(a);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(norm(b), 1.0, 1e-8);
    }

    LOLUNIT_TEST(Reciprocal)
    {
        cmplx a(3.0f, -4.0f);
        cmplx b = re(a);

        LOLUNIT_ASSERT_EQUAL(a * b, b * a);

        cmplx c = 1.0f;

        LOLUNIT_ASSERT_EQUAL(a * b, c);
    }
};

} /* namespace lol */

