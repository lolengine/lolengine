//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The vector, complex, quaternion and matrix classes
// --------------------------------------------------
//

#if !defined __LOL_MATH_VECTOR_H__
#define __LOL_MATH_VECTOR_H__

#include <stdint.h>
#include <cmath>
#if !defined __ANDROID__
#   include <iostream>
#endif

namespace lol
{

class half;
class real;

#define DECLARE_VECTOR_TYPEDEFS(tname, suffix) \
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

DECLARE_VECTOR_TYPEDEFS(Vec2, vec2)
DECLARE_VECTOR_TYPEDEFS(Cmplx, cmplx)
DECLARE_VECTOR_TYPEDEFS(Vec3, vec3)
DECLARE_VECTOR_TYPEDEFS(Vec4, vec4)
DECLARE_VECTOR_TYPEDEFS(Quat, quat)
DECLARE_VECTOR_TYPEDEFS(Mat4, mat4)

/*
 * Magic vector swizzling (part 1/2)
 */

template<typename T, int N> struct XVec2
{
    inline Vec2<T> operator =(Vec2<T> that);

    static int const I = (N >> 4) & 3;
    static int const J = (N >> 0) & 3;
    T ptr[1 + (I > J ? I : J)];
};

template<typename T, int N> struct XVec3
{
    inline Vec3<T> operator =(Vec3<T> that);

    static int const I = (N >> 8) & 3;
    static int const J = (N >> 4) & 3;
    static int const K = (N >> 0) & 3;
    T ptr[1 + (I > J ? I > K ? I : K
                     : J > K ? J : K)];
};

template<typename T, int N> struct XVec4
{
    inline Vec4<T> operator =(Vec4<T> that);

    static int const I = (N >> 12) & 3;
    static int const J = (N >> 8) & 3;
    static int const K = (N >> 4) & 3;
    static int const L = (N >> 0) & 3;
    T ptr[1 + (I > J ? I > K ? I > L ? I : L : K > L ? K : L
                     : J > K ? J > L ? J : L : K > L ? K : L)];
};

/*
 * Helper macro for vector type member functions
 */

#define DECLARE_MEMBER_OPS(tname) \
    inline T& operator[](int n) { return *(&x + n); } \
    inline T const& operator[](int n) const { return *(&x + n); } \
    \
    void printf() const; \
    \
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
    inline Vec2() {}
    inline Vec2(T X, T Y) : x(X), y(Y) {}

    explicit inline Vec2(T X) : x(X), y(X) {}

    template<int N>
    inline Vec2(XVec2<T, N> const &v)
      : x(v.ptr[v.I]), y(v.ptr[v.J]) {}

    template<typename U, int N>
    explicit inline Vec2(XVec2<U, N> const &v)
      : x(v.ptr[v.I]), y(v.ptr[v.J]) {}

    DECLARE_MEMBER_OPS(Vec2)

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec2<U> const &v);
#endif

    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T s, t; };

        XVec2<T,0x00> xx, rr, ss;
        XVec2<T,0x01> xy, rg, st;
        XVec2<T,0x10> yx, gr, ts;
        XVec2<T,0x11> yy, gg, tt;

        XVec3<T,0x000> xxx, rrr, sss;
        XVec3<T,0x001> xxy, rrg, sst;
        XVec3<T,0x010> xyx, rgr, sts;
        XVec3<T,0x011> xyy, rgg, stt;
        XVec3<T,0x100> yxx, grr, tss;
        XVec3<T,0x101> yxy, grg, tst;
        XVec3<T,0x110> yyx, ggr, tts;
        XVec3<T,0x111> yyy, ggg, ttt;

        XVec4<T,0x0000> xxxx, rrrr, ssss;
        XVec4<T,0x0001> xxxy, rrrg, ssst;
        XVec4<T,0x0010> xxyx, rrgr, ssts;
        XVec4<T,0x0011> xxyy, rrgg, sstt;
        XVec4<T,0x0100> xyxx, rgrr, stss;
        XVec4<T,0x0101> xyxy, rgrg, stst;
        XVec4<T,0x0110> xyyx, rggr, stts;
        XVec4<T,0x0111> xyyy, rggg, sttt;
        XVec4<T,0x1000> yxxx, grrr, tsss;
        XVec4<T,0x1001> yxxy, grrg, tsst;
        XVec4<T,0x1010> yxyx, grgr, tsts;
        XVec4<T,0x1011> yxyy, grgg, tstt;
        XVec4<T,0x1100> yyxx, ggrr, ttss;
        XVec4<T,0x1101> yyxy, ggrg, ttst;
        XVec4<T,0x1110> yyyx, gggr, ttts;
        XVec4<T,0x1111> yyyy, gggg, tttt;
    };
};

/*
 * 2-element complexes
 */

template <typename T> struct Cmplx
{
    inline Cmplx() {}
    inline Cmplx(T X) : x(X), y(0) {}
    inline Cmplx(T X, T Y) : x(X), y(Y) {}

    DECLARE_MEMBER_OPS(Cmplx)

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

    inline T norm() const { return len(*this); }
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
    inline Vec3() {}
    inline Vec3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    inline Vec3(Vec2<T> XY, T Z) : x(XY.x), y(XY.y), z(Z) {}
    inline Vec3(T X, Vec2<T> YZ) : x(X), y(YZ.x), z(YZ.y) {}

    explicit inline Vec3(T X) : x(X), y(X), z(X) {}

    template<int N>
    inline Vec3(XVec3<T, N> const &v)
      : x(v.ptr[v.I]), y(v.ptr[v.J]), z(v.ptr[v.K]) {}

    template<typename U, int N>
    explicit inline Vec3(XVec3<U, N> const &v)
      : x(v.ptr[v.I]), y(v.ptr[v.J]), z(v.ptr[v.K]) {}

    DECLARE_MEMBER_OPS(Vec3)

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

        XVec2<T,0x00> xx, rr, ss;
        XVec2<T,0x01> xy, rg, st;
        XVec2<T,0x02> xz, rb, sp;
        XVec2<T,0x10> yx, gr, ts;
        XVec2<T,0x11> yy, gg, tt;
        XVec2<T,0x12> yz, gb, tp;
        XVec2<T,0x20> zx, br, ps;
        XVec2<T,0x21> zy, bg, pt;
        XVec2<T,0x22> zz, bb, pp;

        XVec3<T,0x000> xxx, rrr, sss;
        XVec3<T,0x001> xxy, rrg, sst;
        XVec3<T,0x002> xxz, rrb, ssp;
        XVec3<T,0x010> xyx, rgr, sts;
        XVec3<T,0x011> xyy, rgg, stt;
        XVec3<T,0x012> xyz, rgb, stp;
        XVec3<T,0x020> xzx, rbr, sps;
        XVec3<T,0x021> xzy, rbg, spt;
        XVec3<T,0x022> xzz, rbb, spp;
        XVec3<T,0x100> yxx, grr, tss;
        XVec3<T,0x101> yxy, grg, tst;
        XVec3<T,0x102> yxz, grb, tsp;
        XVec3<T,0x110> yyx, ggr, tts;
        XVec3<T,0x111> yyy, ggg, ttt;
        XVec3<T,0x112> yyz, ggb, ttp;
        XVec3<T,0x120> yzx, gbr, tps;
        XVec3<T,0x121> yzy, gbg, tpt;
        XVec3<T,0x122> yzz, gbb, tpp;
        XVec3<T,0x200> zxx, brr, pss;
        XVec3<T,0x201> zxy, brg, pst;
        XVec3<T,0x202> zxz, brb, psp;
        XVec3<T,0x210> zyx, bgr, pts;
        XVec3<T,0x211> zyy, bgg, ptt;
        XVec3<T,0x212> zyz, bgb, ptp;
        XVec3<T,0x220> zzx, bbr, pps;
        XVec3<T,0x221> zzy, bbg, ppt;
        XVec3<T,0x222> zzz, bbb, ppp;

        XVec4<T,0x0000> xxxx, rrrr, ssss;
        XVec4<T,0x0001> xxxy, rrrg, ssst;
        XVec4<T,0x0002> xxxz, rrrb, sssp;
        XVec4<T,0x0010> xxyx, rrgr, ssts;
        XVec4<T,0x0011> xxyy, rrgg, sstt;
        XVec4<T,0x0012> xxyz, rrgb, sstp;
        XVec4<T,0x0020> xxzx, rrbr, ssps;
        XVec4<T,0x0021> xxzy, rrbg, sspt;
        XVec4<T,0x0022> xxzz, rrbb, sspp;
        XVec4<T,0x0100> xyxx, rgrr, stss;
        XVec4<T,0x0101> xyxy, rgrg, stst;
        XVec4<T,0x0102> xyxz, rgrb, stsp;
        XVec4<T,0x0110> xyyx, rggr, stts;
        XVec4<T,0x0111> xyyy, rggg, sttt;
        XVec4<T,0x0112> xyyz, rggb, sttp;
        XVec4<T,0x0120> xyzx, rgbr, stps;
        XVec4<T,0x0121> xyzy, rgbg, stpt;
        XVec4<T,0x0122> xyzz, rgbb, stpp;
        XVec4<T,0x0200> xzxx, rbrr, spss;
        XVec4<T,0x0201> xzxy, rbrg, spst;
        XVec4<T,0x0202> xzxz, rbrb, spsp;
        XVec4<T,0x0210> xzyx, rbgr, spts;
        XVec4<T,0x0211> xzyy, rbgg, sptt;
        XVec4<T,0x0212> xzyz, rbgb, sptp;
        XVec4<T,0x0220> xzzx, rbbr, spps;
        XVec4<T,0x0221> xzzy, rbbg, sppt;
        XVec4<T,0x0222> xzzz, rbbb, sppp;
        XVec4<T,0x1000> yxxx, grrr, tsss;
        XVec4<T,0x1001> yxxy, grrg, tsst;
        XVec4<T,0x1002> yxxz, grrb, tssp;
        XVec4<T,0x1010> yxyx, grgr, tsts;
        XVec4<T,0x1011> yxyy, grgg, tstt;
        XVec4<T,0x1012> yxyz, grgb, tstp;
        XVec4<T,0x1020> yxzx, grbr, tsps;
        XVec4<T,0x1021> yxzy, grbg, tspt;
        XVec4<T,0x1022> yxzz, grbb, tspp;
        XVec4<T,0x1100> yyxx, ggrr, ttss;
        XVec4<T,0x1101> yyxy, ggrg, ttst;
        XVec4<T,0x1102> yyxz, ggrb, ttsp;
        XVec4<T,0x1110> yyyx, gggr, ttts;
        XVec4<T,0x1111> yyyy, gggg, tttt;
        XVec4<T,0x1112> yyyz, gggb, tttp;
        XVec4<T,0x1120> yyzx, ggbr, ttps;
        XVec4<T,0x1121> yyzy, ggbg, ttpt;
        XVec4<T,0x1122> yyzz, ggbb, ttpp;
        XVec4<T,0x1200> yzxx, gbrr, tpss;
        XVec4<T,0x1201> yzxy, gbrg, tpst;
        XVec4<T,0x1202> yzxz, gbrb, tpsp;
        XVec4<T,0x1210> yzyx, gbgr, tpts;
        XVec4<T,0x1211> yzyy, gbgg, tptt;
        XVec4<T,0x1212> yzyz, gbgb, tptp;
        XVec4<T,0x1220> yzzx, gbbr, tpps;
        XVec4<T,0x1221> yzzy, gbbg, tppt;
        XVec4<T,0x1222> yzzz, gbbb, tppp;
        XVec4<T,0x2000> zxxx, brrr, psss;
        XVec4<T,0x2001> zxxy, brrg, psst;
        XVec4<T,0x2002> zxxz, brrb, pssp;
        XVec4<T,0x2010> zxyx, brgr, psts;
        XVec4<T,0x2011> zxyy, brgg, pstt;
        XVec4<T,0x2012> zxyz, brgb, pstp;
        XVec4<T,0x2020> zxzx, brbr, psps;
        XVec4<T,0x2021> zxzy, brbg, pspt;
        XVec4<T,0x2022> zxzz, brbb, pspp;
        XVec4<T,0x2100> zyxx, bgrr, ptss;
        XVec4<T,0x2101> zyxy, bgrg, ptst;
        XVec4<T,0x2102> zyxz, bgrb, ptsp;
        XVec4<T,0x2110> zyyx, bggr, ptts;
        XVec4<T,0x2111> zyyy, bggg, pttt;
        XVec4<T,0x2112> zyyz, bggb, pttp;
        XVec4<T,0x2120> zyzx, bgbr, ptps;
        XVec4<T,0x2121> zyzy, bgbg, ptpt;
        XVec4<T,0x2122> zyzz, bgbb, ptpp;
        XVec4<T,0x2200> zzxx, bbrr, ppss;
        XVec4<T,0x2201> zzxy, bbrg, ppst;
        XVec4<T,0x2202> zzxz, bbrb, ppsp;
        XVec4<T,0x2210> zzyx, bbgr, ppts;
        XVec4<T,0x2211> zzyy, bbgg, pptt;
        XVec4<T,0x2212> zzyz, bbgb, pptp;
        XVec4<T,0x2220> zzzx, bbbr, ppps;
        XVec4<T,0x2221> zzzy, bbbg, pppt;
        XVec4<T,0x2222> zzzz, bbbb, pppp;
    };
};

/*
 * 4-element vectors
 */

template <typename T> struct Vec4
{
    inline Vec4() {}
    inline Vec4(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
    inline Vec4(Vec2<T> XY, T Z, T W) : x(XY.x), y(XY.y), z(Z), w(W) {}
    inline Vec4(T X, Vec2<T> YZ, T W) : x(X), y(YZ.x), z(YZ.y), w(W) {}
    inline Vec4(T X, T Y, Vec2<T> ZW) : x(X), y(Y), z(ZW.x), w(ZW.y) {}
    inline Vec4(Vec2<T> XY, Vec2<T> ZW) : x(XY.x), y(XY.y), z(ZW.x), w(ZW.y) {}
    inline Vec4(Vec3<T> XYZ, T W) : x(XYZ.x), y(XYZ.y), z(XYZ.z), w(W) {}
    inline Vec4(T X, Vec3<T> YZW) : x(X), y(YZW.x), z(YZW.y), w(YZW.z) {}

    explicit inline Vec4(T X) : x(X), y(X), z(X), w(X) {}

    template<int N>
    inline Vec4(XVec4<T, N> const &v)
      : x(v.ptr[v.I]), y(v.ptr[v.J]), z(v.ptr[v.K]), w(v.ptr[v.L]) {}

    template<typename U, int N>
    explicit inline Vec4(XVec4<U, N> const &v)
      : x(v.ptr[v.I]), y(v.ptr[v.J]), z(v.ptr[v.K]), w(v.ptr[v.L]) {}

    DECLARE_MEMBER_OPS(Vec4)

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec4<U> const &v);
#endif

    union
    {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T s, t, p, q; };

        XVec2<T,0x00> xx, rr, ss;
        XVec2<T,0x01> xy, rg, st;
        XVec2<T,0x02> xz, rb, sp;
        XVec2<T,0x03> xw, ra, sq;
        XVec2<T,0x10> yx, gr, ts;
        XVec2<T,0x11> yy, gg, tt;
        XVec2<T,0x12> yz, gb, tp;
        XVec2<T,0x13> yw, ga, tq;
        XVec2<T,0x20> zx, br, ps;
        XVec2<T,0x21> zy, bg, pt;
        XVec2<T,0x22> zz, bb, pp;
        XVec2<T,0x23> zw, ba, pq;
        XVec2<T,0x30> wx, ar, qs;
        XVec2<T,0x31> wy, ag, qt;
        XVec2<T,0x32> wz, ab, qp;
        XVec2<T,0x33> ww, aa, qq;

        XVec3<T,0x000> xxx, rrr, sss;
        XVec3<T,0x001> xxy, rrg, sst;
        XVec3<T,0x002> xxz, rrb, ssp;
        XVec3<T,0x003> xxw, rra, ssq;
        XVec3<T,0x010> xyx, rgr, sts;
        XVec3<T,0x011> xyy, rgg, stt;
        XVec3<T,0x012> xyz, rgb, stp;
        XVec3<T,0x013> xyw, rga, stq;
        XVec3<T,0x020> xzx, rbr, sps;
        XVec3<T,0x021> xzy, rbg, spt;
        XVec3<T,0x022> xzz, rbb, spp;
        XVec3<T,0x023> xzw, rba, spq;
        XVec3<T,0x030> xwx, rar, sqs;
        XVec3<T,0x031> xwy, rag, sqt;
        XVec3<T,0x032> xwz, rab, sqp;
        XVec3<T,0x033> xww, raa, sqq;
        XVec3<T,0x100> yxx, grr, tss;
        XVec3<T,0x101> yxy, grg, tst;
        XVec3<T,0x102> yxz, grb, tsp;
        XVec3<T,0x103> yxw, gra, tsq;
        XVec3<T,0x110> yyx, ggr, tts;
        XVec3<T,0x111> yyy, ggg, ttt;
        XVec3<T,0x112> yyz, ggb, ttp;
        XVec3<T,0x113> yyw, gga, ttq;
        XVec3<T,0x120> yzx, gbr, tps;
        XVec3<T,0x121> yzy, gbg, tpt;
        XVec3<T,0x122> yzz, gbb, tpp;
        XVec3<T,0x123> yzw, gba, tpq;
        XVec3<T,0x130> ywx, gar, tqs;
        XVec3<T,0x131> ywy, gag, tqt;
        XVec3<T,0x132> ywz, gab, tqp;
        XVec3<T,0x133> yww, gaa, tqq;
        XVec3<T,0x200> zxx, brr, pss;
        XVec3<T,0x201> zxy, brg, pst;
        XVec3<T,0x202> zxz, brb, psp;
        XVec3<T,0x203> zxw, bra, psq;
        XVec3<T,0x210> zyx, bgr, pts;
        XVec3<T,0x211> zyy, bgg, ptt;
        XVec3<T,0x212> zyz, bgb, ptp;
        XVec3<T,0x213> zyw, bga, ptq;
        XVec3<T,0x220> zzx, bbr, pps;
        XVec3<T,0x221> zzy, bbg, ppt;
        XVec3<T,0x222> zzz, bbb, ppp;
        XVec3<T,0x223> zzw, bba, ppq;
        XVec3<T,0x230> zwx, bar, pqs;
        XVec3<T,0x231> zwy, bag, pqt;
        XVec3<T,0x232> zwz, bab, pqp;
        XVec3<T,0x233> zww, baa, pqq;
        XVec3<T,0x300> wxx, arr, qss;
        XVec3<T,0x301> wxy, arg, qst;
        XVec3<T,0x302> wxz, arb, qsp;
        XVec3<T,0x303> wxw, ara, qsq;
        XVec3<T,0x310> wyx, agr, qts;
        XVec3<T,0x311> wyy, agg, qtt;
        XVec3<T,0x312> wyz, agb, qtp;
        XVec3<T,0x313> wyw, aga, qtq;
        XVec3<T,0x320> wzx, abr, qps;
        XVec3<T,0x321> wzy, abg, qpt;
        XVec3<T,0x322> wzz, abb, qpp;
        XVec3<T,0x323> wzw, aba, qpq;
        XVec3<T,0x330> wwx, aar, qqs;
        XVec3<T,0x331> wwy, aag, qqt;
        XVec3<T,0x332> wwz, aab, qqp;
        XVec3<T,0x333> www, aaa, qqq;

        XVec4<T,0x0000> xxxx, rrrr, ssss;
        XVec4<T,0x0001> xxxy, rrrg, ssst;
        XVec4<T,0x0002> xxxz, rrrb, sssp;
        XVec4<T,0x0003> xxxw, rrra, sssq;
        XVec4<T,0x0010> xxyx, rrgr, ssts;
        XVec4<T,0x0011> xxyy, rrgg, sstt;
        XVec4<T,0x0012> xxyz, rrgb, sstp;
        XVec4<T,0x0013> xxyw, rrga, sstq;
        XVec4<T,0x0020> xxzx, rrbr, ssps;
        XVec4<T,0x0021> xxzy, rrbg, sspt;
        XVec4<T,0x0022> xxzz, rrbb, sspp;
        XVec4<T,0x0023> xxzw, rrba, sspq;
        XVec4<T,0x0030> xxwx, rrar, ssqs;
        XVec4<T,0x0031> xxwy, rrag, ssqt;
        XVec4<T,0x0032> xxwz, rrab, ssqp;
        XVec4<T,0x0033> xxww, rraa, ssqq;
        XVec4<T,0x0100> xyxx, rgrr, stss;
        XVec4<T,0x0101> xyxy, rgrg, stst;
        XVec4<T,0x0102> xyxz, rgrb, stsp;
        XVec4<T,0x0103> xyxw, rgra, stsq;
        XVec4<T,0x0110> xyyx, rggr, stts;
        XVec4<T,0x0111> xyyy, rggg, sttt;
        XVec4<T,0x0112> xyyz, rggb, sttp;
        XVec4<T,0x0113> xyyw, rgga, sttq;
        XVec4<T,0x0120> xyzx, rgbr, stps;
        XVec4<T,0x0121> xyzy, rgbg, stpt;
        XVec4<T,0x0122> xyzz, rgbb, stpp;
        XVec4<T,0x0123> xyzw, rgba, stpq;
        XVec4<T,0x0130> xywx, rgar, stqs;
        XVec4<T,0x0131> xywy, rgag, stqt;
        XVec4<T,0x0132> xywz, rgab, stqp;
        XVec4<T,0x0133> xyww, rgaa, stqq;
        XVec4<T,0x0200> xzxx, rbrr, spss;
        XVec4<T,0x0201> xzxy, rbrg, spst;
        XVec4<T,0x0202> xzxz, rbrb, spsp;
        XVec4<T,0x0203> xzxw, rbra, spsq;
        XVec4<T,0x0210> xzyx, rbgr, spts;
        XVec4<T,0x0211> xzyy, rbgg, sptt;
        XVec4<T,0x0212> xzyz, rbgb, sptp;
        XVec4<T,0x0213> xzyw, rbga, sptq;
        XVec4<T,0x0220> xzzx, rbbr, spps;
        XVec4<T,0x0221> xzzy, rbbg, sppt;
        XVec4<T,0x0222> xzzz, rbbb, sppp;
        XVec4<T,0x0223> xzzw, rbba, sppq;
        XVec4<T,0x0230> xzwx, rbar, spqs;
        XVec4<T,0x0231> xzwy, rbag, spqt;
        XVec4<T,0x0232> xzwz, rbab, spqp;
        XVec4<T,0x0233> xzww, rbaa, spqq;
        XVec4<T,0x0300> xwxx, rarr, sqss;
        XVec4<T,0x0301> xwxy, rarg, sqst;
        XVec4<T,0x0302> xwxz, rarb, sqsp;
        XVec4<T,0x0303> xwxw, rara, sqsq;
        XVec4<T,0x0310> xwyx, ragr, sqts;
        XVec4<T,0x0311> xwyy, ragg, sqtt;
        XVec4<T,0x0312> xwyz, ragb, sqtp;
        XVec4<T,0x0313> xwyw, raga, sqtq;
        XVec4<T,0x0320> xwzx, rabr, sqps;
        XVec4<T,0x0321> xwzy, rabg, sqpt;
        XVec4<T,0x0322> xwzz, rabb, sqpp;
        XVec4<T,0x0323> xwzw, raba, sqpq;
        XVec4<T,0x0330> xwwx, raar, sqqs;
        XVec4<T,0x0331> xwwy, raag, sqqt;
        XVec4<T,0x0332> xwwz, raab, sqqp;
        XVec4<T,0x0333> xwww, raaa, sqqq;
        XVec4<T,0x1000> yxxx, grrr, tsss;
        XVec4<T,0x1001> yxxy, grrg, tsst;
        XVec4<T,0x1002> yxxz, grrb, tssp;
        XVec4<T,0x1003> yxxw, grra, tssq;
        XVec4<T,0x1010> yxyx, grgr, tsts;
        XVec4<T,0x1011> yxyy, grgg, tstt;
        XVec4<T,0x1012> yxyz, grgb, tstp;
        XVec4<T,0x1013> yxyw, grga, tstq;
        XVec4<T,0x1020> yxzx, grbr, tsps;
        XVec4<T,0x1021> yxzy, grbg, tspt;
        XVec4<T,0x1022> yxzz, grbb, tspp;
        XVec4<T,0x1023> yxzw, grba, tspq;
        XVec4<T,0x1030> yxwx, grar, tsqs;
        XVec4<T,0x1031> yxwy, grag, tsqt;
        XVec4<T,0x1032> yxwz, grab, tsqp;
        XVec4<T,0x1033> yxww, graa, tsqq;
        XVec4<T,0x1100> yyxx, ggrr, ttss;
        XVec4<T,0x1101> yyxy, ggrg, ttst;
        XVec4<T,0x1102> yyxz, ggrb, ttsp;
        XVec4<T,0x1103> yyxw, ggra, ttsq;
        XVec4<T,0x1110> yyyx, gggr, ttts;
        XVec4<T,0x1111> yyyy, gggg, tttt;
        XVec4<T,0x1112> yyyz, gggb, tttp;
        XVec4<T,0x1113> yyyw, ggga, tttq;
        XVec4<T,0x1120> yyzx, ggbr, ttps;
        XVec4<T,0x1121> yyzy, ggbg, ttpt;
        XVec4<T,0x1122> yyzz, ggbb, ttpp;
        XVec4<T,0x1123> yyzw, ggba, ttpq;
        XVec4<T,0x1130> yywx, ggar, ttqs;
        XVec4<T,0x1131> yywy, ggag, ttqt;
        XVec4<T,0x1132> yywz, ggab, ttqp;
        XVec4<T,0x1133> yyww, ggaa, ttqq;
        XVec4<T,0x1200> yzxx, gbrr, tpss;
        XVec4<T,0x1201> yzxy, gbrg, tpst;
        XVec4<T,0x1202> yzxz, gbrb, tpsp;
        XVec4<T,0x1203> yzxw, gbra, tpsq;
        XVec4<T,0x1210> yzyx, gbgr, tpts;
        XVec4<T,0x1211> yzyy, gbgg, tptt;
        XVec4<T,0x1212> yzyz, gbgb, tptp;
        XVec4<T,0x1213> yzyw, gbga, tptq;
        XVec4<T,0x1220> yzzx, gbbr, tpps;
        XVec4<T,0x1221> yzzy, gbbg, tppt;
        XVec4<T,0x1222> yzzz, gbbb, tppp;
        XVec4<T,0x1223> yzzw, gbba, tppq;
        XVec4<T,0x1230> yzwx, gbar, tpqs;
        XVec4<T,0x1231> yzwy, gbag, tpqt;
        XVec4<T,0x1232> yzwz, gbab, tpqp;
        XVec4<T,0x1233> yzww, gbaa, tpqq;
        XVec4<T,0x1300> ywxx, garr, tqss;
        XVec4<T,0x1301> ywxy, garg, tqst;
        XVec4<T,0x1302> ywxz, garb, tqsp;
        XVec4<T,0x1303> ywxw, gara, tqsq;
        XVec4<T,0x1310> ywyx, gagr, tqts;
        XVec4<T,0x1311> ywyy, gagg, tqtt;
        XVec4<T,0x1312> ywyz, gagb, tqtp;
        XVec4<T,0x1313> ywyw, gaga, tqtq;
        XVec4<T,0x1320> ywzx, gabr, tqps;
        XVec4<T,0x1321> ywzy, gabg, tqpt;
        XVec4<T,0x1322> ywzz, gabb, tqpp;
        XVec4<T,0x1323> ywzw, gaba, tqpq;
        XVec4<T,0x1330> ywwx, gaar, tqqs;
        XVec4<T,0x1331> ywwy, gaag, tqqt;
        XVec4<T,0x1332> ywwz, gaab, tqqp;
        XVec4<T,0x1333> ywww, gaaa, tqqq;
        XVec4<T,0x2000> zxxx, brrr, psss;
        XVec4<T,0x2001> zxxy, brrg, psst;
        XVec4<T,0x2002> zxxz, brrb, pssp;
        XVec4<T,0x2003> zxxw, brra, pssq;
        XVec4<T,0x2010> zxyx, brgr, psts;
        XVec4<T,0x2011> zxyy, brgg, pstt;
        XVec4<T,0x2012> zxyz, brgb, pstp;
        XVec4<T,0x2013> zxyw, brga, pstq;
        XVec4<T,0x2020> zxzx, brbr, psps;
        XVec4<T,0x2021> zxzy, brbg, pspt;
        XVec4<T,0x2022> zxzz, brbb, pspp;
        XVec4<T,0x2023> zxzw, brba, pspq;
        XVec4<T,0x2030> zxwx, brar, psqs;
        XVec4<T,0x2031> zxwy, brag, psqt;
        XVec4<T,0x2032> zxwz, brab, psqp;
        XVec4<T,0x2033> zxww, braa, psqq;
        XVec4<T,0x2100> zyxx, bgrr, ptss;
        XVec4<T,0x2101> zyxy, bgrg, ptst;
        XVec4<T,0x2102> zyxz, bgrb, ptsp;
        XVec4<T,0x2103> zyxw, bgra, ptsq;
        XVec4<T,0x2110> zyyx, bggr, ptts;
        XVec4<T,0x2111> zyyy, bggg, pttt;
        XVec4<T,0x2112> zyyz, bggb, pttp;
        XVec4<T,0x2113> zyyw, bgga, pttq;
        XVec4<T,0x2120> zyzx, bgbr, ptps;
        XVec4<T,0x2121> zyzy, bgbg, ptpt;
        XVec4<T,0x2122> zyzz, bgbb, ptpp;
        XVec4<T,0x2123> zyzw, bgba, ptpq;
        XVec4<T,0x2130> zywx, bgar, ptqs;
        XVec4<T,0x2131> zywy, bgag, ptqt;
        XVec4<T,0x2132> zywz, bgab, ptqp;
        XVec4<T,0x2133> zyww, bgaa, ptqq;
        XVec4<T,0x2200> zzxx, bbrr, ppss;
        XVec4<T,0x2201> zzxy, bbrg, ppst;
        XVec4<T,0x2202> zzxz, bbrb, ppsp;
        XVec4<T,0x2203> zzxw, bbra, ppsq;
        XVec4<T,0x2210> zzyx, bbgr, ppts;
        XVec4<T,0x2211> zzyy, bbgg, pptt;
        XVec4<T,0x2212> zzyz, bbgb, pptp;
        XVec4<T,0x2213> zzyw, bbga, pptq;
        XVec4<T,0x2220> zzzx, bbbr, ppps;
        XVec4<T,0x2221> zzzy, bbbg, pppt;
        XVec4<T,0x2222> zzzz, bbbb, pppp;
        XVec4<T,0x2223> zzzw, bbba, pppq;
        XVec4<T,0x2230> zzwx, bbar, ppqs;
        XVec4<T,0x2231> zzwy, bbag, ppqt;
        XVec4<T,0x2232> zzwz, bbab, ppqp;
        XVec4<T,0x2233> zzww, bbaa, ppqq;
        XVec4<T,0x2300> zwxx, barr, pqss;
        XVec4<T,0x2301> zwxy, barg, pqst;
        XVec4<T,0x2302> zwxz, barb, pqsp;
        XVec4<T,0x2303> zwxw, bara, pqsq;
        XVec4<T,0x2310> zwyx, bagr, pqts;
        XVec4<T,0x2311> zwyy, bagg, pqtt;
        XVec4<T,0x2312> zwyz, bagb, pqtp;
        XVec4<T,0x2313> zwyw, baga, pqtq;
        XVec4<T,0x2320> zwzx, babr, pqps;
        XVec4<T,0x2321> zwzy, babg, pqpt;
        XVec4<T,0x2322> zwzz, babb, pqpp;
        XVec4<T,0x2323> zwzw, baba, pqpq;
        XVec4<T,0x2330> zwwx, baar, pqqs;
        XVec4<T,0x2331> zwwy, baag, pqqt;
        XVec4<T,0x2332> zwwz, baab, pqqp;
        XVec4<T,0x2333> zwww, baaa, pqqq;
        XVec4<T,0x3000> wxxx, arrr, qsss;
        XVec4<T,0x3001> wxxy, arrg, qsst;
        XVec4<T,0x3002> wxxz, arrb, qssp;
        XVec4<T,0x3003> wxxw, arra, qssq;
        XVec4<T,0x3010> wxyx, argr, qsts;
        XVec4<T,0x3011> wxyy, argg, qstt;
        XVec4<T,0x3012> wxyz, argb, qstp;
        XVec4<T,0x3013> wxyw, arga, qstq;
        XVec4<T,0x3020> wxzx, arbr, qsps;
        XVec4<T,0x3021> wxzy, arbg, qspt;
        XVec4<T,0x3022> wxzz, arbb, qspp;
        XVec4<T,0x3023> wxzw, arba, qspq;
        XVec4<T,0x3030> wxwx, arar, qsqs;
        XVec4<T,0x3031> wxwy, arag, qsqt;
        XVec4<T,0x3032> wxwz, arab, qsqp;
        XVec4<T,0x3033> wxww, araa, qsqq;
        XVec4<T,0x3100> wyxx, agrr, qtss;
        XVec4<T,0x3101> wyxy, agrg, qtst;
        XVec4<T,0x3102> wyxz, agrb, qtsp;
        XVec4<T,0x3103> wyxw, agra, qtsq;
        XVec4<T,0x3110> wyyx, aggr, qtts;
        XVec4<T,0x3111> wyyy, aggg, qttt;
        XVec4<T,0x3112> wyyz, aggb, qttp;
        XVec4<T,0x3113> wyyw, agga, qttq;
        XVec4<T,0x3120> wyzx, agbr, qtps;
        XVec4<T,0x3121> wyzy, agbg, qtpt;
        XVec4<T,0x3122> wyzz, agbb, qtpp;
        XVec4<T,0x3123> wyzw, agba, qtpq;
        XVec4<T,0x3130> wywx, agar, qtqs;
        XVec4<T,0x3131> wywy, agag, qtqt;
        XVec4<T,0x3132> wywz, agab, qtqp;
        XVec4<T,0x3133> wyww, agaa, qtqq;
        XVec4<T,0x3200> wzxx, abrr, qpss;
        XVec4<T,0x3201> wzxy, abrg, qpst;
        XVec4<T,0x3202> wzxz, abrb, qpsp;
        XVec4<T,0x3203> wzxw, abra, qpsq;
        XVec4<T,0x3210> wzyx, abgr, qpts;
        XVec4<T,0x3211> wzyy, abgg, qptt;
        XVec4<T,0x3212> wzyz, abgb, qptp;
        XVec4<T,0x3213> wzyw, abga, qptq;
        XVec4<T,0x3220> wzzx, abbr, qpps;
        XVec4<T,0x3221> wzzy, abbg, qppt;
        XVec4<T,0x3222> wzzz, abbb, qppp;
        XVec4<T,0x3223> wzzw, abba, qppq;
        XVec4<T,0x3230> wzwx, abar, qpqs;
        XVec4<T,0x3231> wzwy, abag, qpqt;
        XVec4<T,0x3232> wzwz, abab, qpqp;
        XVec4<T,0x3233> wzww, abaa, qpqq;
        XVec4<T,0x3300> wwxx, aarr, qqss;
        XVec4<T,0x3301> wwxy, aarg, qqst;
        XVec4<T,0x3302> wwxz, aarb, qqsp;
        XVec4<T,0x3303> wwxw, aara, qqsq;
        XVec4<T,0x3310> wwyx, aagr, qqts;
        XVec4<T,0x3311> wwyy, aagg, qqtt;
        XVec4<T,0x3312> wwyz, aagb, qqtp;
        XVec4<T,0x3313> wwyw, aaga, qqtq;
        XVec4<T,0x3320> wwzx, aabr, qqps;
        XVec4<T,0x3321> wwzy, aabg, qqpt;
        XVec4<T,0x3322> wwzz, aabb, qqpp;
        XVec4<T,0x3323> wwzw, aaba, qqpq;
        XVec4<T,0x3330> wwwx, aaar, qqqs;
        XVec4<T,0x3331> wwwy, aaag, qqqt;
        XVec4<T,0x3332> wwwz, aaab, qqqp;
        XVec4<T,0x3333> wwww, aaaa, qqqq;
    };
};

/*
 * 4-element quaternions
 */

template <typename T> struct Quat
{
    inline Quat() {}
    inline Quat(T X) : x(0), y(0), z(0), w(X) {}
    inline Quat(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

    Quat(Mat4<T> const &m);

    DECLARE_MEMBER_OPS(Quat)

    inline Quat<T> operator *(Quat<T> const &val) const
    {
        Quat<T> ret;
        Vec3<T> v1(x, y, z);
        Vec3<T> v2(val.x, val.y, val.z);
        Vec3<T> v3 = cross(v1, v2) + w * v2 + val.w * v1;
        return Quat<T>(v3.x, v3.y, v3.z, w * val.w - dot(v1, v2));
    }

    inline Quat<T> operator *=(Quat<T> const &val)
    {
        return *this = (*this) * val;
    }

    inline Quat<T> operator ~() const
    {
        return Quat<T>(-x, -y, -z, w);
    }

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

#define DECLARE_VECTOR_OP(tname, op, tprefix, T) \
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

#define DECLARE_BOOL_OP(tname, op, op2, ret, tprefix, T) \
    tprefix \
    static inline bool operator op(tname<T> const &a, tname<T> const &b) \
    { \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            if (!(a[n] op2 b[n])) \
                return !ret; \
        return ret; \
    }

#define DECLARE_SCALAR_OP(tname, op, tprefix, T) \
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

/* FIXME: this is not used yet */
#define SCALAR_PROMOTE_OP(tname, op, U) \
    template<typename T> \
    static inline tname<U> operator op(U const &val, tname<T> const &a) \
    { \
        tname<U> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(T); n++) \
            ret[n] = val op a[n]; \
        return ret; \
    }

#define DECLARE_ALL_GLOBAL_OPS(tname, tprefix, T) \
    DECLARE_SCALAR_OP(tname, *, tprefix, T) \
    DECLARE_SCALAR_OP(tname, /, tprefix, T) \
    \
    DECLARE_VECTOR_OP(tname, -, tprefix, T) \
    DECLARE_VECTOR_OP(tname, +, tprefix, T) \
    \
    DECLARE_BOOL_OP(tname, ==, ==, true, tprefix, T) \
    DECLARE_BOOL_OP(tname, !=, ==, false, tprefix, T) \
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

#define DECLARE_GLOBAL_TEMPLATE_OPS(tname) \
    DECLARE_ALL_GLOBAL_OPS(tname, template<typename T>, T)

#define DECLARE_ALL_GLOBAL_OPS(tname, tprefix, T) \
    DECLARE_VECTOR_OP(tname, *, tprefix, T) \
    DECLARE_VECTOR_OP(tname, /, tprefix, T) \
    \
    DECLARE_ALL_GLOBAL_OPS(tname, tprefix, T) \
    \
    DECLARE_BOOL_OP(tname, <=, <=, true, tprefix, T) \
    DECLARE_BOOL_OP(tname, >=, >=, true, tprefix, T) \
    DECLARE_BOOL_OP(tname, <, <, true, tprefix, T) \
    DECLARE_BOOL_OP(tname, >, >, true, tprefix, T)

/* FIXME: a few problems need to be fixed before we can use "half" here. It
 * will probably never work until we switch to C++11 because it's not really
 * a POD class. */
#define DECLARE_GLOBAL_TYPED_OPS(tname) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, float) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, double) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, int8_t) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, uint8_t) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, int16_t) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, uint16_t) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, int32_t) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, uint32_t) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, int64_t) \
    DECLARE_ALL_GLOBAL_OPS(tname, /* empty */, uint64_t)

DECLARE_GLOBAL_TEMPLATE_OPS(Cmplx)
DECLARE_GLOBAL_TEMPLATE_OPS(Quat)

DECLARE_GLOBAL_TYPED_OPS(Vec2)
DECLARE_GLOBAL_TYPED_OPS(Vec3)
DECLARE_GLOBAL_TYPED_OPS(Vec4)

#undef DECLARE_VECTOR_TYPEDEFS
#undef DECLARE_MEMBER_OPS
#undef DECLARE_VECTOR_OP
#undef DECLARE_BOOL_OP
#undef DECLARE_SCALAR_OP
#undef DECLARE_ALL_GLOBAL_OPS
#undef DECLARE_GLOBAL_TEMPLATE_OPS
#undef DECLARE_ALL_GLOBAL_OPS
#undef DECLARE_GLOBAL_TYPED_OPS

/*
 * Magic vector swizzling (part 2/2)
 */

template<typename T, int N>
inline Vec2<T> XVec2<T, N>::operator =(Vec2<T> that)
{
    ptr[I] = that[0]; ptr[J] = that[1];
    return *this;
}

template<typename T, int N>
inline Vec3<T> XVec3<T, N>::operator =(Vec3<T> that)
{
    ptr[I] = that[0]; ptr[J] = that[1]; ptr[K] = that[2];
    return *this;
}

template<typename T, int N>
inline Vec4<T> XVec4<T, N>::operator =(Vec4<T> that)
{
    ptr[I] = that[0]; ptr[J] = that[1]; ptr[K] = that[2]; ptr[L] = that[3];
    return *this;
}

/*
 * 4×4-element matrices
 */

template <typename T> struct Mat4
{
    inline Mat4() {}
    inline Mat4(Vec4<T> V0, Vec4<T> V1, Vec4<T> V2, Vec4<T> V3)
      : v0(V0), v1(V1), v2(V2), v3(V3) {}

    explicit inline Mat4(T val)
      : v0(val, (T)0, (T)0, (T)0),
        v1((T)0, val, (T)0, (T)0),
        v2((T)0, (T)0, val, (T)0),
        v3((T)0, (T)0, (T)0, val) {}

    inline Vec4<T>& operator[](int n) { return (&v0)[n]; }
    inline Vec4<T> const& operator[](int n) const { return (&v0)[n]; }

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

    inline Mat4<T> operator +(Mat4<T> const m) const
    {
        return Mat4<T>(v0 + m[0], v1 + m[1], v2 + m[2], v3 + m[3]);
    }

    inline Mat4<T> operator +=(Mat4<T> const m)
    {
        return *this = *this + m;
    }

    inline Mat4<T> operator -(Mat4<T> const m) const
    {
        return Mat4<T>(v0 - m[0], v1 - m[1], v2 - m[2], v3 - m[3]);
    }

    inline Mat4<T> operator -=(Mat4<T> const m)
    {
        return *this = *this - m;
    }

    inline Mat4<T> operator *(Mat4<T> const m) const
    {
        return Mat4<T>(*this * m[0], *this * m[1], *this * m[2], *this * m[3]);
    }

    inline Mat4<T> operator *=(Mat4<T> const m)
    {
        return *this = *this * m;
    }

    inline Vec4<T> operator *(Vec4<T> const m) const
    {
        Vec4<T> ret;
        for (int j = 0; j < 4; j++)
        {
            T tmp = 0;
            for (int k = 0; k < 4; k++)
                tmp += (*this)[k][j] * m[k];
            ret[j] = tmp;
        }
        return ret;
    }

    Vec4<T> v0, v1, v2, v3;
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

#endif // __LOL_MATH_VECTOR_H__

