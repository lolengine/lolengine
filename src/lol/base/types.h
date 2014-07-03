//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_BASE_TYPES_H__
#define __LOL_BASE_TYPES_H__

#include <stdint.h>

namespace lol
{

/* There are many reasons for wanting single-word type names, the most
 * important one being compilation speedups in our vector.h: we can hide
 * some global methods in namespaces that contain the name of the type,
 * but namespaces cannot have spaces in their names. */
typedef long double ldouble;

/* The “real” type used for real numbers. It’s a specialisation of the
 * “Real” template class. */
template<int N> class Real;
typedef Real<16> real;

/* The “half” type used for 16-bit floating point numbers. */
class half;

/*
 * Forward declaration of vec and matrix
 */

int const FULL_SWIZZLE = 0xaaaa;
int const NO_SWIZZLE = 0xbbbb;

template<int N, typename T, int SWIZZLE = FULL_SWIZZLE> struct vec;
template<int COLS, int ROWS, typename T> struct mat;
template<typename T> struct Cmplx;
template<typename T> struct Quat;

/*
 * Generic GLSL-like type names
 */

#define LOL_VECTOR_TYPEDEFS(tleft, tright, suffix) \
    typedef tleft half tright f16##suffix; \
    typedef tleft float tright suffix; \
    typedef tleft double tright d##suffix; \
    typedef tleft ldouble tright f128##suffix; \
    typedef tleft int8_t tright i8##suffix; \
    typedef tleft uint8_t tright u8##suffix; \
    typedef tleft int16_t tright i16##suffix; \
    typedef tleft uint16_t tright u16##suffix; \
    typedef tleft int32_t tright i##suffix; \
    typedef tleft uint32_t tright u##suffix; \
    typedef tleft int64_t tright i64##suffix; \
    typedef tleft uint64_t tright u64##suffix; \
    typedef tleft real tright r##suffix;

/* Idiotic hack to put "," inside a macro argument */
#define COMMA ,

LOL_VECTOR_TYPEDEFS(vec<2 COMMA, >, vec2)
LOL_VECTOR_TYPEDEFS(vec<3 COMMA, >, vec3)
LOL_VECTOR_TYPEDEFS(vec<4 COMMA, >, vec4)

LOL_VECTOR_TYPEDEFS(mat<2 COMMA 2 COMMA, >, mat2)
LOL_VECTOR_TYPEDEFS(mat<3 COMMA 3 COMMA, >, mat3)
LOL_VECTOR_TYPEDEFS(mat<4 COMMA 4 COMMA, >, mat4)

LOL_VECTOR_TYPEDEFS(mat<2 COMMA 3 COMMA, >, mat2x3)
LOL_VECTOR_TYPEDEFS(mat<2 COMMA 4 COMMA, >, mat2x4)
LOL_VECTOR_TYPEDEFS(mat<3 COMMA 2 COMMA, >, mat3x2)
LOL_VECTOR_TYPEDEFS(mat<3 COMMA 4 COMMA, >, mat3x4)
LOL_VECTOR_TYPEDEFS(mat<4 COMMA 2 COMMA, >, mat4x2)
LOL_VECTOR_TYPEDEFS(mat<4 COMMA 3 COMMA, >, mat4x3)

LOL_VECTOR_TYPEDEFS(Cmplx<, >, cmplx)
LOL_VECTOR_TYPEDEFS(Quat<, >, quat)

#undef COMMA
#undef LOL_VECTOR_TYPEDEFS

/*
 * HLSL/Cg-compliant type names
 */

typedef vec2 float2;
typedef vec3 float3;
typedef vec4 float4;
typedef mat2 float2x2;
typedef mat3 float3x3;
typedef mat4 float4x4;
typedef mat2x3 float2x3;
typedef mat2x4 float2x4;
typedef mat3x2 float3x2;
typedef mat3x4 float3x4;
typedef mat4x2 float4x2;
typedef mat4x3 float4x3;

typedef f16vec2 half2;
typedef f16vec3 half3;
typedef f16vec4 half4;
typedef f16mat2 half2x2;
typedef f16mat3 half3x3;
typedef f16mat4 half4x4;
typedef f16mat2x3 half2x3;
typedef f16mat2x4 half2x4;
typedef f16mat3x2 half3x2;
typedef f16mat3x4 half3x4;
typedef f16mat4x2 half4x2;
typedef f16mat4x3 half4x3;

typedef ivec2 int2;
typedef ivec3 int3;
typedef ivec4 int4;
typedef imat2 int2x2;
typedef imat3 int3x3;
typedef imat4 int4x4;
typedef imat2x3 int2x3;
typedef imat2x4 int2x4;
typedef imat3x2 int3x2;
typedef imat3x4 int3x4;
typedef imat4x2 int4x2;
typedef imat4x3 int4x3;

} /* namespace lol */

#endif // __LOL_BASE_TYPES_H__

