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

#define OPERATORS(elems) \
    inline Vec##elems<T> operator+(Vec##elems<T> const &op) const \
    { \
        Vec##elems<T> ret; \
        for (int n = 0; n < elems; n++) \
            ret[n] = (*this)[n] + op[n]; \
        return ret; \
    } \
 \
    inline Vec##elems<T> operator-(Vec##elems<T> const &op) const \
    { \
        Vec##elems<T> ret; \
        for (int n = 0; n < elems; n++) \
            ret[n] = (*this)[n] - op[n]; \
        return ret; \
    }

template <typename T> struct Vec2
{
    Vec2() { x = y = 0; }
    Vec2(T _x, T _y) { x = _x; y = _y; }
    T& operator[](int n) { return *(&x + n); }
    T const& operator[](int n) const { return *(&x + n); }

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
    T& operator[](int n) { return *(&x + n); }

    OPERATORS(3)

    union { T x; T a; T i; };
    union { T y; T b; T j; };
    union { T z; T c; T k; };
};

typedef Vec3<float> Float3;
typedef Vec3<int> Int3;

#endif // __DH_MATRIX_H__

