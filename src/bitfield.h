//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The BitField class
// ------------------
//

#if !defined __DH_BITFIELD_H__
#define __DH_BITFIELD_H__

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
    /* FIXME: use uint32_t here instead */
    unsigned int bits[(COUNT + 31) / 32];
};

#endif // __DH_BITFIELD_H__

