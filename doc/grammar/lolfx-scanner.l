%{
//
//  Lol Engine
//
//  Copyright © 2010-2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
using std::exit;
using std::malloc;
using std::realloc;
using std::free;
#include <iostream>

#include "core.h"
#include "gpu/lolfx-compiler.h"

typedef lol::LolFxParser::token token;
typedef lol::LolFxParser::token_type token_type;

#ifndef YY_DECL
#   define YY_DECL lol::LolFxParser::token_type \
        lol::LolFxScanner::lex(lol::LolFxParser::semantic_type* yylval, \
                               lol::LolFxParser::location_type* yylloc)
#endif

#define yyterminate() return token::T_END
#define YY_NO_UNISTD_H
#define YY_USER_ACTION yylloc->columns(yyleng);
%}

%option c++ prefix="LolFx"
%option batch yywrap nounput stack

%x C_COMMENT
%x CPP_COMMENT
%x PREPROCESSOR_COMMENT

%%

%{
    /* reset location at the beginning of yylex() */
    yylloc->step();

    int language_guess = INITIAL;
    m_pp_stack.Push(BlockIsTrue);
%}

 /*
  * GLSL keywords
  */

"subroutine"                 { return token::GT_SUBROUTINE; }
"patch"                      { return token::GT_PATCH; }
"sample"                     { return token::GT_SAMPLE; }
"flat"                       { return token::GT_FLAT; }
"smooth"                     { return token::GT_SMOOTH; }
"layout"                     { return token::GT_LAYOUT; }
"sampler1dshadow"            { return token::GT_SAMPLER1DSHADOW; }
"sampler2dshadow"            { return token::GT_SAMPLER2DSHADOW; }
"samplercubeshadow"          { return token::GT_SAMPLERCUBESHADOW; }
"sampler1darray"             { return token::GT_SAMPLER1DARRAY; }
"sampler2darray"             { return token::GT_SAMPLER2DARRAY; }
"sampler1darrayshadow"       { return token::GT_SAMPLER1DARRAYSHADOW; }
"sampler2darrayshadow"       { return token::GT_SAMPLER2DARRAYSHADOW; }
"isampler1d"                 { return token::GT_ISAMPLER1D; }
"isampler2d"                 { return token::GT_ISAMPLER2D; }
"isampler3d"                 { return token::GT_ISAMPLER3D; }
"isamplercube"               { return token::GT_ISAMPLERCUBE; }
"isampler1darray"            { return token::GT_ISAMPLER1DARRAY; }
"isampler2darray"            { return token::GT_ISAMPLER2DARRAY; }
"usampler1d"                 { return token::GT_USAMPLER1D; }
"usampler2d"                 { return token::GT_USAMPLER2D; }
"usampler3d"                 { return token::GT_USAMPLER3D; }
"usamplercube"               { return token::GT_USAMPLERCUBE; }
"usampler1darray"            { return token::GT_USAMPLER1DARRAY; }
"usampler2darray"            { return token::GT_USAMPLER2DARRAY; }
"sampler2drect"              { return token::GT_SAMPLER2DRECT; }
"sampler2drectshadow"        { return token::GT_SAMPLER2DRECTSHADOW; }
"isampler2drect"             { return token::GT_ISAMPLER2DRECT; }
"usampler2drect"             { return token::GT_USAMPLER2DRECT; }
"samplerbuffer"              { return token::GT_SAMPLERBUFFER; }
"isamplerbuffer"             { return token::GT_ISAMPLERBUFFER; }
"usamplerbuffer"             { return token::GT_USAMPLERBUFFER; }
"samplercubearray"           { return token::GT_SAMPLERCUBEARRAY; }
"samplercubearrayshadow"     { return token::GT_SAMPLERCUBEARRAYSHADOW; }
"isamplercubearray"          { return token::GT_ISAMPLERCUBEARRAY; }
"usamplercubearray"          { return token::GT_USAMPLERCUBEARRAY; }
"sampler2dms"                { return token::GT_SAMPLER2DMS; }
"isampler2dms"               { return token::GT_ISAMPLER2DMS; }
"usampler2dms"               { return token::GT_USAMPLER2DMS; }
"sampler2dmsarray"           { return token::GT_SAMPLER2DMSARRAY; }
"isampler2dmsarray"          { return token::GT_ISAMPLER2DMSARRAY; }
"usampler2dmsarray"          { return token::GT_USAMPLER2DMSARRAY; }

"highp"                      { return token::GT_HIGHP; }
"mediump"                    { return token::GT_MEDIUMP; }
"lowp"                       { return token::GT_LOWP; }
"precision"                  { return token::GT_PRECISION; }
"invariant"                  { return token::GT_INVARIANT; }

 /*
  * Deprecated GLSL keywords
  */

"attribute"                  { return token::GT_ATTRIBUTE; }
"varying"                    { return token::GT_VARYING; }

 /*
  * GLSL vector types
  */

"bvec2"                      { return token::GT_BVEC2; }
"bvec3"                      { return token::GT_BVEC3; }
"bvec4"                      { return token::GT_BVEC4; }

"ivec2"                      { return token::GT_IVEC2; }
"ivec3"                      { return token::GT_IVEC3; }
"ivec4"                      { return token::GT_IVEC4; }

"uvec2"                      { return token::GT_UVEC2; }
"uvec3"                      { return token::GT_UVEC3; }
"uvec4"                      { return token::GT_UVEC4; }

"vec2"                       { return token::GT_VEC2; }
"vec3"                       { return token::GT_VEC3; }
"vec4"                       { return token::GT_VEC4; }
"mat2"                       { return token::GT_MAT2; }
"mat2x2"                     { return token::GT_MAT2X2; }
"mat2x3"                     { return token::GT_MAT2X3; }
"mat2x4"                     { return token::GT_MAT2X4; }
"mat3"                       { return token::GT_MAT3; }
"mat3x2"                     { return token::GT_MAT3X2; }
"mat3x3"                     { return token::GT_MAT3X3; }
"mat3x4"                     { return token::GT_MAT3X4; }
"mat4"                       { return token::GT_MAT4; }
"mat4x2"                     { return token::GT_MAT4X2; }
"mat4x3"                     { return token::GT_MAT4X3; }
"mat4x4"                     { return token::GT_MAT4X4; }

"dvec2"                      { return token::GT_DVEC2; }
"dvec3"                      { return token::GT_DVEC3; }
"dvec4"                      { return token::GT_DVEC4; }
"dmat2"                      { return token::GT_DMAT2; }
"dmat2x2"                    { return token::GT_DMAT2X2; }
"dmat2x3"                    { return token::GT_DMAT2X3; }
"dmat2x4"                    { return token::GT_DMAT2X4; }
"dmat3"                      { return token::GT_DMAT3; }
"dmat3x2"                    { return token::GT_DMAT3X2; }
"dmat3x3"                    { return token::GT_DMAT3X3; }
"dmat3x4"                    { return token::GT_DMAT3X4; }
"dmat4"                      { return token::GT_DMAT4; }
"dmat4x2"                    { return token::GT_DMAT4X2; }
"dmat4x3"                    { return token::GT_DMAT4X3; }
"dmat4x4"                    { return token::GT_DMAT4X4; }

 /*
  * GLSL constant values
  */

"true"                       { yylval->ival = 1; return token::BOOLCONSTANT; }
"false"                      { yylval->ival = 0; return token::BOOLCONSTANT; }

 /*
  * GLSL keywords that are also valid or reserved in HLSL
  * and HLSL keywords that are also valid or reserved in GLSL.
  */

"bool"                       { return token::GHT_BOOL; }
"break"                      { return token::GHT_BREAK; }
"case"                       { return token::GHT_CASE; }
"centroid"                   { return token::GHT_CENTROID; }
"const"                      { return token::GHT_CONST; }
"continue"                   { return token::GHT_CONTINUE; }
"default"                    { return token::GHT_DEFAULT; }
"discard"                    { return token::GHT_DISCARD; }
"do"                         { return token::GHT_DO; }
"double"                     { return token::GHT_DOUBLE; }
"else"                       { return token::GHT_ELSE; }
"float"                      { return token::GHT_FLOAT; }
"for"                        { return token::GHT_FOR; }
"if"                         { return token::GHT_IF; }
"in"                         { return token::GHT_IN; }
"inout"                      { return token::GHT_INOUT; }
"int"                        { return token::GHT_INT; }
"noperspective"              { return token::GHT_NOPERSPECTIVE; }
"out"                        { return token::GHT_OUT; }
"return"                     { return token::GHT_RETURN; }
"sampler1d"                  { return token::GHT_SAMPLER1D; }
"sampler2d"                  { return token::GHT_SAMPLER2D; }
"sampler3d"                  { return token::GHT_SAMPLER3D; }
"samplercube"                { return token::GHT_SAMPLERCUBE; }
"struct"                     { return token::GHT_STRUCT; }
"switch"                     { return token::GHT_SWITCH; }
"uint"                       { return token::GHT_UINT; }
"uniform"                    { return token::GHT_UNIFORM; }
"void"                       { return token::GHT_VOID; }
"while"                      { return token::GHT_WHILE; }

(?i:bool)                    { return token::HGT_BOOL; }
(?i:break)                   { return token::HGT_BREAK; }
(?i:case)                    { return token::HGT_CASE; }
(?i:centroid)                { return token::HGT_CENTROID; }
(?i:const)                   { return token::HGT_CONST; }
(?i:continue)                { return token::HGT_CONTINUE; }
(?i:default)                 { return token::HGT_DEFAULT; }
(?i:discard)                 { return token::HGT_DISCARD; }
(?i:do)                      { return token::HGT_DO; }
(?i:double)                  { return token::HGT_DOUBLE; }
(?i:else)                    { return token::HGT_ELSE; }
(?i:float)                   { return token::HGT_FLOAT; }
(?i:for)                     { return token::HGT_FOR; }
(?i:if)                      { return token::HGT_IF; }
(?i:in)                      { return token::HGT_IN; }
(?i:inout)                   { return token::HGT_INOUT; }
(?i:int)                     { return token::HGT_INT; }
(?i:noperspective)           { return token::HGT_NOPERSPECTIVE; }
(?i:out)                     { return token::HGT_OUT; }
(?i:return)                  { return token::HGT_RETURN; }
(?i:sampler1d)               { return token::HGT_SAMPLER1D; }
(?i:sampler2d)               { return token::HGT_SAMPLER2D; }
(?i:sampler3d)               { return token::HGT_SAMPLER3D; }
(?i:samplercube)             { return token::HGT_SAMPLERCUBE; }
(?i:struct)                  { return token::HGT_STRUCT; }
(?i:switch)                  { return token::HGT_SWITCH; }
(?i:uint)                    { return token::HGT_UINT; }
(?i:uniform)                 { return token::HGT_UNIFORM; }
(?i:void)                    { return token::HGT_VOID; }
(?i:while)                   { return token::HGT_WHILE; }

 /*
  * HLSL keywords
  * Gathered from http://msdn.microsoft.com/en-us/library/windows/desktop/bb509568%28v=vs.85%29.aspx
  */

(?i:appendstructuredbuffer)  { return token::HT_APPENDSTRUCTUREDBUFFER; }
(?i:asm)                     { return token::HT_ASM; }
(?i:asm_fragment)            { return token::HT_ASM_FRAGMENT; }
(?i:blendstate)              { return token::HT_BLENDSTATE; }
(?i:buffer)                  { return token::HT_BUFFER; }
(?i:byteaddressbuffer)       { return token::HT_BYTEADDRESSBUFFER; }
(?i:cbuffer)                 { return token::HT_CBUFFER; }
(?i:column_major)            { return token::HT_COLUMN_MAJOR; }
(?i:compile)                 { return token::HT_COMPILE; }
(?i:compile_fragment)        { return token::HT_COMPILE_FRAGMENT; }
(?i:compileshader)           { return token::HT_COMPILESHADER; }
(?i:computeshader)           { return token::HT_COMPUTESHADER; }
(?i:consumestructuredbuffer) { return token::HT_CONSUMESTRUCTUREDBUFFER; }
(?i:depthstencilstate)       { return token::HT_DEPTHSTENCILSTATE; }
(?i:depthstencilview)        { return token::HT_DEPTHSTENCILVIEW; }
(?i:domainshader)            { return token::HT_DOMAINSHADER; }
(?i:dword)                   { return token::HT_DWORD; }
(?i:extern)                  { return token::HT_EXTERN; }
(?i:false)                   { return token::HT_FALSE; }
(?i:fxgroup)                 { return token::HT_FXGROUP; }
(?i:geometryshader)          { return token::HT_GEOMETRYSHADER; }
(?i:groupshared)             { return token::HT_GROUPSHARED; }
(?i:half)                    { return token::HT_HALF; }
(?i:hullshader)              { return token::HT_HULLSHADER; }
(?i:inline)                  { return token::HT_INLINE; }
(?i:inputpatch)              { return token::HT_INPUTPATCH; }
(?i:interface)               { return token::HT_INTERFACE; }
(?i:line)                    { return token::HT_LINE; }
(?i:lineadj)                 { return token::HT_LINEADJ; }
(?i:linear)                  { return token::HT_LINEAR; }
(?i:linestream)              { return token::HT_LINESTREAM; }
(?i:matrix)                  { return token::HT_MATRIX; }
(?i:namespace)               { return token::HT_NAMESPACE; }
(?i:nointerpolation)         { return token::HT_NOINTERPOLATION; }
(?i:null)                    { return token::HT_NULL; }
(?i:outputpatch)             { return token::HT_OUTPUTPATCH; }
(?i:packoffset)              { return token::HT_PACKOFFSET; }
(?i:pass)                    { return token::HT_PASS; }
(?i:pixelfragment)           { return token::HT_PIXELFRAGMENT; }
(?i:pixelshader)             { return token::HT_PIXELSHADER; }
(?i:point)                   { return token::HT_POINT; }
(?i:pointstream)             { return token::HT_POINTSTREAM; }
(?i:precise)                 { return token::HT_PRECISE; }
(?i:rasterizerstate)         { return token::HT_RASTERIZERSTATE; }
(?i:rendertargetview)        { return token::HT_RENDERTARGETVIEW; }
(?i:register)                { return token::HT_REGISTER; }
(?i:row_major)               { return token::HT_ROW_MAJOR; }
(?i:rwbuffer)                { return token::HT_RWBUFFER; }
(?i:rwbyteaddressbuffer)     { return token::HT_RWBYTEADDRESSBUFFER; }
(?i:rwstructuredbuffer)      { return token::HT_RWSTRUCTUREDBUFFER; }
(?i:rwtexture1d)             { return token::HT_RWTEXTURE1D; }
(?i:rwtexture1darray)        { return token::HT_RWTEXTURE1DARRAY; }
(?i:rwtexture2d)             { return token::HT_RWTEXTURE2D; }
(?i:rwtexture2darray)        { return token::HT_RWTEXTURE2DARRAY; }
(?i:rwtexture3d)             { return token::HT_RWTEXTURE3D; }
(?i:sampler)                 { return token::HT_SAMPLER; }
(?i:sampler_state)           { return token::HT_SAMPLER_STATE; }
(?i:samplerstate)            { return token::HT_SAMPLERSTATE; }
(?i:samplercomparisonstate)  { return token::HT_SAMPLERCOMPARISONSTATE; }
(?i:shared)                  { return token::HT_SHARED; }
(?i:snorm)                   { return token::HT_SNORM; }
(?i:stateblock)              { return token::HT_STATEBLOCK; }
(?i:stateblock_state)        { return token::HT_STATEBLOCK_STATE; }
(?i:static)                  { return token::HT_STATIC; }
(?i:string)                  { return token::HT_STRING; }
(?i:structuredbuffer)        { return token::HT_STRUCTUREDBUFFER; }
(?i:tbuffer)                 { return token::HT_TBUFFER; }
(?i:technique)               { return token::HT_TECHNIQUE; }
(?i:technique10)             { return token::HT_TECHNIQUE10; }
(?i:technique11xz)           { return token::HT_TECHNIQUE11XZ; }
"texture"                    { return token::HT_TEXTURE; }
(?i:texture1d)               { return token::HT_TEXTURE1D; }
(?i:texture1darray)          { return token::HT_TEXTURE1DARRAY; }
(?i:texture2d)               { return token::HT_TEXTURE2D; }
(?i:texture2darray)          { return token::HT_TEXTURE2DARRAY; }
(?i:texture2dms)             { return token::HT_TEXTURE2DMS; }
(?i:texture2dmsarray)        { return token::HT_TEXTURE2DMSARRAY; }
(?i:texture3d)               { return token::HT_TEXTURE3D; }
(?i:texturecube)             { return token::HT_TEXTURECUBE; }
(?i:texturecubearray)        { return token::HT_TEXTURECUBEARRAY; }
(?i:true)                    { return token::HT_TRUE; }
(?i:typedef)                 { return token::HT_TYPEDEF; }
(?i:triangle)                { return token::HT_TRIANGLE; }
(?i:triangleadj)             { return token::HT_TRIANGLEADJ; }
(?i:trianglestream)          { return token::HT_TRIANGLESTREAM; }
(?i:unorm)                   { return token::HT_UNORM; }
(?i:vector)                  { return token::HT_VECTOR; }
(?i:vertexfragment)          { return token::HT_VERTEXFRAGMENT; }
(?i:vertexshader)            { return token::HT_VERTEXSHADER; }
(?i:volatile)                { return token::HT_VOLATILE; }

 /*
  * HLSL vector types
  * TODO: this can probably be factored somehow
  */

(?i:bool1)                   { return token::HT_BOOL1; }
(?i:bool1x1)                 { return token::HT_BOOL1x1; }
(?i:bool2x1)                 { return token::HT_BOOL2x1; }
(?i:bool3x1)                 { return token::HT_BOOL3x1; }
(?i:bool4x1)                 { return token::HT_BOOL4x1; }
(?i:bool2)                   { return token::HT_BOOL2; }
(?i:bool1x2)                 { return token::HT_BOOL1x2; }
(?i:bool2x2)                 { return token::HT_BOOL2x2; }
(?i:bool3x2)                 { return token::HT_BOOL3x2; }
(?i:bool4x2)                 { return token::HT_BOOL4x2; }
(?i:bool3)                   { return token::HT_BOOL3; }
(?i:bool1x3)                 { return token::HT_BOOL1x3; }
(?i:bool2x3)                 { return token::HT_BOOL2x3; }
(?i:bool3x3)                 { return token::HT_BOOL3x3; }
(?i:bool4x3)                 { return token::HT_BOOL4x3; }
(?i:bool4)                   { return token::HT_BOOL4; }
(?i:bool1x4)                 { return token::HT_BOOL1x4; }
(?i:bool2x4)                 { return token::HT_BOOL2x4; }
(?i:bool3x4)                 { return token::HT_BOOL3x4; }
(?i:bool4x4)                 { return token::HT_BOOL4x4; }

(?i:float1)                  { return token::HT_FLOAT1; }
(?i:float1x1)                { return token::HT_FLOAT1x1; }
(?i:float2x1)                { return token::HT_FLOAT2x1; }
(?i:float3x1)                { return token::HT_FLOAT3x1; }
(?i:float4x1)                { return token::HT_FLOAT4x1; }
(?i:float2)                  { return token::HT_FLOAT2; }
(?i:float1x2)                { return token::HT_FLOAT1x2; }
(?i:float2x2)                { return token::HT_FLOAT2x2; }
(?i:float3x2)                { return token::HT_FLOAT3x2; }
(?i:float4x2)                { return token::HT_FLOAT4x2; }
(?i:float3)                  { return token::HT_FLOAT3; }
(?i:float1x3)                { return token::HT_FLOAT1x3; }
(?i:float2x3)                { return token::HT_FLOAT2x3; }
(?i:float3x3)                { return token::HT_FLOAT3x3; }
(?i:float4x3)                { return token::HT_FLOAT4x3; }
(?i:float4)                  { return token::HT_FLOAT4; }
(?i:float1x4)                { return token::HT_FLOAT1x4; }
(?i:float2x4)                { return token::HT_FLOAT2x4; }
(?i:float3x4)                { return token::HT_FLOAT3x4; }
(?i:float4x4)                { return token::HT_FLOAT4x4; }

(?i:double1)                 { return token::HT_DOUBLE1; }
(?i:double1x1)               { return token::HT_DOUBLE1x1; }
(?i:double2x1)               { return token::HT_DOUBLE2x1; }
(?i:double3x1)               { return token::HT_DOUBLE3x1; }
(?i:double4x1)               { return token::HT_DOUBLE4x1; }
(?i:double2)                 { return token::HT_DOUBLE2; }
(?i:double1x2)               { return token::HT_DOUBLE1x2; }
(?i:double2x2)               { return token::HT_DOUBLE2x2; }
(?i:double3x2)               { return token::HT_DOUBLE3x2; }
(?i:double4x2)               { return token::HT_DOUBLE4x2; }
(?i:double3)                 { return token::HT_DOUBLE3; }
(?i:double1x3)               { return token::HT_DOUBLE1x3; }
(?i:double2x3)               { return token::HT_DOUBLE2x3; }
(?i:double3x3)               { return token::HT_DOUBLE3x3; }
(?i:double4x3)               { return token::HT_DOUBLE4x3; }
(?i:double4)                 { return token::HT_DOUBLE4; }
(?i:double1x4)               { return token::HT_DOUBLE1x4; }
(?i:double2x4)               { return token::HT_DOUBLE2x4; }
(?i:double3x4)               { return token::HT_DOUBLE3x4; }
(?i:double4x4)               { return token::HT_DOUBLE4x4; }

(?i:dword1)                  { return token::HT_DWORD1; }
(?i:dword1x1)                { return token::HT_DWORD1x1; }
(?i:dword2x1)                { return token::HT_DWORD2x1; }
(?i:dword3x1)                { return token::HT_DWORD3x1; }
(?i:dword4x1)                { return token::HT_DWORD4x1; }
(?i:dword2)                  { return token::HT_DWORD2; }
(?i:dword1x2)                { return token::HT_DWORD1x2; }
(?i:dword2x2)                { return token::HT_DWORD2x2; }
(?i:dword3x2)                { return token::HT_DWORD3x2; }
(?i:dword4x2)                { return token::HT_DWORD4x2; }
(?i:dword3)                  { return token::HT_DWORD3; }
(?i:dword1x3)                { return token::HT_DWORD1x3; }
(?i:dword2x3)                { return token::HT_DWORD2x3; }
(?i:dword3x3)                { return token::HT_DWORD3x3; }
(?i:dword4x3)                { return token::HT_DWORD4x3; }
(?i:dword4)                  { return token::HT_DWORD4; }
(?i:dword1x4)                { return token::HT_DWORD1x4; }
(?i:dword2x4)                { return token::HT_DWORD2x4; }
(?i:dword3x4)                { return token::HT_DWORD3x4; }
(?i:dword4x4)                { return token::HT_DWORD4x4; }

(?i:int1)                    { return token::HT_INT1; }
(?i:int1x1)                  { return token::HT_INT1x1; }
(?i:int2x1)                  { return token::HT_INT2x1; }
(?i:int3x1)                  { return token::HT_INT3x1; }
(?i:int4x1)                  { return token::HT_INT4x1; }
(?i:int2)                    { return token::HT_INT2; }
(?i:int1x2)                  { return token::HT_INT1x2; }
(?i:int2x2)                  { return token::HT_INT2x2; }
(?i:int3x2)                  { return token::HT_INT3x2; }
(?i:int4x2)                  { return token::HT_INT4x2; }
(?i:int3)                    { return token::HT_INT3; }
(?i:int1x3)                  { return token::HT_INT1x3; }
(?i:int2x3)                  { return token::HT_INT2x3; }
(?i:int3x3)                  { return token::HT_INT3x3; }
(?i:int4x3)                  { return token::HT_INT4x3; }
(?i:int4)                    { return token::HT_INT4; }
(?i:int1x4)                  { return token::HT_INT1x4; }
(?i:int2x4)                  { return token::HT_INT2x4; }
(?i:int3x4)                  { return token::HT_INT3x4; }
(?i:int4x4)                  { return token::HT_INT4x4; }

(?i:uint1)                   { return token::HT_UINT1; }
(?i:uint1x1)                 { return token::HT_UINT1x1; }
(?i:uint2x1)                 { return token::HT_UINT2x1; }
(?i:uint3x1)                 { return token::HT_UINT3x1; }
(?i:uint4x1)                 { return token::HT_UINT4x1; }
(?i:uint2)                   { return token::HT_UINT2; }
(?i:uint1x2)                 { return token::HT_UINT1x2; }
(?i:uint2x2)                 { return token::HT_UINT2x2; }
(?i:uint3x2)                 { return token::HT_UINT3x2; }
(?i:uint4x2)                 { return token::HT_UINT4x2; }
(?i:uint3)                   { return token::HT_UINT3; }
(?i:uint1x3)                 { return token::HT_UINT1x3; }
(?i:uint2x3)                 { return token::HT_UINT2x3; }
(?i:uint3x3)                 { return token::HT_UINT3x3; }
(?i:uint4x3)                 { return token::HT_UINT4x3; }
(?i:uint4)                   { return token::HT_UINT4; }
(?i:uint1x4)                 { return token::HT_UINT1x4; }
(?i:uint2x4)                 { return token::HT_UINT2x4; }
(?i:uint3x4)                 { return token::HT_UINT3x4; }
(?i:uint4x4)                 { return token::HT_UINT4x4; }

 /*
  * HLSL preprocessor directives
  * Gathered from http://msdn.microsoft.com/en-us/library/windows/desktop/bb943993%28v=vs.85%29.aspx
  */

^[ \t]*"#"[ \t]*"define".* {
    /* FIXME: this is very, very limited */
    char const *tmp = strstr(yytext, "define") + 6;
    while (*tmp == ' ' || *tmp == '\n')
        tmp++;
    if (*tmp == '\0')
        return token::PREPROCESSOR_DEFINE;
    char const *tmp2 = tmp;
    while (*tmp != ' ' && *tmp != '\n' && *tmp != '\0')
        tmp++;
    String key(tmp2, (int)(tmp - tmp2));
    while (*tmp == ' ' || *tmp == '\n')
        tmp++;
    String val(*tmp ? tmp : "1");

    Log::Debug("new macro: ‘%s’ = ‘%s’\n", key.C(), val.C());
    m_pp_defines[key] = val;
}

^[ \t]*"#"[ \t]*"elif" {
    if (m_pp_stack.Count() <= 1)
        return token::PREPROCESSOR_ELIF;
    m_pp_stack.Last() = BlockIsForeverFalse;
    BEGIN(PREPROCESSOR_COMMENT);
}

^[ \t]*"#"[ \t]*"else" {
    if (m_pp_stack.Count() <= 1)
        return token::PREPROCESSOR_ELSE;
    m_pp_stack.Last() = BlockIsForeverFalse;
    BEGIN(PREPROCESSOR_COMMENT);
}

^[ \t]*"#"[ \t]*"endif" {
    if (m_pp_stack.Count() <= 1)
        return token::PREPROCESSOR_ENDIF;
    m_pp_stack.Pop();
}

^[ \t]*"#"[ \t]*"error".*    { return token::PREPROCESSOR_ERROR; }

^[ \t]*"#"[ \t]*"if".* {
    if (IsExpressionTrue(strstr(yytext, "if") + 2))
    {
        m_pp_stack.Push(BlockIsTrue);
    }
    else
    {
        m_pp_stack.Push(BlockIsFalse);
        BEGIN(PREPROCESSOR_COMMENT);
    }
}

^[ \t]*"#"[ \t]*"ifdef" {
    /* FIXME: this is very, very limited */
    char const *tmp = strstr(yytext, "ifdef") + 5;
    while (*tmp == ' ' || *tmp == '\n')
        tmp++;
    if (*tmp == '\0')
        return token::PREPROCESSOR_IFDEF;
    char const *tmp2 = tmp;
    while (*tmp != ' ' && *tmp != '\n' && *tmp != '\0')
        tmp++;
    String key(tmp2, (int)(tmp - tmp2));
    if (m_pp_defines.has_key(key))
    {
        m_pp_stack.Push(BlockIsTrue);
    }
    else
    {
        m_pp_stack.Push(BlockIsFalse);
        BEGIN(PREPROCESSOR_COMMENT);
    }
}

^[ \t]*"#"[ \t]*"ifndef" {
    /* FIXME: this is very, very limited */
    char const *tmp = strstr(yytext, "ifndef") + 6;
    while (*tmp == ' ' || *tmp == '\n')
        tmp++;
    if (*tmp == '\0')
        return token::PREPROCESSOR_IFDEF;
    char const *tmp2 = tmp;
    while (*tmp != ' ' && *tmp != '\n' && *tmp != '\0')
        tmp++;
    String key(tmp2, (int)(tmp - tmp2));
    if (m_pp_defines.has_key(key))
    {
        m_pp_stack.Push(BlockIsFalse);
        BEGIN(PREPROCESSOR_COMMENT);
    }
    else
    {
        m_pp_stack.Push(BlockIsTrue);
    }
}

^[ \t]*"#"[ \t]*"include"    { return token::PREPROCESSOR_INCLUDE; }

^[ \t]*"#"[ \t]*"line".*     { /* ignore for now */ }

^[ \t]*"#"[ \t]*"pragma".*   { /* ignore for now */ }

^[ \t]*"#"[ \t]*"undef" {
    /* FIXME: this is very, very limited */
    char const *tmp = strstr(yytext, "undef") + 5;
    while (*tmp == ' ' || *tmp == '\n')
        tmp++;
    if (*tmp == '\0')
        return token::PREPROCESSOR_UNDEF;
    char const *tmp2 = tmp;
    while (*tmp != ' ' && *tmp != '\n' && *tmp != '\0')
        tmp++;
    String key(tmp2, (int)(tmp - tmp2));

    Log::Debug("undef macro: ‘%s’\n", key.C());
    m_pp_defines.remove(key);
}

<PREPROCESSOR_COMMENT>^[ \t]*"#"[ \t]*(if|ifdef|ifndef)[ \t]+.*$ {
    m_pp_stack.Push(BlockIsForeverFalse);
}
<PREPROCESSOR_COMMENT>^[ \t]*"#"[ \t]*"else" {
    switch (m_pp_stack.Last())
    {
        case BlockIsFalse:
            m_pp_stack.Last() = BlockIsTrue;
            BEGIN(INITIAL);
            break;
        case BlockIsForeverFalse:
            break;
        case BlockIsTrue:
            m_pp_stack.Last() = BlockIsForeverFalse;
            break;
    }
}
<PREPROCESSOR_COMMENT>^[ \t]*"#"[ \t]*"elif" {
    switch (m_pp_stack.Last())
    {
        case BlockIsFalse:
            if (IsExpressionTrue(strstr(yytext, "elif") + 4))
            {
                m_pp_stack.Last() = BlockIsTrue;
                BEGIN(INITIAL);
            }
            break;
        case BlockIsForeverFalse:
            break;
        case BlockIsTrue:
            m_pp_stack.Last() = BlockIsForeverFalse;
            break;
    }
}
<PREPROCESSOR_COMMENT>^[ \t]*"#"[ \t]*"endif" {
    m_pp_stack.Pop();
    if (m_pp_stack.Last() == BlockIsTrue)
        BEGIN(INITIAL);
}
<PREPROCESSOR_COMMENT>\n     { yylloc->lines(1); }
<PREPROCESSOR_COMMENT>[^/#]* { }
<PREPROCESSOR_COMMENT>"/*"   { BEGIN(C_COMMENT); }
<PREPROCESSOR_COMMENT>[/#]   { }

 /*
  * GLSL preprocessor directives
  */

^[ \t]*"#"[ \t]*"version"[^\n]* { /* ignore for now */ }

 /*
  * HLSL reserved keywords
  * Gathered from http://msdn.microsoft.com/en-us/library/windows/desktop/bb509569%28v=vs.85%29.aspx
  */

(?i:auto)                    { return token::HT_AUTO; }
(?i:catch)                   { return token::HT_CATCH; }
(?i:char)                    { return token::HT_CHAR; }
(?i:class)                   { return token::HT_CLASS; }
(?i:const_cast)              { return token::HT_CONST_CAST; }
(?i:delete)                  { return token::HT_DELETE; }
(?i:dynamic_cast)            { return token::HT_DYNAMIC_CAST; }
(?i:enum)                    { return token::HT_ENUM; }
(?i:explicit)                { return token::HT_EXPLICIT; }
(?i:friend)                  { return token::HT_FRIEND; }
(?i:goto)                    { return token::HT_GOTO; }
(?i:long)                    { return token::HT_LONG; }
(?i:mutable)                 { return token::HT_MUTABLE; }
(?i:new)                     { return token::HT_NEW; }
(?i:operator)                { return token::HT_OPERATOR; }
(?i:private)                 { return token::HT_PRIVATE; }
(?i:protected)               { return token::HT_PROTECTED; }
(?i:public)                  { return token::HT_PUBLIC; }
(?i:reinterpret_cast)        { return token::HT_REINTERPRET_CAST; }
(?i:short)                   { return token::HT_SHORT; }
(?i:signed)                  { return token::HT_SIGNED; }
(?i:sizeof)                  { return token::HT_SIZEOF; }
(?i:static_cast)             { return token::HT_STATIC_CAST; }
(?i:template)                { return token::HT_TEMPLATE; }
(?i:this)                    { return token::HT_THIS; }
(?i:throw)                   { return token::HT_THROW; }
(?i:try)                     { return token::HT_TRY; }
(?i:typename)                { return token::HT_TYPENAME; }
(?i:union)                   { return token::HT_UNION; }
(?i:unsigned)                { return token::HT_UNSIGNED; }
(?i:using)                   { return token::HT_USING; }
(?i:virtual)                 { return token::HT_VIRTUAL; }

 /*
  * Various tokens
  */

"++"                    { return token::T_INC; }
"--"                    { return token::T_DEC; }
"<="                    { return token::T_LE; }
">="                    { return token::T_GE; }
"=="                    { return token::T_EQ; }
"!="                    { return token::T_NE; }
"<<"                    { return token::T_LEFT; }
">>"                    { return token::T_RIGHT; }
"&&"                    { return token::T_AND; }
"||"                    { return token::T_OR; }
"^^"                    { return token::T_XOR; }

"*="                    { return token::T_MULEQ; }
"/="                    { return token::T_DIVEQ; }
"%="                    { return token::T_MODEQ; }
"+="                    { return token::T_ADDEQ; }
"-="                    { return token::T_SUBEQ; }
"<<="                   { return token::T_LEFTEQ; }
">>="                   { return token::T_RIGHTEQ; }
"&="                    { return token::T_ANDEQ; }
"^="                    { return token::T_XOREQ; }
"|="                    { return token::T_OREQ; }

(0|[1-9][0-9]*)\.[0-9]+ {
    yylval->fval = std::atof(yytext);
    return token::FLOATCONSTANT;
}
(0|[1-9][0-9]*)[uU]     {
    yylval->uval = std::atoi(yytext);
    return token::UINTCONSTANT;
}
(0|[1-9][0-9]*)         {
    yylval->ival = std::atoi(yytext);
    return token::INTCONSTANT;
}

([xyzw]{1,4}|[rgba]{1,4}|[stpq]{1,4}) {
    /* Copy token for now */
    yylval->sval = strdup(yytext);
    return token::FIELDSELECTION;
}

[a-zA-Z_][a-zA-Z0-9_]*   {
    /* Copy token for now */
    yylval->sval = strdup(yytext);
    return token::IDENTIFIER;
}

[ \t\r]+            { /* ignore whitespace */ }
[\n]                { yylloc->lines(1); }
.                   { return (token_type)*yytext; /* accept all characters */ }

 /*
  * Ignore C comments
  */

"/*"                { BEGIN(C_COMMENT); }
<C_COMMENT>[^*\n]*  { }
<C_COMMENT>\n       { yylloc->lines(1); }
<C_COMMENT>.        { }
<C_COMMENT>"*/"     {
    if (m_pp_stack.Last() == BlockIsTrue)
        BEGIN(INITIAL);
    else
        BEGIN(PREPROCESSOR_COMMENT);
}

 /*
  * Ignore C++ comments
  */

"//"              { BEGIN(CPP_COMMENT); }
<CPP_COMMENT>\n   { yylloc->lines(1); BEGIN(INITIAL); }
<CPP_COMMENT>.*   { }

%%

lol::LolFxScanner::LolFxScanner(char const *command)
    : LolFxFlexLexer(0, 0),
      m_input(command)
{
}

lol::LolFxScanner::~LolFxScanner()
{
}

int lol::LolFxScanner::LexerInput(char* buf, int max_size)
{
    buf[0] = m_input[0];
    if (buf[0])
        ++m_input;
    return buf[0] ? 1 : 0;
}

bool lol::LolFxScanner::IsExpressionTrue(char const *buf)
{
    /* FIXME: this is really too limited */
    return std::atoi(buf) != 0;
}

#ifdef yylex
#undef yylex
#endif
int LolFxFlexLexer::yylex()
{
    std::cerr << "in LolFxFlexLexer::yylex() !" << std::endl;
    return 0;
}

int LolFxFlexLexer::yywrap()
{
    return 1;
}

