//
// LolRemez - Remez algorithm implementation
//
// Copyright: (c) 2005-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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

RemezSolver::RemezSolver(int order, int decimals)
  : m_order(order),
    m_decimals(decimals)
{
}

void RemezSolver::Run(real a, real b,
                      RemezSolver::RealFunc *func,
                      RemezSolver::RealFunc *weight)
{
    using std::printf;

    m_func = func;
    m_weight = weight;
    m_k1 = (b + a) / 2;
    m_k2 = (b - a) / 2;
    m_epsilon = pow((real)10, (real)-(m_decimals + 2));

    Init();

    PrintPoly();

    real error = -1;

    for (int n = 0; ; n++)
    {
        real newerror = FindExtrema();
        printf("Step %i error: ", n);
        newerror.print(m_decimals);
        printf("\n");

        Step();

        if (error >= (real)0 && fabs(newerror - error) < error * m_epsilon)
            break;
        error = newerror;

        PrintPoly();

        FindZeroes();
    }

    PrintPoly();
}

void RemezSolver::Init()
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
        fxn.Push(EvalFunc(m_zeroes[i]));
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

void RemezSolver::FindZeroes()
{
    /* Find m_order + 1 zeroes of the error function. No need to
     * compute the relative error: its zeroes are at the same
     * place as the absolute error! */
    for (int i = 0; i < m_order + 1; i++)
    {
        struct { real value, error; } left, right, mid;

        left.value = m_control[i];
        left.error = EvalCheby(left.value) - EvalFunc(left.value);
        right.value = m_control[i + 1];
        right.error = EvalCheby(right.value) - EvalFunc(right.value);

        static real limit = ldexp((real)1, -500);
        static real zero = (real)0;
        while (fabs(left.value - right.value) > limit)
        {
            mid.value = (left.value + right.value) / 2;
            mid.error = EvalCheby(mid.value) - EvalFunc(mid.value);

            if ((left.error <= zero && mid.error <= zero)
                 || (left.error >= zero && mid.error >= zero))
                left = mid;
            else
                right = mid;
        }

        m_zeroes[i] = mid.value;
    }
}

/* XXX: this is the real costly function */
real RemezSolver::FindExtrema()
{
    using std::printf;

    /* Find m_order + 2 extrema of the error function. We need to
     * compute the relative error, since its extrema are at slightly
     * different locations than the absolute error’s. */
    real final = 0;

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
                    real error = fabs(EvalCheby(c) - EvalFunc(c));
                    real weight = fabs(EvalWeight(c));
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
                if (e > final)
                    final = e;
                m_control[i] = (a + b) / 2;
                break;
            }
        }
    }

    printf("Iterations: Rounds %d Evals %d\n", rounds, evals);
    printf("Times: Cheby %f Func %f EvalWeight %f\n",
           m_stats_cheby, m_stats_func, m_stats_weight);

    return final;
}

void RemezSolver::Step()
{
    /* Pick up x_i where error will be 0 and compute f(x_i) */
    array<real> fxn;
    for (int i = 0; i < m_order + 2; i++)
        fxn.Push(EvalFunc(m_control[i]));

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
        real error = fabs(EvalWeight(m_control[i]));
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

    /* Compute the error */
    real error = 0;
    for (int i = 0; i < m_order + 2; i++)
        error += system[m_order + 1][i] * fxn[i];
}

void RemezSolver::PrintPoly()
{
    using std::printf;

    /* Transform our polynomial in the [-1..1] range into a polynomial
     * in the [a..b] range by composing it with q:
     *  q(x) = 2x / (b-a) - (b+a) / (b-a) */
    polynomial<real> q ({ -m_k1 / m_k2, real(1) / m_k2 });
    polynomial<real> r = m_estimate.eval(q);

    printf("Polynomial estimate: ");
    for (int j = 0; j < m_order + 1; j++)
    {
        if (j)
            printf(" + x**%i * ", j);
        r[j].print(m_decimals);
    }
    printf("\n\n");
}

real RemezSolver::EvalCheby(real const &x)
{
    Timer t;
    real ret = m_estimate.eval(x);
    m_stats_cheby += t.Get();
    return ret;
}

real RemezSolver::EvalFunc(real const &x)
{
    Timer t;
    real ret = m_func(x * m_k2 + m_k1);
    m_stats_func += t.Get();
    return ret;
}

real RemezSolver::EvalWeight(real const &x)
{
    Timer t;
    real ret = m_weight ? m_weight(x * m_k2 + m_k1) : real(1);
    m_stats_weight += t.Get();
    return ret;
}

