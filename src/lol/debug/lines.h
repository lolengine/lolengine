//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Debug line primitives
// ---------------------
//

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

//Screen to world conversion
vec3 WorldToScreen(vec3 pos);
vec3 WorldToScreen(vec3 pos, mat4 view_proj);
vec3 WorldToScreen(vec4 pos);
vec3 WorldToScreen(vec4 pos, mat4 view_proj);
vec3 ScreenToWorld(vec2 pos, float z=-1.f);
vec3 ScreenToWorld(vec3 pos);
vec3 ScreenToWorld(vec2 pos, mat4 inv_view_proj, float z=-1.f);
vec3 ScreenToWorld(vec3 pos, mat4 inv_view_proj);
vec3 ScreenToWorld(vec3 pos, mat4 view, mat4 proj);

//Draw stuff in World
//-- LINE
void DrawLine(vec3 a, vec3 b);
void DrawLine(vec2 a, vec3 b, float az=-1.f);
void DrawLine(vec2 a, vec2 b, float az=-1.f, float bz=-1.f);
void DrawLine(vec3 a, vec3 b, vec4 color);
void DrawLine(vec2 a, vec3 b, vec4 color, float az=-1.f);
void DrawLine(vec2 a, vec2 b, vec4 color, float az=-1.f, float bz=-1.f);
//-- ARROW
void DrawArrow(vec3 a, vec3 b, vec2 s);
void DrawArrow(vec2 a, vec3 b, vec2 s, float az=-1.f);
void DrawArrow(vec2 a, vec2 b, vec2 s, float az=-1.f, float bz=-1.f);
void DrawArrow(vec3 a, vec3 b, vec3 s);
void DrawArrow(vec2 a, vec3 b, vec3 s, float az=-1.f);
void DrawArrow(vec2 a, vec2 b, vec3 s, float az=-1.f, float bz=-1.f);
void DrawArrow(vec3 a, vec3 b, vec3 s, vec4 color);
void DrawArrow(vec2 a, vec3 b, vec3 s, vec4 color, float az=-1.f);
void DrawArrow(vec2 a, vec2 b, vec3 s, vec4 color, float az=-1.f, float bz=-1.f);
//-- BOX
void DrawBox(box3 a);
void DrawBox(box2 a);
void DrawBox(box3 a, vec4 color);
void DrawBox(box2 a, vec4 color);
void DrawBox(vec3 a, vec3 b);
void DrawBox(vec2 a, vec2 b);
void DrawBox(vec2 a, float s);
void DrawBox(vec3 a, vec3 b, vec4 color);
void DrawBox(vec2 a, vec2 b, vec4 color);
void DrawBox(vec2 a, float s, vec4 color);
void DrawBox(box3 a, mat4 transform);
void DrawBox(box2 a, mat2 transform);
void DrawBox(box3 a, mat4 transform, vec4 color);
void DrawBox(box2 a, mat2 transform, vec4 color);
void DrawBox(vec3 a, vec3 b, mat4 transform);
void DrawBox(vec2 a, vec2 b, mat2 transform);
void DrawBox(vec2 a, float s, mat2 transform);
void DrawBox(vec3 a, vec3 b, mat4 transform, vec4 color);
void DrawBox(vec2 a, vec2 b, mat2 transform, vec4 color);
void DrawBox(vec2 a, float s, mat2 transform, vec4 color);
//-- CIRCLE
void DrawCircle(vec2 a, float s);
void DrawCircle(vec3 a, vec3 n);
void DrawCircle(vec2 a, vec2 s);
void DrawCircle(vec3 a, vec3 n, vec4 color);
void DrawCircle(vec2 a, vec2 s, vec4 color);
void DrawCircle(vec3 a, vec3 x, vec3 y);
void DrawCircle(vec2 a, vec2 x, vec2 y);
void DrawCircle(vec3 a, vec3 x, vec3 y, vec4 color);
void DrawCircle(vec2 a, vec2 x, vec2 y, vec4 color);
//-- SPHERE
void DrawSphere(vec3 a, float s);
void DrawSphere(vec3 a, float s, vec4 color);
void DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z);
void DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z, vec4 color);
//-- CAPSULE
void DrawCapsule(vec3 a, float s, vec3 h);
void DrawCapsule(vec3 a, float s, vec3 h, vec4 color);
void DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h);
void DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h, vec4 color);
//-- VIEW PROJ
void DrawViewProj(mat4 view_proj);
void DrawViewProj(mat4 view_proj, vec4 color);
void DrawViewProj(mat4 view, mat4 proj);
void DrawViewProj(mat4 view, mat4 proj, vec4 color);

} /* namespace Debug */

} /* namespace lol */

