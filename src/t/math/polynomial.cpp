//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(PolynomialTest)
{
    lolunit_declare_test(Declaration)
    {
        polynomial<float> p;
        polynomial<real> q;
    }

    lolunit_declare_test(Init)
    {
        polynomial<float> p { };
        lolunit_assert_equal(p[0], 0.f);
        lolunit_assert_equal(p[1], 0.f);
        lolunit_assert_equal(p[2], 0.f);
        lolunit_assert_equal(p.degree(), -1);

        polynomial<float> q { 1.f };
        lolunit_assert_equal(q[0], 1.f);
        lolunit_assert_equal(q[1], 0.f);
        lolunit_assert_equal(q[2], 0.f);
        lolunit_assert_equal(q.degree(), 0);

        polynomial<float> r { 1.f, 2.f };
        lolunit_assert_equal(r[0], 1.f);
        lolunit_assert_equal(r[1], 2.f);
        lolunit_assert_equal(r[2], 0.f);
        lolunit_assert_equal(r.degree(), 1);

        polynomial<float> s { 0.f };
        lolunit_assert_equal(s[0], 0.f);
        lolunit_assert_equal(s[1], 0.f);
        lolunit_assert_equal(s[2], 0.f);
        lolunit_assert_equal(s.degree(), -1);
    }

    lolunit_declare_test(Eval)
    {
        /* Special null polynomial */
        polynomial<float> p;

        float a = p.eval(42.f);
        lolunit_assert_equal(a, 0.f);
    }

    lolunit_declare_test(Eval0)
    {
        /* Constant polynomial p(x) = 1 */
        polynomial<float> p { 1.f };

        float a = p.eval(42.f);
        lolunit_assert_equal(a, 1.f);
    }

    lolunit_declare_test(Eval1)
    {
        /* p(x) = 1 + 2x */
        polynomial<float> p { 1.f, 2.f };

        float a = p.eval(0.f);
        lolunit_assert_equal(a, 1.f);

        float b = p.eval(1.f);
        lolunit_assert_equal(b, 3.f);

        float c = p.eval(2.f);
        lolunit_assert_equal(c, 5.f);
    }

    lolunit_declare_test(Eval2)
    {
        /* p(x) = 1 + 2x + 3x² */
        polynomial<float> p { 1.f, 2.f, 3.f };

        float a = p.eval(0.f);
        lolunit_assert_equal(a, 1.f);

        float b = p.eval(1.f);
        lolunit_assert_equal(b, 6.f);

        float c = p.eval(2.f);
        lolunit_assert_equal(c, 17.f);
    }

    lolunit_declare_test(UnaryPlusMinus)
    {
        /* p(x) = 1 + 2x + 3x² */
        polynomial<float> p { 1.f, 2.f, 3.f };
        polynomial<float> q = +p;
        polynomial<float> r = -p;

        lolunit_assert_equal(q[0], 1.f);
        lolunit_assert_equal(q[1], 2.f);
        lolunit_assert_equal(q[2], 3.f);

        lolunit_assert_equal(r[0], -1.f);
        lolunit_assert_equal(r[1], -2.f);
        lolunit_assert_equal(r[2], -3.f);
    }

    lolunit_declare_test(Addition)
    {
        /* p(x) = 1 + 2x + 3x² */
        /* q(x) = 4 + 5x */
        polynomial<float> p { 1.f, 2.f, 3.f };
        polynomial<float> q { 4.f, 5.f };

        /* r(x) = 5 + 7x + 3x² */
        polynomial<float> r = p + q;
        lolunit_assert_equal(r.degree(), 2);
        lolunit_assert_equal(r[0], 5.f);
        lolunit_assert_equal(r[1], 7.f);
        lolunit_assert_equal(r[2], 3.f);
    }

    lolunit_declare_test(Subtraction)
    {
        /* p(x) = 1 + 2x + 3x² */
        /* q(x) = 4 + 5x */
        polynomial<float> p { 1.f, 2.f, 3.f };
        polynomial<float> q { 4.f, 5.f };

        /* r(x) = -3 + -3x + 3x² */
        polynomial<float> r = p - q;
        lolunit_assert_equal(r.degree(), 2);
        lolunit_assert_equal(r[0], -3.f);
        lolunit_assert_equal(r[1], -3.f);
        lolunit_assert_equal(r[2], 3.f);
    }

    lolunit_declare_test(Multiplication)
    {
        /* p(x) = 1 + 2x + 3x² */
        /* q(x) = 4 + 5x */
        polynomial<float> p { 1.f, 2.f, 3.f };
        polynomial<float> q { 4.f, 5.f };

        /* r(x) = 4 + 13x + 22x² + 15x³ */
        polynomial<float> r = p * q;
        lolunit_assert_equal(r.degree(), 3);
        lolunit_assert_equal(r[0], 4.f);
        lolunit_assert_equal(r[1], 13.f);
        lolunit_assert_equal(r[2], 22.f);
        lolunit_assert_equal(r[3], 15.f);
    }
};

} /* namespace lol */

