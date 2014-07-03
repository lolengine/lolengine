//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The complex and quaternion classes
// ----------------------------------
//

#if !defined __LOL_MATH_TRANSFORM_H__
#define __LOL_MATH_TRANSFORM_H__

#include <ostream>

#include <lol/math/half.h>
#include <lol/math/real.h>

namespace lol
{

#if !LOL_FEATURE_CXX11_CONSTEXPR
#   define constexpr /* */
#endif

/*
 * 2-element complexes
 */

template <typename T> struct Cmplx
{
    typedef Cmplx<T> type;

    inline constexpr Cmplx() {}
    inline constexpr Cmplx(T X) : x(X), y(T(0)) {}
    inline constexpr Cmplx(T X, T Y) : x(X), y(Y) {}

    template<typename U>
    explicit inline constexpr Cmplx(Cmplx<U> const &z)
      : x(z.x), y(z.y) {}

    LOL_COMMON_MEMBER_OPS(x)
    LOL_NONVECTOR_MEMBER_OPS()

    inline Cmplx<T> operator *(Cmplx<T> const &val) const
    {
        return Cmplx<T>(x * val.x - y * val.y, x * val.y + y * val.x);
    }

    inline Cmplx<T> operator *=(Cmplx<T> const &val)
    {
        return *this = (*this) * val;
    }

    inline Cmplx<T> operator ~() const
    {
        return Cmplx<T>(x, -y);
    }

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Cmplx<U> const &v);

    T x, y;
};

template<typename T>
static inline T dot(Cmplx<T> const &z1, Cmplx<T> const &z2)
{
    T ret(0);
    for (size_t i = 0; i < sizeof(Cmplx<T>) / sizeof(T); ++i)
        ret += z1[i] * z2[i];
    return ret;
}

template<typename T>
static inline T sqlength(Cmplx<T> const &z)
{
    return dot(z, z);
}

template<typename T>
static inline T length(Cmplx<T> const &z)
{
    /* FIXME: this is not very nice */
    return (T)sqrt((double)sqlength(z));
}

template<typename T>
static inline T norm(Cmplx<T> const &z)
{
    return length(z);
}

template<typename T>
static inline Cmplx<T> re(Cmplx<T> const &z)
{
    return ~z / sqlength(z);
}

template<typename T>
static inline Cmplx<T> normalize(Cmplx<T> const &z)
{
    T norm = (T)length(z);
    return norm ? z / norm : Cmplx<T>(T(0));
}

template<typename T>
static inline Cmplx<T> operator /(T a, Cmplx<T> const &b)
{
    return a * re(b);
}

template<typename T>
static inline Cmplx<T> operator /(Cmplx<T> a, Cmplx<T> const &b)
{
    return a * re(b);
}

template<typename T>
static inline bool operator ==(Cmplx<T> const &a, T b)
{
    return (a.x == b) && !a.y;
}

template<typename T>
static inline bool operator !=(Cmplx<T> const &a, T b)
{
    return (a.x != b) || a.y;
}

template<typename T>
static inline bool operator ==(T a, Cmplx<T> const &b) { return b == a; }

template<typename T>
static inline bool operator !=(T a, Cmplx<T> const &b) { return b != a; }

/*
 * 4-element quaternions
 */

template <typename T> struct Quat
{
    typedef Quat<T> type;

    inline constexpr Quat() {}
    inline constexpr Quat(T W) : w(W),  x(0), y(0), z(0) {}
    inline constexpr Quat(T W, T X, T Y, T Z) : w(W), x(X), y(Y), z(Z) {}

    template<typename U>
    explicit inline constexpr Quat(Quat<U> const &q)
      : w(q.w), x(q.x), y(q.y), z(q.z) {}

    Quat(matrix<3,3,T> const &m);
    Quat(matrix<4,4,T> const &m);

    LOL_COMMON_MEMBER_OPS(w)
    LOL_NONVECTOR_MEMBER_OPS()

    /* Create a unit quaternion representing a rotation around an axis. */
    static Quat<T> rotate(T degrees, T x, T y, T z);
    static Quat<T> rotate(T degrees, vec<3,T> const &v);

    /* Create a unit quaternion representing a rotation between two vectors.
     * Input vectors need not be normalised. */
    static Quat<T> rotate(vec<3,T> const &src, vec<3,T> const &dst);

    /* Convert from Euler angles. The axes in fromeuler_xyx are
     * x, then y', then x", ie. the axes are attached to the model.
     * If you want to rotate around static axes, just reverse the order
     * of the arguments. Angle values are in degrees. */
    static Quat<T> fromeuler_xyx(vec<3,T> const &v);
    static Quat<T> fromeuler_xzx(vec<3,T> const &v);
    static Quat<T> fromeuler_yxy(vec<3,T> const &v);
    static Quat<T> fromeuler_yzy(vec<3,T> const &v);
    static Quat<T> fromeuler_zxz(vec<3,T> const &v);
    static Quat<T> fromeuler_zyz(vec<3,T> const &v);
    static Quat<T> fromeuler_xyx(T phi, T theta, T psi);
    static Quat<T> fromeuler_xzx(T phi, T theta, T psi);
    static Quat<T> fromeuler_yxy(T phi, T theta, T psi);
    static Quat<T> fromeuler_yzy(T phi, T theta, T psi);
    static Quat<T> fromeuler_zxz(T phi, T theta, T psi);
    static Quat<T> fromeuler_zyz(T phi, T theta, T psi);

    /* Convert from Tait-Bryan angles (incorrectly called Euler angles,
     * but since everyone does it…). The axes in fromeuler_xyz are
     * x, then y', then z", ie. the axes are attached to the model.
     * If you want to apply yaw around x, pitch around y, and roll
     * around z, use fromeuler_xyz. Angle values are in degrees.
     * If you want to rotate around static axes, reverse the order in
     * the function name (_zyx instead of _xyz) AND reverse the order
     * of the arguments. */
    static Quat<T> fromeuler_xyz(vec<3,T> const &v);
    static Quat<T> fromeuler_xzy(vec<3,T> const &v);
    static Quat<T> fromeuler_yxz(vec<3,T> const &v);
    static Quat<T> fromeuler_yzx(vec<3,T> const &v);
    static Quat<T> fromeuler_zxy(vec<3,T> const &v);
    static Quat<T> fromeuler_zyx(vec<3,T> const &v);
    static Quat<T> fromeuler_xyz(T phi, T theta, T psi);
    static Quat<T> fromeuler_xzy(T phi, T theta, T psi);
    static Quat<T> fromeuler_yxz(T phi, T theta, T psi);
    static Quat<T> fromeuler_yzx(T phi, T theta, T psi);
    static Quat<T> fromeuler_zxy(T phi, T theta, T psi);
    static Quat<T> fromeuler_zyx(T phi, T theta, T psi);

    inline Quat<T> operator *(Quat<T> const &val) const;

    inline Quat<T> operator *=(Quat<T> const &val)
    {
        return *this = (*this) * val;
    }

    inline Quat<T> operator ~() const
    {
        return Quat<T>(w, -x, -y, -z);
    }

    inline vec<3,T> transform(vec<3,T> const &v) const
    {
        Quat<T> p = Quat<T>(0, v.x, v.y, v.z);
        Quat<T> q = *this * p / *this;
        return vec<3,T>(q.x, q.y, q.z);
    }

    inline vec<4,T> transform(vec<4,T> const &v) const
    {
        Quat<T> p = Quat<T>(0, v.x, v.y, v.z);
        Quat<T> q = *this * p / *this;
        return vec<4,T>(q.x, q.y, q.z, v.w);
    }

    inline vec<3,T> operator *(vec<3,T> const &v) const
    {
        return transform(v);
    }

    inline vec<4,T> operator *(vec<4,T> const &v) const
    {
        return transform(v);
    }

    inline vec<3,T> axis()
    {
        vec<3,T> v(x, y, z);
        T n2 = sqlength(v);
        if (n2 <= (T)1e-6)
            return vec<3,T>::axis_x;
        return normalize(v);
    }

    inline T angle()
    {
        vec<3,T> v(x, y, z);
        T n2 = sqlength(v);
        if (n2 <= (T)1e-6)
            return (T)0;
        return (T)2 * lol::atan2(lol::sqrt(n2), w);
    }

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Quat<U> const &v);

    /* XXX: storage order is wxyz, unlike vectors! */
    T w, x, y, z;
};

template<typename T>
static inline T dot(Quat<T> const &q1, Quat<T> const &q2)
{
    T ret(0);
    for (size_t i = 0; i < sizeof(Quat<T>) / sizeof(T); ++i)
        ret += q1[i] * q2[i];
    return ret;
}

template<typename T>
static inline T sqlength(Quat<T> const &q)
{
    return dot(q, q);
}

template<typename T>
static inline T length(Quat<T> const &q)
{
    /* FIXME: this is not very nice */
    return (T)sqrt((double)sqlength(q));
}

template<typename T>
static inline T norm(Quat<T> const &q)
{
    return length(q);
}

template<typename T>
static inline Quat<T> re(Quat<T> const &q)
{
    return ~q / sqlength(q);
}

template<typename T>
static inline Quat<T> normalize(Quat<T> const &q)
{
    T norm = (T)length(q);
    return norm ? q / norm : Quat<T>(T(0));
}

template<typename T>
static inline Quat<T> operator /(T x, Quat<T> const &y)
{
    return x * re(y);
}

template<typename T>
static inline Quat<T> operator /(Quat<T> const &x, Quat<T> const &y)
{
    return x * re(y);
}

template<typename T>
extern Quat<T> slerp(Quat<T> const &qa, Quat<T> const &qb, T f);

template<typename T>
inline Quat<T> Quat<T>::operator *(Quat<T> const &val) const
{
    Quat<T> ret;
    vec<3,T> v1(x, y, z);
    vec<3,T> v2(val.x, val.y, val.z);
    vec<3,T> v3 = cross(v1, v2) + w * v2 + val.w * v1;
    return Quat<T>(w * val.w - dot(v1, v2), v3.x, v3.y, v3.z);
}

#if !LOL_FEATURE_CXX11_CONSTEXPR
#undef constexpr
#endif

} /* namespace lol */

#endif // __LOL_MATH_TRANSFORM_H__

