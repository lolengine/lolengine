//
// EasyMesh: A class about generating 3D mesh without using the hands
//           Mesh can be generated using C++ or lua code
//
// Copyright: (c) 2010-2015 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2015 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

LOLFX_RESOURCE_DECLARE(shiny);

namespace lol
{

//-----------------------------------------------------------------------------
EasyMesh::EasyMesh()
  : m_build_data(nullptr)
{
    m_cursors.Push(0, 0);
    m_state = MeshRender::NeedData;
}

//-----------------------------------------------------------------------------
EasyMesh::EasyMesh(const EasyMesh& em)
{
    m_indices = em.m_indices;
    m_vert = em.m_vert;
    m_cursors = em.m_cursors;
    m_build_data = nullptr;
    if (em.m_build_data)
        m_build_data = new EasyMeshBuildData(*em.m_build_data);
    if (m_indices.Count() && m_vert.Count() && m_cursors.Count())
        m_state = MeshRender::NeedConvert;
    else
        m_state = MeshRender::NeedData;
}

//-----------------------------------------------------------------------------
bool EasyMesh::SetRender(bool should_render)
{
    if (m_state == MeshRender::CanRender)
    {
        if (!should_render)
            m_state = MeshRender::IgnoreRender;
        return true;
    }
    else if (m_state == MeshRender::IgnoreRender)
    {
        if (should_render)
            m_state = MeshRender::CanRender;
        return true;
    }
    return false;
}

} /* namespace lol */
