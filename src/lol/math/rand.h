//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Random number generators
// ----------------------------
//

#if !defined __LOL_MATH_RAND_H__
#define __LOL_MATH_RAND_H__

#include <cstdlib>
#include <stdint.h>

namespace lol
{

/* Random number generators */
template<typename T> static inline T rand();
template<typename T> static inline T rand(T a);
template<typename T> static inline T rand(T a, T b);

/* One-value random number generators */
template<typename T> static inline T rand(T a)
{
    return rand<T>() % a;
}

template<> inline half rand<half>(half a)
{
    float f = (float)std::rand() / (float)RAND_MAX;
    return (half)(a * f);
}

template<> inline float rand<float>(float a)
{
    float f = (float)std::rand() / (float)RAND_MAX;
    return a * f;
}

template<> inline double rand<double>(double a)
{
    double f = (double)std::rand() / (double)RAND_MAX;
    return a * f;
}

template<> inline ldouble rand<ldouble>(ldouble a)
{
    ldouble f = (ldouble)std::rand() / (ldouble)RAND_MAX;
    return a * f;
}

/* Two-value random number generator -- no need for specialisation */
template<typename T> static inline T rand(T a, T b)
{
    return a + rand<T>(b - a);
}

/* Default random number generator */
template<typename T> static inline T rand()
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
        ASSERT(false, "rand() doesn’t support types of size %d\n",
               (int)sizeof(T));
        return 0;
    }
}

template<> inline half rand<half>() { return rand<half>(1.f); }
template<> inline float rand<float>() { return rand<float>(1.f); }
template<> inline double rand<double>() { return rand<double>(1.0); }
template<> inline ldouble rand<ldouble>() { return rand<ldouble>(1.0); }

} /* namespace lol */

#endif // __LOL_MATH_RAND_H__

