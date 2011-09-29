//
// Lol Engine - Sample math program: Chebyshev polynomials
//
// Copyright: (c) 2005-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>

#include "core.h"

using namespace lol;

/* The order of the approximation we're looking for */
static int const ORDER = 8;

/* The function we want to approximate */
static real myfun(real const &x)
{
    static real const one = 1.0;
    if (!x)
        return real::R_PI_2;
    return sin(x * real::R_PI_2) / x;
    //return cos(x) - one;
    //return exp(x);
}

/* Naive matrix inversion */
template<int N> struct Matrix
{
    inline Matrix() {}

    Matrix(real x)
    {
        for (int j = 0; j < N; j++)
            for (int i = 0; i < N; i++)
                if (i == j)
                    m[i][j] = x;
                else
                    m[i][j] = 0;
    }

    Matrix<N> inv() const
    {
        Matrix a = *this, b((real)1.0);

        /* Inversion method: iterate through all columns and make sure
         * all the terms are 1 on the diagonal and 0 everywhere else */
        for (int i = 0; i < N; i++)
        {
            /* If the expected coefficient is zero, add one of
             * the other lines. The first we meet will do. */
            if ((double)a.m[i][i] == 0.0)
            {
                for (int j = i + 1; j < N; j++)
                {
                    if ((double)a.m[i][j] == 0.0)
                        continue;
                    /* Add row j to row i */
                    for (int n = 0; n < N; n++)
                    {
                        a.m[n][i] += a.m[n][j];
                        b.m[n][i] += b.m[n][j];
                    }
                    break;
                }
            }

            /* Now we know the diagonal term is non-zero. Get its inverse
             * and use that to nullify all other terms in the column */
            real x = (real)1.0 / a.m[i][i];
            for (int j = 0; j < N; j++)
            {
                if (j == i)
                    continue;
                real mul = x * a.m[i][j];
                for (int n = 0; n < N; n++)
                {
                    a.m[n][j] -= mul * a.m[n][i];
                    b.m[n][j] -= mul * b.m[n][i];
                }
            }

            /* Finally, ensure the diagonal term is 1 */
            for (int n = 0; n < N; n++)
            {
                a.m[n][i] *= x;
                b.m[n][i] *= x;
            }
        }

        return b;
    }

    void print() const
    {
        for (int j = 0; j < N; j++)
        {
            for (int i = 0; i < N; i++)
                printf("%9.5f ", (double)m[j][i]);
            printf("\n");
        }
    }

    real m[N][N];
};


static int cheby[ORDER + 1][ORDER + 1];

/* Fill the Chebyshev tables */
static void cheby_init()
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

static void cheby_coeff(real *coeff, real *bn)
{
    for (int i = 0; i < ORDER + 1; i++)
    {
        bn[i] = 0;
        for (int j = 0; j < ORDER + 1; j++)
	    if (cheby[j][i])
                bn[i] += coeff[j] * (real)cheby[j][i];
    }
}

static real cheby_eval(real *coeff, real const &x)
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

static void remez_init(real *coeff, real *zeroes)
{
    /* Pick up x_i where error will be 0 and compute f(x_i) */
    real fxn[ORDER + 1];
    for (int i = 0; i < ORDER + 1; i++)
    {
        zeroes[i] = (real)(2 * i - ORDER) / (real)(ORDER + 1);
        fxn[i] = myfun(zeroes[i]);
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

static void remez_findzeroes(real *coeff, real *zeroes, real *control)
{
    /* FIXME: this is fake for now */
    for (int i = 0; i < ORDER + 1; i++)
    {
        real a = control[i];
        real ea = cheby_eval(coeff, a) - myfun(a);
        real b = control[i + 1];
        real eb = cheby_eval(coeff, b) - myfun(b);

        while (fabs(a - b) > (real)1e-140)
        {
            real c = (a + b) * (real)0.5;
            real ec = cheby_eval(coeff, c) - myfun(c);

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

static void remez_finderror(real *coeff, real *zeroes, real *control)
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
                real e = fabs(cheby_eval(coeff, c) - myfun(c));
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
                printf("%g (in %g)\n", (double)maxerror, (double)control[i]);
                break;
            }
        }
    }

    printf("Final error: %g\n", (double)final);
}

static void remez_step(real *coeff, real *control)
{
    /* Pick up x_i where error will be 0 and compute f(x_i) */
    real fxn[ORDER + 2];
    for (int i = 0; i < ORDER + 2; i++)
        fxn[i] = myfun(control[i]);

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
        mat.m[i][ORDER + 1] = (real)(-1 + (i & 1) * 2);
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

int main(int argc, char **argv)
{
    cheby_init();

    /* ORDER + 1 chebyshev coefficients and 1 error value */
    real coeff[ORDER + 2];
    /* ORDER + 1 zeroes of the error function */
    real zeroes[ORDER + 1];
    /* ORDER + 2 control points */
    real control[ORDER + 2];

    real bn[ORDER + 1];

    remez_init(coeff, zeroes);

    cheby_coeff(coeff, bn);
    for (int j = 0; j < ORDER + 1; j++)
        printf("%s%12.10gx^%i", j ? "+" : "", (double)bn[j], j);
    printf("\n");

    for (int n = 0; n < 200; n++)
    {
        remez_finderror(coeff, zeroes, control);
        remez_step(coeff, control);

        cheby_coeff(coeff, bn);
        for (int j = 0; j < ORDER + 1; j++)
            printf("%s%12.10gx^%i", j ? "+" : "", (double)bn[j], j);
        printf("\n");

        remez_findzeroes(coeff, zeroes, control);
    }

    remez_finderror(coeff, zeroes, control);
    remez_step(coeff, control);

    cheby_coeff(coeff, bn);
    for (int j = 0; j < ORDER + 1; j++)
        printf("%s%12.10gx^%i", j ? "+" : "", (double)bn[j], j);
    printf("\n");

    return EXIT_SUCCESS;
}

