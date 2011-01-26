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
// The Matrix classes
// ------------------
//

#if !defined __DH_NUMERIC_H__
#define __DH_NUMERIC_H__

#include <cmath>
#include <cstdlib>

/* Random float value */
static inline float RandF()
{
    return (float)rand() / RAND_MAX;
}

static inline float RandF(float val)
{
    return RandF() * val;
}

static inline float RandF(float min, float max)
{
    return min + RandF() * (max - min);
}

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

#endif // __DH_NUMERIC_H__

