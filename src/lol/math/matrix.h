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

#if !defined __LOL_MATH_MATRIX_H__
#define __LOL_MATH_MATRIX_H__

#include <stdint.h>
#include <cmath>
#if !defined __ANDROID__
#   include <iostream>
#endif

namespace lol
{

class half;
class real;

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

/*
 * Magic swizzling (part 1/2)
 */

template<typename T, int I, int J> struct MagicVec2
{
    inline Vec2<T> operator =(Vec2<T> that);

    float ptr[1 + (I > J ? I : J)];
};

template<typename T, int I, int J, int K> struct MagicVec3
{
    inline Vec3<T> operator =(Vec3<T> that);

    float ptr[1 + (I > J ? I > K ? I : K
                         : J > K ? J : K)];
};

template<typename T, int I, int J, int K, int L> struct MagicVec4
{
    inline Vec4<T> operator =(Vec4<T> that);

    float ptr[1 + (I > J ? I > K ? I > L ? I : L : K > L ? K : L
                         : J > K ? J > L ? J : L : K > L ? K : L)];
};

/*
 * Helper macros for vector type member functions
 */

#define MEMBER_OPS() \
    inline T& operator[](int n) { return *(&x + n); } \
    inline T const& operator[](int n) const { return *(&x + n); } \
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
    inline T norm() const { return len(*this); }

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
    }

#define OTHER_MEMBER_OPS(tname) \
    template<typename U> \
    inline operator tname<U>() const \
    { \
        tname<U> ret; \
        for (size_t n = 0; n < sizeof(*this) / sizeof(T); n++) \
            ret[n] = static_cast<U>((*this)[n]); \
        return ret; \
    }

/*
 * 2-element vectors
 */

template <typename T> struct Vec2
{
    typedef Vec2<T> type_t;

    inline Vec2() { }
    explicit inline Vec2(T val) { x = y = val; }
    inline Vec2(T _x, T _y) { x = _x; y = _y; }

    template<int I, int J>
    inline Vec2(MagicVec2<T, I, J> const &v)
      : x(v.ptr[I]), y(v.ptr[J]) {}

    template<typename U, int I, int J>
    explicit inline Vec2(MagicVec2<U, I, J> const &v)
      : x(v.ptr[I]), y(v.ptr[J]) {}

    MEMBER_OPS()
    OTHER_MEMBER_OPS(Vec2)

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec2<U> const &v);
#endif

    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T s, t; };

        MagicVec2<T,0,0> xx, rr, ss;
        MagicVec2<T,0,1> xy, rg, st;
        MagicVec2<T,1,0> yx, gr, ts;
        MagicVec2<T,1,1> yy, gg, tt;
        MagicVec3<T,0,0,0> xxx, rrr, sss;
        MagicVec3<T,0,0,1> xxy, rrg, sst;
        MagicVec3<T,0,1,0> xyx, rgr, sts;
        MagicVec3<T,0,1,1> xyy, rgg, stt;
        MagicVec3<T,1,0,0> yxx, grr, tss;
        MagicVec3<T,1,0,1> yxy, grg, tst;
        MagicVec3<T,1,1,0> yyx, ggr, tts;
        MagicVec3<T,1,1,1> yyy, ggg, ttt;
        MagicVec4<T,0,0,0,0> xxxx, rrrr, ssss;
        MagicVec4<T,0,0,0,1> xxxy, rrrg, ssst;
        MagicVec4<T,0,0,1,0> xxyx, rrgr, ssts;
        MagicVec4<T,0,0,1,1> xxyy, rrgg, sstt;
        MagicVec4<T,0,1,0,0> xyxx, rgrr, stss;
        MagicVec4<T,0,1,0,1> xyxy, rgrg, stst;
        MagicVec4<T,0,1,1,0> xyyx, rggr, stts;
        MagicVec4<T,0,1,1,1> xyyy, rggg, sttt;
        MagicVec4<T,1,0,0,0> yxxx, grrr, tsss;
        MagicVec4<T,1,0,0,1> yxxy, grrg, tsst;
        MagicVec4<T,1,0,1,0> yxyx, grgr, tsts;
        MagicVec4<T,1,0,1,1> yxyy, grgg, tstt;
        MagicVec4<T,1,1,0,0> yyxx, ggrr, ttss;
        MagicVec4<T,1,1,0,1> yyxy, ggrg, ttst;
        MagicVec4<T,1,1,1,0> yyyx, gggr, ttts;
        MagicVec4<T,1,1,1,1> yyyy, gggg, tttt;
    };
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

    MEMBER_OPS()

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
    return ~val / sqlen(val);
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

    template<int I, int J, int K>
    inline Vec3(MagicVec3<T, I, J, K> const &v)
      : x(v.ptr[I]), y(v.ptr[J]), z(v.ptr[K]) {}

    template<typename U, int I, int J, int K>
    explicit inline Vec3(MagicVec3<U, I, J, K> const &v)
      : x(v.ptr[I]), y(v.ptr[J]), z(v.ptr[K]) {}

    MEMBER_OPS()
    OTHER_MEMBER_OPS(Vec3)

    template<typename U>
    friend Vec3<U> cross(Vec3<U>, Vec3<U>);

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec3<U> const &v);
#endif

    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { T s, t, p; };

        MagicVec2<T,0,0> xx, rr, ss;
        MagicVec2<T,0,1> xy, rg, st;
        MagicVec2<T,0,2> xz, rb, sp;
        MagicVec2<T,1,0> yx, gr, ts;
        MagicVec2<T,1,1> yy, gg, tt;
        MagicVec2<T,1,2> yz, gb, tp;
        MagicVec2<T,2,0> zx, br, ps;
        MagicVec2<T,2,1> zy, bg, pt;
        MagicVec2<T,2,2> zz, bb, pp;
        MagicVec3<T,0,0,0> xxx, rrr, sss;
        MagicVec3<T,0,0,1> xxy, rrg, sst;
        MagicVec3<T,0,0,2> xxz, rrb, ssp;
        MagicVec3<T,0,1,0> xyx, rgr, sts;
        MagicVec3<T,0,1,1> xyy, rgg, stt;
        MagicVec3<T,0,1,2> xyz, rgb, stp;
        MagicVec3<T,0,2,0> xzx, rbr, sps;
        MagicVec3<T,0,2,1> xzy, rbg, spt;
        MagicVec3<T,0,2,2> xzz, rbb, spp;
        MagicVec3<T,1,0,0> yxx, grr, tss;
        MagicVec3<T,1,0,1> yxy, grg, tst;
        MagicVec3<T,1,0,2> yxz, grb, tsp;
        MagicVec3<T,1,1,0> yyx, ggr, tts;
        MagicVec3<T,1,1,1> yyy, ggg, ttt;
        MagicVec3<T,1,1,2> yyz, ggb, ttp;
        MagicVec3<T,1,2,0> yzx, gbr, tps;
        MagicVec3<T,1,2,1> yzy, gbg, tpt;
        MagicVec3<T,1,2,2> yzz, gbb, tpp;
        MagicVec3<T,2,0,0> zxx, brr, pss;
        MagicVec3<T,2,0,1> zxy, brg, pst;
        MagicVec3<T,2,0,2> zxz, brb, psp;
        MagicVec3<T,2,1,0> zyx, bgr, pts;
        MagicVec3<T,2,1,1> zyy, bgg, ptt;
        MagicVec3<T,2,1,2> zyz, bgb, ptp;
        MagicVec3<T,2,2,0> zzx, bbr, pps;
        MagicVec3<T,2,2,1> zzy, bbg, ppt;
        MagicVec3<T,2,2,2> zzz, bbb, ppp;
        MagicVec4<T,0,0,0,0> xxxx, rrrr, ssss;
        MagicVec4<T,0,0,0,1> xxxy, rrrg, ssst;
        MagicVec4<T,0,0,0,2> xxxz, rrrb, sssp;
        MagicVec4<T,0,0,1,0> xxyx, rrgr, ssts;
        MagicVec4<T,0,0,1,1> xxyy, rrgg, sstt;
        MagicVec4<T,0,0,1,2> xxyz, rrgb, sstp;
        MagicVec4<T,0,0,2,0> xxzx, rrbr, ssps;
        MagicVec4<T,0,0,2,1> xxzy, rrbg, sspt;
        MagicVec4<T,0,0,2,2> xxzz, rrbb, sspp;
        MagicVec4<T,0,1,0,0> xyxx, rgrr, stss;
        MagicVec4<T,0,1,0,1> xyxy, rgrg, stst;
        MagicVec4<T,0,1,0,2> xyxz, rgrb, stsp;
        MagicVec4<T,0,1,1,0> xyyx, rggr, stts;
        MagicVec4<T,0,1,1,1> xyyy, rggg, sttt;
        MagicVec4<T,0,1,1,2> xyyz, rggb, sttp;
        MagicVec4<T,0,1,2,0> xyzx, rgbr, stps;
        MagicVec4<T,0,1,2,1> xyzy, rgbg, stpt;
        MagicVec4<T,0,1,2,2> xyzz, rgbb, stpp;
        MagicVec4<T,0,2,0,0> xzxx, rbrr, spss;
        MagicVec4<T,0,2,0,1> xzxy, rbrg, spst;
        MagicVec4<T,0,2,0,2> xzxz, rbrb, spsp;
        MagicVec4<T,0,2,1,0> xzyx, rbgr, spts;
        MagicVec4<T,0,2,1,1> xzyy, rbgg, sptt;
        MagicVec4<T,0,2,1,2> xzyz, rbgb, sptp;
        MagicVec4<T,0,2,2,0> xzzx, rbbr, spps;
        MagicVec4<T,0,2,2,1> xzzy, rbbg, sppt;
        MagicVec4<T,0,2,2,2> xzzz, rbbb, sppp;
        MagicVec4<T,1,0,0,0> yxxx, grrr, tsss;
        MagicVec4<T,1,0,0,1> yxxy, grrg, tsst;
        MagicVec4<T,1,0,0,2> yxxz, grrb, tssp;
        MagicVec4<T,1,0,1,0> yxyx, grgr, tsts;
        MagicVec4<T,1,0,1,1> yxyy, grgg, tstt;
        MagicVec4<T,1,0,1,2> yxyz, grgb, tstp;
        MagicVec4<T,1,0,2,0> yxzx, grbr, tsps;
        MagicVec4<T,1,0,2,1> yxzy, grbg, tspt;
        MagicVec4<T,1,0,2,2> yxzz, grbb, tspp;
        MagicVec4<T,1,1,0,0> yyxx, ggrr, ttss;
        MagicVec4<T,1,1,0,1> yyxy, ggrg, ttst;
        MagicVec4<T,1,1,0,2> yyxz, ggrb, ttsp;
        MagicVec4<T,1,1,1,0> yyyx, gggr, ttts;
        MagicVec4<T,1,1,1,1> yyyy, gggg, tttt;
        MagicVec4<T,1,1,1,2> yyyz, gggb, tttp;
        MagicVec4<T,1,1,2,0> yyzx, ggbr, ttps;
        MagicVec4<T,1,1,2,1> yyzy, ggbg, ttpt;
        MagicVec4<T,1,1,2,2> yyzz, ggbb, ttpp;
        MagicVec4<T,1,2,0,0> yzxx, gbrr, tpss;
        MagicVec4<T,1,2,0,1> yzxy, gbrg, tpst;
        MagicVec4<T,1,2,0,2> yzxz, gbrb, tpsp;
        MagicVec4<T,1,2,1,0> yzyx, gbgr, tpts;
        MagicVec4<T,1,2,1,1> yzyy, gbgg, tptt;
        MagicVec4<T,1,2,1,2> yzyz, gbgb, tptp;
        MagicVec4<T,1,2,2,0> yzzx, gbbr, tpps;
        MagicVec4<T,1,2,2,1> yzzy, gbbg, tppt;
        MagicVec4<T,1,2,2,2> yzzz, gbbb, tppp;
        MagicVec4<T,2,0,0,0> zxxx, brrr, psss;
        MagicVec4<T,2,0,0,1> zxxy, brrg, psst;
        MagicVec4<T,2,0,0,2> zxxz, brrb, pssp;
        MagicVec4<T,2,0,1,0> zxyx, brgr, psts;
        MagicVec4<T,2,0,1,1> zxyy, brgg, pstt;
        MagicVec4<T,2,0,1,2> zxyz, brgb, pstp;
        MagicVec4<T,2,0,2,0> zxzx, brbr, psps;
        MagicVec4<T,2,0,2,1> zxzy, brbg, pspt;
        MagicVec4<T,2,0,2,2> zxzz, brbb, pspp;
        MagicVec4<T,2,1,0,0> zyxx, bgrr, ptss;
        MagicVec4<T,2,1,0,1> zyxy, bgrg, ptst;
        MagicVec4<T,2,1,0,2> zyxz, bgrb, ptsp;
        MagicVec4<T,2,1,1,0> zyyx, bggr, ptts;
        MagicVec4<T,2,1,1,1> zyyy, bggg, pttt;
        MagicVec4<T,2,1,1,2> zyyz, bggb, pttp;
        MagicVec4<T,2,1,2,0> zyzx, bgbr, ptps;
        MagicVec4<T,2,1,2,1> zyzy, bgbg, ptpt;
        MagicVec4<T,2,1,2,2> zyzz, bgbb, ptpp;
        MagicVec4<T,2,2,0,0> zzxx, bbrr, ppss;
        MagicVec4<T,2,2,0,1> zzxy, bbrg, ppst;
        MagicVec4<T,2,2,0,2> zzxz, bbrb, ppsp;
        MagicVec4<T,2,2,1,0> zzyx, bbgr, ppts;
        MagicVec4<T,2,2,1,1> zzyy, bbgg, pptt;
        MagicVec4<T,2,2,1,2> zzyz, bbgb, pptp;
        MagicVec4<T,2,2,2,0> zzzx, bbbr, ppps;
        MagicVec4<T,2,2,2,1> zzzy, bbbg, pppt;
        MagicVec4<T,2,2,2,2> zzzz, bbbb, pppp;
    };
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

    template<int I, int J, int K, int L>
    inline Vec4(MagicVec4<T, I, J, K, L> const &v)
      : x(v.ptr[I]), y(v.ptr[J]), z(v.ptr[K]), w(v.ptr[L]) {}

    template<typename U, int I, int J, int K, int L>
    explicit inline Vec4(MagicVec4<U, I, J, K, L> const &v)
      : x(v.ptr[I]), y(v.ptr[J]), z(v.ptr[K]), w(v.ptr[L]) {}

    MEMBER_OPS()
    OTHER_MEMBER_OPS(Vec4)

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec4<U> const &v);
#endif

    union
    {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T s, t, p, q; };

        MagicVec2<T,0,0> xx, rr, ss;
        MagicVec2<T,0,1> xy, rg, st;
        MagicVec2<T,0,2> xz, rb, sp;
        MagicVec2<T,0,3> xw, ra, sq;
        MagicVec2<T,1,0> yx, gr, ts;
        MagicVec2<T,1,1> yy, gg, tt;
        MagicVec2<T,1,2> yz, gb, tp;
        MagicVec2<T,1,3> yw, ga, tq;
        MagicVec2<T,2,0> zx, br, ps;
        MagicVec2<T,2,1> zy, bg, pt;
        MagicVec2<T,2,2> zz, bb, pp;
        MagicVec2<T,2,3> zw, ba, pq;
        MagicVec2<T,3,0> wx, ar, qs;
        MagicVec2<T,3,1> wy, ag, qt;
        MagicVec2<T,3,2> wz, ab, qp;
        MagicVec2<T,3,3> ww, aa, qq;
        MagicVec3<T,0,0,0> xxx, rrr, sss;
        MagicVec3<T,0,0,1> xxy, rrg, sst;
        MagicVec3<T,0,0,2> xxz, rrb, ssp;
        MagicVec3<T,0,0,3> xxw, rra, ssq;
        MagicVec3<T,0,1,0> xyx, rgr, sts;
        MagicVec3<T,0,1,1> xyy, rgg, stt;
        MagicVec3<T,0,1,2> xyz, rgb, stp;
        MagicVec3<T,0,1,3> xyw, rga, stq;
        MagicVec3<T,0,2,0> xzx, rbr, sps;
        MagicVec3<T,0,2,1> xzy, rbg, spt;
        MagicVec3<T,0,2,2> xzz, rbb, spp;
        MagicVec3<T,0,2,3> xzw, rba, spq;
        MagicVec3<T,0,3,0> xwx, rar, sqs;
        MagicVec3<T,0,3,1> xwy, rag, sqt;
        MagicVec3<T,0,3,2> xwz, rab, sqp;
        MagicVec3<T,0,3,3> xww, raa, sqq;
        MagicVec3<T,1,0,0> yxx, grr, tss;
        MagicVec3<T,1,0,1> yxy, grg, tst;
        MagicVec3<T,1,0,2> yxz, grb, tsp;
        MagicVec3<T,1,0,3> yxw, gra, tsq;
        MagicVec3<T,1,1,0> yyx, ggr, tts;
        MagicVec3<T,1,1,1> yyy, ggg, ttt;
        MagicVec3<T,1,1,2> yyz, ggb, ttp;
        MagicVec3<T,1,1,3> yyw, gga, ttq;
        MagicVec3<T,1,2,0> yzx, gbr, tps;
        MagicVec3<T,1,2,1> yzy, gbg, tpt;
        MagicVec3<T,1,2,2> yzz, gbb, tpp;
        MagicVec3<T,1,2,3> yzw, gba, tpq;
        MagicVec3<T,1,3,0> ywx, gar, tqs;
        MagicVec3<T,1,3,1> ywy, gag, tqt;
        MagicVec3<T,1,3,2> ywz, gab, tqp;
        MagicVec3<T,1,3,3> yww, gaa, tqq;
        MagicVec3<T,2,0,0> zxx, brr, pss;
        MagicVec3<T,2,0,1> zxy, brg, pst;
        MagicVec3<T,2,0,2> zxz, brb, psp;
        MagicVec3<T,2,0,3> zxw, bra, psq;
        MagicVec3<T,2,1,0> zyx, bgr, pts;
        MagicVec3<T,2,1,1> zyy, bgg, ptt;
        MagicVec3<T,2,1,2> zyz, bgb, ptp;
        MagicVec3<T,2,1,3> zyw, bga, ptq;
        MagicVec3<T,2,2,0> zzx, bbr, pps;
        MagicVec3<T,2,2,1> zzy, bbg, ppt;
        MagicVec3<T,2,2,2> zzz, bbb, ppp;
        MagicVec3<T,2,2,3> zzw, bba, ppq;
        MagicVec3<T,2,3,0> zwx, bar, pqs;
        MagicVec3<T,2,3,1> zwy, bag, pqt;
        MagicVec3<T,2,3,2> zwz, bab, pqp;
        MagicVec3<T,2,3,3> zww, baa, pqq;
        MagicVec3<T,3,0,0> wxx, arr, qss;
        MagicVec3<T,3,0,1> wxy, arg, qst;
        MagicVec3<T,3,0,2> wxz, arb, qsp;
        MagicVec3<T,3,0,3> wxw, ara, qsq;
        MagicVec3<T,3,1,0> wyx, agr, qts;
        MagicVec3<T,3,1,1> wyy, agg, qtt;
        MagicVec3<T,3,1,2> wyz, agb, qtp;
        MagicVec3<T,3,1,3> wyw, aga, qtq;
        MagicVec3<T,3,2,0> wzx, abr, qps;
        MagicVec3<T,3,2,1> wzy, abg, qpt;
        MagicVec3<T,3,2,2> wzz, abb, qpp;
        MagicVec3<T,3,2,3> wzw, aba, qpq;
        MagicVec3<T,3,3,0> wwx, aar, qqs;
        MagicVec3<T,3,3,1> wwy, aag, qqt;
        MagicVec3<T,3,3,2> wwz, aab, qqp;
        MagicVec3<T,3,3,3> www, aaa, qqq;
        MagicVec4<T,0,0,0,0> xxxx, rrrr, ssss;
        MagicVec4<T,0,0,0,1> xxxy, rrrg, ssst;
        MagicVec4<T,0,0,0,2> xxxz, rrrb, sssp;
        MagicVec4<T,0,0,0,3> xxxw, rrra, sssq;
        MagicVec4<T,0,0,1,0> xxyx, rrgr, ssts;
        MagicVec4<T,0,0,1,1> xxyy, rrgg, sstt;
        MagicVec4<T,0,0,1,2> xxyz, rrgb, sstp;
        MagicVec4<T,0,0,1,3> xxyw, rrga, sstq;
        MagicVec4<T,0,0,2,0> xxzx, rrbr, ssps;
        MagicVec4<T,0,0,2,1> xxzy, rrbg, sspt;
        MagicVec4<T,0,0,2,2> xxzz, rrbb, sspp;
        MagicVec4<T,0,0,2,3> xxzw, rrba, sspq;
        MagicVec4<T,0,0,3,0> xxwx, rrar, ssqs;
        MagicVec4<T,0,0,3,1> xxwy, rrag, ssqt;
        MagicVec4<T,0,0,3,2> xxwz, rrab, ssqp;
        MagicVec4<T,0,0,3,3> xxww, rraa, ssqq;
        MagicVec4<T,0,1,0,0> xyxx, rgrr, stss;
        MagicVec4<T,0,1,0,1> xyxy, rgrg, stst;
        MagicVec4<T,0,1,0,2> xyxz, rgrb, stsp;
        MagicVec4<T,0,1,0,3> xyxw, rgra, stsq;
        MagicVec4<T,0,1,1,0> xyyx, rggr, stts;
        MagicVec4<T,0,1,1,1> xyyy, rggg, sttt;
        MagicVec4<T,0,1,1,2> xyyz, rggb, sttp;
        MagicVec4<T,0,1,1,3> xyyw, rgga, sttq;
        MagicVec4<T,0,1,2,0> xyzx, rgbr, stps;
        MagicVec4<T,0,1,2,1> xyzy, rgbg, stpt;
        MagicVec4<T,0,1,2,2> xyzz, rgbb, stpp;
        MagicVec4<T,0,1,2,3> xyzw, rgba, stpq;
        MagicVec4<T,0,1,3,0> xywx, rgar, stqs;
        MagicVec4<T,0,1,3,1> xywy, rgag, stqt;
        MagicVec4<T,0,1,3,2> xywz, rgab, stqp;
        MagicVec4<T,0,1,3,3> xyww, rgaa, stqq;
        MagicVec4<T,0,2,0,0> xzxx, rbrr, spss;
        MagicVec4<T,0,2,0,1> xzxy, rbrg, spst;
        MagicVec4<T,0,2,0,2> xzxz, rbrb, spsp;
        MagicVec4<T,0,2,0,3> xzxw, rbra, spsq;
        MagicVec4<T,0,2,1,0> xzyx, rbgr, spts;
        MagicVec4<T,0,2,1,1> xzyy, rbgg, sptt;
        MagicVec4<T,0,2,1,2> xzyz, rbgb, sptp;
        MagicVec4<T,0,2,1,3> xzyw, rbga, sptq;
        MagicVec4<T,0,2,2,0> xzzx, rbbr, spps;
        MagicVec4<T,0,2,2,1> xzzy, rbbg, sppt;
        MagicVec4<T,0,2,2,2> xzzz, rbbb, sppp;
        MagicVec4<T,0,2,2,3> xzzw, rbba, sppq;
        MagicVec4<T,0,2,3,0> xzwx, rbar, spqs;
        MagicVec4<T,0,2,3,1> xzwy, rbag, spqt;
        MagicVec4<T,0,2,3,2> xzwz, rbab, spqp;
        MagicVec4<T,0,2,3,3> xzww, rbaa, spqq;
        MagicVec4<T,0,3,0,0> xwxx, rarr, sqss;
        MagicVec4<T,0,3,0,1> xwxy, rarg, sqst;
        MagicVec4<T,0,3,0,2> xwxz, rarb, sqsp;
        MagicVec4<T,0,3,0,3> xwxw, rara, sqsq;
        MagicVec4<T,0,3,1,0> xwyx, ragr, sqts;
        MagicVec4<T,0,3,1,1> xwyy, ragg, sqtt;
        MagicVec4<T,0,3,1,2> xwyz, ragb, sqtp;
        MagicVec4<T,0,3,1,3> xwyw, raga, sqtq;
        MagicVec4<T,0,3,2,0> xwzx, rabr, sqps;
        MagicVec4<T,0,3,2,1> xwzy, rabg, sqpt;
        MagicVec4<T,0,3,2,2> xwzz, rabb, sqpp;
        MagicVec4<T,0,3,2,3> xwzw, raba, sqpq;
        MagicVec4<T,0,3,3,0> xwwx, raar, sqqs;
        MagicVec4<T,0,3,3,1> xwwy, raag, sqqt;
        MagicVec4<T,0,3,3,2> xwwz, raab, sqqp;
        MagicVec4<T,0,3,3,3> xwww, raaa, sqqq;
        MagicVec4<T,1,0,0,0> yxxx, grrr, tsss;
        MagicVec4<T,1,0,0,1> yxxy, grrg, tsst;
        MagicVec4<T,1,0,0,2> yxxz, grrb, tssp;
        MagicVec4<T,1,0,0,3> yxxw, grra, tssq;
        MagicVec4<T,1,0,1,0> yxyx, grgr, tsts;
        MagicVec4<T,1,0,1,1> yxyy, grgg, tstt;
        MagicVec4<T,1,0,1,2> yxyz, grgb, tstp;
        MagicVec4<T,1,0,1,3> yxyw, grga, tstq;
        MagicVec4<T,1,0,2,0> yxzx, grbr, tsps;
        MagicVec4<T,1,0,2,1> yxzy, grbg, tspt;
        MagicVec4<T,1,0,2,2> yxzz, grbb, tspp;
        MagicVec4<T,1,0,2,3> yxzw, grba, tspq;
        MagicVec4<T,1,0,3,0> yxwx, grar, tsqs;
        MagicVec4<T,1,0,3,1> yxwy, grag, tsqt;
        MagicVec4<T,1,0,3,2> yxwz, grab, tsqp;
        MagicVec4<T,1,0,3,3> yxww, graa, tsqq;
        MagicVec4<T,1,1,0,0> yyxx, ggrr, ttss;
        MagicVec4<T,1,1,0,1> yyxy, ggrg, ttst;
        MagicVec4<T,1,1,0,2> yyxz, ggrb, ttsp;
        MagicVec4<T,1,1,0,3> yyxw, ggra, ttsq;
        MagicVec4<T,1,1,1,0> yyyx, gggr, ttts;
        MagicVec4<T,1,1,1,1> yyyy, gggg, tttt;
        MagicVec4<T,1,1,1,2> yyyz, gggb, tttp;
        MagicVec4<T,1,1,1,3> yyyw, ggga, tttq;
        MagicVec4<T,1,1,2,0> yyzx, ggbr, ttps;
        MagicVec4<T,1,1,2,1> yyzy, ggbg, ttpt;
        MagicVec4<T,1,1,2,2> yyzz, ggbb, ttpp;
        MagicVec4<T,1,1,2,3> yyzw, ggba, ttpq;
        MagicVec4<T,1,1,3,0> yywx, ggar, ttqs;
        MagicVec4<T,1,1,3,1> yywy, ggag, ttqt;
        MagicVec4<T,1,1,3,2> yywz, ggab, ttqp;
        MagicVec4<T,1,1,3,3> yyww, ggaa, ttqq;
        MagicVec4<T,1,2,0,0> yzxx, gbrr, tpss;
        MagicVec4<T,1,2,0,1> yzxy, gbrg, tpst;
        MagicVec4<T,1,2,0,2> yzxz, gbrb, tpsp;
        MagicVec4<T,1,2,0,3> yzxw, gbra, tpsq;
        MagicVec4<T,1,2,1,0> yzyx, gbgr, tpts;
        MagicVec4<T,1,2,1,1> yzyy, gbgg, tptt;
        MagicVec4<T,1,2,1,2> yzyz, gbgb, tptp;
        MagicVec4<T,1,2,1,3> yzyw, gbga, tptq;
        MagicVec4<T,1,2,2,0> yzzx, gbbr, tpps;
        MagicVec4<T,1,2,2,1> yzzy, gbbg, tppt;
        MagicVec4<T,1,2,2,2> yzzz, gbbb, tppp;
        MagicVec4<T,1,2,2,3> yzzw, gbba, tppq;
        MagicVec4<T,1,2,3,0> yzwx, gbar, tpqs;
        MagicVec4<T,1,2,3,1> yzwy, gbag, tpqt;
        MagicVec4<T,1,2,3,2> yzwz, gbab, tpqp;
        MagicVec4<T,1,2,3,3> yzww, gbaa, tpqq;
        MagicVec4<T,1,3,0,0> ywxx, garr, tqss;
        MagicVec4<T,1,3,0,1> ywxy, garg, tqst;
        MagicVec4<T,1,3,0,2> ywxz, garb, tqsp;
        MagicVec4<T,1,3,0,3> ywxw, gara, tqsq;
        MagicVec4<T,1,3,1,0> ywyx, gagr, tqts;
        MagicVec4<T,1,3,1,1> ywyy, gagg, tqtt;
        MagicVec4<T,1,3,1,2> ywyz, gagb, tqtp;
        MagicVec4<T,1,3,1,3> ywyw, gaga, tqtq;
        MagicVec4<T,1,3,2,0> ywzx, gabr, tqps;
        MagicVec4<T,1,3,2,1> ywzy, gabg, tqpt;
        MagicVec4<T,1,3,2,2> ywzz, gabb, tqpp;
        MagicVec4<T,1,3,2,3> ywzw, gaba, tqpq;
        MagicVec4<T,1,3,3,0> ywwx, gaar, tqqs;
        MagicVec4<T,1,3,3,1> ywwy, gaag, tqqt;
        MagicVec4<T,1,3,3,2> ywwz, gaab, tqqp;
        MagicVec4<T,1,3,3,3> ywww, gaaa, tqqq;
        MagicVec4<T,2,0,0,0> zxxx, brrr, psss;
        MagicVec4<T,2,0,0,1> zxxy, brrg, psst;
        MagicVec4<T,2,0,0,2> zxxz, brrb, pssp;
        MagicVec4<T,2,0,0,3> zxxw, brra, pssq;
        MagicVec4<T,2,0,1,0> zxyx, brgr, psts;
        MagicVec4<T,2,0,1,1> zxyy, brgg, pstt;
        MagicVec4<T,2,0,1,2> zxyz, brgb, pstp;
        MagicVec4<T,2,0,1,3> zxyw, brga, pstq;
        MagicVec4<T,2,0,2,0> zxzx, brbr, psps;
        MagicVec4<T,2,0,2,1> zxzy, brbg, pspt;
        MagicVec4<T,2,0,2,2> zxzz, brbb, pspp;
        MagicVec4<T,2,0,2,3> zxzw, brba, pspq;
        MagicVec4<T,2,0,3,0> zxwx, brar, psqs;
        MagicVec4<T,2,0,3,1> zxwy, brag, psqt;
        MagicVec4<T,2,0,3,2> zxwz, brab, psqp;
        MagicVec4<T,2,0,3,3> zxww, braa, psqq;
        MagicVec4<T,2,1,0,0> zyxx, bgrr, ptss;
        MagicVec4<T,2,1,0,1> zyxy, bgrg, ptst;
        MagicVec4<T,2,1,0,2> zyxz, bgrb, ptsp;
        MagicVec4<T,2,1,0,3> zyxw, bgra, ptsq;
        MagicVec4<T,2,1,1,0> zyyx, bggr, ptts;
        MagicVec4<T,2,1,1,1> zyyy, bggg, pttt;
        MagicVec4<T,2,1,1,2> zyyz, bggb, pttp;
        MagicVec4<T,2,1,1,3> zyyw, bgga, pttq;
        MagicVec4<T,2,1,2,0> zyzx, bgbr, ptps;
        MagicVec4<T,2,1,2,1> zyzy, bgbg, ptpt;
        MagicVec4<T,2,1,2,2> zyzz, bgbb, ptpp;
        MagicVec4<T,2,1,2,3> zyzw, bgba, ptpq;
        MagicVec4<T,2,1,3,0> zywx, bgar, ptqs;
        MagicVec4<T,2,1,3,1> zywy, bgag, ptqt;
        MagicVec4<T,2,1,3,2> zywz, bgab, ptqp;
        MagicVec4<T,2,1,3,3> zyww, bgaa, ptqq;
        MagicVec4<T,2,2,0,0> zzxx, bbrr, ppss;
        MagicVec4<T,2,2,0,1> zzxy, bbrg, ppst;
        MagicVec4<T,2,2,0,2> zzxz, bbrb, ppsp;
        MagicVec4<T,2,2,0,3> zzxw, bbra, ppsq;
        MagicVec4<T,2,2,1,0> zzyx, bbgr, ppts;
        MagicVec4<T,2,2,1,1> zzyy, bbgg, pptt;
        MagicVec4<T,2,2,1,2> zzyz, bbgb, pptp;
        MagicVec4<T,2,2,1,3> zzyw, bbga, pptq;
        MagicVec4<T,2,2,2,0> zzzx, bbbr, ppps;
        MagicVec4<T,2,2,2,1> zzzy, bbbg, pppt;
        MagicVec4<T,2,2,2,2> zzzz, bbbb, pppp;
        MagicVec4<T,2,2,2,3> zzzw, bbba, pppq;
        MagicVec4<T,2,2,3,0> zzwx, bbar, ppqs;
        MagicVec4<T,2,2,3,1> zzwy, bbag, ppqt;
        MagicVec4<T,2,2,3,2> zzwz, bbab, ppqp;
        MagicVec4<T,2,2,3,3> zzww, bbaa, ppqq;
        MagicVec4<T,2,3,0,0> zwxx, barr, pqss;
        MagicVec4<T,2,3,0,1> zwxy, barg, pqst;
        MagicVec4<T,2,3,0,2> zwxz, barb, pqsp;
        MagicVec4<T,2,3,0,3> zwxw, bara, pqsq;
        MagicVec4<T,2,3,1,0> zwyx, bagr, pqts;
        MagicVec4<T,2,3,1,1> zwyy, bagg, pqtt;
        MagicVec4<T,2,3,1,2> zwyz, bagb, pqtp;
        MagicVec4<T,2,3,1,3> zwyw, baga, pqtq;
        MagicVec4<T,2,3,2,0> zwzx, babr, pqps;
        MagicVec4<T,2,3,2,1> zwzy, babg, pqpt;
        MagicVec4<T,2,3,2,2> zwzz, babb, pqpp;
        MagicVec4<T,2,3,2,3> zwzw, baba, pqpq;
        MagicVec4<T,2,3,3,0> zwwx, baar, pqqs;
        MagicVec4<T,2,3,3,1> zwwy, baag, pqqt;
        MagicVec4<T,2,3,3,2> zwwz, baab, pqqp;
        MagicVec4<T,2,3,3,3> zwww, baaa, pqqq;
        MagicVec4<T,3,0,0,0> wxxx, arrr, qsss;
        MagicVec4<T,3,0,0,1> wxxy, arrg, qsst;
        MagicVec4<T,3,0,0,2> wxxz, arrb, qssp;
        MagicVec4<T,3,0,0,3> wxxw, arra, qssq;
        MagicVec4<T,3,0,1,0> wxyx, argr, qsts;
        MagicVec4<T,3,0,1,1> wxyy, argg, qstt;
        MagicVec4<T,3,0,1,2> wxyz, argb, qstp;
        MagicVec4<T,3,0,1,3> wxyw, arga, qstq;
        MagicVec4<T,3,0,2,0> wxzx, arbr, qsps;
        MagicVec4<T,3,0,2,1> wxzy, arbg, qspt;
        MagicVec4<T,3,0,2,2> wxzz, arbb, qspp;
        MagicVec4<T,3,0,2,3> wxzw, arba, qspq;
        MagicVec4<T,3,0,3,0> wxwx, arar, qsqs;
        MagicVec4<T,3,0,3,1> wxwy, arag, qsqt;
        MagicVec4<T,3,0,3,2> wxwz, arab, qsqp;
        MagicVec4<T,3,0,3,3> wxww, araa, qsqq;
        MagicVec4<T,3,1,0,0> wyxx, agrr, qtss;
        MagicVec4<T,3,1,0,1> wyxy, agrg, qtst;
        MagicVec4<T,3,1,0,2> wyxz, agrb, qtsp;
        MagicVec4<T,3,1,0,3> wyxw, agra, qtsq;
        MagicVec4<T,3,1,1,0> wyyx, aggr, qtts;
        MagicVec4<T,3,1,1,1> wyyy, aggg, qttt;
        MagicVec4<T,3,1,1,2> wyyz, aggb, qttp;
        MagicVec4<T,3,1,1,3> wyyw, agga, qttq;
        MagicVec4<T,3,1,2,0> wyzx, agbr, qtps;
        MagicVec4<T,3,1,2,1> wyzy, agbg, qtpt;
        MagicVec4<T,3,1,2,2> wyzz, agbb, qtpp;
        MagicVec4<T,3,1,2,3> wyzw, agba, qtpq;
        MagicVec4<T,3,1,3,0> wywx, agar, qtqs;
        MagicVec4<T,3,1,3,1> wywy, agag, qtqt;
        MagicVec4<T,3,1,3,2> wywz, agab, qtqp;
        MagicVec4<T,3,1,3,3> wyww, agaa, qtqq;
        MagicVec4<T,3,2,0,0> wzxx, abrr, qpss;
        MagicVec4<T,3,2,0,1> wzxy, abrg, qpst;
        MagicVec4<T,3,2,0,2> wzxz, abrb, qpsp;
        MagicVec4<T,3,2,0,3> wzxw, abra, qpsq;
        MagicVec4<T,3,2,1,0> wzyx, abgr, qpts;
        MagicVec4<T,3,2,1,1> wzyy, abgg, qptt;
        MagicVec4<T,3,2,1,2> wzyz, abgb, qptp;
        MagicVec4<T,3,2,1,3> wzyw, abga, qptq;
        MagicVec4<T,3,2,2,0> wzzx, abbr, qpps;
        MagicVec4<T,3,2,2,1> wzzy, abbg, qppt;
        MagicVec4<T,3,2,2,2> wzzz, abbb, qppp;
        MagicVec4<T,3,2,2,3> wzzw, abba, qppq;
        MagicVec4<T,3,2,3,0> wzwx, abar, qpqs;
        MagicVec4<T,3,2,3,1> wzwy, abag, qpqt;
        MagicVec4<T,3,2,3,2> wzwz, abab, qpqp;
        MagicVec4<T,3,2,3,3> wzww, abaa, qpqq;
        MagicVec4<T,3,3,0,0> wwxx, aarr, qqss;
        MagicVec4<T,3,3,0,1> wwxy, aarg, qqst;
        MagicVec4<T,3,3,0,2> wwxz, aarb, qqsp;
        MagicVec4<T,3,3,0,3> wwxw, aara, qqsq;
        MagicVec4<T,3,3,1,0> wwyx, aagr, qqts;
        MagicVec4<T,3,3,1,1> wwyy, aagg, qqtt;
        MagicVec4<T,3,3,1,2> wwyz, aagb, qqtp;
        MagicVec4<T,3,3,1,3> wwyw, aaga, qqtq;
        MagicVec4<T,3,3,2,0> wwzx, aabr, qqps;
        MagicVec4<T,3,3,2,1> wwzy, aabg, qqpt;
        MagicVec4<T,3,3,2,2> wwzz, aabb, qqpp;
        MagicVec4<T,3,3,2,3> wwzw, aaba, qqpq;
        MagicVec4<T,3,3,3,0> wwwx, aaar, qqqs;
        MagicVec4<T,3,3,3,1> wwwy, aaag, qqqt;
        MagicVec4<T,3,3,3,2> wwwz, aaab, qqqp;
        MagicVec4<T,3,3,3,3> wwww, aaaa, qqqq;
    };
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

    MEMBER_OPS()

    QUATERNION_OPS()

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Quat<U> const &v);
#endif

    T x, y, z, w;
};

template<typename T>
inline T norm(Quat<T> const &val)
{
    return sqlen(val);
}

template<typename T>
static inline Quat<T> re(Quat<T> const &val)
{
    return ~val / norm(val);
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

#define VECTOR_OP(tname, op, tprefix, T) \
    tprefix \
    static inline tname<T> operator op(tname<T> const &a, tname<T> const &b) \
    { \
        tname<T> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            ret[n] = a[n] op b[n]; \
        return ret; \
    } \
    \
    tprefix \
    static inline tname<T> operator op##=(tname<T> &a, tname<T> const &b) \
    { \
        return a = a op b; \
    }

#define BOOL_OP(tname, op, op2, ret, tprefix, T) \
    tprefix \
    static inline bool operator op(tname<T> const &a, tname<T> const &b) \
    { \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            if (!(a[n] op2 b[n])) \
                return !ret; \
        return ret; \
    }

#define SCALAR_OP(tname, op, tprefix, T) \
    tprefix \
    static inline tname<T> operator op(tname<T> const &a, T const &val) \
    { \
        tname<T> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            ret[n] = a[n] op val; \
        return ret; \
    } \
    \
    tprefix \
    static inline tname<T> operator op(T const &val, tname<T> const &a) \
    { \
        tname<T> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            ret[n] = a[n] op val; \
        return ret; \
    } \
    \
    tprefix \
    static inline tname<T> operator op##=(tname<T> &a, T const &val) \
    { \
        return a = a op val; \
    }

#define SCALAR_PROMOTE_OP(tname, op, U) \
    template<typename T> \
    static inline tname<U> operator op(U const &val, tname<T> const &a) \
    { \
        tname<U> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            ret[n] = val op a[n]; \
        return ret; \
    }

#define GLOBAL_OPS(tname, tprefix, T) \
    SCALAR_OP(tname, *, tprefix, T) \
    SCALAR_OP(tname, /, tprefix, T) \
    \
    VECTOR_OP(tname, -, tprefix, T) \
    VECTOR_OP(tname, +, tprefix, T) \
    \
    BOOL_OP(tname, ==, ==, true, tprefix, T) \
    BOOL_OP(tname, !=, ==, false, tprefix, T) \
    \
    tprefix \
    static inline tname<T> operator -(tname<T> const &a) \
    { \
        tname<T> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            ret[n] = -a[n]; \
        return ret; \
    } \
    \
    tprefix \
    static inline T sqlen(tname<T> const &a) \
    { \
        T acc = 0; \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            acc += a[n] * a[n]; \
        return acc; \
    } \
    \
    tprefix \
    static inline double len(tname<T> const &a) \
    { \
        using namespace std; \
        return sqrt((double)sqlen(a)); \
    } \
    \
    tprefix \
    static inline T dot(tname<T> const &a, tname<T> const &b) \
    { \
        T ret = 0; \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            ret += a[n] * b[n]; \
        return ret; \
    } \
    \
    tprefix \
    static inline tname<T> normalize(tname<T> const &val) \
    { \
        T norm = len(val); \
        return norm ? val / norm : val * (T)0; \
    }

#define GLOBAL_TEMPLATE_OPS(tname) \
    GLOBAL_OPS(tname, template<typename T>, T)

#define ALL_GLOBAL_OPS(tname, tprefix, T) \
    VECTOR_OP(tname, *, tprefix, T) \
    VECTOR_OP(tname, /, tprefix, T) \
    \
    GLOBAL_OPS(tname, tprefix, T) \
    \
    BOOL_OP(tname, <=, <=, true, tprefix, T) \
    BOOL_OP(tname, >=, >=, true, tprefix, T) \
    BOOL_OP(tname, <, <, true, tprefix, T) \
    BOOL_OP(tname, >, >, true, tprefix, T)

/* FIXME: a few problems need to be fixed before we can use "half" here. It
 * will probably never work until we switch to C++11 because it's not really
 * a POD class. */
#define GLOBAL_TYPED_OPS(tname) \
    ALL_GLOBAL_OPS(tname, , float) \
    ALL_GLOBAL_OPS(tname, , double) \
    ALL_GLOBAL_OPS(tname, , int8_t) \
    ALL_GLOBAL_OPS(tname, , uint8_t) \
    ALL_GLOBAL_OPS(tname, , int16_t) \
    ALL_GLOBAL_OPS(tname, , uint16_t) \
    ALL_GLOBAL_OPS(tname, , int32_t) \
    ALL_GLOBAL_OPS(tname, , uint32_t) \
    ALL_GLOBAL_OPS(tname, , int64_t) \
    ALL_GLOBAL_OPS(tname, , uint64_t)

GLOBAL_TEMPLATE_OPS(Cmplx)
GLOBAL_TEMPLATE_OPS(Quat)

GLOBAL_TYPED_OPS(Vec2)
GLOBAL_TYPED_OPS(Vec3)
GLOBAL_TYPED_OPS(Vec4)

/*
 * Magic swizzling (part 2/2)
 */

template<typename T, int I, int J>
inline Vec2<T> MagicVec2<T, I, J>::operator =(Vec2<T> that)
{
    ptr[I] = that.x; ptr[J] = that.y;
    return *this;
}

template<typename T, int I, int J, int K>
inline Vec3<T> MagicVec3<T, I, J, K>::operator =(Vec3<T> that)
{
    ptr[I] = that.x; ptr[J] = that.y; ptr[K] = that.z;
    return *this;
}

template<typename T, int I, int J, int K, int L>
inline Vec4<T> MagicVec4<T, I, J, K, L>::operator =(Vec4<T> that)
{
    ptr[I] = that.x; ptr[J] = that.y; ptr[K] = that.z; ptr[L] = that.w;
    return *this;
}

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

/*
 * Arbitrarily-sized square matrices; for now this only supports
 * naive inversion and is used for the Remez inversion method.
 */

template<int N, typename T> struct Mat
{
    inline Mat<N, T>() {}

    Mat(T x)
    {
        for (int j = 0; j < N; j++)
            for (int i = 0; i < N; i++)
                if (i == j)
                    m[i][j] = x;
                else
                    m[i][j] = 0;
    }

    /* Naive matrix inversion */
    Mat<N, T> inv() const
    {
        Mat a = *this, b((T)1);

        /* Inversion method: iterate through all columns and make sure
         * all the terms are 1 on the diagonal and 0 everywhere else */
        for (int i = 0; i < N; i++)
        {
            /* If the expected coefficient is zero, add one of
             * the other lines. The first we meet will do. */
            if (!a.m[i][i])
            {
                for (int j = i + 1; j < N; j++)
                {
                    if (!a.m[i][j])
                        continue;
                    /* Add row j to row i */
                    for (int n = 0; n < N; n++)
                    {
                        a.m[n][i] += a.m[n][j];
                        b.m[n][i] += b.m[n][j];
                    }
                    break;
                }
            }

            /* Now we know the diagonal term is non-zero. Get its inverse
             * and use that to nullify all other terms in the column */
            T x = (T)1 / a.m[i][i];
            for (int j = 0; j < N; j++)
            {
                if (j == i)
                    continue;
                T mul = x * a.m[i][j];
                for (int n = 0; n < N; n++)
                {
                    a.m[n][j] -= mul * a.m[n][i];
                    b.m[n][j] -= mul * b.m[n][i];
                }
            }

            /* Finally, ensure the diagonal term is 1 */
            for (int n = 0; n < N; n++)
            {
                a.m[n][i] *= x;
                b.m[n][i] *= x;
            }
        }

        return b;
    }

    T m[N][N];
};

} /* namespace lol */

#endif // __LOL_MATH_MATRIX_H__

