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

#include <stdint.h>

namespace lol
{

class half
{
private:
    uint16_t m_bits;

public:
    inline half() { }

    inline half(float f)
    {
        *this = makefast(f);
    }

    inline int isnan() const
    {
        return ((m_bits & 0x7c00u) == 0x7c00u) && (m_bits & 0x03ffu);
    }

    inline int isfinite() const
    {
        return (m_bits & 0x7c00u) != 0x7c00u;
    }

    inline int isinf() const
    {
        return (uint16_t)(m_bits << 1) == (0x7c00u << 1);
    }

    inline int isnormal() const
    {
        return (isfinite() && (m_bits & 0x7c00u)) || ((m_bits & 0x7fffu) == 0);
    }

    inline uint16_t bits()
    {
        return m_bits;
    }

    /* Cast to other types */
    operator float() const;
    inline operator int() const { return (int)(float)*this; }

    /* Operations */
    inline half operator -() { return makebits(m_bits ^ 0x8000u); }
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
    static half makeslow(float f);
    static half makefast(float f);
    static inline half makebits(uint16_t x)
    {
        half ret;
        ret.m_bits = x;
        return ret;
    }
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

