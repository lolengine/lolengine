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

//Resets draw infos
void DrawSetupReset();
//Sets draw infos
void DrawSetupTime(float new_time);
void DrawSetupMask(int new_mask);
void DrawSetupSegment(float segment_size);
void DrawSetupColor(vec4 color);
void DrawSetup(float new_time, int new_mask, float segment_size, vec4 color);

//Draw stuff in World
void DrawLine(vec3 a, vec3 b);
void DrawLine(vec3 a, vec3 b, vec4 color);
void DrawBox(box3 a);
void DrawBox(box3 a, vec4 color);
void DrawBox(vec3 a, vec3 b);
void DrawBox(vec3 a, vec3 b, vec4 color);
void DrawBox(box3 a, mat4 transform);
void DrawBox(box3 a, mat4 transform, vec4 color);
void DrawBox(vec3 a, vec3 b, mat4 transform);
void DrawBox(vec3 a, vec3 b, mat4 transform, vec4 color);
void DrawCircle(vec3 a, vec3 n);
void DrawCircle(vec3 a, vec3 n, vec4 color);
void DrawCircle(vec3 a, vec3 x, vec3 y);
void DrawCircle(vec3 a, vec3 x, vec3 y, vec4 color);
void DrawSphere(vec3 a, float s);
void DrawSphere(vec3 a, float s, vec4 color);
void DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z);
void DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z, vec4 color);
void DrawCapsule(vec3 a, float s, vec3 h);
void DrawCapsule(vec3 a, float s, vec3 h, vec4 color);
void DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h);
void DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h, vec4 color);
void DrawViewProj(mat4 view_proj);
void DrawViewProj(mat4 view_proj, vec4 color);
void DrawViewProj(mat4 view, mat4 proj);
void DrawViewProj(mat4 view, mat4 proj, vec4 color);

} /* namespace Debug */

} /* namespace lol */

#endif // __LOL_DEBUG_LINES_H__

