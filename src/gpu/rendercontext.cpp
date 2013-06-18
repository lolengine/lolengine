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

class TrackedState
{
public:
    inline TrackedState()
      : m_state(Unchanged)
    {}

    inline void TrackValue(bool set)
    {
        m_state = set ? MustSet : MustUnset;
    }

    inline bool HasChanged()
    {
        return m_state != Unchanged;
    }

    inline bool GetValue()
    {
        return m_state == MustSet;
    }

private:
    enum
    {
        Unchanged,
        MustSet,
        MustUnset,
    }
    m_state;
};

class RenderContextData
{
    friend class RenderContext;

private:
    Scene *m_scene;

    TrackedState m_alpha_blend, m_depth_test, m_face_culling;
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
    if (m_data->m_alpha_blend.HasChanged())
        g_renderer->SetAlphaBlend(m_data->m_alpha_blend.GetValue());

    if (m_data->m_depth_test.HasChanged())
        g_renderer->SetDepthTest(m_data->m_depth_test.GetValue());

    if (m_data->m_face_culling.HasChanged())
        g_renderer->SetFaceCulling(m_data->m_face_culling.GetValue());

    delete m_data;
}

void RenderContext::SetAlphaBlend(bool set)
{
    if (!m_data->m_alpha_blend.HasChanged())
        m_data->m_alpha_blend.TrackValue(g_renderer->GetAlphaBlend());

    g_renderer->SetAlphaBlend(set);
}

void RenderContext::SetDepthTest(bool set)
{
    if (!m_data->m_depth_test.HasChanged())
        m_data->m_depth_test.TrackValue(g_renderer->GetDepthTest());

    g_renderer->SetDepthTest(set);
}

void RenderContext::SetFaceCulling(bool set)
{
    if (!m_data->m_face_culling.HasChanged())
        m_data->m_face_culling.TrackValue(g_renderer->GetFaceCulling());

    g_renderer->SetFaceCulling(set);
}

} /* namespace lol */

