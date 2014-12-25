"""
""" Experimental Lol Engine Vim plugin
"""
""" More info here:
""" http://lolengine.net/wiki/dev/setup/vim
"""


"""
""" Add syntax highlighting for new C++ types
"""

" some custom base types
au Syntax cpp
   \ syn keyword cType
   \ half ldouble lldouble real uint

" Some custom container types
au Syntax cpp
   \ syn keyword cType
   \ tuple array array2d array3d hash map

" GLSL types and the Lol Engine extensions
au Syntax cpp
   \ syn match cType
   \ "\<\(f16\|\|f64\|f128\|r\|[iu]\(8\|16\|\|64\)\)\(vec\([23456789]\|1[012]\)\|cmplx\|quat\|dualquat\|mat\([234]\|2x3\|3x2\|3x4\|4x3\|2x4\|4x2\)\)\>"

" HLSL types and the Lol Engine extensions
au Syntax cpp
   \ syn match cType
   \ "\<\(int\|half\|float\)\([23456789]\|1[012]\|[234]x[234]\)\>"

" More GLSL-like types from the Lol Engine
au Syntax cpp
   \ syn keyword cType
   \ box2 dbox2 ibox2 ubox2
   \ box3 dbox3 ibox3 ubox3

" Ensure we know about nullptr
au Syntax cpp
   \ syn keyword cConstant
   \ nullptr

" New maths constants
au Syntax cpp
   \ syn match cConstant
   \ "\<\(F\|D\|LD\)_\(PI\|PI_[234]\|[12]_PI\|SQRT_[23]\|SQRT_1_2\)\>"

" Unit testing
au Syntax cpp
   \ syn match cOperator
   \ "\<lolunit_\(fail\|assert\)[a-z_]*\>"

" Global keywords
au Syntax cpp
  \ syn keyword cConstant
  \ UNUSED ASSERT


"""
""" LolFx language handler
"""

" For now, pretend .lolfx is C++
au BufRead,BufNewFile *.lolfx set syntax=cpp

" New sampler types (GLSL)
au BufRead,BufNewFile *.lolfx syn match cType
   \ "\<sampler\(\([12]D\|Cube\)\(Array\|\)\(Shadow\|\)\)\>"
   \ "\<sampler\(3D\|2DRect\|Buffer\|2DMS\|2DMSArray\|2DRectShadow\)\>"

" Type constructs (LolFx)
au BufRead,BufNewFile *.lolfx syn keyword cppStructure
   \ technique pass precision

" Variable attributes (GLSL and some HLSL)
au BufRead,BufNewFile *.lolfx syn keyword cType
   \ in out uniform attribute varying

" Texture operators (GLSL and HLSL)
au BufRead,BufNewFile *.lolfx syn keyword cppOperator
   \ texture1D texture2D texture3D tex2D tex3D

" Handle #version constructs
au BufRead,BufNewFile *.lolfx syn region cPreProc
   \ start="^\s*\(%:\|#\)\s*version\>" skip="\\$" end="$" keepend contains=ALLBUT,@cPreProcGroup,@Spell

" LolFx shader region
au BufRead,BufNewFile *.lolfx syn region cSpecial
   \ start="^\s*\[[^\]]*\(glsl\|hlsl\)\]" end="$" contains=ALLBUT,@cPreProcGroup,@Spell

