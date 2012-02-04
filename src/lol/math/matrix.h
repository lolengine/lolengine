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
 * Magic vector swizzling (part 1/2)
 */

template<typename T, int N> struct XVec2
{
    inline Vec2<T> operator =(Vec2<T> that);

    static int const I = (N >> 2) & 3;
    static int const J = (N >> 0) & 3;
    T ptr[1 + (I > J ? I : J)];
};

template<typename T, int N> struct XVec3
{
    inline Vec3<T> operator =(Vec3<T> that);

    static int const I = (N >> 4) & 3;
    static int const J = (N >> 2) & 3;
    static int const K = (N >> 0) & 3;
    T ptr[1 + (I > J ? I > K ? I : K
                     : J > K ? J : K)];
};

template<typename T, int N> struct XVec4
{
    inline Vec4<T> operator =(Vec4<T> that);

    static int const I = (N >> 6) & 3;
    static int const J = (N >> 4) & 3;
    static int const K = (N >> 2) & 3;
    static int const L = (N >> 0) & 3;
    T ptr[1 + (I > J ? I > K ? I > L ? I : L : K > L ? K : L
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
    inline Vec2() {}
    inline Vec2(T X, T Y) : x(X), y(Y) {}

    explicit inline Vec2(T X) : x(X), y(X) {}

    template<int N>
    inline Vec2(XVec2<T, N> const &v)
      : x(v.ptr[v.I]), y(v.ptr[v.J]) {}

    template<typename U, int N>
    explicit inline Vec2(XVec2<U, N> const &v)
      : x(v.ptr[v.I]), y(v.ptr[v.J]) {}

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

        XVec2<T,0> xx, rr, ss;
        XVec2<T,1> xy, rg, st;
        XVec2<T,4> yx, gr, ts;
        XVec2<T,5> yy, gg, tt;
        XVec3<T,0> xxx, rrr, sss;
        XVec3<T,1> xxy, rrg, sst;
        XVec3<T,4> xyx, rgr, sts;
        XVec3<T,5> xyy, rgg, stt;
        XVec3<T,16> yxx, grr, tss;
        XVec3<T,17> yxy, grg, tst;
        XVec3<T,20> yyx, ggr, tts;
        XVec3<T,21> yyy, ggg, ttt;
        XVec4<T,0> xxxx, rrrr, ssss;
        XVec4<T,1> xxxy, rrrg, ssst;
        XVec4<T,4> xxyx, rrgr, ssts;
        XVec4<T,5> xxyy, rrgg, sstt;
        XVec4<T,16> xyxx, rgrr, stss;
        XVec4<T,17> xyxy, rgrg, stst;
        XVec4<T,20> xyyx, rggr, stts;
        XVec4<T,21> xyyy, rggg, sttt;
        XVec4<T,64> yxxx, grrr, tsss;
        XVec4<T,65> yxxy, grrg, tsst;
        XVec4<T,68> yxyx, grgr, tsts;
        XVec4<T,69> yxyy, grgg, tstt;
        XVec4<T,80> yyxx, ggrr, ttss;
        XVec4<T,81> yyxy, ggrg, ttst;
        XVec4<T,84> yyyx, gggr, ttts;
        XVec4<T,85> yyyy, gggg, tttt;
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

    MEMBER_OPS()

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

        XVec2<T,0> xx, rr, ss;
        XVec2<T,1> xy, rg, st;
        XVec2<T,2> xz, rb, sp;
        XVec2<T,4> yx, gr, ts;
        XVec2<T,5> yy, gg, tt;
        XVec2<T,6> yz, gb, tp;
        XVec2<T,8> zx, br, ps;
        XVec2<T,9> zy, bg, pt;
        XVec2<T,10> zz, bb, pp;
        XVec3<T,0> xxx, rrr, sss;
        XVec3<T,1> xxy, rrg, sst;
        XVec3<T,2> xxz, rrb, ssp;
        XVec3<T,4> xyx, rgr, sts;
        XVec3<T,5> xyy, rgg, stt;
        XVec3<T,6> xyz, rgb, stp;
        XVec3<T,8> xzx, rbr, sps;
        XVec3<T,9> xzy, rbg, spt;
        XVec3<T,10> xzz, rbb, spp;
        XVec3<T,16> yxx, grr, tss;
        XVec3<T,17> yxy, grg, tst;
        XVec3<T,18> yxz, grb, tsp;
        XVec3<T,20> yyx, ggr, tts;
        XVec3<T,21> yyy, ggg, ttt;
        XVec3<T,22> yyz, ggb, ttp;
        XVec3<T,24> yzx, gbr, tps;
        XVec3<T,25> yzy, gbg, tpt;
        XVec3<T,26> yzz, gbb, tpp;
        XVec3<T,32> zxx, brr, pss;
        XVec3<T,33> zxy, brg, pst;
        XVec3<T,34> zxz, brb, psp;
        XVec3<T,36> zyx, bgr, pts;
        XVec3<T,37> zyy, bgg, ptt;
        XVec3<T,38> zyz, bgb, ptp;
        XVec3<T,40> zzx, bbr, pps;
        XVec3<T,41> zzy, bbg, ppt;
        XVec3<T,42> zzz, bbb, ppp;
        XVec4<T,0> xxxx, rrrr, ssss;
        XVec4<T,1> xxxy, rrrg, ssst;
        XVec4<T,2> xxxz, rrrb, sssp;
        XVec4<T,4> xxyx, rrgr, ssts;
        XVec4<T,5> xxyy, rrgg, sstt;
        XVec4<T,6> xxyz, rrgb, sstp;
        XVec4<T,8> xxzx, rrbr, ssps;
        XVec4<T,9> xxzy, rrbg, sspt;
        XVec4<T,10> xxzz, rrbb, sspp;
        XVec4<T,16> xyxx, rgrr, stss;
        XVec4<T,17> xyxy, rgrg, stst;
        XVec4<T,18> xyxz, rgrb, stsp;
        XVec4<T,20> xyyx, rggr, stts;
        XVec4<T,21> xyyy, rggg, sttt;
        XVec4<T,22> xyyz, rggb, sttp;
        XVec4<T,24> xyzx, rgbr, stps;
        XVec4<T,25> xyzy, rgbg, stpt;
        XVec4<T,26> xyzz, rgbb, stpp;
        XVec4<T,32> xzxx, rbrr, spss;
        XVec4<T,33> xzxy, rbrg, spst;
        XVec4<T,34> xzxz, rbrb, spsp;
        XVec4<T,36> xzyx, rbgr, spts;
        XVec4<T,37> xzyy, rbgg, sptt;
        XVec4<T,38> xzyz, rbgb, sptp;
        XVec4<T,40> xzzx, rbbr, spps;
        XVec4<T,41> xzzy, rbbg, sppt;
        XVec4<T,42> xzzz, rbbb, sppp;
        XVec4<T,64> yxxx, grrr, tsss;
        XVec4<T,65> yxxy, grrg, tsst;
        XVec4<T,66> yxxz, grrb, tssp;
        XVec4<T,68> yxyx, grgr, tsts;
        XVec4<T,69> yxyy, grgg, tstt;
        XVec4<T,70> yxyz, grgb, tstp;
        XVec4<T,72> yxzx, grbr, tsps;
        XVec4<T,73> yxzy, grbg, tspt;
        XVec4<T,74> yxzz, grbb, tspp;
        XVec4<T,80> yyxx, ggrr, ttss;
        XVec4<T,81> yyxy, ggrg, ttst;
        XVec4<T,82> yyxz, ggrb, ttsp;
        XVec4<T,84> yyyx, gggr, ttts;
        XVec4<T,85> yyyy, gggg, tttt;
        XVec4<T,86> yyyz, gggb, tttp;
        XVec4<T,88> yyzx, ggbr, ttps;
        XVec4<T,89> yyzy, ggbg, ttpt;
        XVec4<T,90> yyzz, ggbb, ttpp;
        XVec4<T,96> yzxx, gbrr, tpss;
        XVec4<T,97> yzxy, gbrg, tpst;
        XVec4<T,98> yzxz, gbrb, tpsp;
        XVec4<T,100> yzyx, gbgr, tpts;
        XVec4<T,101> yzyy, gbgg, tptt;
        XVec4<T,102> yzyz, gbgb, tptp;
        XVec4<T,104> yzzx, gbbr, tpps;
        XVec4<T,105> yzzy, gbbg, tppt;
        XVec4<T,106> yzzz, gbbb, tppp;
        XVec4<T,128> zxxx, brrr, psss;
        XVec4<T,129> zxxy, brrg, psst;
        XVec4<T,130> zxxz, brrb, pssp;
        XVec4<T,132> zxyx, brgr, psts;
        XVec4<T,133> zxyy, brgg, pstt;
        XVec4<T,134> zxyz, brgb, pstp;
        XVec4<T,136> zxzx, brbr, psps;
        XVec4<T,137> zxzy, brbg, pspt;
        XVec4<T,138> zxzz, brbb, pspp;
        XVec4<T,144> zyxx, bgrr, ptss;
        XVec4<T,145> zyxy, bgrg, ptst;
        XVec4<T,146> zyxz, bgrb, ptsp;
        XVec4<T,148> zyyx, bggr, ptts;
        XVec4<T,149> zyyy, bggg, pttt;
        XVec4<T,150> zyyz, bggb, pttp;
        XVec4<T,152> zyzx, bgbr, ptps;
        XVec4<T,153> zyzy, bgbg, ptpt;
        XVec4<T,154> zyzz, bgbb, ptpp;
        XVec4<T,160> zzxx, bbrr, ppss;
        XVec4<T,161> zzxy, bbrg, ppst;
        XVec4<T,162> zzxz, bbrb, ppsp;
        XVec4<T,164> zzyx, bbgr, ppts;
        XVec4<T,165> zzyy, bbgg, pptt;
        XVec4<T,166> zzyz, bbgb, pptp;
        XVec4<T,168> zzzx, bbbr, ppps;
        XVec4<T,169> zzzy, bbbg, pppt;
        XVec4<T,170> zzzz, bbbb, pppp;
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

        XVec2<T,0> xx, rr, ss;
        XVec2<T,1> xy, rg, st;
        XVec2<T,2> xz, rb, sp;
        XVec2<T,3> xw, ra, sq;
        XVec2<T,4> yx, gr, ts;
        XVec2<T,5> yy, gg, tt;
        XVec2<T,6> yz, gb, tp;
        XVec2<T,7> yw, ga, tq;
        XVec2<T,8> zx, br, ps;
        XVec2<T,9> zy, bg, pt;
        XVec2<T,10> zz, bb, pp;
        XVec2<T,11> zw, ba, pq;
        XVec2<T,12> wx, ar, qs;
        XVec2<T,13> wy, ag, qt;
        XVec2<T,14> wz, ab, qp;
        XVec2<T,15> ww, aa, qq;
        XVec3<T,0> xxx, rrr, sss;
        XVec3<T,1> xxy, rrg, sst;
        XVec3<T,2> xxz, rrb, ssp;
        XVec3<T,3> xxw, rra, ssq;
        XVec3<T,4> xyx, rgr, sts;
        XVec3<T,5> xyy, rgg, stt;
        XVec3<T,6> xyz, rgb, stp;
        XVec3<T,7> xyw, rga, stq;
        XVec3<T,8> xzx, rbr, sps;
        XVec3<T,9> xzy, rbg, spt;
        XVec3<T,10> xzz, rbb, spp;
        XVec3<T,11> xzw, rba, spq;
        XVec3<T,12> xwx, rar, sqs;
        XVec3<T,13> xwy, rag, sqt;
        XVec3<T,14> xwz, rab, sqp;
        XVec3<T,15> xww, raa, sqq;
        XVec3<T,16> yxx, grr, tss;
        XVec3<T,17> yxy, grg, tst;
        XVec3<T,18> yxz, grb, tsp;
        XVec3<T,19> yxw, gra, tsq;
        XVec3<T,20> yyx, ggr, tts;
        XVec3<T,21> yyy, ggg, ttt;
        XVec3<T,22> yyz, ggb, ttp;
        XVec3<T,23> yyw, gga, ttq;
        XVec3<T,24> yzx, gbr, tps;
        XVec3<T,25> yzy, gbg, tpt;
        XVec3<T,26> yzz, gbb, tpp;
        XVec3<T,27> yzw, gba, tpq;
        XVec3<T,28> ywx, gar, tqs;
        XVec3<T,29> ywy, gag, tqt;
        XVec3<T,30> ywz, gab, tqp;
        XVec3<T,31> yww, gaa, tqq;
        XVec3<T,32> zxx, brr, pss;
        XVec3<T,33> zxy, brg, pst;
        XVec3<T,34> zxz, brb, psp;
        XVec3<T,35> zxw, bra, psq;
        XVec3<T,36> zyx, bgr, pts;
        XVec3<T,37> zyy, bgg, ptt;
        XVec3<T,38> zyz, bgb, ptp;
        XVec3<T,39> zyw, bga, ptq;
        XVec3<T,40> zzx, bbr, pps;
        XVec3<T,41> zzy, bbg, ppt;
        XVec3<T,42> zzz, bbb, ppp;
        XVec3<T,43> zzw, bba, ppq;
        XVec3<T,44> zwx, bar, pqs;
        XVec3<T,45> zwy, bag, pqt;
        XVec3<T,46> zwz, bab, pqp;
        XVec3<T,47> zww, baa, pqq;
        XVec3<T,48> wxx, arr, qss;
        XVec3<T,49> wxy, arg, qst;
        XVec3<T,50> wxz, arb, qsp;
        XVec3<T,51> wxw, ara, qsq;
        XVec3<T,52> wyx, agr, qts;
        XVec3<T,53> wyy, agg, qtt;
        XVec3<T,54> wyz, agb, qtp;
        XVec3<T,55> wyw, aga, qtq;
        XVec3<T,56> wzx, abr, qps;
        XVec3<T,57> wzy, abg, qpt;
        XVec3<T,58> wzz, abb, qpp;
        XVec3<T,59> wzw, aba, qpq;
        XVec3<T,60> wwx, aar, qqs;
        XVec3<T,61> wwy, aag, qqt;
        XVec3<T,62> wwz, aab, qqp;
        XVec3<T,63> www, aaa, qqq;
        XVec4<T,0> xxxx, rrrr, ssss;
        XVec4<T,1> xxxy, rrrg, ssst;
        XVec4<T,2> xxxz, rrrb, sssp;
        XVec4<T,3> xxxw, rrra, sssq;
        XVec4<T,4> xxyx, rrgr, ssts;
        XVec4<T,5> xxyy, rrgg, sstt;
        XVec4<T,6> xxyz, rrgb, sstp;
        XVec4<T,7> xxyw, rrga, sstq;
        XVec4<T,8> xxzx, rrbr, ssps;
        XVec4<T,9> xxzy, rrbg, sspt;
        XVec4<T,10> xxzz, rrbb, sspp;
        XVec4<T,11> xxzw, rrba, sspq;
        XVec4<T,12> xxwx, rrar, ssqs;
        XVec4<T,13> xxwy, rrag, ssqt;
        XVec4<T,14> xxwz, rrab, ssqp;
        XVec4<T,15> xxww, rraa, ssqq;
        XVec4<T,16> xyxx, rgrr, stss;
        XVec4<T,17> xyxy, rgrg, stst;
        XVec4<T,18> xyxz, rgrb, stsp;
        XVec4<T,19> xyxw, rgra, stsq;
        XVec4<T,20> xyyx, rggr, stts;
        XVec4<T,21> xyyy, rggg, sttt;
        XVec4<T,22> xyyz, rggb, sttp;
        XVec4<T,23> xyyw, rgga, sttq;
        XVec4<T,24> xyzx, rgbr, stps;
        XVec4<T,25> xyzy, rgbg, stpt;
        XVec4<T,26> xyzz, rgbb, stpp;
        XVec4<T,27> xyzw, rgba, stpq;
        XVec4<T,28> xywx, rgar, stqs;
        XVec4<T,29> xywy, rgag, stqt;
        XVec4<T,30> xywz, rgab, stqp;
        XVec4<T,31> xyww, rgaa, stqq;
        XVec4<T,32> xzxx, rbrr, spss;
        XVec4<T,33> xzxy, rbrg, spst;
        XVec4<T,34> xzxz, rbrb, spsp;
        XVec4<T,35> xzxw, rbra, spsq;
        XVec4<T,36> xzyx, rbgr, spts;
        XVec4<T,37> xzyy, rbgg, sptt;
        XVec4<T,38> xzyz, rbgb, sptp;
        XVec4<T,39> xzyw, rbga, sptq;
        XVec4<T,40> xzzx, rbbr, spps;
        XVec4<T,41> xzzy, rbbg, sppt;
        XVec4<T,42> xzzz, rbbb, sppp;
        XVec4<T,43> xzzw, rbba, sppq;
        XVec4<T,44> xzwx, rbar, spqs;
        XVec4<T,45> xzwy, rbag, spqt;
        XVec4<T,46> xzwz, rbab, spqp;
        XVec4<T,47> xzww, rbaa, spqq;
        XVec4<T,48> xwxx, rarr, sqss;
        XVec4<T,49> xwxy, rarg, sqst;
        XVec4<T,50> xwxz, rarb, sqsp;
        XVec4<T,51> xwxw, rara, sqsq;
        XVec4<T,52> xwyx, ragr, sqts;
        XVec4<T,53> xwyy, ragg, sqtt;
        XVec4<T,54> xwyz, ragb, sqtp;
        XVec4<T,55> xwyw, raga, sqtq;
        XVec4<T,56> xwzx, rabr, sqps;
        XVec4<T,57> xwzy, rabg, sqpt;
        XVec4<T,58> xwzz, rabb, sqpp;
        XVec4<T,59> xwzw, raba, sqpq;
        XVec4<T,60> xwwx, raar, sqqs;
        XVec4<T,61> xwwy, raag, sqqt;
        XVec4<T,62> xwwz, raab, sqqp;
        XVec4<T,63> xwww, raaa, sqqq;
        XVec4<T,64> yxxx, grrr, tsss;
        XVec4<T,65> yxxy, grrg, tsst;
        XVec4<T,66> yxxz, grrb, tssp;
        XVec4<T,67> yxxw, grra, tssq;
        XVec4<T,68> yxyx, grgr, tsts;
        XVec4<T,69> yxyy, grgg, tstt;
        XVec4<T,70> yxyz, grgb, tstp;
        XVec4<T,71> yxyw, grga, tstq;
        XVec4<T,72> yxzx, grbr, tsps;
        XVec4<T,73> yxzy, grbg, tspt;
        XVec4<T,74> yxzz, grbb, tspp;
        XVec4<T,75> yxzw, grba, tspq;
        XVec4<T,76> yxwx, grar, tsqs;
        XVec4<T,77> yxwy, grag, tsqt;
        XVec4<T,78> yxwz, grab, tsqp;
        XVec4<T,79> yxww, graa, tsqq;
        XVec4<T,80> yyxx, ggrr, ttss;
        XVec4<T,81> yyxy, ggrg, ttst;
        XVec4<T,82> yyxz, ggrb, ttsp;
        XVec4<T,83> yyxw, ggra, ttsq;
        XVec4<T,84> yyyx, gggr, ttts;
        XVec4<T,85> yyyy, gggg, tttt;
        XVec4<T,86> yyyz, gggb, tttp;
        XVec4<T,87> yyyw, ggga, tttq;
        XVec4<T,88> yyzx, ggbr, ttps;
        XVec4<T,89> yyzy, ggbg, ttpt;
        XVec4<T,90> yyzz, ggbb, ttpp;
        XVec4<T,91> yyzw, ggba, ttpq;
        XVec4<T,92> yywx, ggar, ttqs;
        XVec4<T,93> yywy, ggag, ttqt;
        XVec4<T,94> yywz, ggab, ttqp;
        XVec4<T,95> yyww, ggaa, ttqq;
        XVec4<T,96> yzxx, gbrr, tpss;
        XVec4<T,97> yzxy, gbrg, tpst;
        XVec4<T,98> yzxz, gbrb, tpsp;
        XVec4<T,99> yzxw, gbra, tpsq;
        XVec4<T,100> yzyx, gbgr, tpts;
        XVec4<T,101> yzyy, gbgg, tptt;
        XVec4<T,102> yzyz, gbgb, tptp;
        XVec4<T,103> yzyw, gbga, tptq;
        XVec4<T,104> yzzx, gbbr, tpps;
        XVec4<T,105> yzzy, gbbg, tppt;
        XVec4<T,106> yzzz, gbbb, tppp;
        XVec4<T,107> yzzw, gbba, tppq;
        XVec4<T,108> yzwx, gbar, tpqs;
        XVec4<T,109> yzwy, gbag, tpqt;
        XVec4<T,110> yzwz, gbab, tpqp;
        XVec4<T,111> yzww, gbaa, tpqq;
        XVec4<T,112> ywxx, garr, tqss;
        XVec4<T,113> ywxy, garg, tqst;
        XVec4<T,114> ywxz, garb, tqsp;
        XVec4<T,115> ywxw, gara, tqsq;
        XVec4<T,116> ywyx, gagr, tqts;
        XVec4<T,117> ywyy, gagg, tqtt;
        XVec4<T,118> ywyz, gagb, tqtp;
        XVec4<T,119> ywyw, gaga, tqtq;
        XVec4<T,120> ywzx, gabr, tqps;
        XVec4<T,121> ywzy, gabg, tqpt;
        XVec4<T,122> ywzz, gabb, tqpp;
        XVec4<T,123> ywzw, gaba, tqpq;
        XVec4<T,124> ywwx, gaar, tqqs;
        XVec4<T,125> ywwy, gaag, tqqt;
        XVec4<T,126> ywwz, gaab, tqqp;
        XVec4<T,127> ywww, gaaa, tqqq;
        XVec4<T,128> zxxx, brrr, psss;
        XVec4<T,129> zxxy, brrg, psst;
        XVec4<T,130> zxxz, brrb, pssp;
        XVec4<T,131> zxxw, brra, pssq;
        XVec4<T,132> zxyx, brgr, psts;
        XVec4<T,133> zxyy, brgg, pstt;
        XVec4<T,134> zxyz, brgb, pstp;
        XVec4<T,135> zxyw, brga, pstq;
        XVec4<T,136> zxzx, brbr, psps;
        XVec4<T,137> zxzy, brbg, pspt;
        XVec4<T,138> zxzz, brbb, pspp;
        XVec4<T,139> zxzw, brba, pspq;
        XVec4<T,140> zxwx, brar, psqs;
        XVec4<T,141> zxwy, brag, psqt;
        XVec4<T,142> zxwz, brab, psqp;
        XVec4<T,143> zxww, braa, psqq;
        XVec4<T,144> zyxx, bgrr, ptss;
        XVec4<T,145> zyxy, bgrg, ptst;
        XVec4<T,146> zyxz, bgrb, ptsp;
        XVec4<T,147> zyxw, bgra, ptsq;
        XVec4<T,148> zyyx, bggr, ptts;
        XVec4<T,149> zyyy, bggg, pttt;
        XVec4<T,150> zyyz, bggb, pttp;
        XVec4<T,151> zyyw, bgga, pttq;
        XVec4<T,152> zyzx, bgbr, ptps;
        XVec4<T,153> zyzy, bgbg, ptpt;
        XVec4<T,154> zyzz, bgbb, ptpp;
        XVec4<T,155> zyzw, bgba, ptpq;
        XVec4<T,156> zywx, bgar, ptqs;
        XVec4<T,157> zywy, bgag, ptqt;
        XVec4<T,158> zywz, bgab, ptqp;
        XVec4<T,159> zyww, bgaa, ptqq;
        XVec4<T,160> zzxx, bbrr, ppss;
        XVec4<T,161> zzxy, bbrg, ppst;
        XVec4<T,162> zzxz, bbrb, ppsp;
        XVec4<T,163> zzxw, bbra, ppsq;
        XVec4<T,164> zzyx, bbgr, ppts;
        XVec4<T,165> zzyy, bbgg, pptt;
        XVec4<T,166> zzyz, bbgb, pptp;
        XVec4<T,167> zzyw, bbga, pptq;
        XVec4<T,168> zzzx, bbbr, ppps;
        XVec4<T,169> zzzy, bbbg, pppt;
        XVec4<T,170> zzzz, bbbb, pppp;
        XVec4<T,171> zzzw, bbba, pppq;
        XVec4<T,172> zzwx, bbar, ppqs;
        XVec4<T,173> zzwy, bbag, ppqt;
        XVec4<T,174> zzwz, bbab, ppqp;
        XVec4<T,175> zzww, bbaa, ppqq;
        XVec4<T,176> zwxx, barr, pqss;
        XVec4<T,177> zwxy, barg, pqst;
        XVec4<T,178> zwxz, barb, pqsp;
        XVec4<T,179> zwxw, bara, pqsq;
        XVec4<T,180> zwyx, bagr, pqts;
        XVec4<T,181> zwyy, bagg, pqtt;
        XVec4<T,182> zwyz, bagb, pqtp;
        XVec4<T,183> zwyw, baga, pqtq;
        XVec4<T,184> zwzx, babr, pqps;
        XVec4<T,185> zwzy, babg, pqpt;
        XVec4<T,186> zwzz, babb, pqpp;
        XVec4<T,187> zwzw, baba, pqpq;
        XVec4<T,188> zwwx, baar, pqqs;
        XVec4<T,189> zwwy, baag, pqqt;
        XVec4<T,190> zwwz, baab, pqqp;
        XVec4<T,191> zwww, baaa, pqqq;
        XVec4<T,192> wxxx, arrr, qsss;
        XVec4<T,193> wxxy, arrg, qsst;
        XVec4<T,194> wxxz, arrb, qssp;
        XVec4<T,195> wxxw, arra, qssq;
        XVec4<T,196> wxyx, argr, qsts;
        XVec4<T,197> wxyy, argg, qstt;
        XVec4<T,198> wxyz, argb, qstp;
        XVec4<T,199> wxyw, arga, qstq;
        XVec4<T,200> wxzx, arbr, qsps;
        XVec4<T,201> wxzy, arbg, qspt;
        XVec4<T,202> wxzz, arbb, qspp;
        XVec4<T,203> wxzw, arba, qspq;
        XVec4<T,204> wxwx, arar, qsqs;
        XVec4<T,205> wxwy, arag, qsqt;
        XVec4<T,206> wxwz, arab, qsqp;
        XVec4<T,207> wxww, araa, qsqq;
        XVec4<T,208> wyxx, agrr, qtss;
        XVec4<T,209> wyxy, agrg, qtst;
        XVec4<T,210> wyxz, agrb, qtsp;
        XVec4<T,211> wyxw, agra, qtsq;
        XVec4<T,212> wyyx, aggr, qtts;
        XVec4<T,213> wyyy, aggg, qttt;
        XVec4<T,214> wyyz, aggb, qttp;
        XVec4<T,215> wyyw, agga, qttq;
        XVec4<T,216> wyzx, agbr, qtps;
        XVec4<T,217> wyzy, agbg, qtpt;
        XVec4<T,218> wyzz, agbb, qtpp;
        XVec4<T,219> wyzw, agba, qtpq;
        XVec4<T,220> wywx, agar, qtqs;
        XVec4<T,221> wywy, agag, qtqt;
        XVec4<T,222> wywz, agab, qtqp;
        XVec4<T,223> wyww, agaa, qtqq;
        XVec4<T,224> wzxx, abrr, qpss;
        XVec4<T,225> wzxy, abrg, qpst;
        XVec4<T,226> wzxz, abrb, qpsp;
        XVec4<T,227> wzxw, abra, qpsq;
        XVec4<T,228> wzyx, abgr, qpts;
        XVec4<T,229> wzyy, abgg, qptt;
        XVec4<T,230> wzyz, abgb, qptp;
        XVec4<T,231> wzyw, abga, qptq;
        XVec4<T,232> wzzx, abbr, qpps;
        XVec4<T,233> wzzy, abbg, qppt;
        XVec4<T,234> wzzz, abbb, qppp;
        XVec4<T,235> wzzw, abba, qppq;
        XVec4<T,236> wzwx, abar, qpqs;
        XVec4<T,237> wzwy, abag, qpqt;
        XVec4<T,238> wzwz, abab, qpqp;
        XVec4<T,239> wzww, abaa, qpqq;
        XVec4<T,240> wwxx, aarr, qqss;
        XVec4<T,241> wwxy, aarg, qqst;
        XVec4<T,242> wwxz, aarb, qqsp;
        XVec4<T,243> wwxw, aara, qqsq;
        XVec4<T,244> wwyx, aagr, qqts;
        XVec4<T,245> wwyy, aagg, qqtt;
        XVec4<T,246> wwyz, aagb, qqtp;
        XVec4<T,247> wwyw, aaga, qqtq;
        XVec4<T,248> wwzx, aabr, qqps;
        XVec4<T,249> wwzy, aabg, qqpt;
        XVec4<T,250> wwzz, aabb, qqpp;
        XVec4<T,251> wwzw, aaba, qqpq;
        XVec4<T,252> wwwx, aaar, qqqs;
        XVec4<T,253> wwwy, aaag, qqqt;
        XVec4<T,254> wwwz, aaab, qqqp;
        XVec4<T,255> wwww, aaaa, qqqq;
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

    MEMBER_OPS()

    inline Quat<T> operator *(Quat<T> const &val) const
    {
        Quat<T> ret;
        Vec3<T> v1(x, y, z);
        Vec3<T> v2(val.x, val.y, val.z);
        Vec3<T> v3 = cross(v1, v2) + w * v2 + val.w * v1;
        ret.x = v3.x;
        ret.y = v3.y;
        ret.z = v3.z;
        ret.w = w * val.w - dot(v1, v2);
        return ret;
    }

    inline Quat<T> operator *=(Quat<T> const &val)
    {
        return *this = (*this) * val;
    }

    inline Quat<T> operator ~() const
    {
        Quat<T> ret;
        for (int n = 0; n < 3; n++)
            ret[n] = -(*this)[n];
        ret[3] = (*this)[3];
        return ret;
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
 * Magic vector swizzling (part 2/2)
 */

template<typename T, int N>
inline Vec2<T> XVec2<T, N>::operator =(Vec2<T> that)
{
    ptr[I] = that.x; ptr[J] = that.y;
    return *this;
}

template<typename T, int N>
inline Vec3<T> XVec3<T, N>::operator =(Vec3<T> that)
{
    ptr[I] = that.x; ptr[J] = that.y; ptr[K] = that.z;
    return *this;
}

template<typename T, int N>
inline Vec4<T> XVec4<T, N>::operator =(Vec4<T> that)
{
    ptr[I] = that.x; ptr[J] = that.y; ptr[K] = that.z; ptr[L] = that.w;
    return *this;
}

/*
 * 4×4-element matrices
 */

template <typename T> struct Mat4
{
    inline Mat4() {}
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

