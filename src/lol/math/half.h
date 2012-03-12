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
// The Half class
// --------------
//

#if !defined __LOL_HALF_H__
#define __LOL_HALF_H__

#include <cstdio>
#include <stdint.h>

namespace lol
{

class half
{
public:
    /* Constructors. Always inline so that the code can work in registers
     * instead of calling routines with the hidden "this" parameter. */
    inline half() { }
    inline half(int f) { *this = makefast((float)f); }
    inline half(float f) { *this = makefast(f); }
    inline half(double f) { *this = makefast((float)f); }

    inline int is_nan() const
    {
        return ((bits & 0x7c00u) == 0x7c00u) && (bits & 0x03ffu);
    }

    inline int is_finite() const
    {
        return (bits & 0x7c00u) != 0x7c00u;
    }

    inline int is_inf() const
    {
        return (uint16_t)(bits << 1) == (0x7c00u << 1);
    }

    inline int is_normal() const
    {
        return (is_finite() && (bits & 0x7c00u)) || ((bits & 0x7fffu) == 0);
    }

    /* Cast to other types -- always inline, see constructors */
    inline half &operator =(int f) { return *this = makefast((float)f); }
    inline half &operator =(float f) { return *this = makefast(f); }
    inline half &operator =(double f) { return *this = makefast((float)f); }
    inline operator int() const { return (int)tofloat(*this); }
    inline operator float() const { return tofloat(*this); }

    static float tofloat(half h);

    /* Array conversions */
    static size_t convert(half *dst, float const *src, size_t nelem);
    static size_t convert(float *dst, half const *src, size_t nelem);

    /* Operations */
    bool operator ==(half x) const { return (float)*this == (float)x; }
    bool operator !=(half x) const { return (float)*this != (float)x; }
    bool operator <(half x) const { return (float)*this < (float)x; }
    bool operator >(half x) const { return (float)*this > (float)x; }
    bool operator <=(half x) const { return (float)*this <= (float)x; }
    bool operator >=(half x) const { return (float)*this >= (float)x; }

    bool operator !() const { return !(bits & 0x7fffu); }
    operator bool() const { return !!*this; }

    inline half operator -() const { return makebits(bits ^ 0x8000u); }
    inline half operator +() const { return *this; }
    inline half &operator +=(float f) { return (*this = (half)(*this + f)); }
    inline half &operator -=(float f) { return (*this = (half)(*this - f)); }
    inline half &operator *=(float f) { return (*this = (half)(*this * f)); }
    inline half &operator /=(float f) { return (*this = (half)(*this / f)); }
    inline half &operator +=(half h) { return (*this = (half)(*this + h)); }
    inline half &operator -=(half h) { return (*this = (half)(*this - h)); }
    inline half &operator *=(half h) { return (*this = (half)(*this * h)); }
    inline half &operator /=(half h) { return (*this = (half)(*this / h)); }

    inline float operator +(float f) const { return (float)*this + f; }
    inline float operator -(float f) const { return (float)*this - f; }
    inline float operator *(float f) const { return (float)*this * f; }
    inline float operator /(float f) const { return (float)*this / f; }
    inline float operator +(half h) const { return (float)*this + (float)h; }
    inline float operator -(half h) const { return (float)*this - (float)h; }
    inline float operator *(half h) const { return (float)*this * (float)h; }
    inline float operator /(half h) const { return (float)*this / (float)h; }

    /* Factories */
    static half makefast(float f);
    static half makeaccurate(float f);
    static inline half makebits(uint16_t x)
    {
        half ret;
        ret.bits = x;
        return ret;
    }

    /* Internal representation */
    uint16_t bits;
};

inline float &operator +=(float &f, half h) { return f += (float)h; }
inline float &operator -=(float &f, half h) { return f -= (float)h; }
inline float &operator *=(float &f, half h) { return f *= (float)h; }
inline float &operator /=(float &f, half h) { return f /= (float)h; }

inline float operator +(float f, half h) { return f + (float)h; }
inline float operator -(float f, half h) { return f - (float)h; }
inline float operator *(float f, half h) { return f * (float)h; }
inline float operator /(float f, half h) { return f / (float)h; }

} /* namespace lol */

#endif // __LOL_HALF_H__

