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

#include <cstdlib>

namespace lol
{

template<typename T> class tracked_var
{
public:
    inline tracked_var()
      : m_dirty(false)
    {}

    inline void set(T const &value)
    {
        m_value = value;
        m_dirty = true;
    }

    inline bool is_dirty()
    {
        return m_dirty;
    }

    inline T get()
    {
        return m_value;
    }

private:
    T m_value;
    bool m_dirty;
};

class RenderContextData
{
    friend class RenderContext;

private:
    tracked_var<ibox2> m_viewport;
    tracked_var<vec4> m_clear_color;
    tracked_var<float> m_clear_depth;
    tracked_var<AlphaFunc> m_alpha_func;
    tracked_var<float> m_alpha_value;
    tracked_var<BlendEquation> m_blend_rgb, m_blend_alpha;
    tracked_var<BlendFunc> m_blend_src, m_blend_dst;
    tracked_var<DepthFunc> m_depth_func;
    tracked_var<DepthMask> m_depth_mask;
    tracked_var<CullMode> m_cull_mode;
    tracked_var<PolygonMode> m_polygon_mode;
    tracked_var<ScissorMode> m_scissor_mode;
    tracked_var<vec4> m_scissor_rect;
};

/*
 * Public RenderContext class
 */

RenderContext::RenderContext(Renderer *renderer)
  : m_renderer(renderer),
    m_data(new RenderContextData())
{
}

RenderContext::~RenderContext()
{
    if (m_data->m_viewport.is_dirty())
        m_renderer->SetViewport(m_data->m_viewport.get());

    if (m_data->m_clear_color.is_dirty())
        m_renderer->SetClearColor(m_data->m_clear_color.get());

    if (m_data->m_clear_depth.is_dirty())
        m_renderer->SetClearDepth(m_data->m_clear_depth.get());

    if (m_data->m_alpha_func.is_dirty())
        m_renderer->SetAlphaFunc(m_data->m_alpha_func.get(),
                                 m_data->m_alpha_value.get());

    if (m_data->m_blend_src.is_dirty())
        m_renderer->SetBlendFunc(m_data->m_blend_src.get(),
                                 m_data->m_blend_dst.get());

    if (m_data->m_depth_func.is_dirty())
        m_renderer->SetDepthFunc(m_data->m_depth_func.get());

    if (m_data->m_depth_mask.is_dirty())
        m_renderer->SetDepthMask(m_data->m_depth_mask.get());

    if (m_data->m_cull_mode.is_dirty())
        m_renderer->SetCullMode(m_data->m_cull_mode.get());

    if (m_data->m_polygon_mode.is_dirty())
        m_renderer->SetPolygonMode(m_data->m_polygon_mode.get());

    if (m_data->m_scissor_mode.is_dirty())
        m_renderer->SetScissorMode(m_data->m_scissor_mode.get());

    delete m_data;
}

void RenderContext::SetViewport(ibox2 viewport)
{
    if (!m_data->m_viewport.is_dirty())
        m_data->m_viewport.set(m_renderer->GetViewport());

    m_renderer->SetViewport(viewport);
}

ibox2 RenderContext::GetViewport()
{
    return m_renderer->GetViewport();
}

void RenderContext::SetClearColor(vec4 color)
{
    if (!m_data->m_clear_color.is_dirty())
        m_data->m_clear_color.set(m_renderer->GetClearColor());

    m_renderer->SetClearColor(color);
}

vec4 RenderContext::GetClearColor()
{
    return m_renderer->GetClearColor();
}

void RenderContext::SetClearDepth(float depth)
{
    if (!m_data->m_clear_depth.is_dirty())
        m_data->m_clear_depth.set(m_renderer->GetClearDepth());

    m_renderer->SetClearDepth(depth);
}

float RenderContext::GetClearDepth()
{
    return m_renderer->GetClearDepth();
}

void RenderContext::SetAlphaFunc(AlphaFunc func, float alpha)
{
    if (!m_data->m_alpha_func.is_dirty())
        m_data->m_alpha_func.set(m_renderer->GetAlphaFunc());
    if (!m_data->m_alpha_value.is_dirty())
        m_data->m_alpha_value.set(m_renderer->GetAlphaValue());

    m_renderer->SetAlphaFunc(func, alpha);
}

AlphaFunc RenderContext::GetAlphaFunc()
{
    return m_renderer->GetAlphaFunc();
}

float RenderContext::GetAlphaValue()
{
    return m_renderer->GetAlphaValue();
}

void RenderContext::SetBlendEquation(BlendEquation rgb, BlendEquation alpha)
{
    if (!m_data->m_blend_rgb.is_dirty())
        m_data->m_blend_rgb.set(m_renderer->GetBlendEquationRgb());
    if (!m_data->m_blend_alpha.is_dirty())
        m_data->m_blend_alpha.set(m_renderer->GetBlendEquationAlpha());

    m_renderer->SetBlendEquation(rgb, alpha);
}

BlendEquation RenderContext::GetBlendEquationRgb()
{
    return m_renderer->GetBlendEquationRgb();
}

BlendEquation RenderContext::GetBlendEquationAlpha()
{
    return m_renderer->GetBlendEquationAlpha();
}

void RenderContext::SetBlendFunc(BlendFunc src, BlendFunc dst)
{
    if (!m_data->m_blend_src.is_dirty())
        m_data->m_blend_src.set(m_renderer->GetBlendFuncSrc());
    if (!m_data->m_blend_dst.is_dirty())
        m_data->m_blend_dst.set(m_renderer->GetBlendFuncDst());

    m_renderer->SetBlendFunc(src, dst);
}

BlendFunc RenderContext::GetBlendFuncSrc()
{
    return m_renderer->GetBlendFuncSrc();
}

BlendFunc RenderContext::GetBlendFuncDst()
{
    return m_renderer->GetBlendFuncDst();
}

void RenderContext::SetDepthFunc(DepthFunc func)
{
    if (!m_data->m_depth_func.is_dirty())
        m_data->m_depth_func.set(m_renderer->GetDepthFunc());

    m_renderer->SetDepthFunc(func);
}

DepthFunc RenderContext::GetDepthFunc()
{
    return m_renderer->GetDepthFunc();
}

void RenderContext::SetDepthMask(DepthMask mask)
{
    if (!m_data->m_depth_mask.is_dirty())
        m_data->m_depth_mask.set(m_renderer->GetDepthMask());

    m_renderer->SetDepthMask(mask);
}

DepthMask RenderContext::GetDepthMask()
{
    return m_renderer->GetDepthMask();
}

void RenderContext::SetCullMode(CullMode mode)
{
    if (!m_data->m_cull_mode.is_dirty())
        m_data->m_cull_mode.set(m_renderer->GetCullMode());

    m_renderer->SetCullMode(mode);
}

CullMode RenderContext::GetCullMode()
{
    return m_renderer->GetCullMode();
}

void RenderContext::SetPolygonMode(PolygonMode mode)
{
    if (!m_data->m_polygon_mode.is_dirty())
        m_data->m_polygon_mode.set(m_renderer->GetPolygonMode());

    m_renderer->SetPolygonMode(mode);
}

PolygonMode RenderContext::GetPolygonMode()
{
    return m_renderer->GetPolygonMode();
}

void RenderContext::SetScissorMode(ScissorMode mode)
{
    if (!m_data->m_scissor_mode.is_dirty())
        m_data->m_scissor_mode.set(m_renderer->GetScissorMode());

    m_renderer->SetScissorMode(mode);
}

void RenderContext::SetScissorRect(vec4 rect)
{
    if (!m_data->m_scissor_rect.is_dirty())
        m_data->m_scissor_rect.set(m_renderer->GetScissorRect());

    m_renderer->SetScissorRect(rect);
}

ScissorMode RenderContext::GetScissorMode()
{
    return m_renderer->GetScissorMode();
}

vec4 RenderContext::GetScissorRect()
{
    return m_renderer->GetScissorRect();
}

} /* namespace lol */

