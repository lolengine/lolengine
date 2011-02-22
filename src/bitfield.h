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
// The BitField class
// ------------------
//

#if !defined __DH_BITFIELD_H__
#define __DH_BITFIELD_H__

#include <stdint.h>

namespace lol
{

template class BitField<unsigned int COUNT>
{
public:
    BitField()
    {
        memset(bits, 0, sizeof(bits));
    }

    inline unsigned int IsSet(unsigned int index)
    {
        return bits[index / 32] & (1 << (index & 31));
    }

    inline void Set(unsigned int index)
    {
        bits[index / 32] |= (1 << (index & 31));
    }

    inline void Unset(unsigned int index)
    {
        bits[index / 32] &= ~(1 << (index & 31));
    }


private:
    uint32_t bits[(COUNT + 31) / 32];
};

} /* namespace lol */

#endif // __DH_BITFIELD_H__

