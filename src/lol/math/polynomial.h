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
// The data structure is a simple dynamic array of scalars, with the
// added guarantee that the leading coefficient is always non-zero.
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
        m_coefficients.push(a);
        reduce_degree();
    }

    /* Create a polynomial from a list of coefficients */
    explicit polynomial(std::initializer_list<T> const &init)
    {
        for (auto a : init)
            m_coefficients.push(a);

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
            ret.m_coefficients.push(T(coeff(k, n)));
        return ret;
    }

    /* We define the zero polynomial (with no coefficients) as having
     * degree -1 on purpose. */
    inline int degree() const
    {
        return (int)m_coefficients.count() - 1;
    }

    /* Set one of the polynomial’s coefficients */
    void set(int n, T const &a)
    {
        ASSERT(n >= 0);

        if (n > degree() && !a)
            return;

        while (n > degree())
            m_coefficients.push(T(0));

        m_coefficients[n] = a;
        reduce_degree();
    }

    /* Evaluate polynomial at a given value. This method can also
     * be used to compose polynomials, i.e. using another polynomial
     * as the value instead of a scalar. */
    template<typename U> U eval(U x) const
    {
        U ret(leading());
        for (int i = degree() - 1; i >= 0; --i)
            ret = ret * x + U(m_coefficients[i]);
        return ret;
    }

    polynomial<T> derive() const
    {
        /* No need to reduce the degree after deriving. */
        polynomial<T> ret;
        for (int i = 1; i <= degree(); ++i)
            ret.m_coefficients.push(m_coefficients[i] * T(i));
        return ret;
    }

    array<T> roots() const
    {
        /* For now we can only solve polynomials of degrees 0, 1, 2 or 3. */
        ASSERT(degree() >= 0 && degree() <= 3,
               "roots() called on polynomial of degree %d", degree());

        if (degree() == 0)
        {
            /* p(x) = a > 0 */
            return array<T> {};
        }
        else if (degree() == 1)
        {
            /* p(x) = ax + b */
            T const &a = m_coefficients[1];
            T const &b = m_coefficients[0];
            return array<T> { -b / a };
        }
        else if (degree() == 2)
        {
            /* p(x) = ax² + bx + c */
            T const &a = m_coefficients[2];
            T const &b = m_coefficients[1];
            T const &c = m_coefficients[0];

            T const k = b / (a + a);
            T const delta = k * k - c / a;

            if (delta < T(0))
            {
                 return array<T> {};
            }
            else if (delta > T(0))
            {
                 T const sqrt_delta = sqrt(delta);
                 return array<T> { -k - sqrt_delta, -k + sqrt_delta };
            }
            else
            {
                 return array<T> { -k };
            }
        }
        else if (degree() == 3)
        {
            /* p(x) = ax³ + bx² + cx + d */
            T const &a = m_coefficients[3];
            T const &b = m_coefficients[2];
            T const &c = m_coefficients[1];
            T const &d = m_coefficients[0];

            /* Using x = (X - k) so that p2(X) = p(X - k) = aX³ + 0×X² + mX + n */
            T const k = b / (3 * a);
            T const m = 3 * a * k * k - 2 * b * k + c;
            T const n = -a * k * k * k + b * k * k - c * k + d;

            /* Assuming X = u + v and 3uv = -m, then
             * p2(u + v) = a(u + v) + n
             *
             * We then need to solve the following system:
             *    u³v³ = -m³/27
             * u³ + v³ = -n/a
             *
             * which gives :
             * u³ - v³ = √((n/a)² + 4m³/27)
             * u³ + v³ = -n/a
             *
             * u³ = (-n/a + √((n/a)² + 4m³/27))/2
             * v³ = (-n/a - √((n/a)² + 4m³/27))/2
             */
            T const delta = (n * n) / (a * a) + 4.f * m * m * m / 27.f;

            /* Because 3×u×v = -m and m is not complex
             * angle(u³) + angle(v³) must equal 0.
             *
             * This is why we compute u³ and v³ by norm and angle separately instead of
             * using a std::complex class
             */
            T u3_norm, u3_angle;
            T v3_norm, v3_angle;

            if (delta < 0)
            {
                v3_norm = u3_norm = sqrt((-n/a) * (-n/a) + abs(delta)) / 2.f;

                u3_angle = atan2(sqrt(abs(delta)), (-n/(2.f*a)));
                v3_angle = -u3_angle;
            }
            else
            {
                u3_norm = -n/(2.0f*a) + sqrt(delta) / 2.f;
                v3_norm = -n/(2.0f*a) - sqrt(delta) / 2.f;

                u3_angle = u3_norm >= 0 ? 0 :  M_PI;
                v3_angle = v3_norm >= 0 ? 0 : -M_PI;

                u3_norm = abs(u3_norm);
                v3_norm = abs(v3_norm);
            }

            T solutions[3];

            for (int i = 0 ; i < 3 ; ++i)
            {
                T u_angle = u3_angle / 3.f + i * 2.f * M_PI / 3.f;
                T v_angle = v3_angle / 3.f - i * 2.f * M_PI / 3.f;

                solutions[i] =
                    pow(u3_norm, 1.f / 3.f) * cos(u_angle) +
                    pow(v3_norm, 1.f / 3.f) * cos(v_angle);
            }

            if (delta < 0) // 3 real solutions
                return array<T> {solutions[0] - k, solutions[1] - k, solutions[2] - k};
            else if (delta > 0) // 1 real solution
                return array<T> {solutions[0] - k};
            else if (u3_norm > 0) // 2 real solutions
                return array<T> {solutions[0] - k, solutions[1] - k};
            else // one triple solution
                return array<T> {solutions[0] - k};
        }

        /* It is an error to reach this point. */
        ASSERT(false);
        return array<T> {};
    }

    /* Access individual coefficients. This is read-only and returns a
     * copy because we cannot let the user mess with the integrity of
     * the structure (i.e. the guarantee that the leading coefficient
     * remains non-zero). */
    inline T operator[](ptrdiff_t n) const
    {
        if (n < 0 || n > degree())
            return T(0);

        return m_coefficients[n];
    }

    /* Return the leading coefficient */
    inline T leading() const
    {
        return (*this)[degree()];
    }

    /* Unary plus */
    polynomial<T> operator +() const
    {
        return *this;
    }

    /* Unary minus */
    polynomial<T> operator -() const
    {
        polynomial<T> ret;
        for (auto a : m_coefficients)
            ret.m_coefficients.push(-a);
        return ret;
    }

    /* Add two polynomials */
    polynomial<T> &operator +=(polynomial<T> const &p)
    {
        int min_degree = lol::min(p.degree(), degree());

        for (int i = 0; i <= min_degree; ++i)
            m_coefficients[i] += p[i];

        for (int i = min_degree + 1; i <= p.degree(); ++i)
            m_coefficients.push(p[i]);

        reduce_degree();
        return *this;
    }

    polynomial<T> operator +(polynomial<T> const &p) const
    {
        return polynomial<T>(*this) += p;
    }

    /* Subtract two polynomials */
    polynomial<T> &operator -=(polynomial<T> const &p)
    {
        return *this += -p;
    }

    polynomial<T> operator -(polynomial<T> const &p) const
    {
        return polynomial<T>(*this) += -p;
    }

    /* Multiply polynomial by a scalar */
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

    /* Divide polynomial by a scalar */
    polynomial<T> &operator /=(T const &k)
    {
        return *this *= (T(1) / k);
    }

    polynomial<T> operator /(T const &k) const
    {
        return *this * (T(1) / k);
    }

    /* Multiply polynomial by another polynomial */
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
                ret.m_coefficients.push(T(0));

            for (int i = 0; i <= p.degree(); ++i)
                for (int j = 0; j <= q.degree(); ++j)
                    ret.m_coefficients[i + j] += p[i] * q[j];

            ret.reduce_degree();
        }

        return ret;
    }

    /* Divide a polynomial by another one. There is no /= variant because
     * the return value contains both the quotient and the remainder. */
    tuple<polynomial<T>, polynomial<T>> operator /(polynomial<T> p) const
    {
        ASSERT(p.degree() >= 0);

        tuple<polynomial<T>, polynomial<T>> ret;
        polynomial<T> &quotient = ret.m1;
        polynomial<T> &remainder = ret.m2;

        remainder = *this / p.leading();
        p /= p.leading();

        for (int n = remainder.degree() - p.degree(); n >= 0; --n)
        {
            quotient.set(n, remainder.leading());
            for (int i = 0; i < p.degree(); ++i)
                remainder.m_coefficients[n + i] -= remainder.leading() * p[i];
            remainder.m_coefficients.pop();
        }

        return ret;
    }

private:
    /* Enforce the non-zero leading coefficient rule. */
    void reduce_degree()
    {
        while (m_coefficients.count() && m_coefficients.last() == T(0))
            m_coefficients.pop();
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

