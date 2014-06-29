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

template<typename T> struct Matrix
{
    inline Matrix<T>(int cols, int rows)
      : m_cols(cols),
        m_rows(rows)
    {
        ASSERT(cols > 0);
        ASSERT(rows > 0);

        m_data.Resize(m_cols * m_rows);
    }

    inline Matrix<T>(Matrix<T> const &other)
    {
        m_cols = other.m_cols;
        m_rows = other.m_rows;
        m_data = other.m_data;
    }

    void Init(T const &x)
    {
        for (int j = 0; j < m_rows; j++)
            for (int i = 0; i < m_cols; i++)
                m(i, j) = (i == j) ? x : (T)0;
    }

    /* Naive matrix inversion */
    Matrix<T> inv() const
    {
        ASSERT(m_cols == m_rows);

        Matrix a(*this), b(m_cols, m_rows);

        b.Init((T)1);

        /* Inversion method: iterate through all columns and make sure
         * all the terms are 1 on the diagonal and 0 everywhere else */
        for (int i = 0; i < m_cols; i++)
        {
            /* If the expected coefficient is zero, add one of
             * the other lines. The first we meet will do. */
            if (!a.m(i, i))
            {
                for (int j = i + 1; j < m_cols; j++)
                {
                    if (!a.m(i, j))
                        continue;
                    /* Add row j to row i */
                    for (int n = 0; n < m_cols; n++)
                    {
                        a.m(n, i) += a.m(n, j);
                        b.m(n, i) += b.m(n, j);
                    }
                    break;
                }
            }

            /* Now we know the diagonal term is non-zero. Get its inverse
             * and use that to nullify all other terms in the column */
            T x = (T)1 / a.m(i, i);
            for (int j = 0; j < m_cols; j++)
            {
                if (j == i)
                    continue;
                T mul = x * a.m(i, j);
                for (int n = 0; n < m_cols; n++)
                {
                    a.m(n, j) -= mul * a.m(n, i);
                    b.m(n, j) -= mul * b.m(n, i);
                }
            }

            /* Finally, ensure the diagonal term is 1 */
            for (int n = 0; n < m_cols; n++)
            {
                a.m(n, i) *= x;
                b.m(n, i) *= x;
            }
        }

        return b;
    }

    inline T & m(int i, int j) { return m_data[m_rows * j + i]; }
    inline T const & m(int i, int j) const { return m_data[m_rows * j + i]; }

    int m_cols, m_rows;

private:
    array<T> m_data;
};

