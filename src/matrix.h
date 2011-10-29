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

#if !defined __LOL_MATRIX_H__
#define __LOL_MATRIX_H__

#include <cmath>
#if !defined __ANDROID__
#   include <iostream>
#endif

namespace lol
{

#define VECTOR_OP(elems, op) \
    template<typename U> \
    inline Vec##elems<T> operator op(Vec##elems<U> const &val) const \
    { \
        Vec##elems<T> ret; \
        for (int n = 0; n < elems; n++) \
            ret[n] = (*this)[n] op val[n]; \
        return ret; \
    } \
    \
    template<typename U> \
    inline Vec##elems<T> operator op##=(Vec##elems<U> const &val) \
    { \
        return *this = (*this) op val; \
    }

#define BOOL_OP(elems, op, op2, ret) \
    inline bool operator op(Vec##elems<T> const &val) const \
    { \
        for (int n = 0; n < elems; n++) \
            if (!((*this)[n] op2 val[n])) \
                return !ret; \
        return ret; \
    }

#define SCALAR_OP(elems, op) \
    inline Vec##elems<T> operator op(T const &val) const \
    { \
        Vec##elems<T> ret; \
        for (int n = 0; n < elems; n++) \
            ret[n] = (*this)[n] op val; \
        return ret; \
    } \
    \
    inline Vec##elems<T> operator op##=(T const &val) \
    { \
        return *this = (*this) op val; \
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
    BOOL_OP(elems, ==, ==, true) \
    BOOL_OP(elems, !=, ==, false) \
    BOOL_OP(elems, <=, <=, true) \
    BOOL_OP(elems, >=, >=, true) \
    BOOL_OP(elems, <, <, true) \
    BOOL_OP(elems, >, >, true) \
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
    inline Vec##elems<T> operator -() const \
    { \
        Vec##elems<T> ret; \
        for (int n = 0; n < elems; n++) \
            ret[n] = -(*this)[n]; \
        return ret; \
    } \
    \
    inline T sqlen() const \
    { \
        T acc = 0; \
        for (int n = 0; n < elems; n++) \
            acc += (*this)[n] * (*this)[n]; \
        return acc; \
    } \
    \
    inline float len() const \
    { \
        using namespace std; \
        return sqrtf((float)sqlen()); \
    }

#define SWIZZLE2(e1, e2) \
    inline Vec2<T> e1##e2() const \
    { \
        return Vec2<T>(e1, e2); \
    }

#define SWIZZLE3(e1, e2, e3) \
    inline Vec3<T> e1##e2##e3() const \
    { \
        return Vec3<T>(e1, e2, e3); \
    }

#define SWIZZLE4(e1, e2, e3, e4) \
    inline Vec4<T> e1##e2##e3##e4() const \
    { \
        return Vec4<T>(e1, e2, e3, e4); \
    }

#define SWIZZLE22(e1) \
    SWIZZLE2(e1, x); SWIZZLE2(e1, y);
#define SWIZZLE23(e1) \
    SWIZZLE2(e1, x); SWIZZLE2(e1, y); SWIZZLE2(e1, z);
#define SWIZZLE24(e1) \
    SWIZZLE2(e1, x); SWIZZLE2(e1, y); SWIZZLE2(e1, z); SWIZZLE2(e1, w);

#define SWIZZLE32(e1, e2) \
    SWIZZLE3(e1, e2, x); SWIZZLE3(e1, e2, y);
#define SWIZZLE322(e1) \
    SWIZZLE32(e1, x); SWIZZLE32(e1, y);
#define SWIZZLE33(e1, e2) \
    SWIZZLE3(e1, e2, x); SWIZZLE3(e1, e2, y); SWIZZLE3(e1, e2, z);
#define SWIZZLE333(e1) \
    SWIZZLE33(e1, x); SWIZZLE33(e1, y); SWIZZLE33(e1, z);
#define SWIZZLE34(e1, e2) \
    SWIZZLE3(e1, e2, x); SWIZZLE3(e1, e2, y); \
    SWIZZLE3(e1, e2, z); SWIZZLE3(e1, e2, w);
#define SWIZZLE344(e1) \
    SWIZZLE34(e1, x); SWIZZLE34(e1, y); \
    SWIZZLE34(e1, z); SWIZZLE34(e1, w);

#define SWIZZLE42(e1, e2, e3) \
    SWIZZLE4(e1, e2, e3, x); SWIZZLE4(e1, e2, e3, y);
#define SWIZZLE422(e1, e2) \
    SWIZZLE42(e1, e2, x); SWIZZLE42(e1, e2, y);
#define SWIZZLE4222(e1) \
    SWIZZLE422(e1, x); SWIZZLE422(e1, y);
#define SWIZZLE43(e1, e2, e3) \
    SWIZZLE4(e1, e2, e3, x); SWIZZLE4(e1, e2, e3, y); SWIZZLE4(e1, e2, e3, z);
#define SWIZZLE433(e1, e2) \
    SWIZZLE43(e1, e2, x); SWIZZLE43(e1, e2, y); SWIZZLE43(e1, e2, z);
#define SWIZZLE4333(e1) \
    SWIZZLE433(e1, x); SWIZZLE433(e1, y); SWIZZLE433(e1, z);
#define SWIZZLE44(e1, e2, e3) \
    SWIZZLE4(e1, e2, e3, x); SWIZZLE4(e1, e2, e3, y); \
    SWIZZLE4(e1, e2, e3, z); SWIZZLE4(e1, e2, e3, w);
#define SWIZZLE444(e1, e2) \
    SWIZZLE44(e1, e2, x); SWIZZLE44(e1, e2, y); \
    SWIZZLE44(e1, e2, z); SWIZZLE44(e1, e2, w);
#define SWIZZLE4444(e1) \
    SWIZZLE444(e1, x); SWIZZLE444(e1, y); SWIZZLE444(e1, z); SWIZZLE444(e1, w);

template <typename T> struct Vec2;
template <typename T> struct Vec3;
template <typename T> struct Vec4;

/*
 * 2-element vectors
 */

template <typename T> struct Vec2
{
    inline Vec2() { }
    inline Vec2(T val) { x = y = val; }
    inline Vec2(T _x, T _y) { x = _x; y = _y; }

    OPERATORS(2)

    SWIZZLE22(x); SWIZZLE22(y);
    SWIZZLE322(x); SWIZZLE322(y);
    SWIZZLE4222(x); SWIZZLE4222(y);

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec2<U> const &v);
#endif

    union { T x; T a; T i; };
    union { T y; T b; T j; };
};

typedef Vec2<float> vec2;
typedef Vec2<int> ivec2;

/*
 * 3-element vectors
 */

template <typename T> struct Vec3
{
    inline Vec3() { }
    inline Vec3(T val) { x = y = z = val; }
    inline Vec3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }
    inline Vec3(Vec2<T> _xy, T _z) { x = _xy.x; y = _xy.y; z = _z; }
    inline Vec3(T _x, Vec2<T> _yz) { x = _x; y = _yz.x; z = _yz.y; }

    OPERATORS(3)

    SWIZZLE23(x); SWIZZLE23(y); SWIZZLE23(z);
    SWIZZLE333(x); SWIZZLE333(y); SWIZZLE333(z);
    SWIZZLE4333(x); SWIZZLE4333(y); SWIZZLE4333(z);

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec3<U> const &v);
#endif

    union { T x; T a; T i; };
    union { T y; T b; T j; };
    union { T z; T c; T k; };
};

typedef Vec3<float> vec3;
typedef Vec3<int> ivec3;

/*
 * 4-element vectors
 */

template <typename T> struct Vec4
{
    inline Vec4() { }
    inline Vec4(T val) { x = y = z = w = val; }
    inline Vec4(T _x, T _y, T _z, T _w) { x = _x; y = _y; z = _z; w = _w; }
    inline Vec4(Vec2<T> _xy, T _z, T _w) { x = _xy.x; y = _xy.y; z = _z; w = _w; }
    inline Vec4(T _x, Vec2<T> _yz, T _w) { x = _x; y = _yz.x; z = _yz.y; w = _w; }
    inline Vec4(T _x, T _y, Vec2<T> _zw) { x = _x; y = _y; z = _zw.x; w = _zw.y; }
    inline Vec4(Vec2<T> _xy, Vec2<T> _zw) { x = _xy.x; y = _xy.y; z = _zw.x; w = _zw.y; }
    inline Vec4(Vec3<T> _xyz, T _w) { x = _xyz.x; y = _xyz.y; z = _xyz.z; w = _w; }
    inline Vec4(T _x, Vec3<T> _yzw) { x = _x; y = _yzw.x; z = _yzw.y; w = _yzw.z; }

    OPERATORS(4)

    SWIZZLE24(x); SWIZZLE24(y); SWIZZLE24(z); SWIZZLE24(w);
    SWIZZLE344(x); SWIZZLE344(y); SWIZZLE344(z); SWIZZLE344(w);
    SWIZZLE4444(x); SWIZZLE4444(y); SWIZZLE4444(z); SWIZZLE4444(w);

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec4<U> const &v);
#endif

    union { T x; T a; T i; };
    union { T y; T b; T j; };
    union { T z; T c; T k; };
    union { T w; T d; T l; };
};

typedef Vec4<float> vec4;
typedef Vec4<int> ivec4;

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
GLOBALS(4)

/*
 * 4×4-element matrices
 */

template <typename T> struct Mat4
{
    inline Mat4() { }
    inline Mat4(T val)
    {
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
                v[i][j] = (i == j) ? val : 0;
    }
    inline Mat4(Vec4<T> v0, Vec4<T> v1, Vec4<T> v2, Vec4<T> v3)
    {
        v[0] = v0; v[1] = v1; v[2] = v2; v[3] = v3;
    }

    inline Vec4<T>& operator[](int n) { return v[n]; }
    inline Vec4<T> const& operator[](int n) const { return v[n]; }

    T det() const;
    Mat4<T> invert() const;

    static Mat4<T> ortho(T left, T right, T bottom, T top, T near, T far);
    static Mat4<T> frustum(T left, T right, T bottom, T top, T near, T far);
    static Mat4<T> perspective(T theta, T width, T height, T near, T far);
    static Mat4<T> translate(T x, T y, T z);
    static Mat4<T> rotate(T theta, T x, T y, T z);

    void printf() const;

#if !defined __ANDROID__
    template<class U>
    friend std::ostream &operator<<(std::ostream &stream, Mat4<U> const &m);
#endif

    inline Mat4<T> operator +(Mat4<T> const val) const
    {
        Mat4<T> ret;
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
                ret[i][j] = v[i][j] + val[i][j];
        return ret;
    }

    inline Mat4<T> operator +=(Mat4<T> const val)
    {
        return *this = *this + val;
    }

    inline Mat4<T> operator -(Mat4<T> const val) const
    {
        Mat4<T> ret;
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
                ret[i][j] = v[i][j] - val[i][j];
        return ret;
    }

    inline Mat4<T> operator -=(Mat4<T> const val)
    {
        return *this = *this - val;
    }

    inline Mat4<T> operator *(Mat4<T> const val) const
    {
        Mat4<T> ret;
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
            {
                T tmp = 0;
                for (int k = 0; k < 4; k++)
                    tmp += v[k][j] * val[i][k];
                ret[i][j] = tmp;
            }
        return ret;
    }

    inline Mat4<T> operator *=(Mat4<T> const val)
    {
        return *this = *this * val;
    }

    inline Vec4<T> operator *(Vec4<T> const val) const
    {
        Vec4<T> ret;
        for (int j = 0; j < 4; j++)
        {
            T tmp = 0;
            for (int i = 0; i < 4; i++)
                tmp += v[i][j] * val[i];
            ret[j] = tmp;
        }
        return ret;
    }

    Vec4<T> v[4];
};

typedef Mat4<float> mat4;
typedef Mat4<int> imat4;

} /* namespace lol */

#endif // __LOL_MATRIX_H__

