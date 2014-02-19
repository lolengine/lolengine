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

namespace lol {

#define LOL_VEC_2_CONST(type, name, a, b) \
    template<> \
    Vec2<type> const Vec2<type>::name = Vec2<type>((type)a, (type)b);

#define LOL_VEC_3_CONST(type, name, a, b, c) \
    template<> \
    Vec3<type> const Vec3<type>::name = Vec3<type>((type)a, (type)b, (type)c);

#define LOL_VEC_4_CONST(type, name, a, b, c, d) \
    template<> \
    Vec4<type> const Vec4<type>::name = Vec4<type>((type)a, (type)b, (type)c, (type)d);

#define LOL_MAT_CONST(type, name, a) \
    template<> \
    Mat2<type> const Mat2<type>::name = Mat2<type>((type)a); \
    \
    template<> \
    Mat3<type> const Mat3<type>::name = Mat3<type>((type)a); \
    \
    template<> \
    Mat4<type> const Mat4<type>::name = Mat4<type>((type)a);

#define LOL_ALL_CONST_INNER(type) \
    LOL_VEC_2_CONST(type, one,      1, 1) \
    LOL_VEC_2_CONST(type, onen,    -1,-1) \
    LOL_VEC_2_CONST(type, zero,     0, 0) \
    LOL_VEC_2_CONST(type, axis_x,   1, 0) \
    LOL_VEC_2_CONST(type, axis_y,   0, 1) \
    LOL_VEC_2_CONST(type, axis_xn,  1,-1) \
    LOL_VEC_2_CONST(type, axis_yn, -1, 1) \
    LOL_VEC_2_CONST(type, v11,      1, 1) \
    LOL_VEC_2_CONST(type, v00,      0, 0) \
    LOL_VEC_2_CONST(type, v01,      0, 1) \
    LOL_VEC_2_CONST(type, v10,      1, 0) \
    LOL_VEC_2_CONST(type, v00n,    -1,-1) \
    LOL_VEC_2_CONST(type, v01n,    -1, 1) \
    LOL_VEC_2_CONST(type, v10n,     1,-1) \
    \
    LOL_VEC_3_CONST(type, one,      1, 1, 1) \
    LOL_VEC_3_CONST(type, onen,    -1,-1,-1) \
    LOL_VEC_3_CONST(type, zero,     0, 0, 0) \
    LOL_VEC_3_CONST(type, axis_x,   1, 0, 0) \
    LOL_VEC_3_CONST(type, axis_y,   0, 1, 0) \
    LOL_VEC_3_CONST(type, axis_z,   0, 0, 1) \
    LOL_VEC_3_CONST(type, axis_xn, -1, 0, 0) \
    LOL_VEC_3_CONST(type, axis_yn,  0,-1, 0) \
    LOL_VEC_3_CONST(type, axis_zn,  0, 0,-1) \
    LOL_VEC_3_CONST(type, v111,     1, 1, 1) \
    LOL_VEC_3_CONST(type, v000,     0, 0, 0) \
    LOL_VEC_3_CONST(type, v100,     1, 0, 0) \
    LOL_VEC_3_CONST(type, v010,     0, 1, 0) \
    LOL_VEC_3_CONST(type, v001,     0, 0, 1) \
    LOL_VEC_3_CONST(type, v110,     1, 1, 0) \
    LOL_VEC_3_CONST(type, v101,     1, 0, 1) \
    LOL_VEC_3_CONST(type, v011,     0, 1, 1) \
    LOL_VEC_3_CONST(type, v000n,   -1,-1,-1) \
    LOL_VEC_3_CONST(type, v100n,    1,-1,-1) \
    LOL_VEC_3_CONST(type, v010n,   -1, 1,-1) \
    LOL_VEC_3_CONST(type, v001n,   -1,-1, 1) \
    LOL_VEC_3_CONST(type, v110n,    1, 1,-1) \
    LOL_VEC_3_CONST(type, v101n,    1,-1, 1) \
    LOL_VEC_3_CONST(type, v011n,   -1, 1, 1) \
    \
    LOL_VEC_4_CONST(type, one,      1, 1, 1, 1) \
    LOL_VEC_4_CONST(type, onen,    -1,-1,-1,-1) \
    LOL_VEC_4_CONST(type, zero,     0, 0, 0, 0) \
    LOL_VEC_4_CONST(type, axis_x,   1, 0, 0, 0) \
    LOL_VEC_4_CONST(type, axis_y,   0, 1, 0, 0) \
    LOL_VEC_4_CONST(type, axis_z,   0, 0, 1, 0) \
    LOL_VEC_4_CONST(type, axis_w,   0, 0, 0, 1) \
    LOL_VEC_4_CONST(type, axis_xn, -1, 0, 0, 0) \
    LOL_VEC_4_CONST(type, axis_yn,  0,-1, 0, 0) \
    LOL_VEC_4_CONST(type, axis_zn,  0, 0,-1, 0) \
    LOL_VEC_4_CONST(type, axis_wn,  0, 0, 0,-1) \
    LOL_VEC_4_CONST(type, v1111,    1, 1, 1, 1) \
    LOL_VEC_4_CONST(type, v0000,    0, 0, 0, 0) \
    LOL_VEC_4_CONST(type, v1000,    1, 0, 0, 0) \
    LOL_VEC_4_CONST(type, v0100,    0, 1, 0, 0) \
    LOL_VEC_4_CONST(type, v0010,    0, 0, 1, 0) \
    LOL_VEC_4_CONST(type, v0001,    0, 0, 0, 1) \
    LOL_VEC_4_CONST(type, v1100,    1, 1, 0, 0) \
    LOL_VEC_4_CONST(type, v0110,    0, 1, 1, 0) \
    LOL_VEC_4_CONST(type, v0011,    0, 0, 1, 1) \
    LOL_VEC_4_CONST(type, v1010,    1, 0, 1, 0) \
    LOL_VEC_4_CONST(type, v0101,    0, 1, 0, 1) \
    LOL_VEC_4_CONST(type, v1001,    1, 0, 0, 1) \
    LOL_VEC_4_CONST(type, v0111,    0, 1, 1, 1) \
    LOL_VEC_4_CONST(type, v1011,    1, 0, 1, 1) \
    LOL_VEC_4_CONST(type, v1101,    1, 1, 0, 1) \
    LOL_VEC_4_CONST(type, v1110,    1, 1, 1, 0) \
    LOL_VEC_4_CONST(type, v0000n,  -1,-1,-1,-1) \
    LOL_VEC_4_CONST(type, v1000n,   1,-1,-1,-1) \
    LOL_VEC_4_CONST(type, v0100n,  -1, 1,-1,-1) \
    LOL_VEC_4_CONST(type, v0010n,  -1,-1, 1,-1) \
    LOL_VEC_4_CONST(type, v0001n,  -1,-1,-1, 1) \
    LOL_VEC_4_CONST(type, v1100n,   1, 1,-1,-1) \
    LOL_VEC_4_CONST(type, v0110n,  -1, 1, 1,-1) \
    LOL_VEC_4_CONST(type, v0011n,  -1,-1, 1, 1) \
    LOL_VEC_4_CONST(type, v1010n,   1,-1, 1,-1) \
    LOL_VEC_4_CONST(type, v0101n,  -1, 1,-1, 1) \
    LOL_VEC_4_CONST(type, v1001n,   1,-1,-1, 1) \
    LOL_VEC_4_CONST(type, v0111n,  -1, 1, 1, 1) \
    LOL_VEC_4_CONST(type, v1011n,   1,-1, 1, 1) \
    LOL_VEC_4_CONST(type, v1101n,   1, 1,-1, 1) \
    LOL_VEC_4_CONST(type, v1110n,   1, 1, 1,-1) \
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

