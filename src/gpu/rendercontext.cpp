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

#include "core.h"

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
    TrackedState<BlendFunc> m_blend_src, m_blend_dst;
    TrackedState<DepthFunc> m_depth_func;
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
        g_renderer->SetViewport(m_data->m_viewport.GetValue());

    if (m_data->m_clear_color.HasChanged())
        g_renderer->SetClearColor(m_data->m_clear_color.GetValue());

    if (m_data->m_clear_depth.HasChanged())
        g_renderer->SetClearDepth(m_data->m_clear_depth.GetValue());

    if (m_data->m_alpha_func.HasChanged())
        g_renderer->SetAlphaFunc(m_data->m_alpha_func.GetValue(),
                                 m_data->m_alpha_value.GetValue());

    if (m_data->m_blend_src.HasChanged())
        g_renderer->SetBlendFunc(m_data->m_blend_src.GetValue(),
                                 m_data->m_blend_dst.GetValue());

    if (m_data->m_depth_func.HasChanged())
        g_renderer->SetDepthFunc(m_data->m_depth_func.GetValue());

    if (m_data->m_cull_mode.HasChanged())
        g_renderer->SetCullMode(m_data->m_cull_mode.GetValue());

    if (m_data->m_polygon_mode.HasChanged())
        g_renderer->SetPolygonMode(m_data->m_polygon_mode.GetValue());

    delete m_data;
}

void RenderContext::SetViewport(ibox2 viewport)
{
    if (!m_data->m_viewport.HasChanged())
        m_data->m_viewport.TrackValue(g_renderer->GetViewport());

    g_renderer->SetViewport(viewport);
}
ibox2 RenderContext::GetViewport()
{
    return g_renderer->GetViewport();
}

void RenderContext::SetClearColor(vec4 color)
{
    if (!m_data->m_clear_color.HasChanged())
        m_data->m_clear_color.TrackValue(g_renderer->GetClearColor());

    g_renderer->SetClearColor(color);
}

vec4 RenderContext::GetClearColor()
{
    return g_renderer->GetClearColor();
}

void RenderContext::SetClearDepth(float depth)
{
    if (!m_data->m_clear_depth.HasChanged())
        m_data->m_clear_depth.TrackValue(g_renderer->GetClearDepth());

    g_renderer->SetClearDepth(depth);
}

float RenderContext::GetClearDepth()
{
    return g_renderer->GetClearDepth();
}

void RenderContext::SetAlphaFunc(AlphaFunc func, float alpha)
{
    if (!m_data->m_alpha_func.HasChanged())
        m_data->m_alpha_func.TrackValue(g_renderer->GetAlphaFunc());
    if (!m_data->m_alpha_value.HasChanged())
        m_data->m_alpha_value.TrackValue(g_renderer->GetAlphaValue());

    g_renderer->SetAlphaFunc(func, alpha);
}

AlphaFunc RenderContext::GetAlphaFunc()
{
    return g_renderer->GetAlphaFunc();
}

float RenderContext::GetAlphaValue()
{
    return g_renderer->GetAlphaValue();
}

void RenderContext::SetBlendFunc(BlendFunc src, BlendFunc dst)
{
    if (!m_data->m_blend_src.HasChanged())
        m_data->m_blend_src.TrackValue(g_renderer->GetBlendFuncSrc());
    if (!m_data->m_blend_dst.HasChanged())
        m_data->m_blend_dst.TrackValue(g_renderer->GetBlendFuncDst());

    g_renderer->SetBlendFunc(src, dst);
}

BlendFunc RenderContext::GetBlendFuncSrc()
{
    return g_renderer->GetBlendFuncSrc();
}

BlendFunc RenderContext::GetBlendFuncDst()
{
    return g_renderer->GetBlendFuncDst();
}

void RenderContext::SetDepthFunc(DepthFunc func)
{
    if (!m_data->m_depth_func.HasChanged())
        m_data->m_depth_func.TrackValue(g_renderer->GetDepthFunc());

    g_renderer->SetDepthFunc(func);
}

DepthFunc RenderContext::GetDepthFunc()
{
    return g_renderer->GetDepthFunc();
}

void RenderContext::SetCullMode(CullMode mode)
{
    if (!m_data->m_cull_mode.HasChanged())
        m_data->m_cull_mode.TrackValue(g_renderer->GetCullMode());

    g_renderer->SetCullMode(mode);
}

CullMode RenderContext::GetCullMode()
{
    return g_renderer->GetCullMode();
}

void RenderContext::SetPolygonMode(PolygonMode mode)
{
    if (!m_data->m_polygon_mode.HasChanged())
        m_data->m_polygon_mode.TrackValue(g_renderer->GetPolygonMode());

    g_renderer->SetPolygonMode(mode);
}

PolygonMode RenderContext::GetPolygonMode()
{
    return g_renderer->GetPolygonMode();
}

} /* namespace lol */

