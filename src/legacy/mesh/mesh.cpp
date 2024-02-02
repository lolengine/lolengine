//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lol/msg> // lol::msg
#include <memory>  // std::shared_ptr
#include <tuple>   // std::make_tuple
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
    //if (scene.HasPrimitiveRenderer(this) < m_submeshes.size())
    {
        for (auto submesh : m_submeshes)
            scene.AddPrimitiveRenderer(this, std::make_shared<PrimitiveMesh>(submesh, matrix));
    }
}

void Mesh::Render()
{
    for (auto submesh : m_submeshes)
        submesh->Render();
}

void Mesh::SetMaterial(std::shared_ptr<Shader> shader)
{
    for (auto submesh : m_submeshes)
        submesh->SetShader(shader);
}

/*
 * SubMesh class
 */

SubMesh::SubMesh(std::shared_ptr<Shader> shader, std::shared_ptr<VertexDeclaration> vdecl)
  : m_mesh_prim(MeshPrimitive::Triangles),
    m_shader(shader),
    m_vdecl(vdecl)
{
}

SubMesh::~SubMesh()
{
}

void SubMesh::SetMeshPrimitive(MeshPrimitive mesh_primitive)
{
    m_mesh_prim = mesh_primitive;
}

void SubMesh::SetShader(std::shared_ptr<Shader> shader)
{
    m_shader = shader;
}

std::shared_ptr<Shader> SubMesh::GetShader()
{
    return m_shader;
}

void SubMesh::SetVertexDeclaration(std::shared_ptr<VertexDeclaration> vdecl)
{
    m_vdecl = vdecl;
}

void SubMesh::SetVertexBuffer(int index, std::shared_ptr<VertexBuffer> vbo)
{
    while (index >= int(m_vbos.size()))
        m_vbos.push_back(nullptr);

    m_vbos[index] = vbo;
}

void SubMesh::SetIndexBuffer(std::shared_ptr<IndexBuffer> ibo)
{
    m_ibo = ibo;
}

void SubMesh::AddTexture(std::string const &name, std::shared_ptr<Texture> texture)
{
    m_textures.push_back(std::make_tuple(name, texture));
}

void SubMesh::Render()
{
    size_t vertex_count = 0;

    m_vdecl->Bind();

    for (size_t i = 0; i < m_vbos.size(); ++i)
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

        vertex_count += m_vbos[i]->size() / m_vdecl->GetStream(i).GetSize();

        m_vdecl->SetStream(m_vbos[i], attribs);
    }

    (void)vertex_count;

    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        // TODO: might be good to cache this
        ShaderUniform u_tex = m_shader->GetUniformLocation(std::get<0>(m_textures[i]));
        m_shader->SetUniform(u_tex, std::get<1>(m_textures[i])->GetTextureUniform(), i);
    }

    m_ibo->Bind();
    m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, (int)(m_ibo->size() / sizeof(uint16_t)));
    m_vdecl->Unbind();
    m_ibo->Unbind();
}

} /* namespace lol */

