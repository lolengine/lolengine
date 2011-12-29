//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Remez class
// ---------------
//

#if !defined __LOL_MATH_REMEZ_H__
#define __LOL_MATH_REMEZ_H__

namespace lol
{

template<int ORDER, typename T> class RemezSolver
{
public:
    typedef T RealFunc(T const &x);

    RemezSolver()
    {
    }

    void Run(T a, T b, RealFunc *func, RealFunc *weight, int steps)
    {
        m_func = func;
        m_weight = weight;
        m_k1 = (b + a) >> 1;
        m_k2 = (b - a) >> 1;
        m_invk2 = re(m_k2);
        m_invk1 = -m_k1 * m_invk2;

        Init();

        PrintPoly();

        for (int n = 0; n < steps; n++)
        {
            FindExtrema();
            Step();

            PrintPoly();

            FindZeroes();
        }

        FindExtrema();
        Step();

        PrintPoly();
    }

    T ChebyEval(T const &x)
    {
        T ret = 0.0, xn = 1.0;

        for (int i = 0; i < ORDER + 1; i++)
        {
            T mul = 0;
            for (int j = 0; j < ORDER + 1; j++)
                mul += coeff[j] * (T)Cheby(j, i);
            ret += mul * xn;
            xn *= x;
        }

        return ret;
    }

    void Init()
    {
        /* Pick up x_i where error will be 0 and compute f(x_i) */
        T fxn[ORDER + 1];
        for (int i = 0; i < ORDER + 1; i++)
        {
            zeroes[i] = (T)(2 * i - ORDER) / (T)(ORDER + 1);
            fxn[i] = Value(zeroes[i]);
        }

        /* We build a matrix of Chebishev evaluations: row i contains the
         * evaluations of x_i for polynomial order n = 0, 1, ... */
        lol::Mat<ORDER + 1, T> mat;
        for (int i = 0; i < ORDER + 1; i++)
        {
            /* Compute the powers of x_i */
            T powers[ORDER + 1];
            powers[0] = 1.0;
            for (int n = 1; n < ORDER + 1; n++)
                 powers[n] = powers[n - 1] * zeroes[i];

            /* Compute the Chebishev evaluations at x_i */
            for (int n = 0; n < ORDER + 1; n++)
            {
                T sum = 0.0;
                for (int k = 0; k < ORDER + 1; k++)
                    sum += (T)Cheby(n, k) * powers[k];
                mat.m[i][n] = sum;
            }
        }

        /* Solve the system */
        mat = mat.inv();

        /* Compute interpolation coefficients */
        for (int j = 0; j < ORDER + 1; j++)
        {
            coeff[j] = 0;
            for (int i = 0; i < ORDER + 1; i++)
                coeff[j] += mat.m[j][i] * fxn[i];
        }
    }

    void FindZeroes()
    {
        /* Find ORDER + 1 zeroes of the error function. No need to
         * compute the relative error: its zeroes are at the same
         * place as the absolute error! */
        for (int i = 0; i < ORDER + 1; i++)
        {
            struct { T value, error; } left, right, mid;

            left.value = control[i];
            left.error = ChebyEval(left.value) - Value(left.value);
            right.value = control[i + 1];
            right.error = ChebyEval(right.value) - Value(right.value);

            static T limit = (T)1 >> 500;
            static T zero = (T)0;
            while (fabs(left.value - right.value) > limit)
            {
                mid.value = (left.value + right.value) >> 1;
                mid.error = ChebyEval(mid.value) - Value(mid.value);

                if ((left.error < zero && mid.error < zero)
                     || (left.error > zero && mid.error > zero))
                    left = mid;
                else
                    right = mid;
            }

            zeroes[i] = mid.value;
        }
    }

    void FindExtrema()
    {
        using std::printf;

        /* Find ORDER + 2 extrema of the error function. We need to
         * compute the relative error, since its extrema are at slightly
         * different locations than the absolute error’s. */
        T final = 0;

        for (int i = 0; i < ORDER + 2; i++)
        {
            T a = -1, b = 1;
            if (i > 0)
                a = zeroes[i - 1];
            if (i < ORDER + 1)
                b = zeroes[i];

            for (;;)
            {
                T c = a, delta = (b - a) >> 3;
                T maxerror = 0;
                T maxweight = 0;
                int best = -1;
                for (int k = 1; k <= 7; k++)
                {
                    T error = ChebyEval(c) - Value(c);
                    T weight = Weight(c);
                    if (fabs(error * maxweight) >= fabs(maxerror * weight))
                    {
                        maxerror = error;
                        maxweight = weight;
                        best = k;
                    }
                    c += delta;
                }

                b = a + (T)(best + 1) * delta;
                a = a + (T)(best - 1) * delta;

                if (b - a < (T)1e-18)
                {
                    T e = maxerror / maxweight;
                    if (e > final)
                        final = e;
                    control[i] = (a + b) >> 1;
                    break;
                }
            }
        }

        printf("Final error: ");
        final.print(40);
    }

    void Step()
    {
        /* Pick up x_i where error will be 0 and compute f(x_i) */
        T fxn[ORDER + 2];
        for (int i = 0; i < ORDER + 2; i++)
            fxn[i] = Value(control[i]);

        /* We build a matrix of Chebishev evaluations: row i contains the
         * evaluations of x_i for polynomial order n = 0, 1, ... */
        lol::Mat<ORDER + 2, T> mat;
        for (int i = 0; i < ORDER + 2; i++)
        {
            /* Compute the powers of x_i */
            T powers[ORDER + 1];
            powers[0] = 1.0;
            for (int n = 1; n < ORDER + 1; n++)
                 powers[n] = powers[n - 1] * control[i];

            /* Compute the Chebishev evaluations at x_i */
            for (int n = 0; n < ORDER + 1; n++)
            {
                T sum = 0.0;
                for (int k = 0; k < ORDER + 1; k++)
                    sum += (T)Cheby(n, k) * powers[k];
                mat.m[i][n] = sum;
            }
            if (i & 1)
                mat.m[i][ORDER + 1] = fabs(Weight(control[i]));
            else
                mat.m[i][ORDER + 1] = -fabs(Weight(control[i]));
        }

        /* Solve the system */
        mat = mat.inv();

        /* Compute interpolation coefficients */
        for (int j = 0; j < ORDER + 1; j++)
        {
            coeff[j] = 0;
            for (int i = 0; i < ORDER + 2; i++)
                coeff[j] += mat.m[j][i] * fxn[i];
        }

        /* Compute the error */
        T error = 0;
        for (int i = 0; i < ORDER + 2; i++)
            error += mat.m[ORDER + 1][i] * fxn[i];
    }

    int Cheby(int n, int k)
    {
        if (k > n || k < 0)
            return 0;
        if (n <= 1)
            return (n ^ k ^ 1) & 1;
        return 2 * Cheby(n - 1, k - 1) - Cheby(n - 2, k);
    }

    int Comb(int n, int k)
    {
        if (k == 0 || k == n)
            return 1;
        return Comb(n - 1, k - 1) + Comb(n - 1, k);
    }

    void PrintPoly()
    {
        using std::printf;

        /* Transform Chebyshev polynomial weights into powers of X^i
         * in the [-1..1] range. */
        T bn[ORDER + 1];

        for (int i = 0; i < ORDER + 1; i++)
        {
            bn[i] = 0;
            for (int j = 0; j < ORDER + 1; j++)
                bn[i] += coeff[j] * (T)Cheby(j, i);
        }

        /* Transform a polynomial in the [-1..1] range into a polynomial
         * in the [a..b] range. */
        T k1p[ORDER + 1], k2p[ORDER + 1];
        T an[ORDER + 1];

        for (int i = 0; i < ORDER + 1; i++)
        {
            k1p[i] = i ? k1p[i - 1] * m_invk1 : (T)1;
            k2p[i] = i ? k2p[i - 1] * m_invk2 : (T)1;
        }

        for (int i = 0; i < ORDER + 1; i++)
        {
            an[i] = 0;
            for (int j = i; j < ORDER + 1; j++)
                an[i] += (T)Comb(j, i) * k1p[j - i] * bn[j];
            an[i] *= k2p[i];
        }

        printf("Polynomial estimate:\n");
        for (int j = 0; j < ORDER + 1; j++)
        {
            if (j)
                printf("+");
            printf("x**%i*", j);
            an[j].print(40);
        }
        printf("\n");
    }

    T Value(T const &x)
    {
        return m_func(x * m_k2 + m_k1);
    }

    T Weight(T const &x)
    {
        return m_weight(x * m_k2 + m_k1);
    }

    /* ORDER + 1 Chebyshev coefficients and 1 error value */
    T coeff[ORDER + 2];
    /* ORDER + 1 zeroes of the error function */
    T zeroes[ORDER + 1];
    /* ORDER + 2 control points */
    T control[ORDER + 2];

private:
    RealFunc *m_func, *m_weight;
    T m_k1, m_k2, m_invk1, m_invk2;
};

} /* namespace lol */

#endif /* __LOL_MATH_REMEZ_H__ */

