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

namespace lol
{

template<typename T>
struct polynomial
{
    inline polynomial()
    {
    }

    explicit polynomial(std::initializer_list<T> const &init)
    {
        for (auto a : init)
            factors.Push(a);

        reduce_degree();
    }

    /* We define the zero polynomial (with no coefficients) as having
     * degree -1 on purpose. */
    inline int degree() const
    {
        return (int)factors.Count() - 1;
    }

    T eval(T x) const
    {
        T ret = this->operator[](degree());
        for (int i = degree() - 1; i >= 0; --i)
            ret = ret * x + factors[i];
        return ret;
    }

    inline T operator[](ptrdiff_t n) const
    {
        if (n < 0 || n > degree())
            return T(0);

        return factors[n];
    }

    polynomial<T> operator +() const
    {
        return *this;
    }

    polynomial<T> operator -() const
    {
        polynomial<T> ret;
        for (auto a : factors)
            ret.factors.Push(-a);
        return ret;
    }

    polynomial<T> &operator +=(polynomial<T> const &p)
    {
        int min_degree = lol::min(p.degree(), degree());

        for (int i = 0; i <= min_degree; ++i)
            factors[i] += p[i];

        for (int i = min_degree + 1; i <= p.degree(); ++i)
            factors.Push(p[i]);

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
        for (auto &a : factors)
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
                ret.factors.Push(T(0));

            for (int i = 0; i <= p.degree(); ++i)
                for (int j = 0; j <= q.degree(); ++j)
                    ret.factors[i + j] += p[i] * q[j];

            ret.reduce_degree();
        }

        return ret;
    }

private:
    void reduce_degree()
    {
        while (factors.Count() && factors.Last() == T(0))
            factors.Pop();
    }

    array<T> factors;
};

} /* namespace lol */

