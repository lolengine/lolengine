//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The polynomial class
// --------------------
//

#include <functional>

namespace lol
{

template<typename T>
struct polynomial
{
    /* The zero polynomial */
    explicit inline polynomial() {}

    /* A constant polynomial */
    explicit inline polynomial(T const &a)
    {
        m_coefficients.Push(a);
        reduce_degree();
    }

    /* Create a polynomial from a list of coefficients */
    explicit polynomial(std::initializer_list<T> const &init)
    {
        for (auto a : init)
            m_coefficients.Push(a);

        reduce_degree();
    }

    /* Factory for Chebyshev polynomials */
    static polynomial<T> chebyshev(int n)
    {
        /* Use T0(x) = 1, T1(x) = x, Tn(x) = 2 x Tn-1(x) - Tn-2(x) */
        std::function<int64_t (int, int)> coeff = [&](int i, int j)
        {
            if (i > j || i < 0 || ((j ^ i) & 1))
                return (int64_t)0;
            if (j < 2)
                return (int64_t)1;
            return 2 * coeff(i - 1, j - 1) - coeff(i, j - 2);
        };

        polynomial<T> ret;
        for (int k = 0; k <= n; ++k)
            ret.m_coefficients.Push(T(coeff(k, n)));
        return ret;
    }

    /* We define the zero polynomial (with no coefficients) as having
     * degree -1 on purpose. */
    inline int degree() const
    {
        return (int)m_coefficients.Count() - 1;
    }

    /* Set one of the polynomial’s coefficients */
    void set(ptrdiff_t n, T const &a)
    {
        ASSERT(n >= 0);

        if (n > degree() && !a)
            return;

        while (n > degree())
            m_coefficients.Push(T(0));

        m_coefficients[n] = a;
        reduce_degree();
    }

    /* Evaluate polynomial at a given value. This method can also
     * be used to compose polynomials, i.e. using another polynomial
     * as the value instead of a scalar. */
    template<typename U> U eval(U x) const
    {
        U ret((*this)[degree()]);
        for (int i = degree() - 1; i >= 0; --i)
            ret = ret * x + U(m_coefficients[i]);
        return ret;
    }

    polynomial<T> derive() const
    {
        /* No need to reduce the degree after deriving. */
        polynomial<T> ret;
        for (int i = 1; i <= degree(); ++i)
            ret.m_coefficients.Push(m_coefficients[i] * T(i));
        return ret;
    }

    inline T operator[](ptrdiff_t n) const
    {
        if (n < 0 || n > degree())
            return T(0);

        return m_coefficients[n];
    }

    polynomial<T> operator +() const
    {
        return *this;
    }

    polynomial<T> operator -() const
    {
        polynomial<T> ret;
        for (auto a : m_coefficients)
            ret.m_coefficients.Push(-a);
        return ret;
    }

    polynomial<T> &operator +=(polynomial<T> const &p)
    {
        int min_degree = lol::min(p.degree(), degree());

        for (int i = 0; i <= min_degree; ++i)
            m_coefficients[i] += p[i];

        for (int i = min_degree + 1; i <= p.degree(); ++i)
            m_coefficients.Push(p[i]);

        reduce_degree();
        return *this;
    }

    polynomial<T> operator +(polynomial<T> const &p) const
    {
        return polynomial<T>(*this) += p;
    }

    polynomial<T> &operator -=(polynomial<T> const &p)
    {
        return *this += -p;
    }

    polynomial<T> operator -(polynomial<T> const &p) const
    {
        return polynomial<T>(*this) += -p;
    }

    polynomial<T> &operator *=(T const &k)
    {
        for (auto &a : m_coefficients)
            a *= k;
        reduce_degree();
        return *this;
    }

    polynomial<T> operator *(T const &k) const
    {
        return polynomial<T>(*this) *= k;
    }

    polynomial<T> &operator *=(polynomial<T> const &p)
    {
        return *this = *this * p;
    }

    polynomial<T> operator *(polynomial<T> const &p) const
    {
        polynomial<T> ret;
        polynomial<T> const &q = *this;

        if (p.degree() >= 0 && q.degree() >= 0)
        {
            int n = p.degree() + q.degree();
            for (int i = 0; i <= n; ++i)
                ret.m_coefficients.Push(T(0));

            for (int i = 0; i <= p.degree(); ++i)
                for (int j = 0; j <= q.degree(); ++j)
                    ret.m_coefficients[i + j] += p[i] * q[j];

            ret.reduce_degree();
        }

        return ret;
    }

private:
    void reduce_degree()
    {
        while (m_coefficients.Count() && m_coefficients.Last() == T(0))
            m_coefficients.Pop();
    }

    /* The polynomial coefficients */
    array<T> m_coefficients;
};

template<typename T>
polynomial<T> operator *(T const &k, polynomial<T> const &p)
{
    /* We assume k * p is the same as p * k */
    return p * k;
}

} /* namespace lol */

