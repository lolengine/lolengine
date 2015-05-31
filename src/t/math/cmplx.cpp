//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
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

lolunit_declare_fixture(complex_test)
{
    lolunit_declare_test(equality)
    {
        cmplx a2(1.0f, 2.0f);
        cmplx b2(0.0f, 2.0f);
        cmplx c2(1.0f, 0.0f);

        lolunit_assert_equal(a2, a2);
        lolunit_refute_different(a2, a2);

        lolunit_assert_different(a2, b2);
        lolunit_refute_equal(a2, b2);
        lolunit_assert_different(a2, c2);
        lolunit_refute_equal(a2, c2);
    }

    lolunit_declare_test(unary_minus)
    {
        cmplx a(1.0f, 3.0f);
        cmplx b(-1.0f, -3.0f);

        lolunit_assert_equal(a, -b);
        lolunit_assert_equal(-a, b);
    }

    lolunit_declare_test(conjugate)
    {
        cmplx a(1.0f, 3.0f);
        cmplx b(1.0f, -3.0f);

        lolunit_assert_equal(a, ~b);
        lolunit_assert_equal(~a, b);
    }

    lolunit_declare_test(complex_norm)
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

    lolunit_declare_test(base)
    {
        cmplx one(1.0f, 0.0f);
        cmplx i(0.0f, 1.0f);

        lolunit_assert_equal(norm(one), 1.0f);
        lolunit_assert_equal(norm(i), 1.0f);

        lolunit_assert_equal(i * i, -one);
    }

    lolunit_declare_test(complex_normalize)
    {
        cmplx a(3.0f, -4.0f);
        cmplx b = normalize(a);

        lolunit_assert_doubles_equal(norm(b), 1.0, 1e-8);
    }

    lolunit_declare_test(complex_inverse)
    {
        cmplx a(3.0f, -4.0f);
        cmplx b = inverse(a);

        lolunit_assert_equal(a * b, b * a);

        cmplx c = 1.0f;

        lolunit_assert_equal(a * b, c);
    }
};

} /* namespace lol */

