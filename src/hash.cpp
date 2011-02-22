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

#include "core.h"

namespace lol
{

/*
 * Hash implementation class
 */

static class HashData
{
    friend class Hash;

public:
    HashData()
    {
        /* Initialise CRC32 table */
        for (int i = 0; i < 256; i++)
        {
            uint32_t tmp = i;
            for (int j = 8; j--; )
                tmp = (tmp >> 1) ^ ((tmp & 1) ? 0xedb88320 : 0);
            crc32_table[i] = tmp;
        }
    }

private:
    uint32_t crc32_table[256];
}
hashdata;

static HashData * const data = &hashdata;

/*
 * Public Hash class
 */

uint32_t Hash::Crc32(char const *str)
{
    uint32_t ret = 0xffffffff, ch;

    while ((ch = (uint8_t)*str++))
        ret = data->crc32_table[(uint8_t)(ret ^ ch)] ^ (ret >> 8);

    return ret ^ 0xffffffff;
}

} /* namespace lol */

