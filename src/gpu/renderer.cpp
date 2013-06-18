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
    bool m_blend;
};

/*
 * Public Renderer class
 */

Renderer::Renderer()
  : m_data(new RendererData())
{
    m_data->m_blend = false;
}

Renderer::~Renderer()
{
    delete m_data;
}

void Renderer::SetBlendState(bool set)
{
    if (m_data->m_blend == set)
        return;

    m_data->m_blend = set;
    if (set)
    {

    }
    else
    {

    }
}

bool Renderer::GetBlendState() const
{
    return m_data->m_blend;
}

} /* namespace lol */

