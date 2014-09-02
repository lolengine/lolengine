//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol {

#define LOL_VEC_2_CONST(T, name, a, b) \
    template<> \
    vec_t<T,2> const vec_t<T,2>::name = vec_t<T,2>((T)a, (T)b);

#define LOL_VEC_3_CONST(T, name, a, b, c) \
    template<> \
    vec_t<T,3> const vec_t<T,3>::name = vec_t<T,3>((T)a, (T)b, (T)c);

#define LOL_VEC_4_CONST(T, name, a, b, c, d) \
    template<> \
    vec_t<T,4> const vec_t<T,4>::name = vec_t<T,4>((T)a, (T)b, (T)c, (T)d);

#define LOL_MAT_CONST(T, name, a) \
    template<> \
    mat_t<T,2,2> const mat_t<T,2,2>::name = mat_t<T,2,2>((T)a); \
    \
    template<> \
    mat_t<T,3,3> const mat_t<T,3,3>::name = mat_t<T,3,3>((T)a); \
    \
    template<> \
    mat_t<T,4,4> const mat_t<T,4,4>::name = mat_t<T,4,4>((T)a);

#define LOL_ALL_CONST_INNER(T) \
    LOL_VEC_2_CONST(T, zero,     0, 0) \
    LOL_VEC_2_CONST(T, axis_x,   1, 0) \
    LOL_VEC_2_CONST(T, axis_y,   0, 1) \
    \
    LOL_VEC_3_CONST(T, zero,     0, 0, 0) \
    LOL_VEC_3_CONST(T, axis_x,   1, 0, 0) \
    LOL_VEC_3_CONST(T, axis_y,   0, 1, 0) \
    LOL_VEC_3_CONST(T, axis_z,   0, 0, 1) \
    \
    LOL_VEC_4_CONST(T, zero,     0, 0, 0, 0) \
    LOL_VEC_4_CONST(T, axis_x,   1, 0, 0, 0) \
    LOL_VEC_4_CONST(T, axis_y,   0, 1, 0, 0) \
    LOL_VEC_4_CONST(T, axis_z,   0, 0, 1, 0) \
    LOL_VEC_4_CONST(T, axis_w,   0, 0, 0, 1) \
    \
    LOL_MAT_CONST(T, identity, 1)

#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
LOL_ALL_CONST_INNER(half)
#endif
LOL_ALL_CONST_INNER(float)
LOL_ALL_CONST_INNER(double)
LOL_ALL_CONST_INNER(ldouble)
#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
LOL_ALL_CONST_INNER(real)
#endif

LOL_ALL_CONST_INNER(int8_t)
LOL_ALL_CONST_INNER(uint8_t)
LOL_ALL_CONST_INNER(int16_t)
LOL_ALL_CONST_INNER(uint16_t)
LOL_ALL_CONST_INNER(int32_t)
LOL_ALL_CONST_INNER(uint32_t)
LOL_ALL_CONST_INNER(int64_t)
LOL_ALL_CONST_INNER(uint64_t)

}; /* namespace lol */

