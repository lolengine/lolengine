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

/* FIXME: find a way to pass g_hwnd from the windowing system */
#if defined USE_D3D9
extern HWND g_hwnd;
#elif defined _XBOX
HWND g_hwnd = 0;
#endif

namespace lol
{

/*
 * The global g_renderer object, initialised by Video::Init
 */

Renderer *g_renderer = nullptr;

/*
 * Private RendererData class
 */

class RendererData
{
    friend class Renderer;

private:
    ibox2 m_viewport;
    vec4 m_clear_color;
    float m_clear_depth;
    AlphaFunc m_alpha_func;
    float m_alpha_value;
    BlendFunc m_blend_src, m_blend_dst;
    DepthFunc m_depth_func;
    CullMode m_cull_mode;
    PolygonMode m_polygon_mode;

private:
#if defined USE_D3D9
    IDirect3D9 *m_d3d_ctx;
    IDirect3DDevice9 *m_d3d_dev;
#elif defined _XBOX
    Direct3D *m_d3d_ctx;
    D3DDevice *m_d3d_dev;
#endif
};

/*
 * Public Renderer class
 */

Renderer::Renderer(ivec2 size)
  : m_data(new RendererData())
{
#if defined USE_D3D9 || defined _XBOX
    /* Create Direct3D context */
    m_data->m_d3d_ctx = Direct3DCreate9(D3D_SDK_VERSION);
    if (!m_data->m_d3d_ctx)
    {
        Log::Error("cannot initialise D3D\n");
        exit(EXIT_FAILURE);
    }

    /* Create Direct3D device */
#   if defined _XBOX
    XVIDEO_MODE VideoMode;
    XGetVideoMode(&VideoMode);
    size = lol::min(size, ivec2(VideoMode.dwDisplayWidth,
                                VideoMode.dwDisplayHeight));
#   endif
    D3DPRESENT_PARAMETERS d3dpp;
    memset(&d3dpp, 0, sizeof(d3dpp));
    d3dpp.BackBufferWidth = size.x;
    d3dpp.BackBufferHeight = size.y;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.hDeviceWindow = g_hwnd;
#   if defined USE_SDL
    d3dpp.Windowed = TRUE;
#   endif
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    HRESULT hr = m_data->m_d3d_ctx->CreateDevice(0, D3DDEVTYPE_HAL, g_hwnd,
                                                 D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                                 &d3dpp, &m_data->m_d3d_dev);
    if (FAILED(hr))
    {
        Log::Error("cannot create D3D device\n");
        exit(EXIT_FAILURE);
    }

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
    m_data->m_viewport = ibox2(0, 0, 0, 0);
    SetViewport(ibox2(vec2(0), size));

    m_data->m_clear_color = vec4(-1.f);
    SetClearColor(vec4(0.1f, 0.2f, 0.3f, 1.0f));

    m_data->m_clear_depth = -1.f;
    SetClearDepth(1.f);

    m_data->m_alpha_func = AlphaFunc::Never;
    m_data->m_alpha_value = -1.0f;
    SetAlphaFunc(AlphaFunc::Disabled, 0.0f);

    m_data->m_blend_src = BlendFunc::Disabled;
    m_data->m_blend_dst = BlendFunc::Disabled;
    SetBlendFunc(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);

    m_data->m_depth_func = DepthFunc::Disabled;
    SetDepthFunc(DepthFunc::LessOrEqual);

    m_data->m_cull_mode = CullMode::Disabled;
    SetCullMode(CullMode::Clockwise);

    m_data->m_polygon_mode = PolygonMode::Point;
    SetPolygonMode(PolygonMode::Fill);

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

void *Renderer::GetDevice()
{
#if defined USE_D3D9 || defined _XBOX
    return m_data->m_d3d_dev;
#else
    return nullptr;
#endif
}

/*
 * Buffer clearing
 */

void Renderer::Clear(ClearMask mask)
{
#if defined USE_D3D9 || defined _XBOX
    int m = 0;
    if (mask & ClearMask::Color)
        m |= D3DCLEAR_TARGET;
    if (mask & ClearMask::Depth)
        m |= D3DCLEAR_ZBUFFER;
    if (mask & ClearMask::Stencil)
        m |= D3DCLEAR_STENCIL;

    vec3 tmp = 255.999f * GetClearColor().rgb;
    D3DCOLOR clear_color = D3DCOLOR_XRGB((int)tmp.r, (int)tmp.g, (int)tmp.b);

    if (FAILED(m_data->m_d3d_dev->Clear(0, nullptr, m, clear_color,
                                        GetClearDepth(), 0)))
        Abort();
#else
    GLbitfield m = 0;
    if (mask & ClearMask::Color)
        m |= GL_COLOR_BUFFER_BIT;
    if (mask & ClearMask::Depth)
        m |= GL_DEPTH_BUFFER_BIT;
    if (mask & ClearMask::Stencil)
        m |= GL_STENCIL_BUFFER_BIT;
    glClear(m);
#endif
}

/*
 * Viewport dimensions
 */

void Renderer::SetViewport(ibox2 viewport)
{
    if (m_data->m_viewport == viewport)
        return;

#if defined USE_D3D9 || defined _XBOX
    D3DVIEWPORT9 vp = { viewport.A.x, viewport.A.y,
                        viewport.B.x, viewport.B.y,
                        0.0f, 1.0f };
    m_data->m_d3d_dev->SetViewport(&vp);
#else
    glViewport(viewport.A.x, viewport.A.y, viewport.B.x, viewport.B.y);
#endif

    m_data->m_viewport = viewport;
}

ibox2 Renderer::GetViewport() const
{
    return m_data->m_viewport;
}

/*
 * Clear color
 */

void Renderer::SetClearColor(vec4 color)
{
    if (m_data->m_clear_color == color)
        return;

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
    if (m_data->m_clear_depth == depth)
        return;

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
 * Alpha testing
 */

void Renderer::SetAlphaFunc(AlphaFunc func, float alpha)
{
    if (m_data->m_alpha_func == func && m_data->m_alpha_value == alpha)
        return;

#if defined USE_D3D9 || defined _XBOX
    switch (func)
    {
        case AlphaFunc::Disabled:
            break; /* Nothing to do */
        case AlphaFunc::Never:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NEVER);
            break;
        case AlphaFunc::Less:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);
            break;
        case AlphaFunc::Equal:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_EQUAL);
            break;
        case AlphaFunc::LessOrEqual:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESSEQUAL);
            break;
        case AlphaFunc::Greater:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
            break;
        case AlphaFunc::NotEqual:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL);
            break;
        case AlphaFunc::GreaterOrEqual:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
            break;
        case AlphaFunc::Always:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
            break;
    }

    if (func == AlphaFunc::Disabled)
    {
        m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    }
    else
    {
        m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHAREF,
                                          (DWORD)(alpha * 255.999f));
    }
#elif defined GL_VERSION_1_1
    switch (func)
    {
        case AlphaFunc::Disabled:
            break; /* Nothing to do */
        case AlphaFunc::Never:
            glAlphaFunc(GL_NEVER, alpha); break;
        case AlphaFunc::Less:
            glAlphaFunc(GL_LESS, alpha); break;
        case AlphaFunc::Equal:
            glAlphaFunc(GL_EQUAL, alpha); break;
        case AlphaFunc::LessOrEqual:
            glAlphaFunc(GL_LEQUAL, alpha); break;
        case AlphaFunc::Greater:
            glAlphaFunc(GL_GREATER, alpha); break;
        case AlphaFunc::NotEqual:
            glAlphaFunc(GL_NOTEQUAL, alpha); break;
        case AlphaFunc::GreaterOrEqual:
            glAlphaFunc(GL_GEQUAL, alpha); break;
        case AlphaFunc::Always:
            glAlphaFunc(GL_ALWAYS, alpha); break;
    }

    if (func == AlphaFunc::Disabled)
        glDisable(GL_ALPHA_TEST);
    else
        glEnable(GL_ALPHA_TEST);
#else
    /* XXX: alpha test not available in GL ES and deprecated anyway. */
#endif

    m_data->m_alpha_func = func;
    m_data->m_alpha_value = alpha;
}

AlphaFunc Renderer::GetAlphaFunc() const
{
    return m_data->m_alpha_func;
}

float Renderer::GetAlphaValue() const
{
    return m_data->m_alpha_value;
}

/*
 * Blend function
 */

void Renderer::SetBlendFunc(BlendFunc src, BlendFunc dst)
{
    if (m_data->m_blend_src == src && m_data->m_blend_dst == dst)
        return;

#if defined USE_D3D9 || defined _XBOX
    D3DBLEND s1[2] = { D3DBLEND_ONE, D3DBLEND_ZERO };
    BlendFunc s2[2] = { src, dst };

    for (int i = 0; i < 2; ++i)
    {
        switch (s2[i])
        {
            case BlendFunc::Disabled:
                break; /* Nothing to do */
            case BlendFunc::Zero:
                s1[i] = D3DBLEND_ZERO; break;
            case BlendFunc::One:
                s1[i] = D3DBLEND_ONE; break;
            case BlendFunc::SrcColor:
                s1[i] = D3DBLEND_SRCCOLOR; break;
            case BlendFunc::OneMinusSrcColor:
                s1[i] = D3DBLEND_INVSRCCOLOR; break;
            case BlendFunc::DstColor:
                s1[i] = D3DBLEND_DESTCOLOR; break;
            case BlendFunc::OneMinusDstColor:
                s1[i] = D3DBLEND_INVDESTCOLOR; break;
            case BlendFunc::SrcAlpha:
                s1[i] = D3DBLEND_SRCALPHA; break;
            case BlendFunc::OneMinusSrcAlpha:
                s1[i] = D3DBLEND_INVSRCALPHA; break;
            case BlendFunc::DstAlpha:
                s1[i] = D3DBLEND_DESTALPHA; break;
            case BlendFunc::OneMinusDstAlpha:
                s1[i] = D3DBLEND_INVDESTALPHA; break;
            /* FiXME: these can be supported using D3DPBLENDCAPS_BLENDFACTOR */
            case BlendFunc::ConstantColor:
                ASSERT(0, "BlendFunc::ConstantColor not supported");
                break;
            case BlendFunc::OneMinusConstantColor:
                ASSERT(0, "BlendFunc::OneMinusConstantColor not supported");
                break;
            case BlendFunc::ConstantAlpha:
                ASSERT(0, "BlendFunc::ConstantAlpha not supported");
                break;
            case BlendFunc::OneMinusConstantAlpha:
                ASSERT(0, "BlendFunc::OneMinusConstantAlpha not supported");
                break;
        }
    }

    if (src == BlendFunc::Disabled)
    {
        m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
    }
    else
    {
        m_data->m_d3d_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
        m_data->m_d3d_dev->SetRenderState(D3DRS_SRCBLEND, s1[0]);
        m_data->m_d3d_dev->SetRenderState(D3DRS_DESTBLEND, s1[1]);
    }
#else
    GLenum s1[2] = { GL_ONE, GL_ZERO };
    BlendFunc s2[2] = { src, dst };

    for (int i = 0; i < 2; ++i)
    {
        switch (s2[i])
        {
            case BlendFunc::Disabled:
                break; /* Nothing to do */
            case BlendFunc::Zero:
                s1[i] = GL_ZERO; break;
            case BlendFunc::One:
                s1[i] = GL_ONE; break;
            case BlendFunc::SrcColor:
                s1[i] = GL_SRC_COLOR; break;
            case BlendFunc::OneMinusSrcColor:
                s1[i] = GL_ONE_MINUS_SRC_COLOR; break;
            case BlendFunc::DstColor:
                s1[i] = GL_DST_COLOR; break;
            case BlendFunc::OneMinusDstColor:
                s1[i] = GL_ONE_MINUS_DST_COLOR; break;
            case BlendFunc::SrcAlpha:
                s1[i] = GL_SRC_ALPHA; break;
            case BlendFunc::OneMinusSrcAlpha:
                s1[i] = GL_ONE_MINUS_SRC_ALPHA; break;
            case BlendFunc::DstAlpha:
                s1[i] = GL_DST_ALPHA; break;
            case BlendFunc::OneMinusDstAlpha:
                s1[i] = GL_ONE_MINUS_DST_ALPHA; break;
            case BlendFunc::ConstantColor:
                s1[i] = GL_CONSTANT_COLOR; break;
            case BlendFunc::OneMinusConstantColor:
                s1[i] = GL_ONE_MINUS_CONSTANT_COLOR; break;
            case BlendFunc::ConstantAlpha:
                s1[i] = GL_CONSTANT_ALPHA; break;
            case BlendFunc::OneMinusConstantAlpha:
                s1[i] = GL_ONE_MINUS_CONSTANT_ALPHA; break;
        }
    }

    if (src == BlendFunc::Disabled)
    {
        glDisable(GL_BLEND);
    }
    else
    {
        glEnable(GL_BLEND);
        glBlendFunc(s1[0], s1[1]);
    }
#endif

    m_data->m_blend_src = src;
    m_data->m_blend_dst = dst;
}

BlendFunc Renderer::GetBlendFuncSrc() const
{
    return m_data->m_blend_src;
}

BlendFunc Renderer::GetBlendFuncDst() const
{
    return m_data->m_blend_dst;
}

/*
 * Depth test
 */

void Renderer::SetDepthFunc(DepthFunc func)
{
    if (m_data->m_depth_func == func)
        return;

#if defined USE_D3D9 || defined _XBOX
    switch (func)
    {
        case DepthFunc::Disabled:
            break; /* Nothing to do */
        case DepthFunc::Never:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
            break;
        case DepthFunc::Less:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
            break;
        case DepthFunc::Equal:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
            break;
        case DepthFunc::LessOrEqual:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
            break;
        case DepthFunc::Greater:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
            break;
        case DepthFunc::NotEqual:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_NOTEQUAL);
            break;
        case DepthFunc::GreaterOrEqual:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
            break;
        case DepthFunc::Always:
            m_data->m_d3d_dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
            break;
    }

    if (func == DepthFunc::Disabled)
        m_data->m_d3d_dev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    else
        m_data->m_d3d_dev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
#else
    switch (func)
    {
        case DepthFunc::Disabled:
            break; /* Nothing to do */
        case DepthFunc::Never:
            glDepthFunc(GL_NEVER); break;
        case DepthFunc::Less:
            glDepthFunc(GL_LESS); break;
        case DepthFunc::Equal:
            glDepthFunc(GL_EQUAL); break;
        case DepthFunc::LessOrEqual:
            glDepthFunc(GL_LEQUAL); break;
        case DepthFunc::Greater:
            glDepthFunc(GL_GREATER); break;
        case DepthFunc::NotEqual:
            glDepthFunc(GL_NOTEQUAL); break;
        case DepthFunc::GreaterOrEqual:
            glDepthFunc(GL_GEQUAL); break;
        case DepthFunc::Always:
            glDepthFunc(GL_ALWAYS); break;
    }

    if (func == DepthFunc::Disabled)
        glDisable(GL_DEPTH_TEST);
    else
        glEnable(GL_DEPTH_TEST);
#endif

    m_data->m_depth_func = func;
}

DepthFunc Renderer::GetDepthFunc() const
{
    return m_data->m_depth_func;
}

/*
 * Face culling
 */

void Renderer::SetCullMode(CullMode mode)
{
    if (m_data->m_cull_mode == mode)
        return;

#if defined USE_D3D9 || defined _XBOX
    switch (mode)
    {
    case CullMode::Disabled:
        m_data->m_d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        break;
    case CullMode::Clockwise:
        m_data->m_d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        break;
    case CullMode::CounterClockwise:
        m_data->m_d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        break;
    }
#else
    switch (mode)
    {
    case CullMode::Disabled:
        glDisable(GL_CULL_FACE);
        break;
    case CullMode::Clockwise:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);
        break;
    case CullMode::CounterClockwise:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);
        break;
    }
#endif

    m_data->m_cull_mode = mode;
}

CullMode Renderer::GetCullMode() const
{
    return m_data->m_cull_mode;
}

/*
 * Polygon rendering mode
 */

void Renderer::SetPolygonMode(PolygonMode mode)
{
    if (m_data->m_polygon_mode == mode)
        return;

#if defined USE_D3D9 || defined _XBOX
    switch (mode)
    {
    case PolygonMode::Point:
        m_data->m_d3d_dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
        break;
    case PolygonMode::Line:
        m_data->m_d3d_dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        break;
    case PolygonMode::Fill:
        m_data->m_d3d_dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        break;
    }
#elif defined __CELLOS_LV2__ || defined GL_VERSION_1_1
    switch (mode)
    {
    case PolygonMode::Point:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    case PolygonMode::Line:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case PolygonMode::Fill:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    }
#endif

    m_data->m_polygon_mode = mode;
}

PolygonMode Renderer::GetPolygonMode() const
{
    return m_data->m_polygon_mode;
}

} /* namespace lol */

