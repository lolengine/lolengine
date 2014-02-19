//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// Debug line primitives
// ---------------------
//

#if !defined __LOL_DEBUG_LINES_H__
#define __LOL_DEBUG_LINES_H__

#include <stdint.h>

namespace lol
{

namespace Debug
{

//This func MUST be specialized
template<typename T> void Draw(T& x, vec4 color)
{
    ASSERT(0);
}

void DrawLine(vec3 a, vec3 b, vec4 color);
void DrawBox(vec3 a, vec3 b, vec4 color);
void DrawViewProj(mat4 view_proj, vec4 color);
void DrawViewProj(mat4 view, mat4 proj, vec4 color);

} /* namespace Debug */

} /* namespace lol */

#endif // __LOL_DEBUG_LINES_H__

