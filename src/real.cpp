//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>

#include "core.h"

using namespace std;

namespace lol
{

template<> real4k::Real(float f)
{
    union { float f; uint32_t x; } u = { f };

    uint32_t sign = u.x & 0x80000000u;
    int e = ((u.x >> 23) & 0xff) + (1 << 30) - (1 << 10);

    m_signexp = sign | e;
    m_mantissa[0] = u.x << 17;
    memset(m_mantissa + 1, 0, sizeof(m_mantissa) - sizeof(m_mantissa[0]));
}

template<> real4k::operator float() const
{
    union { float f; uint32_t x; } u;

    u.x = m_mantissa[0] >> 17;
    u.x |= ((m_signexp & 0x7fffffffu) - (1 << 30) + (1 << 10)) << 23;
    u.x |= m_signexp & 0x80000000u;

    return u.f;
}

} /* namespace lol */

