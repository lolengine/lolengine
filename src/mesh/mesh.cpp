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
    while (index >= m_vbos.Count())
        m_vbos.Push(nullptr);

    m_vbos[index] = vbo;
}

void SubMesh::AddTexture(const char* name, Texture* texture)
{
    m_textures.Push(String(name), texture);
}

void SubMesh::Render(Shader* shader)
{
    int vertex_count = 0;

    for (int i = 0; i < m_vbos.Count(); ++i)
    {
        ShaderAttrib attribs[12];

        if (m_vbos[i] == nullptr)
        {
            Log::Error("trying to render a mesh with a null vbo");
            return;
        }

        int indices[VertexUsage::MAX];
        memset(indices, 0, sizeof(int) * VertexUsage::MAX);

        VertexStreamBase stream = m_vdecl->GetStream(i);
        for (int j = 0; j < stream.GetStreamCount(); ++j)
        {
            VertexUsage usage = stream.GetUsage(j);
            int usage_index = usage.ToScalar();
            attribs[j] = shader->GetAttribLocation(usage, indices[usage_index]++);
        }

        vertex_count = m_vbos[i]->GetSize() / m_vdecl->GetStream(i).GetSize();

        m_vdecl->SetStream(m_vbos[i], attribs);
    }

    for (int i = 0; i < m_textures.Count(); ++i)
    {
        // TODO: might be good to cache this
        ShaderUniform u_tex = shader->GetUniformLocation(m_textures[i].m1.C());
        shader->SetUniform(u_tex, m_textures[i].m2->GetTextureUniform(), i);
    }

    m_vdecl->Bind();
    m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, vertex_count);
    m_vdecl->Unbind();
}

} /* namespace lol */

