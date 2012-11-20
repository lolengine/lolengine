//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
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
 * Hash implementations
 */

static class HashData
{
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

    uint32_t crc32_table[256];
}
const data;

/*
 * Public Hash classes
 */

uint32_t Hash<int8_t>::operator ()(int8_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

uint32_t Hash<uint8_t>::operator ()(uint8_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

uint32_t Hash<int16_t>::operator ()(int16_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 8))] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

uint32_t Hash<uint16_t>::operator ()(uint16_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 8))] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

uint32_t Hash<int32_t>::operator ()(int32_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 8))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 16))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 24))] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

uint32_t Hash<uint32_t>::operator ()(uint32_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 8))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 16))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 24))] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

uint32_t Hash<int64_t>::operator ()(int64_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 8))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 16))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 24))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 32))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 40))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 48))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 56))] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

uint32_t Hash<uint64_t>::operator ()(uint64_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 8))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 16))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 24))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 32))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 40))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 48))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 56))] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

uint32_t Hash<char const *>::operator ()(char const *s)
{
    uint32_t ret = 0xffffffffu, ch;

    while ((ch = (uint8_t)*s++))
        ret = data.crc32_table[(uint8_t)(ret ^ ch)] ^ (ret >> 8);

    return ret ^ 0xffffffffu;
}

} /* namespace lol */

