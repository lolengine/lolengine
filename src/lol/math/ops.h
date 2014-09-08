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
// Operations for vector classes
// -----------------------------
//

#include <ostream>
#include <type_traits>

#include <lol/math/half.h>
#include <lol/math/real.h>

namespace lol
{

/*
 * Utility namespaces for traits -- this file uses a combination of
 * ADL black magic and enable_if to ensure that only the expected type
 * conversions are done.
 *
 * vec_t (swizzle) needs swizzle_ops
 * vec_t (generic) needs linear_ops + componentwise_ops
 * vec_t (specialisation) needs swizzle_ops + linear_ops + componentwise_ops
 * mat_t (all) needs linear_ops
 * cmplx_t quat_t need linear_ops
 *
 * We can only inherit from one class, because Visual Studio will not
 * perform EBCO (empty base class optimisation) when there is multiple
 * inheritance.
 */

namespace linear_ops
{
    template<typename T>
    struct base {};
}

namespace componentwise_ops
{
    template<typename T>
    struct base : public linear_ops::base<T> {};
}

namespace swizzle_ops
{
    template<typename T, int SWIZZLE = FULL_SWIZZLE>
    struct base {};

    template<typename T>
    struct base<T, FULL_SWIZZLE> : public componentwise_ops::base<T> {};
}

/*
 * Operators for swizzled vectors. Since template deduction cannot be
 * done for two arbitrary vec_t<> values, we help the compiler understand
 * the expected type.
 */

namespace swizzle_ops
{

template<typename T, int N, int SWIZZLE1, int SWIZZLE2>
static inline typename std::enable_if<SWIZZLE1 != FULL_SWIZZLE || SWIZZLE2 != FULL_SWIZZLE, bool>::type
operator ==(vec_t<T,N,SWIZZLE1> const &a, vec_t<T,N,SWIZZLE2> const &b)
{
    return vec_t<T,N>(a) == vec_t<T,N>(b);
}

template<typename T, int N, int SWIZZLE1, int SWIZZLE2>
static inline typename std::enable_if<SWIZZLE1 != FULL_SWIZZLE || SWIZZLE2 != FULL_SWIZZLE, bool>::type
operator !=(vec_t<T,N,SWIZZLE1> const &a, vec_t<T,N,SWIZZLE2> const &b)
{
    return vec_t<T,N>(a) != vec_t<T,N>(b);
}

#define LOL_SWIZZLE_V_VV_OP(op) \
    template<typename T, int N, int SWIZZLE1, int SWIZZLE2> \
    inline typename std::enable_if<SWIZZLE1 != FULL_SWIZZLE \
                                || SWIZZLE2 != FULL_SWIZZLE,vec_t<T,N>>::type \
        operator op(vec_t<T,N,SWIZZLE1> const &a, \
                    vec_t<T,N,SWIZZLE2> const &b) \
    { \
        return vec_t<T,N>(a) op vec_t<T,N>(b); \
    } \
    \
    template<typename T, int N, int SWIZZLE> \
    inline typename std::enable_if<SWIZZLE != FULL_SWIZZLE,vec_t<T,N>>::type & \
        operator op##=(vec_t<T,N> &a, \
                       vec_t<T,N,SWIZZLE> const &b) \
    { \
        return a op##= vec_t<T,N>(b); \
    } \
    \
    template<typename T, int N, int SWIZZLE> \
    inline typename std::enable_if<SWIZZLE != FULL_SWIZZLE,vec_t<T,N>>::type \
        operator op(vec_t<T,N,SWIZZLE> const &a, T const &b) \
    { \
        return vec_t<T,N>(a) op b; \
    }

LOL_SWIZZLE_V_VV_OP(+)
LOL_SWIZZLE_V_VV_OP(-)
LOL_SWIZZLE_V_VV_OP(*)
LOL_SWIZZLE_V_VV_OP(/)

#undef LOL_SWIZZLE_V_VV_OP

} /* namespace swizzle_ops */


/*
 * Linear operations: operators and functions that work on all types
 * (vectors, matrices, quaternions...) such as addition or equality test.
 *
 * Others, e.g. multiplication, cannot be implemented here, since it should
 * be implemented as per-component multiplication for vectors, and matrix
 * product for matrices.
 */

namespace linear_ops
{

/*
 * Comparisons
 */

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, bool>::type
operator ==(V const &a, V const &b)
{
    for (int i = 0; i < V::count; ++i)
        if (!(a[i] == b[i]))
            return false;
    return true;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, bool>::type
operator !=(V const &a, V const &b)
{
    for (int i = 0; i < V::count; ++i)
        if (a[i] != b[i])
            return true;
    return false;
}

/*
 * Unary plus and minus
 */

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
operator +(V const &v)
{
    return v;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
operator -(V const &v)
{
    typename V::type ret;
    for (int i = 0; i < V::count; ++i)
        ret[i] = -v[i];
    return ret;
}

/*
 * Addition and subtraction
 */

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
operator +(V const &a, V const &b)
{
    typename V::type ret;
    for (int i = 0; i < V::count; ++i)
        ret[i] = a[i] + b[i];
    return ret;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
&operator +=(V &a, V const &b)
{
    return a = a + b;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
operator -(V const &a, V const &b)
{
    typename V::type ret;
    for (int i = 0; i < V::count; ++i)
        ret[i] = a[i] - b[i];
    return ret;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
&operator -=(V &a, V const &b)
{
    return a = a - b;
}

/*
 * Multiplication by scalar (left)
 */

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
operator *(typename V::element const &val, V const &a)
{
    typename V::type ret;
    for (int i = 0; i < V::count; ++i)
        ret[i] = val * a[i];
    return ret;
}

/*
 * Multiplication/division by scalar (right)
 */

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
operator *(V const &a, typename V::element const &val)
{
    typename V::type ret;
    for (int i = 0; i < V::count; ++i)
        ret[i] = a[i] * val;
    return ret;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type &
operator *=(V &a, typename V::element const &val)
{
    return a = a * val;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
operator /(V const &a, typename V::element const &val)
{
    typename V::type ret;
    for (int i = 0; i < V::count; ++i)
        ret[i] = a[i] / val;
    return ret;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type &
operator /=(V &a, typename V::element const &val)
{
    return a = a / val;
}

} /* namespace linear_ops */


/*
 * Operations that work component-wise, such as comparisons or multiplication.
 * This is only for vector types, as the other types (matrices, quaternions,
 * complexes) have different meanings.
 */

namespace componentwise_ops
{

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
operator *(V const &a, V const &b)
{
    typename V::type ret;
    for (int i = 0; i < V::count; ++i)
        ret[i] = a[i] * b[i];
    return ret;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
&operator *=(V &a, V const &b)
{
    return a = a * b;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
operator /(V const &a, V const &b)
{
    typename V::type ret;
    for (int i = 0; i < V::count; ++i)
        ret[i] = a[i] / b[i];
    return ret;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, typename V::type>::type
&operator /=(V &a, V const &b)
{
    return a = a / b;
}

/*
 * Comparisons
 */

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, bool>::type
operator <(V const &a, V const &b)
{
    for (int i = 0; i < V::count; ++i)
        if (!(a[i] < b[i]))
            return false;
    return true;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, bool>::type
operator >(V const &a, V const &b)
{
    for (int i = 0; i < V::count; ++i)
        if (!(a[i] > b[i]))
            return false;
    return true;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, bool>::type
operator <=(V const &a, V const &b)
{
    for (int i = 0; i < V::count; ++i)
        if (!(a[i] <= b[i]))
            return false;
    return true;
}

template<typename V>
static inline typename std::enable_if<std::is_base_of<base<typename V::element>, V>::value, bool>::type
operator >=(V const &a, V const &b)
{
    for (int i = 0; i < V::count; ++i)
        if (!(a[i] >= b[i]))
            return false;
    return true;
}

} /* namespace componentwise_ops */

} /* namespace lol */

