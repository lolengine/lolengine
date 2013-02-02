//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The vector, complex, quaternion and matrix classes
// --------------------------------------------------
//

#if !defined __LOL_MATH_VECTOR_H__
#define __LOL_MATH_VECTOR_H__

#include <stdint.h>
#include <ostream>

#include "lol/math/math.h"
#include "lol/math/half.h"
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

#define LOL_VECTOR_TYPEDEFS(tname, suffix) \
    template <typename T> struct tname; \
    typedef tname<half> f16##suffix; \
    typedef tname<float> suffix; \
    typedef tname<double> d##suffix; \
    typedef tname<ldouble> f128##suffix; \
    typedef tname<int8_t> i8##suffix; \
    typedef tname<uint8_t> u8##suffix; \
    typedef tname<int16_t> i16##suffix; \
    typedef tname<uint16_t> u16##suffix; \
    typedef tname<int32_t> i##suffix; \
    typedef tname<uint32_t> u##suffix; \
    typedef tname<int64_t> i64##suffix; \
    typedef tname<uint64_t> u64##suffix; \
    typedef tname<real> r##suffix; \

LOL_VECTOR_TYPEDEFS(Vec2, vec2)
LOL_VECTOR_TYPEDEFS(Cmplx, cmplx)
LOL_VECTOR_TYPEDEFS(Vec3, vec3)
LOL_VECTOR_TYPEDEFS(Vec4, vec4)
LOL_VECTOR_TYPEDEFS(Quat, quat)
LOL_VECTOR_TYPEDEFS(Mat2, mat2)
LOL_VECTOR_TYPEDEFS(Mat3, mat3)
LOL_VECTOR_TYPEDEFS(Mat4, mat4)

#undef LOL_VECTOR_TYPEDEFS

/*
 * HLSL/Cg-compliant type names.
 */

typedef vec2 float2;
typedef vec3 float3;
typedef vec4 float4;
typedef mat2 float2x2;
typedef mat3 float3x3;
typedef mat4 float4x4;

typedef ivec2 int2;
typedef ivec3 int3;
typedef ivec4 int4;
typedef imat2 int2x2;
typedef imat3 int3x3;
typedef imat4 int4x4;

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

    inline T& operator[](size_t n)
    {
        int i = (N >> (4 * (1 - n))) & 3;
        return static_cast<T*>(static_cast<void*>(this))[i];
    }
    inline T const& operator[](size_t n) const
    {
        int i = (N >> (4 * (1 - n))) & 3;
        return static_cast<T const*>(static_cast<void const *>(this))[i];
    }
};

template<typename T, int N> struct XVec3
{
    inline Vec3<T> operator =(Vec3<T> const &that);

    inline T& operator[](size_t n)
    {
        int i = (N >> (4 * (2 - n))) & 3;
        return static_cast<T*>(static_cast<void*>(this))[i];
    }
    inline T const& operator[](size_t n) const
    {
        int i = (N >> (4 * (2 - n))) & 3;
        return static_cast<T const*>(static_cast<void const *>(this))[i];
    }
};

template<typename T, int N> struct XVec4
{
    inline Vec4<T> operator =(Vec4<T> const &that);

    inline T& operator[](size_t n)
    {
        int i = (N >> (4 * (3 - n))) & 3;
        return static_cast<T*>(static_cast<void*>(this))[i];
    }
    inline T const& operator[](size_t n) const
    {
        int i = (N >> (4 * (3 - n))) & 3;
        return static_cast<T const*>(static_cast<void const *>(this))[i];
    }
};

/*
 * Helper macro for vector type member functions
 */

#define LOL_MEMBER_OPS(tname, first) \
    inline T& operator[](size_t n) { return *(&this->first + n); } \
    inline T const& operator[](size_t n) const { return *(&this->first + n); } \
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
#if !_DOXYGEN_SKIP_ME
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
#endif
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

    LOL_MEMBER_OPS(Vec2, x)

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec2<U> const &v);
};

/*
 * 2-element complexes
 */

template <typename T> struct Cmplx
{
    inline Cmplx() {}
    inline Cmplx(T X) : x(X), y(0) {}
    inline Cmplx(T X, T Y) : x(X), y(Y) {}

    LOL_MEMBER_OPS(Cmplx, x)

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

    inline T norm() const { return length(*this); }
    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Cmplx<U> const &v);

    T x, y;
};

template<typename T>
static inline Cmplx<T> re(Cmplx<T> const &val)
{
    return ~val / sqlength(val);
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
#if !_DOXYGEN_SKIP_ME
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
#endif
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

    static Vec3<T> toeuler(Quat<T> const &q);

    LOL_MEMBER_OPS(Vec3, x)

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec3<U> const &v);
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
#if !_DOXYGEN_SKIP_ME
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
#endif
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

    LOL_MEMBER_OPS(Vec4, x)

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Vec4<U> const &v);
};

/*
 * 4-element quaternions
 */

template <typename T> struct Quat
{
    inline Quat() {}
    inline Quat(T W) : w(W),  x(0), y(0), z(0) {}
    inline Quat(T W, T X, T Y, T Z) : w(W), x(X), y(Y), z(Z) {}

    Quat(Mat3<T> const &m);
    Quat(Mat4<T> const &m);

    LOL_MEMBER_OPS(Quat, w)

    static Quat<T> rotate(T angle, T x, T y, T z);
    static Quat<T> rotate(T angle, Vec3<T> const &v);

    /* Convert from Euler angles. The axes in fromeuler_xyx are
     * x, then y', then x", ie. the axes are attached to the model.
     * If you want to rotate around static axes, just reverse the order
     * of the arguments. */
    static Quat<T> fromeuler_xyx(Vec3<T> const &v);
    static Quat<T> fromeuler_xzx(Vec3<T> const &v);
    static Quat<T> fromeuler_yxy(Vec3<T> const &v);
    static Quat<T> fromeuler_yzy(Vec3<T> const &v);
    static Quat<T> fromeuler_zxz(Vec3<T> const &v);
    static Quat<T> fromeuler_zyz(Vec3<T> const &v);
    static Quat<T> fromeuler_xyx(T phi, T theta, T psi);
    static Quat<T> fromeuler_xzx(T phi, T theta, T psi);
    static Quat<T> fromeuler_yxy(T phi, T theta, T psi);
    static Quat<T> fromeuler_yzy(T phi, T theta, T psi);
    static Quat<T> fromeuler_zxz(T phi, T theta, T psi);
    static Quat<T> fromeuler_zyz(T phi, T theta, T psi);

    /* Convert from Tait-Bryan angles (incorrectly called Euler angles,
     * but since everyone does it…). The axes in fromeuler_xyz are
     * x, then y', then z", ie. the axes are attached to the model.
     * If you want to apply yaw around x, pitch around y, and roll
     * around z, use fromeuler_xyz.
     * If you want to rotate around static axes, reverse the order in
     * the function name (_zyx instead of _xyz) AND reverse the order
     * of the arguments. */
    static Quat<T> fromeuler_xyz(Vec3<T> const &v);
    static Quat<T> fromeuler_xzy(Vec3<T> const &v);
    static Quat<T> fromeuler_yxz(Vec3<T> const &v);
    static Quat<T> fromeuler_yzx(Vec3<T> const &v);
    static Quat<T> fromeuler_zxy(Vec3<T> const &v);
    static Quat<T> fromeuler_zyx(Vec3<T> const &v);
    static Quat<T> fromeuler_xyz(T phi, T theta, T psi);
    static Quat<T> fromeuler_xzy(T phi, T theta, T psi);
    static Quat<T> fromeuler_yxz(T phi, T theta, T psi);
    static Quat<T> fromeuler_yzx(T phi, T theta, T psi);
    static Quat<T> fromeuler_zxy(T phi, T theta, T psi);
    static Quat<T> fromeuler_zyx(T phi, T theta, T psi);

    inline Quat<T> operator *(Quat<T> const &val) const;

    inline Quat<T> operator *=(Quat<T> const &val)
    {
        return *this = (*this) * val;
    }

    inline Quat<T> operator ~() const
    {
        return Quat<T>(w, -x, -y, -z);
    }

    inline Vec3<T> transform(Vec3<T> const &v)
    {
        Quat<T> p = Quat<T>(0, v.x, v.y, v.z);
        Quat<T> q = *this * p / *this;
        return Vec3<T>(q.x, q.y, q.z);
    }

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, Quat<U> const &v);

    /* XXX: storage order is wxyz, unlike vectors! */
    T w, x, y, z;
};

template<typename T>
inline T norm(Quat<T> const &val)
{
    return sqlength(val);
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
static inline Quat<T> operator /(Quat<T> const &x, Quat<T> const &y)
{
    return x * re(y);
}

template<typename T>
extern Quat<T> slerp(Quat<T> const &qa, Quat<T> const &qb, T f);

/*
 * Common operators for all vector types, including quaternions
 */

/*
 * vec +(vec, vec)   (also complex & quaternion)
 * vec -(vec, vec)   (also complex & quaternion)
 * vec *(vec, vec)
 * vec /(vec, vec)
 */
#define LOL_VECTOR_VECTOR_OP(tname, op, tprefix, type) \
    tprefix \
    inline tname<type> operator op(tname<type> const &a, tname<type> const &b) \
    { \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = a[n] op b[n]; \
        return ret; \
    }

/*
 * vec +=(vec, vec)   (also complex & quaternion)
 * vec -=(vec, vec)   (also complex & quaternion)
 * vec *=(vec, vec)
 * vec /=(vec, vec)
 */
#define LOL_VECTOR_VECTOR_NONCONST_OP(tname, op, tprefix, type) \
    tprefix \
    inline tname<type> operator op##=(tname<type> &a, tname<type> const &b) \
    { \
        return a = a op b; \
    }

/*
 * vec min(vec, vec)     (also max, fmod)
 * vec min(vec, scalar)  (also max, fmod)
 * vec min(scalar, vec)  (also max, fmod)
 */
#define LOL_VECTOR_MINMAX_FUN(tname, op, tprefix, type) \
    tprefix \
    inline tname<type> op(tname<type> const &a, tname<type> const &b) \
    { \
        using lol::op; \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = op(a[n], b[n]); \
        return ret; \
    } \
    \
    tprefix \
    inline tname<type> op(tname<type> const &a, type const &b) \
    { \
        using lol::op; \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = op(a[n], b); \
        return ret; \
    } \
    \
    tprefix \
    inline tname<type> op(type const &a, tname<type> const &b) \
    { \
        using lol::op; \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(b) / sizeof(type); n++) \
            ret[n] = op(a, b[n]); \
        return ret; \
    }

/*
 * vec clamp(vec, vec, vec)
 * vec clamp(vec, vec, scalar)
 * vec clamp(vec, scalar, vec)
 * vec clamp(vec, scalar, scalar)
 */
#define LOL_VECTOR_CLAMP_FUN(tname, tprefix, type) \
    tprefix \
    inline tname<type> clamp(tname<type> const &x, \
                             tname<type> const &a, tname<type> const &b) \
    { \
        return max(min(x, b), a); \
    } \
    \
    tprefix \
    inline tname<type> clamp(tname<type> const &x, \
                             type const &a, tname<type> const &b) \
    { \
        return max(min(x, b), a); \
    } \
    \
    tprefix \
    inline tname<type> clamp(tname<type> const &x, \
                             tname<type> const &a, type const &b) \
    { \
        return max(min(x, b), a); \
    } \
    \
    tprefix \
    inline tname<type> clamp(tname<type> const &x, \
                             type const &a, type const &b) \
    { \
        return max(min(x, b), a); \
    }

/*
 * vec mix(vec, vec, vec)
 * vec mix(vec, vec, scalar)
 */
#define LOL_VECTOR_MIX_FUN(tname, tprefix, type) \
    tprefix \
    inline tname<type> mix(tname<type> const &x, \
                           tname<type> const &y, tname<type> const &a) \
    { \
        return x + a * (y - x); \
    } \
    \
    tprefix \
    inline tname<type> mix(tname<type> const &x, \
                           tname<type> const &y, type const &a) \
    { \
        return x + a * (y - x); \
    }

/*
 * bool ==(vec, vec)   (also complex & quaternion)
 * bool !=(vec, vec)   (also complex & quaternion)
 * bool >=(vec, vec)
 * bool <=(vec, vec)
 * bool >(vec, vec)
 * bool <(vec, vec)
 */
#define LOL_VECTOR_VECTOR_BOOL_OP(tname, op, op2, ret, tprefix, type) \
    tprefix \
    inline bool operator op(tname<type> const &a, tname<type> const &b) \
    { \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            if (!(a[n] op2 b[n])) \
                return !ret; \
        return ret; \
    }

/*
 * vec *(vec, scalar)   (also complex & quaternion)
 * vec /(vec, scalar)   (also complex & quaternion)
 */
#define LOL_VECTOR_SCALAR_OP(tname, op, tprefix, type) \
    tprefix \
    inline tname<type> operator op(tname<type> const &a, type const &val) \
    { \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = a[n] op val; \
        return ret; \
    }

/*
 * vec *(scalar, vec)   (also complex & quaternion)
 * vec /(scalar, vec)   (NOT for complex & quaternion!)
 */
#define LOL_SCALAR_VECTOR_OP(tname, op, tprefix, type) \
    tprefix \
    inline tname<type> operator op(type const &val, tname<type> const &a) \
    { \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = a[n] op val; \
        return ret; \
    }

/*
 * vec *=(vec, scalar)   (also complex & quaternion)
 * vec /=(vec, scalar)   (also complex & quaternion)
 */
#define LOL_VECTOR_SCALAR_NONCONST_OP(tname, op, tprefix, type) \
    tprefix \
    inline tname<type> operator op##=(tname<type> &a, type const &val) \
    { \
        return a = a op val; \
    }

#define LOL_UNARY_OPS(tname, tprefix, type) \
    tprefix \
    inline tname<type> operator -(tname<type> const &a) \
    { \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = -a[n]; \
        return ret; \
    }

#define LOL_UNARY_FUNS(tname, tprefix, type) \
    tprefix \
    inline type sqlength(tname<type> const &a) \
    { \
        type acc = 0; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            acc += a[n] * a[n]; \
        return acc; \
    } \
    \
    tprefix \
    inline type length(tname<type> const &a) \
    { \
        return (type)sqrt((double)sqlength(a)); \
    } \
    \
    tprefix \
    inline tname<type> fract(tname<type> const &a) \
    { \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = fract(a[n]); \
        return ret; \
    } \
    \
    tprefix \
    inline tname<type> normalize(tname<type> const &a) \
    { \
        type norm = (type)length(a); \
        return norm ? a / norm : a * (type)0; \
    } \
    \
    tprefix \
    inline tname<type> abs(tname<type> const &a) \
    { \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = lol::abs(a[n]); \
        return ret; \
    }

#define LOL_BINARY_NONVECTOR_FUNS(tname, tprefix, type) \
    tprefix \
    inline type dot(tname<type> const &a, tname<type> const &b) \
    { \
        type ret = 0; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret += a[n] * b[n]; \
        return ret; \
    } \
    \
    tprefix \
    inline tname<type> lerp(tname<type> const &a, \
                            tname<type> const &b, type x) \
    { \
        tname<type> ret; \
        for (size_t n = 0; n < sizeof(a) / sizeof(type); n++) \
            ret[n] = a[n] + (b[n] - a[n]) * x; \
        return ret; \
    }

#define LOL_VEC_3_FUNS(tname, tprefix, type) \
    tprefix \
    inline tname<type> cross(tname<type> const &a, tname<type> const &b) \
    { \
        return tname<type>((type)(a.y * b.z - a.z * b.y), \
                           (type)(a.z * b.x - a.x * b.z), \
                           (type)(a.x * b.y - a.y * b.x)); \
    }

#define LOL_BINARY_NONVECTOR_OPS(tname, tprefix, type) \
    LOL_VECTOR_VECTOR_OP(tname, -, tprefix, type) \
    LOL_VECTOR_VECTOR_OP(tname, +, tprefix, type) \
    LOL_VECTOR_SCALAR_OP(tname, *, tprefix, type) \
    LOL_VECTOR_SCALAR_OP(tname, /, tprefix, type) \
    LOL_SCALAR_VECTOR_OP(tname, *, tprefix, type) \
    \
    LOL_VECTOR_VECTOR_NONCONST_OP(tname, -, tprefix, type) \
    LOL_VECTOR_VECTOR_NONCONST_OP(tname, +, tprefix, type) \
    LOL_VECTOR_SCALAR_NONCONST_OP(tname, *, tprefix, type) \
    LOL_VECTOR_SCALAR_NONCONST_OP(tname, /, tprefix, type) \
    \
    LOL_VECTOR_VECTOR_BOOL_OP(tname, ==, ==, true, tprefix, type) \
    LOL_VECTOR_VECTOR_BOOL_OP(tname, !=, ==, false, tprefix, type)

#define LOL_BINARY_VECTOR_OPS(tname, tprefix, type) \
    LOL_SCALAR_VECTOR_OP(tname, /, tprefix, type)

#define LOL_BINARY_VECTOR_FUNS(tname, tprefix, type) \
    LOL_VECTOR_MINMAX_FUN(tname, min, tprefix, type) \
    LOL_VECTOR_MINMAX_FUN(tname, max, tprefix, type) \
    LOL_VECTOR_MINMAX_FUN(tname, fmod, tprefix, type) \
    LOL_VECTOR_CLAMP_FUN(tname, tprefix, type) \
    LOL_VECTOR_MIX_FUN(tname, tprefix, type) \
    \
    LOL_VECTOR_VECTOR_BOOL_OP(tname, <=, <=, true, tprefix, type) \
    LOL_VECTOR_VECTOR_BOOL_OP(tname, >=, >=, true, tprefix, type) \
    LOL_VECTOR_VECTOR_BOOL_OP(tname, <, <, true, tprefix, type) \
    LOL_VECTOR_VECTOR_BOOL_OP(tname, >, >, true, tprefix, type)

#define LOL_VECTOR_OPS(tname, tprefix, type) \
    LOL_VECTOR_VECTOR_OP(tname, *, tprefix, type) \
    LOL_VECTOR_VECTOR_OP(tname, /, tprefix, type) \
    \
    LOL_VECTOR_VECTOR_NONCONST_OP(tname, *, tprefix, type) \
    LOL_VECTOR_VECTOR_NONCONST_OP(tname, /, tprefix, type)

#define LOL_ALL_NONVECTOR_OPS_AND_FUNS(tname) \
    LOL_BINARY_NONVECTOR_OPS(tname, template<typename T> static, T) \
    LOL_BINARY_NONVECTOR_FUNS(tname, template<typename T> static, T) \
    LOL_UNARY_OPS(tname, template<typename T> static, T) \
    LOL_UNARY_FUNS(tname, template<typename T> static, T)

#define LOL_ALL_VECTOR_OPS_INNER(tname, type) \
    LOL_BINARY_VECTOR_OPS(tname, static, type) \
    LOL_BINARY_NONVECTOR_OPS(tname, static, type) \
    LOL_UNARY_OPS(tname, static, type) \
    LOL_VECTOR_OPS(tname, static, type)

#define LOL_ALL_VECTOR_FUNS_INNER(tname, type) \
    LOL_BINARY_VECTOR_FUNS(tname, static, type) \
    LOL_BINARY_NONVECTOR_FUNS(tname, static, type) \
    LOL_UNARY_FUNS(tname, static, type)

#define LOL_ALL_VECTOR_OPS_AND_FUNS(type) \
    namespace x##type \
    { \
        LOL_ALL_VECTOR_OPS_INNER(Vec2, type) \
        LOL_ALL_VECTOR_OPS_INNER(Vec3, type) \
        LOL_ALL_VECTOR_OPS_INNER(Vec4, type) \
    } \
    using namespace x##type; \
    LOL_ALL_VECTOR_FUNS_INNER(Vec2, type) \
    LOL_ALL_VECTOR_FUNS_INNER(Vec3, type) \
    LOL_ALL_VECTOR_FUNS_INNER(Vec4, type) \
    LOL_VEC_3_FUNS(Vec3, static, type)

LOL_ALL_NONVECTOR_OPS_AND_FUNS(Cmplx)
LOL_ALL_NONVECTOR_OPS_AND_FUNS(Quat)

/* Disable warning about unary operator applied to unsigned type */
#if defined _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4146)
#endif

LOL_ALL_VECTOR_OPS_AND_FUNS(half)
LOL_ALL_VECTOR_OPS_AND_FUNS(float)
LOL_ALL_VECTOR_OPS_AND_FUNS(double)
LOL_ALL_VECTOR_OPS_AND_FUNS(ldouble)
LOL_ALL_VECTOR_OPS_AND_FUNS(int8_t)
LOL_ALL_VECTOR_OPS_AND_FUNS(uint8_t)
LOL_ALL_VECTOR_OPS_AND_FUNS(int16_t)
LOL_ALL_VECTOR_OPS_AND_FUNS(uint16_t)
LOL_ALL_VECTOR_OPS_AND_FUNS(int32_t)
LOL_ALL_VECTOR_OPS_AND_FUNS(uint32_t)
LOL_ALL_VECTOR_OPS_AND_FUNS(int64_t)
LOL_ALL_VECTOR_OPS_AND_FUNS(uint64_t)

/* FIXME: vectors of "real" are deactivated for now, because we do
 * not implement all combinations of operators for these types yet. */
LOL_ALL_VECTOR_OPS_AND_FUNS(real)

#if defined _MSC_VER
#   pragma warning(pop)
#endif

#undef LOL_MEMBER_OPS

#undef LOL_VECTOR_VECTOR_OP
#undef LOL_VECTOR_VECTOR_NONCONST_OP
#undef LOL_VECTOR_MINMAX_FUN
#undef LOL_VECTOR_CLAMP_FUN
#undef LOL_VECTOR_MIX_FUN
#undef LOL_VECTOR_VECTOR_BOOL_OP
#undef LOL_VECTOR_SCALAR_OP
#undef LOL_SCALAR_VECTOR_OP
#undef LOL_VECTOR_SCALAR_OP

#undef LOL_BINARY_VECTOR_OPS
#undef LOL_BINARY_VECTOR_FUNS
#undef LOL_BINARY_NONVECTOR_OPS
#undef LOL_BINARY_NONVECTOR_FUNS
#undef LOL_UNARY_OPS
#undef LOL_UNARY_FUNS
#undef LOL_VEC_3_FUNS
#undef LOL_VECTOR_OPS

#undef LOL_ALL_NONVECTOR_OPS_AND_FUNS
#undef LOL_ALL_VECTOR_OPS_INNER
#undef LOL_ALL_VECTOR_FUNS_INNER
#undef LOL_ALL_VECTOR_OPS_AND_FUNS

/*
 * Definition of additional functions requiring vector functions
 */

template<typename T>
inline Quat<T> Quat<T>::operator *(Quat<T> const &val) const
{
    Quat<T> ret;
    Vec3<T> v1(x, y, z);
    Vec3<T> v2(val.x, val.y, val.z);
    Vec3<T> v3 = cross(v1, v2) + w * v2 + val.w * v1;
    return Quat<T>(w * val.w - dot(v1, v2), v3.x, v3.y, v3.z);
}

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
 * 2×2-element matrices
 */

template <typename T> struct Mat2
{
    inline Mat2() {}
    inline Mat2(Vec2<T> V0, Vec2<T> V1)
      : v0(V0), v1(V1) {}

    explicit inline Mat2(T val)
      : v0(val, (T)0),
        v1((T)0, val) {}

    explicit inline Mat2(Mat4<T> const &mat)
      : v0(mat[0].xy),
        v1(mat[1].xy) {}

    inline Vec2<T>& operator[](size_t n) { return (&v0)[n]; }
    inline Vec2<T> const& operator[](size_t n) const { return (&v0)[n]; }

    /* Helpers for transformation matrices */
    static Mat2<T> rotate(T angle);

    static inline Mat2<T> rotate(Mat2<T> mat, T angle)
    {
        return rotate(angle) * mat;
    }

    void printf() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream, Mat2<U> const &m);

    inline Mat2<T> operator +(Mat2<T> const m) const
    {
        return Mat2<T>(v0 + m[0], v1 + m[1]);
    }

    inline Mat2<T> operator +=(Mat2<T> const m)
    {
        return *this = *this + m;
    }

    inline Mat2<T> operator -(Mat2<T> const m) const
    {
        return Mat2<T>(v0 - m[0], v1 - m[1]);
    }

    inline Mat2<T> operator -=(Mat2<T> const m)
    {
        return *this = *this - m;
    }

    inline Mat2<T> operator *(Mat2<T> const m) const
    {
        return Mat2<T>(*this * m[0], *this * m[1]);
    }

    inline Mat2<T> operator *=(Mat2<T> const m)
    {
        return *this = *this * m;
    }

    inline Vec2<T> operator *(Vec2<T> const m) const
    {
        Vec2<T> ret;
        for (int j = 0; j < 2; j++)
        {
            T tmp = 0;
            for (int k = 0; k < 2; k++)
                tmp += (*this)[k][j] * m[k];
            ret[j] = tmp;
        }
        return ret;
    }

    Vec2<T> v0, v1;
};

/*
 * 3×3-element matrices
 */

template <typename T> struct Mat3
{
    inline Mat3() {}
    inline Mat3(Vec3<T> V0, Vec3<T> V1, Vec3<T> V2)
      : v0(V0), v1(V1), v2(V2) {}

    explicit inline Mat3(T val)
      : v0(val, (T)0, (T)0),
        v1((T)0, val, (T)0),
        v2((T)0, (T)0, val) {}

    explicit inline Mat3(Mat2<T> mat)
      : v0(mat[0], (T)0),
        v1(mat[1], (T)0),
        v2((T)0, (T)0, (T)0) {}

    explicit inline Mat3(Mat2<T> mat, T val)
      : v0(Vec3<T>(mat[0], (T)0)),
        v1(Vec3<T>(mat[1], (T)0)),
        v2((T)0, (T)0, val) {}

    explicit inline Mat3(Mat4<T> const &mat)
      : v0(mat[0].xyz),
        v1(mat[1].xyz),
        v2(mat[2].xyz) {}

    explicit Mat3(Quat<T> const &q);

    inline Vec3<T>& operator[](size_t n) { return (&v0)[n]; }
    inline Vec3<T> const& operator[](size_t n) const { return (&v0)[n]; }

    /* Helpers for transformation matrices */
    static Mat3<T> scale(T x);
    static Mat3<T> scale(T x, T y, T z);
    static Mat3<T> scale(Vec3<T> v);
    static Mat3<T> rotate(T angle, T x, T y, T z);
    static Mat3<T> rotate(T angle, Vec3<T> v);

    static Mat3<T> fromeuler_xyz(Vec3<T> const &v);
    static Mat3<T> fromeuler_xzy(Vec3<T> const &v);
    static Mat3<T> fromeuler_yxz(Vec3<T> const &v);
    static Mat3<T> fromeuler_yzx(Vec3<T> const &v);
    static Mat3<T> fromeuler_zxy(Vec3<T> const &v);
    static Mat3<T> fromeuler_zyx(Vec3<T> const &v);
    static Mat3<T> fromeuler_xyz(T phi, T theta, T psi);
    static Mat3<T> fromeuler_xzy(T phi, T theta, T psi);
    static Mat3<T> fromeuler_yxz(T phi, T theta, T psi);
    static Mat3<T> fromeuler_yzx(T phi, T theta, T psi);
    static Mat3<T> fromeuler_zxy(T phi, T theta, T psi);
    static Mat3<T> fromeuler_zyx(T phi, T theta, T psi);

    static Mat3<T> fromeuler_xyx(Vec3<T> const &v);
    static Mat3<T> fromeuler_xzx(Vec3<T> const &v);
    static Mat3<T> fromeuler_yxy(Vec3<T> const &v);
    static Mat3<T> fromeuler_yzy(Vec3<T> const &v);
    static Mat3<T> fromeuler_zxz(Vec3<T> const &v);
    static Mat3<T> fromeuler_zyz(Vec3<T> const &v);
    static Mat3<T> fromeuler_xyx(T phi, T theta, T psi);
    static Mat3<T> fromeuler_xzx(T phi, T theta, T psi);
    static Mat3<T> fromeuler_yxy(T phi, T theta, T psi);
    static Mat3<T> fromeuler_yzy(T phi, T theta, T psi);
    static Mat3<T> fromeuler_zxz(T phi, T theta, T psi);
    static Mat3<T> fromeuler_zyz(T phi, T theta, T psi);

    static inline Mat3<T> rotate(Mat3<T> mat, T angle, Vec3<T> v)
    {
        return rotate(angle, v) * mat;
    }

    void printf() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream, Mat3<U> const &m);

    inline Mat3<T> operator +(Mat3<T> const m) const
    {
        return Mat3<T>(v0 + m[0], v1 + m[1], v2 + m[2]);
    }

    inline Mat3<T> operator +=(Mat3<T> const m)
    {
        return *this = *this + m;
    }

    inline Mat3<T> operator -(Mat3<T> const m) const
    {
        return Mat3<T>(v0 - m[0], v1 - m[1], v2 - m[2]);
    }

    inline Mat3<T> operator -=(Mat3<T> const m)
    {
        return *this = *this - m;
    }

    inline Mat3<T> operator *(Mat3<T> const m) const
    {
        return Mat3<T>(*this * m[0], *this * m[1], *this * m[2]);
    }

    inline Mat3<T> operator *=(Mat3<T> const m)
    {
        return *this = *this * m;
    }

    inline Vec3<T> operator *(Vec3<T> const m) const
    {
        Vec3<T> ret;
        for (int j = 0; j < 3; j++)
        {
            T tmp = 0;
            for (int k = 0; k < 3; k++)
                tmp += (*this)[k][j] * m[k];
            ret[j] = tmp;
        }
        return ret;
    }

    Vec3<T> v0, v1, v2;
};

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

    explicit inline Mat4(Mat2<T> mat)
      : v0(mat[0], (T)0, (T)0),
        v1(mat[1], (T)0, (T)0),
        v2((T)0, (T)0, (T)0, (T)0),
        v3((T)0, (T)0, (T)0, (T)0) {}

    explicit inline Mat4(Mat2<T> mat, T val1, T val2)
      : v0(mat[0], (T)0, (T)0),
        v1(mat[1], (T)0, (T)0),
        v2((T)0, (T)0, val1, (T)0),
        v3((T)0, (T)0, (T)0, val2) {}

    explicit inline Mat4(Mat3<T> mat)
      : v0(mat[0], (T)0),
        v1(mat[1], (T)0),
        v2(mat[2], (T)0),
        v3((T)0, (T)0, (T)0, (T)0) {}

    explicit inline Mat4(Mat3<T> mat, T val)
      : v0(mat[0], (T)0),
        v1(mat[1], (T)0),
        v2(mat[2], (T)0),
        v3((T)0, (T)0, (T)0, val) {}

    explicit Mat4(Quat<T> const &q);

    inline Vec4<T>& operator[](size_t n) { return (&v0)[n]; }
    inline Vec4<T> const& operator[](size_t n) const { return (&v0)[n]; }

    /* Helpers for transformation matrices */
    static Mat4<T> translate(T x, T y, T z);
    static Mat4<T> translate(Vec3<T> v);

    static inline Mat4<T> scale(T x)
    {
        return Mat4<T>(Mat3<T>::scale(x), (T)1);
    }

    static inline Mat4<T> scale(T x, T y, T z)
    {
        return Mat4<T>(Mat3<T>::scale(x, y, z), (T)1);
    }

    static inline Mat4<T> scale(Vec3<T> v)
    {
        return Mat4<T>(Mat3<T>::scale(v), (T)1);
    }

    static inline Mat4<T> translate(Mat4<T> const &mat, Vec3<T> v)
    {
        return translate(v) * mat;
    }

    static inline Mat4<T> rotate(T angle, T x, T y, T z)
    {
        return Mat4<T>(Mat3<T>::rotate(angle, x, y, z), (T)1);
    }

    static inline Mat4<T> rotate(T angle, Vec3<T> v)
    {
        return Mat4<T>(Mat3<T>::rotate(angle, v), (T)1);
    }

    static inline Mat4<T> rotate(Mat4<T> &mat, T angle, Vec3<T> v)
    {
        return rotate(angle, v) * mat;
    }

    static Mat4<T> fromeuler_xyz(Vec3<T> const &v);
    static Mat4<T> fromeuler_xzy(Vec3<T> const &v);
    static Mat4<T> fromeuler_yxz(Vec3<T> const &v);
    static Mat4<T> fromeuler_yzx(Vec3<T> const &v);
    static Mat4<T> fromeuler_zxy(Vec3<T> const &v);
    static Mat4<T> fromeuler_zyx(Vec3<T> const &v);
    static Mat4<T> fromeuler_xyz(T phi, T theta, T psi);
    static Mat4<T> fromeuler_xzy(T phi, T theta, T psi);
    static Mat4<T> fromeuler_yxz(T phi, T theta, T psi);
    static Mat4<T> fromeuler_yzx(T phi, T theta, T psi);
    static Mat4<T> fromeuler_zxy(T phi, T theta, T psi);
    static Mat4<T> fromeuler_zyx(T phi, T theta, T psi);

    static Mat4<T> fromeuler_xyx(Vec3<T> const &v);
    static Mat4<T> fromeuler_xzx(Vec3<T> const &v);
    static Mat4<T> fromeuler_yxy(Vec3<T> const &v);
    static Mat4<T> fromeuler_yzy(Vec3<T> const &v);
    static Mat4<T> fromeuler_zxz(Vec3<T> const &v);
    static Mat4<T> fromeuler_zyz(Vec3<T> const &v);
    static Mat4<T> fromeuler_xyx(T phi, T theta, T psi);
    static Mat4<T> fromeuler_xzx(T phi, T theta, T psi);
    static Mat4<T> fromeuler_yxy(T phi, T theta, T psi);
    static Mat4<T> fromeuler_yzy(T phi, T theta, T psi);
    static Mat4<T> fromeuler_zxz(T phi, T theta, T psi);
    static Mat4<T> fromeuler_zyz(T phi, T theta, T psi);

    /* Helpers for view matrices */
    static Mat4<T> lookat(Vec3<T> eye, Vec3<T> center, Vec3<T> up);

    /* Helpers for projection matrices */
    static Mat4<T> ortho(T left, T right, T bottom, T top, T near, T far);
    static Mat4<T> ortho(T width, T height, T near, T far);
    static Mat4<T> frustum(T left, T right, T bottom, T top, T near, T far);
    static Mat4<T> perspective(T fov_y, T width, T height, T near, T far);

    void printf() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream, Mat4<U> const &m);

    inline Mat4<T> operator +(Mat4<T> const &m) const
    {
        return Mat4<T>(v0 + m[0], v1 + m[1], v2 + m[2], v3 + m[3]);
    }

    inline Mat4<T> operator +=(Mat4<T> const &m)
    {
        return *this = *this + m;
    }

    inline Mat4<T> operator -(Mat4<T> const &m) const
    {
        return Mat4<T>(v0 - m[0], v1 - m[1], v2 - m[2], v3 - m[3]);
    }

    inline Mat4<T> operator -=(Mat4<T> const &m)
    {
        return *this = *this - m;
    }

    inline Mat4<T> operator *(Mat4<T> const &m) const
    {
        return Mat4<T>(*this * m[0], *this * m[1], *this * m[2], *this * m[3]);
    }

    inline Mat4<T> operator *=(Mat4<T> const &m)
    {
        return *this = *this * m;
    }

    inline Vec4<T> operator *(Vec4<T> const &m) const
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

template<typename T> T determinant(Mat2<T> const &);
template<typename T> T determinant(Mat3<T> const &);
template<typename T> T determinant(Mat4<T> const &);

template<typename T> Mat2<T> transpose(Mat2<T> const &);
template<typename T> Mat3<T> transpose(Mat3<T> const &);
template<typename T> Mat4<T> transpose(Mat4<T> const &);

template<typename T> Mat2<T> inverse(Mat2<T> const &);
template<typename T> Mat3<T> inverse(Mat3<T> const &);
template<typename T> Mat4<T> inverse(Mat4<T> const &);

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

