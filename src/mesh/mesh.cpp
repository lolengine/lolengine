//
//  Lol Engine
//
//  Copyright: © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>
#include <cstdlib>

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

void Mesh::Render(Scene& scene, mat4 const &matrix)
{
    //if (scene.HasPrimitiveRenderer(this) < m_submeshes.count())
    {
        for (int i = 0; i < m_submeshes.count(); ++i)
            scene.AddPrimitiveRenderer(this, new PrimitiveMesh(m_submeshes[i], matrix));
    }
}

void Mesh::Render()
{
    for (int i = 0; i < m_submeshes.count(); ++i)
        m_submeshes[i]->Render();
}

void Mesh::SetMaterial(Shader *shader)
{
    for (int i = 0; i < m_submeshes.count(); ++i)
        m_submeshes[i]->SetShader(shader);
}

/*
 * SubMesh class
 */

SubMesh::SubMesh(Shader *shader, VertexDeclaration *vdecl)
  : m_mesh_prim(MeshPrimitive::Triangles),
    m_shader(shader),
    m_vdecl(vdecl)
{
    Ticker::Ref(m_shader);
}

SubMesh::~SubMesh()
{
    Ticker::Unref(m_shader);
    // TODO: cleanup
}

void SubMesh::SetMeshPrimitive(MeshPrimitive mesh_primitive)
{
    m_mesh_prim = mesh_primitive;
}

void SubMesh::SetShader(Shader *shader)
{
    Ticker::Unref(m_shader);
    m_shader = shader;
    Ticker::Ref(m_shader);
}

Shader *SubMesh::GetShader()
{
    return m_shader;
}

void SubMesh::SetVertexDeclaration(VertexDeclaration *vdecl)
{
    m_vdecl = vdecl;
}

void SubMesh::SetVertexBuffer(int index, VertexBuffer* vbo)
{
    while (index >= m_vbos.count())
        m_vbos.push(nullptr);

    m_vbos[index] = vbo;
}

void SubMesh::SetIndexBuffer(IndexBuffer* ibo)
{
    m_ibo = ibo;
}

void SubMesh::AddTexture(const char* name, Texture* texture)
{
    m_textures.push(String(name), texture);
}

void SubMesh::Render()
{
    int vertex_count = 0;

    for (int i = 0; i < m_vbos.count(); ++i)
    {
        ShaderAttrib attribs[12];

        if (m_vbos[i] == nullptr)
        {
            msg::error("trying to render a mesh with a null VBO\n");
            continue;
        }

        int usages[VertexUsage::MAX] = { 0 };
        VertexStreamBase stream = m_vdecl->GetStream(i);
        for (int j = 0; j < stream.GetStreamCount(); ++j)
        {
            VertexUsage usage = stream.GetUsage(j);
            int usage_index = usage.ToScalar();
            attribs[j] = m_shader->GetAttribLocation(usage, usages[usage_index]++);
        }

        vertex_count = m_vbos[i]->GetSize() / m_vdecl->GetStream(i).GetSize();

        m_vdecl->SetStream(m_vbos[i], attribs);
    }

    for (int i = 0; i < m_textures.count(); ++i)
    {
        // TODO: might be good to cache this
        ShaderUniform u_tex = m_shader->GetUniformLocation(m_textures[i].m1.C());
        m_shader->SetUniform(u_tex, m_textures[i].m2->GetTextureUniform(), i);
    }

    m_ibo->Bind();
    m_vdecl->Bind();
    m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, 0, 0, vertex_count,
                                 0, m_ibo->GetSize() / sizeof(uint16_t));
    m_vdecl->Unbind();
    m_ibo->Unbind();
}

} /* namespace lol */

