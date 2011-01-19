//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Matrix classes
// ------------------
//

#if !defined __DH_MATRIX_H__
#define __DH_MATRIX_H__

#include <cmath>

#define VECTOR_OP(elems, op) \
    inline Vec##elems<T> operator op(Vec##elems<T> const &val) const \
    { \
        Vec##elems<T> ret; \
        for (int n = 0; n < elems; n++) \
            ret[n] = (*this)[n] op val[n]; \
        return ret; \
    }

#define SCALAR_OP(elems, op) \
    inline Vec##elems<T> operator op(T const &val) const \
    { \
        Vec##elems<T> ret; \
        for (int n = 0; n < elems; n++) \
            ret[n] = (*this)[n] op val; \
        return ret; \
    }

#define OPERATORS(elems) \
    T& operator[](int n) { return *(&x + n); } \
    T const& operator[](int n) const { return *(&x + n); } \
    \
    VECTOR_OP(elems, -) \
    VECTOR_OP(elems, +) \
    VECTOR_OP(elems, *) \
    VECTOR_OP(elems, /) \
    \
    SCALAR_OP(elems, -) \
    SCALAR_OP(elems, +) \
    SCALAR_OP(elems, *) \
    SCALAR_OP(elems, /) \
    \
    inline float len() const \
    { \
        T acc = 0; \
        for (int n = 0; n < elems; n++) \
            acc += (*this)[n] * (*this)[n]; \
        return sqrtf((float)acc); \
    }

template <typename T> struct Vec2
{
    Vec2() { x = y = 0; }
    Vec2(T _x, T _y) { x = _x; y = _y; }

    OPERATORS(2)

    union { T x; T a; T i; };
    union { T y; T b; T j; };
};

typedef Vec2<float> Float2;
typedef Vec2<int> Int2;

template <typename T> struct Vec3
{
    Vec3() { x = y = z = 0; }
    Vec3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }

    OPERATORS(3)

    union { T x; T a; T i; };
    union { T y; T b; T j; };
    union { T z; T c; T k; };
};

typedef Vec3<float> Float3;
typedef Vec3<int> Int3;

#endif // __DH_MATRIX_H__

