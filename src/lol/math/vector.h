//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The vector classes
// ------------------
//

#include <lol/base/assert.h>
#include <lol/math/functions.h>
#include <lol/math/half.h>
#include <lol/math/real.h>
#include <lol/math/ops.h>

#include <ostream>
#include <type_traits>

namespace lol
{

#if !LOL_FEATURE_CXX11_CONSTEXPR
#   define constexpr /* */
#endif

#if !LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
#   define _____ const
#else
#   define _____ /* */
#endif

/*
 * Magic vector swizzling
 *
 * These vectors are empty, but thanks to static_cast we can take their
 * address and access the vector of T's that they are union'ed with. We
 * use static_cast instead of reinterpret_cast because there is a stronger
 * guarantee (by the standard) that the address will stay the same across
 * casts.
 *
 * Using swizzled vectors as lvalues:
 * We need to implement an assignment operator _and_ override the default
 * assignment operator. We try to pass arguments by value so that we don't
 * have to care about overwriting ourselves (e.g. c.rgb = c.bgr).
 * However, Visual Studio 2013 doesn't support unrestricted unions, so
 * fuck it for now.
 */

template<typename T, int N, int SWIZZLE>
struct vec_t
    /* MUST have a different base than e.g. vec_t<T,2> otherwise the unions
     * in vec_t<T,2> with the same base will cause empty base optimisation
     * failures. */
  : public swizzle_ops::base<T, SWIZZLE>
{
    static int const count = N;
    typedef T element;
    typedef vec_t<T,N> type;

    /* Disable all default constructors and destructors; this object
     * is only intended to exist as part of a union. */
#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
    vec_t() = delete;
    vec_t(vec_t<T, N, SWIZZLE> const &) = delete;
    ~vec_t() = delete;

    /* Allow the assignment operator if unrestricted unions are supported. */
    inline vec_t<T, N, SWIZZLE>& operator =(vec_t<T, N> that)
    {
        for (int i = 0; i < N; ++i)
            (*this)[i] = that[i];
        return *this;
    }

    inline vec_t<T, N, SWIZZLE>& operator =(vec_t<T, N, SWIZZLE> const &that)
    {
        /* Pass by value in case this == &that */
        return *this = (vec_t<T,N>)that;
    }
#endif

    inline T& operator[](size_t n)
    {
        int const lut[] = { 1000, 100, 10, 1 };
        int const i = SWIZZLE / lut[n + 4 - N] % 10;
        return static_cast<T*>(static_cast<void*>(this))[i];
    }

    inline T const& operator[](size_t n) const
    {
        int const lut[] = { 1000, 100, 10, 1 };
        int const i = SWIZZLE / lut[n + 4 - N] % 10;
        return static_cast<T const*>(static_cast<void const *>(this))[i];
    }
};

/* The generic “vec_t” type, which is a fixed-size vector with no
 * swizzling. There's an override for N=2, N=3, N=4 that has swizzling. */
template<typename T, int N>
struct vec_t<T, N, FULL_SWIZZLE>
  : public componentwise_ops::base<T>
{
    static int const count = N;
    typedef T element;
    typedef vec_t<T,N> type;

    /* Default constructor, copy constructor, and destructor */
    inline constexpr vec_t() {}
    inline vec_t(vec_t<T,N> const &v)
    {
        for (int i = 0; i < N; ++i)
            m_data[i] = v[i];
    }
    inline ~vec_t() {}

    /* Explicit constructor that takes exactly N arguments thanks to SFINAE. */
    template<typename... ARGS>
#if LOL_FEATURE_CXX11_SFINAE_FOR_CTORS
    explicit inline vec_t(T const &X,
        typename std::enable_if<sizeof...(ARGS) + 2 == N, T>::type const &Y,
        ARGS... args)
#else
    explicit inline vec_t(T const &X, T const &Y, ARGS... args)
#endif
    {
        static_assert(sizeof...(ARGS) + 2 == N,
                      "wrong argument count in vec_t constructor");
        internal_init(m_data, X, Y, args...);
    }

    /* Various explicit constructors */
    explicit inline vec_t(T const &X)
    {
        for (auto &value : m_data)
            value = X;
    }

    /* Explicit constructor for type conversion */
    template<typename U>
    explicit inline vec_t(vec_t<U, N> const &v)
    {
        for (int i = 0; i < N; ++i)
            m_data[i] = T(v[i]);
    }

    /* Factory for base axis vectors, e.g. [1,0,0,…,0] */
    static inline vec_t<T, N> axis(int i)
    {
        ASSERT(i >= 0);
        ASSERT(i < N);
        vec_t<T, N> ret(T(0));
        ret[i] = T(1);
        return ret;
    }

    /* Constructor for initializer_list. We need these ugly
     * loops until C++ lets us initialize m_data directly. */
    inline vec_t(std::initializer_list<element> const &list)
    {
        auto l = list.begin();
        for (int i = 0; i < count && l != list.end(); ++i, ++l)
            m_data[i] = *l;
        for (int i = (int)list.size(); i < count; ++i)
            m_data[i] = T(0);
    }

    inline T& operator[](size_t n) { return m_data[n]; }
    inline T const& operator[](size_t n) const { return m_data[n]; }

    static const vec_t<T,N> zero;

private:
    template<typename... ARGS>
    static inline void internal_init(T *data, T const &x, ARGS... args)
    {
        *data++ = x;
        internal_init(data, args...);
    }

    static inline void internal_init(T *data)
    {
        UNUSED(data);
    }

    T m_data[count];
};

/*
 * Helper macros for vector type member functions
 */

#define LOL_COMMON_MEMBER_OPS(first) \
    inline T& operator[](size_t n) { return (&this->first)[n]; } \
    inline T const& operator[](size_t n) const { return (&this->first)[n]; } \
    \
    /* Visual Studio insists on having an assignment operator. */ \
    inline type & operator =(type const &that) \
    { \
        for (int i = 0; i < type::count; ++i) \
            (*this)[i] = that[i]; \
        return *this; \
    } \
    \
    void printf() const; \
    class String tostring() const;

/*
 * 2-element vectors
 */

template <typename T>
struct vec_t<T,2>
  : public swizzle_ops::base<T>
{
    static int const count = 2;
    typedef T element;
    typedef vec_t<T,2> type;

    /* Default constructor, copy constructor, and destructor */
    inline constexpr vec_t() : x(), y() {}
    inline constexpr vec_t(vec_t<T,2> const &v) : x(v.x), y(v.y) {}
    inline ~vec_t() { x.~T(); y.~T(); }

    /* Implicit constructor for swizzling */
    template<int SWIZZLE>
    inline constexpr vec_t(vec_t<T, 2, SWIZZLE> const &v)
      : x(v[0]), y(v[1]) {}

    /* Explicit constructor for type conversion */
    template<typename U, int SWIZZLE>
    explicit inline constexpr vec_t(vec_t<U, 2, SWIZZLE> const &v)
      : x(T(v[0])), y(T(v[1])) {}

    /* Constructor for initializer_list. We need these ugly
     * loops until C++ lets us initialize m_data directly. */
    inline vec_t(std::initializer_list<element> const &list)
    {
        auto l = list.begin();
        for (int i = 0; i < count && l != list.end(); ++i, ++l)
            m_data[i] = *l;
        for (int i = (int)list.size(); i < count; ++i)
            m_data[i] = T(0);
    }

    /* Various explicit constructors */
    explicit inline constexpr vec_t(T X, T Y)
      : x(X), y(Y) {}
    explicit inline constexpr vec_t(T X)
      : x(X), y(X) {}

    /* Factory for base axis vectors, e.g. [1,0,0,…,0] */
    static inline vec_t<T,2> axis(int i)
    {
        ASSERT(i >= 0);
        ASSERT(i < 2);
        return vec_t<T,2>(T(i == 0), T(i == 1));
    }

    LOL_COMMON_MEMBER_OPS(x)

    static const vec_t<T,2> zero;
    static const vec_t<T,2> axis_x;
    static const vec_t<T,2> axis_y;

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, vec_t<U,2> const &v);

    union
    {
        struct { T x, y; }; /* axis */
        struct { T r, g; }; /* red, green */
        struct { T s, t; };

#if !_DOXYGEN_SKIP_ME
        vec_t<T, 2, 9000> const xx, rr, ss/**/, ww;
        vec_t<T, 2, 9001> _____ xy, rg, st, rt, wh;
        vec_t<T, 2, 9010> _____ yx, gr, ts, tr, hw;
        vec_t<T, 2, 9011> const yy, gg, tt/**/, hh;

        vec_t<T, 3, 90000> const xxx, rrr, sss /**/, www;
        vec_t<T, 3, 90001> const xxy, rrg, sst, rrt, wwh;
        vec_t<T, 3, 90010> const xyx, rgr, sts, rtr, whw;
        vec_t<T, 3, 90011> const xyy, rgg, stt, rtt, whh;
        vec_t<T, 3, 90100> const yxx, grr, tss, trr, hww;
        vec_t<T, 3, 90101> const yxy, grg, tst, trt, hwh;
        vec_t<T, 3, 90110> const yyx, ggr, tts, ttr, hhw;
        vec_t<T, 3, 90111> const yyy, ggg, ttt /**/, hhh;

        vec_t<T, 4, 900000> const xxxx, rrrr, ssss  /**/, wwww;
        vec_t<T, 4, 900001> const xxxy, rrrg, ssst, rrrt, wwwh;
        vec_t<T, 4, 900010> const xxyx, rrgr, ssts, rrtr, wwhw;
        vec_t<T, 4, 900011> const xxyy, rrgg, sstt, rrtt, wwhh;
        vec_t<T, 4, 900100> const xyxx, rgrr, stss, rtrr, whww;
        vec_t<T, 4, 900101> const xyxy, rgrg, stst, rtrt, whwh;
        vec_t<T, 4, 900110> const xyyx, rggr, stts, rttr, whhw;
        vec_t<T, 4, 900111> const xyyy, rggg, sttt, rttt, whhh;
        vec_t<T, 4, 901000> const yxxx, grrr, tsss, trrr, hwww;
        vec_t<T, 4, 901001> const yxxy, grrg, tsst, trrt, hwwh;
        vec_t<T, 4, 901010> const yxyx, grgr, tsts, trtr, hwhw;
        vec_t<T, 4, 901011> const yxyy, grgg, tstt, trtt, hwhh;
        vec_t<T, 4, 901100> const yyxx, ggrr, ttss, ttrr, hhww;
        vec_t<T, 4, 901101> const yyxy, ggrg, ttst, ttrt, hhwh;
        vec_t<T, 4, 901110> const yyyx, gggr, ttts, tttr, hhhw;
        vec_t<T, 4, 901111> const yyyy, gggg, tttt  /**/, hhhh;
#endif

        T m_data[count];
    };
};

static_assert(sizeof(i8vec2) == 2, "sizeof(i8vec2) == 2");
static_assert(sizeof(i16vec2) == 4, "sizeof(i16vec2) == 4");
static_assert(sizeof(ivec2) == 8, "sizeof(ivec2) == 8");
static_assert(sizeof(i64vec2) == 16, "sizeof(i64vec2) == 16");

#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
static_assert(sizeof(f16vec2) == 4, "sizeof(f16vec2) == 4");
#endif
static_assert(sizeof(vec2) == 8, "sizeof(vec2) == 8");
static_assert(sizeof(dvec2) == 16, "sizeof(dvec2) == 16");

/*
 * 3-element vectors
 */

template <typename T>
struct vec_t<T,3>
  : public swizzle_ops::base<T>
{
    static int const count = 3;
    typedef T element;
    typedef vec_t<T,3> type;

    /* Default constructor, copy constructor, and destructor */
    inline constexpr vec_t() : x(), y(), z() {}
    inline constexpr vec_t(vec_t<T,3> const &v) : x(v.x), y(v.y), z(v.z) {}
    inline ~vec_t() { x.~T(); y.~T(); z.~T(); }

    /* Implicit constructor for swizzling */
    template<int SWIZZLE>
    inline constexpr vec_t(vec_t<T, 3, SWIZZLE> const &v)
      : x(v[0]), y(v[1]), z(v[2]) {}

    /* Explicit constructor for type conversion */
    template<typename U, int SWIZZLE>
    explicit inline constexpr vec_t(vec_t<U, 3, SWIZZLE> const &v)
      : x(T(v[0])), y(T(v[1])), z(T(v[2])) {}

    /* Constructor for initializer_list. We need these ugly
     * loops until C++ lets us initialize m_data directly. */
    inline vec_t(std::initializer_list<element> const &list)
    {
        auto l = list.begin();
        for (int i = 0; i < count && l != list.end(); ++i, ++l)
            m_data[i] = *l;
        for (int i = (int)list.size(); i < count; ++i)
            m_data[i] = T(0);
    }

    /* Various explicit constructors */
    explicit inline constexpr vec_t(T X)
      : x(X), y(X), z(X) {}
    explicit inline constexpr vec_t(T X, T Y, T Z)
      : x(X), y(Y), z(Z) {}
    explicit inline constexpr vec_t(vec_t<T,2> XY, T Z)
      : x(XY.x), y(XY.y), z(Z) {}
    explicit inline constexpr vec_t(T X, vec_t<T,2> YZ)
      : x(X), y(YZ.x), z(YZ.y) {}

    /* Factory for base axis vectors, e.g. [1,0,0,…,0] */
    static inline vec_t<T,3> axis(int i)
    {
        ASSERT(i >= 0);
        ASSERT(i < 3);
        return vec_t<T,3>(T(i == 0), T(i == 1), T(i == 2));
    }

    LOL_COMMON_MEMBER_OPS(x)

    static vec_t<T,3> toeuler_xyx(quat_t<T> const &q);
    static vec_t<T,3> toeuler_xzx(quat_t<T> const &q);
    static vec_t<T,3> toeuler_yxy(quat_t<T> const &q);
    static vec_t<T,3> toeuler_yzy(quat_t<T> const &q);
    static vec_t<T,3> toeuler_zxz(quat_t<T> const &q);
    static vec_t<T,3> toeuler_zyz(quat_t<T> const &q);

    static vec_t<T,3> toeuler_xyz(quat_t<T> const &q);
    static vec_t<T,3> toeuler_xzy(quat_t<T> const &q);
    static vec_t<T,3> toeuler_yxz(quat_t<T> const &q);
    static vec_t<T,3> toeuler_yzx(quat_t<T> const &q);
    static vec_t<T,3> toeuler_zxy(quat_t<T> const &q);
    static vec_t<T,3> toeuler_zyx(quat_t<T> const &q);

    /* Return the cross product (vector product) of “a” and “b” */ \
    friend inline type cross(type const &a, type const &b)
    {
        return type(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
    }

    /* Return a vector that is orthogonal to “a” */
    friend inline type orthogonal(type const &a)
    {
        return lol::abs(a.x) > lol::abs(a.z)
             ? type(-a.y, a.x, T(0))
             : type(T(0), -a.z, a.y);
    }

    /* Return a vector that is orthonormal to “a” */
    friend inline type orthonormal(type const &a)
    {
        return normalize(orthogonal(a));
    }

    static const vec_t<T,3> zero;
    static const vec_t<T,3> axis_x;
    static const vec_t<T,3> axis_y;
    static const vec_t<T,3> axis_z;

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, vec_t<U,3> const &v);

    union
    {
        struct { T x, y, z; }; /* axis */
        struct { T r, g, b; }; /* red, green, blue */
        struct { T s, t, p; };

#if !_DOXYGEN_SKIP_ME
        vec_t<T, 2, 9000> const xx, rr, ss/**/, ww;
        vec_t<T, 2, 9001> _____ xy, rg, st, rt, wh;
        vec_t<T, 2, 9002> _____ xz, rb, sp, rp, wd;
        vec_t<T, 2, 9010> _____ yx, gr, ts, tr, hw;
        vec_t<T, 2, 9011> const yy, gg, tt/**/, hh;
        vec_t<T, 2, 9012> _____ yz, gb, tp/**/, hd;
        vec_t<T, 2, 9020> _____ zx, br, ps, pr, dw;
        vec_t<T, 2, 9021> _____ zy, bg, pt/**/, dh;
        vec_t<T, 2, 9022> const zz, bb, pp/**/, dd;

        vec_t<T, 3, 90000> const xxx, rrr, sss /**/, www;
        vec_t<T, 3, 90001> const xxy, rrg, sst, rrt, wwh;
        vec_t<T, 3, 90002> const xxz, rrb, ssp, rrp, wwd;
        vec_t<T, 3, 90010> const xyx, rgr, sts, rtr, whw;
        vec_t<T, 3, 90011> const xyy, rgg, stt, rtt, whh;
        vec_t<T, 3, 90012> _____ xyz, rgb, stp, rtp, whd;
        vec_t<T, 3, 90020> const xzx, rbr, sps, rpr, wdw;
        vec_t<T, 3, 90021> _____ xzy, rbg, spt, rpt, wdh;
        vec_t<T, 3, 90022> const xzz, rbb, spp, rpp, wdd;
        vec_t<T, 3, 90100> const yxx, grr, tss, trr, hww;
        vec_t<T, 3, 90101> const yxy, grg, tst, trt, hwh;
        vec_t<T, 3, 90102> _____ yxz, grb, tsp, trp, hwd;
        vec_t<T, 3, 90110> const yyx, ggr, tts, ttr, hhw;
        vec_t<T, 3, 90111> const yyy, ggg, ttt /**/, hhh;
        vec_t<T, 3, 90112> const yyz, ggb, ttp /**/, hhd;
        vec_t<T, 3, 90120> _____ yzx, gbr, tps, tpr, hdw;
        vec_t<T, 3, 90121> const yzy, gbg, tpt /**/, hdh;
        vec_t<T, 3, 90122> const yzz, gbb, tpp /**/, hdd;
        vec_t<T, 3, 90200> const zxx, brr, pss, prr, dww;
        vec_t<T, 3, 90201> _____ zxy, brg, pst, prt, dwh;
        vec_t<T, 3, 90202> const zxz, brb, psp, prp, dwd;
        vec_t<T, 3, 90210> _____ zyx, bgr, pts, ptr, dhw;
        vec_t<T, 3, 90211> const zyy, bgg, ptt /**/, dhh;
        vec_t<T, 3, 90212> const zyz, bgb, ptp /**/, dhd;
        vec_t<T, 3, 90220> const zzx, bbr, pps, ppr, ddw;
        vec_t<T, 3, 90221> const zzy, bbg, ppt /**/, ddh;
        vec_t<T, 3, 90222> const zzz, bbb, ppp /**/, ddd;

        vec_t<T,4,900000> const xxxx, rrrr, ssss;
        vec_t<T,4,900001> const xxxy, rrrg, ssst;
        vec_t<T,4,900002> const xxxz, rrrb, sssp;
        vec_t<T,4,900010> const xxyx, rrgr, ssts;
        vec_t<T,4,900011> const xxyy, rrgg, sstt;
        vec_t<T,4,900012> const xxyz, rrgb, sstp;
        vec_t<T,4,900020> const xxzx, rrbr, ssps;
        vec_t<T,4,900021> const xxzy, rrbg, sspt;
        vec_t<T,4,900022> const xxzz, rrbb, sspp;
        vec_t<T,4,900100> const xyxx, rgrr, stss;
        vec_t<T,4,900101> const xyxy, rgrg, stst;
        vec_t<T,4,900102> const xyxz, rgrb, stsp;
        vec_t<T,4,900110> const xyyx, rggr, stts;
        vec_t<T,4,900111> const xyyy, rggg, sttt;
        vec_t<T,4,900112> const xyyz, rggb, sttp;
        vec_t<T,4,900120> const xyzx, rgbr, stps;
        vec_t<T,4,900121> const xyzy, rgbg, stpt;
        vec_t<T,4,900122> const xyzz, rgbb, stpp;
        vec_t<T,4,900200> const xzxx, rbrr, spss;
        vec_t<T,4,900201> const xzxy, rbrg, spst;
        vec_t<T,4,900202> const xzxz, rbrb, spsp;
        vec_t<T,4,900210> const xzyx, rbgr, spts;
        vec_t<T,4,900211> const xzyy, rbgg, sptt;
        vec_t<T,4,900212> const xzyz, rbgb, sptp;
        vec_t<T,4,900220> const xzzx, rbbr, spps;
        vec_t<T,4,900221> const xzzy, rbbg, sppt;
        vec_t<T,4,900222> const xzzz, rbbb, sppp;
        vec_t<T,4,901000> const yxxx, grrr, tsss;
        vec_t<T,4,901001> const yxxy, grrg, tsst;
        vec_t<T,4,901002> const yxxz, grrb, tssp;
        vec_t<T,4,901010> const yxyx, grgr, tsts;
        vec_t<T,4,901011> const yxyy, grgg, tstt;
        vec_t<T,4,901012> const yxyz, grgb, tstp;
        vec_t<T,4,901020> const yxzx, grbr, tsps;
        vec_t<T,4,901021> const yxzy, grbg, tspt;
        vec_t<T,4,901022> const yxzz, grbb, tspp;
        vec_t<T,4,901100> const yyxx, ggrr, ttss;
        vec_t<T,4,901101> const yyxy, ggrg, ttst;
        vec_t<T,4,901102> const yyxz, ggrb, ttsp;
        vec_t<T,4,901110> const yyyx, gggr, ttts;
        vec_t<T,4,901111> const yyyy, gggg, tttt;
        vec_t<T,4,901112> const yyyz, gggb, tttp;
        vec_t<T,4,901120> const yyzx, ggbr, ttps;
        vec_t<T,4,901121> const yyzy, ggbg, ttpt;
        vec_t<T,4,901122> const yyzz, ggbb, ttpp;
        vec_t<T,4,901200> const yzxx, gbrr, tpss;
        vec_t<T,4,901201> const yzxy, gbrg, tpst;
        vec_t<T,4,901202> const yzxz, gbrb, tpsp;
        vec_t<T,4,901210> const yzyx, gbgr, tpts;
        vec_t<T,4,901211> const yzyy, gbgg, tptt;
        vec_t<T,4,901212> const yzyz, gbgb, tptp;
        vec_t<T,4,901220> const yzzx, gbbr, tpps;
        vec_t<T,4,901221> const yzzy, gbbg, tppt;
        vec_t<T,4,901222> const yzzz, gbbb, tppp;
        vec_t<T,4,902000> const zxxx, brrr, psss;
        vec_t<T,4,902001> const zxxy, brrg, psst;
        vec_t<T,4,902002> const zxxz, brrb, pssp;
        vec_t<T,4,902010> const zxyx, brgr, psts;
        vec_t<T,4,902011> const zxyy, brgg, pstt;
        vec_t<T,4,902012> const zxyz, brgb, pstp;
        vec_t<T,4,902020> const zxzx, brbr, psps;
        vec_t<T,4,902021> const zxzy, brbg, pspt;
        vec_t<T,4,902022> const zxzz, brbb, pspp;
        vec_t<T,4,902100> const zyxx, bgrr, ptss;
        vec_t<T,4,902101> const zyxy, bgrg, ptst;
        vec_t<T,4,902102> const zyxz, bgrb, ptsp;
        vec_t<T,4,902110> const zyyx, bggr, ptts;
        vec_t<T,4,902111> const zyyy, bggg, pttt;
        vec_t<T,4,902112> const zyyz, bggb, pttp;
        vec_t<T,4,902120> const zyzx, bgbr, ptps;
        vec_t<T,4,902121> const zyzy, bgbg, ptpt;
        vec_t<T,4,902122> const zyzz, bgbb, ptpp;
        vec_t<T,4,902200> const zzxx, bbrr, ppss;
        vec_t<T,4,902201> const zzxy, bbrg, ppst;
        vec_t<T,4,902202> const zzxz, bbrb, ppsp;
        vec_t<T,4,902210> const zzyx, bbgr, ppts;
        vec_t<T,4,902211> const zzyy, bbgg, pptt;
        vec_t<T,4,902212> const zzyz, bbgb, pptp;
        vec_t<T,4,902220> const zzzx, bbbr, ppps;
        vec_t<T,4,902221> const zzzy, bbbg, pppt;
        vec_t<T,4,902222> const zzzz, bbbb, pppp;
#endif

        T m_data[count];
    };
};

static_assert(sizeof(i8vec3) == 3, "sizeof(i8vec3) == 3");
static_assert(sizeof(i16vec3) == 6, "sizeof(i16vec3) == 6");
static_assert(sizeof(ivec3) == 12, "sizeof(ivec3) == 12");
static_assert(sizeof(i64vec3) == 24, "sizeof(i64vec3) == 24");

#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
static_assert(sizeof(f16vec3) == 6, "sizeof(f16vec3) == 6");
#endif
static_assert(sizeof(vec3) == 12, "sizeof(vec3) == 12");
static_assert(sizeof(dvec3) == 24, "sizeof(dvec3) == 24");

/*
 * 4-element vectors
 */

template <typename T>
struct vec_t<T,4>
  : public swizzle_ops::base<T>
{
    static int const count = 4;
    typedef T element;
    typedef vec_t<T,4> type;

    /* Default constructor, copy constructor, and destructor */
    inline constexpr vec_t() : x(), y(), z(), w() {}
    inline constexpr vec_t(vec_t<T,4> const &v)
      : x(v.x), y(v.y), z(v.z), w(v.w) {}
    inline ~vec_t() { x.~T(); y.~T(); z.~T(); w.~T(); }

    /* Implicit constructor for swizzling */
    template<int SWIZZLE>
    inline constexpr vec_t(vec_t<T, 4, SWIZZLE> const &v)
      : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

    /* Explicit constructor for type conversion */
    template<typename U, int SWIZZLE>
    explicit inline constexpr vec_t(vec_t<U, 4, SWIZZLE> const &v)
      : x(T(v[0])), y(T(v[1])), z(T(v[2])), w(T(v[3])) {}

    /* Constructor for initializer_list. We need these ugly
     * loops until C++ lets us initialize m_data directly. */
    inline vec_t(std::initializer_list<element> const &list)
    {
        auto l = list.begin();
        for (int i = 0; i < count && l != list.end(); ++i, ++l)
            m_data[i] = *l;
        for (int i = (int)list.size(); i < count; ++i)
            m_data[i] = T(0);
    }

    /* Various explicit constructors */
    explicit inline constexpr vec_t(T X)
      : x(X), y(X), z(X), w(X) {}
    explicit inline constexpr vec_t(T X, T Y, T Z, T W)
      : x(X), y(Y), z(Z), w(W) {}
    explicit inline constexpr vec_t(vec_t<T,2> XY, T Z, T W)
      : x(XY.x), y(XY.y), z(Z), w(W) {}
    explicit inline constexpr vec_t(T X, vec_t<T,2> YZ, T W)
      : x(X), y(YZ.x), z(YZ.y), w(W) {}
    explicit inline constexpr vec_t(T X, T Y, vec_t<T,2> ZW)
      : x(X), y(Y), z(ZW.x), w(ZW.y) {}
    explicit inline constexpr vec_t(vec_t<T,2> XY, vec_t<T,2> ZW)
      : x(XY.x), y(XY.y), z(ZW.x), w(ZW.y) {}
    explicit inline constexpr vec_t(vec_t<T,3> XYZ, T W)
      : x(XYZ.x), y(XYZ.y), z(XYZ.z), w(W) {}
    explicit inline constexpr vec_t(T X, vec_t<T,3> YZW)
      : x(X), y(YZW.x), z(YZW.y), w(YZW.z) {}

    /* Factory for base axis vectors, e.g. [1,0,0,…,0] */
    static inline vec_t<T,4> axis(int i)
    {
        ASSERT(i >= 0);
        ASSERT(i < 4);
        return vec_t<T,4>(T(i == 0), T(i == 1), T(i == 2), T(i == 3));
    }

    LOL_COMMON_MEMBER_OPS(x)

    static const vec_t<T,4> zero;
    static const vec_t<T,4> axis_x;
    static const vec_t<T,4> axis_y;
    static const vec_t<T,4> axis_z;
    static const vec_t<T,4> axis_w;

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, vec_t<U,4> const &v);

    union
    {
        struct { T x, y, z, w; }; /* axis */
        struct { T r, g, b, a; }; /* red, green, blue, alpha */
        struct { T s, t, p, q; };

#if !_DOXYGEN_SKIP_ME
        vec_t<T,2,9000> const xx, rr, ss;
        vec_t<T,2,9001> _____ xy, rg, st;
        vec_t<T,2,9002> _____ xz, rb, sp;
        vec_t<T,2,9003> _____ xw, ra, sq;
        vec_t<T,2,9010> _____ yx, gr, ts;
        vec_t<T,2,9011> const yy, gg, tt;
        vec_t<T,2,9012> _____ yz, gb, tp;
        vec_t<T,2,9013> _____ yw, ga, tq;
        vec_t<T,2,9020> _____ zx, br, ps;
        vec_t<T,2,9021> _____ zy, bg, pt;
        vec_t<T,2,9022> const zz, bb, pp;
        vec_t<T,2,9023> _____ zw, ba, pq;
        vec_t<T,2,9030> _____ wx, ar, qs;
        vec_t<T,2,9031> _____ wy, ag, qt;
        vec_t<T,2,9032> _____ wz, ab, qp;
        vec_t<T,2,9033> const ww, aa, qq;

        vec_t<T,3,90000> const xxx, rrr, sss;
        vec_t<T,3,90001> const xxy, rrg, sst;
        vec_t<T,3,90002> const xxz, rrb, ssp;
        vec_t<T,3,90003> const xxw, rra, ssq;
        vec_t<T,3,90010> const xyx, rgr, sts;
        vec_t<T,3,90011> const xyy, rgg, stt;
        vec_t<T,3,90012> _____ xyz, rgb, stp;
        vec_t<T,3,90013> _____ xyw, rga, stq;
        vec_t<T,3,90020> const xzx, rbr, sps;
        vec_t<T,3,90021> _____ xzy, rbg, spt;
        vec_t<T,3,90022> const xzz, rbb, spp;
        vec_t<T,3,90023> _____ xzw, rba, spq;
        vec_t<T,3,90030> const xwx, rar, sqs;
        vec_t<T,3,90031> _____ xwy, rag, sqt;
        vec_t<T,3,90032> _____ xwz, rab, sqp;
        vec_t<T,3,90033> const xww, raa, sqq;
        vec_t<T,3,90100> const yxx, grr, tss;
        vec_t<T,3,90101> const yxy, grg, tst;
        vec_t<T,3,90102> _____ yxz, grb, tsp;
        vec_t<T,3,90103> _____ yxw, gra, tsq;
        vec_t<T,3,90110> const yyx, ggr, tts;
        vec_t<T,3,90111> const yyy, ggg, ttt;
        vec_t<T,3,90112> const yyz, ggb, ttp;
        vec_t<T,3,90113> const yyw, gga, ttq;
        vec_t<T,3,90120> _____ yzx, gbr, tps;
        vec_t<T,3,90121> const yzy, gbg, tpt;
        vec_t<T,3,90122> const yzz, gbb, tpp;
        vec_t<T,3,90123> _____ yzw, gba, tpq;
        vec_t<T,3,90130> _____ ywx, gar, tqs;
        vec_t<T,3,90131> const ywy, gag, tqt;
        vec_t<T,3,90132> _____ ywz, gab, tqp;
        vec_t<T,3,90133> const yww, gaa, tqq;
        vec_t<T,3,90200> const zxx, brr, pss;
        vec_t<T,3,90201> _____ zxy, brg, pst;
        vec_t<T,3,90202> const zxz, brb, psp;
        vec_t<T,3,90203> _____ zxw, bra, psq;
        vec_t<T,3,90210> _____ zyx, bgr, pts;
        vec_t<T,3,90211> const zyy, bgg, ptt;
        vec_t<T,3,90212> const zyz, bgb, ptp;
        vec_t<T,3,90213> _____ zyw, bga, ptq;
        vec_t<T,3,90220> const zzx, bbr, pps;
        vec_t<T,3,90221> const zzy, bbg, ppt;
        vec_t<T,3,90222> const zzz, bbb, ppp;
        vec_t<T,3,90223> const zzw, bba, ppq;
        vec_t<T,3,90230> _____ zwx, bar, pqs;
        vec_t<T,3,90231> _____ zwy, bag, pqt;
        vec_t<T,3,90232> const zwz, bab, pqp;
        vec_t<T,3,90233> const zww, baa, pqq;
        vec_t<T,3,90300> const wxx, arr, qss;
        vec_t<T,3,90301> _____ wxy, arg, qst;
        vec_t<T,3,90302> _____ wxz, arb, qsp;
        vec_t<T,3,90303> const wxw, ara, qsq;
        vec_t<T,3,90310> _____ wyx, agr, qts;
        vec_t<T,3,90311> const wyy, agg, qtt;
        vec_t<T,3,90312> _____ wyz, agb, qtp;
        vec_t<T,3,90313> const wyw, aga, qtq;
        vec_t<T,3,90320> _____ wzx, abr, qps;
        vec_t<T,3,90321> _____ wzy, abg, qpt;
        vec_t<T,3,90322> const wzz, abb, qpp;
        vec_t<T,3,90323> const wzw, aba, qpq;
        vec_t<T,3,90330> const wwx, aar, qqs;
        vec_t<T,3,90331> const wwy, aag, qqt;
        vec_t<T,3,90332> const wwz, aab, qqp;
        vec_t<T,3,90333> const www, aaa, qqq;

        vec_t<T,4,900000> const xxxx, rrrr, ssss;
        vec_t<T,4,900001> const xxxy, rrrg, ssst;
        vec_t<T,4,900002> const xxxz, rrrb, sssp;
        vec_t<T,4,900003> const xxxw, rrra, sssq;
        vec_t<T,4,900010> const xxyx, rrgr, ssts;
        vec_t<T,4,900011> const xxyy, rrgg, sstt;
        vec_t<T,4,900012> const xxyz, rrgb, sstp;
        vec_t<T,4,900013> const xxyw, rrga, sstq;
        vec_t<T,4,900020> const xxzx, rrbr, ssps;
        vec_t<T,4,900021> const xxzy, rrbg, sspt;
        vec_t<T,4,900022> const xxzz, rrbb, sspp;
        vec_t<T,4,900023> const xxzw, rrba, sspq;
        vec_t<T,4,900030> const xxwx, rrar, ssqs;
        vec_t<T,4,900031> const xxwy, rrag, ssqt;
        vec_t<T,4,900032> const xxwz, rrab, ssqp;
        vec_t<T,4,900033> const xxww, rraa, ssqq;
        vec_t<T,4,900100> const xyxx, rgrr, stss;
        vec_t<T,4,900101> const xyxy, rgrg, stst;
        vec_t<T,4,900102> const xyxz, rgrb, stsp;
        vec_t<T,4,900103> const xyxw, rgra, stsq;
        vec_t<T,4,900110> const xyyx, rggr, stts;
        vec_t<T,4,900111> const xyyy, rggg, sttt;
        vec_t<T,4,900112> const xyyz, rggb, sttp;
        vec_t<T,4,900113> const xyyw, rgga, sttq;
        vec_t<T,4,900120> const xyzx, rgbr, stps;
        vec_t<T,4,900121> const xyzy, rgbg, stpt;
        vec_t<T,4,900122> const xyzz, rgbb, stpp;
        vec_t<T,4,900123> _____ xyzw, rgba, stpq;
        vec_t<T,4,900130> const xywx, rgar, stqs;
        vec_t<T,4,900131> const xywy, rgag, stqt;
        vec_t<T,4,900132> _____ xywz, rgab, stqp;
        vec_t<T,4,900133> const xyww, rgaa, stqq;
        vec_t<T,4,900200> const xzxx, rbrr, spss;
        vec_t<T,4,900201> const xzxy, rbrg, spst;
        vec_t<T,4,900202> const xzxz, rbrb, spsp;
        vec_t<T,4,900203> const xzxw, rbra, spsq;
        vec_t<T,4,900210> const xzyx, rbgr, spts;
        vec_t<T,4,900211> const xzyy, rbgg, sptt;
        vec_t<T,4,900212> const xzyz, rbgb, sptp;
        vec_t<T,4,900213> _____ xzyw, rbga, sptq;
        vec_t<T,4,900220> const xzzx, rbbr, spps;
        vec_t<T,4,900221> const xzzy, rbbg, sppt;
        vec_t<T,4,900222> const xzzz, rbbb, sppp;
        vec_t<T,4,900223> const xzzw, rbba, sppq;
        vec_t<T,4,900230> const xzwx, rbar, spqs;
        vec_t<T,4,900231> _____ xzwy, rbag, spqt;
        vec_t<T,4,900232> const xzwz, rbab, spqp;
        vec_t<T,4,900233> const xzww, rbaa, spqq;
        vec_t<T,4,900300> const xwxx, rarr, sqss;
        vec_t<T,4,900301> const xwxy, rarg, sqst;
        vec_t<T,4,900302> const xwxz, rarb, sqsp;
        vec_t<T,4,900303> const xwxw, rara, sqsq;
        vec_t<T,4,900310> const xwyx, ragr, sqts;
        vec_t<T,4,900311> const xwyy, ragg, sqtt;
        vec_t<T,4,900312> _____ xwyz, ragb, sqtp;
        vec_t<T,4,900313> const xwyw, raga, sqtq;
        vec_t<T,4,900320> const xwzx, rabr, sqps;
        vec_t<T,4,900321> _____ xwzy, rabg, sqpt;
        vec_t<T,4,900322> const xwzz, rabb, sqpp;
        vec_t<T,4,900323> const xwzw, raba, sqpq;
        vec_t<T,4,900330> const xwwx, raar, sqqs;
        vec_t<T,4,900331> const xwwy, raag, sqqt;
        vec_t<T,4,900332> const xwwz, raab, sqqp;
        vec_t<T,4,900333> const xwww, raaa, sqqq;
        vec_t<T,4,901000> const yxxx, grrr, tsss;
        vec_t<T,4,901001> const yxxy, grrg, tsst;
        vec_t<T,4,901002> const yxxz, grrb, tssp;
        vec_t<T,4,901003> const yxxw, grra, tssq;
        vec_t<T,4,901010> const yxyx, grgr, tsts;
        vec_t<T,4,901011> const yxyy, grgg, tstt;
        vec_t<T,4,901012> const yxyz, grgb, tstp;
        vec_t<T,4,901013> const yxyw, grga, tstq;
        vec_t<T,4,901020> const yxzx, grbr, tsps;
        vec_t<T,4,901021> const yxzy, grbg, tspt;
        vec_t<T,4,901022> const yxzz, grbb, tspp;
        vec_t<T,4,901023> _____ yxzw, grba, tspq;
        vec_t<T,4,901030> const yxwx, grar, tsqs;
        vec_t<T,4,901031> const yxwy, grag, tsqt;
        vec_t<T,4,901032> _____ yxwz, grab, tsqp;
        vec_t<T,4,901033> const yxww, graa, tsqq;
        vec_t<T,4,901100> const yyxx, ggrr, ttss;
        vec_t<T,4,901101> const yyxy, ggrg, ttst;
        vec_t<T,4,901102> const yyxz, ggrb, ttsp;
        vec_t<T,4,901103> const yyxw, ggra, ttsq;
        vec_t<T,4,901110> const yyyx, gggr, ttts;
        vec_t<T,4,901111> const yyyy, gggg, tttt;
        vec_t<T,4,901112> const yyyz, gggb, tttp;
        vec_t<T,4,901113> const yyyw, ggga, tttq;
        vec_t<T,4,901120> const yyzx, ggbr, ttps;
        vec_t<T,4,901121> const yyzy, ggbg, ttpt;
        vec_t<T,4,901122> const yyzz, ggbb, ttpp;
        vec_t<T,4,901123> const yyzw, ggba, ttpq;
        vec_t<T,4,901130> const yywx, ggar, ttqs;
        vec_t<T,4,901131> const yywy, ggag, ttqt;
        vec_t<T,4,901132> const yywz, ggab, ttqp;
        vec_t<T,4,901133> const yyww, ggaa, ttqq;
        vec_t<T,4,901200> const yzxx, gbrr, tpss;
        vec_t<T,4,901201> const yzxy, gbrg, tpst;
        vec_t<T,4,901202> const yzxz, gbrb, tpsp;
        vec_t<T,4,901203> _____ yzxw, gbra, tpsq;
        vec_t<T,4,901210> const yzyx, gbgr, tpts;
        vec_t<T,4,901211> const yzyy, gbgg, tptt;
        vec_t<T,4,901212> const yzyz, gbgb, tptp;
        vec_t<T,4,901213> const yzyw, gbga, tptq;
        vec_t<T,4,901220> const yzzx, gbbr, tpps;
        vec_t<T,4,901221> const yzzy, gbbg, tppt;
        vec_t<T,4,901222> const yzzz, gbbb, tppp;
        vec_t<T,4,901223> const yzzw, gbba, tppq;
        vec_t<T,4,901230> _____ yzwx, gbar, tpqs;
        vec_t<T,4,901231> const yzwy, gbag, tpqt;
        vec_t<T,4,901232> const yzwz, gbab, tpqp;
        vec_t<T,4,901233> const yzww, gbaa, tpqq;
        vec_t<T,4,901300> const ywxx, garr, tqss;
        vec_t<T,4,901301> const ywxy, garg, tqst;
        vec_t<T,4,901302> _____ ywxz, garb, tqsp;
        vec_t<T,4,901303> const ywxw, gara, tqsq;
        vec_t<T,4,901310> const ywyx, gagr, tqts;
        vec_t<T,4,901311> const ywyy, gagg, tqtt;
        vec_t<T,4,901312> const ywyz, gagb, tqtp;
        vec_t<T,4,901313> const ywyw, gaga, tqtq;
        vec_t<T,4,901320> _____ ywzx, gabr, tqps;
        vec_t<T,4,901321> const ywzy, gabg, tqpt;
        vec_t<T,4,901322> const ywzz, gabb, tqpp;
        vec_t<T,4,901323> const ywzw, gaba, tqpq;
        vec_t<T,4,901330> const ywwx, gaar, tqqs;
        vec_t<T,4,901331> const ywwy, gaag, tqqt;
        vec_t<T,4,901332> const ywwz, gaab, tqqp;
        vec_t<T,4,901333> const ywww, gaaa, tqqq;
        vec_t<T,4,902000> const zxxx, brrr, psss;
        vec_t<T,4,902001> const zxxy, brrg, psst;
        vec_t<T,4,902002> const zxxz, brrb, pssp;
        vec_t<T,4,902003> const zxxw, brra, pssq;
        vec_t<T,4,902010> const zxyx, brgr, psts;
        vec_t<T,4,902011> const zxyy, brgg, pstt;
        vec_t<T,4,902012> const zxyz, brgb, pstp;
        vec_t<T,4,902013> _____ zxyw, brga, pstq;
        vec_t<T,4,902020> const zxzx, brbr, psps;
        vec_t<T,4,902021> const zxzy, brbg, pspt;
        vec_t<T,4,902022> const zxzz, brbb, pspp;
        vec_t<T,4,902023> const zxzw, brba, pspq;
        vec_t<T,4,902030> const zxwx, brar, psqs;
        vec_t<T,4,902031> _____ zxwy, brag, psqt;
        vec_t<T,4,902032> const zxwz, brab, psqp;
        vec_t<T,4,902033> const zxww, braa, psqq;
        vec_t<T,4,902100> const zyxx, bgrr, ptss;
        vec_t<T,4,902101> const zyxy, bgrg, ptst;
        vec_t<T,4,902102> const zyxz, bgrb, ptsp;
        vec_t<T,4,902103> _____ zyxw, bgra, ptsq;
        vec_t<T,4,902110> const zyyx, bggr, ptts;
        vec_t<T,4,902111> const zyyy, bggg, pttt;
        vec_t<T,4,902112> const zyyz, bggb, pttp;
        vec_t<T,4,902113> const zyyw, bgga, pttq;
        vec_t<T,4,902120> const zyzx, bgbr, ptps;
        vec_t<T,4,902121> const zyzy, bgbg, ptpt;
        vec_t<T,4,902122> const zyzz, bgbb, ptpp;
        vec_t<T,4,902123> const zyzw, bgba, ptpq;
        vec_t<T,4,902130> _____ zywx, bgar, ptqs;
        vec_t<T,4,902131> const zywy, bgag, ptqt;
        vec_t<T,4,902132> const zywz, bgab, ptqp;
        vec_t<T,4,902133> const zyww, bgaa, ptqq;
        vec_t<T,4,902200> const zzxx, bbrr, ppss;
        vec_t<T,4,902201> const zzxy, bbrg, ppst;
        vec_t<T,4,902202> const zzxz, bbrb, ppsp;
        vec_t<T,4,902203> const zzxw, bbra, ppsq;
        vec_t<T,4,902210> const zzyx, bbgr, ppts;
        vec_t<T,4,902211> const zzyy, bbgg, pptt;
        vec_t<T,4,902212> const zzyz, bbgb, pptp;
        vec_t<T,4,902213> const zzyw, bbga, pptq;
        vec_t<T,4,902220> const zzzx, bbbr, ppps;
        vec_t<T,4,902221> const zzzy, bbbg, pppt;
        vec_t<T,4,902222> const zzzz, bbbb, pppp;
        vec_t<T,4,902223> const zzzw, bbba, pppq;
        vec_t<T,4,902230> const zzwx, bbar, ppqs;
        vec_t<T,4,902231> const zzwy, bbag, ppqt;
        vec_t<T,4,902232> const zzwz, bbab, ppqp;
        vec_t<T,4,902233> const zzww, bbaa, ppqq;
        vec_t<T,4,902300> const zwxx, barr, pqss;
        vec_t<T,4,902301> _____ zwxy, barg, pqst;
        vec_t<T,4,902302> const zwxz, barb, pqsp;
        vec_t<T,4,902303> const zwxw, bara, pqsq;
        vec_t<T,4,902310> _____ zwyx, bagr, pqts;
        vec_t<T,4,902311> const zwyy, bagg, pqtt;
        vec_t<T,4,902312> const zwyz, bagb, pqtp;
        vec_t<T,4,902313> const zwyw, baga, pqtq;
        vec_t<T,4,902320> const zwzx, babr, pqps;
        vec_t<T,4,902321> const zwzy, babg, pqpt;
        vec_t<T,4,902322> const zwzz, babb, pqpp;
        vec_t<T,4,902323> const zwzw, baba, pqpq;
        vec_t<T,4,902330> const zwwx, baar, pqqs;
        vec_t<T,4,902331> const zwwy, baag, pqqt;
        vec_t<T,4,902332> const zwwz, baab, pqqp;
        vec_t<T,4,902333> const zwww, baaa, pqqq;
        vec_t<T,4,903000> const wxxx, arrr, qsss;
        vec_t<T,4,903001> const wxxy, arrg, qsst;
        vec_t<T,4,903002> const wxxz, arrb, qssp;
        vec_t<T,4,903003> const wxxw, arra, qssq;
        vec_t<T,4,903010> const wxyx, argr, qsts;
        vec_t<T,4,903011> const wxyy, argg, qstt;
        vec_t<T,4,903012> _____ wxyz, argb, qstp;
        vec_t<T,4,903013> const wxyw, arga, qstq;
        vec_t<T,4,903020> const wxzx, arbr, qsps;
        vec_t<T,4,903021> _____ wxzy, arbg, qspt;
        vec_t<T,4,903022> const wxzz, arbb, qspp;
        vec_t<T,4,903023> const wxzw, arba, qspq;
        vec_t<T,4,903030> const wxwx, arar, qsqs;
        vec_t<T,4,903031> const wxwy, arag, qsqt;
        vec_t<T,4,903032> const wxwz, arab, qsqp;
        vec_t<T,4,903033> const wxww, araa, qsqq;
        vec_t<T,4,903100> const wyxx, agrr, qtss;
        vec_t<T,4,903101> const wyxy, agrg, qtst;
        vec_t<T,4,903102> _____ wyxz, agrb, qtsp;
        vec_t<T,4,903103> const wyxw, agra, qtsq;
        vec_t<T,4,903110> const wyyx, aggr, qtts;
        vec_t<T,4,903111> const wyyy, aggg, qttt;
        vec_t<T,4,903112> const wyyz, aggb, qttp;
        vec_t<T,4,903113> const wyyw, agga, qttq;
        vec_t<T,4,903120> _____ wyzx, agbr, qtps;
        vec_t<T,4,903121> const wyzy, agbg, qtpt;
        vec_t<T,4,903122> const wyzz, agbb, qtpp;
        vec_t<T,4,903123> const wyzw, agba, qtpq;
        vec_t<T,4,903130> const wywx, agar, qtqs;
        vec_t<T,4,903131> const wywy, agag, qtqt;
        vec_t<T,4,903132> const wywz, agab, qtqp;
        vec_t<T,4,903133> const wyww, agaa, qtqq;
        vec_t<T,4,903200> const wzxx, abrr, qpss;
        vec_t<T,4,903201> _____ wzxy, abrg, qpst;
        vec_t<T,4,903202> const wzxz, abrb, qpsp;
        vec_t<T,4,903203> const wzxw, abra, qpsq;
        vec_t<T,4,903210> _____ wzyx, abgr, qpts;
        vec_t<T,4,903211> const wzyy, abgg, qptt;
        vec_t<T,4,903212> const wzyz, abgb, qptp;
        vec_t<T,4,903213> const wzyw, abga, qptq;
        vec_t<T,4,903220> const wzzx, abbr, qpps;
        vec_t<T,4,903221> const wzzy, abbg, qppt;
        vec_t<T,4,903222> const wzzz, abbb, qppp;
        vec_t<T,4,903223> const wzzw, abba, qppq;
        vec_t<T,4,903230> const wzwx, abar, qpqs;
        vec_t<T,4,903231> const wzwy, abag, qpqt;
        vec_t<T,4,903232> const wzwz, abab, qpqp;
        vec_t<T,4,903233> const wzww, abaa, qpqq;
        vec_t<T,4,903300> const wwxx, aarr, qqss;
        vec_t<T,4,903301> const wwxy, aarg, qqst;
        vec_t<T,4,903302> const wwxz, aarb, qqsp;
        vec_t<T,4,903303> const wwxw, aara, qqsq;
        vec_t<T,4,903310> const wwyx, aagr, qqts;
        vec_t<T,4,903311> const wwyy, aagg, qqtt;
        vec_t<T,4,903312> const wwyz, aagb, qqtp;
        vec_t<T,4,903313> const wwyw, aaga, qqtq;
        vec_t<T,4,903320> const wwzx, aabr, qqps;
        vec_t<T,4,903321> const wwzy, aabg, qqpt;
        vec_t<T,4,903322> const wwzz, aabb, qqpp;
        vec_t<T,4,903323> const wwzw, aaba, qqpq;
        vec_t<T,4,903330> const wwwx, aaar, qqqs;
        vec_t<T,4,903331> const wwwy, aaag, qqqt;
        vec_t<T,4,903332> const wwwz, aaab, qqqp;
        vec_t<T,4,903333> const wwww, aaaa, qqqq;
#endif

        T m_data[count];
    };
};

static_assert(sizeof(i8vec4) == 4, "sizeof(i8vec4) == 4");
static_assert(sizeof(i16vec4) == 8, "sizeof(i16vec4) == 8");
static_assert(sizeof(ivec4) == 16, "sizeof(ivec4) == 16");
static_assert(sizeof(i64vec4) == 32, "sizeof(i64vec4) == 32");

#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
static_assert(sizeof(f16vec4) == 8, "sizeof(f16vec4) == 8");
#endif
static_assert(sizeof(vec4) == 16, "sizeof(vec4) == 16");
static_assert(sizeof(dvec4) == 32, "sizeof(dvec4) == 32");

/*
 * vec_t *(scalar, vec_t)
 */

template<typename T, int N, int SWIZZLE>
static inline typename std::enable_if<SWIZZLE != FULL_SWIZZLE, vec_t<T,N>>::type
operator *(T const &val, vec_t<T,N,SWIZZLE> const &a)
{
    vec_t<T,N> ret;
    for (int i = 0; i < N; ++i)
        ret[i] = val * a[i];
    return ret;
}

/*
 * vec_t min|max|fmod(vec_t, vec_t|scalar)
 * vec_t min|max|fmod(scalar, vec_t)
 */

#define LOL_SWIZZLE_V_VV_FUN(fun) \
    template<typename T, int N, int SWIZZLE1, int SWIZZLE2> \
    inline vec_t<T,N> fun(vec_t<T,N,SWIZZLE1> const &a, \
                          vec_t<T,N,SWIZZLE2> const &b) \
    { \
        using lol::fun; \
        vec_t<T,N> ret; \
        for (int i = 0; i < N; ++i) \
            ret[i] = fun(a[i], b[i]); \
        return ret; \
    } \
    \
    template<typename T, int N, int SWIZZLE> \
    inline vec_t<T,N> fun(vec_t<T,N,SWIZZLE> const &a, T const &b) \
    { \
        using lol::fun; \
        vec_t<T,N> ret; \
        for (int i = 0; i < N; ++i) \
            ret[i] = fun(a[i], b); \
        return ret; \
    } \
    \
    template<typename T, int N, int SWIZZLE> \
    inline vec_t<T,N> fun(T const &a, vec_t<T,N,SWIZZLE> const &b) \
    { \
        using lol::fun; \
        vec_t<T,N> ret; \
        for (int i = 0; i < N; ++i) \
            ret[i] = fun(a, b[i]); \
        return ret; \
    }

LOL_SWIZZLE_V_VV_FUN(min)
LOL_SWIZZLE_V_VV_FUN(max)
LOL_SWIZZLE_V_VV_FUN(fmod)

#undef LOL_SWIZZLE_V_VV_FUN

/*
 * vec_t clamp(vec_t, vec_t, vec_t)
 * vec_t clamp(vec_t, scalar, vec_t)
 * vec_t clamp(vec_t, vec_t, scalar)
 * vec_t clamp(vec_t, scalar, scalar)
 */

template<typename T, int N, int SWIZZLE1, int SWIZZLE2, int SWIZZLE3>
static inline vec_t<T,N> clamp(vec_t<T,N,SWIZZLE1> const &x,
                               vec_t<T,N,SWIZZLE2> const &a,
                               vec_t<T,N,SWIZZLE3> const &b)
{
    return max(min(x, b), a);
}

template<typename T, int N, int SWIZZLE1, int SWIZZLE2>
static inline vec_t<T,N> clamp(vec_t<T,N,SWIZZLE1> const &x,
                               T const &a,
                               vec_t<T,N,SWIZZLE2> const &b)
{
    return max(min(x, b), a);
}

template<typename T, int N, int SWIZZLE1, int SWIZZLE2>
static inline vec_t<T,N> clamp(vec_t<T,N,SWIZZLE1> const &x,
                               vec_t<T,N,SWIZZLE2> const &a,
                               T const &b)
{
    return max(min(x, b), a);
}

template<typename T, int N, int SWIZZLE1>
static inline vec_t<T,N> clamp(vec_t<T,N,SWIZZLE1> const &x,
                               T const &a,
                               T const &b)
{
    return max(min(x, b), a);
}

/*
 * vec_t mix(vec_t, vec_t, vec_t)
 * vec_t mix(vec_t, vec_t, scalar)
 */

template<typename T, int N, int SWIZZLE1, int SWIZZLE2, int SWIZZLE3>
static inline vec_t<T,N> mix(vec_t<T,N,SWIZZLE1> const &x,
                             vec_t<T,N,SWIZZLE2> const &y,
                             vec_t<T,N,SWIZZLE3> const &a)
{
    return x + a * (y - x);
}

template<typename T, int N, int SWIZZLE1, int SWIZZLE2>
static inline vec_t<T,N> mix(vec_t<T,N,SWIZZLE1> const &x,
                             vec_t<T,N,SWIZZLE2> const &y,
                             T const &a)
{
    return x + a * (y - x);
}

/*
 * Some GLSL-like functions.
 */

template<typename T, int N, int SWIZZLE1, int SWIZZLE2>
static inline T dot(vec_t<T,N,SWIZZLE1> const &a,
                    vec_t<T,N,SWIZZLE2> const &b)
{
    T ret(0);
    for (int i = 0; i < N; ++i)
        ret += a[i] * b[i];
    return ret;
}

template<typename T, int N, int SWIZZLE>
static inline T sqlength(vec_t<T,N,SWIZZLE> const &a)
{
    return dot(a, a);
}

template<typename T, int N, int SWIZZLE>
static inline T length(vec_t<T,N,SWIZZLE> const &a)
{
    /* FIXME: this is not very nice */
    return T(sqrt((double)sqlength(a)));
}

template<typename T, int N, int SWIZZLE1, int SWIZZLE2>
static inline vec_t<T,N> lerp(vec_t<T,N,SWIZZLE1> const &a,
                              vec_t<T,N,SWIZZLE2> const &b,
                              T const &s)
{
    vec_t<T,N> ret;
    for (int i = 0; i < N; ++i)
        ret[i] = a[i] + s * (b[i] - a[i]);
    return ret;
}

template<typename T, int N, int SWIZZLE1, int SWIZZLE2>
static inline T distance(vec_t<T,N,SWIZZLE1> const &a,
                         vec_t<T,N,SWIZZLE2> const &b)
{
    return length(a - b);
}

template<typename T, int N, int SWIZZLE>
static inline vec_t<T,N> fract(vec_t<T,N,SWIZZLE> const &a)
{
    vec_t<T,N> ret;
    for (int i = 0; i < N; ++i)
        ret[i] = fract(a[i]);
    return ret;
}

template<typename T, int N, int SWIZZLE>
static inline vec_t<T,N> saturate(vec_t<T,N,SWIZZLE> const &a)
{
    vec_t<T,N> ret;
    for (int i = 0; i < N; ++i)
        ret[i] = saturate(a[i]);
    return ret;
}

template<typename T, int N, int SWIZZLE>
static inline vec_t<T,N> normalize(vec_t<T,N,SWIZZLE> const &a)
{
    T norm = T(length(a));
    return norm ? a / norm : vec_t<T,N>(T(0));
}

template<typename T, int N, int SWIZZLE>
static inline vec_t<T,N> abs(vec_t<T,N,SWIZZLE> const &a)
{
    vec_t<T,N> ret;
    for (int i = 0; i < N; ++i)
        ret[i] = lol::abs(a[i]);
    return ret;
}

template<typename T, int N, int SWIZZLE>
static inline vec_t<T,N> degrees(vec_t<T,N,SWIZZLE> const &a)
{
    vec_t<T,N> ret;
    for (int i = 0; i < N; ++i)
        ret[i] = lol::degrees(a[i]);
    return ret;
}

template<typename T, int N, int SWIZZLE>
static inline vec_t<T, N> radians(vec_t<T, N, SWIZZLE> const &a)
{
    vec_t<T, N> ret;
    for (int i = 0; i < N; ++i)
        ret[i] = lol::radians(a[i]);
    return ret;
}

// Cartesian operation consider that you're in spherical coordinate
template<typename T, int SWIZZLE>
static inline vec_t<T, 2> cartesian(vec_t<T, 2, SWIZZLE> const &a)
{
    vec_t<T, 2> ret;
    ret.x = a[0] * lol::cos(a[1]);
    ret.y = a[0] * lol::sin(a[1]);
    return ret;
}

template<typename T, int SWIZZLE>
static inline vec_t<T, 3> cartesian(vec_t<T, 3, SWIZZLE> const &a)
{
    vec_t<T, 3> ret;
    ret.x = a[0] * lol::sin(a[2]) * lol::cos(a[1]);
    ret.y = a[0] * lol::cos(a[2]);
    ret.z = a[0] * lol::sin(a[2]) * lol::sin(a[1]);
    return ret;
}

// Spherical operation consider that you're in cartesian coordinate
template<typename T, int SWIZZLE>
static inline vec_t<T, 2> spherical(vec_t<T, 2, SWIZZLE> const &a)
{
    vec_t<T, 2> ret;
    ret[0] = sqlength(a);
    ret[1] = lol::atan2(a.y, a.x);
    return ret;
}

template<typename T, int SWIZZLE>
static inline vec_t<T, 3> spherical(vec_t<T, 3, SWIZZLE> const &a)
{
    vec_t<T, 3> ret;
    ret[0] = sqlength(a);
    ret[1] = lol::atan(a.y / a.x);
    ret[2] = lol::acos(a.z / ret[0]);
    return ret;
}

/*
 * C++11 iterators
 */

template<typename T, int N, int SWIZZLE>
class vec_const_iter
{
public:
    inline vec_const_iter(vec_t<T,N,SWIZZLE> const &vec, int pos)
      : m_vec(vec),
        m_pos(pos)
    { }

    inline bool operator !=(vec_const_iter<T,N,SWIZZLE> const & that) const
    {
        return m_pos != that.m_pos;
    }

    template<int S = SWIZZLE>
    inline typename std::enable_if<S != FULL_SWIZZLE, T const &>::type
    operator *() const
    {
        return m_vec[m_pos];
    }

    template<int S = SWIZZLE>
    inline typename std::enable_if<S == FULL_SWIZZLE, T const &>::type
    operator *() const
    {
        return m_vec[m_pos];
    }

    inline vec_const_iter<T,N,SWIZZLE> & operator ++()
    {
        ++m_pos;
        return *this;
    }

private:
    vec_t<T,N,SWIZZLE> const &m_vec;
    int m_pos;
};

/* FIXME: do we need a non-const iterator? Looks almost useless to me,
 * and a lot of hassle with swizzling. */
template<typename T, int N, int SWIZZLE>
inline vec_const_iter<T,N,SWIZZLE> begin(vec_t<T,N,SWIZZLE> const &a)
{
    return vec_const_iter<T,N,SWIZZLE>(a, 0);
}

template<typename T, int N, int SWIZZLE>
inline vec_const_iter<T,N,SWIZZLE> end(vec_t<T,N,SWIZZLE> const &a)
{
    return vec_const_iter<T,N,SWIZZLE>(a, N);
}

/*
 * Constants
 */

template<typename T, int N>
vec_t<T,N> const vec_t<T,N>::zero = vec_t<T,N>(T(0));
template<typename T>
vec_t<T,2> const vec_t<T,2>::zero = vec_t<T,2>(T(0));
template<typename T>
vec_t<T,3> const vec_t<T,3>::zero = vec_t<T,3>(T(0));
template<typename T>
vec_t<T,4> const vec_t<T,4>::zero = vec_t<T,4>(T(0));

template<typename T>
vec_t<T,2> const vec_t<T,2>::axis_x = vec_t<T,2>(T(1), T(0));
template<typename T>
vec_t<T,2> const vec_t<T,2>::axis_y = vec_t<T,2>(T(0), T(1));

template<typename T>
vec_t<T,3> const vec_t<T,3>::axis_x = vec_t<T,3>(T(1), T(0), T(0));
template<typename T>
vec_t<T,3> const vec_t<T,3>::axis_y = vec_t<T,3>(T(0), T(1), T(0));
template<typename T>
vec_t<T,3> const vec_t<T,3>::axis_z = vec_t<T,3>(T(0), T(0), T(1));

template<typename T>
vec_t<T,4> const vec_t<T,4>::axis_x = vec_t<T,4>(T(1), T(0), T(0), T(0));
template<typename T>
vec_t<T,4> const vec_t<T,4>::axis_y = vec_t<T,4>(T(0), T(1), T(0), T(0));
template<typename T>
vec_t<T,4> const vec_t<T,4>::axis_z = vec_t<T,4>(T(0), T(0), T(1), T(0));
template<typename T>
vec_t<T,4> const vec_t<T,4>::axis_w = vec_t<T,4>(T(0), T(0), T(0), T(1));

#if !LOL_FEATURE_CXX11_CONSTEXPR
#undef constexpr
#endif

} /* namespace lol */

