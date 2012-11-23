"""
""" Experimental Lol Engine Vim plugin
"""
""" More info here:
""" http://lol.zoy.org/wiki/dev/setup/vim
"""
"
"
""" XXX: intentionally no empty lines because some versions of Vim
""" for Windows will be unable to parse them if a version control
""" system has added CRLF line endings.
"
"
"""
""" Add syntax highlighting for new C++ types
"""
"
" some custom base types
au Syntax cpp
   \ syn keyword cType
   \ half ldouble lldouble real uint
"
" GLSL types and the Lol Engine extensions
au Syntax cpp
   \ syn keyword cType
   \ f16vec2 f16cmplx f16vec3 f16vec4 f16quat f16mat2 f16mat3 f16mat4
   \ vec2 cmplx vec3 vec4 quat mat2 mat3 mat4
   \ f64vec2 f64cmplx f64vec3 f64vec4 f64quat f64mat2 f64mat3 f64mat4
   \ f128vec2 f128cmplx f128vec3 f128vec4 f128quat f128mat2 f128mat3 f128mat4
   \ rvec2 rcmplx rvec3 rvec4 rquat rmat2 rmat3 rmat4
   \
   \ i8vec2 i8cmplx i8vec3 i8vec4 i8quat i8mat2 i8mat3 i8mat4
   \ u8vec2 u8cmplx u8vec3 u8vec4 u8quat u8mat2 u8mat3 u8mat4
   \ i16vec2 i16cmplx i16vec3 i16vec4 i16quat i16mat2 i16mat3 i16mat4
   \ u16vec2 u16cmplx u16vec3 u16vec4 u16quat u16mat2 u16mat3 u16mat4
   \ ivec2 icmplx ivec3 ivec4 iquat imat2 imat3 imat4
   \ uvec2 ucmplx uvec3 uvec4 uquat umat2 umat3 umat4
   \ i64vec2 i64cmplx i64vec3 i64vec4 i64quat i64mat2 i64mat3 i64mat4
   \ u64vec2 u64cmplx u64vec3 u64vec4 u64quat u64mat2 u64mat3 u64mat4
"
" HLSL types
au Syntax cpp
   \ syn keyword cType
   \ int2 int3 int4 int2x2 int3x3 int4x4
   \ float2 float3 float4 float2x2 float3x3 float4x4
"
"
"""
""" For now, pretend .lolfx is C++
"""
au BufRead,BufNewFile *.lolfx let b:current_syntax = "cpp"
"
