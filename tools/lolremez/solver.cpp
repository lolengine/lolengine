//
// LolRemez - Remez algorithm implementation
//
// Copyright: (c) 2005-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

#include <lol/math/real.h>

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
    m_invk2 = re(m_k2);
    m_invk1 = -m_k1 * m_invk2;
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

real RemezSolver::EvalCheby(real const &x)
{
    real ret = 0.0, xn = 1.0;

    for (int i = 0; i < m_order + 1; i++)
    {
        real mul = 0;
        for (int j = 0; j < m_order + 1; j++)
            mul += m_coeff[j] * (real)Cheby(j, i);
        ret += mul * xn;
        xn *= x;
    }

    return ret;
}

void RemezSolver::Init()
{
    /* m_order + 1 Chebyshev coefficients, plus 1 error value */
    m_coeff.Resize(m_order + 2);

    /* m_order + 1 zeroes of the error function */
    m_zeroes.Resize(m_order + 1);

    /* m_order + 2 control points */
    m_control.Resize(m_order + 2);

    /* Pick up x_i where error will be 0 and compute f(x_i) */
    array<real> fxn;
    for (int i = 0; i < m_order + 1; i++)
    {
        m_zeroes[i] = (real)(2 * i - m_order) / (real)(m_order + 1);
        fxn.Push(EvalFunc(m_zeroes[i]));
    }

    /* We build a matrix of Chebishev evaluations: row i contains the
     * evaluations of x_i for polynomial order n = 0, 1, ... */
    Matrix<real> mat(m_order + 1, m_order + 1);
    for (int i = 0; i < m_order + 1; i++)
    {
        /* Compute the powers of x_i */
        array<real> powers;
        powers.Push(real(1.0));
        for (int n = 1; n < m_order + 1; n++)
             powers.Push(powers.Last() * m_zeroes[i]);

        /* Compute the Chebishev evaluations at x_i */
        for (int n = 0; n < m_order + 1; n++)
        {
            real sum = 0.0;
            for (int k = 0; k < m_order + 1; k++)
                sum += (real)Cheby(n, k) * powers[k];
            mat.m(i, n) = sum;
        }
    }

    /* Solve the system */
    mat = mat.inv();

    /* Compute interpolation coefficients */
    for (int j = 0; j < m_order + 1; j++)
    {
        m_coeff[j] = 0;
        for (int i = 0; i < m_order + 1; i++)
            m_coeff[j] += mat.m(j, i) * fxn[i];
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

real RemezSolver::FindExtrema()
{
    using std::printf;

    /* Find m_order + 2 extrema of the error function. We need to
     * compute the relative error, since its extrema are at slightly
     * different locations than the absolute error’s. */
    real final = 0;

    for (int i = 0; i < m_order + 2; i++)
    {
        real a = -1, b = 1;
        if (i > 0)
            a = m_zeroes[i - 1];
        if (i < m_order + 1)
            b = m_zeroes[i];

        for (int round = 0; ; round++)
        {
            real maxerror = 0, maxweight = 0;
            int best = -1;

            real c = a, delta = (b - a) / 4;
            for (int k = 0; k <= 4; k++)
            {
                if (round == 0 || (k & 1))
                {
                    real error = fabs(EvalCheby(c) - EvalFunc(c));
                    real weight = fabs(Weight(c));
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
    Matrix<real> mat(m_order + 2, m_order + 2);
    for (int i = 0; i < m_order + 2; i++)
    {
        /* Compute the powers of x_i */
        array<real> powers;
        powers.Push(real(1.0));
        for (int n = 1; n < m_order + 1; n++)
             powers.Push(powers.Last() * m_control[i]);

        /* Compute the Chebishev evaluations at x_i */
        for (int n = 0; n < m_order + 1; n++)
        {
            real sum = 0.0;
            for (int k = 0; k < m_order + 1; k++)
                sum += (real)Cheby(n, k) * powers[k];
            mat.m(i, n) = sum;
        }
        if (i & 1)
            mat.m(i, m_order + 1) = fabs(Weight(m_control[i]));
        else
            mat.m(i, m_order + 1) = -fabs(Weight(m_control[i]));
    }

    /* Solve the system */
    mat = mat.inv();

    /* Compute interpolation coefficients */
    for (int j = 0; j < m_order + 1; j++)
    {
        m_coeff[j] = 0;
        for (int i = 0; i < m_order + 2; i++)
            m_coeff[j] += mat.m(j, i) * fxn[i];
    }

    /* Compute the error */
    real error = 0;
    for (int i = 0; i < m_order + 2; i++)
        error += mat.m(m_order + 1, i) * fxn[i];
}

int RemezSolver::Cheby(int n, int k)
{
    if (k > n || k < 0)
        return 0;
    if (n <= 1)
        return (n ^ k ^ 1) & 1;
    return 2 * Cheby(n - 1, k - 1) - Cheby(n - 2, k);
}

int RemezSolver::Comb(int n, int k)
{
    if (k == 0 || k == n)
        return 1;
    return Comb(n - 1, k - 1) + Comb(n - 1, k);
}

void RemezSolver::PrintPoly()
{
    using std::printf;

    /* Transform Chebyshev polynomial weights into powers of X^i
     * in the [-1..1] range. */
    array<real> bn;

    for (int i = 0; i < m_order + 1; i++)
    {
        real tmp = 0;
        for (int j = 0; j < m_order + 1; j++)
            tmp += m_coeff[j] * (real)Cheby(j, i);
        bn.Push(tmp);
    }

    /* Transform a polynomial in the [-1..1] range into a polynomial
     * in the [a..b] range. */
    array<real> k1p, k2p, an;

    for (int i = 0; i < m_order + 1; i++)
    {
        k1p.Push(i ? k1p[i - 1] * m_invk1 : (real)1);
        k2p.Push(i ? k2p[i - 1] * m_invk2 : (real)1);
    }

    for (int i = 0; i < m_order + 1; i++)
    {
        real tmp = 0;
        for (int j = i; j < m_order + 1; j++)
            tmp += (real)Comb(j, i) * k1p[j - i] * bn[j];
        an.Push(tmp * k2p[i]);
    }

    printf("Polynomial estimate: ");
    for (int j = 0; j < m_order + 1; j++)
    {
        if (j)
            printf(" + x**%i * ", j);
        an[j].print(m_decimals);
    }
    printf("\n\n");
}

real RemezSolver::EvalFunc(real const &x)
{
    return m_func(x * m_k2 + m_k1);
}

real RemezSolver::Weight(real const &x)
{
    if (m_weight)
        return m_weight(x * m_k2 + m_k1);
    return 1;
}

