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
static int const ORDER = 3;

/* The function we want to approximate */
double myfun(double x)
{
    return exp(x);
}

real myfun(real const &x)
{
    return exp(x);
}

/* Naive matrix inversion */
template<int N> class Matrix
{
public:
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
        Matrix a = *this, b(real(1.0));

        /* Inversion method: iterate through all columns and make sure
         * all the terms are zero except on the diagonal where it is one */
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
            real x = fres(a.m[i][i]);
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
static void make_table()
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

int main(int argc, char **argv)
{
    make_table();

    /* We start with ORDER+1 points and their images through myfun() */
    real xn[ORDER + 1];
    real fxn[ORDER + 1];
    for (int i = 0; i < ORDER + 1; i++)
    {
        //xn[i] = real(2 * i - ORDER) / real(ORDER + 1);
        xn[i] = real(2 * i - ORDER + 1) / real(ORDER - 1);
        fxn[i] = myfun(xn[i]);
    }

    /* We build a matrix of Chebishev evaluations: one row per point
     * in our array, and column i is the evaluation of the ith order
     * polynomial. */
    Matrix<ORDER + 1> mat;
    for (int j = 0; j < ORDER + 1; j++)
    {
        /* Compute the powers of x_j */
        real powers[ORDER + 1];
        powers[0] = 1.0;
        for (int i = 1; i < ORDER + 1; i++)
             powers[i] = powers[i - 1] * xn[j];

        /* Compute the Chebishev evaluations at x_j */
        for (int i = 0; i < ORDER + 1; i++)
        {
            real sum = 0.0;
            for (int k = 0; k < ORDER + 1; k++)
                if (cheby[i][k])
                    sum += real(cheby[i][k]) * powers[k];
            mat.m[j][i] = sum;
        }
    }

    /* Invert the matrix and build interpolation coefficients */
    mat = mat.inv();
    real an[ORDER + 1];
    for (int j = 0; j < ORDER + 1; j++)
    {
        an[j] = 0;
        for (int i = 0; i < ORDER + 1; i++)
            an[j] += mat.m[j][i] * fxn[i];
        an[j].print(10);
    }

    return EXIT_SUCCESS;
}

