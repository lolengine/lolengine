//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Debug line primitives
// ---------------------
//

#include <../legacy/lol/math/geometry.h> // box3
#include <lol/vector> // vec3, vec4 etc.

#include <stdint.h>

namespace lol
{

namespace Debug
{
    class DrawContext
    {
    public:
        class Data
        {
        public:
            vec4 m_color = vec4(1.f);
            float m_duration = 0.0f;
            int m_mask = 0;
            float m_segment_size = 8;

            Data() {}
            Data(vec4 color, float duration = -1.f, int mask = 0xFFFFFFFF, float segment_size = 1.f)
            {
                m_color = color;
                m_duration = duration;
                m_mask = mask;
                m_segment_size = segment_size;
            }
            inline Data& operator=(vec4 const& color) { m_color = color; return *this; }
        };

    private:
        static Data m_global;
        Data m_previous;
        Data m_current;

        DrawContext(vec4 color, float duration, int mask, float segment_size)
        {
            //Store old datas
            m_previous.m_color = m_global.m_color;
            m_previous.m_duration = m_global.m_duration;
            m_previous.m_mask = m_global.m_mask;
            m_previous.m_segment_size = m_global.m_segment_size;

            //Create new one
            SetColor(color);
            SetDuration(duration);
            SetMask(mask);
            SetSegmentSize(segment_size);
        }

    public:
        ~DrawContext()
        {
            m_global.m_color = m_previous.m_color;
            m_global.m_duration = m_previous.m_duration;
            m_global.m_mask = m_previous.m_mask;
            m_global.m_segment_size = m_previous.m_segment_size;
        }

        static DrawContext New(vec4 color, float duration = -1.f, int mask = 0xFFFFFFFF, float segment_size = 1.f)
        {
            return DrawContext(color, duration, mask, segment_size);
        }

        static DrawContext New(DrawContext& dc)
        {
            return DrawContext(dc.m_current.m_color, dc.m_current.m_duration, dc.m_current.m_mask, dc.m_current.m_segment_size);
        }

        static Data GetGlobalData() { return m_global; }

        //Setup methods
        void SetColor(vec4 color)               { m_global.m_color = m_current.m_color = color; }
        void SetDuration(float duration)        { m_global.m_duration = m_current.m_duration = duration; }
        void SetMask(int mask)                  { m_global.m_mask = m_current.m_mask = mask; }
        void SetSegmentSize(float segment_size) { m_global.m_segment_size = m_current.m_segment_size = segment_size; }
    };

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
    //Draw stuff in World
    //-- LINE
    void DrawLine(vec3 a, vec3 b, DrawContext::Data data);
    void DrawLine(vec2 a, vec3 b, DrawContext::Data data, float az = -1.f);
    void DrawLine(vec2 a, vec2 b, DrawContext::Data data, float az = -1.f, float bz = -1.f);
    void DrawLine(vec3 a, vec3 b);
    void DrawLine(vec2 a, vec3 b, float az = -1.f);
    void DrawLine(vec2 a, vec2 b, float az = -1.f, float bz = -1.f);
    //-- GIZMO
    void DrawGizmo(vec3 pos, vec3 x, vec3 y, vec3 z, float size);
    void DrawGizmo(vec2 pos, vec3 x, vec3 y, vec3 z, float size, float posz = -1.f);
    //-- GRID
    void DrawGrid(vec3 pos, vec3 x, vec3 y, vec3 z, float size, bool draw_3d=false);
    //-- ARROW
    void DrawArrow(vec3 a, vec3 b, vec2 s);
    void DrawArrow(vec2 a, vec3 b, vec2 s, float az=-1.f);
    void DrawArrow(vec2 a, vec2 b, vec2 s, float az=-1.f, float bz=-1.f);
    void DrawArrow(vec3 a, vec3 b, vec3 s);
    void DrawArrow(vec2 a, vec3 b, vec3 s, float az=-1.f);
    void DrawArrow(vec2 a, vec2 b, vec3 s, float az=-1.f, float bz=-1.f);
    void DrawArrow(vec3 a, vec3 b, vec3 s, DrawContext::Data data);
    void DrawArrow(vec2 a, vec3 b, vec3 s, DrawContext::Data data, float az=-1.f);
    void DrawArrow(vec2 a, vec2 b, vec3 s, DrawContext::Data data, float az=-1.f, float bz=-1.f);
    //-- BOX
    void DrawBox(box3 a);
    void DrawBox(box2 a);
    void DrawBox(box3 a, DrawContext::Data data);
    void DrawBox(box2 a, DrawContext::Data data);
    void DrawBox(vec3 a, vec3 b);
    void DrawBox(vec2 a, vec2 b);
    void DrawBox(vec2 a, float s);
    void DrawBox(vec3 a, vec3 b, DrawContext::Data data);
    void DrawBox(vec2 a, vec2 b, DrawContext::Data data);
    void DrawBox(vec2 a, float s, DrawContext::Data data);
    void DrawBox(box3 a, mat4 transform);
    void DrawBox(box2 a, mat2 transform);
    void DrawBox(box3 a, mat4 transform, DrawContext::Data data);
    void DrawBox(box2 a, mat2 transform, DrawContext::Data data);
    void DrawBox(vec3 a, vec3 b, mat4 transform);
    void DrawBox(vec2 a, vec2 b, mat2 transform);
    void DrawBox(vec2 a, float s, mat2 transform);
    void DrawBox(vec3 a, vec3 b, mat4 transform, DrawContext::Data data);
    void DrawBox(vec2 a, vec2 b, mat2 transform, DrawContext::Data data);
    void DrawBox(vec2 a, float s, mat2 transform, DrawContext::Data data);
    //-- CIRCLE
    void DrawCircle(vec2 a, float s);
    void DrawCircle(vec3 a, vec3 n);
    void DrawCircle(vec2 a, vec2 s);
    void DrawCircle(vec3 a, vec3 n, DrawContext::Data data);
    void DrawCircle(vec2 a, vec2 s, DrawContext::Data data);
    void DrawCircle(vec3 a, vec3 x, vec3 y);
    void DrawCircle(vec2 a, vec2 x, vec2 y);
    void DrawCircle(vec3 a, vec3 x, vec3 y, DrawContext::Data data);
    void DrawCircle(vec2 a, vec2 x, vec2 y, DrawContext::Data data);
    //-- SPHERE
    void DrawSphere(vec3 a, float s);
    void DrawSphere(vec3 a, float s, DrawContext::Data data);
    void DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z);
    void DrawSphere(vec3 a, vec3 x, vec3 y, vec3 z, DrawContext::Data data);
    //-- CAPSULE
    void DrawCapsule(vec3 a, float s, vec3 h);
    void DrawCapsule(vec3 a, float s, vec3 h, DrawContext::Data data);
    void DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h);
    void DrawCapsule(vec3 a, vec3 x, vec3 y, vec3 z, vec3 h, DrawContext::Data data);
    //-- VIEW PROJ
    void DrawViewProj(mat4 view_proj);
    void DrawViewProj(mat4 view_proj, DrawContext::Data data);
    void DrawViewProj(mat4 view, mat4 proj);
    void DrawViewProj(mat4 view, mat4 proj, DrawContext::Data data);

} /* namespace Debug */

} /* namespace lol */

