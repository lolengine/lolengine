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
#include <lol/math/ops.h>

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

template<typename T, int N, int SWIZZLE>
struct vec_t
    /* MUST be a different base than e.g. vec_t<T,2> otherwise the unions
     * in vec_t<T,2> with the same base will cause empty base optimisation
     * failures. */
  : public swizzle_ops::base<T, SWIZZLE>
{
    static int const count = N;
    typedef T element;
    typedef vec_t<T,N> type;

    inline vec_t<T, N, SWIZZLE>& operator =(vec_t<T, N> that);

#if LOL_FEATURE_CXX11_RELAXED_UNIONS
    inline vec_t<T, N, SWIZZLE>& operator =(vec_t<T, N, SWIZZLE> const &that)
    {
        /* Pass by value in case this == &that */
        return *this = (vec_t<T,N>)that;
    }
#endif

    inline T& operator[](size_t n)
    {
        int i = (SWIZZLE >> (4 * (N - 1 - n))) & 3;
        return static_cast<T*>(static_cast<void*>(this))[i];
    }

    inline T const& operator[](size_t n) const
    {
        int i = (SWIZZLE >> (4 * (N - 1 - n))) & 3;
        return static_cast<T const*>(static_cast<void const *>(this))[i];
    }
};

/* The generic “vec_t” type, which is a fixed-size vector with no
 * swizzling. There's an override for N=2, N=3, N=4 that has swizzling. */
template<typename T, int N>
struct vec_t<T, N, FULL_SWIZZLE>
  : public linear_ops::base<T>,
    public componentwise_ops::base<T>
{
    static int const count = N;
    typedef T element;
    typedef vec_t<T,N> type;

    inline T& operator[](size_t n) { return m_data[n]; }
    inline T const& operator[](size_t n) const { return m_data[n]; }

private:
    T m_data[N];
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
    String tostring() const;

/*
 * 2-element vectors
 */

template <typename T>
struct vec_t<T,2>
  : public swizzle_ops::base<T>,
    public linear_ops::base<T>,
    public componentwise_ops::base<T>
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
      : x(v[0]), y(v[1]) {}

    /* Various explicit constructors */
    explicit inline constexpr vec_t(T X, T Y)
      : x(X), y(Y) {}
    explicit inline constexpr vec_t(T X)
      : x(X), y(X) {}

    LOL_COMMON_MEMBER_OPS(x)

    static const vec_t<T,2> zero;
    static const vec_t<T,2> axis_x;
    static const vec_t<T,2> axis_y;

    template<typename U>
    friend std::ostream &operator<<(std::ostream &stream, vec_t<U,2> const &v);

    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T s, t; };

#if !_DOXYGEN_SKIP_ME
        vec_t<T,2,0x00> const xx, rr, ss;
        vec_t<T,2,0x01> _____ xy, rg, st;
        vec_t<T,2,0x10> _____ yx, gr, ts;
        vec_t<T,2,0x11> const yy, gg, tt;

        vec_t<T,3,0x000> const xxx, rrr, sss;
        vec_t<T,3,0x001> const xxy, rrg, sst;
        vec_t<T,3,0x010> const xyx, rgr, sts;
        vec_t<T,3,0x011> const xyy, rgg, stt;
        vec_t<T,3,0x100> const yxx, grr, tss;
        vec_t<T,3,0x101> const yxy, grg, tst;
        vec_t<T,3,0x110> const yyx, ggr, tts;
        vec_t<T,3,0x111> const yyy, ggg, ttt;

        vec_t<T,4,0x0000> const xxxx, rrrr, ssss;
        vec_t<T,4,0x0001> const xxxy, rrrg, ssst;
        vec_t<T,4,0x0010> const xxyx, rrgr, ssts;
        vec_t<T,4,0x0011> const xxyy, rrgg, sstt;
        vec_t<T,4,0x0100> const xyxx, rgrr, stss;
        vec_t<T,4,0x0101> const xyxy, rgrg, stst;
        vec_t<T,4,0x0110> const xyyx, rggr, stts;
        vec_t<T,4,0x0111> const xyyy, rggg, sttt;
        vec_t<T,4,0x1000> const yxxx, grrr, tsss;
        vec_t<T,4,0x1001> const yxxy, grrg, tsst;
        vec_t<T,4,0x1010> const yxyx, grgr, tsts;
        vec_t<T,4,0x1011> const yxyy, grgg, tstt;
        vec_t<T,4,0x1100> const yyxx, ggrr, ttss;
        vec_t<T,4,0x1101> const yyxy, ggrg, ttst;
        vec_t<T,4,0x1110> const yyyx, gggr, ttts;
        vec_t<T,4,0x1111> const yyyy, gggg, tttt;
#endif
    };
};

static_assert(sizeof(i8vec2) == 2, "sizeof(i8vec2) == 2");
static_assert(sizeof(i16vec2) == 4, "sizeof(i16vec2) == 4");
static_assert(sizeof(ivec2) == 8, "sizeof(ivec2) == 8");
static_assert(sizeof(i64vec2) == 16, "sizeof(i64vec2) == 16");

static_assert(sizeof(f16vec2) == 4, "sizeof(f16vec2) == 4");
static_assert(sizeof(vec2) == 8, "sizeof(vec2) == 8");
static_assert(sizeof(dvec2) == 16, "sizeof(dvec2) == 16");

/*
 * 3-element vectors
 */

template <typename T>
struct vec_t<T,3>
  : public swizzle_ops::base<T>,
    public linear_ops::base<T>,
    public componentwise_ops::base<T>
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
      : x(v[0]), y(v[1]), z(v[2]) {}

    /* Various explicit constructors */
    explicit inline constexpr vec_t(T X)
      : x(X), y(X), z(X) {}
    explicit inline constexpr vec_t(T X, T Y, T Z)
      : x(X), y(Y), z(Z) {}
    explicit inline constexpr vec_t(vec_t<T,2> XY, T Z)
      : x(XY.x), y(XY.y), z(Z) {}
    explicit inline constexpr vec_t(T X, vec_t<T,2> YZ)
      : x(X), y(YZ.x), z(YZ.y) {}

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
             ? type(-a.y, a.x, (T)0)
             : type((T)0, -a.z, a.y);
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
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { T s, t, p; };

#if !_DOXYGEN_SKIP_ME
        vec_t<T,2,0x00> const xx, rr, ss;
        vec_t<T,2,0x01> _____ xy, rg, st;
        vec_t<T,2,0x02> _____ xz, rb, sp;
        vec_t<T,2,0x10> _____ yx, gr, ts;
        vec_t<T,2,0x11> const yy, gg, tt;
        vec_t<T,2,0x12> _____ yz, gb, tp;
        vec_t<T,2,0x20> _____ zx, br, ps;
        vec_t<T,2,0x21> _____ zy, bg, pt;
        vec_t<T,2,0x22> const zz, bb, pp;

        vec_t<T,3,0x000> const xxx, rrr, sss;
        vec_t<T,3,0x001> const xxy, rrg, sst;
        vec_t<T,3,0x002> const xxz, rrb, ssp;
        vec_t<T,3,0x010> const xyx, rgr, sts;
        vec_t<T,3,0x011> const xyy, rgg, stt;
        vec_t<T,3,0x012> _____ xyz, rgb, stp;
        vec_t<T,3,0x020> const xzx, rbr, sps;
        vec_t<T,3,0x021> _____ xzy, rbg, spt;
        vec_t<T,3,0x022> const xzz, rbb, spp;
        vec_t<T,3,0x100> const yxx, grr, tss;
        vec_t<T,3,0x101> const yxy, grg, tst;
        vec_t<T,3,0x102> _____ yxz, grb, tsp;
        vec_t<T,3,0x110> const yyx, ggr, tts;
        vec_t<T,3,0x111> const yyy, ggg, ttt;
        vec_t<T,3,0x112> const yyz, ggb, ttp;
        vec_t<T,3,0x120> _____ yzx, gbr, tps;
        vec_t<T,3,0x121> const yzy, gbg, tpt;
        vec_t<T,3,0x122> const yzz, gbb, tpp;
        vec_t<T,3,0x200> const zxx, brr, pss;
        vec_t<T,3,0x201> _____ zxy, brg, pst;
        vec_t<T,3,0x202> const zxz, brb, psp;
        vec_t<T,3,0x210> _____ zyx, bgr, pts;
        vec_t<T,3,0x211> const zyy, bgg, ptt;
        vec_t<T,3,0x212> const zyz, bgb, ptp;
        vec_t<T,3,0x220> const zzx, bbr, pps;
        vec_t<T,3,0x221> const zzy, bbg, ppt;
        vec_t<T,3,0x222> const zzz, bbb, ppp;

        vec_t<T,4,0x0000> const xxxx, rrrr, ssss;
        vec_t<T,4,0x0001> const xxxy, rrrg, ssst;
        vec_t<T,4,0x0002> const xxxz, rrrb, sssp;
        vec_t<T,4,0x0010> const xxyx, rrgr, ssts;
        vec_t<T,4,0x0011> const xxyy, rrgg, sstt;
        vec_t<T,4,0x0012> const xxyz, rrgb, sstp;
        vec_t<T,4,0x0020> const xxzx, rrbr, ssps;
        vec_t<T,4,0x0021> const xxzy, rrbg, sspt;
        vec_t<T,4,0x0022> const xxzz, rrbb, sspp;
        vec_t<T,4,0x0100> const xyxx, rgrr, stss;
        vec_t<T,4,0x0101> const xyxy, rgrg, stst;
        vec_t<T,4,0x0102> const xyxz, rgrb, stsp;
        vec_t<T,4,0x0110> const xyyx, rggr, stts;
        vec_t<T,4,0x0111> const xyyy, rggg, sttt;
        vec_t<T,4,0x0112> const xyyz, rggb, sttp;
        vec_t<T,4,0x0120> const xyzx, rgbr, stps;
        vec_t<T,4,0x0121> const xyzy, rgbg, stpt;
        vec_t<T,4,0x0122> const xyzz, rgbb, stpp;
        vec_t<T,4,0x0200> const xzxx, rbrr, spss;
        vec_t<T,4,0x0201> const xzxy, rbrg, spst;
        vec_t<T,4,0x0202> const xzxz, rbrb, spsp;
        vec_t<T,4,0x0210> const xzyx, rbgr, spts;
        vec_t<T,4,0x0211> const xzyy, rbgg, sptt;
        vec_t<T,4,0x0212> const xzyz, rbgb, sptp;
        vec_t<T,4,0x0220> const xzzx, rbbr, spps;
        vec_t<T,4,0x0221> const xzzy, rbbg, sppt;
        vec_t<T,4,0x0222> const xzzz, rbbb, sppp;
        vec_t<T,4,0x1000> const yxxx, grrr, tsss;
        vec_t<T,4,0x1001> const yxxy, grrg, tsst;
        vec_t<T,4,0x1002> const yxxz, grrb, tssp;
        vec_t<T,4,0x1010> const yxyx, grgr, tsts;
        vec_t<T,4,0x1011> const yxyy, grgg, tstt;
        vec_t<T,4,0x1012> const yxyz, grgb, tstp;
        vec_t<T,4,0x1020> const yxzx, grbr, tsps;
        vec_t<T,4,0x1021> const yxzy, grbg, tspt;
        vec_t<T,4,0x1022> const yxzz, grbb, tspp;
        vec_t<T,4,0x1100> const yyxx, ggrr, ttss;
        vec_t<T,4,0x1101> const yyxy, ggrg, ttst;
        vec_t<T,4,0x1102> const yyxz, ggrb, ttsp;
        vec_t<T,4,0x1110> const yyyx, gggr, ttts;
        vec_t<T,4,0x1111> const yyyy, gggg, tttt;
        vec_t<T,4,0x1112> const yyyz, gggb, tttp;
        vec_t<T,4,0x1120> const yyzx, ggbr, ttps;
        vec_t<T,4,0x1121> const yyzy, ggbg, ttpt;
        vec_t<T,4,0x1122> const yyzz, ggbb, ttpp;
        vec_t<T,4,0x1200> const yzxx, gbrr, tpss;
        vec_t<T,4,0x1201> const yzxy, gbrg, tpst;
        vec_t<T,4,0x1202> const yzxz, gbrb, tpsp;
        vec_t<T,4,0x1210> const yzyx, gbgr, tpts;
        vec_t<T,4,0x1211> const yzyy, gbgg, tptt;
        vec_t<T,4,0x1212> const yzyz, gbgb, tptp;
        vec_t<T,4,0x1220> const yzzx, gbbr, tpps;
        vec_t<T,4,0x1221> const yzzy, gbbg, tppt;
        vec_t<T,4,0x1222> const yzzz, gbbb, tppp;
        vec_t<T,4,0x2000> const zxxx, brrr, psss;
        vec_t<T,4,0x2001> const zxxy, brrg, psst;
        vec_t<T,4,0x2002> const zxxz, brrb, pssp;
        vec_t<T,4,0x2010> const zxyx, brgr, psts;
        vec_t<T,4,0x2011> const zxyy, brgg, pstt;
        vec_t<T,4,0x2012> const zxyz, brgb, pstp;
        vec_t<T,4,0x2020> const zxzx, brbr, psps;
        vec_t<T,4,0x2021> const zxzy, brbg, pspt;
        vec_t<T,4,0x2022> const zxzz, brbb, pspp;
        vec_t<T,4,0x2100> const zyxx, bgrr, ptss;
        vec_t<T,4,0x2101> const zyxy, bgrg, ptst;
        vec_t<T,4,0x2102> const zyxz, bgrb, ptsp;
        vec_t<T,4,0x2110> const zyyx, bggr, ptts;
        vec_t<T,4,0x2111> const zyyy, bggg, pttt;
        vec_t<T,4,0x2112> const zyyz, bggb, pttp;
        vec_t<T,4,0x2120> const zyzx, bgbr, ptps;
        vec_t<T,4,0x2121> const zyzy, bgbg, ptpt;
        vec_t<T,4,0x2122> const zyzz, bgbb, ptpp;
        vec_t<T,4,0x2200> const zzxx, bbrr, ppss;
        vec_t<T,4,0x2201> const zzxy, bbrg, ppst;
        vec_t<T,4,0x2202> const zzxz, bbrb, ppsp;
        vec_t<T,4,0x2210> const zzyx, bbgr, ppts;
        vec_t<T,4,0x2211> const zzyy, bbgg, pptt;
        vec_t<T,4,0x2212> const zzyz, bbgb, pptp;
        vec_t<T,4,0x2220> const zzzx, bbbr, ppps;
        vec_t<T,4,0x2221> const zzzy, bbbg, pppt;
        vec_t<T,4,0x2222> const zzzz, bbbb, pppp;
#endif
    };
};

static_assert(sizeof(i8vec3) == 3, "sizeof(i8vec3) == 3");
static_assert(sizeof(i16vec3) == 6, "sizeof(i16vec3) == 6");
static_assert(sizeof(ivec3) == 12, "sizeof(ivec3) == 12");
static_assert(sizeof(i64vec3) == 24, "sizeof(i64vec3) == 24");

static_assert(sizeof(f16vec3) == 6, "sizeof(f16vec3) == 6");
static_assert(sizeof(vec3) == 12, "sizeof(vec3) == 12");
static_assert(sizeof(dvec3) == 24, "sizeof(dvec3) == 24");

/*
 * 4-element vectors
 */

template <typename T>
struct vec_t<T,4>
  : public swizzle_ops::base<T>,
    public linear_ops::base<T>,
    public componentwise_ops::base<T>
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
      : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

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
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T s, t, p, q; };

#if !_DOXYGEN_SKIP_ME
        vec_t<T,2,0x00> const xx, rr, ss;
        vec_t<T,2,0x01> _____ xy, rg, st;
        vec_t<T,2,0x02> _____ xz, rb, sp;
        vec_t<T,2,0x03> _____ xw, ra, sq;
        vec_t<T,2,0x10> _____ yx, gr, ts;
        vec_t<T,2,0x11> const yy, gg, tt;
        vec_t<T,2,0x12> _____ yz, gb, tp;
        vec_t<T,2,0x13> _____ yw, ga, tq;
        vec_t<T,2,0x20> _____ zx, br, ps;
        vec_t<T,2,0x21> _____ zy, bg, pt;
        vec_t<T,2,0x22> const zz, bb, pp;
        vec_t<T,2,0x23> _____ zw, ba, pq;
        vec_t<T,2,0x30> _____ wx, ar, qs;
        vec_t<T,2,0x31> _____ wy, ag, qt;
        vec_t<T,2,0x32> _____ wz, ab, qp;
        vec_t<T,2,0x33> const ww, aa, qq;

        vec_t<T,3,0x000> const xxx, rrr, sss;
        vec_t<T,3,0x001> const xxy, rrg, sst;
        vec_t<T,3,0x002> const xxz, rrb, ssp;
        vec_t<T,3,0x003> const xxw, rra, ssq;
        vec_t<T,3,0x010> const xyx, rgr, sts;
        vec_t<T,3,0x011> const xyy, rgg, stt;
        vec_t<T,3,0x012> _____ xyz, rgb, stp;
        vec_t<T,3,0x013> _____ xyw, rga, stq;
        vec_t<T,3,0x020> const xzx, rbr, sps;
        vec_t<T,3,0x021> _____ xzy, rbg, spt;
        vec_t<T,3,0x022> const xzz, rbb, spp;
        vec_t<T,3,0x023> _____ xzw, rba, spq;
        vec_t<T,3,0x030> const xwx, rar, sqs;
        vec_t<T,3,0x031> _____ xwy, rag, sqt;
        vec_t<T,3,0x032> _____ xwz, rab, sqp;
        vec_t<T,3,0x033> const xww, raa, sqq;
        vec_t<T,3,0x100> const yxx, grr, tss;
        vec_t<T,3,0x101> const yxy, grg, tst;
        vec_t<T,3,0x102> _____ yxz, grb, tsp;
        vec_t<T,3,0x103> _____ yxw, gra, tsq;
        vec_t<T,3,0x110> const yyx, ggr, tts;
        vec_t<T,3,0x111> const yyy, ggg, ttt;
        vec_t<T,3,0x112> const yyz, ggb, ttp;
        vec_t<T,3,0x113> const yyw, gga, ttq;
        vec_t<T,3,0x120> _____ yzx, gbr, tps;
        vec_t<T,3,0x121> const yzy, gbg, tpt;
        vec_t<T,3,0x122> const yzz, gbb, tpp;
        vec_t<T,3,0x123> _____ yzw, gba, tpq;
        vec_t<T,3,0x130> _____ ywx, gar, tqs;
        vec_t<T,3,0x131> const ywy, gag, tqt;
        vec_t<T,3,0x132> _____ ywz, gab, tqp;
        vec_t<T,3,0x133> const yww, gaa, tqq;
        vec_t<T,3,0x200> const zxx, brr, pss;
        vec_t<T,3,0x201> _____ zxy, brg, pst;
        vec_t<T,3,0x202> const zxz, brb, psp;
        vec_t<T,3,0x203> _____ zxw, bra, psq;
        vec_t<T,3,0x210> _____ zyx, bgr, pts;
        vec_t<T,3,0x211> const zyy, bgg, ptt;
        vec_t<T,3,0x212> const zyz, bgb, ptp;
        vec_t<T,3,0x213> _____ zyw, bga, ptq;
        vec_t<T,3,0x220> const zzx, bbr, pps;
        vec_t<T,3,0x221> const zzy, bbg, ppt;
        vec_t<T,3,0x222> const zzz, bbb, ppp;
        vec_t<T,3,0x223> const zzw, bba, ppq;
        vec_t<T,3,0x230> _____ zwx, bar, pqs;
        vec_t<T,3,0x231> _____ zwy, bag, pqt;
        vec_t<T,3,0x232> const zwz, bab, pqp;
        vec_t<T,3,0x233> const zww, baa, pqq;
        vec_t<T,3,0x300> const wxx, arr, qss;
        vec_t<T,3,0x301> _____ wxy, arg, qst;
        vec_t<T,3,0x302> _____ wxz, arb, qsp;
        vec_t<T,3,0x303> const wxw, ara, qsq;
        vec_t<T,3,0x310> _____ wyx, agr, qts;
        vec_t<T,3,0x311> const wyy, agg, qtt;
        vec_t<T,3,0x312> _____ wyz, agb, qtp;
        vec_t<T,3,0x313> const wyw, aga, qtq;
        vec_t<T,3,0x320> _____ wzx, abr, qps;
        vec_t<T,3,0x321> _____ wzy, abg, qpt;
        vec_t<T,3,0x322> const wzz, abb, qpp;
        vec_t<T,3,0x323> const wzw, aba, qpq;
        vec_t<T,3,0x330> const wwx, aar, qqs;
        vec_t<T,3,0x331> const wwy, aag, qqt;
        vec_t<T,3,0x332> const wwz, aab, qqp;
        vec_t<T,3,0x333> const www, aaa, qqq;

        vec_t<T,4,0x0000> const xxxx, rrrr, ssss;
        vec_t<T,4,0x0001> const xxxy, rrrg, ssst;
        vec_t<T,4,0x0002> const xxxz, rrrb, sssp;
        vec_t<T,4,0x0003> const xxxw, rrra, sssq;
        vec_t<T,4,0x0010> const xxyx, rrgr, ssts;
        vec_t<T,4,0x0011> const xxyy, rrgg, sstt;
        vec_t<T,4,0x0012> const xxyz, rrgb, sstp;
        vec_t<T,4,0x0013> const xxyw, rrga, sstq;
        vec_t<T,4,0x0020> const xxzx, rrbr, ssps;
        vec_t<T,4,0x0021> const xxzy, rrbg, sspt;
        vec_t<T,4,0x0022> const xxzz, rrbb, sspp;
        vec_t<T,4,0x0023> const xxzw, rrba, sspq;
        vec_t<T,4,0x0030> const xxwx, rrar, ssqs;
        vec_t<T,4,0x0031> const xxwy, rrag, ssqt;
        vec_t<T,4,0x0032> const xxwz, rrab, ssqp;
        vec_t<T,4,0x0033> const xxww, rraa, ssqq;
        vec_t<T,4,0x0100> const xyxx, rgrr, stss;
        vec_t<T,4,0x0101> const xyxy, rgrg, stst;
        vec_t<T,4,0x0102> const xyxz, rgrb, stsp;
        vec_t<T,4,0x0103> const xyxw, rgra, stsq;
        vec_t<T,4,0x0110> const xyyx, rggr, stts;
        vec_t<T,4,0x0111> const xyyy, rggg, sttt;
        vec_t<T,4,0x0112> const xyyz, rggb, sttp;
        vec_t<T,4,0x0113> const xyyw, rgga, sttq;
        vec_t<T,4,0x0120> const xyzx, rgbr, stps;
        vec_t<T,4,0x0121> const xyzy, rgbg, stpt;
        vec_t<T,4,0x0122> const xyzz, rgbb, stpp;
        vec_t<T,4,0x0123> _____ xyzw, rgba, stpq;
        vec_t<T,4,0x0130> const xywx, rgar, stqs;
        vec_t<T,4,0x0131> const xywy, rgag, stqt;
        vec_t<T,4,0x0132> _____ xywz, rgab, stqp;
        vec_t<T,4,0x0133> const xyww, rgaa, stqq;
        vec_t<T,4,0x0200> const xzxx, rbrr, spss;
        vec_t<T,4,0x0201> const xzxy, rbrg, spst;
        vec_t<T,4,0x0202> const xzxz, rbrb, spsp;
        vec_t<T,4,0x0203> const xzxw, rbra, spsq;
        vec_t<T,4,0x0210> const xzyx, rbgr, spts;
        vec_t<T,4,0x0211> const xzyy, rbgg, sptt;
        vec_t<T,4,0x0212> const xzyz, rbgb, sptp;
        vec_t<T,4,0x0213> _____ xzyw, rbga, sptq;
        vec_t<T,4,0x0220> const xzzx, rbbr, spps;
        vec_t<T,4,0x0221> const xzzy, rbbg, sppt;
        vec_t<T,4,0x0222> const xzzz, rbbb, sppp;
        vec_t<T,4,0x0223> const xzzw, rbba, sppq;
        vec_t<T,4,0x0230> const xzwx, rbar, spqs;
        vec_t<T,4,0x0231> _____ xzwy, rbag, spqt;
        vec_t<T,4,0x0232> const xzwz, rbab, spqp;
        vec_t<T,4,0x0233> const xzww, rbaa, spqq;
        vec_t<T,4,0x0300> const xwxx, rarr, sqss;
        vec_t<T,4,0x0301> const xwxy, rarg, sqst;
        vec_t<T,4,0x0302> const xwxz, rarb, sqsp;
        vec_t<T,4,0x0303> const xwxw, rara, sqsq;
        vec_t<T,4,0x0310> const xwyx, ragr, sqts;
        vec_t<T,4,0x0311> const xwyy, ragg, sqtt;
        vec_t<T,4,0x0312> _____ xwyz, ragb, sqtp;
        vec_t<T,4,0x0313> const xwyw, raga, sqtq;
        vec_t<T,4,0x0320> const xwzx, rabr, sqps;
        vec_t<T,4,0x0321> _____ xwzy, rabg, sqpt;
        vec_t<T,4,0x0322> const xwzz, rabb, sqpp;
        vec_t<T,4,0x0323> const xwzw, raba, sqpq;
        vec_t<T,4,0x0330> const xwwx, raar, sqqs;
        vec_t<T,4,0x0331> const xwwy, raag, sqqt;
        vec_t<T,4,0x0332> const xwwz, raab, sqqp;
        vec_t<T,4,0x0333> const xwww, raaa, sqqq;
        vec_t<T,4,0x1000> const yxxx, grrr, tsss;
        vec_t<T,4,0x1001> const yxxy, grrg, tsst;
        vec_t<T,4,0x1002> const yxxz, grrb, tssp;
        vec_t<T,4,0x1003> const yxxw, grra, tssq;
        vec_t<T,4,0x1010> const yxyx, grgr, tsts;
        vec_t<T,4,0x1011> const yxyy, grgg, tstt;
        vec_t<T,4,0x1012> const yxyz, grgb, tstp;
        vec_t<T,4,0x1013> const yxyw, grga, tstq;
        vec_t<T,4,0x1020> const yxzx, grbr, tsps;
        vec_t<T,4,0x1021> const yxzy, grbg, tspt;
        vec_t<T,4,0x1022> const yxzz, grbb, tspp;
        vec_t<T,4,0x1023> _____ yxzw, grba, tspq;
        vec_t<T,4,0x1030> const yxwx, grar, tsqs;
        vec_t<T,4,0x1031> const yxwy, grag, tsqt;
        vec_t<T,4,0x1032> _____ yxwz, grab, tsqp;
        vec_t<T,4,0x1033> const yxww, graa, tsqq;
        vec_t<T,4,0x1100> const yyxx, ggrr, ttss;
        vec_t<T,4,0x1101> const yyxy, ggrg, ttst;
        vec_t<T,4,0x1102> const yyxz, ggrb, ttsp;
        vec_t<T,4,0x1103> const yyxw, ggra, ttsq;
        vec_t<T,4,0x1110> const yyyx, gggr, ttts;
        vec_t<T,4,0x1111> const yyyy, gggg, tttt;
        vec_t<T,4,0x1112> const yyyz, gggb, tttp;
        vec_t<T,4,0x1113> const yyyw, ggga, tttq;
        vec_t<T,4,0x1120> const yyzx, ggbr, ttps;
        vec_t<T,4,0x1121> const yyzy, ggbg, ttpt;
        vec_t<T,4,0x1122> const yyzz, ggbb, ttpp;
        vec_t<T,4,0x1123> const yyzw, ggba, ttpq;
        vec_t<T,4,0x1130> const yywx, ggar, ttqs;
        vec_t<T,4,0x1131> const yywy, ggag, ttqt;
        vec_t<T,4,0x1132> const yywz, ggab, ttqp;
        vec_t<T,4,0x1133> const yyww, ggaa, ttqq;
        vec_t<T,4,0x1200> const yzxx, gbrr, tpss;
        vec_t<T,4,0x1201> const yzxy, gbrg, tpst;
        vec_t<T,4,0x1202> const yzxz, gbrb, tpsp;
        vec_t<T,4,0x1203> _____ yzxw, gbra, tpsq;
        vec_t<T,4,0x1210> const yzyx, gbgr, tpts;
        vec_t<T,4,0x1211> const yzyy, gbgg, tptt;
        vec_t<T,4,0x1212> const yzyz, gbgb, tptp;
        vec_t<T,4,0x1213> const yzyw, gbga, tptq;
        vec_t<T,4,0x1220> const yzzx, gbbr, tpps;
        vec_t<T,4,0x1221> const yzzy, gbbg, tppt;
        vec_t<T,4,0x1222> const yzzz, gbbb, tppp;
        vec_t<T,4,0x1223> const yzzw, gbba, tppq;
        vec_t<T,4,0x1230> _____ yzwx, gbar, tpqs;
        vec_t<T,4,0x1231> const yzwy, gbag, tpqt;
        vec_t<T,4,0x1232> const yzwz, gbab, tpqp;
        vec_t<T,4,0x1233> const yzww, gbaa, tpqq;
        vec_t<T,4,0x1300> const ywxx, garr, tqss;
        vec_t<T,4,0x1301> const ywxy, garg, tqst;
        vec_t<T,4,0x1302> _____ ywxz, garb, tqsp;
        vec_t<T,4,0x1303> const ywxw, gara, tqsq;
        vec_t<T,4,0x1310> const ywyx, gagr, tqts;
        vec_t<T,4,0x1311> const ywyy, gagg, tqtt;
        vec_t<T,4,0x1312> const ywyz, gagb, tqtp;
        vec_t<T,4,0x1313> const ywyw, gaga, tqtq;
        vec_t<T,4,0x1320> _____ ywzx, gabr, tqps;
        vec_t<T,4,0x1321> const ywzy, gabg, tqpt;
        vec_t<T,4,0x1322> const ywzz, gabb, tqpp;
        vec_t<T,4,0x1323> const ywzw, gaba, tqpq;
        vec_t<T,4,0x1330> const ywwx, gaar, tqqs;
        vec_t<T,4,0x1331> const ywwy, gaag, tqqt;
        vec_t<T,4,0x1332> const ywwz, gaab, tqqp;
        vec_t<T,4,0x1333> const ywww, gaaa, tqqq;
        vec_t<T,4,0x2000> const zxxx, brrr, psss;
        vec_t<T,4,0x2001> const zxxy, brrg, psst;
        vec_t<T,4,0x2002> const zxxz, brrb, pssp;
        vec_t<T,4,0x2003> const zxxw, brra, pssq;
        vec_t<T,4,0x2010> const zxyx, brgr, psts;
        vec_t<T,4,0x2011> const zxyy, brgg, pstt;
        vec_t<T,4,0x2012> const zxyz, brgb, pstp;
        vec_t<T,4,0x2013> _____ zxyw, brga, pstq;
        vec_t<T,4,0x2020> const zxzx, brbr, psps;
        vec_t<T,4,0x2021> const zxzy, brbg, pspt;
        vec_t<T,4,0x2022> const zxzz, brbb, pspp;
        vec_t<T,4,0x2023> const zxzw, brba, pspq;
        vec_t<T,4,0x2030> const zxwx, brar, psqs;
        vec_t<T,4,0x2031> _____ zxwy, brag, psqt;
        vec_t<T,4,0x2032> const zxwz, brab, psqp;
        vec_t<T,4,0x2033> const zxww, braa, psqq;
        vec_t<T,4,0x2100> const zyxx, bgrr, ptss;
        vec_t<T,4,0x2101> const zyxy, bgrg, ptst;
        vec_t<T,4,0x2102> const zyxz, bgrb, ptsp;
        vec_t<T,4,0x2103> _____ zyxw, bgra, ptsq;
        vec_t<T,4,0x2110> const zyyx, bggr, ptts;
        vec_t<T,4,0x2111> const zyyy, bggg, pttt;
        vec_t<T,4,0x2112> const zyyz, bggb, pttp;
        vec_t<T,4,0x2113> const zyyw, bgga, pttq;
        vec_t<T,4,0x2120> const zyzx, bgbr, ptps;
        vec_t<T,4,0x2121> const zyzy, bgbg, ptpt;
        vec_t<T,4,0x2122> const zyzz, bgbb, ptpp;
        vec_t<T,4,0x2123> const zyzw, bgba, ptpq;
        vec_t<T,4,0x2130> _____ zywx, bgar, ptqs;
        vec_t<T,4,0x2131> const zywy, bgag, ptqt;
        vec_t<T,4,0x2132> const zywz, bgab, ptqp;
        vec_t<T,4,0x2133> const zyww, bgaa, ptqq;
        vec_t<T,4,0x2200> const zzxx, bbrr, ppss;
        vec_t<T,4,0x2201> const zzxy, bbrg, ppst;
        vec_t<T,4,0x2202> const zzxz, bbrb, ppsp;
        vec_t<T,4,0x2203> const zzxw, bbra, ppsq;
        vec_t<T,4,0x2210> const zzyx, bbgr, ppts;
        vec_t<T,4,0x2211> const zzyy, bbgg, pptt;
        vec_t<T,4,0x2212> const zzyz, bbgb, pptp;
        vec_t<T,4,0x2213> const zzyw, bbga, pptq;
        vec_t<T,4,0x2220> const zzzx, bbbr, ppps;
        vec_t<T,4,0x2221> const zzzy, bbbg, pppt;
        vec_t<T,4,0x2222> const zzzz, bbbb, pppp;
        vec_t<T,4,0x2223> const zzzw, bbba, pppq;
        vec_t<T,4,0x2230> const zzwx, bbar, ppqs;
        vec_t<T,4,0x2231> const zzwy, bbag, ppqt;
        vec_t<T,4,0x2232> const zzwz, bbab, ppqp;
        vec_t<T,4,0x2233> const zzww, bbaa, ppqq;
        vec_t<T,4,0x2300> const zwxx, barr, pqss;
        vec_t<T,4,0x2301> _____ zwxy, barg, pqst;
        vec_t<T,4,0x2302> const zwxz, barb, pqsp;
        vec_t<T,4,0x2303> const zwxw, bara, pqsq;
        vec_t<T,4,0x2310> _____ zwyx, bagr, pqts;
        vec_t<T,4,0x2311> const zwyy, bagg, pqtt;
        vec_t<T,4,0x2312> const zwyz, bagb, pqtp;
        vec_t<T,4,0x2313> const zwyw, baga, pqtq;
        vec_t<T,4,0x2320> const zwzx, babr, pqps;
        vec_t<T,4,0x2321> const zwzy, babg, pqpt;
        vec_t<T,4,0x2322> const zwzz, babb, pqpp;
        vec_t<T,4,0x2323> const zwzw, baba, pqpq;
        vec_t<T,4,0x2330> const zwwx, baar, pqqs;
        vec_t<T,4,0x2331> const zwwy, baag, pqqt;
        vec_t<T,4,0x2332> const zwwz, baab, pqqp;
        vec_t<T,4,0x2333> const zwww, baaa, pqqq;
        vec_t<T,4,0x3000> const wxxx, arrr, qsss;
        vec_t<T,4,0x3001> const wxxy, arrg, qsst;
        vec_t<T,4,0x3002> const wxxz, arrb, qssp;
        vec_t<T,4,0x3003> const wxxw, arra, qssq;
        vec_t<T,4,0x3010> const wxyx, argr, qsts;
        vec_t<T,4,0x3011> const wxyy, argg, qstt;
        vec_t<T,4,0x3012> _____ wxyz, argb, qstp;
        vec_t<T,4,0x3013> const wxyw, arga, qstq;
        vec_t<T,4,0x3020> const wxzx, arbr, qsps;
        vec_t<T,4,0x3021> _____ wxzy, arbg, qspt;
        vec_t<T,4,0x3022> const wxzz, arbb, qspp;
        vec_t<T,4,0x3023> const wxzw, arba, qspq;
        vec_t<T,4,0x3030> const wxwx, arar, qsqs;
        vec_t<T,4,0x3031> const wxwy, arag, qsqt;
        vec_t<T,4,0x3032> const wxwz, arab, qsqp;
        vec_t<T,4,0x3033> const wxww, araa, qsqq;
        vec_t<T,4,0x3100> const wyxx, agrr, qtss;
        vec_t<T,4,0x3101> const wyxy, agrg, qtst;
        vec_t<T,4,0x3102> _____ wyxz, agrb, qtsp;
        vec_t<T,4,0x3103> const wyxw, agra, qtsq;
        vec_t<T,4,0x3110> const wyyx, aggr, qtts;
        vec_t<T,4,0x3111> const wyyy, aggg, qttt;
        vec_t<T,4,0x3112> const wyyz, aggb, qttp;
        vec_t<T,4,0x3113> const wyyw, agga, qttq;
        vec_t<T,4,0x3120> _____ wyzx, agbr, qtps;
        vec_t<T,4,0x3121> const wyzy, agbg, qtpt;
        vec_t<T,4,0x3122> const wyzz, agbb, qtpp;
        vec_t<T,4,0x3123> const wyzw, agba, qtpq;
        vec_t<T,4,0x3130> const wywx, agar, qtqs;
        vec_t<T,4,0x3131> const wywy, agag, qtqt;
        vec_t<T,4,0x3132> const wywz, agab, qtqp;
        vec_t<T,4,0x3133> const wyww, agaa, qtqq;
        vec_t<T,4,0x3200> const wzxx, abrr, qpss;
        vec_t<T,4,0x3201> _____ wzxy, abrg, qpst;
        vec_t<T,4,0x3202> const wzxz, abrb, qpsp;
        vec_t<T,4,0x3203> const wzxw, abra, qpsq;
        vec_t<T,4,0x3210> _____ wzyx, abgr, qpts;
        vec_t<T,4,0x3211> const wzyy, abgg, qptt;
        vec_t<T,4,0x3212> const wzyz, abgb, qptp;
        vec_t<T,4,0x3213> const wzyw, abga, qptq;
        vec_t<T,4,0x3220> const wzzx, abbr, qpps;
        vec_t<T,4,0x3221> const wzzy, abbg, qppt;
        vec_t<T,4,0x3222> const wzzz, abbb, qppp;
        vec_t<T,4,0x3223> const wzzw, abba, qppq;
        vec_t<T,4,0x3230> const wzwx, abar, qpqs;
        vec_t<T,4,0x3231> const wzwy, abag, qpqt;
        vec_t<T,4,0x3232> const wzwz, abab, qpqp;
        vec_t<T,4,0x3233> const wzww, abaa, qpqq;
        vec_t<T,4,0x3300> const wwxx, aarr, qqss;
        vec_t<T,4,0x3301> const wwxy, aarg, qqst;
        vec_t<T,4,0x3302> const wwxz, aarb, qqsp;
        vec_t<T,4,0x3303> const wwxw, aara, qqsq;
        vec_t<T,4,0x3310> const wwyx, aagr, qqts;
        vec_t<T,4,0x3311> const wwyy, aagg, qqtt;
        vec_t<T,4,0x3312> const wwyz, aagb, qqtp;
        vec_t<T,4,0x3313> const wwyw, aaga, qqtq;
        vec_t<T,4,0x3320> const wwzx, aabr, qqps;
        vec_t<T,4,0x3321> const wwzy, aabg, qqpt;
        vec_t<T,4,0x3322> const wwzz, aabb, qqpp;
        vec_t<T,4,0x3323> const wwzw, aaba, qqpq;
        vec_t<T,4,0x3330> const wwwx, aaar, qqqs;
        vec_t<T,4,0x3331> const wwwy, aaag, qqqt;
        vec_t<T,4,0x3332> const wwwz, aaab, qqqp;
        vec_t<T,4,0x3333> const wwww, aaaa, qqqq;
#endif
    };
};

static_assert(sizeof(i8vec4) == 4, "sizeof(i8vec4) == 4");
static_assert(sizeof(i16vec4) == 8, "sizeof(i16vec4) == 8");
static_assert(sizeof(ivec4) == 16, "sizeof(ivec4) == 16");
static_assert(sizeof(i64vec4) == 32, "sizeof(i64vec4) == 32");

static_assert(sizeof(f16vec4) == 8, "sizeof(f16vec4) == 8");
static_assert(sizeof(vec4) == 16, "sizeof(vec4) == 16");
static_assert(sizeof(dvec4) == 32, "sizeof(dvec4) == 32");

/*
 * vec_t *(scalar, vec_t)
 */

template<typename T, int N, int SWIZZLE>
static inline vec_t<T,N> operator *(T const &val, vec_t<T,N,SWIZZLE> const &a)
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
    return (T)sqrt((double)sqlength(a));
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
static inline vec_t<T,N> normalize(vec_t<T,N,SWIZZLE> const &a)
{
    T norm = (T)length(a);
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
static inline vec_t<T,N> radians(vec_t<T,N,SWIZZLE> const &a)
{
    vec_t<T,N> ret;
    for (int i = 0; i < N; ++i)
        ret[i] = lol::radians(a[i]);
    return ret;
}

/*
 * Magic vector swizzling (part 2/2)
 */

#if LOL_FEATURE_CXX11_RELAXED_UNIONS
template<int N, typename T, int SWIZZLE>
inline vec_t<T, N, SWIZZLE>& vec_t<T, N, SWIZZLE>::operator =(vec_t<T,N> that)
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

