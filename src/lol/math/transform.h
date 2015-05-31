//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The complex, quaternion and dual quaternion classes
// ---------------------------------------------------
//

#include <ostream>

namespace lol
{

#if !LOL_FEATURE_CXX11_CONSTEXPR
#   define constexpr /* */
#endif

/*
 * 2-element transforms: complex numbers
 */

template<typename T>
struct cmplx_t : public linear_ops::base<T>
{
    static int const count = 2;
    typedef T element;
    typedef cmplx_t<T> type;

    inline constexpr cmplx_t() {}
    inline constexpr cmplx_t(T X) : x(X), y(T(0)) {}
    inline constexpr cmplx_t(T X, T Y) : x(X), y(Y) {}

    template<typename U>
    explicit inline constexpr cmplx_t(cmplx_t<U> const &z)
      : x(z.x), y(z.y) {}

    LOL_COMMON_MEMBER_OPS(x)

    inline cmplx_t<T> operator *(cmplx_t<T> const &val) const
    {
        return cmplx_t<T>(x * val.x - y * val.y, x * val.y + y * val.x);
    }

    inline cmplx_t<T> operator *=(cmplx_t<T> const &val)
    {
        return *this = (*this) * val;
    }

    inline cmplx_t<T> operator ~() const
    {
        return cmplx_t<T>(x, -y);
    }

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, cmplx_t<U> const &v);

    T x, y;
};

static_assert(sizeof(f16cmplx) == 4, "sizeof(f16cmplx) == 4");
static_assert(sizeof(cmplx) == 8, "sizeof(cmplx) == 8");
static_assert(sizeof(dcmplx) == 16, "sizeof(dcmplx) == 16");

/*
 * 4-element transforms: quaternions
 */

template<typename T>
struct quat_t : public linear_ops::base<T>
{
    static int const count = 4;
    typedef T element;
    typedef quat_t<T> type;

    /* Default constructor and copy constructor */
    inline constexpr quat_t() : w(), x(), y(), z() {}
    inline constexpr quat_t(quat_t<T> const &q)
      : w(q.w), x(q.x), y(q.y), z(q.z) {}

    /* Explicit constructor for type conversion */
    template<typename U>
    explicit inline constexpr quat_t(quat_t<U> const &q)
      : w(q.w), x(q.x), y(q.y), z(q.z) {}

    /* Various explicit constructors */
    explicit inline constexpr quat_t(T W, T X, T Y, T Z)
      : w(W), x(X), y(Y), z(Z) {}
    explicit inline constexpr quat_t(T W)
      : w(W), x(0), y(0), z(0) {}

    /* Construct a unit quaternion from a pure rotation matrix */
    explicit quat_t(mat_t<T,3,3> const &m)
    {
        T tr = m[0][0] + m[1][1] + m[2][2];

        if (tr > T(0))
        {
            T const p = T(0.5) * std::sqrt(T(1) + tr);
            T const q = T(0.25) / p;

            w = p;
            x = q * (m[1][2] - m[2][1]);
            y = q * (m[2][0] - m[0][2]);
            z = q * (m[0][1] - m[1][0]);
        }
        else
        {
            int i = (m[0][0] > m[1][1] && m[0][0] > m[2][2]) ? 0
                  : (m[1][1] > m[2][2]) ? 1
                  : 2;
            int j = (i + 1) % 3, k = (i + 2) % 3;

            T const p = T(0.5) * lol::sqrt(T(1) - tr + m[i][i] + m[i][i]);
            T const q = T(0.25) / p;

            w              = q * (m[j][k] - m[k][j]);
            (*this)[1 + i] = p;
            (*this)[1 + j] = q * (m[i][j] + m[j][i]);
            (*this)[1 + k] = q * (m[k][i] + m[i][k]);
        }
    }

    LOL_COMMON_MEMBER_OPS(w)

    inline quat_t operator *(quat_t const &val) const
    {
        vec_t<T,3> v1(x, y, z);
        vec_t<T,3> v2(val.x, val.y, val.z);
        vec_t<T,3> v3 = cross(v1, v2) + w * v2 + val.w * v1;
        return quat_t(w * val.w - dot(v1, v2), v3.x, v3.y, v3.z);
    }

    inline quat_t operator *=(quat_t const &val)
    {
        return *this = (*this * val);
    }

    /* Create a unit quaternion representing a rotation around an axis. */
    static quat_t rotate(T degrees, T x, T y, T z);
    static quat_t rotate(T degrees, vec_t<T,3> const &v);

    /* Create a unit quaternion representing a rotation between two vectors.
     * Input vectors need not be normalised. */
    static quat_t rotate(vec_t<T,3> const &src, vec_t<T,3> const &dst);

    /* Convert from Euler angles. The axes in fromeuler_xyx are
     * x, then y', then x", ie. the axes are attached to the model.
     * If you want to rotate around static axes, just reverse the order
     * of the arguments. Angle values are in degrees. */
    static quat_t fromeuler_xyx(vec_t<T,3> const &v);
    static quat_t fromeuler_xzx(vec_t<T,3> const &v);
    static quat_t fromeuler_yxy(vec_t<T,3> const &v);
    static quat_t fromeuler_yzy(vec_t<T,3> const &v);
    static quat_t fromeuler_zxz(vec_t<T,3> const &v);
    static quat_t fromeuler_zyz(vec_t<T,3> const &v);
    static quat_t fromeuler_xyx(T phi, T theta, T psi);
    static quat_t fromeuler_xzx(T phi, T theta, T psi);
    static quat_t fromeuler_yxy(T phi, T theta, T psi);
    static quat_t fromeuler_yzy(T phi, T theta, T psi);
    static quat_t fromeuler_zxz(T phi, T theta, T psi);
    static quat_t fromeuler_zyz(T phi, T theta, T psi);

    /* Convert from Tait-Bryan angles (incorrectly called Euler angles,
     * but since everyone does it…). The axes in fromeuler_xyz are
     * x, then y', then z", ie. the axes are attached to the model.
     * If you want to apply yaw around x, pitch around y, and roll
     * around z, use fromeuler_xyz. Angle values are in degrees.
     * If you want to rotate around static axes, reverse the order in
     * the function name (_zyx instead of _xyz) AND reverse the order
     * of the arguments. */
    static quat_t fromeuler_xyz(vec_t<T,3> const &v);
    static quat_t fromeuler_xzy(vec_t<T,3> const &v);
    static quat_t fromeuler_yxz(vec_t<T,3> const &v);
    static quat_t fromeuler_yzx(vec_t<T,3> const &v);
    static quat_t fromeuler_zxy(vec_t<T,3> const &v);
    static quat_t fromeuler_zyx(vec_t<T,3> const &v);
    static quat_t fromeuler_xyz(T phi, T theta, T psi);
    static quat_t fromeuler_xzy(T phi, T theta, T psi);
    static quat_t fromeuler_yxz(T phi, T theta, T psi);
    static quat_t fromeuler_yzx(T phi, T theta, T psi);
    static quat_t fromeuler_zxy(T phi, T theta, T psi);
    static quat_t fromeuler_zyx(T phi, T theta, T psi);

    inline quat_t operator ~() const
    {
        return quat_t(w, -x, -y, -z);
    }

    /* Transform vectors or points */
    inline vec_t<T,3> transform(vec_t<T,3> const &v) const
    {
        quat_t p = quat_t(0, v.x, v.y, v.z);
        quat_t q = *this * p / *this;
        return vec_t<T,3>(q.x, q.y, q.z);
    }

    inline vec_t<T,4> transform(vec_t<T,4> const &v) const
    {
        quat_t p = quat_t(0, v.x, v.y, v.z);
        quat_t q = *this * p / *this;
        return vec_t<T,4>(q.x, q.y, q.z, v.w);
    }

    inline vec_t<T,3> operator *(vec_t<T,3> const &v) const
    {
        return transform(v);
    }

    inline vec_t<T,4> operator *(vec_t<T,4> const &v) const
    {
        return transform(v);
    }

    inline vec_t<T,3> axis()
    {
        vec_t<T,3> v(x, y, z);
        T n2 = sqlength(v);
        if (n2 <= (T)1e-6)
            return vec_t<T,3>::axis_x;
        return normalize(v);
    }

    inline T angle()
    {
        vec_t<T,3> v(x, y, z);
        T n2 = sqlength(v);
        if (n2 <= (T)1e-6)
            return (T)0;
        return (T)2 * lol::atan2(lol::sqrt(n2), w);
    }

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, quat_t<U> const &v);

    /* XXX: storage order is wxyz, unlike vectors! */
    T w, x, y, z;
};

static_assert(sizeof(f16quat) == 8, "sizeof(f16quat) == 8");
static_assert(sizeof(quat) == 16, "sizeof(quat) == 16");
static_assert(sizeof(dquat) == 32, "sizeof(dquat) == 32");

/*
 * SQT transforms: scale / rotation / translation
 */

template<typename T>
struct sqt_t
{
    /* Default constructor and copy constructor */
    inline constexpr sqt_t() : s(), q(), t() {}
    inline constexpr sqt_t(sqt_t<T> const &other)
      : q(other.q), t(other.t), s(other.s) {}

    /* Explicit constructor for type conversion */
    template<typename U>
    explicit inline constexpr sqt_t(sqt_t<U> const &other)
      : q(other.q), t(other.t), s(other.s) {}

    /* Various explicit constructors */
    explicit inline constexpr sqt_t(T const &s_,
                                    quat_t<T> const &q_,
                                    vec_t<T,3> const &t_)
      : q(q_), t(t_), s(s_) {}

    explicit inline constexpr sqt_t(T const &s_)
      : q(1.f), t(0.f), s(s_) {}

    explicit inline constexpr sqt_t(quat_t<T> const &q_)
      : q(q_), t(0.f), s(1.f) {}

    explicit inline constexpr sqt_t(vec_t<T,3> const &t_)
      : q(1.f), t(t_), s(1.f) {}

    /* Transform vectors or points */
    inline vec_t<T,3> transform(vec_t<T,3> const &v) const
    {
        return t + q.transform(s * v);
    }

    inline vec_t<T,4> transform(vec_t<T,4> const &v) const
    {
        // XXX: needs serious testing for w != 1
        vec_t<T,4> tmp = q.transform(vec_t<T,4>(s * v.xyz, v.w));
        return vec_t<T,4>(tmp.xyz, 0.f) + vec_t<T,4>(t, 1.f) * tmp.w;
    }

    inline vec_t<T,3> operator *(vec_t<T,3> const &v) const
    {
        return transform(v);
    }

    inline vec_t<T,4> operator *(vec_t<T,4> const &v) const
    {
        return transform(v);
    }

    /* Compose two SQTs together */
    inline sqt_t<T> operator *(sqt_t<T> const &other) const
    {
        return sqt_t<T>(s * other.s,
                        q * other.q,
                        transform(other.t));
    }

    quat_t<T> q;
    vec_t<T,3> t;
    T s;
};

/*
 * Common operations on transforms
 */

template<typename T>
static inline T dot(cmplx_t<T> const &t1, cmplx_t<T> const &t2)
{
    T ret(0);
    for (size_t i = 0; i < sizeof(t1) / sizeof(T); ++i)
        ret += t1[i] * t2[i];
    return ret;
}

template<typename T>
static inline T sqlength(cmplx_t<T> const &t)
{
    return dot(t, t);
}

template<typename T>
static inline T length(cmplx_t<T> const &t)
{
    /* FIXME: this is not very nice */
    return (T)sqrt((double)sqlength(t));
}

template<typename T>
static inline T norm(cmplx_t<T> const &t)
{
    return length(t);
}

template<typename T>
static inline cmplx_t<T> normalize(cmplx_t<T> const &z)
{
    T norm = (T)length(z);
    return norm ? z / norm : cmplx_t<T>(T(0));
}

/* XXX: duplicate */

template<typename T>
static inline T dot(quat_t<T> const &t1, quat_t<T> const &t2)
{
    T ret(0);
    for (size_t i = 0; i < sizeof(t1) / sizeof(T); ++i)
        ret += t1[i] * t2[i];
    return ret;
}

template<typename T>
static inline T sqlength(quat_t<T> const &t)
{
    return dot(t, t);
}

template<typename T>
static inline T length(quat_t<T> const &t)
{
    /* FIXME: this is not very nice */
    return (T)sqrt((double)sqlength(t));
}

template<typename T>
static inline T norm(quat_t<T> const &t)
{
    return length(t);
}

template<typename T>
static inline quat_t<T> normalize(quat_t<T> const &z)
{
    T norm = (T)length(z);
    return norm ? z / norm : quat_t<T>(T(0));
}

/*
 * Complex numbers only
 */

template<typename T>
static inline cmplx_t<T> re(cmplx_t<T> const &z)
{
    return ~z / sqlength(z);
}

template<typename T>
static inline cmplx_t<T> operator /(T a, cmplx_t<T> const &b)
{
    return a * re(b);
}

template<typename T>
static inline cmplx_t<T> operator /(cmplx_t<T> a, cmplx_t<T> const &b)
{
    return a * re(b);
}

template<typename T>
static inline bool operator ==(cmplx_t<T> const &a, T b)
{
    return (a.x == b) && !a.y;
}

template<typename T>
static inline bool operator !=(cmplx_t<T> const &a, T b)
{
    return (a.x != b) || a.y;
}

template<typename T>
static inline bool operator ==(T a, cmplx_t<T> const &b) { return b == a; }

template<typename T>
static inline bool operator !=(T a, cmplx_t<T> const &b) { return b != a; }

/*
 * Quaternions only
 */

template<typename T>
static inline quat_t<T> re(quat_t<T> const &q)
{
    return ~q / sqlength(q);
}

template<typename T>
static inline quat_t<T> operator /(T x, quat_t<T> const &y)
{
    return x * re(y);
}

template<typename T>
static inline quat_t<T> operator /(quat_t<T> const &x, quat_t<T> const &y)
{
    return x * re(y);
}

template<typename T>
extern quat_t<T> slerp(quat_t<T> const &qa, quat_t<T> const &qb, T f);

#if !LOL_FEATURE_CXX11_CONSTEXPR
#undef constexpr
#endif

} /* namespace lol */

