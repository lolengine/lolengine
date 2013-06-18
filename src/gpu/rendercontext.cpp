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

    TrackedState m_blend;
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
    if (m_data->m_blend.HasChanged())
        g_renderer->SetBlendState(m_data->m_blend.GetValue());

    delete m_data;
}

void RenderContext::SetBlendState(bool set)
{
    if (!m_data->m_blend.HasChanged())
        m_data->m_blend.TrackValue(g_renderer->GetBlendState());

    g_renderer->SetBlendState(set);
}

} /* namespace lol */

