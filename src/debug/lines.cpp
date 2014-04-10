//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

namespace lol
{

//Resets draw infos
void Debug::DrawSetupReset()
{
    g_scene->SetLineTime();
    g_scene->SetLineMask();
}
//Sets draw infos
void Debug::DrawSetupTime(float new_time)
{
    g_scene->SetLineTime(new_time);
}

//--
void Debug::DrawSetupMask(int new_mask)
{
    g_scene->SetLineMask(new_mask);
}

//--
void Debug::DrawSetupSegment(float new_segment_size)
{
    g_scene->SetLineSegmentSize(new_segment_size);
}

//--
void Debug::DrawSetupColor(vec4 color)
{
    g_scene->SetLineColor(color);
}

//--
void Debug::DrawSetup(float new_time, int new_mask, float segment_size, vec4 color)
{
    Debug::DrawSetupTime(new_time);
    Debug::DrawSetupMask(new_mask);
    Debug::DrawSetupSegment(segment_size);
    Debug::DrawSetupColor(color);
}

//Draw stuff
void Debug::DrawLine(vec3 a, vec3 b)
{
    Debug::DrawLine(a, b, g_scene->GetLineColor());
}
void Debug::DrawLine(vec3 a, vec3 b, vec4 color)
{ g_scene->AddLine(a, b, color); }

//--
void Debug::DrawBox(box3 a)
{ DrawBox(a.A, a.B, g_scene->GetLineColor()); }
void Debug::DrawBox(box3 a, vec4 color)
{
    DrawBox(a.A, a.B, color);
}

//--
void Debug::DrawBox(vec3 a, vec3 b)
{ Debug::DrawBox(a, b, g_scene->GetLineColor()); }
void Debug::DrawBox(vec3 a, vec3 b, vec4 color)
{
    Debug::DrawBox(a, b, mat4::identity, color);
}

//--
void Debug::DrawBox(box3 a, mat4 transform)
{ DrawBox(a.A, a.B, transform, g_scene->GetLineColor()); }
void Debug::DrawBox(box3 a, mat4 transform, vec4 color)
{
    DrawBox(a.A, a.B, transform, color);
}

//--
void Debug::DrawBox(vec3 a, vec3 b, mat4 transform)
{ Debug::DrawBox(a, b, transform, g_scene->GetLineColor()); }
void Debug::DrawBox(vec3 a, vec3 b, mat4 transform, vec4 color)
{
    vec4 v[8];
    for (int i = 0; i < 8; i++)
    {
        v[i].x = i & 1 ? a.x : b.x;
        v[i].y = i & 2 ? a.y : b.y;
        v[i].z = i & 4 ? a.z : b.z;
        v[i].w = 1.f;
    }

    for (int i = 0; i < 4; i++)
    {
        int j = ((i & 1) << 1) | ((i >> 1) ^ 1);

        Debug::DrawLine((transform * v[i]).xyz,     (transform * v[i + 4]).xyz, color);
        Debug::DrawLine((transform * v[i]).xyz,     (transform * v[j]).xyz,     color);
        Debug::DrawLine((transform * v[i + 4]).xyz, (transform * v[j + 4]).xyz, color);
    }
}

//--
void Debug::DrawCircle(vec3 a, vec3 n)
{ Debug::DrawCircle(a, n, g_scene->GetLineColor()); }
void Debug::DrawCircle(vec3 a, vec3 n, vec4 color)
{
    vec3 x = orthogonal(n);
    vec3 y = cross(normalize(n), normalize(x)) * length(n);
    DrawCircle(a, x, y, color);
}

//--
void Debug::DrawCircle(vec3 a, vec3 x, vec3 y)
{ Debug::DrawCircle(a, x, y, g_scene->GetLineColor()); }
void Debug::DrawCircle(vec3 a, vec3 x, vec3 y, vec4 color)
{
    float size = F_PI * 2.f * lol::max(length(x), length(y));
    int segment_nb = lol::max(1, (int)((size * .25f) / g_scene->GetLineSegmentSize()));
    for (int i = 0; i < segment_nb; i++)
    {
        float a0 = (((float)i)     / (float)segment_nb) * F_PI_2;
        float a1 = (((float)i + 1) / (float)segment_nb) * F_PI_2;
        vec2 p0 = vec2(lol::cos(a0), lol::sin(a0));
        vec2 p1 = vec2(lol::cos(a1), lol::sin(a1));

        Debug::DrawLine(a + p0.x *  x + p0.y *  y, a + p1.x *  x + p1.y *  y, color);
        Debug::DrawLine(a + p0.x * -x + p0.y * -y, a + p1.x * -x + p1.y * -y, color);
        Debug::DrawLine(a + p0.x *  x + p0.y * -y, a + p1.x *  x + p1.y * -y, color);
        Debug::DrawLine(a + p0.x * -x + p0.y *  y, a + p1.x * -x + p1.y *  y, color);
    }
}

//--
void Debug::DrawSphere(vec3 a, float s)
{ Debug::DrawSphere(a, s, g_scene->GetLineColor()); }
void Debug::DrawSphere(vec3 a, float s, vec4 color)
{
    Debug::DrawSphere(a, vec3::axis_x * s, vec3::axis_y * s, vec3::axis_z * s, color);
}

//--
void Debug::DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z)
{ Debug::DrawSphere(a, x, y, z, g_scene->GetLineColor()); }
void Debug::DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z, vec4 color)
{
    Debug::DrawCircle(a, x, y, color);
    Debug::DrawCircle(a, y, z, color);
    Debug::DrawCircle(a, z, x, color);
}

//--
void Debug::DrawCapsule(vec3 a, float s, vec3 h)
{ Debug::DrawCapsule(a, s, h, g_scene->GetLineColor()); }
void Debug::DrawCapsule(vec3 a, float s, vec3 h, vec4 color)
{
    vec3 x = orthonormal(h) * s;
    vec3 y = cross(normalize(h), normalize(x)) * s;
    Debug::DrawCapsule(a, x, y, normalize(h) * s, h, color);
}

//--
void Debug::DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h)
{ Debug::DrawCapsule(a, x, y, z, h, g_scene->GetLineColor()); }
void Debug::DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h, vec4 color)
{
    float size = F_PI * 2.f * lol::max(length(x), length(y));
    int segment_nb = lol::max(1, (int)((size * .25f) / g_scene->GetLineSegmentSize()));
    for (int i = -1; i < 2; i += 2)
    {
        vec3 b = a + h * .5f * (float)i;
        vec3 c = a - h * .5f * (float)i;
        Debug::DrawCircle(b, x, y, color);
        Debug::DrawLine(b + x * (float)i, c + x * (float)i, color);
        Debug::DrawLine(b + y * (float)i, c + y * (float)i, color);
        for (int j = 0; j < segment_nb; j++)
        {
            float a0 = (((float)j)     / (float)segment_nb) * F_PI_2;
            float a1 = (((float)j + 1) / (float)segment_nb) * F_PI_2;
            vec2 p0 = vec2(lol::cos(a0), lol::sin(a0));
            vec2 p1 = vec2(lol::cos(a1), lol::sin(a1));

            Debug::DrawLine(b + p0.x *  x + p0.y * z * (float)i, b + p1.x *  x + p1.y * z * (float)i, color);
            Debug::DrawLine(b + p0.x * -x + p0.y * z * (float)i, b + p1.x * -x + p1.y * z * (float)i, color);
            Debug::DrawLine(b + p0.x *  y + p0.y * z * (float)i, b + p1.x *  y + p1.y * z * (float)i, color);
            Debug::DrawLine(b + p0.x * -y + p0.y * z * (float)i, b + p1.x * -y + p1.y * z * (float)i, color);
        }
    }
}

//--
void Debug::DrawViewProj(mat4 view, mat4 proj)
{ Debug::DrawViewProj(view, proj, g_scene->GetLineColor()); }
void Debug::DrawViewProj(mat4 view, mat4 proj, vec4 color)
{
    mat4 const view_proj = proj * view;

    //Pos to center
    vec3 p0 = (inverse(view) * vec4(vec3::zero, 1.f)).xyz;

    //Near plane
    mat4 const inv_view_proj = inverse(view_proj);
    vec4 p[4] = { inv_view_proj * vec4(-1.f, 1.f, -1.f, 1.f),
                  inv_view_proj * vec4( 1.f, 1.f, -1.f, 1.f),
                  inv_view_proj * vec4( 1.f,-1.f, -1.f, 1.f),
                  inv_view_proj * vec4(-1.f,-1.f, -1.f, 1.f) };
    for (int i = 0; i < 4; i++)
        p[i] = p[i] / p[i].w;

    //Draw near
    for (int i = 0; i < 4; i++)
        Debug::DrawLine(p[i].xyz, p0, color);

    Debug::DrawViewProj(view_proj, color);
}

//--
void Debug::DrawViewProj(mat4 view_proj)
{ Debug::DrawViewProj(view_proj, g_scene->GetLineColor()); }
void Debug::DrawViewProj(mat4 view_proj, vec4 color)
{
    //Near plane
    mat4 const inv_view_proj = inverse(view_proj);
    vec4 p[8] = { inv_view_proj * vec4(-1.f, 1.f, 1.f, 1.f),
                  inv_view_proj * vec4( 1.f, 1.f, 1.f, 1.f),
                  inv_view_proj * vec4( 1.f,-1.f, 1.f, 1.f),
                  inv_view_proj * vec4(-1.f,-1.f, 1.f, 1.f),
                  inv_view_proj * vec4(-1.f, 1.f,-1.f, 1.f),
                  inv_view_proj * vec4( 1.f, 1.f,-1.f, 1.f),
                  inv_view_proj * vec4( 1.f,-1.f,-1.f, 1.f),
                  inv_view_proj * vec4(-1.f,-1.f,-1.f, 1.f)
                };
    for (int i = 0; i < 8; i++)
        p[i] = p[i] / p[i].w;

    //Draw near
    for (int i = 0; i < 4; i++)
        Debug::DrawLine(p[i].xyz, p[(i + 1) % 4].xyz, color);
    //Draw far
    for (int i = 4; i < 8; i++)
        Debug::DrawLine(p[i].xyz, p[(i - 4 + 1) % 4 + 4].xyz, color);
    //Draw near to far
    for (int i = 0; i < 4; i++)
        Debug::DrawLine(p[i].xyz, p[i + 4].xyz, color);
    //Draw diagonal
    for (int i = 2; i < 6; i++)
        Debug::DrawLine(p[i].xyz, p[i + ((i < 4)?(-2):(+2))].xyz, color);
}

} /* namespace lol */

