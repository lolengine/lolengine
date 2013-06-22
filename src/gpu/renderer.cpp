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

#include <cstdlib>

#if defined _XBOX
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#elif defined _WIN32
#   if defined USE_D3D9
#      include <d3d9.h>
#   endif
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

#include "core.h"
#include "lolgl.h"

#if defined USE_D3D9
extern IDirect3DDevice9 *g_d3ddevice;
#elif defined _XBOX
extern D3DDevice *g_d3ddevice;
#endif

namespace lol
{

/*
 * The global g_renderer object, initialised by Video::Init
 */
Renderer *g_renderer;

/*
 * Private RendererData class
 */
class RendererData
{
    friend class Renderer;

private:
    vec4 m_clear_color;
    float m_clear_depth;
    BlendFactor m_blend_src, m_blend_dst;
    CullMode m_face_culling;
    bool m_alpha_blend, m_alpha_test, m_depth_test;

#if defined USE_D3D9
    IDirect3DDevice9 *m_d3d_dev;
#elif defined _XBOX
    D3DDevice *m_d3d_dev;
#endif
};

/*
 * Public Renderer class
 */

Renderer::Renderer()
  : m_data(new RendererData())
{
#if defined USE_D3D9 || defined _XBOX
    /* FIXME: we should be in charge of creating this */
    m_data->m_d3d_dev = g_d3ddevice;
#else
#   if defined USE_GLEW && !defined __APPLE__
    /* Initialise GLEW if necessary */
    GLenum glerr = glewInit();
    if (glerr != GLEW_OK)
    {
        Log::Error("cannot initialise GLEW: %s\n", glewGetErrorString(glerr));
        exit(EXIT_FAILURE);
    }
#   endif
#endif

    /* Initialise rendering states */
    m_data->m_clear_color = vec4(-1.f);
    SetClearColor(vec4(0.1f, 0.2f, 0.3f, 1.0f));

    m_data->m_clear_depth = -1.f;
    SetClearDepth(1.f);

    m_data->m_alpha_blend = false;
    SetAlphaBlend(true);

    m_data->m_alpha_test = true;
    SetAlphaTest(false);

    m_data->m_blend_src = BlendFactor::Zero;
    m_data->m_blend_dst = BlendFactor::Zero;
    SetBlendFunc(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);

    m_data->m_depth_test = false;
    SetDepthTest(true);

    m_data->m_face_culling = CullMode::None;
    SetFaceCulling(CullMode::CCW);

    /* Add some rendering states that we don't export to the user */
#if defined USE_D3D9 || defined _XBOX
    /* TODO */
#else
#   if defined HAVE_GL_2X && !defined __APPLE__
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#   endif
#endif
}

Renderer::~Renderer()
{
    delete m_data;
}

/*
 * Clear color
 */

void Renderer::SetClearColor(vec4 color)
{
#if defined USE_D3D9 || defined _XBOX
    /* Nothing to do */
#else
    glClearColor(color.r, color.g, color.b, color.a);
#endif

    m_data->m_clear_color = color;
}

vec4 Renderer::GetClearColor() const
{
    return m_data->m_clear_color;
}

/*
 * Clear depth
 */

void Renderer::SetClearDepth(float depth)
{
#if defined USE_D3D9 || defined _XBOX
    /* Nothing to do */
#elif defined HAVE_GLES_2X
    glClearDepthf(depth);
#else
    glClearDepth(depth);
#endif

    m_data->m_clear_depth = depth;
}

float Renderer::GetClearDepth() const
{
    return m_data->m_clear_depth;
}

/*
 * Alpha blending
 */

void Renderer::SetAlphaBlend(bool set)
{
    if (m_data->m_alpha_blend == set)
        return;

#if defined USE_D3D9 || defined _XBOX
    m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, set ? 1 : 0);
#else
    if (set)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
#endif

    m_data->m_alpha_blend = set;
}

bool Renderer::GetAlphaBlend() const
{
    return m_data->m_alpha_blend;
}

/*
 * Blend function
 */

void Renderer::SetBlendFunc(BlendFactor src, BlendFactor dst)
{
    if (m_data->m_blend_src == src && m_data->m_blend_dst == dst)
        return;

#if defined USE_D3D9 || defined _XBOX
    enum D3DBLEND s1[2] = { D3DBLEND_ONE, D3DBLEND_ZERO };
    BlendFactor s2[2] = { src, dst };

    for (int i = 0; i < 2; ++i)
    {
        switch (s2[i])
        {
            case BlendFactor::Zero:
                s1[i] = D3DBLEND_ZERO; break;
            case BlendFactor::One:
                s1[i] = D3DBLEND_ONE; break;
            case BlendFactor::SrcColor:
                s1[i] = D3DBLEND_SRCCOLOR; break;
            case BlendFactor::OneMinusSrcColor:
                s1[i] = D3DBLEND_INVSRCCOLOR; break;
            case BlendFactor::DstColor:
                s1[i] = D3DBLEND_DESTCOLOR; break;
            case BlendFactor::OneMinusDstColor:
                s1[i] = D3DBLEND_INVDESTCOLOR; break;
            case BlendFactor::SrcAlpha:
                s1[i] = D3DBLEND_SRCALPHA; break;
            case BlendFactor::OneMinusSrcAlpha:
                s1[i] = D3DBLEND_INVSRCALPHA; break;
            case BlendFactor::DstAlpha:
                s1[i] = D3DBLEND_DESTALPHA; break;
            case BlendFactor::OneMinusDstAlpha:
                s1[i] = D3DBLEND_INVDESTALPHA; break;
            /* FiXME: these can be supported using D3DPBLENDCAPS_BLENDFACTOR */
            case BlendFactor::ConstantColor:
                assert(0, "BlendFactor::ConstantColor not supported");
                break;
            case BlendFactor::OneMinusConstantColor:
                assert(0, "BlendFactor::OneMinusConstantColor not supported");
                break;
            case BlendFactor::ConstantAlpha:
                assert(0, "BlendFactor::ConstantAlpha not supported");
                break;
            case BlendFactor::OneMinusConstantAlpha:
                assert(0, "BlendFactor::OneMinusConstantAlpha not supported");
                break;
        }
    }

    m_data->m_d3d_dev->SetRenderState(D3DRS_SRCBLEND, s1[0]);
    m_data->m_d3d_dev->SetRenderState(D3DRS_DESTBLEND, s1[1]);
#else
    GLenum s1[2] = { GL_ONE, GL_ZERO };
    BlendFactor s2[2] = { src, dst };

    for (int i = 0; i < 2; ++i)
    {
        switch (s2[i])
        {
            case BlendFactor::Zero:
                s1[i] = GL_ZERO; break;
            case BlendFactor::One:
                s1[i] = GL_ONE; break;
            case BlendFactor::SrcColor:
                s1[i] = GL_SRC_COLOR; break;
            case BlendFactor::OneMinusSrcColor:
                s1[i] = GL_ONE_MINUS_SRC_COLOR; break;
            case BlendFactor::DstColor:
                s1[i] = GL_DST_COLOR; break;
            case BlendFactor::OneMinusDstColor:
                s1[i] = GL_ONE_MINUS_DST_COLOR; break;
            case BlendFactor::SrcAlpha:
                s1[i] = GL_SRC_ALPHA; break;
            case BlendFactor::OneMinusSrcAlpha:
                s1[i] = GL_ONE_MINUS_SRC_ALPHA; break;
            case BlendFactor::DstAlpha:
                s1[i] = GL_DST_ALPHA; break;
            case BlendFactor::OneMinusDstAlpha:
                s1[i] = GL_ONE_MINUS_DST_ALPHA; break;
            case BlendFactor::ConstantColor:
                s1[i] = GL_CONSTANT_COLOR; break;
            case BlendFactor::OneMinusConstantColor:
                s1[i] = GL_ONE_MINUS_CONSTANT_COLOR; break;
            case BlendFactor::ConstantAlpha:
                s1[i] = GL_CONSTANT_ALPHA; break;
            case BlendFactor::OneMinusConstantAlpha:
                s1[i] = GL_ONE_MINUS_CONSTANT_ALPHA; break;
        }
    }

    glBlendFunc(s1[0], s1[1]);
#endif

    m_data->m_blend_src = src;
    m_data->m_blend_dst = dst;
}

BlendFactor Renderer::GetBlendFuncSrc() const
{
    return m_data->m_blend_src;
}

BlendFactor Renderer::GetBlendFuncDst() const
{
    return m_data->m_blend_dst;
}

/*
 * Alpha testing
 */

void Renderer::SetAlphaTest(bool set)
{
    if (m_data->m_alpha_test == set)
        return;

#if defined USE_D3D9 || defined _XBOX
    m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHATESTENABLE, set ? 1 : 0);
#else
    if (set)
        glEnable(GL_ALPHA_TEST);
    else
        glDisable(GL_ALPHA_TEST);
#endif

    m_data->m_alpha_test = set;
}

bool Renderer::GetAlphaTest() const
{
    return m_data->m_alpha_test;
}

/*
 * Depth test
 */

void Renderer::SetDepthTest(bool set)
{
    if (m_data->m_depth_test == set)
        return;

#if defined USE_D3D9 || defined _XBOX
#   define STR0(x) #x
#   define STR(x) STR0(x)
#   pragma message(__FILE__ "(" STR(__LINE__) "): warning: Renderer::SetDepthTest() not implemented")
#else
    if (set)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
#endif

    m_data->m_depth_test = set;
}

bool Renderer::GetDepthTest() const
{
    return m_data->m_depth_test;
}

/*
 * Face culling
 */

void Renderer::SetFaceCulling(CullMode mode)
{
    if (m_data->m_face_culling == mode)
        return;

#if defined USE_D3D9 || defined _XBOX
    switch (mode)
    {
    case CullMode::None:
        m_data->m_d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        break;
    case CullMode::CW:
        m_data->m_d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        break;
    case CullMode::CCW:
        m_data->m_d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        break;
    }
#else
    switch (mode)
    {
    case CullMode::None:
        glDisable(GL_CULL_FACE);
        break;
    case CullMode::CW:
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        break;
    case CullMode::CCW:
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        break;
    }
#endif

    m_data->m_face_culling = mode;
}

CullMode Renderer::GetFaceCulling() const
{
    return m_data->m_face_culling;
}

} /* namespace lol */

