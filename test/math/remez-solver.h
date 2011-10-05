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
        ChebyInit();
    }

    void Run(RealFunc *func, RealFunc *error, int steps)
    {
        m_func = func;
        m_error = error;

        Init();

        ChebyCoeff();
        for (int j = 0; j < ORDER + 1; j++)
            printf("%s%14.12gx^%i", j && (bn[j] >= real::R_0) ? "+" : "", (double)bn[j], j);
        printf("\n");

        for (int n = 0; n < steps; n++)
        {
            FindError();
            Step();

            ChebyCoeff();
            for (int j = 0; j < ORDER + 1; j++)
                printf("%s%14.12gx^%i", j && (bn[j] >= real::R_0) ? "+" : "", (double)bn[j], j);
            printf("\n");

            FindZeroes();
        }

        FindError();
        Step();

        ChebyCoeff();
        for (int j = 0; j < ORDER + 1; j++)
            printf("%s%14.12gx^%i", j && (bn[j] >= real::R_0) ? "+" : "", (double)bn[j], j);
        printf("\n");
    }

    /* Fill the Chebyshev tables */
    void ChebyInit()
    {
        memset(cheby, 0, sizeof(cheby));

        cheby[0][0] = 1;
        cheby[1][1] = 1;

        for (int i = 2; i < ORDER + 1; i++)
        {
            cheby[i][0] = -cheby[i - 2][0];
            for (int j = 1; j < ORDER + 1; j++)
                cheby[i][j] = 2 * cheby[i - 1][j - 1] - cheby[i - 2][j];
        }
    }

    void ChebyCoeff()
    {
        for (int i = 0; i < ORDER + 1; i++)
        {
            bn[i] = 0;
            for (int j = 0; j < ORDER + 1; j++)
    	    if (cheby[j][i])
                    bn[i] += coeff[j] * (real)cheby[j][i];
        }
    }

    real ChebyEval(real const &x)
    {
        real ret = 0.0, xn = 1.0;

        for (int i = 0; i < ORDER + 1; i++)
        {
            real mul = 0;
            for (int j = 0; j < ORDER + 1; j++)
    	    if (cheby[j][i])
                    mul += coeff[j] * (real)cheby[j][i];
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
            fxn[i] = m_func(zeroes[i]);
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
                    if (cheby[n][k])
                        sum += (real)cheby[n][k] * powers[k];
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
            real ea = ChebyEval(a) - m_func(a);
            real b = control[i + 1];
            real eb = ChebyEval(b) - m_func(b);

            while (fabs(a - b) > (real)1e-140)
            {
                real c = (a + b) * (real)0.5;
                real ec = ChebyEval(c) - m_func(c);

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
                    real e = fabs(ChebyEval(c) - m_func(c));
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

        printf("Final error: %g\n", (double)final);
    }

    void Step()
    {
        /* Pick up x_i where error will be 0 and compute f(x_i) */
        real fxn[ORDER + 2];
        for (int i = 0; i < ORDER + 2; i++)
            fxn[i] = m_func(control[i]);

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
                    if (cheby[n][k])
                        sum += (real)cheby[n][k] * powers[k];
                mat.m[i][n] = sum;
            }
            if (i & 1)
                mat.m[i][ORDER + 1] = fabs(m_error(control[i]));
            else
                mat.m[i][ORDER + 1] = -fabs(m_error(control[i]));
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

    int cheby[ORDER + 1][ORDER + 1];

    /* ORDER + 1 chebyshev coefficients and 1 error value */
    real coeff[ORDER + 2];
    /* ORDER + 1 zeroes of the error function */
    real zeroes[ORDER + 1];
    /* ORDER + 2 control points */
    real control[ORDER + 2];

    real bn[ORDER + 1];

private:
    RealFunc *m_func;
    RealFunc *m_error;
};

#endif /* __REMEZ_SOLVER_H__ */

