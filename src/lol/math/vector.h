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

#include "half.h"
#include "lol/math/real.h"

namespace lol
{

/* Some compilers do not support const members in anonymous unions. So
 * far, GCC (>= 4.6), CLang (3.0) and Visual Studio (>= 2010) appear to
 * work properly. */
#undef LOL_NO_CONST_MEMBERS_IN_ANONYMOUS_UNIONS
#if defined __GNUC__ && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6))
#   define LOL_NO_CONST_MEMBERS_IN_ANONYMOUS_UNIONS 1
#endif

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
    typedef tname<uint64_t> u64##suffix; \
    typedef tname<real> r##suffix; \

DECLARE_VECTOR_TYPEDEFS(Vec2, vec2)
DECLARE_VECTOR_TYPEDEFS(Cmplx, cmplx)
DECLARE_VECTOR_TYPEDEFS(Vec3, vec3)
DECLARE_VECTOR_TYPEDEFS(Vec4, vec4)
DECLARE_VECTOR_TYPEDEFS(Quat, quat)
DECLARE_VECTOR_TYPEDEFS(Mat4, mat4)

/*
 * Magic vector swizzling (part 1/2)
 * These vectors are empty, but thanks to static_cast we can take their
 * address and access the vector of T's that they are union'ed with. We
 * use static_cast instead of reinterpret_cast because there is a stronger
 * guarantee (by the standard) that the address will stay the same across
 * casts.
 */

template<typename T, int N> struct XVec2
{
    inline Vec2<T> operator =(Vec2<T> const &that);

    inline T& operator[](int n)
    {
        int i = (N >> (4 * (1 - n))) & 3;
        return static_cast<T*>(static_cast<void*>(this))[i];
    }
    inline T const& operator[](int n) const
    {
        int i = (N >> (4 * (1 - n))) & 3;
        return static_cast<T const*>(static_cast<void const *>(this))[i];
    }
};

template<typename T, int N> struct XVec3
{
    inline Vec3<T> operator =(Vec3<T> const &that);

    inline T& operator[](int n)
    {
        int i = (N >> (4 * (2 - n))) & 3;
        return static_cast<T*>(static_cast<void*>(this))[i];
    }
    inline T const& operator[](int n) const
    {
        int i = (N >> (4 * (2 - n))) & 3;
        return static_cast<T const*>(static_cast<void const *>(this))[i];
    }
};

template<typename T, int N> struct XVec4
{
    inline Vec4<T> operator =(Vec4<T> const &that);

    inline T& operator[](int n)
    {
        int i = (N >> (4 * (3 - n))) & 3;
        return static_cast<T*>(static_cast<void*>(this))[i];
    }
    inline T const& operator[](int n) const
    {
        int i = (N >> (4 * (3 - n))) & 3;
        return static_cast<T const*>(static_cast<void const *>(this))[i];
    }
};

/*
 * Helper macro for vector type member functions
 */

#define DECLARE_MEMBER_OPS(tname) \
    inline T& operator[](int n) { return *(&this->x + n); } \
    inline T const& operator[](int n) const { return *(&this->x + n); } \
    \
    /* Visual Studio insists on having an assignment operator. */ \
    inline tname<T> const & operator =(tname<T> const &that) \
    { \
        for (size_t n = 0; n < sizeof(*this) / sizeof(T); n++) \
            (*this)[n] = that[n]; \
        return *this; \
    } \
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
    void printf() const;

/*
 * 2-element vectors
 */

template <typename T> struct BVec2
{
    explicit inline BVec2() {}
    explicit inline BVec2(T X, T Y) : x(X), y(Y) {}

    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T s, t; };

#if LOL_NO_CONST_MEMBERS_IN_ANONYMOUS_UNIONS
#   define const /* disabled */
#endif
        XVec2<T,0x00> const xx, rr, ss;
        XVec2<T,0x01> const xy, rg, st; /* lvalue */
        XVec2<T,0x10> const yx, gr, ts; /* lvalue */
        XVec2<T,0x11> const yy, gg, tt;

        XVec3<T,0x000> const xxx, rrr, sss;
        XVec3<T,0x001> const xxy, rrg, sst;
        XVec3<T,0x010> const xyx, rgr, sts;
        XVec3<T,0x011> const xyy, rgg, stt;
        XVec3<T,0x100> const yxx, grr, tss;
        XVec3<T,0x101> const yxy, grg, tst;
        XVec3<T,0x110> const yyx, ggr, tts;
        XVec3<T,0x111> const yyy, ggg, ttt;

        XVec4<T,0x0000> const xxxx, rrrr, ssss;
        XVec4<T,0x0001> const xxxy, rrrg, ssst;
        XVec4<T,0x0010> const xxyx, rrgr, ssts;
        XVec4<T,0x0011> const xxyy, rrgg, sstt;
        XVec4<T,0x0100> const xyxx, rgrr, stss;
        XVec4<T,0x0101> const xyxy, rgrg, stst;
        XVec4<T,0x0110> const xyyx, rggr, stts;
        XVec4<T,0x0111> const xyyy, rggg, sttt;
        XVec4<T,0x1000> const yxxx, grrr, tsss;
        XVec4<T,0x1001> const yxxy, grrg, tsst;
        XVec4<T,0x1010> const yxyx, grgr, tsts;
        XVec4<T,0x1011> const yxyy, grgg, tstt;
        XVec4<T,0x1100> const yyxx, ggrr, ttss;
        XVec4<T,0x1101> const yyxy, ggrg, ttst;
        XVec4<T,0x1110> const yyyx, gggr, ttts;
        XVec4<T,0x1111> const yyyy, gggg, tttt;
#if LOL_NO_CONST_MEMBERS_IN_ANONYMOUS_UNIONS
#   undef const
#endif
    };
};

template <> struct BVec2<half>
{
    explicit inline BVec2() {}
    explicit inline BVec2(half X, half Y) : x(X), y(Y) {}

    half x, y;
};

template <> struct BVec2<real>
{
    explicit inline BVec2() {}
    explicit inline BVec2(real X, real Y) : x(X), y(Y) {}

    real x, y;
};

template <typename T> struct Vec2 : BVec2<T>
{
    inline Vec2() {}
    inline Vec2(T X, T Y) : BVec2<T>(X, Y) {}

    explicit inline Vec2(T X) : BVec2<T>(X, X) {}

    template<int N>
    inline Vec2(XVec2<T, N> const &v)
      : BVec2<T>(v[0], v[1]) {}

    template<typename U, int N>
    explicit inline Vec2(XVec2<U, N> const &v)
      : BVec2<T>(v[0], v[1]) {}

    DECLARE_MEMBER_OPS(Vec2)

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec2<U> const &v);
#endif
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

template <typename T> struct BVec3
{
    explicit inline BVec3() {}
    explicit inline BVec3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { T s, t, p; };

#if LOL_NO_CONST_MEMBERS_IN_ANONYMOUS_UNIONS
#   define const /* disabled */
#endif
        XVec2<T,0x00> const xx, rr, ss;
        XVec2<T,0x01> const xy, rg, st; /* lvalue */
        XVec2<T,0x02> const xz, rb, sp; /* lvalue */
        XVec2<T,0x10> const yx, gr, ts; /* lvalue */
        XVec2<T,0x11> const yy, gg, tt;
        XVec2<T,0x12> const yz, gb, tp; /* lvalue */
        XVec2<T,0x20> const zx, br, ps; /* lvalue */
        XVec2<T,0x21> const zy, bg, pt; /* lvalue */
        XVec2<T,0x22> const zz, bb, pp;

        XVec3<T,0x000> const xxx, rrr, sss;
        XVec3<T,0x001> const xxy, rrg, sst;
        XVec3<T,0x002> const xxz, rrb, ssp;
        XVec3<T,0x010> const xyx, rgr, sts;
        XVec3<T,0x011> const xyy, rgg, stt;
        XVec3<T,0x012> const xyz, rgb, stp; /* lvalue */
        XVec3<T,0x020> const xzx, rbr, sps;
        XVec3<T,0x021> const xzy, rbg, spt; /* lvalue */
        XVec3<T,0x022> const xzz, rbb, spp;
        XVec3<T,0x100> const yxx, grr, tss;
        XVec3<T,0x101> const yxy, grg, tst;
        XVec3<T,0x102> const yxz, grb, tsp; /* lvalue */
        XVec3<T,0x110> const yyx, ggr, tts;
        XVec3<T,0x111> const yyy, ggg, ttt;
        XVec3<T,0x112> const yyz, ggb, ttp;
        XVec3<T,0x120> const yzx, gbr, tps; /* lvalue */
        XVec3<T,0x121> const yzy, gbg, tpt;
        XVec3<T,0x122> const yzz, gbb, tpp;
        XVec3<T,0x200> const zxx, brr, pss;
        XVec3<T,0x201> const zxy, brg, pst; /* lvalue */
        XVec3<T,0x202> const zxz, brb, psp;
        XVec3<T,0x210> const zyx, bgr, pts; /* lvalue */
        XVec3<T,0x211> const zyy, bgg, ptt;
        XVec3<T,0x212> const zyz, bgb, ptp;
        XVec3<T,0x220> const zzx, bbr, pps;
        XVec3<T,0x221> const zzy, bbg, ppt;
        XVec3<T,0x222> const zzz, bbb, ppp;

        XVec4<T,0x0000> const xxxx, rrrr, ssss;
        XVec4<T,0x0001> const xxxy, rrrg, ssst;
        XVec4<T,0x0002> const xxxz, rrrb, sssp;
        XVec4<T,0x0010> const xxyx, rrgr, ssts;
        XVec4<T,0x0011> const xxyy, rrgg, sstt;
        XVec4<T,0x0012> const xxyz, rrgb, sstp;
        XVec4<T,0x0020> const xxzx, rrbr, ssps;
        XVec4<T,0x0021> const xxzy, rrbg, sspt;
        XVec4<T,0x0022> const xxzz, rrbb, sspp;
        XVec4<T,0x0100> const xyxx, rgrr, stss;
        XVec4<T,0x0101> const xyxy, rgrg, stst;
        XVec4<T,0x0102> const xyxz, rgrb, stsp;
        XVec4<T,0x0110> const xyyx, rggr, stts;
        XVec4<T,0x0111> const xyyy, rggg, sttt;
        XVec4<T,0x0112> const xyyz, rggb, sttp;
        XVec4<T,0x0120> const xyzx, rgbr, stps;
        XVec4<T,0x0121> const xyzy, rgbg, stpt;
        XVec4<T,0x0122> const xyzz, rgbb, stpp;
        XVec4<T,0x0200> const xzxx, rbrr, spss;
        XVec4<T,0x0201> const xzxy, rbrg, spst;
        XVec4<T,0x0202> const xzxz, rbrb, spsp;
        XVec4<T,0x0210> const xzyx, rbgr, spts;
        XVec4<T,0x0211> const xzyy, rbgg, sptt;
        XVec4<T,0x0212> const xzyz, rbgb, sptp;
        XVec4<T,0x0220> const xzzx, rbbr, spps;
        XVec4<T,0x0221> const xzzy, rbbg, sppt;
        XVec4<T,0x0222> const xzzz, rbbb, sppp;
        XVec4<T,0x1000> const yxxx, grrr, tsss;
        XVec4<T,0x1001> const yxxy, grrg, tsst;
        XVec4<T,0x1002> const yxxz, grrb, tssp;
        XVec4<T,0x1010> const yxyx, grgr, tsts;
        XVec4<T,0x1011> const yxyy, grgg, tstt;
        XVec4<T,0x1012> const yxyz, grgb, tstp;
        XVec4<T,0x1020> const yxzx, grbr, tsps;
        XVec4<T,0x1021> const yxzy, grbg, tspt;
        XVec4<T,0x1022> const yxzz, grbb, tspp;
        XVec4<T,0x1100> const yyxx, ggrr, ttss;
        XVec4<T,0x1101> const yyxy, ggrg, ttst;
        XVec4<T,0x1102> const yyxz, ggrb, ttsp;
        XVec4<T,0x1110> const yyyx, gggr, ttts;
        XVec4<T,0x1111> const yyyy, gggg, tttt;
        XVec4<T,0x1112> const yyyz, gggb, tttp;
        XVec4<T,0x1120> const yyzx, ggbr, ttps;
        XVec4<T,0x1121> const yyzy, ggbg, ttpt;
        XVec4<T,0x1122> const yyzz, ggbb, ttpp;
        XVec4<T,0x1200> const yzxx, gbrr, tpss;
        XVec4<T,0x1201> const yzxy, gbrg, tpst;
        XVec4<T,0x1202> const yzxz, gbrb, tpsp;
        XVec4<T,0x1210> const yzyx, gbgr, tpts;
        XVec4<T,0x1211> const yzyy, gbgg, tptt;
        XVec4<T,0x1212> const yzyz, gbgb, tptp;
        XVec4<T,0x1220> const yzzx, gbbr, tpps;
        XVec4<T,0x1221> const yzzy, gbbg, tppt;
        XVec4<T,0x1222> const yzzz, gbbb, tppp;
        XVec4<T,0x2000> const zxxx, brrr, psss;
        XVec4<T,0x2001> const zxxy, brrg, psst;
        XVec4<T,0x2002> const zxxz, brrb, pssp;
        XVec4<T,0x2010> const zxyx, brgr, psts;
        XVec4<T,0x2011> const zxyy, brgg, pstt;
        XVec4<T,0x2012> const zxyz, brgb, pstp;
        XVec4<T,0x2020> const zxzx, brbr, psps;
        XVec4<T,0x2021> const zxzy, brbg, pspt;
        XVec4<T,0x2022> const zxzz, brbb, pspp;
        XVec4<T,0x2100> const zyxx, bgrr, ptss;
        XVec4<T,0x2101> const zyxy, bgrg, ptst;
        XVec4<T,0x2102> const zyxz, bgrb, ptsp;
        XVec4<T,0x2110> const zyyx, bggr, ptts;
        XVec4<T,0x2111> const zyyy, bggg, pttt;
        XVec4<T,0x2112> const zyyz, bggb, pttp;
        XVec4<T,0x2120> const zyzx, bgbr, ptps;
        XVec4<T,0x2121> const zyzy, bgbg, ptpt;
        XVec4<T,0x2122> const zyzz, bgbb, ptpp;
        XVec4<T,0x2200> const zzxx, bbrr, ppss;
        XVec4<T,0x2201> const zzxy, bbrg, ppst;
        XVec4<T,0x2202> const zzxz, bbrb, ppsp;
        XVec4<T,0x2210> const zzyx, bbgr, ppts;
        XVec4<T,0x2211> const zzyy, bbgg, pptt;
        XVec4<T,0x2212> const zzyz, bbgb, pptp;
        XVec4<T,0x2220> const zzzx, bbbr, ppps;
        XVec4<T,0x2221> const zzzy, bbbg, pppt;
        XVec4<T,0x2222> const zzzz, bbbb, pppp;
#if LOL_NO_CONST_MEMBERS_IN_ANONYMOUS_UNIONS
#   undef const
#endif
    };
};

template <> struct BVec3<half>
{
    explicit inline BVec3() {}
    explicit inline BVec3(half X, half Y, half Z) : x(X), y(Y), z(Z) {}

    half x, y, z;
};

template <> struct BVec3<real>
{
    explicit inline BVec3() {}
    explicit inline BVec3(real X, real Y, real Z) : x(X), y(Y), z(Z) {}

    real x, y, z;
};

template <typename T> struct Vec3 : BVec3<T>
{
    inline Vec3() {}
    inline Vec3(T X, T Y, T Z) : BVec3<T>(X, Y, Z) {}
    inline Vec3(Vec2<T> XY, T Z) : BVec3<T>(XY.x, XY.y, Z) {}
    inline Vec3(T X, Vec2<T> YZ) : BVec3<T>(X, YZ.x, YZ.y) {}

    explicit inline Vec3(T X) : BVec3<T>(X, X, X) {}

    template<int N>
    inline Vec3(XVec3<T, N> const &v)
      : BVec3<T>(v[0], v[1], v[2]) {}

    template<typename U, int N>
    explicit inline Vec3(XVec3<U, N> const &v)
      : BVec3<T>(v[0], v[1], v[2]) {}

    DECLARE_MEMBER_OPS(Vec3)

    template<typename U>
    friend Vec3<U> cross(Vec3<U>, Vec3<U>);

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec3<U> const &v);
#endif
};

/*
 * 4-element vectors
 */

template <typename T> struct BVec4
{
    explicit inline BVec4() {}
    explicit inline BVec4(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

    union
    {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T s, t, p, q; };

#if LOL_NO_CONST_MEMBERS_IN_ANONYMOUS_UNIONS
#   define const /* disabled */
#endif
        XVec2<T,0x00> const xx, rr, ss;
        XVec2<T,0x01> const xy, rg, st; /* lvalue */
        XVec2<T,0x02> const xz, rb, sp; /* lvalue */
        XVec2<T,0x03> const xw, ra, sq; /* lvalue */
        XVec2<T,0x10> const yx, gr, ts; /* lvalue */
        XVec2<T,0x11> const yy, gg, tt;
        XVec2<T,0x12> const yz, gb, tp; /* lvalue */
        XVec2<T,0x13> const yw, ga, tq; /* lvalue */
        XVec2<T,0x20> const zx, br, ps; /* lvalue */
        XVec2<T,0x21> const zy, bg, pt; /* lvalue */
        XVec2<T,0x22> const zz, bb, pp;
        XVec2<T,0x23> const zw, ba, pq; /* lvalue */
        XVec2<T,0x30> const wx, ar, qs; /* lvalue */
        XVec2<T,0x31> const wy, ag, qt; /* lvalue */
        XVec2<T,0x32> const wz, ab, qp; /* lvalue */
        XVec2<T,0x33> const ww, aa, qq;

        XVec3<T,0x000> const xxx, rrr, sss;
        XVec3<T,0x001> const xxy, rrg, sst;
        XVec3<T,0x002> const xxz, rrb, ssp;
        XVec3<T,0x003> const xxw, rra, ssq;
        XVec3<T,0x010> const xyx, rgr, sts;
        XVec3<T,0x011> const xyy, rgg, stt;
        XVec3<T,0x012> const xyz, rgb, stp; /* lvalue */
        XVec3<T,0x013> const xyw, rga, stq; /* lvalue */
        XVec3<T,0x020> const xzx, rbr, sps;
        XVec3<T,0x021> const xzy, rbg, spt; /* lvalue */
        XVec3<T,0x022> const xzz, rbb, spp;
        XVec3<T,0x023> const xzw, rba, spq; /* lvalue */
        XVec3<T,0x030> const xwx, rar, sqs;
        XVec3<T,0x031> const xwy, rag, sqt; /* lvalue */
        XVec3<T,0x032> const xwz, rab, sqp; /* lvalue */
        XVec3<T,0x033> const xww, raa, sqq;
        XVec3<T,0x100> const yxx, grr, tss;
        XVec3<T,0x101> const yxy, grg, tst;
        XVec3<T,0x102> const yxz, grb, tsp; /* lvalue */
        XVec3<T,0x103> const yxw, gra, tsq; /* lvalue */
        XVec3<T,0x110> const yyx, ggr, tts;
        XVec3<T,0x111> const yyy, ggg, ttt;
        XVec3<T,0x112> const yyz, ggb, ttp;
        XVec3<T,0x113> const yyw, gga, ttq;
        XVec3<T,0x120> const yzx, gbr, tps; /* lvalue */
        XVec3<T,0x121> const yzy, gbg, tpt;
        XVec3<T,0x122> const yzz, gbb, tpp;
        XVec3<T,0x123> const yzw, gba, tpq; /* lvalue */
        XVec3<T,0x130> const ywx, gar, tqs; /* lvalue */
        XVec3<T,0x131> const ywy, gag, tqt;
        XVec3<T,0x132> const ywz, gab, tqp; /* lvalue */
        XVec3<T,0x133> const yww, gaa, tqq;
        XVec3<T,0x200> const zxx, brr, pss;
        XVec3<T,0x201> const zxy, brg, pst; /* lvalue */
        XVec3<T,0x202> const zxz, brb, psp;
        XVec3<T,0x203> const zxw, bra, psq; /* lvalue */
        XVec3<T,0x210> const zyx, bgr, pts; /* lvalue */
        XVec3<T,0x211> const zyy, bgg, ptt;
        XVec3<T,0x212> const zyz, bgb, ptp;
        XVec3<T,0x213> const zyw, bga, ptq; /* lvalue */
        XVec3<T,0x220> const zzx, bbr, pps;
        XVec3<T,0x221> const zzy, bbg, ppt;
        XVec3<T,0x222> const zzz, bbb, ppp;
        XVec3<T,0x223> const zzw, bba, ppq;
        XVec3<T,0x230> const zwx, bar, pqs; /* lvalue */
        XVec3<T,0x231> const zwy, bag, pqt; /* lvalue */
        XVec3<T,0x232> const zwz, bab, pqp;
        XVec3<T,0x233> const zww, baa, pqq;
        XVec3<T,0x300> const wxx, arr, qss;
        XVec3<T,0x301> const wxy, arg, qst; /* lvalue */
        XVec3<T,0x302> const wxz, arb, qsp; /* lvalue */
        XVec3<T,0x303> const wxw, ara, qsq;
        XVec3<T,0x310> const wyx, agr, qts; /* lvalue */
        XVec3<T,0x311> const wyy, agg, qtt;
        XVec3<T,0x312> const wyz, agb, qtp; /* lvalue */
        XVec3<T,0x313> const wyw, aga, qtq;
        XVec3<T,0x320> const wzx, abr, qps; /* lvalue */
        XVec3<T,0x321> const wzy, abg, qpt; /* lvalue */
        XVec3<T,0x322> const wzz, abb, qpp;
        XVec3<T,0x323> const wzw, aba, qpq;
        XVec3<T,0x330> const wwx, aar, qqs;
        XVec3<T,0x331> const wwy, aag, qqt;
        XVec3<T,0x332> const wwz, aab, qqp;
        XVec3<T,0x333> const www, aaa, qqq;

        XVec4<T,0x0000> const xxxx, rrrr, ssss;
        XVec4<T,0x0001> const xxxy, rrrg, ssst;
        XVec4<T,0x0002> const xxxz, rrrb, sssp;
        XVec4<T,0x0003> const xxxw, rrra, sssq;
        XVec4<T,0x0010> const xxyx, rrgr, ssts;
        XVec4<T,0x0011> const xxyy, rrgg, sstt;
        XVec4<T,0x0012> const xxyz, rrgb, sstp;
        XVec4<T,0x0013> const xxyw, rrga, sstq;
        XVec4<T,0x0020> const xxzx, rrbr, ssps;
        XVec4<T,0x0021> const xxzy, rrbg, sspt;
        XVec4<T,0x0022> const xxzz, rrbb, sspp;
        XVec4<T,0x0023> const xxzw, rrba, sspq;
        XVec4<T,0x0030> const xxwx, rrar, ssqs;
        XVec4<T,0x0031> const xxwy, rrag, ssqt;
        XVec4<T,0x0032> const xxwz, rrab, ssqp;
        XVec4<T,0x0033> const xxww, rraa, ssqq;
        XVec4<T,0x0100> const xyxx, rgrr, stss;
        XVec4<T,0x0101> const xyxy, rgrg, stst;
        XVec4<T,0x0102> const xyxz, rgrb, stsp;
        XVec4<T,0x0103> const xyxw, rgra, stsq;
        XVec4<T,0x0110> const xyyx, rggr, stts;
        XVec4<T,0x0111> const xyyy, rggg, sttt;
        XVec4<T,0x0112> const xyyz, rggb, sttp;
        XVec4<T,0x0113> const xyyw, rgga, sttq;
        XVec4<T,0x0120> const xyzx, rgbr, stps;
        XVec4<T,0x0121> const xyzy, rgbg, stpt;
        XVec4<T,0x0122> const xyzz, rgbb, stpp;
        XVec4<T,0x0123> const xyzw, rgba, stpq; /* lvalue */
        XVec4<T,0x0130> const xywx, rgar, stqs;
        XVec4<T,0x0131> const xywy, rgag, stqt;
        XVec4<T,0x0132> const xywz, rgab, stqp; /* lvalue */
        XVec4<T,0x0133> const xyww, rgaa, stqq;
        XVec4<T,0x0200> const xzxx, rbrr, spss;
        XVec4<T,0x0201> const xzxy, rbrg, spst;
        XVec4<T,0x0202> const xzxz, rbrb, spsp;
        XVec4<T,0x0203> const xzxw, rbra, spsq;
        XVec4<T,0x0210> const xzyx, rbgr, spts;
        XVec4<T,0x0211> const xzyy, rbgg, sptt;
        XVec4<T,0x0212> const xzyz, rbgb, sptp;
        XVec4<T,0x0213> const xzyw, rbga, sptq; /* lvalue */
        XVec4<T,0x0220> const xzzx, rbbr, spps;
        XVec4<T,0x0221> const xzzy, rbbg, sppt;
        XVec4<T,0x0222> const xzzz, rbbb, sppp;
        XVec4<T,0x0223> const xzzw, rbba, sppq;
        XVec4<T,0x0230> const xzwx, rbar, spqs;
        XVec4<T,0x0231> const xzwy, rbag, spqt; /* lvalue */
        XVec4<T,0x0232> const xzwz, rbab, spqp;
        XVec4<T,0x0233> const xzww, rbaa, spqq;
        XVec4<T,0x0300> const xwxx, rarr, sqss;
        XVec4<T,0x0301> const xwxy, rarg, sqst;
        XVec4<T,0x0302> const xwxz, rarb, sqsp;
        XVec4<T,0x0303> const xwxw, rara, sqsq;
        XVec4<T,0x0310> const xwyx, ragr, sqts;
        XVec4<T,0x0311> const xwyy, ragg, sqtt;
        XVec4<T,0x0312> const xwyz, ragb, sqtp; /* lvalue */
        XVec4<T,0x0313> const xwyw, raga, sqtq;
        XVec4<T,0x0320> const xwzx, rabr, sqps;
        XVec4<T,0x0321> const xwzy, rabg, sqpt; /* lvalue */
        XVec4<T,0x0322> const xwzz, rabb, sqpp;
        XVec4<T,0x0323> const xwzw, raba, sqpq;
        XVec4<T,0x0330> const xwwx, raar, sqqs;
        XVec4<T,0x0331> const xwwy, raag, sqqt;
        XVec4<T,0x0332> const xwwz, raab, sqqp;
        XVec4<T,0x0333> const xwww, raaa, sqqq;
        XVec4<T,0x1000> const yxxx, grrr, tsss;
        XVec4<T,0x1001> const yxxy, grrg, tsst;
        XVec4<T,0x1002> const yxxz, grrb, tssp;
        XVec4<T,0x1003> const yxxw, grra, tssq;
        XVec4<T,0x1010> const yxyx, grgr, tsts;
        XVec4<T,0x1011> const yxyy, grgg, tstt;
        XVec4<T,0x1012> const yxyz, grgb, tstp;
        XVec4<T,0x1013> const yxyw, grga, tstq;
        XVec4<T,0x1020> const yxzx, grbr, tsps;
        XVec4<T,0x1021> const yxzy, grbg, tspt;
        XVec4<T,0x1022> const yxzz, grbb, tspp;
        XVec4<T,0x1023> const yxzw, grba, tspq; /* lvalue */
        XVec4<T,0x1030> const yxwx, grar, tsqs;
        XVec4<T,0x1031> const yxwy, grag, tsqt;
        XVec4<T,0x1032> const yxwz, grab, tsqp; /* lvalue */
        XVec4<T,0x1033> const yxww, graa, tsqq;
        XVec4<T,0x1100> const yyxx, ggrr, ttss;
        XVec4<T,0x1101> const yyxy, ggrg, ttst;
        XVec4<T,0x1102> const yyxz, ggrb, ttsp;
        XVec4<T,0x1103> const yyxw, ggra, ttsq;
        XVec4<T,0x1110> const yyyx, gggr, ttts;
        XVec4<T,0x1111> const yyyy, gggg, tttt;
        XVec4<T,0x1112> const yyyz, gggb, tttp;
        XVec4<T,0x1113> const yyyw, ggga, tttq;
        XVec4<T,0x1120> const yyzx, ggbr, ttps;
        XVec4<T,0x1121> const yyzy, ggbg, ttpt;
        XVec4<T,0x1122> const yyzz, ggbb, ttpp;
        XVec4<T,0x1123> const yyzw, ggba, ttpq;
        XVec4<T,0x1130> const yywx, ggar, ttqs;
        XVec4<T,0x1131> const yywy, ggag, ttqt;
        XVec4<T,0x1132> const yywz, ggab, ttqp;
        XVec4<T,0x1133> const yyww, ggaa, ttqq;
        XVec4<T,0x1200> const yzxx, gbrr, tpss;
        XVec4<T,0x1201> const yzxy, gbrg, tpst;
        XVec4<T,0x1202> const yzxz, gbrb, tpsp;
        XVec4<T,0x1203> const yzxw, gbra, tpsq; /* lvalue */
        XVec4<T,0x1210> const yzyx, gbgr, tpts;
        XVec4<T,0x1211> const yzyy, gbgg, tptt;
        XVec4<T,0x1212> const yzyz, gbgb, tptp;
        XVec4<T,0x1213> const yzyw, gbga, tptq;
        XVec4<T,0x1220> const yzzx, gbbr, tpps;
        XVec4<T,0x1221> const yzzy, gbbg, tppt;
        XVec4<T,0x1222> const yzzz, gbbb, tppp;
        XVec4<T,0x1223> const yzzw, gbba, tppq;
        XVec4<T,0x1230> const yzwx, gbar, tpqs; /* lvalue */
        XVec4<T,0x1231> const yzwy, gbag, tpqt;
        XVec4<T,0x1232> const yzwz, gbab, tpqp;
        XVec4<T,0x1233> const yzww, gbaa, tpqq;
        XVec4<T,0x1300> const ywxx, garr, tqss;
        XVec4<T,0x1301> const ywxy, garg, tqst;
        XVec4<T,0x1302> const ywxz, garb, tqsp; /* lvalue */
        XVec4<T,0x1303> const ywxw, gara, tqsq;
        XVec4<T,0x1310> const ywyx, gagr, tqts;
        XVec4<T,0x1311> const ywyy, gagg, tqtt;
        XVec4<T,0x1312> const ywyz, gagb, tqtp;
        XVec4<T,0x1313> const ywyw, gaga, tqtq;
        XVec4<T,0x1320> const ywzx, gabr, tqps; /* lvalue */
        XVec4<T,0x1321> const ywzy, gabg, tqpt;
        XVec4<T,0x1322> const ywzz, gabb, tqpp;
        XVec4<T,0x1323> const ywzw, gaba, tqpq;
        XVec4<T,0x1330> const ywwx, gaar, tqqs;
        XVec4<T,0x1331> const ywwy, gaag, tqqt;
        XVec4<T,0x1332> const ywwz, gaab, tqqp;
        XVec4<T,0x1333> const ywww, gaaa, tqqq;
        XVec4<T,0x2000> const zxxx, brrr, psss;
        XVec4<T,0x2001> const zxxy, brrg, psst;
        XVec4<T,0x2002> const zxxz, brrb, pssp;
        XVec4<T,0x2003> const zxxw, brra, pssq;
        XVec4<T,0x2010> const zxyx, brgr, psts;
        XVec4<T,0x2011> const zxyy, brgg, pstt;
        XVec4<T,0x2012> const zxyz, brgb, pstp;
        XVec4<T,0x2013> const zxyw, brga, pstq; /* lvalue */
        XVec4<T,0x2020> const zxzx, brbr, psps;
        XVec4<T,0x2021> const zxzy, brbg, pspt;
        XVec4<T,0x2022> const zxzz, brbb, pspp;
        XVec4<T,0x2023> const zxzw, brba, pspq;
        XVec4<T,0x2030> const zxwx, brar, psqs;
        XVec4<T,0x2031> const zxwy, brag, psqt; /* lvalue */
        XVec4<T,0x2032> const zxwz, brab, psqp;
        XVec4<T,0x2033> const zxww, braa, psqq;
        XVec4<T,0x2100> const zyxx, bgrr, ptss;
        XVec4<T,0x2101> const zyxy, bgrg, ptst;
        XVec4<T,0x2102> const zyxz, bgrb, ptsp;
        XVec4<T,0x2103> const zyxw, bgra, ptsq; /* lvalue */
        XVec4<T,0x2110> const zyyx, bggr, ptts;
        XVec4<T,0x2111> const zyyy, bggg, pttt;
        XVec4<T,0x2112> const zyyz, bggb, pttp;
        XVec4<T,0x2113> const zyyw, bgga, pttq;
        XVec4<T,0x2120> const zyzx, bgbr, ptps;
        XVec4<T,0x2121> const zyzy, bgbg, ptpt;
        XVec4<T,0x2122> const zyzz, bgbb, ptpp;
        XVec4<T,0x2123> const zyzw, bgba, ptpq;
        XVec4<T,0x2130> const zywx, bgar, ptqs; /* lvalue */
        XVec4<T,0x2131> const zywy, bgag, ptqt;
        XVec4<T,0x2132> const zywz, bgab, ptqp;
        XVec4<T,0x2133> const zyww, bgaa, ptqq;
        XVec4<T,0x2200> const zzxx, bbrr, ppss;
        XVec4<T,0x2201> const zzxy, bbrg, ppst;
        XVec4<T,0x2202> const zzxz, bbrb, ppsp;
        XVec4<T,0x2203> const zzxw, bbra, ppsq;
        XVec4<T,0x2210> const zzyx, bbgr, ppts;
        XVec4<T,0x2211> const zzyy, bbgg, pptt;
        XVec4<T,0x2212> const zzyz, bbgb, pptp;
        XVec4<T,0x2213> const zzyw, bbga, pptq;
        XVec4<T,0x2220> const zzzx, bbbr, ppps;
        XVec4<T,0x2221> const zzzy, bbbg, pppt;
        XVec4<T,0x2222> const zzzz, bbbb, pppp;
        XVec4<T,0x2223> const zzzw, bbba, pppq;
        XVec4<T,0x2230> const zzwx, bbar, ppqs;
        XVec4<T,0x2231> const zzwy, bbag, ppqt;
        XVec4<T,0x2232> const zzwz, bbab, ppqp;
        XVec4<T,0x2233> const zzww, bbaa, ppqq;
        XVec4<T,0x2300> const zwxx, barr, pqss;
        XVec4<T,0x2301> const zwxy, barg, pqst; /* lvalue */
        XVec4<T,0x2302> const zwxz, barb, pqsp;
        XVec4<T,0x2303> const zwxw, bara, pqsq;
        XVec4<T,0x2310> const zwyx, bagr, pqts; /* lvalue */
        XVec4<T,0x2311> const zwyy, bagg, pqtt;
        XVec4<T,0x2312> const zwyz, bagb, pqtp;
        XVec4<T,0x2313> const zwyw, baga, pqtq;
        XVec4<T,0x2320> const zwzx, babr, pqps;
        XVec4<T,0x2321> const zwzy, babg, pqpt;
        XVec4<T,0x2322> const zwzz, babb, pqpp;
        XVec4<T,0x2323> const zwzw, baba, pqpq;
        XVec4<T,0x2330> const zwwx, baar, pqqs;
        XVec4<T,0x2331> const zwwy, baag, pqqt;
        XVec4<T,0x2332> const zwwz, baab, pqqp;
        XVec4<T,0x2333> const zwww, baaa, pqqq;
        XVec4<T,0x3000> const wxxx, arrr, qsss;
        XVec4<T,0x3001> const wxxy, arrg, qsst;
        XVec4<T,0x3002> const wxxz, arrb, qssp;
        XVec4<T,0x3003> const wxxw, arra, qssq;
        XVec4<T,0x3010> const wxyx, argr, qsts;
        XVec4<T,0x3011> const wxyy, argg, qstt;
        XVec4<T,0x3012> const wxyz, argb, qstp; /* lvalue */
        XVec4<T,0x3013> const wxyw, arga, qstq;
        XVec4<T,0x3020> const wxzx, arbr, qsps;
        XVec4<T,0x3021> const wxzy, arbg, qspt; /* lvalue */
        XVec4<T,0x3022> const wxzz, arbb, qspp;
        XVec4<T,0x3023> const wxzw, arba, qspq;
        XVec4<T,0x3030> const wxwx, arar, qsqs;
        XVec4<T,0x3031> const wxwy, arag, qsqt;
        XVec4<T,0x3032> const wxwz, arab, qsqp;
        XVec4<T,0x3033> const wxww, araa, qsqq;
        XVec4<T,0x3100> const wyxx, agrr, qtss;
        XVec4<T,0x3101> const wyxy, agrg, qtst;
        XVec4<T,0x3102> const wyxz, agrb, qtsp; /* lvalue */
        XVec4<T,0x3103> const wyxw, agra, qtsq;
        XVec4<T,0x3110> const wyyx, aggr, qtts;
        XVec4<T,0x3111> const wyyy, aggg, qttt;
        XVec4<T,0x3112> const wyyz, aggb, qttp;
        XVec4<T,0x3113> const wyyw, agga, qttq;
        XVec4<T,0x3120> const wyzx, agbr, qtps; /* lvalue */
        XVec4<T,0x3121> const wyzy, agbg, qtpt;
        XVec4<T,0x3122> const wyzz, agbb, qtpp;
        XVec4<T,0x3123> const wyzw, agba, qtpq;
        XVec4<T,0x3130> const wywx, agar, qtqs;
        XVec4<T,0x3131> const wywy, agag, qtqt;
        XVec4<T,0x3132> const wywz, agab, qtqp;
        XVec4<T,0x3133> const wyww, agaa, qtqq;
        XVec4<T,0x3200> const wzxx, abrr, qpss;
        XVec4<T,0x3201> const wzxy, abrg, qpst; /* lvalue */
        XVec4<T,0x3202> const wzxz, abrb, qpsp;
        XVec4<T,0x3203> const wzxw, abra, qpsq;
        XVec4<T,0x3210> const wzyx, abgr, qpts; /* lvalue */
        XVec4<T,0x3211> const wzyy, abgg, qptt;
        XVec4<T,0x3212> const wzyz, abgb, qptp;
        XVec4<T,0x3213> const wzyw, abga, qptq;
        XVec4<T,0x3220> const wzzx, abbr, qpps;
        XVec4<T,0x3221> const wzzy, abbg, qppt;
        XVec4<T,0x3222> const wzzz, abbb, qppp;
        XVec4<T,0x3223> const wzzw, abba, qppq;
        XVec4<T,0x3230> const wzwx, abar, qpqs;
        XVec4<T,0x3231> const wzwy, abag, qpqt;
        XVec4<T,0x3232> const wzwz, abab, qpqp;
        XVec4<T,0x3233> const wzww, abaa, qpqq;
        XVec4<T,0x3300> const wwxx, aarr, qqss;
        XVec4<T,0x3301> const wwxy, aarg, qqst;
        XVec4<T,0x3302> const wwxz, aarb, qqsp;
        XVec4<T,0x3303> const wwxw, aara, qqsq;
        XVec4<T,0x3310> const wwyx, aagr, qqts;
        XVec4<T,0x3311> const wwyy, aagg, qqtt;
        XVec4<T,0x3312> const wwyz, aagb, qqtp;
        XVec4<T,0x3313> const wwyw, aaga, qqtq;
        XVec4<T,0x3320> const wwzx, aabr, qqps;
        XVec4<T,0x3321> const wwzy, aabg, qqpt;
        XVec4<T,0x3322> const wwzz, aabb, qqpp;
        XVec4<T,0x3323> const wwzw, aaba, qqpq;
        XVec4<T,0x3330> const wwwx, aaar, qqqs;
        XVec4<T,0x3331> const wwwy, aaag, qqqt;
        XVec4<T,0x3332> const wwwz, aaab, qqqp;
        XVec4<T,0x3333> const wwww, aaaa, qqqq;
#if LOL_NO_CONST_MEMBERS_IN_ANONYMOUS_UNIONS
#   undef const
#endif
    };
};

template <> struct BVec4<half>
{
    explicit inline BVec4() {}
    explicit inline BVec4(half X, half Y, half Z, half W)
     : x(X), y(Y), z(Z), w(W) {}

    half x, y, z, w;
};

template <> struct BVec4<real>
{
    explicit inline BVec4() {}
    explicit inline BVec4(real X, real Y, real Z, real W)
     : x(X), y(Y), z(Z), w(W) {}

    real x, y, z, w;
};

template <typename T> struct Vec4 : BVec4<T>
{
    inline Vec4() {}
    inline Vec4(T X, T Y, T Z, T W) : BVec4<T>(X, Y, Z, W) {}
    inline Vec4(Vec2<T> XY, T Z, T W) : BVec4<T>(XY.x, XY.y, Z, W) {}
    inline Vec4(T X, Vec2<T> YZ, T W) : BVec4<T>(X, YZ.x, YZ.y, W) {}
    inline Vec4(T X, T Y, Vec2<T> ZW) : BVec4<T>(X, Y, ZW.x, ZW.y) {}
    inline Vec4(Vec2<T> XY, Vec2<T> ZW) : BVec4<T>(XY.x, XY.y, ZW.x, ZW.y) {}
    inline Vec4(Vec3<T> XYZ, T W) : BVec4<T>(XYZ.x, XYZ.y, XYZ.z, W) {}
    inline Vec4(T X, Vec3<T> YZW) : BVec4<T>(X, YZW.x, YZW.y, YZW.z) {}

    explicit inline Vec4(T X) : BVec4<T>(X, X, X, X) {}

    template<int N>
    inline Vec4(XVec4<T, N> const &v)
      : BVec4<T>(v[0], v[1], v[2], v[3]) {}

    template<typename U, int N>
    explicit inline Vec4(XVec4<U, N> const &v)
      : BVec4<T>(v[0], v[1], v[2], v[3]) {}

    DECLARE_MEMBER_OPS(Vec4)

#if !defined __ANDROID__
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec4<U> const &v);
#endif
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

#define DECLARE_VECTOR_VECTOR_COERCE_OP(tname, op, tprefix, t1, t2, tf) \
    tprefix \
    inline tname<tf> operator op(tname<t1> const &a, tname<t2> const &b) \
    { \
        tname<tf> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(t1); n++) \
            ret[n] = a[n] op b[n]; \
        return ret; \
    }

#define DECLARE_VECTOR_VECTOR_OP(tname, op, tprefix, type) \
    tprefix \
    inline tname<type> operator op##=(tname<type> &a, tname<type> const &b) \
    { \
        return a = a op b; \
    }

#define DECLARE_VECTOR_VECTOR_BOOLOP(tname, op, op2, ret, tprefix, t1, t2) \
    tprefix \
    inline bool operator op(tname<t1> const &a, tname<t2> const &b) \
    { \
        for (size_t n = 0; n < sizeof(a) / sizeof(t1); n++) \
            if (!(a[n] op2 b[n])) \
                return !ret; \
        return ret; \
    }

#define DECLARE_VECTOR_SCALAR_COERCE_OP(tname, op, tprefix, t1, t2, tf) \
    tprefix \
    inline tname<tf> operator op(tname<t1> const &a, t2 const &val) \
    { \
        tname<tf> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(t1); n++) \
            ret[n] = a[n] op val; \
        return ret; \
    } \
    \
    tprefix \
    inline tname<tf> operator op(t1 const &val, tname<t2> const &a) \
    { \
        tname<tf> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(t2); n++) \
            ret[n] = a[n] op val; \
        return ret; \
    }

#define DECLARE_VECTOR_SCALAR_OP(tname, op, tprefix, type) \
    tprefix \
    inline tname<type> operator op##=(tname<type> &a, type const &val) \
    { \
        return a = a op val; \
    }

#define DECLARE_UNARY_OPS(tname, tprefix, type) \
    tprefix \
    inline tname<type> operator -(tname<type> const &a) \
    { \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = -a[n]; \
        return ret; \
    } \
    \
    tprefix \
    inline type sqlen(tname<type> const &a) \
    { \
        type acc = 0; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            acc += a[n] * a[n]; \
        return acc; \
    } \
    \
    tprefix \
    inline double len(tname<type> const &a) \
    { \
        using namespace std; \
        return sqrt((double)sqlen(a)); \
    } \
    \
    tprefix \
    inline tname<type> normalize(tname<type> const &val) \
    { \
        type norm = (type)len(val); \
        return norm ? val / norm : val * (type)0; \
    }

#define DECLARE_BINARY_COERCE_OPS(tname, tprefix, t1, t2, tf) \
    DECLARE_VECTOR_SCALAR_COERCE_OP(tname, *, tprefix, t1, t2, tf) \
    DECLARE_VECTOR_SCALAR_COERCE_OP(tname, /, tprefix, t1, t2, tf) \
    \
    DECLARE_VECTOR_VECTOR_COERCE_OP(tname, -, tprefix, t1, t2, tf) \
    DECLARE_VECTOR_VECTOR_COERCE_OP(tname, +, tprefix, t1, t2, tf) \
    \
    DECLARE_VECTOR_VECTOR_BOOLOP(tname, ==, ==, true, tprefix, t1, t2) \
    DECLARE_VECTOR_VECTOR_BOOLOP(tname, !=, ==, false, tprefix, t1, t2) \
    \
    tprefix \
    inline tf dot(tname<t1> const &a, tname<t2> const &b) \
    { \
        tf ret = 0; \
        for (size_t n = 0; n < sizeof(a) / sizeof(t1); n++) \
            ret += a[n] * b[n]; \
        return ret; \
    }

#define DECLARE_BINARY_OPS(tname, tprefix, type) \
    DECLARE_BINARY_COERCE_OPS(tname, tprefix, type, type, type) \
    \
    DECLARE_VECTOR_SCALAR_OP(tname, *, tprefix, type) \
    DECLARE_VECTOR_SCALAR_OP(tname, /, tprefix, type) \
    \
    DECLARE_VECTOR_VECTOR_OP(tname, -, tprefix, type) \
    DECLARE_VECTOR_VECTOR_OP(tname, +, tprefix, type)

#define DECLARE_VECTOR_COERCE_OPS(tname, tprefix, t1, t2, tf) \
    DECLARE_VECTOR_VECTOR_COERCE_OP(tname, *, tprefix, t1, t2, tf) \
    DECLARE_VECTOR_VECTOR_COERCE_OP(tname, /, tprefix, t1, t2, tf) \
    \
    DECLARE_VECTOR_VECTOR_BOOLOP(tname, <=, <=, true, tprefix, t1, t2) \
    DECLARE_VECTOR_VECTOR_BOOLOP(tname, >=, >=, true, tprefix, t1, t2) \
    DECLARE_VECTOR_VECTOR_BOOLOP(tname, <, <, true, tprefix, t1, t2) \
    DECLARE_VECTOR_VECTOR_BOOLOP(tname, >, >, true, tprefix, t1, t2)

#define DECLARE_VECTOR_OPS(tname, tprefix, type) \
    DECLARE_VECTOR_COERCE_OPS(tname, static, type, type, type) \
    \
    DECLARE_VECTOR_VECTOR_OP(tname, *, tprefix, type) \
    DECLARE_VECTOR_VECTOR_OP(tname, /, tprefix, type)

#define DECLARE_ALL_NONVECTOR_OPS(tname) \
    DECLARE_BINARY_OPS(tname, template<typename T> static, T) \
    DECLARE_UNARY_OPS(tname, template<typename T> static, T)

#define DECLARE_ALL_VECTOR_OPS_INNER(tname, type) \
    DECLARE_BINARY_OPS(tname, static, type) \
    DECLARE_UNARY_OPS(tname, static, type) \
    DECLARE_VECTOR_OPS(tname, static, type) \

#define DECLARE_ALL_VECTOR_OPS(tname) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, half) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, float) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, double) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, int8_t) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, uint8_t) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, int16_t) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, uint16_t) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, int32_t) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, uint32_t) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, int64_t) \
    DECLARE_ALL_VECTOR_OPS_INNER(tname, uint64_t)

#define DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, tlow, thigh) \
    DECLARE_BINARY_COERCE_OPS(tname, static, tlow, thigh, thigh) \
    DECLARE_BINARY_COERCE_OPS(tname, static, thigh, tlow, thigh) \
    \
    DECLARE_VECTOR_COERCE_OPS(tname, static, tlow, thigh, thigh) \
    DECLARE_VECTOR_COERCE_OPS(tname, static, thigh, tlow, thigh)

#define DECLARE_ALL_VECTOR_COERCE_OPS(tname) \
    /* Apply the same coercion rules as in the C++ standard. However,   */ \
    /* instead of promoting int8_t etc. to int, we apply our own rules. */ \
    /* FIXME: "half" and "real" are deactivated for now, because we do  */ \
    /* not implement all combinations of operators for these types yet. */ \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, uint8_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, int16_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, uint16_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, int32_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, uint32_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, int64_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, uint64_t) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, half) */ \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, float) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, double) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int8_t, real) */ \
    \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, int16_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, uint16_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, int32_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, uint32_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, int64_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, uint64_t) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, half) */ \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, float) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, double) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint8_t, real) */ \
    \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, uint16_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, int32_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, uint32_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, int64_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, uint64_t) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, half) */ \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, float) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, double) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int16_t, real) */ \
    \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint16_t, int32_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint16_t, uint32_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint16_t, int64_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint16_t, uint64_t) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint16_t, half) */ \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint16_t, float) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint16_t, double) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint16_t, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint16_t, real) */ \
    \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int32_t, uint32_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int32_t, int64_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int32_t, uint64_t) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int32_t, half) */ \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int32_t, float) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int32_t, double) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int32_t, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int32_t, real) */ \
    \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint32_t, int64_t) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint32_t, uint64_t) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint32_t, half) */ \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint32_t, float) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint32_t, double) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint32_t, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint32_t, real) */ \
    \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int64_t, uint64_t) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int64_t, half) */ \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int64_t, float) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int64_t, double) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int64_t, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, int64_t, real) */ \
    \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint64_t, half) */ \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint64_t, float) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint64_t, double) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint64_t, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, uint64_t, real) */ \
    \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, half, float) */ \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, half, double) */ \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, half, long double) */ \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, half, real) */ \
    \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, float, double) \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, float, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, float, real) */ \
    \
    DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, double, long double) \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, double, real) */ \
    \
    /* DECLARE_ALL_VECTOR_COERCE_OPS_INNER(tname, long double, real) */

DECLARE_ALL_NONVECTOR_OPS(Cmplx)
DECLARE_ALL_NONVECTOR_OPS(Quat)

DECLARE_ALL_VECTOR_OPS(Vec2)
DECLARE_ALL_VECTOR_OPS(Vec3)
DECLARE_ALL_VECTOR_OPS(Vec4)

/* Disable warnings in the >= > etc. operators about comparing signed and
 * unsigned. Ideally we would like to get these warnings only when the
 * inlined operators are actually used, but they seem to be triggered at
 * the code parsing step, so the compiler does not yet know whether they
 * will be used.
 * Also we do this for the whole block of declarations, because GCC prior
 * to 4.6.3 does not appear to support _Pragma() inside a macro. */
#if defined __GNUC__ && (__GNUC__ >= 4)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wsign-compare"
#elif defined _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4018)
#endif

DECLARE_ALL_VECTOR_COERCE_OPS(Vec2)
DECLARE_ALL_VECTOR_COERCE_OPS(Vec3)
DECLARE_ALL_VECTOR_COERCE_OPS(Vec4)

#if defined __GNUC__ && (__GNUC__ >= 4)
#   pragma GCC diagnostic pop
#elif defined _MSC_VER
#   pragma warning(pop)
#endif

#undef DECLARE_VECTOR_TYPEDEFS
#undef DECLARE_MEMBER_OPS
#undef DECLARE_VECTOR_VECTOR_OP
#undef DECLARE_VECTOR_VECTOR_BOOLOP
#undef DECLARE_VECTOR_SCALAR_OP
#undef DECLARE_BINARY_OPS
#undef DECLARE_UNARY_OPS
#undef DECLARE_ALL_NONVECTOR_OPS
#undef DECLARE_ALL_VECTOR_OPS_INNER
#undef DECLARE_ALL_VECTOR_OPS

/*
 * Magic vector swizzling (part 2/2)
 * Unfortunately these assignment operators cannot be used for now, because
 * we would also need to override the default copy assignment operator, and
 * in C++98 unions cannot contain such objects. This is why all the swizzling
 * magic objects are marked 'const' even those that could be lvalues.
 */

template<typename T, int N>
inline Vec2<T> XVec2<T, N>::operator =(Vec2<T> const &that)
{
    for (int i = 0; i < 2; i++)
        *this[i] = that[i];
    return *this;
}

template<typename T, int N>
inline Vec3<T> XVec3<T, N>::operator =(Vec3<T> const &that)
{
    for (int i = 0; i < 3; i++)
        *this[i] = that[i];
    return *this;
}

template<typename T, int N>
inline Vec4<T> XVec4<T, N>::operator =(Vec4<T> const &that)
{
    for (int i = 0; i < 4; i++)
        *this[i] = that[i];
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

