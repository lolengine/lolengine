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

#define VECTOR_TYPES(tname, suffix) \
    template <typename T> struct tname; \
    typedef tname<half> f16##suffix; \
    typedef tname<float> suffix; \
    typedef tname<double> f64##suffix; \
    typedef tname<int8_t> i8##suffix; \
    typedef tname<uint8_t> u8##suffix; \
    typedef tname<int16_t> i16##suffix; \
    typedef tname<uint16_t> u16##suffix; \
    typedef tname<int32_t> i##suffix; \
    typedef tname<uint32_t> u##suffix; \
    typedef tname<int64_t> i64##suffix; \
    typedef tname<uint64_t> u64##suffix;

VECTOR_TYPES(Vec2, vec2)
VECTOR_TYPES(Cmplx, cmplx)
VECTOR_TYPES(Vec3, vec3)
VECTOR_TYPES(Vec4, vec4)
VECTOR_TYPES(Quat, quat)
VECTOR_TYPES(Mat4, mat4)

#define VECTOR_OP(op) \
    inline type_t operator op(type_t const &val) const \
    { \
        type_t ret; \
        for (size_t n = 0; n < sizeof(*this) / sizeof(T); n++) \
            ret[n] = (*this)[n] op val[n]; \
        return ret; \
    } \
    \
    inline type_t operator op##=(type_t const &val) \
    { \
        return *this = (*this) op val; \
    }

#define BOOL_OP(op, op2, ret) \
    inline bool operator op(type_t const &val) const \
    { \
        for (size_t n = 0; n < sizeof(*this) / sizeof(T); n++) \
            if (!((*this)[n] op2 val[n])) \
                return !ret; \
        return ret; \
    }

#define SCALAR_OP(op) \
    inline type_t operator op(T const &val) const \
    { \
        type_t ret; \
        for (size_t n = 0; n < sizeof(*this) / sizeof(T); n++) \
            ret[n] = (*this)[n] op val; \
        return ret; \
    } \
    \
    inline type_t operator op##=(T const &val) \
    { \
        return *this = (*this) op val; \
    }

#define LINEAR_OPS() \
    inline T& operator[](int n) { return *(&x + n); } \
    inline T const& operator[](int n) const { return *(&x + n); } \
    \
    VECTOR_OP(-) \
    VECTOR_OP(+) \
    \
    BOOL_OP(==, ==, true) \
    BOOL_OP(!=, ==, false) \
    \
    SCALAR_OP(*) \
    SCALAR_OP(/) \
    \
    inline type_t operator -() const \
    { \
        type_t ret; \
        for (size_t n = 0; n < sizeof(*this) / sizeof(T); n++) \
            ret[n] = -(*this)[n]; \
        return ret; \
    } \
    \
    inline T sqlen() const \
    { \
        T acc = 0; \
        for (size_t n = 0; n < sizeof(*this) / sizeof(T); n++) \
            acc += (*this)[n] * (*this)[n]; \
        return acc; \
    } \
    \
    inline double len() const \
    { \
        using namespace std; \
        return sqrt((double)sqlen()); \
    } \
    \
    void printf() const;

#define COMPLEX_OPS() \
    inline type_t operator *(type_t const &val) const \
    { \
        return type_t(x * val.x - y * val.y, x * val.y + y * val.x); \
    } \
    \
    inline type_t operator *=(type_t const &val) \
    { \
        return *this = (*this) * val; \
    } \
    \
    inline type_t operator ~() const \
    { \
        return type_t(x, -y); \
    } \
    \
    inline T norm() const { return len(); }

#define QUATERNION_OPS() \
    inline type_t operator *(type_t const &val) const \
    { \
        type_t ret; \
        Vec3<T> v1(x, y, z); \
        Vec3<T> v2(val.x, val.y, val.z); \
        Vec3<T> v3 = cross(v1, v2) + w * v2 + val.w * v1; \
        ret.x = v3.x; \
        ret.y = v3.y; \
        ret.z = v3.z; \
        ret.w = w * val.w - dot(v1, v2); \
        return ret; \
    } \
    \
    inline type_t operator *=(type_t const &val) \
    { \
        return *this = (*this) * val; \
    } \
    \
    inline type_t operator ~() const \
    { \
        type_t ret; \
        for (int n = 0; n < 3; n++) \
            ret[n] = -(*this)[n]; \
        ret[3] = (*this)[3]; \
        return ret; \
    } \
    \
    inline T norm() const { return sqlen(); }

#define OTHER_OPS(tname) \
    VECTOR_OP(*) \
    VECTOR_OP(/) \
    \
    BOOL_OP(<=, <=, true) \
    BOOL_OP(>=, >=, true) \
    BOOL_OP(<, <, true) \
    BOOL_OP(>, >, true) \
    \
    template<typename U> \
    inline operator tname<U>() const \
    { \
        tname<U> ret; \
        for (size_t n = 0; n < sizeof(*this) / sizeof(T); n++) \
            ret[n] = static_cast<U>((*this)[n]); \
        return ret; \
    } \
    \
    template<typename U> \
    friend U dot(tname<U>, tname<U>);

#define SWIZZLE2(e1, e2) \
    inline Vec2<T> e1##e2() const \
    { \
        return Vec2<T>(this->e1, this->e2); \
    }

#define SWIZZLE3(e1, e2, e3) \
    inline Vec3<T> e1##e2##e3() const \
    { \
        return Vec3<T>(this->e1, this->e2, this->e3); \
    }

#define SWIZZLE4(e1, e2, e3, e4) \
    inline Vec4<T> e1##e2##e3##e4() const \
    { \
        return Vec4<T>(this->e1, this->e2, this->e3, this->e4); \
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

/*
 * 2-element vectors
 */

template <typename T> struct Vec2
{
    typedef Vec2<T> type_t;

    inline Vec2() { }
    explicit inline Vec2(T val) { x = y = val; }
    inline Vec2(T _x, T _y) { x = _x; y = _y; }

    LINEAR_OPS()
    OTHER_OPS(Vec2)

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

/*
 * 2-element complexes
 */

template <typename T> struct Cmplx
{
    typedef Cmplx<T> type_t;

    inline Cmplx() { }
    inline Cmplx(T val) : x(val), y(0) { }
    inline Cmplx(T _x, T _y) : x(_x), y(_y) { }

    LINEAR_OPS()
    COMPLEX_OPS()

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Cmplx<U> const &v);
#endif

    T x, y;
};

template<typename T>
static inline Cmplx<T> re(Cmplx<T> const &val)
{
    return ~val / val.sqlen();
}

template<typename T>
static inline Cmplx<T> operator /(T x, Cmplx<T> const &y)
{
    return x * re(y);
}

template<typename T>
static inline Cmplx<T> operator /(Cmplx<T> x, Cmplx<T> const &y)
{
    return x * re(y);
}

/*
 * 3-element vectors
 */

template <typename T> struct Vec3
{
    typedef Vec3<T> type_t;

    inline Vec3() { }
    explicit inline Vec3(T val) { x = y = z = val; }
    inline Vec3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }
    inline Vec3(Vec2<T> _xy, T _z) { x = _xy.x; y = _xy.y; z = _z; }
    inline Vec3(T _x, Vec2<T> _yz) { x = _x; y = _yz.x; z = _yz.y; }

    LINEAR_OPS()
    OTHER_OPS(Vec3)

    SWIZZLE23(x); SWIZZLE23(y); SWIZZLE23(z);
    SWIZZLE333(x); SWIZZLE333(y); SWIZZLE333(z);
    SWIZZLE4333(x); SWIZZLE4333(y); SWIZZLE4333(z);

    template<typename U>
    friend Vec3<U> cross(Vec3<U>, Vec3<U>);

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec3<U> const &v);
#endif

    union { T x; T a; T i; };
    union { T y; T b; T j; };
    union { T z; T c; T k; };
};

/*
 * 4-element vectors
 */

template <typename T> struct Vec4
{
    typedef Vec4<T> type_t;

    inline Vec4() { }
    explicit inline Vec4(T val) : x(val), y(val), z(val), w(val) { }
    inline Vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }
    inline Vec4(Vec2<T> _xy, T _z, T _w) : x(_xy.x), y(_xy.y), z(_z), w(_w) { }
    inline Vec4(T _x, Vec2<T> _yz, T _w) : x(_x), y(_yz.x), z(_yz.y), w(_w) { }
    inline Vec4(T _x, T _y, Vec2<T> _zw) : x(_x), y(_y), z(_zw.x), w(_zw.y) { }
    inline Vec4(Vec2<T> _xy, Vec2<T> _zw) : x(_xy.x), y(_xy.y), z(_zw.x), w(_zw.y) { }
    inline Vec4(Vec3<T> _xyz, T _w) : x(_xyz.x), y(_xyz.y), z(_xyz.z), w(_w) { }
    inline Vec4(T _x, Vec3<T> _yzw) : x(_x), y(_yzw.x), z(_yzw.y), w(_yzw.z) { }

    LINEAR_OPS()
    OTHER_OPS(Vec4)

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

/*
 * 4-element quaternions
 */

template <typename T> struct Quat
{
    typedef Quat<T> type_t;

    inline Quat() { }
    inline Quat(T val) : x(0), y(0), z(0), w(val) { }
    inline Quat(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }

    Quat(Mat4<T> const &m);

    LINEAR_OPS()
    QUATERNION_OPS()

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Quat<U> const &v);
#endif

    T x, y, z, w;
};

template<typename T>
static inline Quat<T> re(Quat<T> const &val)
{
    return ~val / val.norm();
}

template<typename T>
static inline Quat<T> operator /(T x, Quat<T> const &y)
{
    return x * re(y);
}

template<typename T>
static inline Quat<T> operator /(Quat<T> x, Quat<T> const &y)
{
    return x * re(y);
}

/*
 * Common operators for all vector types, including quaternions
 */

#define SCALAR_GLOBAL(tname, op, U) \
    template<typename T> \
    static inline tname<U> operator op(U const &val, tname<T> const &that) \
    { \
        tname<U> ret; \
        for (size_t n = 0; n < sizeof(that) / sizeof(that[0]); n++) \
            ret[n] = val op that[n]; \
        return ret; \
    }

#define SCALAR_GLOBAL2(tname, op) \
    SCALAR_GLOBAL(tname, op, int) \
    SCALAR_GLOBAL(tname, op, float) \
    SCALAR_GLOBAL(tname, op, double)

#define GLOBALS(tname) \
    SCALAR_GLOBAL2(tname, *) \
    \
    template<typename T> \
    static inline tname<T> normalize(tname<T> const &val) \
    { \
        T norm = val.len(); \
        return norm ? val / norm : val * 0; \
    }

GLOBALS(Vec2)
GLOBALS(Cmplx)
GLOBALS(Vec3)
GLOBALS(Vec4)
GLOBALS(Quat)

/*
 * 4×4-element matrices
 */

template <typename T> struct Mat4
{
    typedef Mat4<T> type_t;

    inline Mat4() { }
    explicit inline Mat4(T val)
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

    /* Helpers for transformation matrices */
    static Mat4<T> translate(T x, T y, T z);
    static Mat4<T> translate(Vec3<T> v);
    static Mat4<T> rotate(T angle, T x, T y, T z);
    static Mat4<T> rotate(T angle, Vec3<T> v);
    static Mat4<T> rotate(Quat<T> q);

    static inline Mat4<T> translate(Mat4<T> mat, Vec3<T> v)
    {
        return translate(v) * mat;
    }

    static inline Mat4<T> rotate(Mat4<T> mat, T angle, Vec3<T> v)
    {
        return rotate(angle, v) * mat;
    }

    /* Helpers for view matrices */
    static Mat4<T> lookat(Vec3<T> eye, Vec3<T> center, Vec3<T> up);

    /* Helpers for projection matrices */
    static Mat4<T> ortho(T left, T right, T bottom, T top, T near, T far);
    static Mat4<T> frustum(T left, T right, T bottom, T top, T near, T far);
    static Mat4<T> perspective(T fov_y, T width, T height, T near, T far);

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

} /* namespace lol */

#endif // __LOL_MATRIX_H__

