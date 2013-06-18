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
    bool m_blend, m_depth_test, m_face_culling;
};

/*
 * Public Renderer class
 */

Renderer::Renderer()
  : m_data(new RendererData())
{
    m_data->m_blend = false;
    SetAlphaBlend(true);

    m_data->m_depth_test = false;
    SetDepthTest(true);

    m_data->m_face_culling = false;
    SetFaceCulling(true);
}

Renderer::~Renderer()
{
    delete m_data;
}

/*
 * Alpha blending
 */

void Renderer::SetAlphaBlend(bool set)
{
    if (m_data->m_blend == set)
        return;

    m_data->m_blend = set;

#if defined USE_D3D9 || defined _XBOX
#   define STR0(x) #x
#   define STR(x) STR0(x)
#   pragma message(__FILE__ "(" STR(__LINE__) "): warning: Renderer::SetAlphaBlend() not implemented")
#else
    if (set)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
#endif
}

bool Renderer::GetAlphaBlend() const
{
    return m_data->m_blend;
}

/*
 * Depth test
 */

void Renderer::SetDepthTest(bool set)
{
    if (m_data->m_depth_test == set)
        return;

    m_data->m_depth_test = set;

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
}

bool Renderer::GetDepthTest() const
{
    return m_data->m_depth_test;
}

/*
 * Face culling
 */

void Renderer::SetFaceCulling(bool set)
{
    if (m_data->m_face_culling == set)
        return;

    m_data->m_face_culling = set;

#if defined USE_D3D9 || defined _XBOX
#   define STR0(x) #x
#   define STR(x) STR0(x)
#   pragma message(__FILE__ "(" STR(__LINE__) "): warning: Renderer::SetFaceCulling() not implemented")
#else
    if (set)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
#endif
}

bool Renderer::GetFaceCulling() const
{
    return m_data->m_face_culling;
}

} /* namespace lol */

