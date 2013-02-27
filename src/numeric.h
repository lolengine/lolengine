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
// The Matrix classes
// ------------------
//

#if !defined __LOL_NUMERIC_H__
#define __LOL_NUMERIC_H__

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

template<>
inline half rand<half>(half a) { return a * std::rand() / RAND_MAX; }
template<>
inline float rand<float>(float a) { return a * std::rand() / RAND_MAX; }
template<>
inline double rand<double>(double a) { return a * std::rand() / RAND_MAX; }
template<>
inline ldouble rand<ldouble>(ldouble a) { return a * std::rand() / RAND_MAX; }

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
        return static_cast<T>(std::rand() & 0xff);
    case 2:
        if (RAND_MAX >= 0xffff)
            return static_cast<T>(std::rand());
        else
            return static_cast<T>((std::rand() << 8) ^ std::rand());
    case 4:
        if (RAND_MAX >= 0xffff)
            return static_cast<T>((std::rand() << 16) ^ std::rand());
        else
            return static_cast<T>((std::rand() << 24) ^ (std::rand() << 16)
                                 ^ (std::rand() << 8) ^ std::rand());
    case 8:
        if (RAND_MAX >= 0xffff)
            return static_cast<T>(((uint64_t)std::rand() << 48)
                                ^ ((uint64_t)std::rand() << 32)
                                ^ ((uint64_t)std::rand() << 16)
                                ^ ((uint64_t)std::rand()));
        else
            return static_cast<T>(((uint64_t)std::rand() << 56)
                                ^ ((uint64_t)std::rand() << 48)
                                ^ ((uint64_t)std::rand() << 40)
                                ^ ((uint64_t)std::rand() << 32)
                                ^ ((uint64_t)std::rand() << 24)
                                ^ ((uint64_t)std::rand() << 16)
                                ^ ((uint64_t)std::rand() << 8)
                                ^ ((uint64_t)std::rand()));
    default:
        ASSERT(false, "rand() doesn’t support types of size %d\n",
               (int)sizeof(T));
    }
}

template<> inline half rand<half>() { return rand<half>(1.f); }
template<> inline float rand<float>() { return rand<float>(1.f); }
template<> inline double rand<double>() { return rand<double>(1.0); }
template<> inline ldouble rand<ldouble>() { return rand<ldouble>(1.0); }


/* Next power of two. */
template <typename T> static inline T PotUp(T val)
{
    val = val - 1;
    if (sizeof(val) > 4) val = val | ((uint64_t)val >> 32);
    if (sizeof(val) > 2) val = val | ((uint64_t)val >> 16);
    if (sizeof(val) > 1) val = val | ((uint64_t)val >> 8);
    val = val | ((uint64_t)val >> 4);
    val = val | ((uint64_t)val >> 2);
    val = val | ((uint64_t)val >> 1);
    return val + 1;
}

//Damp for float
template <typename T1, typename T2, typename Tf> static inline T1 damp(const T1 &a, const T2 &b, const Tf &x, const Tf &dt)
{
    if (dt <= .0f)
        return a;
    return lol::lerp(a, b, dt / (dt + x));
}

} /* namespace lol */

#endif // __LOL_NUMERIC_H__

