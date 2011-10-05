//
// Lol Engine - Sample math program: Chebyshev polynomials
//
// Copyright: (c) 2005-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if !defined __REMEZ_SOLVER_H__
#define __REMEZ_SOLVER_H__

template<int ORDER> class RemezSolver
{
public:
    typedef real RealFunc(real const &x);

    RemezSolver()
    {
    }

    void Run(real a, real b, RealFunc *func, RealFunc *error, int steps)
    {
        m_func = func;
        m_error = error;
        m_k1 = (b + a) >> 1;
        m_k2 = (b - a) >> 1;
        m_invk2 = re(m_k2);
        m_invk1 = -m_k1 * m_invk2;

        Init();

        PrintPoly();

        for (int n = 0; n < steps; n++)
        {
            FindError();
            Step();

            PrintPoly();

            FindZeroes();
        }

        FindError();
        Step();

        PrintPoly();
    }

    real ChebyEval(real const &x)
    {
        real ret = 0.0, xn = 1.0;

        for (int i = 0; i < ORDER + 1; i++)
        {
            real mul = 0;
            for (int j = 0; j < ORDER + 1; j++)
                mul += coeff[j] * (real)Cheby(j, i);
            ret += mul * xn;
            xn *= x;
        }

        return ret;
    }

    void Init()
    {
        /* Pick up x_i where error will be 0 and compute f(x_i) */
        real fxn[ORDER + 1];
        for (int i = 0; i < ORDER + 1; i++)
        {
            zeroes[i] = (real)(2 * i - ORDER) / (real)(ORDER + 1);
            fxn[i] = Value(zeroes[i]);
        }

        /* We build a matrix of Chebishev evaluations: row i contains the
         * evaluations of x_i for polynomial order n = 0, 1, ... */
        Matrix<ORDER + 1> mat;
        for (int i = 0; i < ORDER + 1; i++)
        {
            /* Compute the powers of x_i */
            real powers[ORDER + 1];
            powers[0] = 1.0;
            for (int n = 1; n < ORDER + 1; n++)
                 powers[n] = powers[n - 1] * zeroes[i];

            /* Compute the Chebishev evaluations at x_i */
            for (int n = 0; n < ORDER + 1; n++)
            {
                real sum = 0.0;
                for (int k = 0; k < ORDER + 1; k++)
                    sum += (real)Cheby(n, k) * powers[k];
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
        for (int i = 0; i < ORDER + 1; i++)
        {
            real a = control[i];
            real ea = ChebyEval(a) - Value(a);
            real b = control[i + 1];
            real eb = ChebyEval(b) - Value(b);

            while (fabs(a - b) > (real)1e-140)
            {
                real c = (a + b) * (real)0.5;
                real ec = ChebyEval(c) - Value(c);

                if ((ea < (real)0 && ec < (real)0)
                     || (ea > (real)0 && ec > (real)0))
                {
                    a = c;
                    ea = ec;
                }
                else
                {
                    b = c;
                    eb = ec;
                }
            }

            zeroes[i] = a;
        }
    }

    void FindError()
    {
        real final = 0;

        for (int i = 0; i < ORDER + 2; i++)
        {
            real a = -1, b = 1;
            if (i > 0)
                a = zeroes[i - 1];
            if (i < ORDER + 1)
                b = zeroes[i];

            printf("Error for [%g..%g]: ", (double)a, (double)b);
            for (;;)
            {
                real c = a, delta = (b - a) / (real)10.0;
                real maxerror = 0;
                int best = -1;
                for (int k = 0; k <= 10; k++)
                {
                    real e = fabs(ChebyEval(c) - Value(c));
                    if (e > maxerror)
                    {
                        maxerror = e;
                        best = k;
                    }
                    c += delta;
                }

                if (best == 0)
                    best = 1;
                if (best == 10)
                    best = 9;

                b = a + (real)(best + 1) * delta;
                a = a + (real)(best - 1) * delta;

                if (b - a < (real)1e-15)
                {
                    if (maxerror > final)
                        final = maxerror;
                    control[i] = (a + b) * (real)0.5;
                    printf("%g (at %g)\n", (double)maxerror, (double)control[i]);
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
        real fxn[ORDER + 2];
        for (int i = 0; i < ORDER + 2; i++)
            fxn[i] = Value(control[i]);

        /* We build a matrix of Chebishev evaluations: row i contains the
         * evaluations of x_i for polynomial order n = 0, 1, ... */
        Matrix<ORDER + 2> mat;
        for (int i = 0; i < ORDER + 2; i++)
        {
            /* Compute the powers of x_i */
            real powers[ORDER + 1];
            powers[0] = 1.0;
            for (int n = 1; n < ORDER + 1; n++)
                 powers[n] = powers[n - 1] * control[i];

            /* Compute the Chebishev evaluations at x_i */
            for (int n = 0; n < ORDER + 1; n++)
            {
                real sum = 0.0;
                for (int k = 0; k < ORDER + 1; k++)
                    sum += (real)Cheby(n, k) * powers[k];
                mat.m[i][n] = sum;
            }
            if (i & 1)
                mat.m[i][ORDER + 1] = fabs(Error(control[i]));
            else
                mat.m[i][ORDER + 1] = -fabs(Error(control[i]));
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
        real error = 0;
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
        /* Transform Chebyshev polynomial weights into powers of X^i
         * in the [-1..1] range. */
        real bn[ORDER + 1];

        for (int i = 0; i < ORDER + 1; i++)
        {
            bn[i] = 0;
            for (int j = 0; j < ORDER + 1; j++)
                bn[i] += coeff[j] * (real)Cheby(j, i);
        }

        /* Transform a polynomial in the [-1..1] range into a polynomial
         * in the [a..b] range. */
        real k1p[ORDER + 1], k2p[ORDER + 1];
        real an[ORDER + 1];

        for (int i = 0; i < ORDER + 1; i++)
        {
            k1p[i] = i ? k1p[i - 1] * m_invk1 : real::R_1;
            k2p[i] = i ? k2p[i - 1] * m_invk2 : real::R_1;
        }

        for (int i = 0; i < ORDER + 1; i++)
        {
            an[i] = 0;
            for (int j = i; j < ORDER + 1; j++)
                an[i] += (real)Comb(j, i) * k1p[j - i] * bn[j];
            an[i] *= k2p[i];
        }

        for (int j = 0; j < ORDER + 1; j++)
            printf("%s%18.16gx^%i", j && (an[j] >= real::R_0) ? "+" : "", (double)an[j], j);
        printf("\n");
    }

    real Value(real const &x)
    {
        return m_func(x * m_k2 + m_k1);
    }

    real Error(real const &x)
    {
        return m_error(x * m_k2 + m_k1);
    }

    /* ORDER + 1 Chebyshev coefficients and 1 error value */
    real coeff[ORDER + 2];
    /* ORDER + 1 zeroes of the error function */
    real zeroes[ORDER + 1];
    /* ORDER + 2 control points */
    real control[ORDER + 2];

private:
    RealFunc *m_func, *m_error;
    real m_k1, m_k2, m_invk1, m_invk2;
};

#endif /* __REMEZ_SOLVER_H__ */

