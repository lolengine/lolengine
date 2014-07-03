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

#include <lol/main.h>

namespace lol {

#define LOL_VEC_2_CONST(type, name, a, b) \
    template<> \
    vec<2,type> const vec<2,type>::name = vec<2,type>((type)a, (type)b);

#define LOL_VEC_3_CONST(type, name, a, b, c) \
    template<> \
    vec<3,type> const vec<3,type>::name = vec<3,type>((type)a, (type)b, (type)c);

#define LOL_VEC_4_CONST(type, name, a, b, c, d) \
    template<> \
    vec<4,type> const vec<4,type>::name = vec<4,type>((type)a, (type)b, (type)c, (type)d);

#define LOL_MAT_CONST(type, name, a) \
    template<> \
    mat<2,2,type> const mat<2,2,type>::name = mat<2,2,type>((type)a); \
    \
    template<> \
    mat<3,3,type> const mat<3,3,type>::name = mat<3,3,type>((type)a); \
    \
    template<> \
    mat<4,4,type> const mat<4,4,type>::name = mat<4,4,type>((type)a);

#define LOL_ALL_CONST_INNER(type) \
    LOL_VEC_2_CONST(type, zero,     0, 0) \
    LOL_VEC_2_CONST(type, axis_x,   1, 0) \
    LOL_VEC_2_CONST(type, axis_y,   0, 1) \
    \
    LOL_VEC_3_CONST(type, zero,     0, 0, 0) \
    LOL_VEC_3_CONST(type, axis_x,   1, 0, 0) \
    LOL_VEC_3_CONST(type, axis_y,   0, 1, 0) \
    LOL_VEC_3_CONST(type, axis_z,   0, 0, 1) \
    \
    LOL_VEC_4_CONST(type, zero,     0, 0, 0, 0) \
    LOL_VEC_4_CONST(type, axis_x,   1, 0, 0, 0) \
    LOL_VEC_4_CONST(type, axis_y,   0, 1, 0, 0) \
    LOL_VEC_4_CONST(type, axis_z,   0, 0, 1, 0) \
    LOL_VEC_4_CONST(type, axis_w,   0, 0, 0, 1) \
    \
    LOL_MAT_CONST(type, identity, 1)

LOL_ALL_CONST_INNER(half)
LOL_ALL_CONST_INNER(float)
LOL_ALL_CONST_INNER(double)
LOL_ALL_CONST_INNER(ldouble)
LOL_ALL_CONST_INNER(real)

LOL_ALL_CONST_INNER(int8_t)
LOL_ALL_CONST_INNER(uint8_t)
LOL_ALL_CONST_INNER(int16_t)
LOL_ALL_CONST_INNER(uint16_t)
LOL_ALL_CONST_INNER(int32_t)
LOL_ALL_CONST_INNER(uint32_t)
LOL_ALL_CONST_INNER(int64_t)
LOL_ALL_CONST_INNER(uint64_t)

}; /* namespace lol */

