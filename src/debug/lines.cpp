//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{

//Resets draw infos
void Debug::DrawSetupReset()
{
    Scene& scene = Scene::GetScene();
    scene.SetLineTime();
    scene.SetLineMask();
}
//Sets draw infos
void Debug::DrawSetupTime(float new_time)
{
    Scene& scene = Scene::GetScene();
    scene.SetLineTime(new_time);
}

//--
void Debug::DrawSetupMask(int new_mask)
{
    Scene& scene = Scene::GetScene();
    scene.SetLineMask(new_mask);
}

//--
void Debug::DrawSetupSegment(float new_segment_size)
{
    Scene& scene = Scene::GetScene();
    scene.SetLineSegmentSize(new_segment_size);
}

//--
void Debug::DrawSetupColor(vec4 color)
{
    Scene& scene = Scene::GetScene();
    scene.SetLineColor(color);
}

//--
void Debug::DrawSetup(float new_time, int new_mask, float segment_size, vec4 color)
{
    Debug::DrawSetupTime(new_time);
    Debug::DrawSetupMask(new_mask);
    Debug::DrawSetupSegment(segment_size);
    Debug::DrawSetupColor(color);
}

//Screen to world conversion
vec3 Debug::WorldToScreen(vec3 pos)
{
    return Debug::WorldToScreen(vec4(pos, 1.f));
}
vec3 Debug::WorldToScreen(vec3 pos, mat4 view_proj)
{
    return Debug::WorldToScreen(vec4(pos, 1.f), view_proj);
}
vec3 Debug::WorldToScreen(vec4 pos)
{
    Scene& scene = Scene::GetScene();
    if (!scene.GetCamera())
        return vec3::zero;
    mat4 const view_proj = scene.GetCamera()->GetProjection() * scene.GetCamera()->GetView();
    return Debug::WorldToScreen(pos, view_proj);
}
vec3 Debug::WorldToScreen(vec4 pos, mat4 view_proj)
{
    vec4 screen_pos = view_proj * pos;
    return (screen_pos / screen_pos.w).xyz;
}

//--
vec3 Debug::ScreenToWorld(vec2 pos, float z)
   { return Debug::ScreenToWorld(vec3(pos, z)); }
vec3 Debug::ScreenToWorld(vec3 pos)
{
    Scene& scene = Scene::GetScene();
    if (!scene.GetCamera())
        return vec3::zero;
    mat4 const inv_view_proj = inverse(scene.GetCamera()->GetProjection() * scene.GetCamera()->GetView());
    return Debug::ScreenToWorld(pos, inv_view_proj);
}
vec3 Debug::ScreenToWorld(vec2 pos, mat4 inv_view_proj, float z)
   { return Debug::ScreenToWorld(vec3(pos, z), inv_view_proj); }
vec3 Debug::ScreenToWorld(vec3 pos, mat4 inv_view_proj)
{
    vec4 screen_pos = inv_view_proj * vec4(pos, 1.f);
    return (screen_pos / screen_pos.w).xyz;
}
vec3 Debug::ScreenToWorld(vec3 pos, mat4 view, mat4 proj)
{
    vec4 screen_pos = inverse(proj) * vec4(pos, 1.f);
    return (inverse(view) * (screen_pos / screen_pos.w)).xyz;
}

//Draw stuff in World
//-- LINE: 3D -2D - 3D_to_2D --------------------------------------------------

void Debug::DrawLine(vec3 a, vec3 b)                                { Scene& scene = Scene::GetScene(); Debug::DrawLine(a, b, scene.GetLineColor()); }
void Debug::DrawLine(vec2 a, vec3 b, float az)                      { Scene& scene = Scene::GetScene(); Debug::DrawLine(a, b, scene.GetLineColor(), az); }
void Debug::DrawLine(vec2 a, vec2 b, float az, float bz)            { Scene& scene = Scene::GetScene(); Debug::DrawLine(a, b, scene.GetLineColor(), az, bz); }
void Debug::DrawLine(vec3 a, vec3 b, vec4 color)                    { Scene& scene = Scene::GetScene(); scene.AddLine(a, b, color); }
void Debug::DrawLine(vec2 a, vec3 b, vec4 color, float az)          { Scene& scene = Scene::GetScene(); scene.AddLine(vec3(a, az), b, color); }
void Debug::DrawLine(vec2 a, vec2 b, vec4 color, float az, float bz){ Scene& scene = Scene::GetScene(); scene.AddLine(vec3(a, az), vec3(b, bz), color); }

//-- GIZMO --------------------------------------------------------------------
void Debug::DrawGizmo(vec3 pos, vec3 x, vec3 y, vec3 z, float size)
{
    Debug::DrawLine(pos, pos + x * size, Color::red);
    Debug::DrawLine(pos, pos + y * size, Color::green);
    Debug::DrawLine(pos, pos + z * size, Color::blue);
}
void Debug::DrawGizmo(vec2 pos, vec3 x, vec3 y, vec3 z, float size, float posz)
{
    Debug::DrawLine(pos, pos + x.xy * size, Color::red, posz, posz + x.z * size);
    Debug::DrawLine(pos, pos + y.xy * size, Color::green, posz, posz + y.z * size);
    Debug::DrawLine(pos, pos + z.xy * size, Color::blue, posz, posz + z.z * size);
}

//-- GRID ---------------------------------------------------------------------
void Debug::DrawGrid(vec3 pos, vec3 x, vec3 y, vec3 z, float size, bool draw_3d)
{
    Scene& scene = Scene::GetScene();
    float seg_sz = scene.GetLineSegmentSize();
    int seg_nb = lol::max((int)(size / seg_sz), 1);
    seg_sz = size / (float)seg_nb;

    //Draw Gizmo
    Debug::DrawGizmo(pos, x, y, z, size);
    Debug::DrawLine(pos, pos - x * size, Color::gray);
    Debug::DrawLine(pos, pos - y * size, Color::gray);
    Debug::DrawLine(pos, pos - z * size, Color::gray);

    //grid
    for (float i = (float)-seg_nb; i <= seg_nb; i++)
    {
        if (i != 0)
        {
            float iter = seg_sz * ((float)i);
            Debug::DrawLine(vec3(-size, 0, iter), vec3(size, 0, iter), Color::dark_gray);
            Debug::DrawLine(vec3(iter, 0, -size), vec3(iter, 0, size), Color::dark_gray);

            if (draw_3d)
            {
                Debug::DrawLine(vec3(0, -size, iter), vec3(0, size, iter), Color::dark_gray);
                Debug::DrawLine(vec3(0, iter, -size), vec3(0, iter, size), Color::dark_gray);

                Debug::DrawLine(vec3(-size, iter, 0), vec3(size, iter, 0), Color::dark_gray);
                Debug::DrawLine(vec3(iter, -size, 0), vec3(iter, size, 0), Color::dark_gray);
            }
        }
    }

}

//-- ARROW: 3D -2D - 3D_to_2D -------------------------------------------------
void Debug::DrawArrow(vec3 a, vec3 b, vec2 s)                       { Debug::DrawArrow(a, b, vec3(s.x, s.y, s.y)); }
void Debug::DrawArrow(vec2 a, vec3 b, vec2 s, float az)             { Debug::DrawArrow(a, b.xy, vec3(s.x, s.y, s.y), az, b.z); }
void Debug::DrawArrow(vec2 a, vec2 b, vec2 s, float az, float bz)   { Debug::DrawArrow(a, b, vec3(s.x, s.y, s.y), az, bz); }
void Debug::DrawArrow(vec3 a, vec3 b, vec3 s)                       { Scene& scene = Scene::GetScene(); Debug::DrawArrow(a, b,    vec3(s.x, s.y, s.y), scene.GetLineColor()); }
void Debug::DrawArrow(vec2 a, vec3 b, vec3 s, float az)             { Scene& scene = Scene::GetScene(); Debug::DrawArrow(a, b.xy, vec3(s.x, s.y, s.y), scene.GetLineColor(), az, b.z); }
void Debug::DrawArrow(vec2 a, vec2 b, vec3 s, float az, float bz)   { Scene& scene = Scene::GetScene(); Debug::DrawArrow(a, b,    vec3(s.x, s.y, s.y), scene.GetLineColor(), az, bz); }
void Debug::DrawArrow(vec3 a, vec3 b, vec3 s, vec4 color)
{
    vec3 z = s.x * normalize(b - a);
    vec3 x = s.z * orthonormal(b - a);
    vec3 y = s.y * cross(normalize(x), normalize(z));

    Debug::DrawLine(a, b, color);
    Debug::DrawLine(b, b - z + x, color);
    Debug::DrawLine(b, b - z - x, color);
    Debug::DrawLine(b, b - z + y, color);
    Debug::DrawLine(b, b - z - y, color);

    Debug::DrawLine(b - z + x, b - z + y, color);
    Debug::DrawLine(b - z + x, b - z - y, color);
    Debug::DrawLine(b - z - x, b - z + y, color);
    Debug::DrawLine(b - z - x, b - z - y, color);

    Debug::DrawLine(b - z + x, b - z - x, color);
    Debug::DrawLine(b - z + y, b - z - y, color);
}
void Debug::DrawArrow(vec2 a, vec3 b, vec3 s, vec4 color, float az)
{
    vec3 bn = Debug::WorldToScreen(b);
    DrawArrow(a, bn.xy, s, color, az, bn.z);
}
void Debug::DrawArrow(vec2 a, vec2 b, vec3 s, vec4 color, float az, float bz)
{
    vec3 an = vec3(a, az);
    vec3 bn = vec3(b, bz);
    vec3 z = s.x * normalize(bn - an);
    vec3 x = s.z * orthonormal(bn - an);
    vec3 y = s.y * cross(normalize(x), normalize(z));

    Debug::DrawLine(a, b, color, az, bz);
    Debug::DrawLine(b, b - (z + x).xy, color, bz, bz - (z + x).z);
    Debug::DrawLine(b, b - (z - x).xy, color, bz, bz - (z - x).z);
    Debug::DrawLine(b, b - (z + y).xy, color, bz, bz - (z + y).z);
    Debug::DrawLine(b, b - (z - y).xy, color, bz, bz - (z - y).z);

    Debug::DrawLine(b - (z + x).xy, b - (z + y).xy, color, bz - (z + x).z, bz - (z + y).z);
    Debug::DrawLine(b - (z + x).xy, b - (z - y).xy, color, bz - (z + x).z, bz - (z - y).z);
    Debug::DrawLine(b - (z - x).xy, b - (z + y).xy, color, bz - (z - x).z, bz - (z + y).z);
    Debug::DrawLine(b - (z - x).xy, b - (z - y).xy, color, bz - (z - x).z, bz - (z - y).z);

    Debug::DrawLine(b - (z + x).xy, b - (z - x).xy, color, bz - (z + x).z, bz - (z - x).z);
    Debug::DrawLine(b - (z + y).xy, b - (z - y).xy, color, bz - (z + y).z, bz - (z - y).z);
}

//-- BOX: 3D -2D - 3D_to_2D ---------------------------------------------------
void Debug::DrawBox(box3 a)                             { Scene& scene = Scene::GetScene(); Debug::DrawBox(a.aa, a.bb, scene.GetLineColor()); }
void Debug::DrawBox(box2 a)                             { Scene& scene = Scene::GetScene(); Debug::DrawBox(a.aa, a.bb, scene.GetLineColor()); }
void Debug::DrawBox(box3 a, vec4 color)                 { Scene& scene = Scene::GetScene(); Debug::DrawBox(a.aa, a.bb, color); }
void Debug::DrawBox(box2 a, vec4 color)                 { Scene& scene = Scene::GetScene(); Debug::DrawBox(a.aa, a.bb, color); }
void Debug::DrawBox(vec3 a, vec3 b)                     { Scene& scene = Scene::GetScene(); Debug::DrawBox(a, b, scene.GetLineColor()); }
void Debug::DrawBox(vec2 a, vec2 b)                     { Scene& scene = Scene::GetScene(); Debug::DrawBox(a, b, scene.GetLineColor()); }
void Debug::DrawBox(vec2 a, float s)                    { Scene& scene = Scene::GetScene(); Debug::DrawBox(a, s, scene.GetLineColor()); }
void Debug::DrawBox(vec3 a, vec3 b, vec4 color)         { Scene& scene = Scene::GetScene(); Debug::DrawBox(a, b, mat4::identity, color); }
void Debug::DrawBox(vec2 a, vec2 b, vec4 color)         { Scene& scene = Scene::GetScene(); Debug::DrawBox(a, b, mat2::identity, color); }
void Debug::DrawBox(vec2 a, float s, vec4 color)        { Scene& scene = Scene::GetScene(); Debug::DrawBox(a, s, mat2::identity, color); }
void Debug::DrawBox(box3 a, mat4 transform)             { Scene& scene = Scene::GetScene(); Debug::DrawBox(a.aa, a.bb, transform, scene.GetLineColor()); }
void Debug::DrawBox(box2 a, mat2 transform)             { Scene& scene = Scene::GetScene(); Debug::DrawBox(a.aa, a.bb, transform, scene.GetLineColor()); }
void Debug::DrawBox(box3 a, mat4 transform, vec4 color) { Scene& scene = Scene::GetScene(); Debug::DrawBox(a.aa, a.bb, transform, color); }
void Debug::DrawBox(box2 a, mat2 transform, vec4 color) { Scene& scene = Scene::GetScene(); Debug::DrawBox(a.aa, a.bb, transform, color); }
void Debug::DrawBox(vec3 a, vec3 b, mat4 transform)     { Scene& scene = Scene::GetScene(); Debug::DrawBox(a, b, transform, scene.GetLineColor()); }
void Debug::DrawBox(vec2 a, vec2 b, mat2 transform)     { Scene& scene = Scene::GetScene(); Debug::DrawBox(a, b, transform, scene.GetLineColor()); }
void Debug::DrawBox(vec2 a, float s, mat2 transform)    { Scene& scene = Scene::GetScene(); Debug::DrawBox(a, s, transform, scene.GetLineColor()); }
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
void Debug::DrawBox(vec2 a, vec2 b, mat2 transform, vec4 color)
{
    vec2 v[4];
    v[0] = a;
    v[1] = vec2(a.x, b.y);
    v[2] = b;
    v[3] = vec2(b.x, a.y);

    int i = 0;
    Debug::DrawLine((transform * v[0]).xy, (transform * v[1]).xy, color);
    Debug::DrawLine((transform * v[1]).xy, (transform * v[2]).xy, color);
    Debug::DrawLine((transform * v[2]).xy, (transform * v[3]).xy, color);
    Debug::DrawLine((transform * v[3]).xy, (transform * v[0]).xy, color);
}
void Debug::DrawBox(vec2 a, float s, mat2 transform, vec4 color)
{
    vec2 b = s * vec2(1.f, g_renderer->GetXYRatio());
    Debug::DrawBox(a - b, a + b, transform, color);
}

//-- CIRCLE -------------------------------------------------------------------
void Debug::DrawCircle(vec2 a, float s)                 { Scene& scene = Scene::GetScene(); Debug::DrawCircle(a, s * vec2(1.f, g_renderer->GetXYRatio()), scene.GetLineColor()); }
void Debug::DrawCircle(vec3 a, vec3 n)                  { Scene& scene = Scene::GetScene(); Debug::DrawCircle(a, n, scene.GetLineColor()); }
void Debug::DrawCircle(vec2 a, vec2 s)                  { Scene& scene = Scene::GetScene(); Debug::DrawCircle(a, s * vec2(1.f, g_renderer->GetXYRatio()), scene.GetLineColor()); }
void Debug::DrawCircle(vec3 a, vec3 x, vec3 y)          { Scene& scene = Scene::GetScene(); Debug::DrawCircle(a, x, y, scene.GetLineColor()); }
void Debug::DrawCircle(vec2 a, vec2 x, vec2 y)          { Scene& scene = Scene::GetScene(); Debug::DrawCircle(a, x, y, scene.GetLineColor()); }
void Debug::DrawCircle(vec3 a, vec3 n, vec4 color)
{
    vec3 x = orthogonal(n);
    vec3 y = cross(normalize(n), normalize(x)) * length(n);
    DrawCircle(a, x, y, color);
}
void Debug::DrawCircle(vec2 a, vec2 s, vec4 color)
{
    vec2 x = vec2::axis_x * s.x;
    vec2 y = vec2::axis_y * s.y;
    DrawCircle(a, x, y, color);
}
//--
void Debug::DrawCircle(vec3 a, vec3 x, vec3 y, vec4 color)
{
    Scene& scene = Scene::GetScene();
    float size = F_PI * 2.f * lol::max(length(x), length(y));
    int segment_nb = lol::max(1, (int)((size * .25f) / scene.GetLineSegmentSize()));
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
void Debug::DrawCircle(vec2 a, vec2 x, vec2 y, vec4 color)
{
    Scene& scene = Scene::GetScene();
    float size = F_PI * 2.f * lol::max(length(x), length(y));
    int segment_nb = lol::max(1, (int)((size * .25f) / scene.GetLineSegmentSize()));
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

//-- SPHERE -------------------------------------------------------------------
void Debug::DrawSphere(vec3 a, float s)                 { Scene& scene = Scene::GetScene(); Debug::DrawSphere(a, s, scene.GetLineColor()); }
void Debug::DrawSphere(vec3 a, float s, vec4 color)     { Debug::DrawSphere(a, vec3::axis_x * s, vec3::axis_y * s, vec3::axis_z * s, color); }
void Debug::DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z)  { Scene& scene = Scene::GetScene(); Debug::DrawSphere(a, x, y, z, scene.GetLineColor()); }
void Debug::DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z, vec4 color)
{
    Debug::DrawCircle(a, x, y, color);
    Debug::DrawCircle(a, x, (y + z) * .707f, color);
    Debug::DrawCircle(a, x, (y - z) * .707f, color);
    Debug::DrawCircle(a, z, x, color);
    Debug::DrawCircle(a, z, (x + y) * .707f, color);
    Debug::DrawCircle(a, z, (x - y) * .707f, color);
    Debug::DrawCircle(a, y, z, color);
    Debug::DrawCircle(a, y, (z + x) * .707f, color);
    Debug::DrawCircle(a, y, (z - x) * .707f, color);
}

//-- CAPSULE ------------------------------------------------------------------
void Debug::DrawCapsule(vec3 a, float s, vec3 h)                { Scene& scene = Scene::GetScene(); Debug::DrawCapsule(a, s, h, scene.GetLineColor()); }
void Debug::DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h) { Scene& scene = Scene::GetScene(); Debug::DrawCapsule(a, x, y, z, h, scene.GetLineColor()); }
void Debug::DrawCapsule(vec3 a, float s, vec3 h, vec4 color)
{
    vec3 x = orthonormal(h) * s;
    vec3 y = cross(normalize(h), normalize(x)) * s;
    Debug::DrawCapsule(a, x, y, normalize(h) * s, h, color);
}
//--
void Debug::DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h, vec4 color)
{
    Scene& scene = Scene::GetScene();
    float size = F_PI * 2.f * lol::max(length(x), length(y));
    int segment_nb = lol::max(1, (int)((size * .25f) / scene.GetLineSegmentSize()));
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

//-- VIEW PROJ ----------------------------------------------------------------
void Debug::DrawViewProj(mat4 view, mat4 proj)  { Scene& scene = Scene::GetScene(); Debug::DrawViewProj(view, proj, scene.GetLineColor()); }
void Debug::DrawViewProj(mat4 view_proj)        { Scene& scene = Scene::GetScene(); Debug::DrawViewProj(view_proj,  scene.GetLineColor()); }
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

