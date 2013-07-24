//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
 * Helper hash functions
 */

static inline uint32_t Hash8(uint8_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

static inline uint32_t Hash16(uint16_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 8))] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

static inline uint32_t Hash32(uint32_t x)
{
    uint32_t ret = 0xffffffffu;
    ret = data.crc32_table[(uint8_t)(ret ^ x)] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 8))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 16))] ^ (ret >> 8);
    ret = data.crc32_table[(uint8_t)(ret ^ (x >> 24))] ^ (ret >> 8);
    return ret ^ 0xffffffffu;
}

static inline uint32_t Hash64(uint64_t x)
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

/*
 * Integer hash functions
 */

uint32_t Hash<int8_t>::operator ()(int8_t x) const
{
    return Hash8((uint8_t)x);
}

uint32_t Hash<uint8_t>::operator ()(uint8_t x) const
{
    return Hash8(x);
}

uint32_t Hash<int16_t>::operator ()(int16_t x) const
{
    return Hash16((uint16_t)x);
}

uint32_t Hash<uint16_t>::operator ()(uint16_t x) const
{
    return Hash16(x);
}

uint32_t Hash<int32_t>::operator ()(int32_t x) const
{
    return Hash32((uint32_t)x);
}

uint32_t Hash<uint32_t>::operator ()(uint32_t x) const
{
    return Hash32(x);
}

uint32_t Hash<int64_t>::operator ()(int64_t x) const
{
    return Hash64((uint64_t)x);
}

uint32_t Hash<uint64_t>::operator ()(uint64_t x) const
{
    return Hash64(x);
}

/*
 * Floating-point hash functions
 */

uint32_t Hash<half>::operator ()(half f) const
{
    return Hash16(f.bits);
}

uint32_t Hash<float>::operator ()(float f) const
{
    union { float tmp; uint32_t x; } u = { f };
    return Hash32(u.x);
}

uint32_t Hash<double>::operator ()(double f) const
{
    union { double tmp; uint64_t x; } u = { f };
    return Hash64(u.x);
}

/*
 * String and array hash functions
 */

static uint32_t HashCharString(char const *s)
{
    uint32_t ret = 0xffffffffu, ch;

    while ((ch = (uint8_t)*s++))
        ret = data.crc32_table[(uint8_t)(ret ^ ch)] ^ (ret >> 8);

    return ret ^ 0xffffffffu;
}

uint32_t Hash<char const *>::operator ()(char const *s) const
{
    return HashCharString(s);
}

uint32_t Hash<char const *>::operator ()(String const &s) const
{
    return HashCharString(&s[0]);
}

uint32_t Hash<String>::operator ()(String const &s) const
{
    return HashCharString(&s[0]);
}

uint32_t Hash<String>::operator ()(char const *s) const
{
    return HashCharString(s);
}

} /* namespace lol */

