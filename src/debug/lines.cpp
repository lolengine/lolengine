//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{
Debug::DrawContext::Data Debug::DrawContext::m_global = Debug::DrawContext::Data(vec4(1.f));

typedef Debug::DrawContext       DC;
typedef Debug::DrawContext::Data DCD;

//Screen to world conversion
vec3 Debug::WorldToScreen(vec3 pos)                 { return Debug::WorldToScreen(vec4(pos, 1.f)); }
vec3 Debug::WorldToScreen(vec3 pos, mat4 view_proj) { return Debug::WorldToScreen(vec4(pos, 1.f), view_proj); }
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
vec3 Debug::ScreenToWorld(vec2 pos, float z) { return Debug::ScreenToWorld(vec3(pos, z)); }
vec3 Debug::ScreenToWorld(vec3 pos)
{
    Scene& scene = Scene::GetScene();
    if (!scene.GetCamera())
        return vec3::zero;
    mat4 const inv_view_proj = inverse(scene.GetCamera()->GetProjection() * scene.GetCamera()->GetView());
    return Debug::ScreenToWorld(pos, inv_view_proj);
}
vec3 Debug::ScreenToWorld(vec2 pos, mat4 inv_view_proj, float z) { return Debug::ScreenToWorld(vec3(pos, z), inv_view_proj); }
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

//Root func
void Debug::DrawLine(vec3 a, vec3 b, DCD data)
{
    Scene::GetScene().AddLine(a, b, data.m_color, data.m_duration, data.m_mask);
}
void Debug::DrawLine(vec2 a, vec3 b, DCD data, float az)           { DrawLine(vec3(a, az), b, data); }
void Debug::DrawLine(vec2 a, vec2 b, DCD data, float az, float bz) { DrawLine(vec3(a, az), vec3(b, bz), data); }
void Debug::DrawLine(vec3 a, vec3 b)                               { DrawLine(a, b, DC::GetGlobalData()); }
void Debug::DrawLine(vec2 a, vec3 b, float az)                     { DrawLine(a, b, DC::GetGlobalData(), az); }
void Debug::DrawLine(vec2 a, vec2 b, float az, float bz)           { DrawLine(a, b, DC::GetGlobalData(), az, bz); }

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
    float seg_sz = DC::GetGlobalData().m_segment_size;
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
void Debug::DrawArrow(vec3 a, vec3 b, vec2 s)                     { Debug::DrawArrow(a, b, vec3(s.x, s.y, s.y)); }
void Debug::DrawArrow(vec2 a, vec3 b, vec2 s, float az)           { Debug::DrawArrow(a, b.xy, vec3(s.x, s.y, s.y), az, b.z); }
void Debug::DrawArrow(vec2 a, vec2 b, vec2 s, float az, float bz) { Debug::DrawArrow(a, b, vec3(s.x, s.y, s.y), az, bz); }
void Debug::DrawArrow(vec3 a, vec3 b, vec3 s)                     { Debug::DrawArrow(a, b,    vec3(s.x, s.y, s.y), DC::GetGlobalData()); }
void Debug::DrawArrow(vec2 a, vec3 b, vec3 s, float az)           { Debug::DrawArrow(a, b.xy, vec3(s.x, s.y, s.y), DC::GetGlobalData(), az, b.z); }
void Debug::DrawArrow(vec2 a, vec2 b, vec3 s, float az, float bz) { Debug::DrawArrow(a, b,    vec3(s.x, s.y, s.y), DC::GetGlobalData(), az, bz); }
void Debug::DrawArrow(vec3 a, vec3 b, vec3 s, DCD data)
{
    vec3 z = s.x * normalize(b - a);
    vec3 x = s.z * orthonormal(b - a);
    vec3 y = s.y * cross(normalize(x), normalize(z));

    Debug::DrawLine(a, b, data);
    Debug::DrawLine(b, b - z + x, data);
    Debug::DrawLine(b, b - z - x, data);
    Debug::DrawLine(b, b - z + y, data);
    Debug::DrawLine(b, b - z - y, data);

    Debug::DrawLine(b - z + x, b - z + y, data);
    Debug::DrawLine(b - z + x, b - z - y, data);
    Debug::DrawLine(b - z - x, b - z + y, data);
    Debug::DrawLine(b - z - x, b - z - y, data);

    Debug::DrawLine(b - z + x, b - z - x, data);
    Debug::DrawLine(b - z + y, b - z - y, data);
}
void Debug::DrawArrow(vec2 a, vec3 b, vec3 s, DCD data, float az)
{
    vec3 bn = Debug::WorldToScreen(b);
    DrawArrow(a, bn.xy, s, data, az, bn.z);
}
void Debug::DrawArrow(vec2 a, vec2 b, vec3 s, DCD data, float az, float bz)
{
    vec3 an = vec3(a, az);
    vec3 bn = vec3(b, bz);
    vec3 z = s.x * normalize(bn - an);
    vec3 x = s.z * orthonormal(bn - an);
    vec3 y = s.y * cross(normalize(x), normalize(z));

    Debug::DrawLine(a, b, data, az, bz);
    Debug::DrawLine(b, b - (z + x).xy, data, bz, bz - (z + x).z);
    Debug::DrawLine(b, b - (z - x).xy, data, bz, bz - (z - x).z);
    Debug::DrawLine(b, b - (z + y).xy, data, bz, bz - (z + y).z);
    Debug::DrawLine(b, b - (z - y).xy, data, bz, bz - (z - y).z);

    Debug::DrawLine(b - (z + x).xy, b - (z + y).xy, data, bz - (z + x).z, bz - (z + y).z);
    Debug::DrawLine(b - (z + x).xy, b - (z - y).xy, data, bz - (z + x).z, bz - (z - y).z);
    Debug::DrawLine(b - (z - x).xy, b - (z + y).xy, data, bz - (z - x).z, bz - (z + y).z);
    Debug::DrawLine(b - (z - x).xy, b - (z - y).xy, data, bz - (z - x).z, bz - (z - y).z);

    Debug::DrawLine(b - (z + x).xy, b - (z - x).xy, data, bz - (z + x).z, bz - (z - x).z);
    Debug::DrawLine(b - (z + y).xy, b - (z - y).xy, data, bz - (z + y).z, bz - (z - y).z);
}

//-- BOX: 3D -2D - 3D_to_2D ---------------------------------------------------
void Debug::DrawBox(box3 a)                           { Debug::DrawBox(a.aa, a.bb, DC::GetGlobalData()); }
void Debug::DrawBox(box2 a)                           { Debug::DrawBox(a.aa, a.bb, DC::GetGlobalData()); }
void Debug::DrawBox(box3 a, DCD data)                 { Debug::DrawBox(a.aa, a.bb, data); }
void Debug::DrawBox(box2 a, DCD data)                 { Debug::DrawBox(a.aa, a.bb, data); }
void Debug::DrawBox(vec3 a, vec3 b)                   { Debug::DrawBox(a, b, DC::GetGlobalData()); }
void Debug::DrawBox(vec2 a, vec2 b)                   { Debug::DrawBox(a, b, DC::GetGlobalData()); }
void Debug::DrawBox(vec2 a, float s)                  { Debug::DrawBox(a, s, DC::GetGlobalData()); }
void Debug::DrawBox(vec3 a, vec3 b, DCD data)         { Debug::DrawBox(a, b, mat4::identity, data); }
void Debug::DrawBox(vec2 a, vec2 b, DCD data)         { Debug::DrawBox(a, b, mat2::identity, data); }
void Debug::DrawBox(vec2 a, float s, DCD data)        { Debug::DrawBox(a, s, mat2::identity, data); }
void Debug::DrawBox(box3 a, mat4 transform)           { Debug::DrawBox(a.aa, a.bb, transform, DC::GetGlobalData()); }
void Debug::DrawBox(box2 a, mat2 transform)           { Debug::DrawBox(a.aa, a.bb, transform, DC::GetGlobalData()); }
void Debug::DrawBox(box3 a, mat4 transform, DCD data) { Debug::DrawBox(a.aa, a.bb, transform, data); }
void Debug::DrawBox(box2 a, mat2 transform, DCD data) { Debug::DrawBox(a.aa, a.bb, transform, data); }
void Debug::DrawBox(vec3 a, vec3 b, mat4 transform)   { Debug::DrawBox(a, b, transform, DC::GetGlobalData()); }
void Debug::DrawBox(vec2 a, vec2 b, mat2 transform)   { Debug::DrawBox(a, b, transform, DC::GetGlobalData()); }
void Debug::DrawBox(vec2 a, float s, mat2 transform)  { Debug::DrawBox(a, s, transform, DC::GetGlobalData()); }
void Debug::DrawBox(vec3 a, vec3 b, mat4 transform, DCD data)
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

        Debug::DrawLine((transform * v[i]).xyz,     (transform * v[i + 4]).xyz, data);
        Debug::DrawLine((transform * v[i]).xyz,     (transform * v[j]).xyz,     data);
        Debug::DrawLine((transform * v[i + 4]).xyz, (transform * v[j + 4]).xyz, data);
    }
}
void Debug::DrawBox(vec2 a, vec2 b, mat2 transform, DCD data)
{
    vec2 v[4];
    v[0] = a;
    v[1] = vec2(a.x, b.y);
    v[2] = b;
    v[3] = vec2(b.x, a.y);

    Debug::DrawLine((transform * v[0]).xy, (transform * v[1]).xy, data);
    Debug::DrawLine((transform * v[1]).xy, (transform * v[2]).xy, data);
    Debug::DrawLine((transform * v[2]).xy, (transform * v[3]).xy, data);
    Debug::DrawLine((transform * v[3]).xy, (transform * v[0]).xy, data);
}
void Debug::DrawBox(vec2 a, float s, mat2 transform, DCD data)
{
    vec2 b = s * vec2(1.f, Scene::GetScene(0).get_renderer()->GetXYRatio());
    Debug::DrawBox(a - b, a + b, transform, data);
}

//-- CIRCLE -------------------------------------------------------------------
void Debug::DrawCircle(vec2 a, float s)        { Debug::DrawCircle(a, s * vec2(1.f, Scene::GetScene(0).get_renderer()->GetXYRatio()), DC::GetGlobalData()); }
void Debug::DrawCircle(vec3 a, vec3 n)         { Debug::DrawCircle(a, n, DC::GetGlobalData()); }
void Debug::DrawCircle(vec2 a, vec2 s)         { Debug::DrawCircle(a, s * vec2(1.f, Scene::GetScene(0).get_renderer()->GetXYRatio()), DC::GetGlobalData()); }
void Debug::DrawCircle(vec3 a, vec3 x, vec3 y) { Debug::DrawCircle(a, x, y, DC::GetGlobalData()); }
void Debug::DrawCircle(vec2 a, vec2 x, vec2 y) { Debug::DrawCircle(a, x, y, DC::GetGlobalData()); }
void Debug::DrawCircle(vec3 a, vec3 n, DCD data)
{
    vec3 x = orthogonal(n);
    vec3 y = cross(normalize(n), normalize(x)) * length(n);
    DrawCircle(a, x, y, data);
}
void Debug::DrawCircle(vec2 a, vec2 s, DCD data)
{
    vec2 x = vec2::axis_x * s.x;
    vec2 y = vec2::axis_y * s.y;
    DrawCircle(a, x, y, data);
}
//--
void Debug::DrawCircle(vec3 a, vec3 x, vec3 y, DCD data)
{
    float size = F_PI * 2.f * lol::max(length(x), length(y));
    int segment_nb = lol::max(1, (int)((size * .25f) / data.m_segment_size));
    for (int i = 0; i < segment_nb; i++)
    {
        float a0 = (((float)i)     / (float)segment_nb) * F_PI_2;
        float a1 = (((float)i + 1) / (float)segment_nb) * F_PI_2;
        vec2 p0 = vec2(lol::cos(a0), lol::sin(a0));
        vec2 p1 = vec2(lol::cos(a1), lol::sin(a1));

        Debug::DrawLine(a + p0.x *  x + p0.y *  y, a + p1.x *  x + p1.y *  y, data);
        Debug::DrawLine(a + p0.x * -x + p0.y * -y, a + p1.x * -x + p1.y * -y, data);
        Debug::DrawLine(a + p0.x *  x + p0.y * -y, a + p1.x *  x + p1.y * -y, data);
        Debug::DrawLine(a + p0.x * -x + p0.y *  y, a + p1.x * -x + p1.y *  y, data);
    }
}
//--
void Debug::DrawCircle(vec2 a, vec2 x, vec2 y, DCD data)
{
    float size = F_PI * 2.f * lol::max(length(x), length(y));
    int segment_nb = lol::max(1, (int)((size * .25f) / data.m_segment_size));
    for (int i = 0; i < segment_nb; i++)
    {
        float a0 = (((float)i)     / (float)segment_nb) * F_PI_2;
        float a1 = (((float)i + 1) / (float)segment_nb) * F_PI_2;
        vec2 p0 = vec2(lol::cos(a0), lol::sin(a0));
        vec2 p1 = vec2(lol::cos(a1), lol::sin(a1));

        Debug::DrawLine(a + p0.x *  x + p0.y *  y, a + p1.x *  x + p1.y *  y, data);
        Debug::DrawLine(a + p0.x * -x + p0.y * -y, a + p1.x * -x + p1.y * -y, data);
        Debug::DrawLine(a + p0.x *  x + p0.y * -y, a + p1.x *  x + p1.y * -y, data);
        Debug::DrawLine(a + p0.x * -x + p0.y *  y, a + p1.x * -x + p1.y *  y, data);
    }
}

//-- SPHERE -------------------------------------------------------------------
void Debug::DrawSphere(vec3 a, float s)                { Debug::DrawSphere(a, s, DC::GetGlobalData()); }
void Debug::DrawSphere(vec3 a, float s, DCD data)      { Debug::DrawSphere(a, vec3::axis_x * s, vec3::axis_y * s, vec3::axis_z * s, data); }
void Debug::DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z) { Debug::DrawSphere(a, x, y, z, DC::GetGlobalData()); }
void Debug::DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z, DCD data)
{
    Debug::DrawCircle(a, x, y, data);
    Debug::DrawCircle(a, x, (y + z) * .707f, data);
    Debug::DrawCircle(a, x, (y - z) * .707f, data);
    Debug::DrawCircle(a, z, x, data);
    Debug::DrawCircle(a, z, (x + y) * .707f, data);
    Debug::DrawCircle(a, z, (x - y) * .707f, data);
    Debug::DrawCircle(a, y, z, data);
    Debug::DrawCircle(a, y, (z + x) * .707f, data);
    Debug::DrawCircle(a, y, (z - x) * .707f, data);
}

//-- CAPSULE ------------------------------------------------------------------
void Debug::DrawCapsule(vec3 a, float s, vec3 h)                { Debug::DrawCapsule(a, s, h, DC::GetGlobalData()); }
void Debug::DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h) { Debug::DrawCapsule(a, x, y, z, h, DC::GetGlobalData()); }
void Debug::DrawCapsule(vec3 a, float s, vec3 h, DCD data)
{
    vec3 x = orthonormal(h) * s;
    vec3 y = cross(normalize(h), normalize(x)) * s;
    Debug::DrawCapsule(a, x, y, normalize(h) * s, h, data);
}
//--
void Debug::DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h, DCD data)
{
    float size = F_PI * 2.f * lol::max(length(x), length(y));
    int segment_nb = lol::max(1, (int)((size * .25f) / data.m_segment_size));
    for (int i = -1; i < 2; i += 2)
    {
        vec3 b = a + h * .5f * (float)i;
        vec3 c = a - h * .5f * (float)i;
        Debug::DrawCircle(b, x, y, data);
        Debug::DrawLine(b + x * (float)i, c + x * (float)i, data);
        Debug::DrawLine(b + y * (float)i, c + y * (float)i, data);
        for (int j = 0; j < segment_nb; j++)
        {
            float a0 = (((float)j)     / (float)segment_nb) * F_PI_2;
            float a1 = (((float)j + 1) / (float)segment_nb) * F_PI_2;
            vec2 p0 = vec2(lol::cos(a0), lol::sin(a0));
            vec2 p1 = vec2(lol::cos(a1), lol::sin(a1));

            Debug::DrawLine(b + p0.x *  x + p0.y * z * (float)i, b + p1.x *  x + p1.y * z * (float)i, data);
            Debug::DrawLine(b + p0.x * -x + p0.y * z * (float)i, b + p1.x * -x + p1.y * z * (float)i, data);
            Debug::DrawLine(b + p0.x *  y + p0.y * z * (float)i, b + p1.x *  y + p1.y * z * (float)i, data);
            Debug::DrawLine(b + p0.x * -y + p0.y * z * (float)i, b + p1.x * -y + p1.y * z * (float)i, data);
        }
    }
}

//-- VIEW PROJ ----------------------------------------------------------------
void Debug::DrawViewProj(mat4 view, mat4 proj) { Debug::DrawViewProj(view, proj, DC::GetGlobalData()); }
void Debug::DrawViewProj(mat4 view_proj)       { Debug::DrawViewProj(view_proj,  DC::GetGlobalData()); }
void Debug::DrawViewProj(mat4 view, mat4 proj, DCD data)
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
        Debug::DrawLine(p[i].xyz, p0, data);

    Debug::DrawViewProj(view_proj, data);
}

//--
void Debug::DrawViewProj(mat4 view_proj, DCD data)
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
        Debug::DrawLine(p[i].xyz, p[(i + 1) % 4].xyz, data);
    //Draw far
    for (int i = 4; i < 8; i++)
        Debug::DrawLine(p[i].xyz, p[(i - 4 + 1) % 4 + 4].xyz, data);
    //Draw near to far
    for (int i = 0; i < 4; i++)
        Debug::DrawLine(p[i].xyz, p[i + 4].xyz, data);
    //Draw diagonal
    for (int i = 2; i < 6; i++)
        Debug::DrawLine(p[i].xyz, p[i + ((i < 4)?(-2):(+2))].xyz, data);
}

} /* namespace lol */

