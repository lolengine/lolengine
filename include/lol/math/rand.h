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
// The Random number generators
// ————————————————————————————
//

#include <lol/base/features.h>

#include <cassert>
#include <cstdlib>
#include <stdint.h>

namespace lol
{

/* Random number generators */
template<typename T> LOL_ATTR_NODISCARD static inline T rand();
template<typename T> LOL_ATTR_NODISCARD static inline T rand(T a);
template<typename T> LOL_ATTR_NODISCARD static inline T rand(T a, T b);

/* One-value random number generators */
template<typename T> LOL_ATTR_NODISCARD static inline T rand(T a)
{
    return a ? rand<T>() % a : T(0);
}

#if 0
template<> LOL_ATTR_NODISCARD inline half rand<half>(half a)
{
    float f = (float)std::rand() / (float)RAND_MAX;
    return (half)(a * f);
}
#endif

template<> LOL_ATTR_NODISCARD inline float rand<float>(float a)
{
    auto f = (float)std::rand() / (float)RAND_MAX;
    return a * f;
}

template<> LOL_ATTR_NODISCARD inline double rand<double>(double a)
{
    auto f = (double)std::rand() / (double)RAND_MAX;
    return a * f;
}

template<> LOL_ATTR_NODISCARD inline long double rand<long double>(long double a)
{
    auto f = (long double)std::rand() / (long double)RAND_MAX;
    return a * f;
}

/* Two-value random number generator -- no need for specialisation */
template<typename T> LOL_ATTR_NODISCARD static inline T rand(T a, T b)
{
    return a + rand<T>(b - a);
}

/* Default random number generator */
template<typename T> LOL_ATTR_NODISCARD static inline T rand()
{
    switch (sizeof(T))
    {
    case 1:
        return static_cast<T>(std::rand() & 0x7f);
    case 2:
    {
        uint16_t ret = std::rand();
        if (RAND_MAX < 0x7fff)
            ret = (ret << 7) ^ std::rand();
        return static_cast<T>(ret & 0x7fffu);
    }
    case 4:
    {
        uint32_t ret = std::rand();
        if (RAND_MAX >= 0xffff)
            ret = (ret << 16) ^ std::rand();
        else
        {
            ret = (ret << 8) ^ std::rand();
            ret = (ret << 8) ^ std::rand();
            ret = (ret << 8) ^ std::rand();
        }
        return static_cast<T>(ret & 0x7fffffffu);
    }
    case 8:
    {
        uint64_t ret = std::rand();
        if (RAND_MAX >= 0xffff)
        {
            ret = (ret << 16) ^ std::rand();
            ret = (ret << 16) ^ std::rand();
            ret = (ret << 16) ^ std::rand();
        }
        else
        {
            ret = (ret << 8) ^ std::rand();
            ret = (ret << 8) ^ std::rand();
            ret = (ret << 8) ^ std::rand();
            ret = (ret << 8) ^ std::rand();
            ret = (ret << 8) ^ std::rand();
            ret = (ret << 8) ^ std::rand();
            ret = (ret << 8) ^ std::rand();
        }
        return static_cast<T>(ret & (~(uint64_t)0 >> 1));
    }
    default:
        assert(false);
        return 0;
    }
}

#if 0
template<> LOL_ATTR_NODISCARD inline half rand<half>() { return rand<half>(1.f); }
#endif
template<> LOL_ATTR_NODISCARD inline float rand<float>() { return rand<float>(1.f); }
template<> LOL_ATTR_NODISCARD inline double rand<double>() { return rand<double>(1.0); }
template<> LOL_ATTR_NODISCARD inline long double rand<long double>() { return rand<long double>(1.0); }

} /* namespace lol */

