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

    static half makeslow(float f);
    static half makefast(float f);

    static inline half makebits(uint16_t x)
    {
        half ret;
        ret.m_bits = x;
        return ret;
    }

    inline operator float() const
    {
        int s = m_bits & 0x8000u;
        int e = m_bits & 0x7c00u;
        int m = m_bits & 0x03ffu;

        union { float f; uint32_t x; } u;
        u.x = 0;
        u.x |= s << 16;
        u.x |= (-15 + (e >> 10) + 127) << 23;
        u.x |= m << 13;

        return u.f;
    }

    inline uint16_t bits()
    {
        return m_bits;
    }
};

} /* namespace lol */

#endif // __LOL_HALF_H__

