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

lolunit_declare_fixture(ComplexTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(Equality)
    {
        cmplx a2(1.0f, 2.0f);
        cmplx b2(0.0f, 2.0f);
        cmplx c2(1.0f, 0.0f);

        lolunit_assert_equal(a2, a2);
        lolunit_assert_not_different(a2, a2);

        lolunit_assert_different(a2, b2);
        lolunit_assert_not_equal(a2, b2);
        lolunit_assert_different(a2, c2);
        lolunit_assert_not_equal(a2, c2);
    }

    lolunit_declare_test(UnaryMinus)
    {
        cmplx a(1.0f, 3.0f);
        cmplx b(-1.0f, -3.0f);

        lolunit_assert_equal(a, -b);
        lolunit_assert_equal(-a, b);
    }

    lolunit_declare_test(Conjugate)
    {
        cmplx a(1.0f, 3.0f);
        cmplx b(1.0f, -3.0f);

        lolunit_assert_equal(a, ~b);
        lolunit_assert_equal(~a, b);
    }

    lolunit_declare_test(Norm)
    {
        cmplx a(3.0f, -4.0f);

        lolunit_assert_equal(norm(a), 5.0f);

        cmplx b = a * ~a;
        cmplx c = norm(a) * norm(a);

        lolunit_assert_equal(b, c);

        cmplx d(5.0f, 12.0f);

        lolunit_assert_equal(norm(d), 13.0f);
        lolunit_assert_equal(norm(a * d), norm(a) * norm(d));
    }

    lolunit_declare_test(Base)
    {
        cmplx one(1.0f, 0.0f);
        cmplx i(0.0f, 1.0f);

        lolunit_assert_equal(norm(one), 1.0f);
        lolunit_assert_equal(norm(i), 1.0f);

        lolunit_assert_equal(i * i, -one);
    }

    lolunit_declare_test(Normalize)
    {
        cmplx a(3.0f, -4.0f);
        cmplx b = normalize(a);

        lolunit_assert_doubles_equal(norm(b), 1.0, 1e-8);
    }

    lolunit_declare_test(Reciprocal)
    {
        cmplx a(3.0f, -4.0f);
        cmplx b = re(a);

        lolunit_assert_equal(a * b, b * a);

        cmplx c = 1.0f;

        lolunit_assert_equal(a * b, c);
    }
};

} /* namespace lol */

