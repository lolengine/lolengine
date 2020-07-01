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

#include <lol/engine-internal.h>
#include <lol/msg>

#include <cstdlib>

#if defined _WIN32
#   define WIN32_LEAN_AND_MEAN 1
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#   undef near
#   undef far
#endif

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
#include "lolgl.h"
#endif

namespace lol
{

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
    BlendEquation m_blend_rgb, m_blend_alpha;
    BlendFunc m_blend_src, m_blend_dst;
    DepthFunc m_depth_func;
    DepthMask m_depth_mask;
    CullMode m_cull_mode;
    PolygonMode m_polygon_mode;
    ScissorMode m_scissor_mode;
    vec4 m_scissor_rect;
};

/*
 * Public Renderer class
 */

Renderer::Renderer(ivec2 size)
  : m_data(new RendererData())
{
#if defined LOL_USE_GLEW && !defined __APPLE__
    // Initialise GLEW if necessary
    // FIXME: move this somewhere else
    GLenum glerr = glewInit();
    if (glerr != GLEW_OK)
    {
        msg::error("cannot initialise GLEW: %s\n", glewGetErrorString(glerr));
        exit(EXIT_FAILURE);
    }
#endif

    gpu::setup_debug();

    /* Initialise rendering states */
    m_data->m_viewport = ibox2(0, 0, 0, 0);
    viewport(ibox2(ivec2::zero, size));

    m_data->m_clear_color = vec4(-1.f);
    clear_color(vec4(0.1f, 0.2f, 0.3f, 1.0f));

    m_data->m_clear_depth = -1.f;
    clear_depth(1.f);

    m_data->m_alpha_func = AlphaFunc::Never;
    m_data->m_alpha_value = -1.0f;
    SetAlphaFunc(AlphaFunc::Disabled, 0.0f);

    m_data->m_blend_rgb = BlendEquation::Subtract;
    m_data->m_blend_alpha = BlendEquation::Subtract;
    SetBlendEquation(BlendEquation::Add, BlendEquation::Add);

    m_data->m_blend_src = BlendFunc::Disabled;
    m_data->m_blend_dst = BlendFunc::Disabled;
    SetBlendFunc(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);

    m_data->m_depth_func = DepthFunc::Disabled;
    SetDepthFunc(DepthFunc::LessOrEqual);

    m_data->m_depth_mask = DepthMask::Disabled;
    SetDepthMask(DepthMask::Enabled);

    m_data->m_cull_mode = CullMode::Disabled;
    SetCullMode(CullMode::Clockwise);

    m_data->m_polygon_mode = PolygonMode::Point;
    SetPolygonMode(PolygonMode::Fill);

    m_data->m_scissor_mode = ScissorMode::Disabled;
    SetPolygonMode(PolygonMode::Fill);

    /* Add some rendering states that we don't export to the user */
#if defined HAVE_GL_2X && !defined GL_VERSION_3_1
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
}

Renderer::~Renderer()
{
}

/*
 * Buffer clearing
 */

void Renderer::clear(ClearMask mask)
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
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

void Renderer::viewport(ibox2 bounds)
{
    if (m_data->m_viewport == bounds)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glViewport(bounds.aa.x, bounds.aa.y, bounds.bb.x, bounds.bb.y);
#endif

    m_data->m_viewport = bounds;
}

ibox2 Renderer::viewport() const
{
    return m_data->m_viewport;
}

float Renderer::xy_ratio() const
{
    ivec2 s = viewport().extent();
    return (float)s.x / s.y;
}

float Renderer::yx_ratio() const
{
    ivec2 s = viewport().extent();
    return (float)s.y / s.x;
}

/*
 * Clear color
 */

void Renderer::clear_color(vec4 c)
{
    if (m_data->m_clear_color == c)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glClearColor(c.r, c.g, c.b, c.a);
#endif

    m_data->m_clear_color = c;
}

vec4 Renderer::clear_color() const
{
    return m_data->m_clear_color;
}

/*
 * Clear depth
 */

void Renderer::clear_depth(float depth)
{
    if (m_data->m_clear_depth == depth)
        return;

#if defined HAVE_GLES_2X
    glClearDepthf(depth);
#elif defined LOL_USE_GLEW || defined HAVE_GL_2X
    glClearDepth(depth);
#endif

    m_data->m_clear_depth = depth;
}

float Renderer::clear_depth() const
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

#if defined HAVE_GLES_2X
    /* not supported */
#elif defined GL_VERSION_3_1
    /* deprecated */
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
 * Blend equation
 */

void Renderer::SetBlendEquation(BlendEquation rgb, BlendEquation alpha)
{
    if (m_data->m_blend_rgb == rgb && m_data->m_blend_alpha == alpha)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    GLenum s1[2] = { GL_FUNC_ADD, GL_FUNC_ADD };
    BlendEquation s2[2] = { rgb, alpha };

    for (int i = 0; i < 2; ++i)
    {
        switch (s2[i])
        {
            case BlendEquation::Add:
                s1[i] = GL_FUNC_ADD; break;
            case BlendEquation::Subtract:
                s1[i] = GL_FUNC_SUBTRACT; break;
            case BlendEquation::ReverseSubtract:
                s1[i] = GL_FUNC_REVERSE_SUBTRACT; break;
#if defined GL_MIN && defined GL_MAX
            case BlendEquation::Min:
                s1[i] = GL_MIN; break;
            case BlendEquation::Max:
                s1[i] = GL_MAX; break;
#else
            case BlendEquation::Min:
                s1[i] = GL_MIN_EXT; break;
            case BlendEquation::Max:
                s1[i] = GL_MAX_EXT; break;
#endif
        }
    }

    glBlendEquationSeparate(s1[0], s1[1]);
#endif

    m_data->m_blend_rgb = rgb;
    m_data->m_blend_alpha = alpha;
}

BlendEquation Renderer::GetBlendEquationRgb() const
{
    return m_data->m_blend_rgb;
}

BlendEquation Renderer::GetBlendEquationAlpha() const
{
    return m_data->m_blend_alpha;
}

/*
 * Blend function
 */

void Renderer::SetBlendFunc(BlendFunc src, BlendFunc dst)
{
    if (m_data->m_blend_src == src && m_data->m_blend_dst == dst)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
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

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
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
 * Depth mask
 */

void Renderer::SetDepthMask(DepthMask mask)
{
    if (m_data->m_depth_mask == mask)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    if (mask == DepthMask::Disabled)
        glDepthMask(GL_FALSE);
    else
        glDepthMask(GL_TRUE);
#endif

    m_data->m_depth_mask = mask;
}

DepthMask Renderer::GetDepthMask() const
{
    return m_data->m_depth_mask;
}

/*
 * Face culling
 */

void Renderer::SetCullMode(CullMode mode)
{
    if (m_data->m_cull_mode == mode)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
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

#if defined HAVE_GLES_2X
    /* not supported */
#elif defined GL_VERSION_1_1
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

/*
* Scissor test mode
*/

void Renderer::SetScissorMode(ScissorMode mode)
{
    if (m_data->m_scissor_mode == mode)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    if (mode == ScissorMode::Enabled)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
#endif

    m_data->m_scissor_mode = mode;
}

void Renderer::SetScissorRect(vec4 rect)
{
    m_data->m_scissor_rect = rect;
    if (m_data->m_scissor_mode == ScissorMode::Enabled)
    {
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
        glScissor((int)rect.x, (int)(Video::GetSize().y - rect.w), (int)(rect.z - rect.x), (int)(rect.w - rect.y));
        //glScissor((int)rect.x, (int)rect.y, (int)(rect.z - rect.x), (int)(rect.w - rect.y));
#endif
    }
}

ScissorMode Renderer::GetScissorMode() const
{
    return m_data->m_scissor_mode;
}

vec4 Renderer::GetScissorRect() const
{
    return m_data->m_scissor_rect;
}

} // namespace lol
