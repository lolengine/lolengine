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
    template<typename U> \
    inline Vec##elems<T> operator op(Vec##elems<U> const &val) const \
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
    inline T& operator[](int n) { return *(&x + n); } \
    inline T const& operator[](int n) const { return *(&x + n); } \
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
    template<typename U> \
    inline operator Vec##elems<U>() const \
    { \
        Vec##elems<U> ret; \
        for (int n = 0; n < elems; n++) \
            ret[n] = static_cast<U>((*this)[n]); \
        return ret; \
    } \
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
    inline Vec2() { x = y = 0; }
    inline Vec2(T val) { x = y = val; }
    inline Vec2(T _x, T _y) { x = _x; y = _y; }

    OPERATORS(2)

    union { T x; T a; T i; };
    union { T y; T b; T j; };
};

typedef Vec2<float> Float2;
typedef Vec2<int> Int2;

template <typename T> struct Vec3
{
    inline Vec3() { x = y = z = 0; }
    inline Vec3(T val) { x = y = z = val; }
    inline Vec3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }

    OPERATORS(3)

    union { T x; T a; T i; };
    union { T y; T b; T j; };
    union { T z; T c; T k; };
};

typedef Vec3<float> Float3;
typedef Vec3<int> Int3;

#define SCALAR_GLOBAL(elems, op, U) \
    template<typename T> \
    static inline Vec##elems<U> operator op(U const &val, \
                                            Vec##elems<T> const &that) \
    { \
        Vec##elems<U> ret; \
        for (int n = 0; n < elems; n++) \
            ret[n] = val op that[n]; \
        return ret; \
    }

#define SCALAR_GLOBAL2(elems, op) \
    SCALAR_GLOBAL(elems, op, int) \
    SCALAR_GLOBAL(elems, op, float)

#define GLOBALS(elems) \
    SCALAR_GLOBAL2(elems, -) \
    SCALAR_GLOBAL2(elems, +) \
    SCALAR_GLOBAL2(elems, *) \
    SCALAR_GLOBAL2(elems, /)

GLOBALS(2)
GLOBALS(3)

#endif // __DH_MATRIX_H__

