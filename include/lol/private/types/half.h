//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Half class
// --------------
//

#include <cmath>
#include <ostream>  // std::ostream
#include <stdint.h> // uint32_t etc.

// FIXME: get rid of this
#include "../math/constants.h"
#include "../math/functions.h"

namespace lol
{

#if _WIN32
#   pragma push_macro("near")
#   pragma push_macro("far")
#   undef near
#   undef far
#endif

namespace half_ops { struct base {}; }

class [[nodiscard]] half
  : half_ops::base
{
public:
    // Constructors. Always inline so that the code can work in registers.
    inline half() {}
    explicit inline half(int f) : half(float(f)) {}
    explicit inline half(double f) : half(float(f)) {}
    explicit inline half(long double f) : half(float(f)) {}

    explicit inline half(float f)
    {
        union { float f; uint32_t x; } u = { f };

        // This method is faster than the OpenEXR implementation (very often
        // used, eg. in Ogre), with the additional benefit of rounding, inspired
        // by James Tursa’s half-precision code.
        m_bits = (u.x >> 16) & 0x8000;       // Get the sign
        uint16_t m = (u.x >> 12) & 0x07ff;   // Keep one extra bit for rounding
        unsigned int e = (u.x >> 23) & 0xff; // Using int is faster here

        if (e < 103)
        {
            // If zero, or denormal, or exponent underflows too much for a denormal
            // half, return signed zero.
        }
        else if (e > 142)
        {
            // If NaN, return NaN. If Inf or exponent overflow, return Inf.
            m_bits |= 0x7c00u;
            // If exponent was 0xff and one mantissa bit was set, it means NaN,
            // not Inf, so make sure we set one mantissa bit too.
            m_bits |= e == 255 && (u.x & 0x007fffffu);
        }
        else if (e < 113)
        {
            // If exponent underflows but not too much, return a denormal
            m |= 0x0800u;
            // Extra rounding may overflow and set mantissa to 0 and exponent
            // to 1, which is OK.
            m_bits |= (m >> (114 - e)) + ((m >> (113 - e)) & 1);
        }
        else
        {
            m_bits |= ((e - 112) << 10) | (m >> 1);
            // Extra rounding. An overflow will set mantissa to 0 and increment
            // the exponent, which is OK.
            m_bits += m & 1;
        }
    }


    [[nodiscard]] inline int is_nan() const
    {
        return ((m_bits & 0x7c00u) == 0x7c00u) && (m_bits & 0x03ffu);
    }

    [[nodiscard]] inline int is_finite() const
    {
        return (m_bits & 0x7c00u) != 0x7c00u;
    }

    [[nodiscard]] inline int is_inf() const
    {
        return (uint16_t)(m_bits << 1) == (0x7c00u << 1);
    }

    [[nodiscard]] inline int is_normal() const
    {
        return (is_finite() && (m_bits & 0x7c00u)) || ((m_bits & 0x7fffu) == 0);
    }

    // Convert to string
    friend std::ostream &operator<<(std::ostream &stream, half const &v)
    {
        return stream << float(v);
    }

    // Cast to other types -- always inline, see constructors
    inline half &operator =(int f) { return *this = half(f); }
    inline half &operator =(float f) { return *this = half(f); }
    inline half &operator =(double f) { return *this = half(f); }
    inline half &operator =(long double f) { return *this = half(f); }
    [[nodiscard]] inline operator int8_t() const { return (int8_t)(float)*this; }
    [[nodiscard]] inline operator uint8_t() const { return (uint8_t)(float)*this; }
    [[nodiscard]] inline operator int16_t() const { return (int16_t)(float)*this; }
    [[nodiscard]] inline operator uint16_t() const { return (uint16_t)(float)*this; }
    [[nodiscard]] inline operator int32_t() const { return (int32_t)(float)*this; }
    [[nodiscard]] inline operator uint32_t() const { return (uint32_t)(float)*this; }
    [[nodiscard]] inline operator int64_t() const { return (int64_t)(float)*this; }
    [[nodiscard]] inline operator uint64_t() const { return (uint64_t)(float)*this; }

    [[nodiscard]] inline operator float() const
    {
        union { uint32_t x; float f; } u = { tofloatbits() };
        return u.f;
    }

    [[nodiscard]] inline operator double() const { return (float)(*this); }
    [[nodiscard]] inline operator long double() const { return (float)(*this); }

    // Operators
    [[nodiscard]] bool operator ==(half x) const { return (float)*this == (float)x; }
    [[nodiscard]] bool operator !=(half x) const { return (float)*this != (float)x; }
    [[nodiscard]] bool operator <(half x) const { return (float)*this < (float)x; }
    [[nodiscard]] bool operator >(half x) const { return (float)*this > (float)x; }
    [[nodiscard]] bool operator <=(half x) const { return (float)*this <= (float)x; }
    [[nodiscard]] bool operator >=(half x) const { return (float)*this >= (float)x; }

    [[nodiscard]] bool operator !() const { return !(m_bits & 0x7fffu); }
    [[nodiscard]] operator bool() const { return !!*this; }

    inline half operator -() const { return frombits(m_bits ^ 0x8000u); }
    inline half operator +() const { return *this; }
    inline half &operator +=(half h) { return (*this = (half)(*this + h)); }
    inline half &operator -=(half h) { return (*this = (half)(*this - h)); }
    inline half &operator *=(half h) { return (*this = (half)(*this * h)); }
    inline half &operator /=(half h) { return (*this = (half)(*this / h)); }

    [[nodiscard]] inline float operator +(half h) const { return (float)*this + (float)h; }
    [[nodiscard]] inline float operator -(half h) const { return (float)*this - (float)h; }
    [[nodiscard]] inline float operator *(half h) const { return (float)*this * (float)h; }
    [[nodiscard]] inline float operator /(half h) const { return (float)*this / (float)h; }

    inline uint16_t bits() const { return m_bits; }

    static inline half frombits(uint16_t x)
    {
        half ret;
        ret.m_bits = x;
        return ret;
    }

private:
    inline uint32_t tofloatbits() const
    {
        // This algorithm is similar to the OpenEXR implementation, except it
        // uses branchless code in the denormal path. This is slower than the
        // table version, but will be more friendly to the cache for occasional
        // uses.
        uint32_t s = (m_bits & 0x8000u) << 16;

        if ((m_bits & 0x7fffu) == 0)
            return (uint32_t)m_bits << 16;

        uint32_t e = m_bits & 0x7c00u;
        uint32_t m = m_bits & 0x03ffu;

        if (e == 0)
        {
            // We use this magic table, inspired by De Bruijn sequences, to
            // compute a branchless integer log2. The actual value fetched is
            // 24-log2(x+1) for x in 1, 3, 7, f, 1f, 3f, 7f, ff, 1fe, 1ff, 3fc,
            // 3fd, 3fe, 3ff. For an explanation of how 0x5a1a1a2u was obtained
            // see: http://lolengine.net/blog/2012/04/03/beyond-de-bruijn
            static uint32_t const shifttable[16] =
            {
                23, 22, 21, 15, 0, 20, 18, 14, 14, 16, 19, 0, 17, 0, 0, 0,
            };
            static uint32_t const shiftmagic = 0x5a1a1a2u;

            /* m has 10 significant bits but replicating the leading bit to
             * 8 positions instead of 16 works just as well because of our
             * handcrafted shiftmagic table. */
            uint32_t v = m | (m >> 1);
            v |= v >> 2;
            v |= v >> 4;

            e = shifttable[(v * shiftmagic) >> 28];

            /* We don't have to remove the 10th mantissa bit because it gets
             * added to our underestimated exponent. */
            return s | (((125 - e) << 23) + (m << e));
        }

        if (e == 0x7c00u)
        {
            /* The amd64 pipeline likes the if() better than a ternary operator
             * or any other trick I could find. --sam */
            if (m == 0)
                return s | 0x7f800000u;
            return s | 0x7fc00000u;
        }

        return s | (((e >> 10) + 112) << 23) | (m << 13);
    }

    // Internal representation
    uint16_t m_bits;
};

static_assert(sizeof(half) == 2, "sizeof(half) == 2");

//
// Standard math and GLSL functions
//

static inline half min(half a, half b) { return a < b ? a : b; }
static inline half max(half a, half b) { return a > b ? a : b; }
static inline float fmod(half a, half b)
{
    using std::fmod;
    return fmod((float)a, (float)b);
}
static inline float fract(half a) { return fract((float)a); }
static inline float degrees(half a) { return degrees((float)a); }
static inline float radians(half a) { return radians((float)a); }
static inline half fabs(half a) { return half::frombits(a.bits() & 0x7fffu); }

static inline half clamp(half x, half a, half b)
{
    return (x < a) ? a : (x > b) ? b : x;
}

//
// Standard math operators
//

namespace half_ops
{
    // Enumerate the types for which operations with half are valid
    template<typename FROM, typename TO = void> struct valid {};

    template<typename TO> struct valid<uint8_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<int8_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<uint16_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<int16_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<uint32_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<int32_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<uint64_t, TO>
        { typedef half from; typedef TO to; };
    template<typename TO> struct valid<int64_t, TO>
        { typedef half from; typedef TO to; };

    template<typename TO> struct valid<float, TO>
        { typedef float from; typedef TO to; };
    template<typename TO> struct valid<double, TO>
        { typedef double from; typedef TO to; };
    template<typename TO> struct valid<long double, TO>
        { typedef long double from; typedef TO to; };

#define DECLARE_HALF_NUMERIC_OPS(op) \
    /* other + half */ \
    template<typename T> \
    static inline typename valid<T>::from operator op(T x, half h) \
    { return (typename valid<T>::from)x op (typename valid<T>::from)h; } \
    \
    /* half + other */ \
    template<typename T> \
    static inline typename valid<T>::from operator op(half h, T x) \
    { return (typename valid<T>::from)h op (typename valid<T>::from)x; } \
    \
    /* other += half */ \
    template<typename T> \
    static inline typename valid<T,T>::to& operator op##=(T& x, half h) \
    { return x = (typename valid<T>::from)x op (typename valid<T>::from)h; } \
    \
    /* half += other */ \
    template<typename T> \
    static inline typename valid<T,half>::to& operator op##=(half& h, T x) \
    { return h = (typename valid<T>::from)h op (typename valid<T>::from)x; }

DECLARE_HALF_NUMERIC_OPS(+)
DECLARE_HALF_NUMERIC_OPS(-)
DECLARE_HALF_NUMERIC_OPS(*)
DECLARE_HALF_NUMERIC_OPS(/)

#undef DECLARE_HALF_NUMERIC_OPS

#define DECLARE_HALF_BOOL_OPS(op) \
    /* half == other */ \
    template<typename T> \
    static inline typename valid<T,bool>::to operator op(half h, T x) \
    { return (typename valid<T>::from)h op (typename valid<T>::from)x; } \
    \
    /* other == half */ \
    template<typename T> \
    static inline typename valid<T,bool>::to operator op(T x, half h) \
    { return (typename valid<T>::from)x op (typename valid<T>::from)h; }

DECLARE_HALF_BOOL_OPS(==)
DECLARE_HALF_BOOL_OPS(!=)
DECLARE_HALF_BOOL_OPS(>)
DECLARE_HALF_BOOL_OPS(<)
DECLARE_HALF_BOOL_OPS(>=)
DECLARE_HALF_BOOL_OPS(<=)

#undef DECLARE_HALF_BOOL_OPS

} // namespace half_ops

} // namespace lol

#if _WIN32
#   pragma pop_macro("near")
#   pragma pop_macro("far")
#endif
