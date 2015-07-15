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

lolunit_declare_fixture(polynomial_test)
{
    lolunit_declare_test(declaration)
    {
        polynomial<float> p;
        polynomial<real> q;
    }

    lolunit_declare_test(init)
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

    lolunit_declare_test(derive)
    {
        polynomial<float> p {};
        p = p.derive();
        lolunit_assert_equal(p.degree(), -1);

        polynomial<float> q { 1.f };
        q = q.derive();
        lolunit_assert_equal(q.degree(), -1);

        polynomial<float> r { 1.f, 2.f };
        r = r.derive();
        lolunit_assert_equal(r.degree(), 0);
        lolunit_assert_equal(r[0], 2.f);

        polynomial<float> s { 1.f, 2.f, 3.f, 4.f };
        s = s.derive();
        lolunit_assert_equal(s.degree(), 2);
        lolunit_assert_equal(s[0], 2.f);
        lolunit_assert_equal(s[1], 6.f);
        lolunit_assert_equal(s[2], 12.f);
    }

    lolunit_declare_test(eval)
    {
        /* Special null polynomial */
        polynomial<float> p;

        float a = p.eval(42.f);
        lolunit_assert_equal(a, 0.f);
    }

    lolunit_declare_test(eval_degree_0)
    {
        /* Constant polynomial p(x) = 1 */
        polynomial<float> p { 1.f };

        float a = p.eval(42.f);
        lolunit_assert_equal(a, 1.f);
    }

    lolunit_declare_test(eval_degree_1)
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

    lolunit_declare_test(eval_degree_2)
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

    lolunit_declare_test(unary_plus_and_minus)
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

    lolunit_declare_test(addition)
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

    lolunit_declare_test(subtraction)
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

    lolunit_declare_test(multiplication)
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

    lolunit_declare_test(division)
    {
        /* p(x) = -4 - 2x² + x³ */
        /* q(x) = -3 + x */
        polynomial<float> p { -4.f, 0.f, -2.f, 1.f };
        polynomial<float> q { -3.f, 1.f };

        /* p(x) = r(x) q(x) + s(x)
         * r(x) = 3 + x + x²
         * s(x) = 5 */
        auto r = p / q;
        lolunit_assert_equal(r.m1.degree(), 2);
        lolunit_assert_doubles_equal(r.m1[0], 3.f, 1e-5f);
        lolunit_assert_doubles_equal(r.m1[1], 1.f, 1e-5f);
        lolunit_assert_doubles_equal(r.m1[2], 1.f, 1e-5f);
        lolunit_assert_equal(r.m2.degree(), 0);
        lolunit_assert_doubles_equal(r.m2[0], 5.f, 1e-5f);
    }

    lolunit_declare_test(composition_degree_2_2)
    {
        /* p(x) = 1 + x² */
        polynomial<float> p({ 1, 0, 1 });

        /* q(x) = (p o p)(x) = 2 + 2x² + x⁴ */
        polynomial<float> q = p.eval(p);
        lolunit_assert_equal(q.degree(), 4);
        lolunit_assert_equal(q[0], 2.f);
        lolunit_assert_equal(q[1], 0.f);
        lolunit_assert_equal(q[2], 2.f);
        lolunit_assert_equal(q[3], 0.f);
        lolunit_assert_equal(q[4], 1.f);
    }

    lolunit_declare_test(composition_degree_2_3)
    {
        /* p(x) = 1 + x */
        polynomial<float> p({ 1, 1 });

        /* q(x) = 1 + x + x² */
        polynomial<float> q({ 1, 1, 1 });

        /* r(x) = (q o p)(x) = 3 + 3x + x² */
        polynomial<float> r = q.eval(p);
        lolunit_assert_equal(r.degree(), 2);
        lolunit_assert_equal(r[0], 3.f);
        lolunit_assert_equal(r[1], 3.f);
        lolunit_assert_equal(r[2], 1.f);
    }

    lolunit_declare_test(degree_0_root)
    {
        /* p(x) = 42 */
        polynomial<float> p { 42.f };
        auto roots = p.roots();

        lolunit_assert_equal(roots.count(), 0);
    }

    lolunit_declare_test(degree_1_root)
    {
        /* p(x) = -6 + 2x */
        polynomial<float> p { -6.f, 2.f };
        auto roots = p.roots();

        lolunit_assert_equal(roots.count(), 1);
        lolunit_assert_equal(roots[0], 3.f);
    }

    lolunit_declare_test(degree_2_root)
    {
        /* p(x) = 81 - 18x + x² */
        polynomial<float> p { 81.f, -18.f, 1.f };
        auto roots1 = p.roots();

        lolunit_assert_equal(roots1.count(), 1);
        lolunit_assert_equal(roots1[0], 9.f);

        /* p(x) = 42 - 20x + 2x² */
        polynomial<float> q { 42.f, -20.f, 2.f };
        auto roots2 = q.roots();

        lolunit_assert_equal(roots2.count(), 2);
        lolunit_assert_equal(roots2[0], 3.f);
        lolunit_assert_equal(roots2[1], 7.f);
    }

    lolunit_declare_test(degree_3_triple_root)
    {
        polynomial<float> p { 1.f, 3.f, 3.f, 1.f };
        auto roots1 = p.roots();

        lolunit_assert_equal(roots1.count(), 1);
        lolunit_assert_doubles_equal(roots1[0], -1, 0);
    }

    lolunit_declare_test(degree_3_double_root)
    {
        polynomial<float> p { 2.f, 5.f, 4.f, 1.f };
        auto roots1 = p.roots();

        // Should have 2 solutions only, but precision leads to 3 solutions
        lolunit_assert_equal(roots1.count(), 2);
        lolunit_assert_doubles_equal(roots1[0], -1, 1e-6);
        lolunit_assert_doubles_equal(roots1[1], -2, 1e-6);
    }

    lolunit_declare_test(degree_3_three_roots)
    {
        polynomial<float> p { 6.f, 11.f, 6.f, 1.f };
        auto roots1 = p.roots();

        lolunit_assert_equal(roots1.count(), 3);
        lolunit_assert_doubles_equal(roots1[0], -1, 1e-8);
        lolunit_assert_doubles_equal(roots1[1], -3, 1e-8);
        lolunit_assert_doubles_equal(roots1[2], -2, 1e-8);
    }

    lolunit_declare_test(degree_3_three_large_roots)
    {
        polynomial<float> p { -12000.f, 1000.f - 1200.f - 120.f, 100.f + 10.0f - 12.f, 1.f };
        auto roots1 = p.roots();

        lolunit_assert_equal(roots1.count(), 3);

        lolunit_assert_doubles_equal(roots1[0], 12, 1e-5);
        lolunit_assert_doubles_equal(roots1[1], -100, 1e-5);
        lolunit_assert_doubles_equal(roots1[2], -10, 1e-5);
    }

    lolunit_declare_test(chebyshev)
    {
        polynomial<float> t0 = polynomial<float>::chebyshev(0);
        polynomial<float> t1 = polynomial<float>::chebyshev(1);
        polynomial<float> t2 = polynomial<float>::chebyshev(2);
        polynomial<float> t3 = polynomial<float>::chebyshev(3);
        polynomial<float> t4 = polynomial<float>::chebyshev(4);

        /* Taken from the sequence at http://oeis.org/A028297 */
        lolunit_assert_equal(t0.degree(), 0);
        lolunit_assert_equal(t0[0], 1.f);

        lolunit_assert_equal(t1.degree(), 1);
        lolunit_assert_equal(t1[0], 0.f);
        lolunit_assert_equal(t1[1], 1.f);

        lolunit_assert_equal(t2.degree(), 2);
        lolunit_assert_equal(t2[0], -1.f);
        lolunit_assert_equal(t2[1], 0.f);
        lolunit_assert_equal(t2[2], 2.f);

        lolunit_assert_equal(t3.degree(), 3);
        lolunit_assert_equal(t3[0], 0.f);
        lolunit_assert_equal(t3[1], -3.f);
        lolunit_assert_equal(t3[2], 0.f);
        lolunit_assert_equal(t3[3], 4.f);

        lolunit_assert_equal(t4.degree(), 4);
        lolunit_assert_equal(t4[0], 1.f);
        lolunit_assert_equal(t4[1], 0.f);
        lolunit_assert_equal(t4[2], -8.f);
        lolunit_assert_equal(t4[3], 0.f);
        lolunit_assert_equal(t4[4], 8.f);
    }
};

} /* namespace lol */

