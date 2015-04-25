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

#include <cstdlib>

namespace lol
{

template<typename T> class TrackedState
{
public:
    inline TrackedState()
      : m_changed(false)
    {}

    inline void TrackValue(T const &value)
    {
        m_value = value;
        m_changed = true;
    }

    inline bool HasChanged()
    {
        return m_changed;
    }

    inline T GetValue()
    {
        return m_value;
    }

private:
    T m_value;
    bool m_changed;
};

class RenderContextData
{
    friend class RenderContext;

private:
    TrackedState<ibox2> m_viewport;
    TrackedState<vec4> m_clear_color;
    TrackedState<float> m_clear_depth;
    TrackedState<AlphaFunc> m_alpha_func;
    TrackedState<float> m_alpha_value;
    TrackedState<BlendEquation> m_blend_rgb, m_blend_alpha;
    TrackedState<BlendFunc> m_blend_src, m_blend_dst;
    TrackedState<DepthFunc> m_depth_func;
    TrackedState<DepthMask> m_depth_mask;
    TrackedState<CullMode> m_cull_mode;
    TrackedState<PolygonMode> m_polygon_mode;
};

/*
 * Public RenderContext class
 */

RenderContext::RenderContext()
  : m_data(new RenderContextData())
{
}

RenderContext::~RenderContext()
{
    if (m_data->m_viewport.HasChanged())
        Renderer::Get()->SetViewport(m_data->m_viewport.GetValue());

    if (m_data->m_clear_color.HasChanged())
        Renderer::Get()->SetClearColor(m_data->m_clear_color.GetValue());

    if (m_data->m_clear_depth.HasChanged())
        Renderer::Get()->SetClearDepth(m_data->m_clear_depth.GetValue());

    if (m_data->m_alpha_func.HasChanged())
        Renderer::Get()->SetAlphaFunc(m_data->m_alpha_func.GetValue(),
                                 m_data->m_alpha_value.GetValue());

    if (m_data->m_blend_src.HasChanged())
        Renderer::Get()->SetBlendFunc(m_data->m_blend_src.GetValue(),
                                 m_data->m_blend_dst.GetValue());

    if (m_data->m_depth_func.HasChanged())
        Renderer::Get()->SetDepthFunc(m_data->m_depth_func.GetValue());

    if (m_data->m_depth_mask.HasChanged())
        Renderer::Get()->SetDepthMask(m_data->m_depth_mask.GetValue());

    if (m_data->m_cull_mode.HasChanged())
        Renderer::Get()->SetCullMode(m_data->m_cull_mode.GetValue());

    if (m_data->m_polygon_mode.HasChanged())
        Renderer::Get()->SetPolygonMode(m_data->m_polygon_mode.GetValue());

    delete m_data;
}

void RenderContext::SetViewport(ibox2 viewport)
{
    if (!m_data->m_viewport.HasChanged())
        m_data->m_viewport.TrackValue(Renderer::Get()->GetViewport());

    Renderer::Get()->SetViewport(viewport);
}

ibox2 RenderContext::GetViewport()
{
    return Renderer::Get()->GetViewport();
}

void RenderContext::SetClearColor(vec4 color)
{
    if (!m_data->m_clear_color.HasChanged())
        m_data->m_clear_color.TrackValue(Renderer::Get()->GetClearColor());

    Renderer::Get()->SetClearColor(color);
}

vec4 RenderContext::GetClearColor()
{
    return Renderer::Get()->GetClearColor();
}

void RenderContext::SetClearDepth(float depth)
{
    if (!m_data->m_clear_depth.HasChanged())
        m_data->m_clear_depth.TrackValue(Renderer::Get()->GetClearDepth());

    Renderer::Get()->SetClearDepth(depth);
}

float RenderContext::GetClearDepth()
{
    return Renderer::Get()->GetClearDepth();
}

void RenderContext::SetAlphaFunc(AlphaFunc func, float alpha)
{
    if (!m_data->m_alpha_func.HasChanged())
        m_data->m_alpha_func.TrackValue(Renderer::Get()->GetAlphaFunc());
    if (!m_data->m_alpha_value.HasChanged())
        m_data->m_alpha_value.TrackValue(Renderer::Get()->GetAlphaValue());

    Renderer::Get()->SetAlphaFunc(func, alpha);
}

AlphaFunc RenderContext::GetAlphaFunc()
{
    return Renderer::Get()->GetAlphaFunc();
}

float RenderContext::GetAlphaValue()
{
    return Renderer::Get()->GetAlphaValue();
}

void RenderContext::SetBlendEquation(BlendEquation rgb, BlendEquation alpha)
{
    if (!m_data->m_blend_rgb.HasChanged())
        m_data->m_blend_rgb.TrackValue(Renderer::Get()->GetBlendEquationRgb());
    if (!m_data->m_blend_alpha.HasChanged())
        m_data->m_blend_alpha.TrackValue(Renderer::Get()->GetBlendEquationAlpha());

    Renderer::Get()->SetBlendEquation(rgb, alpha);
}

BlendEquation RenderContext::GetBlendEquationRgb()
{
    return Renderer::Get()->GetBlendEquationRgb();
}

BlendEquation RenderContext::GetBlendEquationAlpha()
{
    return Renderer::Get()->GetBlendEquationAlpha();
}

void RenderContext::SetBlendFunc(BlendFunc src, BlendFunc dst)
{
    if (!m_data->m_blend_src.HasChanged())
        m_data->m_blend_src.TrackValue(Renderer::Get()->GetBlendFuncSrc());
    if (!m_data->m_blend_dst.HasChanged())
        m_data->m_blend_dst.TrackValue(Renderer::Get()->GetBlendFuncDst());

    Renderer::Get()->SetBlendFunc(src, dst);
}

BlendFunc RenderContext::GetBlendFuncSrc()
{
    return Renderer::Get()->GetBlendFuncSrc();
}

BlendFunc RenderContext::GetBlendFuncDst()
{
    return Renderer::Get()->GetBlendFuncDst();
}

void RenderContext::SetDepthFunc(DepthFunc func)
{
    if (!m_data->m_depth_func.HasChanged())
        m_data->m_depth_func.TrackValue(Renderer::Get()->GetDepthFunc());

    Renderer::Get()->SetDepthFunc(func);
}

DepthFunc RenderContext::GetDepthFunc()
{
    return Renderer::Get()->GetDepthFunc();
}

void RenderContext::SetDepthMask(DepthMask mask)
{
    if (!m_data->m_depth_mask.HasChanged())
        m_data->m_depth_mask.TrackValue(Renderer::Get()->GetDepthMask());

    Renderer::Get()->SetDepthMask(mask);
}

DepthMask RenderContext::GetDepthMask()
{
    return Renderer::Get()->GetDepthMask();
}

void RenderContext::SetCullMode(CullMode mode)
{
    if (!m_data->m_cull_mode.HasChanged())
        m_data->m_cull_mode.TrackValue(Renderer::Get()->GetCullMode());

    Renderer::Get()->SetCullMode(mode);
}

CullMode RenderContext::GetCullMode()
{
    return Renderer::Get()->GetCullMode();
}

void RenderContext::SetPolygonMode(PolygonMode mode)
{
    if (!m_data->m_polygon_mode.HasChanged())
        m_data->m_polygon_mode.TrackValue(Renderer::Get()->GetPolygonMode());

    Renderer::Get()->SetPolygonMode(mode);
}

PolygonMode RenderContext::GetPolygonMode()
{
    return Renderer::Get()->GetPolygonMode();
}

} /* namespace lol */

