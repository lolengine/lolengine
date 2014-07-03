//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The vector classes
// ------------------
//

#if !defined __LOL_MATH_VECTOR_H__
#define __LOL_MATH_VECTOR_H__

#include <ostream>

#include <lol/math/half.h>
#include <lol/math/real.h>

namespace lol
{

#if !LOL_FEATURE_CXX11_CONSTEXPR
#   define constexpr /* */
#endif

#if !LOL_FEATURE_CXX11_RELAXED_UNIONS
#   define _____ const
#else
#   define _____ /* */
#endif

/*
 * Magic vector swizzling (part 1/2)
 * These vectors are empty, but thanks to static_cast we can take their
 * address and access the vector of T's that they are union'ed with. We
 * use static_cast instead of reinterpret_cast because there is a stronger
 * guarantee (by the standard) that the address will stay the same across
 * casts.
 * We need to implement an assignment operator _and_ override the default
 * assignment operator. We try to pass arguments by value so that we don't
 * have to care about overwriting ourselves (e.g. c.rgb = c.bgr).
 * However, Visual Studio 2012 doesn't support unrestricted unions, so
 * fuck it.
 */

template<int N, typename T, int MASK>
struct vec
{
    typedef vec<N,T> type;

    inline vec<N, T, MASK>& operator =(vec<N, T> that);

#if LOL_FEATURE_CXX11_RELAXED_UNIONS
    inline vec<N, T, MASK>& operator =(vec<N, T, MASK> const &that)
    {
        /* Pass by value in case this == &that */
        return *this = (vec<N,T>)that;
    }
#endif

    inline T& operator[](size_t n)
    {
        int i = (MASK >> (4 * (N - 1 - n))) & 3;
        return static_cast<T*>(static_cast<void*>(this))[i];
    }

    inline T const& operator[](size_t n) const
    {
        int i = (MASK >> (4 * (N - 1 - n))) & 3;
        return static_cast<T const*>(static_cast<void const *>(this))[i];
    }
};

/* The generic "vec" type, which is a fixed-size vector */
template<int N, typename T>
struct vec<N, T, -1>
{
    typedef vec<N,T> type;

    inline T& operator[](size_t n) { return m_data[n]; }
    inline T const& operator[](size_t n) const { return m_data[n]; }

private:
    T m_data[N];
};

/*
 * Helper macros for vector type member functions
 */

#define LOL_V_VV_OP(op) \
    friend inline type operator op(type const &a, type const &b) \
    { \
        type ret; \
        for (size_t i = 0; i < sizeof(type) / sizeof(T); ++i) \
            ret[i] = a[i] op b[i]; \
        return ret; \
    }

#define LOL_V_VV_ASSIGN_OP(op) \
    friend inline type &operator op##=(type &a, type const &b) \
    { \
        return a = a op b; \
    }

#define LOL_V_VS_OP(op) \
    friend inline type operator op(type const &a, T const &val) \
    { \
        type ret; \
        for (size_t i = 0; i < sizeof(type) / sizeof(T); ++i) \
            ret[i] = a[i] op val; \
        return ret; \
    }

#define LOL_V_VS_ASSIGN_OP(op) \
    friend inline type operator op##=(type &a, T const &val) \
    { \
        return a = a op val; \
    }

#define LOL_B_VV_OP(op, op2, ret) \
    friend inline bool operator op(type const &a, type const &b) \
    { \
        for (size_t i = 0; i < sizeof(type) / sizeof(T); ++i) \
            if (!(a[i] op2 b[i])) \
                return !ret; \
        return ret; \
    }

#define LOL_COMMON_MEMBER_OPS(first) \
    inline T& operator[](size_t n) { return (&this->first)[n]; } \
    inline T const& operator[](size_t n) const { return (&this->first)[n]; } \
    \
    /* Visual Studio insists on having an assignment operator. */ \
    inline type & operator =(type const &that) \
    { \
        for (size_t i = 0; i < sizeof(type) / sizeof(T); ++i) \
            (*this)[i] = that[i]; \
        return *this; \
    } \
    \
    void printf() const; \
    String tostring() const; \
    \
    /* vec -(vec)
     * vec +(vec) */ \
    friend inline type operator -(type const &a) \
    { \
        type ret; \
        for (size_t i = 0; i < sizeof(type) / sizeof(T); ++i) \
            ret[i] = -a[i]; \
        return ret; \
    } \
    \
    friend inline type operator +(type const &a) \
    { \
        return a; \
    } \
    \
    /* vec +(vec, vec)
     * vec -(vec, vec)
     * vec +=(vec, vec)
     * vec -=(vec, vec) */ \
    LOL_V_VV_OP(+); \
    LOL_V_VV_OP(-); \
    LOL_V_VV_ASSIGN_OP(+); \
    LOL_V_VV_ASSIGN_OP(-); \
    \
    /* vec *(vec, scalar)
     * vec /(vec, scalar)
     * vec *=(vec, scalar)
     * vec /=(vec, scalar) */ \
    LOL_V_VS_OP(*) \
    LOL_V_VS_OP(/) \
    LOL_V_VS_ASSIGN_OP(*) \
    LOL_V_VS_ASSIGN_OP(/) \
    \
    /* bool ==(vec, vec)
     * bool !=(vec, vec) */ \
    LOL_B_VV_OP(==, ==, true) \
    LOL_B_VV_OP(!=, ==, false)

#define LOL_VECTOR_MEMBER_OPS() \
    /* vec *(vec, vec)
     * vec /(vec, vec)
     * vec *=(vec, vec)
     * vec /=(vec, vec) */ \
    LOL_V_VV_OP(*); \
    LOL_V_VV_OP(/); \
    LOL_V_VV_ASSIGN_OP(*); \
    LOL_V_VV_ASSIGN_OP(/); \
    \
    /* bool >=(vec, vec)
     * bool <=(vec, vec)
     * bool >(vec, vec)
     * bool <(vec, vec) */ \
    LOL_B_VV_OP(<=, <=, true) \
    LOL_B_VV_OP(>=, >=, true) \
    LOL_B_VV_OP(<, <, true) \
    LOL_B_VV_OP(>, >, true)

#define LOL_NONVECTOR_MEMBER_OPS() \
    /* vec *(scalar, vec) (no division, it works differently) */ \
    friend inline type operator *(T const &val, type const &a) \
    { \
        type ret; \
        for (size_t i = 0; i < sizeof(type) / sizeof(T); ++i) \
            ret[i] = val * a[i]; \
        return ret; \
    }

/*
 * 2-element vectors
 */

template <typename T> struct base_vec2
{
    explicit inline constexpr base_vec2() {}
    explicit inline constexpr base_vec2(T X, T Y) : x(X), y(Y) {}

    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T s, t; };

#if !_DOXYGEN_SKIP_ME
        vec<2,T,0x00> const xx, rr, ss;
        vec<2,T,0x01> _____ xy, rg, st;
        vec<2,T,0x10> _____ yx, gr, ts;
        vec<2,T,0x11> const yy, gg, tt;

        vec<3,T,0x000> const xxx, rrr, sss;
        vec<3,T,0x001> const xxy, rrg, sst;
        vec<3,T,0x010> const xyx, rgr, sts;
        vec<3,T,0x011> const xyy, rgg, stt;
        vec<3,T,0x100> const yxx, grr, tss;
        vec<3,T,0x101> const yxy, grg, tst;
        vec<3,T,0x110> const yyx, ggr, tts;
        vec<3,T,0x111> const yyy, ggg, ttt;

        vec<4,T,0x0000> const xxxx, rrrr, ssss;
        vec<4,T,0x0001> const xxxy, rrrg, ssst;
        vec<4,T,0x0010> const xxyx, rrgr, ssts;
        vec<4,T,0x0011> const xxyy, rrgg, sstt;
        vec<4,T,0x0100> const xyxx, rgrr, stss;
        vec<4,T,0x0101> const xyxy, rgrg, stst;
        vec<4,T,0x0110> const xyyx, rggr, stts;
        vec<4,T,0x0111> const xyyy, rggg, sttt;
        vec<4,T,0x1000> const yxxx, grrr, tsss;
        vec<4,T,0x1001> const yxxy, grrg, tsst;
        vec<4,T,0x1010> const yxyx, grgr, tsts;
        vec<4,T,0x1011> const yxyy, grgg, tstt;
        vec<4,T,0x1100> const yyxx, ggrr, ttss;
        vec<4,T,0x1101> const yyxy, ggrg, ttst;
        vec<4,T,0x1110> const yyyx, gggr, ttts;
        vec<4,T,0x1111> const yyyy, gggg, tttt;
#endif
    };
};

template <> struct base_vec2<half>
{
    explicit inline base_vec2() {}
    explicit inline base_vec2(half X, half Y) : x(X), y(Y) {}

    half x, y;
};

template <> struct base_vec2<real>
{
    explicit inline base_vec2() {}
    explicit inline base_vec2(real X, real Y) : x(X), y(Y) {}

    real x, y;
};

template <typename T>
struct vec<2,T> : base_vec2<T>
{
    typedef vec<2,T> type;

    inline constexpr vec() {}
    inline constexpr vec(T X, T Y) : base_vec2<T>(X, Y) {}

    explicit inline constexpr vec(T X) : base_vec2<T>(X, X) {}

    template<int MASK>
    inline constexpr vec(vec<2, T, MASK> const &v)
      : base_vec2<T>(v[0], v[1]) {}

    template<typename U, int MASK>
    explicit inline constexpr vec(vec<2, U, MASK> const &v)
      : base_vec2<T>(v[0], v[1]) {}

    LOL_COMMON_MEMBER_OPS(x)
    LOL_VECTOR_MEMBER_OPS()

    static const vec<2,T> zero;
    static const vec<2,T> axis_x;
    static const vec<2,T> axis_y;

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, vec<2,U> const &v);
};

/*
 * 3-element vectors
 */

template <typename T> struct base_vec3
{
    explicit inline constexpr base_vec3() {}
    explicit inline constexpr base_vec3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { T s, t, p; };

#if !_DOXYGEN_SKIP_ME
        vec<2,T,0x00> const xx, rr, ss;
        vec<2,T,0x01> _____ xy, rg, st;
        vec<2,T,0x02> _____ xz, rb, sp;
        vec<2,T,0x10> _____ yx, gr, ts;
        vec<2,T,0x11> const yy, gg, tt;
        vec<2,T,0x12> _____ yz, gb, tp;
        vec<2,T,0x20> _____ zx, br, ps;
        vec<2,T,0x21> _____ zy, bg, pt;
        vec<2,T,0x22> const zz, bb, pp;

        vec<3,T,0x000> const xxx, rrr, sss;
        vec<3,T,0x001> const xxy, rrg, sst;
        vec<3,T,0x002> const xxz, rrb, ssp;
        vec<3,T,0x010> const xyx, rgr, sts;
        vec<3,T,0x011> const xyy, rgg, stt;
        vec<3,T,0x012> _____ xyz, rgb, stp;
        vec<3,T,0x020> const xzx, rbr, sps;
        vec<3,T,0x021> _____ xzy, rbg, spt;
        vec<3,T,0x022> const xzz, rbb, spp;
        vec<3,T,0x100> const yxx, grr, tss;
        vec<3,T,0x101> const yxy, grg, tst;
        vec<3,T,0x102> _____ yxz, grb, tsp;
        vec<3,T,0x110> const yyx, ggr, tts;
        vec<3,T,0x111> const yyy, ggg, ttt;
        vec<3,T,0x112> const yyz, ggb, ttp;
        vec<3,T,0x120> _____ yzx, gbr, tps;
        vec<3,T,0x121> const yzy, gbg, tpt;
        vec<3,T,0x122> const yzz, gbb, tpp;
        vec<3,T,0x200> const zxx, brr, pss;
        vec<3,T,0x201> _____ zxy, brg, pst;
        vec<3,T,0x202> const zxz, brb, psp;
        vec<3,T,0x210> _____ zyx, bgr, pts;
        vec<3,T,0x211> const zyy, bgg, ptt;
        vec<3,T,0x212> const zyz, bgb, ptp;
        vec<3,T,0x220> const zzx, bbr, pps;
        vec<3,T,0x221> const zzy, bbg, ppt;
        vec<3,T,0x222> const zzz, bbb, ppp;

        vec<4,T,0x0000> const xxxx, rrrr, ssss;
        vec<4,T,0x0001> const xxxy, rrrg, ssst;
        vec<4,T,0x0002> const xxxz, rrrb, sssp;
        vec<4,T,0x0010> const xxyx, rrgr, ssts;
        vec<4,T,0x0011> const xxyy, rrgg, sstt;
        vec<4,T,0x0012> const xxyz, rrgb, sstp;
        vec<4,T,0x0020> const xxzx, rrbr, ssps;
        vec<4,T,0x0021> const xxzy, rrbg, sspt;
        vec<4,T,0x0022> const xxzz, rrbb, sspp;
        vec<4,T,0x0100> const xyxx, rgrr, stss;
        vec<4,T,0x0101> const xyxy, rgrg, stst;
        vec<4,T,0x0102> const xyxz, rgrb, stsp;
        vec<4,T,0x0110> const xyyx, rggr, stts;
        vec<4,T,0x0111> const xyyy, rggg, sttt;
        vec<4,T,0x0112> const xyyz, rggb, sttp;
        vec<4,T,0x0120> const xyzx, rgbr, stps;
        vec<4,T,0x0121> const xyzy, rgbg, stpt;
        vec<4,T,0x0122> const xyzz, rgbb, stpp;
        vec<4,T,0x0200> const xzxx, rbrr, spss;
        vec<4,T,0x0201> const xzxy, rbrg, spst;
        vec<4,T,0x0202> const xzxz, rbrb, spsp;
        vec<4,T,0x0210> const xzyx, rbgr, spts;
        vec<4,T,0x0211> const xzyy, rbgg, sptt;
        vec<4,T,0x0212> const xzyz, rbgb, sptp;
        vec<4,T,0x0220> const xzzx, rbbr, spps;
        vec<4,T,0x0221> const xzzy, rbbg, sppt;
        vec<4,T,0x0222> const xzzz, rbbb, sppp;
        vec<4,T,0x1000> const yxxx, grrr, tsss;
        vec<4,T,0x1001> const yxxy, grrg, tsst;
        vec<4,T,0x1002> const yxxz, grrb, tssp;
        vec<4,T,0x1010> const yxyx, grgr, tsts;
        vec<4,T,0x1011> const yxyy, grgg, tstt;
        vec<4,T,0x1012> const yxyz, grgb, tstp;
        vec<4,T,0x1020> const yxzx, grbr, tsps;
        vec<4,T,0x1021> const yxzy, grbg, tspt;
        vec<4,T,0x1022> const yxzz, grbb, tspp;
        vec<4,T,0x1100> const yyxx, ggrr, ttss;
        vec<4,T,0x1101> const yyxy, ggrg, ttst;
        vec<4,T,0x1102> const yyxz, ggrb, ttsp;
        vec<4,T,0x1110> const yyyx, gggr, ttts;
        vec<4,T,0x1111> const yyyy, gggg, tttt;
        vec<4,T,0x1112> const yyyz, gggb, tttp;
        vec<4,T,0x1120> const yyzx, ggbr, ttps;
        vec<4,T,0x1121> const yyzy, ggbg, ttpt;
        vec<4,T,0x1122> const yyzz, ggbb, ttpp;
        vec<4,T,0x1200> const yzxx, gbrr, tpss;
        vec<4,T,0x1201> const yzxy, gbrg, tpst;
        vec<4,T,0x1202> const yzxz, gbrb, tpsp;
        vec<4,T,0x1210> const yzyx, gbgr, tpts;
        vec<4,T,0x1211> const yzyy, gbgg, tptt;
        vec<4,T,0x1212> const yzyz, gbgb, tptp;
        vec<4,T,0x1220> const yzzx, gbbr, tpps;
        vec<4,T,0x1221> const yzzy, gbbg, tppt;
        vec<4,T,0x1222> const yzzz, gbbb, tppp;
        vec<4,T,0x2000> const zxxx, brrr, psss;
        vec<4,T,0x2001> const zxxy, brrg, psst;
        vec<4,T,0x2002> const zxxz, brrb, pssp;
        vec<4,T,0x2010> const zxyx, brgr, psts;
        vec<4,T,0x2011> const zxyy, brgg, pstt;
        vec<4,T,0x2012> const zxyz, brgb, pstp;
        vec<4,T,0x2020> const zxzx, brbr, psps;
        vec<4,T,0x2021> const zxzy, brbg, pspt;
        vec<4,T,0x2022> const zxzz, brbb, pspp;
        vec<4,T,0x2100> const zyxx, bgrr, ptss;
        vec<4,T,0x2101> const zyxy, bgrg, ptst;
        vec<4,T,0x2102> const zyxz, bgrb, ptsp;
        vec<4,T,0x2110> const zyyx, bggr, ptts;
        vec<4,T,0x2111> const zyyy, bggg, pttt;
        vec<4,T,0x2112> const zyyz, bggb, pttp;
        vec<4,T,0x2120> const zyzx, bgbr, ptps;
        vec<4,T,0x2121> const zyzy, bgbg, ptpt;
        vec<4,T,0x2122> const zyzz, bgbb, ptpp;
        vec<4,T,0x2200> const zzxx, bbrr, ppss;
        vec<4,T,0x2201> const zzxy, bbrg, ppst;
        vec<4,T,0x2202> const zzxz, bbrb, ppsp;
        vec<4,T,0x2210> const zzyx, bbgr, ppts;
        vec<4,T,0x2211> const zzyy, bbgg, pptt;
        vec<4,T,0x2212> const zzyz, bbgb, pptp;
        vec<4,T,0x2220> const zzzx, bbbr, ppps;
        vec<4,T,0x2221> const zzzy, bbbg, pppt;
        vec<4,T,0x2222> const zzzz, bbbb, pppp;
#endif
    };
};

template <> struct base_vec3<half>
{
    explicit inline base_vec3() {}
    explicit inline base_vec3(half X, half Y, half Z)
      : x(X), y(Y), z(Z) {}

    half x, y, z;
};

template <> struct base_vec3<real>
{
    explicit inline base_vec3() {}
    explicit inline base_vec3(real X, real Y, real Z) : x(X), y(Y), z(Z) {}

    real x, y, z;
};

template <typename T>
struct vec<3,T> : base_vec3<T>
{
    typedef vec<3,T> type;

    inline constexpr vec() {}
    inline constexpr vec(T X, T Y, T Z) : base_vec3<T>(X, Y, Z) {}
    inline constexpr vec(vec<2,T> XY, T Z) : base_vec3<T>(XY.x, XY.y, Z) {}
    inline constexpr vec(T X, vec<2,T> YZ) : base_vec3<T>(X, YZ.x, YZ.y) {}

    explicit inline constexpr vec(T X) : base_vec3<T>(X, X, X) {}

    template<int MASK>
    inline constexpr vec(vec<3, T, MASK> const &v)
      : base_vec3<T>(v[0], v[1], v[2]) {}

    template<typename U, int MASK>
    explicit inline constexpr vec(vec<3, U, MASK> const &v)
      : base_vec3<T>(v[0], v[1], v[2]) {}

    LOL_COMMON_MEMBER_OPS(x)
    LOL_VECTOR_MEMBER_OPS()

    static vec<3,T> toeuler_xyx(Quat<T> const &q);
    static vec<3,T> toeuler_xzx(Quat<T> const &q);
    static vec<3,T> toeuler_yxy(Quat<T> const &q);
    static vec<3,T> toeuler_yzy(Quat<T> const &q);
    static vec<3,T> toeuler_zxz(Quat<T> const &q);
    static vec<3,T> toeuler_zyz(Quat<T> const &q);

    static vec<3,T> toeuler_xyz(Quat<T> const &q);
    static vec<3,T> toeuler_xzy(Quat<T> const &q);
    static vec<3,T> toeuler_yxz(Quat<T> const &q);
    static vec<3,T> toeuler_yzx(Quat<T> const &q);
    static vec<3,T> toeuler_zxy(Quat<T> const &q);
    static vec<3,T> toeuler_zyx(Quat<T> const &q);

    /* Return the cross product (vector product) of "a" and "b" */ \
    friend inline type cross(type const &a, type const &b)
    {
        return type(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
    }

    /* Return a vector that is orthogonal to "a" */
    friend inline type orthogonal(type const &a)
    {
        return lol::abs(a.x) > lol::abs(a.z)
             ? type(-a.y, a.x, (T)0)
             : type((T)0, -a.z, a.y);
    }

    /* Return a vector that is orthonormal to "a" */
    friend inline type orthonormal(type const &a)
    {
        return normalize(orthogonal(a));
    }

    static const vec<3,T> zero;
    static const vec<3,T> axis_x;
    static const vec<3,T> axis_y;
    static const vec<3,T> axis_z;

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, vec<3,U> const &v);
};

/*
 * 4-element vectors
 */

template <typename T> struct base_vec4
{
    explicit inline constexpr base_vec4() {}
    explicit inline constexpr base_vec4(T X, T Y, T Z, T W)
      : x(X), y(Y), z(Z), w(W) {}

    union
    {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T s, t, p, q; };

#if !_DOXYGEN_SKIP_ME
        vec<2,T,0x00> const xx, rr, ss;
        vec<2,T,0x01> _____ xy, rg, st;
        vec<2,T,0x02> _____ xz, rb, sp;
        vec<2,T,0x03> _____ xw, ra, sq;
        vec<2,T,0x10> _____ yx, gr, ts;
        vec<2,T,0x11> const yy, gg, tt;
        vec<2,T,0x12> _____ yz, gb, tp;
        vec<2,T,0x13> _____ yw, ga, tq;
        vec<2,T,0x20> _____ zx, br, ps;
        vec<2,T,0x21> _____ zy, bg, pt;
        vec<2,T,0x22> const zz, bb, pp;
        vec<2,T,0x23> _____ zw, ba, pq;
        vec<2,T,0x30> _____ wx, ar, qs;
        vec<2,T,0x31> _____ wy, ag, qt;
        vec<2,T,0x32> _____ wz, ab, qp;
        vec<2,T,0x33> const ww, aa, qq;

        vec<3,T,0x000> const xxx, rrr, sss;
        vec<3,T,0x001> const xxy, rrg, sst;
        vec<3,T,0x002> const xxz, rrb, ssp;
        vec<3,T,0x003> const xxw, rra, ssq;
        vec<3,T,0x010> const xyx, rgr, sts;
        vec<3,T,0x011> const xyy, rgg, stt;
        vec<3,T,0x012> _____ xyz, rgb, stp;
        vec<3,T,0x013> _____ xyw, rga, stq;
        vec<3,T,0x020> const xzx, rbr, sps;
        vec<3,T,0x021> _____ xzy, rbg, spt;
        vec<3,T,0x022> const xzz, rbb, spp;
        vec<3,T,0x023> _____ xzw, rba, spq;
        vec<3,T,0x030> const xwx, rar, sqs;
        vec<3,T,0x031> _____ xwy, rag, sqt;
        vec<3,T,0x032> _____ xwz, rab, sqp;
        vec<3,T,0x033> const xww, raa, sqq;
        vec<3,T,0x100> const yxx, grr, tss;
        vec<3,T,0x101> const yxy, grg, tst;
        vec<3,T,0x102> _____ yxz, grb, tsp;
        vec<3,T,0x103> _____ yxw, gra, tsq;
        vec<3,T,0x110> const yyx, ggr, tts;
        vec<3,T,0x111> const yyy, ggg, ttt;
        vec<3,T,0x112> const yyz, ggb, ttp;
        vec<3,T,0x113> const yyw, gga, ttq;
        vec<3,T,0x120> _____ yzx, gbr, tps;
        vec<3,T,0x121> const yzy, gbg, tpt;
        vec<3,T,0x122> const yzz, gbb, tpp;
        vec<3,T,0x123> _____ yzw, gba, tpq;
        vec<3,T,0x130> _____ ywx, gar, tqs;
        vec<3,T,0x131> const ywy, gag, tqt;
        vec<3,T,0x132> _____ ywz, gab, tqp;
        vec<3,T,0x133> const yww, gaa, tqq;
        vec<3,T,0x200> const zxx, brr, pss;
        vec<3,T,0x201> _____ zxy, brg, pst;
        vec<3,T,0x202> const zxz, brb, psp;
        vec<3,T,0x203> _____ zxw, bra, psq;
        vec<3,T,0x210> _____ zyx, bgr, pts;
        vec<3,T,0x211> const zyy, bgg, ptt;
        vec<3,T,0x212> const zyz, bgb, ptp;
        vec<3,T,0x213> _____ zyw, bga, ptq;
        vec<3,T,0x220> const zzx, bbr, pps;
        vec<3,T,0x221> const zzy, bbg, ppt;
        vec<3,T,0x222> const zzz, bbb, ppp;
        vec<3,T,0x223> const zzw, bba, ppq;
        vec<3,T,0x230> _____ zwx, bar, pqs;
        vec<3,T,0x231> _____ zwy, bag, pqt;
        vec<3,T,0x232> const zwz, bab, pqp;
        vec<3,T,0x233> const zww, baa, pqq;
        vec<3,T,0x300> const wxx, arr, qss;
        vec<3,T,0x301> _____ wxy, arg, qst;
        vec<3,T,0x302> _____ wxz, arb, qsp;
        vec<3,T,0x303> const wxw, ara, qsq;
        vec<3,T,0x310> _____ wyx, agr, qts;
        vec<3,T,0x311> const wyy, agg, qtt;
        vec<3,T,0x312> _____ wyz, agb, qtp;
        vec<3,T,0x313> const wyw, aga, qtq;
        vec<3,T,0x320> _____ wzx, abr, qps;
        vec<3,T,0x321> _____ wzy, abg, qpt;
        vec<3,T,0x322> const wzz, abb, qpp;
        vec<3,T,0x323> const wzw, aba, qpq;
        vec<3,T,0x330> const wwx, aar, qqs;
        vec<3,T,0x331> const wwy, aag, qqt;
        vec<3,T,0x332> const wwz, aab, qqp;
        vec<3,T,0x333> const www, aaa, qqq;

        vec<4,T,0x0000> const xxxx, rrrr, ssss;
        vec<4,T,0x0001> const xxxy, rrrg, ssst;
        vec<4,T,0x0002> const xxxz, rrrb, sssp;
        vec<4,T,0x0003> const xxxw, rrra, sssq;
        vec<4,T,0x0010> const xxyx, rrgr, ssts;
        vec<4,T,0x0011> const xxyy, rrgg, sstt;
        vec<4,T,0x0012> const xxyz, rrgb, sstp;
        vec<4,T,0x0013> const xxyw, rrga, sstq;
        vec<4,T,0x0020> const xxzx, rrbr, ssps;
        vec<4,T,0x0021> const xxzy, rrbg, sspt;
        vec<4,T,0x0022> const xxzz, rrbb, sspp;
        vec<4,T,0x0023> const xxzw, rrba, sspq;
        vec<4,T,0x0030> const xxwx, rrar, ssqs;
        vec<4,T,0x0031> const xxwy, rrag, ssqt;
        vec<4,T,0x0032> const xxwz, rrab, ssqp;
        vec<4,T,0x0033> const xxww, rraa, ssqq;
        vec<4,T,0x0100> const xyxx, rgrr, stss;
        vec<4,T,0x0101> const xyxy, rgrg, stst;
        vec<4,T,0x0102> const xyxz, rgrb, stsp;
        vec<4,T,0x0103> const xyxw, rgra, stsq;
        vec<4,T,0x0110> const xyyx, rggr, stts;
        vec<4,T,0x0111> const xyyy, rggg, sttt;
        vec<4,T,0x0112> const xyyz, rggb, sttp;
        vec<4,T,0x0113> const xyyw, rgga, sttq;
        vec<4,T,0x0120> const xyzx, rgbr, stps;
        vec<4,T,0x0121> const xyzy, rgbg, stpt;
        vec<4,T,0x0122> const xyzz, rgbb, stpp;
        vec<4,T,0x0123> _____ xyzw, rgba, stpq;
        vec<4,T,0x0130> const xywx, rgar, stqs;
        vec<4,T,0x0131> const xywy, rgag, stqt;
        vec<4,T,0x0132> _____ xywz, rgab, stqp;
        vec<4,T,0x0133> const xyww, rgaa, stqq;
        vec<4,T,0x0200> const xzxx, rbrr, spss;
        vec<4,T,0x0201> const xzxy, rbrg, spst;
        vec<4,T,0x0202> const xzxz, rbrb, spsp;
        vec<4,T,0x0203> const xzxw, rbra, spsq;
        vec<4,T,0x0210> const xzyx, rbgr, spts;
        vec<4,T,0x0211> const xzyy, rbgg, sptt;
        vec<4,T,0x0212> const xzyz, rbgb, sptp;
        vec<4,T,0x0213> _____ xzyw, rbga, sptq;
        vec<4,T,0x0220> const xzzx, rbbr, spps;
        vec<4,T,0x0221> const xzzy, rbbg, sppt;
        vec<4,T,0x0222> const xzzz, rbbb, sppp;
        vec<4,T,0x0223> const xzzw, rbba, sppq;
        vec<4,T,0x0230> const xzwx, rbar, spqs;
        vec<4,T,0x0231> _____ xzwy, rbag, spqt;
        vec<4,T,0x0232> const xzwz, rbab, spqp;
        vec<4,T,0x0233> const xzww, rbaa, spqq;
        vec<4,T,0x0300> const xwxx, rarr, sqss;
        vec<4,T,0x0301> const xwxy, rarg, sqst;
        vec<4,T,0x0302> const xwxz, rarb, sqsp;
        vec<4,T,0x0303> const xwxw, rara, sqsq;
        vec<4,T,0x0310> const xwyx, ragr, sqts;
        vec<4,T,0x0311> const xwyy, ragg, sqtt;
        vec<4,T,0x0312> _____ xwyz, ragb, sqtp;
        vec<4,T,0x0313> const xwyw, raga, sqtq;
        vec<4,T,0x0320> const xwzx, rabr, sqps;
        vec<4,T,0x0321> _____ xwzy, rabg, sqpt;
        vec<4,T,0x0322> const xwzz, rabb, sqpp;
        vec<4,T,0x0323> const xwzw, raba, sqpq;
        vec<4,T,0x0330> const xwwx, raar, sqqs;
        vec<4,T,0x0331> const xwwy, raag, sqqt;
        vec<4,T,0x0332> const xwwz, raab, sqqp;
        vec<4,T,0x0333> const xwww, raaa, sqqq;
        vec<4,T,0x1000> const yxxx, grrr, tsss;
        vec<4,T,0x1001> const yxxy, grrg, tsst;
        vec<4,T,0x1002> const yxxz, grrb, tssp;
        vec<4,T,0x1003> const yxxw, grra, tssq;
        vec<4,T,0x1010> const yxyx, grgr, tsts;
        vec<4,T,0x1011> const yxyy, grgg, tstt;
        vec<4,T,0x1012> const yxyz, grgb, tstp;
        vec<4,T,0x1013> const yxyw, grga, tstq;
        vec<4,T,0x1020> const yxzx, grbr, tsps;
        vec<4,T,0x1021> const yxzy, grbg, tspt;
        vec<4,T,0x1022> const yxzz, grbb, tspp;
        vec<4,T,0x1023> _____ yxzw, grba, tspq;
        vec<4,T,0x1030> const yxwx, grar, tsqs;
        vec<4,T,0x1031> const yxwy, grag, tsqt;
        vec<4,T,0x1032> _____ yxwz, grab, tsqp;
        vec<4,T,0x1033> const yxww, graa, tsqq;
        vec<4,T,0x1100> const yyxx, ggrr, ttss;
        vec<4,T,0x1101> const yyxy, ggrg, ttst;
        vec<4,T,0x1102> const yyxz, ggrb, ttsp;
        vec<4,T,0x1103> const yyxw, ggra, ttsq;
        vec<4,T,0x1110> const yyyx, gggr, ttts;
        vec<4,T,0x1111> const yyyy, gggg, tttt;
        vec<4,T,0x1112> const yyyz, gggb, tttp;
        vec<4,T,0x1113> const yyyw, ggga, tttq;
        vec<4,T,0x1120> const yyzx, ggbr, ttps;
        vec<4,T,0x1121> const yyzy, ggbg, ttpt;
        vec<4,T,0x1122> const yyzz, ggbb, ttpp;
        vec<4,T,0x1123> const yyzw, ggba, ttpq;
        vec<4,T,0x1130> const yywx, ggar, ttqs;
        vec<4,T,0x1131> const yywy, ggag, ttqt;
        vec<4,T,0x1132> const yywz, ggab, ttqp;
        vec<4,T,0x1133> const yyww, ggaa, ttqq;
        vec<4,T,0x1200> const yzxx, gbrr, tpss;
        vec<4,T,0x1201> const yzxy, gbrg, tpst;
        vec<4,T,0x1202> const yzxz, gbrb, tpsp;
        vec<4,T,0x1203> _____ yzxw, gbra, tpsq;
        vec<4,T,0x1210> const yzyx, gbgr, tpts;
        vec<4,T,0x1211> const yzyy, gbgg, tptt;
        vec<4,T,0x1212> const yzyz, gbgb, tptp;
        vec<4,T,0x1213> const yzyw, gbga, tptq;
        vec<4,T,0x1220> const yzzx, gbbr, tpps;
        vec<4,T,0x1221> const yzzy, gbbg, tppt;
        vec<4,T,0x1222> const yzzz, gbbb, tppp;
        vec<4,T,0x1223> const yzzw, gbba, tppq;
        vec<4,T,0x1230> _____ yzwx, gbar, tpqs;
        vec<4,T,0x1231> const yzwy, gbag, tpqt;
        vec<4,T,0x1232> const yzwz, gbab, tpqp;
        vec<4,T,0x1233> const yzww, gbaa, tpqq;
        vec<4,T,0x1300> const ywxx, garr, tqss;
        vec<4,T,0x1301> const ywxy, garg, tqst;
        vec<4,T,0x1302> _____ ywxz, garb, tqsp;
        vec<4,T,0x1303> const ywxw, gara, tqsq;
        vec<4,T,0x1310> const ywyx, gagr, tqts;
        vec<4,T,0x1311> const ywyy, gagg, tqtt;
        vec<4,T,0x1312> const ywyz, gagb, tqtp;
        vec<4,T,0x1313> const ywyw, gaga, tqtq;
        vec<4,T,0x1320> _____ ywzx, gabr, tqps;
        vec<4,T,0x1321> const ywzy, gabg, tqpt;
        vec<4,T,0x1322> const ywzz, gabb, tqpp;
        vec<4,T,0x1323> const ywzw, gaba, tqpq;
        vec<4,T,0x1330> const ywwx, gaar, tqqs;
        vec<4,T,0x1331> const ywwy, gaag, tqqt;
        vec<4,T,0x1332> const ywwz, gaab, tqqp;
        vec<4,T,0x1333> const ywww, gaaa, tqqq;
        vec<4,T,0x2000> const zxxx, brrr, psss;
        vec<4,T,0x2001> const zxxy, brrg, psst;
        vec<4,T,0x2002> const zxxz, brrb, pssp;
        vec<4,T,0x2003> const zxxw, brra, pssq;
        vec<4,T,0x2010> const zxyx, brgr, psts;
        vec<4,T,0x2011> const zxyy, brgg, pstt;
        vec<4,T,0x2012> const zxyz, brgb, pstp;
        vec<4,T,0x2013> _____ zxyw, brga, pstq;
        vec<4,T,0x2020> const zxzx, brbr, psps;
        vec<4,T,0x2021> const zxzy, brbg, pspt;
        vec<4,T,0x2022> const zxzz, brbb, pspp;
        vec<4,T,0x2023> const zxzw, brba, pspq;
        vec<4,T,0x2030> const zxwx, brar, psqs;
        vec<4,T,0x2031> _____ zxwy, brag, psqt;
        vec<4,T,0x2032> const zxwz, brab, psqp;
        vec<4,T,0x2033> const zxww, braa, psqq;
        vec<4,T,0x2100> const zyxx, bgrr, ptss;
        vec<4,T,0x2101> const zyxy, bgrg, ptst;
        vec<4,T,0x2102> const zyxz, bgrb, ptsp;
        vec<4,T,0x2103> _____ zyxw, bgra, ptsq;
        vec<4,T,0x2110> const zyyx, bggr, ptts;
        vec<4,T,0x2111> const zyyy, bggg, pttt;
        vec<4,T,0x2112> const zyyz, bggb, pttp;
        vec<4,T,0x2113> const zyyw, bgga, pttq;
        vec<4,T,0x2120> const zyzx, bgbr, ptps;
        vec<4,T,0x2121> const zyzy, bgbg, ptpt;
        vec<4,T,0x2122> const zyzz, bgbb, ptpp;
        vec<4,T,0x2123> const zyzw, bgba, ptpq;
        vec<4,T,0x2130> _____ zywx, bgar, ptqs;
        vec<4,T,0x2131> const zywy, bgag, ptqt;
        vec<4,T,0x2132> const zywz, bgab, ptqp;
        vec<4,T,0x2133> const zyww, bgaa, ptqq;
        vec<4,T,0x2200> const zzxx, bbrr, ppss;
        vec<4,T,0x2201> const zzxy, bbrg, ppst;
        vec<4,T,0x2202> const zzxz, bbrb, ppsp;
        vec<4,T,0x2203> const zzxw, bbra, ppsq;
        vec<4,T,0x2210> const zzyx, bbgr, ppts;
        vec<4,T,0x2211> const zzyy, bbgg, pptt;
        vec<4,T,0x2212> const zzyz, bbgb, pptp;
        vec<4,T,0x2213> const zzyw, bbga, pptq;
        vec<4,T,0x2220> const zzzx, bbbr, ppps;
        vec<4,T,0x2221> const zzzy, bbbg, pppt;
        vec<4,T,0x2222> const zzzz, bbbb, pppp;
        vec<4,T,0x2223> const zzzw, bbba, pppq;
        vec<4,T,0x2230> const zzwx, bbar, ppqs;
        vec<4,T,0x2231> const zzwy, bbag, ppqt;
        vec<4,T,0x2232> const zzwz, bbab, ppqp;
        vec<4,T,0x2233> const zzww, bbaa, ppqq;
        vec<4,T,0x2300> const zwxx, barr, pqss;
        vec<4,T,0x2301> _____ zwxy, barg, pqst;
        vec<4,T,0x2302> const zwxz, barb, pqsp;
        vec<4,T,0x2303> const zwxw, bara, pqsq;
        vec<4,T,0x2310> _____ zwyx, bagr, pqts;
        vec<4,T,0x2311> const zwyy, bagg, pqtt;
        vec<4,T,0x2312> const zwyz, bagb, pqtp;
        vec<4,T,0x2313> const zwyw, baga, pqtq;
        vec<4,T,0x2320> const zwzx, babr, pqps;
        vec<4,T,0x2321> const zwzy, babg, pqpt;
        vec<4,T,0x2322> const zwzz, babb, pqpp;
        vec<4,T,0x2323> const zwzw, baba, pqpq;
        vec<4,T,0x2330> const zwwx, baar, pqqs;
        vec<4,T,0x2331> const zwwy, baag, pqqt;
        vec<4,T,0x2332> const zwwz, baab, pqqp;
        vec<4,T,0x2333> const zwww, baaa, pqqq;
        vec<4,T,0x3000> const wxxx, arrr, qsss;
        vec<4,T,0x3001> const wxxy, arrg, qsst;
        vec<4,T,0x3002> const wxxz, arrb, qssp;
        vec<4,T,0x3003> const wxxw, arra, qssq;
        vec<4,T,0x3010> const wxyx, argr, qsts;
        vec<4,T,0x3011> const wxyy, argg, qstt;
        vec<4,T,0x3012> _____ wxyz, argb, qstp;
        vec<4,T,0x3013> const wxyw, arga, qstq;
        vec<4,T,0x3020> const wxzx, arbr, qsps;
        vec<4,T,0x3021> _____ wxzy, arbg, qspt;
        vec<4,T,0x3022> const wxzz, arbb, qspp;
        vec<4,T,0x3023> const wxzw, arba, qspq;
        vec<4,T,0x3030> const wxwx, arar, qsqs;
        vec<4,T,0x3031> const wxwy, arag, qsqt;
        vec<4,T,0x3032> const wxwz, arab, qsqp;
        vec<4,T,0x3033> const wxww, araa, qsqq;
        vec<4,T,0x3100> const wyxx, agrr, qtss;
        vec<4,T,0x3101> const wyxy, agrg, qtst;
        vec<4,T,0x3102> _____ wyxz, agrb, qtsp;
        vec<4,T,0x3103> const wyxw, agra, qtsq;
        vec<4,T,0x3110> const wyyx, aggr, qtts;
        vec<4,T,0x3111> const wyyy, aggg, qttt;
        vec<4,T,0x3112> const wyyz, aggb, qttp;
        vec<4,T,0x3113> const wyyw, agga, qttq;
        vec<4,T,0x3120> _____ wyzx, agbr, qtps;
        vec<4,T,0x3121> const wyzy, agbg, qtpt;
        vec<4,T,0x3122> const wyzz, agbb, qtpp;
        vec<4,T,0x3123> const wyzw, agba, qtpq;
        vec<4,T,0x3130> const wywx, agar, qtqs;
        vec<4,T,0x3131> const wywy, agag, qtqt;
        vec<4,T,0x3132> const wywz, agab, qtqp;
        vec<4,T,0x3133> const wyww, agaa, qtqq;
        vec<4,T,0x3200> const wzxx, abrr, qpss;
        vec<4,T,0x3201> _____ wzxy, abrg, qpst;
        vec<4,T,0x3202> const wzxz, abrb, qpsp;
        vec<4,T,0x3203> const wzxw, abra, qpsq;
        vec<4,T,0x3210> _____ wzyx, abgr, qpts;
        vec<4,T,0x3211> const wzyy, abgg, qptt;
        vec<4,T,0x3212> const wzyz, abgb, qptp;
        vec<4,T,0x3213> const wzyw, abga, qptq;
        vec<4,T,0x3220> const wzzx, abbr, qpps;
        vec<4,T,0x3221> const wzzy, abbg, qppt;
        vec<4,T,0x3222> const wzzz, abbb, qppp;
        vec<4,T,0x3223> const wzzw, abba, qppq;
        vec<4,T,0x3230> const wzwx, abar, qpqs;
        vec<4,T,0x3231> const wzwy, abag, qpqt;
        vec<4,T,0x3232> const wzwz, abab, qpqp;
        vec<4,T,0x3233> const wzww, abaa, qpqq;
        vec<4,T,0x3300> const wwxx, aarr, qqss;
        vec<4,T,0x3301> const wwxy, aarg, qqst;
        vec<4,T,0x3302> const wwxz, aarb, qqsp;
        vec<4,T,0x3303> const wwxw, aara, qqsq;
        vec<4,T,0x3310> const wwyx, aagr, qqts;
        vec<4,T,0x3311> const wwyy, aagg, qqtt;
        vec<4,T,0x3312> const wwyz, aagb, qqtp;
        vec<4,T,0x3313> const wwyw, aaga, qqtq;
        vec<4,T,0x3320> const wwzx, aabr, qqps;
        vec<4,T,0x3321> const wwzy, aabg, qqpt;
        vec<4,T,0x3322> const wwzz, aabb, qqpp;
        vec<4,T,0x3323> const wwzw, aaba, qqpq;
        vec<4,T,0x3330> const wwwx, aaar, qqqs;
        vec<4,T,0x3331> const wwwy, aaag, qqqt;
        vec<4,T,0x3332> const wwwz, aaab, qqqp;
        vec<4,T,0x3333> const wwww, aaaa, qqqq;
#endif
    };
};

template <> struct base_vec4<half>
{
    explicit inline base_vec4() {}
    explicit inline base_vec4(half X, half Y, half Z, half W)
     : x(X), y(Y), z(Z), w(W) {}

    half x, y, z, w;
};

template <> struct base_vec4<real>
{
    explicit inline base_vec4() {}
    explicit inline base_vec4(real X, real Y, real Z, real W)
     : x(X), y(Y), z(Z), w(W) {}

    real x, y, z, w;
};

template <typename T>
struct vec<4,T> : base_vec4<T>
{
    typedef vec<4,T> type;

    inline constexpr vec() {}
    inline constexpr vec(T X, T Y, T Z, T W)
      : base_vec4<T>(X, Y, Z, W) {}
    inline constexpr vec(vec<2,T> XY, T Z, T W)
      : base_vec4<T>(XY.x, XY.y, Z, W) {}
    inline constexpr vec(T X, vec<2,T> YZ, T W)
      : base_vec4<T>(X, YZ.x, YZ.y, W) {}
    inline constexpr vec(T X, T Y, vec<2,T> ZW)
      : base_vec4<T>(X, Y, ZW.x, ZW.y) {}
    inline constexpr vec(vec<2,T> XY, vec<2,T> ZW)
      : base_vec4<T>(XY.x, XY.y, ZW.x, ZW.y) {}
    inline constexpr vec(vec<3,T> XYZ, T W)
      : base_vec4<T>(XYZ.x, XYZ.y, XYZ.z, W) {}
    inline constexpr vec(T X, vec<3,T> YZW)
      : base_vec4<T>(X, YZW.x, YZW.y, YZW.z) {}

    explicit inline constexpr vec(T X) : base_vec4<T>(X, X, X, X) {}

    template<int MASK>
    inline constexpr vec(vec<4, T, MASK> const &v)
      : base_vec4<T>(v[0], v[1], v[2], v[3]) {}

    template<typename U, int MASK>
    explicit inline constexpr vec(vec<4, U, MASK> const &v)
      : base_vec4<T>(v[0], v[1], v[2], v[3]) {}

    LOL_COMMON_MEMBER_OPS(x)
    LOL_VECTOR_MEMBER_OPS()

    static const vec<4,T> zero;
    static const vec<4,T> axis_x;
    static const vec<4,T> axis_y;
    static const vec<4,T> axis_z;
    static const vec<4,T> axis_w;

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, vec<4,U> const &v);
};

/*
 * Operators for swizzled vectors. Since template deduction cannot be
 * done for two arbitrary vec<> values, we help the compiler understand
 * the expected type.
 */

template<int N, int MASK1, int MASK2, typename T>
static inline bool operator ==(vec<N,T,MASK1> const &a,
                               vec<N,T,MASK2> const &b)
{
    for (size_t i = 0; i < N; ++i)
        if (!(a[i] == b[i]))
            return false;
    return true;
}

template<int N, int MASK1, int MASK2, typename T>
static inline bool operator !=(vec<N,T,MASK1> const &a,
                               vec<N,T,MASK2> const &b)
{
    for (size_t i = 0; i < N; ++i)
        if (a[i] != b[i])
            return true;
    return false;
}

#define LOL_SWIZZLE_V_VV_OP(op) \
    template<int N, int MASK1, int MASK2, typename T> \
    inline vec<N,T> operator op(vec<N,T,MASK1> const &a, \
                                vec<N,T,MASK2> const &b) \
    { \
        return vec<N,T>(a) op vec<N,T>(b); \
    } \
    \
    template<int N, int MASK, typename T> \
    inline vec<N,T> operator op(vec<N,T,MASK> a, T const &b) \
    { \
        return vec<N,T>(a) op b; \
    }

LOL_SWIZZLE_V_VV_OP(+)
LOL_SWIZZLE_V_VV_OP(-)
LOL_SWIZZLE_V_VV_OP(*)
LOL_SWIZZLE_V_VV_OP(/)

#undef LOL_SWIZZLE_V_VV_OP

template<int N, int MASK, typename T>
static inline vec<N,T> operator *(T const &val, vec<N,T,MASK> const &a)
{
    vec<N,T> ret;
    for (size_t i = 0; i < N; ++i)
        ret[i] = val * a[i];
    return ret;
}

/*
 * vec min|max|fmod(vec, vec|scalar)
 * vec min|max|fmod(scalar, vec)
 */

#define LOL_SWIZZLE_V_VV_FUN(fun) \
    template<int N, int MASK1, int MASK2, typename T> \
    inline vec<N,T> fun(vec<N,T,MASK1> const &a, vec<N,T,MASK2> const &b) \
    { \
        using lol::fun; \
        vec<N,T> ret; \
        for (size_t i = 0; i < N; ++i) \
            ret[i] = fun(a[i], b[i]); \
        return ret; \
    } \
    \
    template<int N, int MASK, typename T> \
    inline vec<N,T> fun(vec<N,T,MASK> const &a, T const &b) \
    { \
        using lol::fun; \
        vec<N,T> ret; \
        for (size_t i = 0; i < N; ++i) \
            ret[i] = fun(a[i], b); \
        return ret; \
    } \
    \
    template<int N, int MASK, typename T> \
    inline vec<N,T> fun(T const &a, vec<N,T,MASK> const &b) \
    { \
        using lol::fun; \
        vec<N,T> ret; \
        for (size_t i = 0; i < N; ++i) \
            ret[i] = fun(a, b[i]); \
        return ret; \
    }

LOL_SWIZZLE_V_VV_FUN(min)
LOL_SWIZZLE_V_VV_FUN(max)
LOL_SWIZZLE_V_VV_FUN(fmod)

#undef LOL_SWIZZLE_V_VV_FUN

/*
 * vec clamp(vec, vec, vec)
 * vec clamp(vec, scalar, vec)
 * vec clamp(vec, vec, scalar)
 * vec clamp(vec, scalar, scalar)
 */

template<int N, int MASK1, int MASK2, int MASK3, typename T>
static inline vec<N,T> clamp(vec<N,T,MASK1> const &x,
                             vec<N,T,MASK2> const &a,
                             vec<N,T,MASK3> const &b)
{
    return max(min(x, b), a);
}

template<int N, int MASK1, int MASK2, typename T>
static inline vec<N,T> clamp(vec<N,T,MASK1> const &x,
                             T const &a,
                             vec<N,T,MASK2> const &b)
{
    return max(min(x, b), a);
}

template<int N, int MASK1, int MASK2, typename T>
static inline vec<N,T> clamp(vec<N,T,MASK1> const &x,
                             vec<N,T,MASK2> const &a,
                             T const &b)
{
    return max(min(x, b), a);
}

template<int N, int MASK1, typename T>
static inline vec<N,T> clamp(vec<N,T,MASK1> const &x,
                             T const &a,
                             T const &b)
{
    return max(min(x, b), a);
}

/*
 * vec mix(vec, vec, vec)
 * vec mix(vec, vec, scalar)
 */

template<int N, int MASK1, int MASK2, int MASK3, typename T>
static inline vec<N,T> mix(vec<N,T,MASK1> const &x,
                           vec<N,T,MASK2> const &y,
                           vec<N,T,MASK3> const &a)
{
    return x + a * (y - x);
}

template<int N, int MASK1, int MASK2, typename T>
static inline vec<N,T> mix(vec<N,T,MASK1> const &x,
                           vec<N,T,MASK2> const &y,
                           T const &a)
{
    return x + a * (y - x);
}

/*
 * Some GLSL-like functions.
 */

template<int N, int MASK1, int MASK2, typename T>
static inline T dot(vec<N,T,MASK1> const &a, vec<N,T,MASK2> const &b)
{
    T ret(0);
    for (size_t i = 0; i < N; ++i)
        ret += a[i] * b[i];
    return ret;
}

template<int N, int MASK, typename T>
static inline T sqlength(vec<N,T,MASK> const &a)
{
    return dot(a, a);
}

template<int N, int MASK, typename T>
static inline T length(vec<N,T,MASK> const &a)
{
    /* FIXME: this is not very nice */
    return (T)sqrt((double)sqlength(a));
}

template<int N, int MASK1, int MASK2, typename T>
static inline vec<N,T> lerp(vec<N,T,MASK1> const &a,
                            vec<N,T,MASK2> const &b,
                            T const &s)
{
    vec<N,T> ret;
    for (size_t i = 0; i < N; ++i)
        ret[i] = a[i] + s * (b[i] - a[i]);
    return ret;
}

template<int N, int MASK1, int MASK2, typename T>
static inline T distance(vec<N,T,MASK1> const &a, vec<N,T,MASK2> const &b)
{
    return length(a - b);
}

template<int N, int MASK, typename T>
static inline vec<N,T> fract(vec<N,T,MASK> const &a)
{
    vec<N,T> ret;
    for (size_t i = 0; i < N; ++i)
        ret[i] = fract(a[i]);
    return ret;
}

template<int N, int MASK, typename T>
static inline vec<N,T> normalize(vec<N,T,MASK> const &a)
{
    T norm = (T)length(a);
    return norm ? a / norm : vec<N,T>(T(0));
}

template<int N, int MASK, typename T>
static inline vec<N,T> abs(vec<N,T,MASK> const &a)
{
    vec<N,T> ret;
    for (size_t i = 0; i < N; ++i)
        ret[i] = lol::abs(a[i]);
    return ret;
}

template<int N, int MASK, typename T>
static inline vec<N,T> degrees(vec<N,T,MASK> const &a)
{
    vec<N,T> ret;
    for (size_t i = 0; i < N; ++i)
        ret[i] = lol::degrees(a[i]);
    return ret;
}

template<int N, int MASK, typename T>
static inline vec<N,T> radians(vec<N,T,MASK> const &a)
{
    vec<N,T> ret;
    for (size_t i = 0; i < N; ++i)
        ret[i] = lol::radians(a[i]);
    return ret;
}

/*
 * Magic vector swizzling (part 2/2)
 */

#if LOL_FEATURE_CXX11_RELAXED_UNIONS
template<int N, typename T, int MASK>
inline vec<N, T, MASK>& vec<N, T, MASK>::operator =(vec<N,T> that)
{
    for (int i = 0; i < N; ++i)
        (*this)[i] = that[i];
    return *this;
}
#endif

#if !LOL_FEATURE_CXX11_CONSTEXPR
#undef constexpr
#endif

} /* namespace lol */

#endif // __LOL_MATH_VECTOR_H__

