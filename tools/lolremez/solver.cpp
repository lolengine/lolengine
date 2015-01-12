//
//  LolRemez - Remez algorithm implementation
//
//  Copyright © 2005-2015 Sam Hocevar <sam@hocevar.net>
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
    using std::printf;

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

    /* We build a matrix of Chebishev evaluations: row i contains the
     * evaluations of x_i for polynomial order n = 0, 1, ... */
    LinearSystem<real> system(m_order + 1);
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
    /* Pick up x_i where error will be 0 and compute f(x_i) */
    array<real> fxn;
    for (int i = 0; i < m_order + 2; i++)
        fxn.Push(eval_func(m_control[i]));

    /* We build a matrix of Chebishev evaluations: row i contains the
     * evaluations of x_i for polynomial order n = 0, 1, ... */
    LinearSystem<real> system(m_order + 2);
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
}

void remez_solver::find_zeroes()
{
    m_stats_cheby = m_stats_func = m_stats_weight = 0.f;

    /* Find m_order + 1 zeroes of the error function. No need to
     * compute the relative error: its zeroes are at the same
     * place as the absolute error! */
    for (int i = 0; i < m_order + 1; i++)
    {
        struct { real value, error; } a, b, c;

        a.value = m_control[i];
        a.error = eval_estimate(a.value) - eval_func(a.value);
        b.value = m_control[i + 1];
        b.error = eval_estimate(b.value) - eval_func(b.value);

        static real limit = ldexp((real)1, -500);
        static real zero = (real)0;
        while (fabs(a.value - b.value) > limit)
        {
            /* Interpolate linearly instead of taking the midpoint, this
             * leads to far better convergence (6:1 speedups). */
            real t = abs(b.error) / (abs(a.error) + abs(b.error));
            real newc = b.value + t * (a.value - b.value);

            /* If the third point didn't change since last iteration,
             * we may be at an inflection point. Use the midpoint to get
             * out of this situation. */
            c.value = newc == c.value ? (a.value + b.value) / 2 : newc;
            c.error = eval_estimate(c.value) - eval_func(c.value);

            if (c.error == zero)
                break;

            if ((a.error < zero && c.error < zero)
                 || (a.error > zero && c.error > zero))
                a = c;
            else
                b = c;
        }

        m_zeroes[i] = c.value;
    }

    printf(" -:- timings for zeroes: estimate %f func %f weight %f\n",
           m_stats_cheby, m_stats_func, m_stats_weight);
}

/* XXX: this is the real costly function */
void remez_solver::find_extrema()
{
    using std::printf;

    /* Find m_order + 2 extrema of the error function. We need to
     * compute the relative error, since its extrema are at slightly
     * different locations than the absolute error’s. */
    m_error = 0;

    m_stats_cheby = m_stats_func = m_stats_weight = 0.f;
    int evals = 0, rounds = 0;

    for (int i = 0; i < m_order + 2; i++)
    {
        real a = -1, b = 1;
        if (i > 0)
            a = m_zeroes[i - 1];
        if (i < m_order + 1)
            b = m_zeroes[i];

        for (int r = 0; ; ++r, ++rounds)
        {
            real maxerror = 0, maxweight = 0;
            int best = -1;

            real c = a, delta = (b - a) / 4;
            for (int k = 0; k <= 4; k++)
            {
                if (r == 0 || (k & 1))
                {
                    ++evals;
                    real error = fabs(eval_estimate(c) - eval_func(c));
                    real weight = fabs(eval_weight(c));
                    /* if error/weight >= maxerror/maxweight */
                    if (error * maxweight >= maxerror * weight)
                    {
                        maxerror = error;
                        maxweight = weight;
                        best = k;
                    }
                }
                c += delta;
            }

            switch (best)
            {
            case 0:
                b = a + delta * 2;
                break;
            case 4:
                a = b - delta * 2;
                break;
            default:
                b = a + delta * (best + 1);
                a = a + delta * (best - 1);
                break;
            }

            if (delta < m_epsilon)
            {
                real e = fabs(maxerror / maxweight);
                if (e > m_error)
                    m_error = e;
                m_control[i] = (a + b) / 2;
                break;
            }
        }
    }

    printf(" -:- timings for extrema: estimate %f func %f weight %f\n",
           m_stats_cheby, m_stats_func, m_stats_weight);
    printf(" -:- calls: %d rounds, %d evals\n", rounds, evals);
    printf(" -:- error: ");
    m_error.print(m_decimals);
    printf("\n");
}

void remez_solver::print_poly()
{
    using std::printf;

    /* Transform our polynomial in the [-1..1] range into a polynomial
     * in the [a..b] range by composing it with q:
     *  q(x) = 2x / (b-a) - (b+a) / (b-a) */
    polynomial<real> q ({ -m_k1 / m_k2, real(1) / m_k2 });
    polynomial<real> r = m_estimate.eval(q);

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
    Timer t;
    real ret = m_estimate.eval(x);
    m_stats_cheby += t.Get();
    return ret;
}

real remez_solver::eval_func(real const &x)
{
    Timer t;
    real ret = m_func.eval(x * m_k2 + m_k1);
    m_stats_func += t.Get();
    return ret;
}

real remez_solver::eval_weight(real const &x)
{
    Timer t;
    real ret = m_has_weight ? m_weight.eval(x * m_k2 + m_k1) : real(1);
    m_stats_weight += t.Get();
    return ret;
}

