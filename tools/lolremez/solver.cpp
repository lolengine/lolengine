//
//  LolRemez - Remez algorithm implementation
//
//  Copyright © 2005—2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <functional>

#include <lol/engine.h>

#include <lol/math/real.h>
#include <lol/math/polynomial.h>

#include "matrix.h"
#include "solver.h"

using lol::real;

remez_solver::remez_solver(int order, int decimals)
  : m_order(order),
    m_decimals(decimals),
    m_has_weight(false)
{
}

void remez_solver::run(real a, real b, char const *func, char const *weight)
{
    m_func.parse(func);

    if (weight)
    {
        m_weight.parse(weight);
        m_has_weight = true;
    }

    m_k1 = (b + a) / 2;
    m_k2 = (b - a) / 2;
    m_epsilon = pow((real)10, (real)-(m_decimals + 2));

    remez_init();
    print_poly();

    for (int n = 0; ; n++)
    {
        real old_error = m_error;

        find_extrema();
        remez_step();

        if (m_error >= (real)0
             && fabs(m_error - old_error) < m_error * m_epsilon)
            break;

        print_poly();
        find_zeroes();
    }

    print_poly();
}

/*
 * This is basically the first Remez step: we solve a system of
 * order N+1 and get a good initial polynomial estimate.
 */
void remez_solver::remez_init()
{
    /* m_order + 1 zeroes of the error function */
    m_zeroes.Resize(m_order + 1);

    /* m_order + 2 control points */
    m_control.Resize(m_order + 2);

    /* Initial estimates for the x_i where the error will be zero and
     * precompute f(x_i). */
    array<real> fxn;
    for (int i = 0; i < m_order + 1; i++)
    {
        m_zeroes[i] = (real)(2 * i - m_order) / (real)(m_order + 1);
        fxn.Push(eval_func(m_zeroes[i]));
    }

    /* We build a matrix of Chebyshev evaluations: row i contains the
     * evaluations of x_i for polynomial order n = 0, 1, ... */
    linear_system<real> system(m_order + 1);
    for (int n = 0; n < m_order + 1; n++)
    {
        auto p = polynomial<real>::chebyshev(n);
        for (int i = 0; i < m_order + 1; i++)
            system[i][n] = p.eval(m_zeroes[i]);
    }

    /* Solve the system */
    system = system.inverse();

    /* Compute new Chebyshev estimate */
    m_estimate = polynomial<real>();
    for (int n = 0; n < m_order + 1; n++)
    {
        real weight = 0;
        for (int i = 0; i < m_order + 1; i++)
            weight += system[n][i] * fxn[i];

        m_estimate += weight * polynomial<real>::chebyshev(n);
    }
}

/*
 * Every subsequent iteration of the Remez algorithm: we solve a system
 * of order N+2 to both refine the estimate and compute the error.
 */
void remez_solver::remez_step()
{
    Timer t;

    /* Pick up x_i where error will be 0 and compute f(x_i) */
    array<real> fxn;
    for (int i = 0; i < m_order + 2; i++)
        fxn.Push(eval_func(m_control[i]));

    /* We build a matrix of Chebyshev evaluations: row i contains the
     * evaluations of x_i for polynomial order n = 0, 1, ... */
    linear_system<real> system(m_order + 2);
    for (int n = 0; n < m_order + 1; n++)
    {
        auto p = polynomial<real>::chebyshev(n);
        for (int i = 0; i < m_order + 2; i++)
            system[i][n] = p.eval(m_control[i]);
    }

    /* The last line of the system is the oscillating error */
    for (int i = 0; i < m_order + 2; i++)
    {
        real error = fabs(eval_weight(m_control[i]));
        system[i][m_order + 1] = (i & 1) ? error : -error;
    }

    /* Solve the system */
    system = system.inverse();

    /* Compute new polynomial estimate */
    m_estimate = polynomial<real>();
    for (int n = 0; n < m_order + 1; n++)
    {
        real weight = 0;
        for (int i = 0; i < m_order + 2; i++)
            weight += system[n][i] * fxn[i];

        m_estimate += weight * polynomial<real>::chebyshev(n);
    }

    /* Compute the error (FIXME: unused?) */
    real error = 0;
    for (int i = 0; i < m_order + 2; i++)
        error += system[m_order + 1][i] * fxn[i];

    using std::printf;
    printf(" -:- timing for inversion: %f ms\n", t.Get() * 1000.f);
}

/*
 * Find m_order + 1 zeroes of the error function. No need to compute the
 * relative error: its zeroes are at the same place as the absolute error!
 *
 * The algorithm used here is naïve regula falsi. It still performs a lot
 * better than the midpoint algorithm.
 */
void remez_solver::find_zeroes()
{
    Timer t;

    for (int i = 0; i < m_order + 1; i++)
    {
        struct { real x, err; } a, b, c;

        a.x = m_control[i];
        a.err = eval_estimate(a.x) - eval_func(a.x);
        b.x = m_control[i + 1];
        b.err = eval_estimate(b.x) - eval_func(b.x);

        static real limit = ldexp((real)1, -500);
        static real zero = (real)0;
        while (fabs(a.x - b.x) > limit)
        {
            real s = abs(b.err) / (abs(a.err) + abs(b.err));
            real newc = b.x + s * (a.x - b.x);

            /* If the third point didn't change since last iteration,
             * we may be at an inflection point. Use the midpoint to get
             * out of this situation. */
            c.x = newc == c.x ? (a.x + b.x) / 2 : newc;
            c.err = eval_estimate(c.x) - eval_func(c.x);

            if (c.err == zero)
                break;

            if ((a.err < zero && c.err < zero)
                 || (a.err > zero && c.err > zero))
                a = c;
            else
                b = c;
        }

        m_zeroes[i] = c.x;
    }

    using std::printf;
    printf(" -:- timing for zeroes: %f ms\n", t.Get() * 1000.f);
}

/*
 * Find m_order extrema of the error function. We maximise the relative
 * error, since its extrema are at slightly different locations than the
 * absolute error’s.
 *
 * The algorithm used here is successive parabolic interpolation. FIXME: we
 * could use Brent’s method instead, which combines parabolic interpolation
 * and golden ratio search and has superlinear convergence.
 */
void remez_solver::find_extrema()
{
    Timer t;

    m_control[0] = -1;
    m_control[m_order + 1] = 1;
    m_error = 0;

    for (int i = 1; i < m_order + 1; i++)
    {
        struct { real x, err; } a, b, c, d;

        a.x = m_zeroes[i - 1];
        b.x = m_zeroes[i];
        c.x = a.x + (b.x - a.x) * real(rand(0.4f, 0.6f));

        a.err = eval_error(a.x);
        b.err = eval_error(b.x);
        c.err = eval_error(c.x);

        while (b.x - a.x > m_epsilon)
        {
            real d1 = c.x - a.x, d2 = c.x - b.x;
            real k1 = d1 * (c.err - b.err);
            real k2 = d2 * (c.err - a.err);
            d.x = c.x - (d1 * k1 - d2 * k2) / (k1 - k2) / 2;

            /* If parabolic interpolation failed, pick a number
             * inbetween. */
            if (d.x <= a.x || d.x >= b.x)
                d.x = (a.x + b.x) / 2;

            d.err = eval_error(d.x);

            /* Update bracketing depending on the new point. */
            if (d.err < c.err)
            {
                (d.x > c.x ? b : a) = d;
            }
            else
            {
                (d.x > c.x ? a : b) = c;
                c = d;
            }
        }

        if (c.err > m_error)
            m_error = c.err;

        m_control[i] = c.x;
    }

    using std::printf;
    printf(" -:- timing for extrema: %f ms\n", t.Get() * 1000.f);
    printf(" -:- error: ");
    m_error.print(m_decimals);
    printf("\n");
}

void remez_solver::print_poly()
{
    /* Transform our polynomial in the [-1..1] range into a polynomial
     * in the [a..b] range by composing it with q:
     *  q(x) = 2x / (b-a) - (b+a) / (b-a) */
    polynomial<real> q ({ -m_k1 / m_k2, real(1) / m_k2 });
    polynomial<real> r = m_estimate.eval(q);

    using std::printf;
    printf("\n");
    for (int j = 0; j < m_order + 1; j++)
    {
        if (j)
            printf(" + x**%i * ", j);
        r[j].print(m_decimals);
    }
    printf("\n\n");
}

real remez_solver::eval_estimate(real const &x)
{
    return m_estimate.eval(x);
}

real remez_solver::eval_func(real const &x)
{
    return m_func.eval(x * m_k2 + m_k1);
}

real remez_solver::eval_weight(real const &x)
{
    return m_has_weight ? m_weight.eval(x * m_k2 + m_k1) : real(1);
}

real remez_solver::eval_error(real const &x)
{
    return fabs((eval_estimate(x) - eval_func(x)) / eval_weight(x));
}

