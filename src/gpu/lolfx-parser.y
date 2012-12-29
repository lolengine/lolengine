%{
//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

#include <string>

%}

%require "2.3"
%debug
%defines
%skeleton "lalr1.cc"
%name-prefix="lol"
%define parser_class_name "LolFxParser"
%locations
%parse-param { class LolFxCompiler& mc }
%error-verbose

/* The classic Bison union trick */
%union
{
    int ival;
    unsigned uval;
    float fval;
    char *sval;
}

/* %type <sval> lolfx_shader_name */

 /*
  * GLSL and HLSL generic tokens
  */

%token <ival> BOOLCONSTANT
%token <ival> INTCONSTANT
%token <uval> UINTCONSTANT
%token <fval> FLOATCONSTANT
%token <sval> FIELDSELECTION
%token <sval> IDENTIFIER
%token <sval> TYPENAME

 /*
  * GLSL keywords
  * FIXME: unsorted
  */

%token GT_SUBROUTINE GT_PATCH GT_SAMPLE GT_FLAT GT_SMOOTH GT_LAYOUT
%token GT_INVARIANT GT_HIGHP GT_MEDIUMP GT_LOWP GT_PRECISION

 /*
  * Deprecated GLSL keywords
  */

%token GT_ATTRIBUTE GT_VARYING

 /*
  * GLSL types
  */

%token GT_VEC2 GT_VEC3 GT_VEC4
%token GT_BVEC2 GT_BVEC3 GT_BVEC4
%token GT_DVEC2 GT_DVEC3 GT_DVEC4
%token GT_IVEC2 GT_IVEC3 GT_IVEC4
%token GT_UVEC2 GT_UVEC3 GT_UVEC4

%token GT_MAT2 GT_MAT2X2 GT_MAT2X3 GT_MAT2X4
%token GT_MAT3 GT_MAT3X2 GT_MAT3X3 GT_MAT3X4
%token GT_MAT4 GT_MAT4X2 GT_MAT4X3 GT_MAT4X4
%token GT_DMAT2 GT_DMAT2X2 GT_DMAT2X3 GT_DMAT2X4
%token GT_DMAT3 GT_DMAT3X2 GT_DMAT3X3 GT_DMAT3X4
%token GT_DMAT4 GT_DMAT4X2 GT_DMAT4X3 GT_DMAT4X4

%token GT_SAMPLER1DSHADOW GT_SAMPLER1DARRAY GT_SAMPLER1DARRAYSHADOW
%token GT_SAMPLER2DSHADOW GT_SAMPLER2DARRAY GT_SAMPLER2DARRAYSHADOW
%token GT_SAMPLER2DRECT GT_SAMPLER2DRECTSHADOW
%token GT_SAMPLER2DMS GT_SAMPLER2DMSARRAY
%token GT_SAMPLERCUBESHADOW GT_SAMPLERCUBEARRAY GT_SAMPLERCUBEARRAYSHADOW
%token GT_SAMPLERBUFFER

%token GT_ISAMPLER1D GT_ISAMPLER1DARRAY
%token GT_ISAMPLER2D GT_ISAMPLER2DARRAY
%token GT_ISAMPLER2DRECT
%token GT_ISAMPLER2DMS GT_ISAMPLER2DMSARRAY
%token GT_ISAMPLER3D
%token GT_ISAMPLERCUBE GT_ISAMPLERCUBEARRAY
%token GT_ISAMPLERBUFFER

%token GT_USAMPLER1D GT_USAMPLER1DARRAY
%token GT_USAMPLER2D GT_USAMPLER2DARRAY
%token GT_USAMPLER2DRECT
%token GT_USAMPLER2DMS GT_USAMPLER2DMSARRAY
%token GT_USAMPLER3D
%token GT_USAMPLERCUBE GT_USAMPLERCUBEARRAY
%token GT_USAMPLERBUFFER

 /*
  * GLSL keywords that are also in HLSL
  */

%token GHT_BOOL GHT_BREAK GHT_CASE GHT_CENTROID GHT_CONST GHT_CONTINUE
%token GHT_DEFAULT GHT_DISCARD GHT_DO GHT_DOUBLE GHT_ELSE GHT_FLOAT GHT_FOR
%token GHT_IF GHT_IN GHT_INOUT GHT_INT GHT_NOPERSPECTIVE GHT_OUT GHT_RETURN
%token GHT_SAMPLER1D GHT_SAMPLER2D GHT_SAMPLER3D GHT_SAMPLERCUBE GHT_STRUCT
%token GHT_SWITCH GHT_UINT GHT_UNIFORM GHT_VOID GHT_WHILE

 /*
  * HLSL keywords that are also in GLSL
  */

%token HGT_BOOL HGT_BREAK HGT_CASE HGT_CENTROID HGT_CONST HGT_CONTINUE
%token HGT_DEFAULT HGT_DISCARD HGT_DO HGT_DOUBLE HGT_ELSE HGT_FLOAT HGT_FOR
%token HGT_IF HGT_IN HGT_INOUT HGT_INT HGT_NOPERSPECTIVE HGT_OUT HGT_RETURN
%token HGT_SAMPLER1D HGT_SAMPLER2D HGT_SAMPLER3D HGT_SAMPLERCUBE HGT_STRUCT
%token HGT_SWITCH HGT_UINT HGT_UNIFORM HGT_VOID HGT_WHILE

 /*
  * HLSL keywords
  */

%token HT_APPENDSTRUCTUREDBUFFER HT_ASM HT_ASM_FRAGMENT HT_BLENDSTATE
%token HT_BUFFER HT_BYTEADDRESSBUFFER HT_CBUFFER
%token HT_COLUMN_MAJOR HT_COMPILE HT_COMPILE_FRAGMENT HT_COMPILESHADER
%token HT_COMPUTESHADER HT_CONSUMESTRUCTUREDBUFFER
%token HT_DEPTHSTENCILSTATE HT_DEPTHSTENCILVIEW
%token HT_DOMAINSHADER HT_DWORD HT_EXTERN HT_FALSE
%token HT_FXGROUP HT_GEOMETRYSHADER HT_GROUPSHARED HT_HALF HT_HULLSHADER
%token HT_INLINE HT_INPUTPATCH HT_INTERFACE HT_LINE
%token HT_LINEADJ HT_LINEAR HT_LINESTREAM HT_MATRIX HT_NAMESPACE
%token HT_NOINTERPOLATION HT_NULL HT_OUTPUTPATCH
%token HT_PACKOFFSET HT_PASS HT_PIXELFRAGMENT HT_PIXELSHADER HT_POINT
%token HT_POINTSTREAM HT_PRECISE HT_RASTERIZERSTATE HT_RENDERTARGETVIEW
%token HT_REGISTER HT_ROW_MAJOR HT_RWBUFFER HT_RWBYTEADDRESSBUFFER
%token HT_RWSTRUCTUREDBUFFER HT_RWTEXTURE1D HT_RWTEXTURE1DARRAY HT_RWTEXTURE2D
%token HT_RWTEXTURE2DARRAY HT_RWTEXTURE3D HT_SAMPLER
%token HT_SAMPLER_STATE HT_SAMPLERSTATE
%token HT_SAMPLERCOMPARISONSTATE HT_SHARED HT_SNORM HT_STATEBLOCK
%token HT_STATEBLOCK_STATE HT_STATIC HT_STRING
%token HT_STRUCTUREDBUFFER HT_TBUFFER HT_TECHNIQUE HT_TECHNIQUE10
%token HT_TECHNIQUE11XZ HT_TEXTURE HT_TEXTURE1D HT_TEXTURE1DARRAY HT_TEXTURE2D
%token HT_TEXTURE2DARRAY HT_TEXTURE2DMS HT_TEXTURE2DMSARRAY HT_TEXTURE3D
%token HT_TEXTURECUBE HT_TEXTURECUBEARRAY HT_TRUE HT_TYPEDEF HT_TRIANGLE
%token HT_TRIANGLEADJ HT_TRIANGLESTREAM HT_UNORM HT_VECTOR
%token HT_VERTEXFRAGMENT HT_VERTEXSHADER HT_VOLATILE

 /*
  * HLSL types
  */

%token HT_BOOL1 HT_BOOL1x1 HT_BOOL2x1 HT_BOOL3x1 HT_BOOL4x1
%token HT_BOOL2 HT_BOOL1x2 HT_BOOL2x2 HT_BOOL3x2 HT_BOOL4x2
%token HT_BOOL3 HT_BOOL1x3 HT_BOOL2x3 HT_BOOL3x3 HT_BOOL4x3
%token HT_BOOL4 HT_BOOL1x4 HT_BOOL2x4 HT_BOOL3x4 HT_BOOL4x4

%token HT_FLOAT1 HT_FLOAT1x1 HT_FLOAT2x1 HT_FLOAT3x1 HT_FLOAT4x1
%token HT_FLOAT2 HT_FLOAT1x2 HT_FLOAT2x2 HT_FLOAT3x2 HT_FLOAT4x2
%token HT_FLOAT3 HT_FLOAT1x3 HT_FLOAT2x3 HT_FLOAT3x3 HT_FLOAT4x3
%token HT_FLOAT4 HT_FLOAT1x4 HT_FLOAT2x4 HT_FLOAT3x4 HT_FLOAT4x4

%token HT_DOUBLE1 HT_DOUBLE1x1 HT_DOUBLE2x1 HT_DOUBLE3x1 HT_DOUBLE4x1
%token HT_DOUBLE2 HT_DOUBLE1x2 HT_DOUBLE2x2 HT_DOUBLE3x2 HT_DOUBLE4x2
%token HT_DOUBLE3 HT_DOUBLE1x3 HT_DOUBLE2x3 HT_DOUBLE3x3 HT_DOUBLE4x3
%token HT_DOUBLE4 HT_DOUBLE1x4 HT_DOUBLE2x4 HT_DOUBLE3x4 HT_DOUBLE4x4

%token HT_DWORD1 HT_DWORD1x1 HT_DWORD2x1 HT_DWORD3x1 HT_DWORD4x1
%token HT_DWORD2 HT_DWORD1x2 HT_DWORD2x2 HT_DWORD3x2 HT_DWORD4x2
%token HT_DWORD3 HT_DWORD1x3 HT_DWORD2x3 HT_DWORD3x3 HT_DWORD4x3
%token HT_DWORD4 HT_DWORD1x4 HT_DWORD2x4 HT_DWORD3x4 HT_DWORD4x4

%token HT_INT1 HT_INT1x1 HT_INT2x1 HT_INT3x1 HT_INT4x1
%token HT_INT2 HT_INT1x2 HT_INT2x2 HT_INT3x2 HT_INT4x2
%token HT_INT3 HT_INT1x3 HT_INT2x3 HT_INT3x3 HT_INT4x3
%token HT_INT4 HT_INT1x4 HT_INT2x4 HT_INT3x4 HT_INT4x4

%token HT_UINT1 HT_UINT1x1 HT_UINT2x1 HT_UINT3x1 HT_UINT4x1
%token HT_UINT2 HT_UINT1x2 HT_UINT2x2 HT_UINT3x2 HT_UINT4x2
%token HT_UINT3 HT_UINT1x3 HT_UINT2x3 HT_UINT3x3 HT_UINT4x3
%token HT_UINT4 HT_UINT1x4 HT_UINT2x4 HT_UINT3x4 HT_UINT4x4

 /*
  * HLSL preprocessor directives
  */

%token PREPROCESSOR_DEFINE PREPROCESSOR_ELIF PREPROCESSOR_ELSE
%token PREPROCESSOR_ENDIF PREPROCESSOR_ERROR PREPROCESSOR_IF
%token PREPROCESSOR_IFDEF PREPROCESSOR_IFNDEF PREPROCESSOR_INCLUDE
%token PREPROCESSOR_LINE PREPROCESSOR_PRAGMA PREPROCESSOR_UNDEF
%token PREPROCESSOR_REGION

 /*
  * HLSL reserved keywords
  */

%token HT_AUTO HT_CATCH HT_CHAR HT_CLASS HT_CONST_CAST
%token HT_DELETE HT_DYNAMIC_CAST HT_ENUM HT_EXPLICIT HT_FRIEND HT_GOTO HT_LONG
%token HT_MUTABLE HT_NEW HT_OPERATOR HT_PRIVATE HT_PROTECTED HT_PUBLIC
%token HT_REINTERPRET_CAST HT_SHORT HT_SIGNED HT_SIZEOF HT_STATIC_CAST
%token HT_TEMPLATE HT_THIS HT_THROW HT_TRY HT_TYPENAME HT_UNION HT_UNSIGNED
%token HT_USING HT_VIRTUAL

 /*
  * Language symbols
  */

%token T_INC T_DEC T_LE T_GE T_EQ T_NE T_LEFT T_RIGHT T_AND T_OR T_XOR
%token T_MULEQ T_DIVEQ T_MODEQ T_ADDEQ T_SUBEQ T_LEFTEQ T_RIGHTEQ T_ANDEQ
%token T_XOREQ T_OREQ

 /*
  * Temporary shit
  */

%token <fval> FLOAT
%token <sval> STRING NAME

 /*
  * Special tokens
  */

%token T_END 0
%token T_ERROR

 /*
  * Our entry point
  */

%start lolfx_file

%{
#include "gpu/lolfx-compiler.h"

#undef yylex
#define yylex mc.m_lexer->lex
%}

%%

 /*
  * Shortcuts for token groups
  */

group_glsl_keyword:
    GT_SUBROUTINE
  | GT_PATCH
  | GT_SAMPLE
  | GT_FLAT
  | GT_SMOOTH
  | GT_LAYOUT
  | GT_INVARIANT
  | GT_HIGHP
  | GT_MEDIUMP
  | GT_LOWP
  | GT_PRECISION
  | GT_ATTRIBUTE /* deprecated */
  | GT_VARYING /* deprecated */
    ;

group_glsl_type:
    GT_VEC2
  | GT_VEC3
  | GT_VEC4
  | GT_BVEC2
  | GT_BVEC3
  | GT_BVEC4
  | GT_DVEC2
  | GT_DVEC3
  | GT_DVEC4
  | GT_IVEC2
  | GT_IVEC3
  | GT_IVEC4
  | GT_UVEC2
  | GT_UVEC3
  | GT_UVEC4
  | GT_MAT2
  | GT_MAT2X2
  | GT_MAT2X3
  | GT_MAT2X4
  | GT_MAT3
  | GT_MAT3X2
  | GT_MAT3X3
  | GT_MAT3X4
  | GT_MAT4
  | GT_MAT4X2
  | GT_MAT4X3
  | GT_MAT4X4
  | GT_DMAT2
  | GT_DMAT2X2
  | GT_DMAT2X3
  | GT_DMAT2X4
  | GT_DMAT3
  | GT_DMAT3X2
  | GT_DMAT3X3
  | GT_DMAT3X4
  | GT_DMAT4
  | GT_DMAT4X2
  | GT_DMAT4X3
  | GT_DMAT4X4
  | GT_SAMPLER1DSHADOW
  | GT_SAMPLER1DARRAY
  | GT_SAMPLER1DARRAYSHADOW
  | GT_SAMPLER2DSHADOW
  | GT_SAMPLER2DARRAY
  | GT_SAMPLER2DARRAYSHADOW
  | GT_SAMPLER2DRECT
  | GT_SAMPLER2DRECTSHADOW
  | GT_SAMPLER2DMS
  | GT_SAMPLER2DMSARRAY
  | GT_SAMPLERCUBESHADOW
  | GT_SAMPLERCUBEARRAY
  | GT_SAMPLERCUBEARRAYSHADOW
  | GT_SAMPLERBUFFER
  | GT_ISAMPLER1D
  | GT_ISAMPLER1DARRAY
  | GT_ISAMPLER2D
  | GT_ISAMPLER2DARRAY
  | GT_ISAMPLER2DRECT
  | GT_ISAMPLER2DMS
  | GT_ISAMPLER2DMSARRAY
  | GT_ISAMPLER3D
  | GT_ISAMPLERCUBE
  | GT_ISAMPLERCUBEARRAY
  | GT_ISAMPLERBUFFER
  | GT_USAMPLER1D
  | GT_USAMPLER1DARRAY
  | GT_USAMPLER2D
  | GT_USAMPLER2DARRAY
  | GT_USAMPLER2DRECT
  | GT_USAMPLER2DMS
  | GT_USAMPLER2DMSARRAY
  | GT_USAMPLER3D
  | GT_USAMPLERCUBE
  | GT_USAMPLERCUBEARRAY
  | GT_USAMPLERBUFFER
    ;

group_glsl_keyword_in_hlsl:
    GHT_BOOL
  | GHT_BREAK
  | GHT_CASE
  | GHT_CENTROID
  | GHT_CONST
  | GHT_CONTINUE
  | GHT_DEFAULT
  | GHT_DISCARD
  | GHT_DO
  | GHT_DOUBLE
  | GHT_ELSE
  | GHT_FLOAT
  | GHT_FOR
  | GHT_IF
  | GHT_IN
  | GHT_INOUT
  | GHT_INT
  | GHT_NOPERSPECTIVE
  | GHT_OUT
  | GHT_RETURN
  | GHT_SAMPLER1D
  | GHT_SAMPLER2D
  | GHT_SAMPLER3D
  | GHT_SAMPLERCUBE
  | GHT_STRUCT
  | GHT_SWITCH
  | GHT_UINT
  | GHT_UNIFORM
  | GHT_VOID
  | GHT_WHILE
    ;

group_hlsl_keyword_in_glsl:
    HGT_BOOL
  | HGT_BREAK
  | HGT_CASE
  | HGT_CENTROID
  | HGT_CONST
  | HGT_CONTINUE
  | HGT_DEFAULT
  | HGT_DISCARD
  | HGT_DO
  | HGT_DOUBLE
  | HGT_ELSE
  | HGT_FLOAT
  | HGT_FOR
  | HGT_IF
  | HGT_IN
  | HGT_INOUT
  | HGT_INT
  | HGT_NOPERSPECTIVE
  | HGT_OUT
  | HGT_RETURN
  | HGT_SAMPLER1D
  | HGT_SAMPLER2D
  | HGT_SAMPLER3D
  | HGT_SAMPLERCUBE
  | HGT_STRUCT
  | HGT_SWITCH
  | HGT_UINT
  | HGT_UNIFORM
  | HGT_VOID
  | HGT_WHILE
    ;

group_hlsl_keyword:
    HT_APPENDSTRUCTUREDBUFFER
  | HT_ASM
  | HT_ASM_FRAGMENT
  | HT_BLENDSTATE
  | HT_BUFFER
  | HT_BYTEADDRESSBUFFER
  | HT_CBUFFER
  | HT_COLUMN_MAJOR
  | HT_COMPILE
  | HT_COMPILE_FRAGMENT
  | HT_COMPILESHADER
  | HT_COMPUTESHADER
  | HT_CONSUMESTRUCTUREDBUFFER
  | HT_DEPTHSTENCILSTATE
  | HT_DEPTHSTENCILVIEW
  | HT_DOMAINSHADER
  | HT_DWORD
  | HT_EXTERN
  | HT_FALSE
  | HT_FXGROUP
  | HT_GEOMETRYSHADER
  | HT_GROUPSHARED
  | HT_HALF
  | HT_HULLSHADER
  | HT_INLINE
  | HT_INPUTPATCH
  | HT_INTERFACE
  | HT_LINE
  | HT_LINEADJ
  | HT_LINEAR
  | HT_LINESTREAM
  | HT_MATRIX
  | HT_NAMESPACE
  | HT_NOINTERPOLATION
  | HT_NULL
  | HT_OUTPUTPATCH
  | HT_PACKOFFSET
  | HT_PASS
  | HT_PIXELFRAGMENT
  | HT_PIXELSHADER
  | HT_POINT
  | HT_POINTSTREAM
  | HT_PRECISE
  | HT_RASTERIZERSTATE
  | HT_RENDERTARGETVIEW
  | HT_REGISTER
  | HT_ROW_MAJOR
  | HT_RWBUFFER
  | HT_RWBYTEADDRESSBUFFER
  | HT_RWSTRUCTUREDBUFFER
  | HT_RWTEXTURE1D
  | HT_RWTEXTURE1DARRAY
  | HT_RWTEXTURE2D
  | HT_RWTEXTURE2DARRAY
  | HT_RWTEXTURE3D
  | HT_SAMPLER
  | HT_SAMPLER_STATE
  | HT_SAMPLERSTATE
  | HT_SAMPLERCOMPARISONSTATE
  | HT_SHARED
  | HT_SNORM
  | HT_STATEBLOCK
  | HT_STATEBLOCK_STATE
  | HT_STATIC
  | HT_STRING
  | HT_STRUCTUREDBUFFER
  | HT_TBUFFER
  | HT_TECHNIQUE
  | HT_TECHNIQUE10
  | HT_TECHNIQUE11XZ
  | HT_TEXTURE
  | HT_TEXTURE1D
  | HT_TEXTURE1DARRAY
  | HT_TEXTURE2D
  | HT_TEXTURE2DARRAY
  | HT_TEXTURE2DMS
  | HT_TEXTURE2DMSARRAY
  | HT_TEXTURE3D
  | HT_TEXTURECUBE
  | HT_TEXTURECUBEARRAY
  | HT_TRUE
  | HT_TYPEDEF
  | HT_TRIANGLE
  | HT_TRIANGLEADJ
  | HT_TRIANGLESTREAM
  | HT_UNORM
  | HT_VECTOR
  | HT_VERTEXFRAGMENT
  | HT_VERTEXSHADER
  | HT_VOLATILE
    ;

group_hlsl_type:
    HT_BOOL1
  | HT_BOOL1x1
  | HT_BOOL2x1
  | HT_BOOL3x1
  | HT_BOOL4x1
  | HT_BOOL2
  | HT_BOOL1x2
  | HT_BOOL2x2
  | HT_BOOL3x2
  | HT_BOOL4x2
  | HT_BOOL3
  | HT_BOOL1x3
  | HT_BOOL2x3
  | HT_BOOL3x3
  | HT_BOOL4x3
  | HT_BOOL4
  | HT_BOOL1x4
  | HT_BOOL2x4
  | HT_BOOL3x4
  | HT_BOOL4x4
  | HT_FLOAT1
  | HT_FLOAT1x1
  | HT_FLOAT2x1
  | HT_FLOAT3x1
  | HT_FLOAT4x1
  | HT_FLOAT2
  | HT_FLOAT1x2
  | HT_FLOAT2x2
  | HT_FLOAT3x2
  | HT_FLOAT4x2
  | HT_FLOAT3
  | HT_FLOAT1x3
  | HT_FLOAT2x3
  | HT_FLOAT3x3
  | HT_FLOAT4x3
  | HT_FLOAT4
  | HT_FLOAT1x4
  | HT_FLOAT2x4
  | HT_FLOAT3x4
  | HT_FLOAT4x4
  | HT_DOUBLE1
  | HT_DOUBLE1x1
  | HT_DOUBLE2x1
  | HT_DOUBLE3x1
  | HT_DOUBLE4x1
  | HT_DOUBLE2
  | HT_DOUBLE1x2
  | HT_DOUBLE2x2
  | HT_DOUBLE3x2
  | HT_DOUBLE4x2
  | HT_DOUBLE3
  | HT_DOUBLE1x3
  | HT_DOUBLE2x3
  | HT_DOUBLE3x3
  | HT_DOUBLE4x3
  | HT_DOUBLE4
  | HT_DOUBLE1x4
  | HT_DOUBLE2x4
  | HT_DOUBLE3x4
  | HT_DOUBLE4x4
  | HT_DWORD1
  | HT_DWORD1x1
  | HT_DWORD2x1
  | HT_DWORD3x1
  | HT_DWORD4x1
  | HT_DWORD2
  | HT_DWORD1x2
  | HT_DWORD2x2
  | HT_DWORD3x2
  | HT_DWORD4x2
  | HT_DWORD3
  | HT_DWORD1x3
  | HT_DWORD2x3
  | HT_DWORD3x3
  | HT_DWORD4x3
  | HT_DWORD4
  | HT_DWORD1x4
  | HT_DWORD2x4
  | HT_DWORD3x4
  | HT_DWORD4x4
  | HT_INT1
  | HT_INT1x1
  | HT_INT2x1
  | HT_INT3x1
  | HT_INT4x1
  | HT_INT2
  | HT_INT1x2
  | HT_INT2x2
  | HT_INT3x2
  | HT_INT4x2
  | HT_INT3
  | HT_INT1x3
  | HT_INT2x3
  | HT_INT3x3
  | HT_INT4x3
  | HT_INT4
  | HT_INT1x4
  | HT_INT2x4
  | HT_INT3x4
  | HT_INT4x4
  | HT_UINT1
  | HT_UINT1x1
  | HT_UINT2x1
  | HT_UINT3x1
  | HT_UINT4x1
  | HT_UINT2
  | HT_UINT1x2
  | HT_UINT2x2
  | HT_UINT3x2
  | HT_UINT4x2
  | HT_UINT3
  | HT_UINT1x3
  | HT_UINT2x3
  | HT_UINT3x3
  | HT_UINT4x3
  | HT_UINT4
  | HT_UINT1x4
  | HT_UINT2x4
  | HT_UINT3x4
  | HT_UINT4x4
    ;

group_hlsl_preprocessor:
    PREPROCESSOR_DEFINE
  | PREPROCESSOR_ELIF
  | PREPROCESSOR_ELSE
  | PREPROCESSOR_ENDIF
  | PREPROCESSOR_ERROR
  | PREPROCESSOR_IF
  | PREPROCESSOR_IFDEF
  | PREPROCESSOR_IFNDEF
  | PREPROCESSOR_INCLUDE
  | PREPROCESSOR_LINE
  | PREPROCESSOR_PRAGMA
  | PREPROCESSOR_UNDEF
  | PREPROCESSOR_REGION
    ;

group_hlsl_reserved:
    HT_AUTO
  | HT_CATCH
  | HT_CHAR
  | HT_CLASS
  | HT_CONST_CAST
  | HT_DELETE
  | HT_DYNAMIC_CAST
  | HT_ENUM
  | HT_EXPLICIT
  | HT_FRIEND
  | HT_GOTO
  | HT_LONG
  | HT_MUTABLE
  | HT_NEW
  | HT_OPERATOR
  | HT_PRIVATE
  | HT_PROTECTED
  | HT_PUBLIC
  | HT_REINTERPRET_CAST
  | HT_SHORT
  | HT_SIGNED
  | HT_SIZEOF
  | HT_STATIC_CAST
  | HT_TEMPLATE
  | HT_THIS
  | HT_THROW
  | HT_TRY
  | HT_TYPENAME
  | HT_UNION
  | HT_UNSIGNED
  | HT_USING
  | HT_VIRTUAL
    ;

group_language_symbols:
    T_INC
  | T_DEC
  | T_LE
  | T_GE
  | T_EQ
  | T_NE
  | T_LEFT
  | T_RIGHT
  | T_AND
  | T_OR
  | T_XOR
  | T_MULEQ
  | T_DIVEQ
  | T_MODEQ
  | T_ADDEQ
  | T_SUBEQ
  | T_LEFTEQ
  | T_RIGHTEQ
  | T_ANDEQ
  | T_XOREQ
  | T_OREQ
    ;

 /*
  * Grammar for LolFx files
  */

lolfx_file:
    lolfx_section_list
    ;

lolfx_section_list:
    lolfx_section
  | lolfx_section lolfx_section_list
    ;

lolfx_section:
    lolfx_technique
  | lolfx_shader
    ;

 /*
  * Grammar for techniques
  */

lolfx_technique:
    HT_TECHNIQUE lolfx_identifier '{' pass_list '}' { std::cout << "New tech " << std::endl; }
    ;

 /*
  * The valid LolFx identifiers
  */

lolfx_identifier:
    IDENTIFIER     { std::cout << "New name " << $1 << std::endl; }
  | FIELDSELECTION { std::cout << "New name " << $1 << std::endl; }
    ;

 /*
  * Grammar for passes
  */

pass_list:
    pass
  | pass pass_list
    ;

pass:
    HT_PASS lolfx_identifier '{' pass_stmt_list '}' { std::cout << "New pass " << std::endl; }
    ;

pass_stmt_list:
    pass_stmt
  | pass_stmt pass_stmt_list
    ;

 /* Shitty shit */
pass_stmt:
    ';'
  | IDENTIFIER '=' IDENTIFIER ';'
  | IDENTIFIER '=' INTCONSTANT ';'
  | IDENTIFIER '=' BOOLCONSTANT ';'
  | IDENTIFIER '=' HT_FALSE ';'
  | IDENTIFIER '=' HT_TRUE ';'
  | IDENTIFIER '[' INTCONSTANT ']' '=' IDENTIFIER ';'
  | IDENTIFIER '[' INTCONSTANT ']' '=' INTCONSTANT ';'
  | HT_TEXTURE '[' INTCONSTANT ']' '=' IDENTIFIER ';'
  | HT_TEXTURE '[' INTCONSTANT ']' '=' HT_NULL ';'
  | HT_DOMAINSHADER '=' IDENTIFIER ';'
  | HT_GEOMETRYSHADER '=' IDENTIFIER ';'
  | HT_HULLSHADER '=' IDENTIFIER ';'
  | HT_PIXELSHADER '=' IDENTIFIER ';'
  | HT_VERTEXSHADER '=' IDENTIFIER ';'
    ;

 /*
  * Grammar for shaders
  */

lolfx_shader:
    lolfx_shader_declaration glsl_translation_unit
    ;

lolfx_shader_declaration:
    '[' lolfx_shader_name_list ']' { std::cout << "new shader" << std::endl; }
    ;

lolfx_shader_name_list:
    lolfx_shader_name '.' lolfx_shader_name_list
  | lolfx_shader_name
    ;

lolfx_shader_name:
    lolfx_identifier
    ;

 /*
  * Grammar for GLSL
  */

glsl_variable_identifier:
    lolfx_identifier
    ;

glsl_primary_expression:
    glsl_variable_identifier
  | INTCONSTANT
  | UINTCONSTANT
  | FLOATCONSTANT
  | BOOLCONSTANT
  | '(' glsl_expression ')'
    ;

glsl_postfix_expression:
    glsl_primary_expression
  | glsl_postfix_expression '[' glsl_integer_expression ']'
  | glsl_function_call
  | glsl_postfix_expression '.' FIELDSELECTION
  | glsl_postfix_expression T_INC
  | glsl_postfix_expression T_DEC
    ;

glsl_integer_expression:
    glsl_expression
    ;

glsl_function_call:
    glsl_function_call_or_method
    ;

glsl_function_call_or_method:
    glsl_function_call_generic
  | glsl_postfix_expression '.' glsl_function_call_generic
    ;

glsl_function_call_generic:
    glsl_function_call_header_with_parameters ')'
  | glsl_function_call_header_no_parameters ')'
    ;

glsl_function_call_header_no_parameters:
    glsl_function_call_header GHT_VOID
  | glsl_function_call_header
    ;

glsl_function_call_header_with_parameters:
    glsl_function_call_header glsl_assignment_expression
  | glsl_function_call_header_with_parameters ',' glsl_assignment_expression
    ;

glsl_function_call_header:
    glsl_function_identifier '('
    ;

glsl_function_identifier:
    glsl_type_specifier
  | IDENTIFIER
  | FIELDSELECTION
    ;

glsl_unary_expression:
    glsl_postfix_expression
  | T_INC glsl_unary_expression
  | T_DEC glsl_unary_expression
  | glsl_unary_operator glsl_unary_expression
    ;

glsl_unary_operator:
    '+'
  | '-'
  | '!'
  | '~'
    ;

glsl_multiplicative_expression:
    glsl_unary_expression
  | glsl_multiplicative_expression '*' glsl_unary_expression
  | glsl_multiplicative_expression '/' glsl_unary_expression
  | glsl_multiplicative_expression '%' glsl_unary_expression
    ;

glsl_additive_expression:
    glsl_multiplicative_expression
  | glsl_additive_expression '+' glsl_multiplicative_expression
  | glsl_additive_expression '-' glsl_multiplicative_expression
    ;

glsl_shift_expression:
    glsl_additive_expression
  | glsl_shift_expression T_LEFT glsl_additive_expression
  | glsl_shift_expression T_RIGHT glsl_additive_expression
    ;

glsl_relational_expression:
    glsl_shift_expression
  | glsl_relational_expression '<' glsl_shift_expression
  | glsl_relational_expression '>' glsl_shift_expression
  | glsl_relational_expression T_LE glsl_shift_expression
  | glsl_relational_expression T_GE glsl_shift_expression
    ;

glsl_equality_expression:
    glsl_relational_expression
  | glsl_equality_expression T_EQ glsl_relational_expression
  | glsl_equality_expression T_NE glsl_relational_expression
    ;

glsl_and_expression:
    glsl_equality_expression
  | glsl_and_expression '&' glsl_equality_expression
    ;

glsl_exclusive_or_expression:
    glsl_and_expression
  | glsl_exclusive_or_expression '^' glsl_and_expression
    ;

glsl_inclusive_or_expression:
    glsl_exclusive_or_expression
  | glsl_inclusive_or_expression '|' glsl_exclusive_or_expression
    ;

glsl_logical_and_expression:
    glsl_inclusive_or_expression
  | glsl_logical_and_expression T_AND glsl_inclusive_or_expression
    ;

glsl_logical_xor_expression:
    glsl_logical_and_expression
  | glsl_logical_xor_expression T_XOR glsl_logical_and_expression
    ;

glsl_logical_or_expression:
    glsl_logical_xor_expression
  | glsl_logical_or_expression T_OR glsl_logical_xor_expression
    ;

glsl_conditional_expression:
    glsl_logical_or_expression
  | glsl_logical_or_expression '?' glsl_expression ':' glsl_assignment_expression
    ;

glsl_assignment_expression:
    glsl_conditional_expression
  | glsl_unary_expression glsl_assignment_operator glsl_assignment_expression
    ;

glsl_assignment_operator:
    '='
  | T_MULEQ
  | T_DIVEQ
  | T_MODEQ
  | T_ADDEQ
  | T_SUBEQ
  | T_LEFTEQ
  | T_RIGHTEQ
  | T_ANDEQ
  | T_XOREQ
  | T_OREQ
    ;

glsl_expression:
    glsl_assignment_expression
  | glsl_expression ',' glsl_assignment_expression
    ;

glsl_constant_expression:
    glsl_conditional_expression
    ;

glsl_declaration:
    glsl_function_prototype ';'
  | glsl_init_declarator_list ';'
  | GT_PRECISION glsl_precision_qualifier glsl_type_specifier_no_prec ';'
  | glsl_type_qualifier lolfx_identifier '{' glsl_struct_declaration_list '}' ';'
  | glsl_type_qualifier lolfx_identifier '{' glsl_struct_declaration_list '}' lolfx_identifier ';'
  | glsl_type_qualifier lolfx_identifier '{' glsl_struct_declaration_list '}' lolfx_identifier '[' ']' ';'
  | glsl_type_qualifier lolfx_identifier '{' glsl_struct_declaration_list '}' lolfx_identifier '[' glsl_constant_expression ']' ';'
  | glsl_type_qualifier ';'
    ;

glsl_function_prototype:
    glsl_function_declarator ')'
    ;

glsl_function_declarator:
    glsl_function_header
  | glsl_function_header_with_parameters
    ;

glsl_function_header_with_parameters:
    glsl_function_header lolfx_parameter_declaration
  | glsl_function_header_with_parameters ',' lolfx_parameter_declaration
    ;

glsl_function_header:
    glsl_fully_specified_type lolfx_identifier '('
    ;

glsl_parameter_declarator:
    glsl_type_specifier lolfx_identifier
  | glsl_type_specifier lolfx_identifier '[' glsl_constant_expression ']'
    ;

lolfx_parameter_declaration:
    glsl_parameter_declaration ':' lolfx_identifier /* HLSL only */
  | glsl_parameter_declaration
    ;

glsl_parameter_declaration:
    glsl_parameter_type_qualifier lolfx_parameter_qualifier glsl_parameter_declarator
  | lolfx_parameter_qualifier glsl_parameter_declarator
  | glsl_parameter_type_qualifier lolfx_parameter_qualifier glsl_parameter_type_specifier
  | lolfx_parameter_qualifier glsl_parameter_type_specifier
    ;

lolfx_parameter_qualifier:
    glsl_parameter_qualifier
  | GHT_UNIFORM /* HLSL only */
    ;

glsl_parameter_qualifier:
    /* empty */
  | GHT_IN
  | GHT_OUT
  | GHT_INOUT
  | GT_ATTRIBUTE /* deprecated */
  | GT_VARYING /* deprecated */
    ;

glsl_parameter_type_specifier:
    glsl_type_specifier
    ;

glsl_init_declarator_list:
    glsl_single_declaration
  | glsl_init_declarator_list ',' lolfx_identifier
  | glsl_init_declarator_list ',' lolfx_identifier '[' ']'
  | glsl_init_declarator_list ',' lolfx_identifier '[' glsl_constant_expression ']'
  | glsl_init_declarator_list ',' lolfx_identifier '[' ']' '=' glsl_initializer
  | glsl_init_declarator_list ',' lolfx_identifier '[' glsl_constant_expression ']' '=' glsl_initializer
  | glsl_init_declarator_list ',' lolfx_identifier '=' glsl_initializer
    ;

glsl_single_declaration:
    glsl_fully_specified_type
  | glsl_fully_specified_type lolfx_identifier
  | glsl_fully_specified_type lolfx_identifier '[' ']'
  | glsl_fully_specified_type lolfx_identifier '[' glsl_constant_expression ']'
  | glsl_fully_specified_type lolfx_identifier '[' ']' '=' glsl_initializer
  | glsl_fully_specified_type lolfx_identifier '[' glsl_constant_expression ']' '=' glsl_initializer
  | glsl_fully_specified_type lolfx_identifier '=' glsl_initializer
  | GT_INVARIANT lolfx_identifier
    ;

glsl_fully_specified_type:
    glsl_type_specifier
  | glsl_type_qualifier glsl_type_specifier
    ;

glsl_invariant_qualifier:
    GT_INVARIANT
    ;

glsl_interpolation_qualifier:
    GT_SMOOTH
  | GT_FLAT
  | GHT_NOPERSPECTIVE
    ;

glsl_layout_qualifier:
    GT_LAYOUT '(' glsl_layout_qualifier_id_list ')'
    ;

glsl_layout_qualifier_id_list:
    glsl_layout_qualifier_id
  | glsl_layout_qualifier_id_list ',' glsl_layout_qualifier_id
    ;

glsl_layout_qualifier_id:
    lolfx_identifier
  | lolfx_identifier '=' INTCONSTANT
    ;

glsl_parameter_type_qualifier:
    GHT_CONST
    ;

glsl_type_qualifier:
    glsl_storage_qualifier
  | glsl_layout_qualifier
  | glsl_layout_qualifier glsl_storage_qualifier
  | glsl_interpolation_qualifier glsl_storage_qualifier
  | glsl_interpolation_qualifier
  | glsl_invariant_qualifier glsl_storage_qualifier
  | glsl_invariant_qualifier glsl_interpolation_qualifier glsl_storage_qualifier
  | glsl_invariant_qualifier
    ;

glsl_storage_qualifier:
    GHT_CONST
  | GHT_IN
  | GHT_OUT
  | GHT_CENTROID GHT_IN
  | GHT_CENTROID GHT_OUT
  | GT_PATCH GHT_IN
  | GT_PATCH GHT_OUT
  | GT_SAMPLE GHT_IN
  | GT_SAMPLE GHT_OUT
  | GHT_UNIFORM
  | GT_ATTRIBUTE /* deprecated */
  | GT_VARYING /* deprecated */
    ;

glsl_type_specifier:
    glsl_type_specifier_no_prec
  | glsl_precision_qualifier glsl_type_specifier_no_prec
    ;

glsl_type_specifier_no_prec:
    lolfx_type_specifier_nonarray
  | lolfx_type_specifier_nonarray '[' ']'
  | lolfx_type_specifier_nonarray '[' glsl_constant_expression ']'

lolfx_type_specifier_nonarray:
    glsl_type_specifier_nonarray
  | group_hlsl_type /* HLSL only */
    ;

glsl_type_specifier_nonarray:
    GHT_VOID
  | GHT_FLOAT
  | GHT_DOUBLE
  | GHT_INT
  | GHT_UINT
  | GHT_BOOL
  | group_glsl_type
  | GHT_SAMPLER1D
  | GHT_SAMPLER2D
  | GHT_SAMPLER3D
  | GHT_SAMPLERCUBE
  | glsl_struct_specifier
  | TYPENAME
    ;

glsl_precision_qualifier:
    GT_HIGHP
  | GT_MEDIUMP
  | GT_LOWP
    ;

glsl_struct_specifier:
    GHT_STRUCT lolfx_identifier '{' glsl_struct_declaration_list '}'
  | GHT_STRUCT '{' glsl_struct_declaration_list '}'
    ;

glsl_struct_declaration_list:
    glsl_struct_declaration
  | glsl_struct_declaration_list glsl_struct_declaration
    ;

glsl_struct_declaration:
    glsl_type_specifier glsl_struct_declarator_list ';'
  | glsl_type_qualifier glsl_type_specifier glsl_struct_declarator_list ';'
    ;

glsl_struct_declarator_list:
    glsl_struct_declarator
  | glsl_struct_declarator_list ',' glsl_struct_declarator
    ;

glsl_struct_declarator:
    lolfx_identifier
  | lolfx_identifier '[' ']'
  | lolfx_identifier '[' glsl_constant_expression ']'
    ;

glsl_initializer:
    glsl_assignment_expression
    ;

glsl_declaration_statement:
    glsl_declaration
    ;

glsl_statement:
    glsl_compound_statement
  | glsl_simple_statement
    ;

glsl_simple_statement:
    glsl_declaration_statement
  | glsl_expression_statement
  | glsl_selection_statement
  | glsl_switch_statement
  | glsl_case_label
  | glsl_iteration_statement
  | glsl_jump_statement
    ;

glsl_compound_statement:
    '{' '}'
  | '{' glsl_statement_list '}'
    ;

glsl_statement_no_new_scope:
    glsl_compound_statement_no_new_scope
  | glsl_simple_statement
    ;

glsl_compound_statement_no_new_scope:
    '{' '}'
  | '{' glsl_statement_list '}'
    ;

glsl_statement_list:
    glsl_statement
  | glsl_statement_list glsl_statement
    ;

glsl_expression_statement:
    ';'
  | glsl_expression ';'
    ;

glsl_selection_statement:
    GHT_IF '(' glsl_expression ')' glsl_selection_rest_statement
    ;

glsl_selection_rest_statement:
    glsl_statement GHT_ELSE glsl_statement
  | glsl_statement
    ;

glsl_condition:
    glsl_expression
  | glsl_fully_specified_type lolfx_identifier '=' glsl_initializer
    ;

glsl_switch_statement:
    GHT_SWITCH '(' glsl_expression ')' '{' glsl_switch_statement_list '}'
    ;

glsl_switch_statement_list:
    /* nothing */
  | glsl_statement_list
    ;

glsl_case_label:
    GHT_CASE glsl_expression ':'
  | GHT_DEFAULT ':'
    ;

glsl_iteration_statement:
    GHT_WHILE '(' glsl_condition ')' glsl_statement_no_new_scope
  | GHT_DO glsl_statement GHT_WHILE '(' glsl_expression ')' ';'
  | GHT_FOR '(' glsl_for_init_statement glsl_for_rest_statement ')' glsl_statement_no_new_scope
    ;

glsl_for_init_statement:
    glsl_expression_statement
  | glsl_declaration_statement
    ;

glsl_conditionopt:
    glsl_condition
  | /* empty */
    ;

glsl_for_rest_statement:
    glsl_conditionopt ';'
  | glsl_conditionopt ';' glsl_expression
    ;

glsl_jump_statement:
    GHT_CONTINUE ';'
  | GHT_BREAK ';'
  | GHT_RETURN ';'
  | GHT_RETURN glsl_expression ';'
  | GHT_DISCARD ';'
    ;

glsl_translation_unit:
    glsl_external_declaration
  | glsl_translation_unit glsl_external_declaration
    ;

glsl_external_declaration:
    glsl_function_definition
  | glsl_declaration
    ;

glsl_function_definition:
    glsl_function_prototype glsl_compound_statement_no_new_scope
    ;

%%

void lol::LolFxParser::error(const LolFxParser::location_type& l,
                             const std::string& m)
{
    mc.Error(l, m);
}

