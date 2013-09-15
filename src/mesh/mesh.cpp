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

#include <cstring>
#include <cstdlib>

#include "core.h"

namespace lol
{

/*
 * Mesh class
 */

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::Render(mat4 const &model)
{
    //for (int i = 0; i < m_submeshes.Count(); ++i)
    //    m_submeshes[i]->Render(model);
}

/*
 * SubMesh class
 */

SubMesh::SubMesh(lol::VertexDeclaration* vdecl)
    : m_mesh_prim(MeshPrimitive::Triangles)
{
    m_vdecl = vdecl;
    m_vbos = new VertexBuffer*[vdecl->GetStreamCount()];
    memset(m_vbos, 0, sizeof(VertexBuffer*) * vdecl->GetStreamCount());
    m_vertex_count = -1;
}

SubMesh::~SubMesh()
{
    // TODO: cleanup
}

void SubMesh::SetMeshPrimitive(MeshPrimitive mesh_primitive)
{
    m_mesh_prim = mesh_primitive;
}

void SubMesh::SetVertexBuffer(int index, VertexBuffer* vbo)
{
    m_vbos[index] = vbo;
    if (m_vertex_count < 0)
        m_vertex_count = vbo->GetSize() / m_vdecl->GetStream(index).GetSize();
}

void SubMesh::AddTexture(const char* name, Texture* texture)
{
    m_textures.Push(String(name), texture);
}

void SubMesh::Render(Shader* shader)
{
    ShaderAttrib attribs[12];

    int c = 0;
    for (int i = 0; i < m_vdecl->GetStreamCount(); ++i)
    {
        ShaderAttrib attribs[12];

        if (m_vbos[i] == nullptr)
        {
            Log::Error("trying to render a mesh with a null vbo");
            return;
        }

        int indices[VertexUsage::Max];
        memset(indices, 0, sizeof(int) * VertexUsage::Max);

        VertexStreamBase stream = m_vdecl->GetStream(i);
        for (int j = 0; j < stream.GetStreamCount(); ++j)
        {
            VertexUsage usage = stream.GetUsage(j);
            attribs[j] = shader->GetAttribLocation(usage, indices[usage]++);
        }

        m_vdecl->SetStream(m_vbos[i], attribs);
    }

    for (int i = 0; i < m_textures.Count(); ++i)
    {
        // TODO: might be good to cache this
        ShaderUniform uniform = shader->GetUniformLocation(m_textures[i].m1.C());
        shader->SetUniform(uniform, m_textures[i].m2->GetTexture(), i);
    }

    m_vdecl->Bind();
    m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, m_vertex_count);
    m_vdecl->Unbind();
}

} /* namespace lol */

