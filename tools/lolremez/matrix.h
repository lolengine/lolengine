//
// LolRemez - Remez algorithm implementation
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

using namespace lol;

/*
 * Arbitrarily-sized square matrices; for now this only supports
 * naive inversion and is used for the Remez inversion method.
 */

template<typename T>
struct LinearSystem : public array2d<T>
{
    inline LinearSystem<T>(int cols)
    {
        ASSERT(cols > 0);

        this->SetSize(ivec2(cols));
    }

    void Init(T const &x)
    {
        int const n = this->GetSize().x;

        for (int j = 0; j < n; j++)
            for (int i = 0; i < n; i++)
                (*this)[i][j] = (i == j) ? x : (T)0;
    }

    /* Naive matrix inversion */
    LinearSystem<T> inverse() const
    {
        int const n = this->GetSize().x;
        LinearSystem a(*this), b(n);

        b.Init((T)1);

        /* Inversion method: iterate through all columns and make sure
         * all the terms are 1 on the diagonal and 0 everywhere else */
        for (int i = 0; i < n; i++)
        {
            /* If the expected coefficient is zero, add one of
             * the other lines. The first we meet will do. */
            if (!a[i][i])
            {
                for (int j = i + 1; j < n; j++)
                {
                    if (!a[i][j])
                        continue;
                    /* Add row j to row i */
                    for (int k = 0; k < n; k++)
                    {
                        a[k][i] += a[k][j];
                        b[k][i] += b[k][j];
                    }
                    break;
                }
            }

            /* Now we know the diagonal term is non-zero. Get its inverse
             * and use that to nullify all other terms in the column */
            T x = (T)1 / a[i][i];
            for (int j = 0; j < n; j++)
            {
                if (j == i)
                    continue;
                T mul = x * a[i][j];
                for (int k = 0; k < n; k++)
                {
                    a[k][j] -= mul * a[k][i];
                    b[k][j] -= mul * b[k][i];
                }
            }

            /* Finally, ensure the diagonal term is 1 */
            for (int k = 0; k < n; k++)
            {
                a[k][i] *= x;
                b[k][i] *= x;
            }
        }

        return b;
    }
};

