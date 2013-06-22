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
    Scene *m_scene;

    TrackedState<vec4> m_clear_color;
    TrackedState<float> m_clear_depth;
    TrackedState<bool> m_alpha_blend;
    TrackedState<BlendFunc> m_blend_src;
    TrackedState<BlendFunc> m_blend_dst;
    TrackedState<bool> m_alpha_test;
    TrackedState<DepthFunc> m_depth_func;
    TrackedState<CullMode> m_face_culling;
};

/*
 * Public RenderContext class
 */

RenderContext::RenderContext()
  : m_data(new RenderContextData())
{
    m_data->m_scene = Scene::GetDefault();
}

RenderContext::~RenderContext()
{
    if (m_data->m_clear_color.HasChanged())
        g_renderer->SetClearColor(m_data->m_clear_color.GetValue());

    if (m_data->m_clear_depth.HasChanged())
        g_renderer->SetClearDepth(m_data->m_clear_depth.GetValue());

    if (m_data->m_alpha_blend.HasChanged())
        g_renderer->SetAlphaBlend(m_data->m_alpha_blend.GetValue());

    if (m_data->m_blend_src.HasChanged())
        g_renderer->SetBlendFunc(m_data->m_blend_src.GetValue(),
                                 m_data->m_blend_dst.GetValue());

    if (m_data->m_alpha_test.HasChanged())
        g_renderer->SetAlphaTest(m_data->m_alpha_test.GetValue());

    if (m_data->m_depth_func.HasChanged())
        g_renderer->SetDepthFunc(m_data->m_depth_func.GetValue());

    if (m_data->m_face_culling.HasChanged())
        g_renderer->SetFaceCulling(m_data->m_face_culling.GetValue());

    delete m_data;
}

void RenderContext::SetClearColor(vec4 color)
{
    if (!m_data->m_clear_color.HasChanged())
        m_data->m_clear_color.TrackValue(g_renderer->GetClearColor());

    g_renderer->SetClearColor(color);
}

void RenderContext::SetClearDepth(float depth)
{
    if (!m_data->m_clear_depth.HasChanged())
        m_data->m_clear_depth.TrackValue(g_renderer->GetClearDepth());

    g_renderer->SetClearDepth(depth);
}

void RenderContext::SetAlphaBlend(bool set)
{
    if (!m_data->m_alpha_blend.HasChanged())
        m_data->m_alpha_blend.TrackValue(g_renderer->GetAlphaBlend());

    g_renderer->SetAlphaBlend(set);
}

void RenderContext::SetBlendFunc(BlendFunc src, BlendFunc dst)
{
    if (!m_data->m_blend_src.HasChanged())
        m_data->m_blend_src.TrackValue(g_renderer->GetBlendFuncSrc());
    if (!m_data->m_blend_dst.HasChanged())
        m_data->m_blend_dst.TrackValue(g_renderer->GetBlendFuncDst());

    g_renderer->SetBlendFunc(src, dst);
}

void RenderContext::SetAlphaTest(bool set)
{
    if (!m_data->m_alpha_test.HasChanged())
        m_data->m_alpha_test.TrackValue(g_renderer->GetAlphaTest());

    g_renderer->SetAlphaTest(set);
}

void RenderContext::SetDepthFunc(DepthFunc func)
{
    if (!m_data->m_depth_func.HasChanged())
        m_data->m_depth_func.TrackValue(g_renderer->GetDepthFunc());

    g_renderer->SetDepthFunc(func);
}

void RenderContext::SetFaceCulling(CullMode mode)
{
    if (!m_data->m_face_culling.HasChanged())
        m_data->m_face_culling.TrackValue(g_renderer->GetFaceCulling());

    g_renderer->SetFaceCulling(mode);
}

} /* namespace lol */

