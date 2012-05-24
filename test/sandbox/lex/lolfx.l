%{
#include <iostream>
#define YY_DECL extern "C" int yylex()
#include "lolfx.tab.h"
%}

%x C_COMMENT
%x CPP_COMMENT

%%

 /*
  * GLSL keywords
  */

"subroutine"                 { return GT_SUBROUTINE; }
"patch"                      { return GT_PATCH; }
"sample"                     { return GT_SAMPLE; }
"flat"                       { return GT_FLAT; }
"smooth"                     { return GT_SMOOTH; }
"layout"                     { return GT_LAYOUT; }
"sampler1dshadow"            { return GT_SAMPLER1DSHADOW; }
"sampler2dshadow"            { return GT_SAMPLER2DSHADOW; }
"samplercubeshadow"          { return GT_SAMPLERCUBESHADOW; }
"sampler1darray"             { return GT_SAMPLER1DARRAY; }
"sampler2darray"             { return GT_SAMPLER2DARRAY; }
"sampler1darrayshadow"       { return GT_SAMPLER1DARRAYSHADOW; }
"sampler2darrayshadow"       { return GT_SAMPLER2DARRAYSHADOW; }
"isampler1d"                 { return GT_ISAMPLER1D; }
"isampler2d"                 { return GT_ISAMPLER2D; }
"isampler3d"                 { return GT_ISAMPLER3D; }
"isamplercube"               { return GT_ISAMPLERCUBE; }
"isampler1darray"            { return GT_ISAMPLER1DARRAY; }
"isampler2darray"            { return GT_ISAMPLER2DARRAY; }
"usampler1d"                 { return GT_USAMPLER1D; }
"usampler2d"                 { return GT_USAMPLER2D; }
"usampler3d"                 { return GT_USAMPLER3D; }
"usamplercube"               { return GT_USAMPLERCUBE; }
"usampler1darray"            { return GT_USAMPLER1DARRAY; }
"usampler2darray"            { return GT_USAMPLER2DARRAY; }
"sampler2drect"              { return GT_SAMPLER2DRECT; }
"sampler2drectshadow"        { return GT_SAMPLER2DRECTSHADOW; }
"isampler2drect"             { return GT_ISAMPLER2DRECT; }
"usampler2drect"             { return GT_USAMPLER2DRECT; }
"samplerbuffer"              { return GT_SAMPLERBUFFER; }
"isamplerbuffer"             { return GT_ISAMPLERBUFFER; }
"usamplerbuffer"             { return GT_USAMPLERBUFFER; }
"samplercubearray"           { return GT_SAMPLERCUBEARRAY; }
"samplercubearrayshadow"     { return GT_SAMPLERCUBEARRAYSHADOW; }
"isamplercubearray"          { return GT_ISAMPLERCUBEARRAY; }
"usamplercubearray"          { return GT_USAMPLERCUBEARRAY; }
"sampler2dms"                { return GT_SAMPLER2DMS; }
"isampler2dms"               { return GT_ISAMPLER2DMS; }
"usampler2dms"               { return GT_USAMPLER2DMS; }
"sampler2dmsarray"           { return GT_SAMPLER2DMSARRAY; }
"isampler2dmsarray"          { return GT_ISAMPLER2DMSARRAY; }
"usampler2dmsarray"          { return GT_USAMPLER2DMSARRAY; }

"identifier"                 { return GT_IDENTIFIER; }
"type_name"                  { return GT_TYPE_NAME; }
"floatconstant"              { return GT_FLOATCONSTANT; }
"intconstant"                { return GT_INTCONSTANT; }
"uintconstant"               { return GT_UINTCONSTANT; }
"boolconstant"               { return GT_BOOLCONSTANT; }

"field_selection"            { return GT_FIELD_SELECTION; }
"left_op"                    { return GT_LEFT_OP; }
"right_op"                   { return GT_RIGHT_OP; }
"and_op"                     { return GT_AND_OP; }
"or_op"                      { return GT_OR_OP; }
"xor_op"                     { return GT_XOR_OP; }
"mul_assign"                 { return GT_MUL_ASSIGN; }
"div_assign"                 { return GT_DIV_ASSIGN; }
"add_assign"                 { return GT_ADD_ASSIGN; }
"mod_assign"                 { return GT_MOD_ASSIGN; }
"left_assign"                { return GT_LEFT_ASSIGN; }
"right_assign"               { return GT_RIGHT_ASSIGN; }
"and_assign"                 { return GT_AND_ASSIGN; }
"xor_assign"                 { return GT_XOR_ASSIGN; }
"or_assign"                  { return GT_OR_ASSIGN; }
"sub_assign"                 { return GT_SUB_ASSIGN; }
"left_paren"                 { return GT_LEFT_PAREN; }
"right_paren"                { return GT_RIGHT_PAREN; }
"left_bracket"               { return GT_LEFT_BRACKET; }
"right_bracket"              { return GT_RIGHT_BRACKET; }
"left_brace"                 { return GT_LEFT_BRACE; }
"right_brace"                { return GT_RIGHT_BRACE; }
"dot"                        { return GT_DOT; }
"comma"                      { return GT_COMMA; }
"colon"                      { return GT_COLON; }
"equal"                      { return GT_EQUAL; }
"semicolon"                  { return GT_SEMICOLON; }
"bang"                       { return GT_BANG; }
"dash"                       { return GT_DASH; }
"tilde"                      { return GT_TILDE; }
"plus"                       { return GT_PLUS; }
"star"                       { return GT_STAR; }
"slash"                      { return GT_SLASH; }
"percent"                    { return GT_PERCENT; }
"left_angle"                 { return GT_LEFT_ANGLE; }
"right_angle"                { return GT_RIGHT_ANGLE; }
"vertical_bar"               { return GT_VERTICAL_BAR; }
"caret"                      { return GT_CARET; }
"ampersand"                  { return GT_AMPERSAND; }
"question"                   { return GT_QUESTION; }
"invariant"                  { return GT_INVARIANT; }
"high_precision"             { return GT_HIGH_PRECISION; }
"medium_precision"           { return GT_MEDIUM_PRECISION; }
"low_precision"              { return GT_LOW_PRECISION; }
"precision"                  { return GT_PRECISION; }

 /*
  * GLSL vector types
  */

"bvec2"                      { return GT_BVEC2; }
"bvec3"                      { return GT_BVEC3; }
"bvec4"                      { return GT_BVEC4; }

"ivec2"                      { return GT_IVEC2; }
"ivec3"                      { return GT_IVEC3; }
"ivec4"                      { return GT_IVEC4; }

"uvec2"                      { return GT_UVEC2; }
"uvec3"                      { return GT_UVEC3; }
"uvec4"                      { return GT_UVEC4; }

"vec2"                       { return GT_VEC2; }
"vec3"                       { return GT_VEC3; }
"vec4"                       { return GT_VEC4; }
"mat2"                       { return GT_MAT2; }
"mat2x2"                     { return GT_MAT2X2; }
"mat2x3"                     { return GT_MAT2X3; }
"mat2x4"                     { return GT_MAT2X4; }
"mat3"                       { return GT_MAT3; }
"mat3x2"                     { return GT_MAT3X2; }
"mat3x3"                     { return GT_MAT3X3; }
"mat3x4"                     { return GT_MAT3X4; }
"mat4"                       { return GT_MAT4; }
"mat4x2"                     { return GT_MAT4X2; }
"mat4x3"                     { return GT_MAT4X3; }
"mat4x4"                     { return GT_MAT4X4; }

"dvec2"                      { return GT_DVEC2; }
"dvec3"                      { return GT_DVEC3; }
"dvec4"                      { return GT_DVEC4; }
"dmat2"                      { return GT_DMAT2; }
"dmat2x2"                    { return GT_DMAT2X2; }
"dmat2x3"                    { return GT_DMAT2X3; }
"dmat2x4"                    { return GT_DMAT2X4; }
"dmat3"                      { return GT_DMAT3; }
"dmat3x2"                    { return GT_DMAT3X2; }
"dmat3x3"                    { return GT_DMAT3X3; }
"dmat3x4"                    { return GT_DMAT3X4; }
"dmat4"                      { return GT_DMAT4; }
"dmat4x2"                    { return GT_DMAT4X2; }
"dmat4x3"                    { return GT_DMAT4X3; }
"dmat4x4"                    { return GT_DMAT4X4; }

 /*
  * GLSL keywords that are also valid or reserved in HLSL
  * and HLSL keywords that are also valid or reserved in GLSL.
  */

"bool"                       { return GHT_BOOL; }
"break"                      { return GHT_BREAK; }
"case"                       { return GHT_CASE; }
"centroid"                   { return GHT_CENTROID; }
"const"                      { return GHT_CONST; }
"continue"                   { return GHT_CONTINUE; }
"default"                    { return GHT_DEFAULT; }
"discard"                    { return GHT_DISCARD; }
"do"                         { return GHT_DO; }
"double"                     { return GHT_DOUBLE; }
"else"                       { return GHT_ELSE; }
"float"                      { return GHT_FLOAT; }
"for"                        { return GHT_FOR; }
"if"                         { return GHT_IF; }
"in"                         { return GHT_IN; }
"inout"                      { return GHT_INOUT; }
"int"                        { return GHT_INT; }
"noperspective"              { return GHT_NOPERSPECTIVE; }
"out"                        { return GHT_OUT; }
"return"                     { return GHT_RETURN; }
"sampler1d"                  { return GHT_SAMPLER1D; }
"sampler2d"                  { return GHT_SAMPLER2D; }
"sampler3d"                  { return GHT_SAMPLER3D; }
"samplercube"                { return GHT_SAMPLERCUBE; }
"struct"                     { return GHT_STRUCT; }
"switch"                     { return GHT_SWITCH; }
"uint"                       { return GHT_UINT; }
"uniform"                    { return GHT_UNIFORM; }
"void"                       { return GHT_VOID; }
"while"                      { return GHT_WHILE; }

(?i:bool)                    { return HGT_BOOL; }
(?i:break)                   { return HGT_BREAK; }
(?i:case)                    { return HGT_CASE; }
(?i:centroid)                { return HGT_CENTROID; }
(?i:const)                   { return HGT_CONST; }
(?i:continue)                { return HGT_CONTINUE; }
(?i:default)                 { return HGT_DEFAULT; }
(?i:discard)                 { return HGT_DISCARD; }
(?i:do)                      { return HGT_DO; }
(?i:double)                  { return HGT_DOUBLE; }
(?i:else)                    { return HGT_ELSE; }
(?i:float)                   { return HGT_FLOAT; }
(?i:for)                     { return HGT_FOR; }
(?i:if)                      { return HGT_IF; }
(?i:in)                      { return HGT_IN; }
(?i:inout)                   { return HGT_INOUT; }
(?i:int)                     { return HGT_INT; }
(?i:noperspective)           { return HGT_NOPERSPECTIVE; }
(?i:out)                     { return HGT_OUT; }
(?i:return)                  { return HGT_RETURN; }
(?i:sampler1d)               { return HGT_SAMPLER1D; }
(?i:sampler2d)               { return HGT_SAMPLER2D; }
(?i:sampler3d)               { return HGT_SAMPLER3D; }
(?i:samplercube)             { return HGT_SAMPLERCUBE; }
(?i:struct)                  { return HGT_STRUCT; }
(?i:switch)                  { return HGT_SWITCH; }
(?i:uint)                    { return HGT_UINT; }
(?i:uniform)                 { return HGT_UNIFORM; }
(?i:void)                    { return HGT_VOID; }
(?i:while)                   { return HGT_WHILE; }

 /*
  * HLSL keywords
  * Gathered from http://msdn.microsoft.com/en-us/library/windows/desktop/bb509568%28v=vs.85%29.aspx
  */

(?i:appendstructuredbuffer)  { return HT_APPENDSTRUCTUREDBUFFER; }
(?i:asm)                     { return HT_ASM; }
(?i:asm_fragment)            { return HT_ASM_FRAGMENT; }
(?i:blendstate)              { return HT_BLENDSTATE; }
(?i:buffer)                  { return HT_BUFFER; }
(?i:byteaddressbuffer)       { return HT_BYTEADDRESSBUFFER; }
(?i:cbuffer)                 { return HT_CBUFFER; }
(?i:column_major)            { return HT_COLUMN_MAJOR; }
(?i:compile)                 { return HT_COMPILE; }
(?i:compile_fragment)        { return HT_COMPILE_FRAGMENT; }
(?i:compileshader)           { return HT_COMPILESHADER; }
(?i:computeshader)           { return HT_COMPUTESHADER; }
(?i:consumestructuredbuffer) { return HT_CONSUMESTRUCTUREDBUFFER; }
(?i:depthstencilstate)       { return HT_DEPTHSTENCILSTATE; }
(?i:depthstencilview)        { return HT_DEPTHSTENCILVIEW; }
(?i:domainshader)            { return HT_DOMAINSHADER; }
(?i:dword)                   { return HT_DWORD; }
(?i:extern)                  { return HT_EXTERN; }
(?i:false)                   { return HT_FALSE; }
(?i:fxgroup)                 { return HT_FXGROUP; }
(?i:geometryshader)          { return HT_GEOMETRYSHADER; }
(?i:groupshared)             { return HT_GROUPSHARED; }
(?i:half)                    { return HT_HALF; }
(?i:hullshader)              { return HT_HULLSHADER; }
(?i:inline)                  { return HT_INLINE; }
(?i:inputpatch)              { return HT_INPUTPATCH; }
(?i:interface)               { return HT_INTERFACE; }
(?i:line)                    { return HT_LINE; }
(?i:lineadj)                 { return HT_LINEADJ; }
(?i:linear)                  { return HT_LINEAR; }
(?i:linestream)              { return HT_LINESTREAM; }
(?i:matrix)                  { return HT_MATRIX; }
(?i:namespace)               { return HT_NAMESPACE; }
(?i:nointerpolation)         { return HT_NOINTERPOLATION; }
(?i:null)                    { return HT_NULL; }
(?i:outputpatch)             { return HT_OUTPUTPATCH; }
(?i:packoffset)              { return HT_PACKOFFSET; }
(?i:pass)                    { return HT_PASS; }
(?i:pixelfragment)           { return HT_PIXELFRAGMENT; }
(?i:pixelshader)             { return HT_PIXELSHADER; }
(?i:point)                   { return HT_POINT; }
(?i:pointstream)             { return HT_POINTSTREAM; }
(?i:precise)                 { return HT_PRECISE; }
(?i:rasterizerstate)         { return HT_RASTERIZERSTATE; }
(?i:rendertargetview)        { return HT_RENDERTARGETVIEW; }
(?i:register)                { return HT_REGISTER; }
(?i:row_major)               { return HT_ROW_MAJOR; }
(?i:rwbuffer)                { return HT_RWBUFFER; }
(?i:rwbyteaddressbuffer)     { return HT_RWBYTEADDRESSBUFFER; }
(?i:rwstructuredbuffer)      { return HT_RWSTRUCTUREDBUFFER; }
(?i:rwtexture1d)             { return HT_RWTEXTURE1D; }
(?i:rwtexture1darray)        { return HT_RWTEXTURE1DARRAY; }
(?i:rwtexture2d)             { return HT_RWTEXTURE2D; }
(?i:rwtexture2darray)        { return HT_RWTEXTURE2DARRAY; }
(?i:rwtexture3d)             { return HT_RWTEXTURE3D; }
(?i:sampler)                 { return HT_SAMPLER; }
(?i:sampler_state)           { return HT_SAMPLER_STATE; }
(?i:samplerstate)            { return HT_SAMPLERSTATE; }
(?i:samplercomparisonstate)  { return HT_SAMPLERCOMPARISONSTATE; }
(?i:shared)                  { return HT_SHARED; }
(?i:snorm)                   { return HT_SNORM; }
(?i:stateblock)              { return HT_STATEBLOCK; }
(?i:stateblock_state)        { return HT_STATEBLOCK_STATE; }
(?i:static)                  { return HT_STATIC; }
(?i:string)                  { return HT_STRING; }
(?i:structuredbuffer)        { return HT_STRUCTUREDBUFFER; }
(?i:tbuffer)                 { return HT_TBUFFER; }
(?i:technique)               { return HT_TECHNIQUE; }
(?i:technique10)             { return HT_TECHNIQUE10; }
(?i:technique11xz)           { return HT_TECHNIQUE11XZ; }
"texture"                    { return HT_TEXTURE; }
(?i:texture1d)               { return HT_TEXTURE1D; }
(?i:texture1darray)          { return HT_TEXTURE1DARRAY; }
(?i:texture2d)               { return HT_TEXTURE2D; }
(?i:texture2darray)          { return HT_TEXTURE2DARRAY; }
(?i:texture2dms)             { return HT_TEXTURE2DMS; }
(?i:texture2dmsarray)        { return HT_TEXTURE2DMSARRAY; }
(?i:texture3d)               { return HT_TEXTURE3D; }
(?i:texturecube)             { return HT_TEXTURECUBE; }
(?i:texturecubearray)        { return HT_TEXTURECUBEARRAY; }
(?i:true)                    { return HT_TRUE; }
(?i:typedef)                 { return HT_TYPEDEF; }
(?i:triangle)                { return HT_TRIANGLE; }
(?i:triangleadj)             { return HT_TRIANGLEADJ; }
(?i:trianglestream)          { return HT_TRIANGLESTREAM; }
(?i:unorm)                   { return HT_UNORM; }
(?i:vector)                  { return HT_VECTOR; }
(?i:vertexfragment)          { return HT_VERTEXFRAGMENT; }
(?i:vertexshader)            { return HT_VERTEXSHADER; }
(?i:volatile)                { return HT_VOLATILE; }

 /*
  * HLSL vector types
  * TODO: this can probably be factored somehow
  */

(?i:bool1)                   { return HT_BOOL1; }
(?i:bool1x1)                 { return HT_BOOL1x1; }
(?i:bool2x1)                 { return HT_BOOL2x1; }
(?i:bool3x1)                 { return HT_BOOL3x1; }
(?i:bool4x1)                 { return HT_BOOL4x1; }
(?i:bool2)                   { return HT_BOOL2; }
(?i:bool1x2)                 { return HT_BOOL1x2; }
(?i:bool2x2)                 { return HT_BOOL2x2; }
(?i:bool3x2)                 { return HT_BOOL3x2; }
(?i:bool4x2)                 { return HT_BOOL4x2; }
(?i:bool3)                   { return HT_BOOL3; }
(?i:bool1x3)                 { return HT_BOOL1x3; }
(?i:bool2x3)                 { return HT_BOOL2x3; }
(?i:bool3x3)                 { return HT_BOOL3x3; }
(?i:bool4x3)                 { return HT_BOOL4x3; }
(?i:bool4)                   { return HT_BOOL4; }
(?i:bool1x4)                 { return HT_BOOL1x4; }
(?i:bool2x4)                 { return HT_BOOL2x4; }
(?i:bool3x4)                 { return HT_BOOL3x4; }
(?i:bool4x4)                 { return HT_BOOL4x4; }

(?i:float1)                  { return HT_FLOAT1; }
(?i:float1x1)                { return HT_FLOAT1x1; }
(?i:float2x1)                { return HT_FLOAT2x1; }
(?i:float3x1)                { return HT_FLOAT3x1; }
(?i:float4x1)                { return HT_FLOAT4x1; }
(?i:float2)                  { return HT_FLOAT2; }
(?i:float1x2)                { return HT_FLOAT1x2; }
(?i:float2x2)                { return HT_FLOAT2x2; }
(?i:float3x2)                { return HT_FLOAT3x2; }
(?i:float4x2)                { return HT_FLOAT4x2; }
(?i:float3)                  { return HT_FLOAT3; }
(?i:float1x3)                { return HT_FLOAT1x3; }
(?i:float2x3)                { return HT_FLOAT2x3; }
(?i:float3x3)                { return HT_FLOAT3x3; }
(?i:float4x3)                { return HT_FLOAT4x3; }
(?i:float4)                  { return HT_FLOAT4; }
(?i:float1x4)                { return HT_FLOAT1x4; }
(?i:float2x4)                { return HT_FLOAT2x4; }
(?i:float3x4)                { return HT_FLOAT3x4; }
(?i:float4x4)                { return HT_FLOAT4x4; }

(?i:double1)                 { return HT_DOUBLE1; }
(?i:double1x1)               { return HT_DOUBLE1x1; }
(?i:double2x1)               { return HT_DOUBLE2x1; }
(?i:double3x1)               { return HT_DOUBLE3x1; }
(?i:double4x1)               { return HT_DOUBLE4x1; }
(?i:double2)                 { return HT_DOUBLE2; }
(?i:double1x2)               { return HT_DOUBLE1x2; }
(?i:double2x2)               { return HT_DOUBLE2x2; }
(?i:double3x2)               { return HT_DOUBLE3x2; }
(?i:double4x2)               { return HT_DOUBLE4x2; }
(?i:double3)                 { return HT_DOUBLE3; }
(?i:double1x3)               { return HT_DOUBLE1x3; }
(?i:double2x3)               { return HT_DOUBLE2x3; }
(?i:double3x3)               { return HT_DOUBLE3x3; }
(?i:double4x3)               { return HT_DOUBLE4x3; }
(?i:double4)                 { return HT_DOUBLE4; }
(?i:double1x4)               { return HT_DOUBLE1x4; }
(?i:double2x4)               { return HT_DOUBLE2x4; }
(?i:double3x4)               { return HT_DOUBLE3x4; }
(?i:double4x4)               { return HT_DOUBLE4x4; }

(?i:dword1)                  { return HT_DWORD1; }
(?i:dword1x1)                { return HT_DWORD1x1; }
(?i:dword2x1)                { return HT_DWORD2x1; }
(?i:dword3x1)                { return HT_DWORD3x1; }
(?i:dword4x1)                { return HT_DWORD4x1; }
(?i:dword2)                  { return HT_DWORD2; }
(?i:dword1x2)                { return HT_DWORD1x2; }
(?i:dword2x2)                { return HT_DWORD2x2; }
(?i:dword3x2)                { return HT_DWORD3x2; }
(?i:dword4x2)                { return HT_DWORD4x2; }
(?i:dword3)                  { return HT_DWORD3; }
(?i:dword1x3)                { return HT_DWORD1x3; }
(?i:dword2x3)                { return HT_DWORD2x3; }
(?i:dword3x3)                { return HT_DWORD3x3; }
(?i:dword4x3)                { return HT_DWORD4x3; }
(?i:dword4)                  { return HT_DWORD4; }
(?i:dword1x4)                { return HT_DWORD1x4; }
(?i:dword2x4)                { return HT_DWORD2x4; }
(?i:dword3x4)                { return HT_DWORD3x4; }
(?i:dword4x4)                { return HT_DWORD4x4; }

(?i:int1)                    { return HT_INT1; }
(?i:int1x1)                  { return HT_INT1x1; }
(?i:int2x1)                  { return HT_INT2x1; }
(?i:int3x1)                  { return HT_INT3x1; }
(?i:int4x1)                  { return HT_INT4x1; }
(?i:int2)                    { return HT_INT2; }
(?i:int1x2)                  { return HT_INT1x2; }
(?i:int2x2)                  { return HT_INT2x2; }
(?i:int3x2)                  { return HT_INT3x2; }
(?i:int4x2)                  { return HT_INT4x2; }
(?i:int3)                    { return HT_INT3; }
(?i:int1x3)                  { return HT_INT1x3; }
(?i:int2x3)                  { return HT_INT2x3; }
(?i:int3x3)                  { return HT_INT3x3; }
(?i:int4x3)                  { return HT_INT4x3; }
(?i:int4)                    { return HT_INT4; }
(?i:int1x4)                  { return HT_INT1x4; }
(?i:int2x4)                  { return HT_INT2x4; }
(?i:int3x4)                  { return HT_INT3x4; }
(?i:int4x4)                  { return HT_INT4x4; }

(?i:uint1)                   { return HT_UINT1; }
(?i:uint1x1)                 { return HT_UINT1x1; }
(?i:uint2x1)                 { return HT_UINT2x1; }
(?i:uint3x1)                 { return HT_UINT3x1; }
(?i:uint4x1)                 { return HT_UINT4x1; }
(?i:uint2)                   { return HT_UINT2; }
(?i:uint1x2)                 { return HT_UINT1x2; }
(?i:uint2x2)                 { return HT_UINT2x2; }
(?i:uint3x2)                 { return HT_UINT3x2; }
(?i:uint4x2)                 { return HT_UINT4x2; }
(?i:uint3)                   { return HT_UINT3; }
(?i:uint1x3)                 { return HT_UINT1x3; }
(?i:uint2x3)                 { return HT_UINT2x3; }
(?i:uint3x3)                 { return HT_UINT3x3; }
(?i:uint4x3)                 { return HT_UINT4x3; }
(?i:uint4)                   { return HT_UINT4; }
(?i:uint1x4)                 { return HT_UINT1x4; }
(?i:uint2x4)                 { return HT_UINT2x4; }
(?i:uint3x4)                 { return HT_UINT3x4; }
(?i:uint4x4)                 { return HT_UINT4x4; }

 /*
  * HLSL preprocessor directives
  * Gathered from http://msdn.microsoft.com/en-us/library/windows/desktop/bb943993%28v=vs.85%29.aspx
  */

"#"[ \t]*"define"            { return PREPROCESSOR_DEFINE; }
"#"[ \t]*"elif"              { return PREPROCESSOR_ELIF; }
"#"[ \t]*"else"              { return PREPROCESSOR_ELSE; }
"#"[ \t]*"endif"             { return PREPROCESSOR_ENDIF; }
"#"[ \t]*"error"             { return PREPROCESSOR_ERROR; }
"#"[ \t]*"if"                { return PREPROCESSOR_IF; }
"#"[ \t]*"ifdef"             { return PREPROCESSOR_IFDEF; }
"#"[ \t]*"ifndef"            { return PREPROCESSOR_IFNDEF; }
"#"[ \t]*"include"           { return PREPROCESSOR_INCLUDE; }
"#"[ \t]*"line"              { return PREPROCESSOR_LINE; }
"#"[ \t]*"pragma"            { return PREPROCESSOR_PRAGMA; }
"#"[ \t]*"undef"             { return PREPROCESSOR_UNDEF; }

 /*
  * HLSL reserved keywords
  * Gathered from http://msdn.microsoft.com/en-us/library/windows/desktop/bb509569%28v=vs.85%29.aspx
  */

(?i:auto)                    { return HT_AUTO; }
(?i:catch)                   { return HT_CATCH; }
(?i:char)                    { return HT_CHAR; }
(?i:class)                   { return HT_CLASS; }
(?i:const_cast)              { return HT_CONST_CAST; }
(?i:delete)                  { return HT_DELETE; }
(?i:dynamic_cast)            { return HT_DYNAMIC_CAST; }
(?i:enum)                    { return HT_ENUM; }
(?i:explicit)                { return HT_EXPLICIT; }
(?i:friend)                  { return HT_FRIEND; }
(?i:goto)                    { return HT_GOTO; }
(?i:long)                    { return HT_LONG; }
(?i:mutable)                 { return HT_MUTABLE; }
(?i:new)                     { return HT_NEW; }
(?i:operator)                { return HT_OPERATOR; }
(?i:private)                 { return HT_PRIVATE; }
(?i:protected)               { return HT_PROTECTED; }
(?i:public)                  { return HT_PUBLIC; }
(?i:reinterpret_cast)        { return HT_REINTERPRET_CAST; }
(?i:short)                   { return HT_SHORT; }
(?i:signed)                  { return HT_SIGNED; }
(?i:sizeof)                  { return HT_SIZEOF; }
(?i:static_cast)             { return HT_STATIC_CAST; }
(?i:template)                { return HT_TEMPLATE; }
(?i:this)                    { return HT_THIS; }
(?i:throw)                   { return HT_THROW; }
(?i:try)                     { return HT_TRY; }
(?i:typename)                { return HT_TYPENAME; }
(?i:union)                   { return HT_UNION; }
(?i:unsigned)                { return HT_UNSIGNED; }
(?i:using)                   { return HT_USING; }
(?i:virtual)                 { return HT_VIRTUAL; }

 /*
  * Various tokens
  */

"++"                    { return T_INC; }
"--"                    { return T_DEC; }
"<="                    { return T_LE; }
">="                    { return T_GE; }
"=="                    { return T_EQ; }
"!="                    { return T_NE; }
"<<"                    { return T_LEFT; }
">>"                    { return T_RIGHT; }
"&&"                    { return T_AND; }
"||"                    { return T_OR; }
"^^"                    { return T_XOR; }

"*="                    { return T_MULEQ; }
"/="                    { return T_DIVEQ; }
"%="                    { return T_MODEQ; }
"+="                    { return T_ADDEQ; }
"-="                    { return T_SUBEQ; }
"<<="                   { return T_LEFTEQ; }
">>="                   { return T_RIGHTEQ; }
"&="                    { return T_ANDEQ; }
"^="                    { return T_XOREQ; }
"|="                    { return T_OREQ; }

[0-9]+\.[0-9]+ { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+         { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z][a-zA-Z0-9_]*   {
    /* Copy token for now */
    yylval.sval = strdup(yytext);
    return NAME;
}
[ \t\n]+       ;
"{"|"}"|"("|")"|"["|"]" { return *yytext; }
.                       { return *yytext; }

 /*
  * Ignore C comments
  */

"/*"              { BEGIN(C_COMMENT); }
<C_COMMENT>"*/"   { BEGIN(INITIAL); }
<C_COMMENT>[^*]*  { }
<C_COMMENT>.      { }

 /*
  * Ignore C++ comments
  */

"//"              { BEGIN(CPP_COMMENT); }
<CPP_COMMENT>\n   { BEGIN(INITIAL); }
<CPP_COMMENT>.*   { }

%%

