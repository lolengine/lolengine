//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The EasyMesh class
// ------------------
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "easymesh/easymesh-compiler.h"

LOLFX_RESOURCE_DECLARE(shiny);
LOLFX_RESOURCE_DECLARE(shinydebugwireframe);
LOLFX_RESOURCE_DECLARE(shinydebuglighting);
LOLFX_RESOURCE_DECLARE(shinydebugnormal);
LOLFX_RESOURCE_DECLARE(shinydebugUV);
LOLFX_RESOURCE_DECLARE(shiny_SK);

namespace lol
{


//-----------------------------------------------------------------------------
GpuShaderData::GpuShaderData()
{
    m_render_mode = DebugRenderMode::Default;
}

//-----------------------------------------------------------------------------
GpuShaderData::GpuShaderData(uint16_t vert_decl_flags, Shader* shader, DebugRenderMode render_mode)
{
    m_render_mode = render_mode;
    m_shader = shader;
    m_vert_decl_flags = vert_decl_flags;
}

//-----------------------------------------------------------------------------
GpuShaderData::~GpuShaderData()
{
    m_shader_uniform.Empty();
    m_shader_attrib.Empty();
}

//-----------------------------------------------------------------------------
void GpuShaderData::AddUniform(const lol::String &new_uniform)
{
    m_shader_uniform.Push(new_uniform, m_shader->GetUniformLocation(new_uniform.C()));
}

//-----------------------------------------------------------------------------
void GpuShaderData::AddAttribute(VertexUsage usage, int index)
{
    m_shader_attrib.Push(m_shader->GetAttribLocation(usage, index));
}

//-----------------------------------------------------------------------------
ShaderUniform const *GpuShaderData::GetUniform(const lol::String &uniform)
{
    for (int i = 0; i < m_shader_uniform.Count(); ++i)
        if (m_shader_uniform[i].m1 == uniform)
            return &m_shader_uniform[i].m2;
    return nullptr;
}

//-----------------------------------------------------------------------------
ShaderAttrib const *GpuShaderData::GetAttribute(VertexUsage usage, int index)
{
    for (int i = 0; i < m_shader_attrib.Count(); ++i)
        if (m_shader_attrib[i].GetUsage() == usage && m_shader_attrib[i].GetIndex() == index)
            return &m_shader_attrib[i];
    return nullptr;
}

//-----------------------------------------------------------------------------
DefaultShaderData::DefaultShaderData(DebugRenderMode render_mode)
{
    bool with_UV = false;
    m_render_mode = render_mode;
    m_vert_decl_flags = (1 << VertexUsage::Position) |
                        (1 << VertexUsage::Normal)   |
                        (1 << VertexUsage::Color);

    if (render_mode == DebugRenderMode::Default)
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(shiny));
    else if (render_mode == DebugRenderMode::Wireframe)
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(shinydebugwireframe));
    else if (render_mode == DebugRenderMode::Lighting)
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(shinydebuglighting));
    else if (render_mode == DebugRenderMode::Normal)
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(shinydebugnormal));
    else if (render_mode == DebugRenderMode::UV)
    {
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(shinydebugUV));
        m_vert_decl_flags |= (1 << VertexUsage::TexCoord);
        with_UV = true;
    }
    SetupDefaultData(with_UV);
}

//-----------------------------------------------------------------------------
DefaultShaderData::DefaultShaderData(uint16_t vert_decl_flags, Shader* shader, bool with_UV)
    : GpuShaderData(vert_decl_flags, shader, DebugRenderMode::Default)
{
    SetupDefaultData(with_UV);
}

//-----------------------------------------------------------------------------
void DefaultShaderData::SetupDefaultData(bool with_UV)
{
    AddUniform("in_ModelView");
    AddUniform("in_Inv_ModelView");
    AddUniform("in_View");
    AddUniform("in_Inv_View");
    AddUniform("in_Proj");
    AddUniform("in_NormalMat");
    AddUniform("in_Damage");
    AddUniform("u_Lights");
}

//-----------------------------------------------------------------------------
void DefaultShaderData::SetupShaderDatas(mat4 const &model)
{
    mat4 proj = g_scene->GetCamera()->GetProjection();
    mat4 view = g_scene->GetCamera()->GetView();
    mat4 modelview = view * model;
    mat3 normalmat = transpose(inverse(mat3(modelview)));

    /* FIXME: this should be hidden in the shader */
    /* FIXME: the 4th component of the position can be used for other things */
    /* FIXME: GetUniform("blabla") is costly */
    Array<Light *> const lights = g_scene->GetLights();
    Array<vec4> light_data;
    for (int i = 0; i < lights.Count(); ++i)
        light_data << lights[i]->GetPosition() << lights[i]->GetColor();
    while (light_data.Count() < 8)
        light_data << vec4(0.f) << vec4(0.f);
    m_shader->SetUniform(*GetUniform("u_Lights"), light_data);

    m_shader->SetUniform(*GetUniform("in_ModelView"), modelview);
    m_shader->SetUniform(*GetUniform("in_Inv_ModelView"), inverse(modelview));
    m_shader->SetUniform(*GetUniform("in_View"), view);
    m_shader->SetUniform(*GetUniform("in_Inv_View"), inverse(view));
    m_shader->SetUniform(*GetUniform("in_Proj"), proj);
    m_shader->SetUniform(*GetUniform("in_NormalMat"), normalmat);
    m_shader->SetUniform(*GetUniform("in_Damage"), 0);
}

//-----------------------------------------------------------------------------
GpuEasyMeshData::GpuEasyMeshData()
{
    m_vertexcount = 0;
    m_indexcount = 0;
    m_ibo = nullptr;
}

//-----------------------------------------------------------------------------
GpuEasyMeshData::~GpuEasyMeshData()
{
    m_gpudatas.Empty();
    m_vdatas.Empty();
    if (m_ibo)
        delete(m_ibo);
}

#define BUILD_VFLAG(bool_value, flag_value, check_flag) \
    bool bool_value = (check_flag & (1 << flag_value)) != 0; \
    check_flag &= ~(1 << flag_value);
#define BUILD_VFLAG_OR(bool_value, flag_value, check_flag) \
    bool_value = (bool_value || (check_flag & (1 << flag_value)) != 0); \
    check_flag &= ~(1 << flag_value);
#define BUILD_VFLAG_COUNT(bool_value, flag_value, check_flag, count_value) \
    BUILD_VFLAG(bool_value, flag_value, check_flag) \
    count_value += (int)bool_value;

//-----------------------------------------------------------------------------
void GpuEasyMeshData::AddGpuData(GpuShaderData* gpudata, EasyMesh* src_mesh)
{
    uint16_t vflags = gpudata->m_vert_decl_flags;

    BUILD_VFLAG(has_position,    VertexUsage::Position,     vflags);
    BUILD_VFLAG(has_normal,      VertexUsage::Normal,       vflags);
    BUILD_VFLAG(has_color,       VertexUsage::Color,        vflags);
    BUILD_VFLAG(has_texcoord,    VertexUsage::TexCoord,     vflags);
    BUILD_VFLAG_OR(has_texcoord, VertexUsage::TexCoordExt,  vflags);
    ASSERT(!vflags, "Vertex Usage setup is not implemented for %s, feel free to do so.",
           VertexUsage::GetNameList(vflags).C());

    if (has_position)   gpudata->AddAttribute(VertexUsage::Position, 0);
    if (has_normal)     gpudata->AddAttribute(VertexUsage::Normal, 0);
    if (has_color)      gpudata->AddAttribute(VertexUsage::Color, 0);
    if (has_texcoord)   gpudata->AddAttribute(VertexUsage::TexCoord, 0);

    SetupVertexData(gpudata->m_vert_decl_flags, src_mesh);

    if (!m_ibo)
    {
        Array<uint16_t> indexlist;
        for (int i = 0; i < src_mesh->m_indices.Count(); i += 3)
        {
            indexlist << src_mesh->m_indices[i + 0];
            indexlist << src_mesh->m_indices[i + 1];
            indexlist << src_mesh->m_indices[i + 2];
        }

        m_ibo = new IndexBuffer(indexlist.Bytes());
        void *indices = m_ibo->Lock(0, 0);
        memcpy(indices, &indexlist[0], indexlist.Bytes());
        m_ibo->Unlock();

        m_indexcount = indexlist.Count();
    }

    if (m_gpudatas.Count() != DebugRenderMode::Max)
    {
        m_gpudatas.Reserve(DebugRenderMode::Max);
        for (int i = 0; i < DebugRenderMode::Max; i++)
            m_gpudatas << nullptr;
    }
    m_gpudatas[gpudata->m_render_mode] = gpudata;
}

//-----------------------------------------------------------------------------
void GpuEasyMeshData::SetupVertexData(uint16_t vflags, EasyMesh* src_mesh)
{
    for (int i = 0; i < m_vdatas.Count(); ++i)
        if (m_vdatas[i].m1 == vflags)
            return;

    VertexDeclaration* new_vdecl = nullptr;
    VertexBuffer* new_vbo = nullptr;
    void *vbo_data = nullptr;
    int vbo_bytes = 0;

#define COPY_VBO \
    new_vbo = new VertexBuffer(vbo_bytes); \
    void *mesh = new_vbo->Lock(0, 0); \
    memcpy(mesh, vbo_data, vbo_bytes); \
    new_vbo->Unlock();

    //Keep a count of the flags
    uint16_t saveflags = vflags;
    int flagnb = 0;
    BUILD_VFLAG_COUNT(has_position,   VertexUsage::Position,    saveflags, flagnb);
    BUILD_VFLAG_COUNT(has_normal,     VertexUsage::Normal,      saveflags, flagnb);
    BUILD_VFLAG_COUNT(has_color,      VertexUsage::Color,       saveflags, flagnb);
    BUILD_VFLAG_COUNT(has_texcoord,   VertexUsage::TexCoord,    saveflags, flagnb);
    BUILD_VFLAG_COUNT(has_texcoordExt,VertexUsage::TexCoordExt, saveflags, flagnb);
    ASSERT(!saveflags, "Vertex Declaration setup is not implemented for %s, feel free to do so.",
           VertexUsage::GetNameList(vflags).C());

    if (has_position && has_normal && has_color && has_texcoord && has_texcoordExt && flagnb == 5)
    {
        new_vdecl = new VertexDeclaration(
                         VertexStream<vec3,vec3,u8vec4,vec4>(
                          VertexUsage::Position,
                          VertexUsage::Normal,
                          VertexUsage::Color,
                          VertexUsage::TexCoord));

        Array<vec3, vec3, u8vec4, vec4> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord,
                            src_mesh->m_vert[i].m_normal,
                            (u8vec4)(src_mesh->m_vert[i].m_color * 255.f),
                            src_mesh->m_vert[i].m_texcoord);

        vbo_data = &vertexlist[0];
        vbo_bytes = vertexlist.Bytes();
        m_vertexcount = vertexlist.Count();

        COPY_VBO;
    }
    else if (has_position && has_texcoord && has_texcoordExt && flagnb == 3)
    {
        new_vdecl = new VertexDeclaration(VertexStream<vec3,vec4>(VertexUsage::Position, VertexUsage::TexCoord));

        Array<vec3, vec4> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord, src_mesh->m_vert[i].m_texcoord);

        vbo_data = &vertexlist[0];
        vbo_bytes = vertexlist.Bytes();
        m_vertexcount = vertexlist.Count();

        COPY_VBO;
    }
    else if (has_position && has_texcoord && flagnb == 2)
    {
        new_vdecl = new VertexDeclaration(VertexStream<vec3,vec2>(VertexUsage::Position, VertexUsage::TexCoord));

        Array<vec3, vec2> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord, src_mesh->m_vert[i].m_texcoord.xy);

        vbo_data = &vertexlist[0];
        vbo_bytes = vertexlist.Bytes();
        m_vertexcount = vertexlist.Count();

        COPY_VBO;
    }
    else if (has_position && has_normal && has_color && has_texcoord && flagnb == 4)
    {
        new_vdecl = new VertexDeclaration(
                         VertexStream<vec3,vec3,u8vec4,vec2>(
                          VertexUsage::Position,
                          VertexUsage::Normal,
                          VertexUsage::Color,
                          VertexUsage::TexCoord));

        Array<vec3, vec3, u8vec4, vec2> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord,
                            src_mesh->m_vert[i].m_normal,
                            (u8vec4)(src_mesh->m_vert[i].m_color * 255.f),
                            src_mesh->m_vert[i].m_texcoord.xy);

        vbo_data = &vertexlist[0];
        vbo_bytes = vertexlist.Bytes();
        m_vertexcount = vertexlist.Count();

        COPY_VBO;
    }
    else if (has_position && has_normal && has_color && flagnb == 3)
    {
        new_vdecl = new VertexDeclaration(
                         VertexStream<vec3,vec3,u8vec4>(
                          VertexUsage::Position,
                          VertexUsage::Normal,
                          VertexUsage::Color));

        Array<vec3,vec3,u8vec4> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord,
                            src_mesh->m_vert[i].m_normal,
                            (u8vec4)(src_mesh->m_vert[i].m_color * 255.f));

        vbo_data = &vertexlist[0];
        vbo_bytes = vertexlist.Bytes();
        m_vertexcount = vertexlist.Count();

        COPY_VBO;
    }
    else if (has_position && has_color && has_texcoord && has_texcoordExt && flagnb == 4)
    {
        new_vdecl = new VertexDeclaration(VertexStream<vec3,vec4,vec4>(VertexUsage::Position, VertexUsage::Color, VertexUsage::TexCoord));

        Array<vec3, vec4, vec4> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord, src_mesh->m_vert[i].m_color, src_mesh->m_vert[i].m_texcoord);

        vbo_data = &vertexlist[0];
        vbo_bytes = vertexlist.Bytes();
        m_vertexcount = vertexlist.Count();

        COPY_VBO;
    }
    else
        ASSERT(!saveflags, "Vertex Declaration combination is not implemented for %s, feel free to do so.",
           VertexUsage::GetNameList(vflags).C());

    m_vdatas.Push(vflags, new_vdecl, new_vbo);
}

//-----------------------------------------------------------------------------
void GpuEasyMeshData::RenderMeshData(mat4 const &model)
{
    DebugRenderMode d = Video::GetDebugRenderMode();
    GpuShaderData& gpu_sd = *(m_gpudatas[d]);

    int vdecl_idx = 0;
    for (; vdecl_idx < m_vdatas.Count(); ++vdecl_idx)
        if (m_vdatas[vdecl_idx].m1 == gpu_sd.m_vert_decl_flags)
            break;

    if (vdecl_idx >= m_vdatas.Count())
        return;

    uint16_t vflags = m_vdatas[vdecl_idx].m1;
    VertexDeclaration* vdecl = m_vdatas[vdecl_idx].m2;
    VertexBuffer* vbo = m_vdatas[vdecl_idx].m3;

    gpu_sd.m_shader->Bind();
    gpu_sd.SetupShaderDatas(model);

    vdecl->Bind();

    BUILD_VFLAG(has_position,   VertexUsage::Position,    vflags);
    BUILD_VFLAG(has_normal,     VertexUsage::Normal,      vflags);
    BUILD_VFLAG(has_color,      VertexUsage::Color,       vflags);
    BUILD_VFLAG(has_texcoord,   VertexUsage::TexCoord,    vflags);
    BUILD_VFLAG_OR(has_texcoord,VertexUsage::TexCoordExt, vflags);
    ASSERT(!vflags, "Vertex Stream setup is not implemented for %s, feel free to do so.",
           VertexUsage::GetNameList(vflags).C());

    int idx = 0;
    ShaderAttrib Attribs[4] = { lol::ShaderAttrib(), lol::ShaderAttrib(), lol::ShaderAttrib(), lol::ShaderAttrib() };

    if (has_position)   Attribs[idx++] = *gpu_sd.GetAttribute(VertexUsage::Position, 0);
    if (has_normal)     Attribs[idx++] = *gpu_sd.GetAttribute(VertexUsage::Normal, 0);
    if (has_color)      Attribs[idx++] = *gpu_sd.GetAttribute(VertexUsage::Color, 0);
    if (has_texcoord)   Attribs[idx++] = *gpu_sd.GetAttribute(VertexUsage::TexCoord, 0);

    vdecl->SetStream(vbo, Attribs[0], Attribs[1], Attribs[2], Attribs[3]);

    m_ibo->Bind();
    vdecl->DrawIndexedElements(MeshPrimitive::Triangles, 0, 0, m_vertexcount, 0, m_indexcount);
    m_ibo->Unbind();
    vdecl->Unbind();
}

//-----------------------------------------------------------------------------
EasyMesh::EasyMesh()
  : m_build_data(nullptr)
{
    m_cursors.Push(0, 0);
}

//-----------------------------------------------------------------------------
bool EasyMesh::Compile(char const *command)
{
    EasyMeshCompiler mc(*this);
    return mc.ParseString(command);
}

//-----------------------------------------------------------------------------
void EasyMesh::OpenBrace()
{
    m_cursors.Push(m_vert.Count(), m_indices.Count());
}

//-----------------------------------------------------------------------------
void EasyMesh::CloseBrace()
{
    m_cursors.Pop();
}
//-----------------------------------------------------------------------------
void EasyMesh::MeshConvert(GpuShaderData* new_gpu_sdata)
{
    delete(m_build_data);
    m_build_data = nullptr;

    if (new_gpu_sdata)
    {
        m_gpu_data.AddGpuData(new_gpu_sdata, this);
        for (int i = DebugRenderMode::Default + 1; i < DebugRenderMode::Max; i++)
            m_gpu_data.AddGpuData(new DefaultShaderData(DebugRenderMode(i)), this);
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::MeshConvert(Shader* provided_shader)
{
    if (provided_shader)
    {
        GpuShaderData *new_gpu_sdata = new DefaultShaderData(((1 << VertexUsage::Position) |
                                                              (1 << VertexUsage::Normal) |
                                                              (1 << VertexUsage::Color)),
                                                              provided_shader,
                                                              false);
        m_gpu_data.AddGpuData(new_gpu_sdata, this);
    }
    else
        m_gpu_data.AddGpuData(new DefaultShaderData(DebugRenderMode::Default), this);
    for (int i = DebugRenderMode::Default + 1; i < DebugRenderMode::Max; i++)
        m_gpu_data.AddGpuData(new DefaultShaderData(DebugRenderMode(i)), this);
}

//-----------------------------------------------------------------------------
void EasyMesh::Render(mat4 const &model)
{
    m_gpu_data.RenderMeshData(model);
}

//-------------------
// "Collisions" functions
//-------------------
#define VX_ALONE -2
#define VX_MASTER -1

//-----------------------------------------------------------------------------
//helpers func to retrieve a vertex.
int VertexDictionnary::FindVertexMaster(const int search_idx)
{
    //Resolve current vertex idx in the dictionnary (if exist)
    for (int j = 0; j < vertex_list.Count(); j++)
        if (vertex_list[j].m1 == search_idx)
            return vertex_list[j].m3;
    return VDictType::DoesNotExist;
}

//-----------------------------------------------------------------------------
//retrieve a list of matching vertices, doesn't include search_idx.
bool VertexDictionnary::FindMatchingVertices(const int search_idx, Array<int> &matching_ids)
{
    int cur_mast = FindVertexMaster(search_idx);

    if (cur_mast == VDictType::DoesNotExist || cur_mast == VDictType::Alone)
        return false;

    if (cur_mast == VDictType::Master)
        cur_mast = search_idx;
    else
        matching_ids << vertex_list[cur_mast].m1;

    for (int j = 0; j < vertex_list.Count(); j++)
        if (vertex_list[j].m3 == cur_mast && vertex_list[j].m1 != search_idx)
            matching_ids << vertex_list[j].m1;

    return (matching_ids.Count() > 0);
}

//-----------------------------------------------------------------------------
//Will return connected vertices (through triangles), if returned vertex has matching ones, it only returns the master.
bool VertexDictionnary::FindConnectedVertices(const int search_idx, const Array<uint16_t> &tri_list, const int tri0, Array<int> &connected_vert, Array<int> const *ignored_tri)
{
    Array<int> connected_tri;
    FindConnectedTriangles(search_idx, tri_list, tri0, connected_tri, ignored_tri);

    for (int i = 0; i < connected_tri.Count(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int v_indice = tri_list[connected_tri[i] + j];
            if (v_indice != search_idx)
            {
                int found_master = FindVertexMaster(tri_list[connected_tri[i] + j]);
                if (found_master == VDictType::Alone || found_master == VDictType::Master)
                    found_master = v_indice;
                if (found_master != search_idx)
                {
                    bool already_exist = false;
                    for (int k = 0; !already_exist && k < connected_vert.Count(); k++)
                        if (connected_vert[k] == found_master)
                            already_exist = true;
                    if (!already_exist)
                        connected_vert << found_master;
                }
            }
        }
    }
    return (connected_vert.Count() > 0);
}
//-----------------------------------------------------------------------------
bool VertexDictionnary::FindConnectedTriangles(const int search_idx, const Array<uint16_t> &tri_list, const int tri0, Array<int> &connected_tri, Array<int> const *ignored_tri)
{
    return FindConnectedTriangles(ivec3(search_idx, search_idx, search_idx), tri_list, tri0, connected_tri, ignored_tri);
}
//-----------------------------------------------------------------------------
bool VertexDictionnary::FindConnectedTriangles(const ivec2 &search_idx, const Array<uint16_t> &tri_list, const int tri0, Array<int> &connected_tri, Array<int> const *ignored_tri)
{
    return FindConnectedTriangles(ivec3(search_idx, search_idx.x), tri_list, tri0, connected_tri, ignored_tri);
}
//-----------------------------------------------------------------------------
bool VertexDictionnary::FindConnectedTriangles(const ivec3 &search_idx, const Array<uint16_t> &tri_list, const int tri0, Array<int> &connected_tri, Array<int> const *ignored_tri)
{
    int needed_validation = 0;
    Array<int> vert_list[3];
    for (int i = 0; i < 3; i++)
    {
        //Small optim since above func will use this one
        if ((i == 1 && search_idx[0] == search_idx[1]) ||
            (i == 2 && (search_idx[0] == search_idx[2] || search_idx[1] == search_idx[2])))
            continue;
        else
        {
            //increment the validation info, hence empty list aren't taken into account.
            needed_validation++;
            vert_list[i] << search_idx[i];
            FindMatchingVertices(search_idx[i], vert_list[i]);
        }
    }

    for (int i = tri0; i < tri_list.Count(); i += 3)
    {
        if (ignored_tri)
        {
            bool should_pass = false;
            for (int j = 0; !should_pass && j < ignored_tri->Count(); j++)
                if ((*ignored_tri)[j] == i)
                    should_pass = true;
            if (should_pass)
                continue;
        }
        int found_validation = 0;
        for (int j = 0; j < 3; j++)
        {
            bool validated = false;
            for (int k = 0; !validated && k < vert_list[j].Count(); k++)
                for (int l = 0; !validated && l < 3; l++)
                    if (vert_list[j][k] == tri_list[i + l])
                        validated = true;
            found_validation += (validated)?(1):(0);
        }
        //triangle is validated store it
        if (found_validation == needed_validation)
            connected_tri << i;
    }

    return (connected_tri.Count() > 0);
}

//-----------------------------------------------------------------------------
//Will update the given list with all the vertices on the same spot.
void VertexDictionnary::AddVertex(const int vert_id, const vec3 vert_coord)
{
    for (int j = 0; j < vertex_list.Count(); j++)
        if (vertex_list[j].m1 == vert_id)
            return;

    //First, build the vertex Dictionnary
    int i = 0;
    for (; i < master_list.Count(); i++)
    {
        int cur_mast  = master_list[i];
        int cur_id    = vertex_list[cur_mast].m1;
        vec3 cur_loc  = vertex_list[cur_mast].m2;
        int &cur_type = vertex_list[cur_mast].m3;

        if (cur_id == vert_id)
            return;

        if (sqlength(cur_loc - vert_coord) < CSG_EPSILON)
        {
            if (cur_type == VDictType::Alone)
                cur_type = VDictType::Master;
            vertex_list.Push(vert_id, vert_coord, cur_mast);
            return;
        }
    }

    //We're here because we couldn't find any matching vertex
    master_list.Push(vertex_list.Count());
    vertex_list.Push(vert_id, vert_coord, VDictType::Alone);
}

//-----------------------------------------------------------------------------
void EasyMesh::MeshCsg(CSGUsage csg_operation)
{
    //A vertex dictionnary for vertices on the same spot.
    Array< int, int > vertex_dict;
    //This list keeps track of the triangle that will need deletion at the end.
    Array< int > triangle_to_kill;
    //Listing for each triangle of the vectors intersecting it. <tri_Id, <Point0, Point1, tri_isec_Normal>>
    Array< int, Array< vec3, vec3, vec3 > > triangle_isec;
    //keep a track of the intersection point on the triangle. <pos, side_id>
    Array< vec3, int > triangle_vertex;
    for (int k = 0; k < 10; k++)
        triangle_vertex.Push(vec3(.0f), 0);

    //bsp infos
    CsgBsp mesh_bsp_0;
    CsgBsp mesh_bsp_1;

    if (m_cursors.Count() == 0)
        return;

    //BSP BUILD : We use the brace logic, csg should be used as : "[ exp .... [exp .... csg]]"
    int cursor_start = (m_cursors.Count() < 2)?(0):(m_cursors[(m_cursors.Count() - 2)].m2);
    for (int mesh_id = 0; mesh_id < 2; mesh_id++)
    {
        int start_point     = (mesh_id == 0)?(cursor_start):(m_cursors.Last().m2);
        int end_point       = (mesh_id == 0)?(m_cursors.Last().m2):(m_indices.Count());
        CsgBsp &mesh_bsp  = (mesh_id == 0)?(mesh_bsp_0):(mesh_bsp_1);
        for (int i = start_point; i < end_point; i += 3)
            mesh_bsp.AddTriangleToTree(i, m_vert[m_indices[i]].m_coord,
                                          m_vert[m_indices[i + 1]].m_coord,
                                          m_vert[m_indices[i + 2]].m_coord);
    }

    //BSP Usage : let's crunch all triangles on the correct BSP
    int indices_count = m_indices.Count();
    for (int mesh_id = 0; mesh_id < 2; mesh_id++)
    {
        int start_point     = (mesh_id == 0)?(cursor_start):(m_cursors.Last().m2);
        int end_point       = (mesh_id == 0)?(m_cursors.Last().m2):(indices_count);
        CsgBsp &mesh_bsp  = (mesh_id == 0)?(mesh_bsp_1):(mesh_bsp_0);
        Array< vec3, int, int, float > vert_list;
        Array< int, int, int, int > tri_list;
        vec3 n0(.0f); vec3 n1(.0f);
        vec4 c0(.0f); vec4 c1(.0f);

        //Reserve some memory
        vert_list.Reserve(3);
        tri_list.Reserve(3);

        for (int i = start_point; i < end_point; i += 3)
        {
            int Result = mesh_bsp.TestTriangleToTree(m_vert[m_indices[i]].m_coord,
                                                     m_vert[m_indices[i + 1]].m_coord,
                                                     m_vert[m_indices[i + 2]].m_coord, vert_list, tri_list);
            int tri_base_idx = m_indices.Count();

            //one split has been done, we need to had the new vertices & the new triangles.
            if (Result == 1)
            {
                triangle_to_kill.Push(i);
#if 1
                int base_idx = m_vert.Count();
                for (int k = 3; k < vert_list.Count(); k++)
                {
                    int P0 = (vert_list[k].m2 < 3)?(m_indices[i + vert_list[k].m2]):(base_idx + vert_list[k].m2 - 3);
                    int P1 = (vert_list[k].m3 < 3)?(m_indices[i + vert_list[k].m3]):(base_idx + vert_list[k].m3 - 3);

                    AddVertex(vert_list[k].m1);

                    //Normal : bad calculations there.
                    n0 = m_vert[P0].m_normal;
                    n1 = m_vert[P1].m_normal;
                    SetCurVertNormal(normalize(n0 + (n1 - n0) * vert_list[k].m4));

#if 1
                    //Color
                    c0 = m_vert[P0].m_color;
                    c1 = m_vert[P1].m_color;
                    vec4 res = c0 + ((c1 - c0) * vert_list[k].m4);
                    SetCurVertColor(res);
#else
                    if (mesh_id == 0)
                        SetCurVertColor(vec4(1.0f, .0f, .0f, 1.0f));
                    else
                        SetCurVertColor(vec4(.0f, 1.0f, 1.0f, 1.0f));
#endif
                }
                for (int k = 0; k < tri_list.Count(); k++)
                {
                    int P0 = (tri_list[k].m2 < 3)?(m_indices[i + tri_list[k].m2]):(base_idx + (tri_list[k].m2 - 3));
                    int P1 = (tri_list[k].m3 < 3)?(m_indices[i + tri_list[k].m3]):(base_idx + (tri_list[k].m3 - 3));
                    int P2 = (tri_list[k].m4 < 3)?(m_indices[i + tri_list[k].m4]):(base_idx + (tri_list[k].m4 - 3));
                    AppendTriangle(P0, P1, P2, 0);
                }
#endif
            }
#if 1
            //Main case
            if (Result >= 0)
            {
                for (int k = 0; k < tri_list.Count(); k++)
                {
                    int tri_idx = ((tri_list.Count() == 1)?(i):(tri_base_idx + k * 3));

                    //Triangle Kill Test
                    if (//csgu : CSGUnion() -> m0_Outside + m1_Outside
                        (csg_operation == CSGUsage::Union && tri_list[k].m1 == LEAF_BACK) ||
                        //csgs : CSGSubstract() -> m0_Outside + m1_Inside-inverted
                        (csg_operation == CSGUsage::Substract &&
                            ((mesh_id == 0 && tri_list[k].m1 == LEAF_BACK) ||
                            (mesh_id == 1 && tri_list[k].m1 == LEAF_FRONT))) ||
                        //csgs : CSGSubstractLoss() -> m0_Outside
                        (csg_operation == CSGUsage::SubstractLoss &&
                            ((mesh_id == 0 && tri_list[k].m1 == LEAF_BACK) || mesh_id == 1)) ||
                        //csga : CSGAnd() -> m0_Inside + m1_Inside
                        (csg_operation == CSGUsage::And && tri_list[k].m1 == LEAF_FRONT))
                    {
                        triangle_to_kill.Push(tri_idx);
                    }

                    //Triangle Invert Test
                    if (//csgs : CSGSubstract() -> m0_Outside + m1_Inside-inverted
                        (csg_operation == CSGUsage::Substract && mesh_id == 1 && tri_list[k].m1 == LEAF_BACK) ||
                        //csgx : CSGXor() -> m0_Outside/m0_Inside-inverted + m1_Outside/m1_Inside-inverted
                        (csg_operation == CSGUsage::Xor && tri_list[k].m1 == LEAF_BACK))
                    {
                        //a Xor means we will share vertices with the outside, so duplicate the vertices.
                        //TODO : This operation disconnect all triangle, in some cases, not a good thing.
                        if (csg_operation == CSGUsage::Xor)
                        {
                            for (int l = 0; l < 3; l++)
                            {
                                AddDuplicateVertex(m_indices[tri_idx + l]);
                                m_indices[tri_idx + l] = m_vert.Count() - 1;
                            }
                        }
                        m_indices[tri_idx + 1] += m_indices[tri_idx + 2];
                        m_indices[tri_idx + 2]  = m_indices[tri_idx + 1] - m_indices[tri_idx + 2];
                        m_indices[tri_idx + 1]  = m_indices[tri_idx + 1] - m_indices[tri_idx + 2];
                        ComputeNormals(tri_idx, 3);
                    }
                }
            }
#endif
            vert_list.Empty();
            tri_list.Empty();
        }
    }

    for (int i = 0; i < m_vert.Count(); i++)
        if (length(m_vert[i].m_normal) < 1.0f)
            i = i;

    int dir = 1;
    for (int i = 0; i >= 0 && i < triangle_to_kill.Count() - 1; i += dir)
    {
        if (triangle_to_kill[i] < triangle_to_kill[i + 1] && dir < 0)
            dir = 1;
        if (triangle_to_kill[i] == triangle_to_kill[i + 1])
        {
            triangle_to_kill.Remove(i);
            dir = -1;
        }
        if (triangle_to_kill[i] > triangle_to_kill[i + 1])
        {
            triangle_to_kill[i]     += triangle_to_kill[i + 1];
            triangle_to_kill[i + 1]  = triangle_to_kill[i] - triangle_to_kill[i + 1];
            triangle_to_kill[i]      = triangle_to_kill[i] - triangle_to_kill[i + 1];
            dir = -1;
        }
        if (i == 0 && dir == -1)
            dir = 1;
    }
    for (int i = triangle_to_kill.Count() - 1; i >= 0; i--)
        m_indices.Remove(triangle_to_kill[i], 3);

    m_cursors.Last().m1 = m_vert.Count();
    m_cursors.Last().m2 = m_indices.Count();

    //DONE for the splitting !
}

//-----------------------------------------------------------------------------
void EasyMesh::ToggleScaleWinding()
{
    BD()->Toggle(MeshBuildOperation::Scale_Winding);
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColor(vec4 const &color)
{
    BD()->Color() = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColor2(vec4 const &color)
{
    BD()->Color2() = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::AddVertex(vec3 const &coord)
{
    m_vert.Push(VertexData(coord, vec3(0.f, 1.f, 0.f), BD()->Color()));
}

//-----------------------------------------------------------------------------
void EasyMesh::AddDuplicateVertex(int i)
{
    m_vert << m_vert[i];
}

//-----------------------------------------------------------------------------
void EasyMesh::AddLerpVertex(int i, int j, float alpha)
{
    m_vert.Push(VertexData(
        lol::lerp(m_vert[i].m_coord,    m_vert[j].m_coord,      alpha),
        lol::lerp(m_vert[i].m_normal,   m_vert[j].m_normal,     alpha),
        lol::lerp(m_vert[i].m_color,    m_vert[j].m_color,      alpha),
        lol::lerp(m_vert[i].m_texcoord, m_vert[j].m_texcoord,   alpha),
        ((alpha < .5f) ? (m_vert[i].m_bone_id) : (m_vert[j].m_bone_id)), /* FIXME ? */
        lol::lerp(m_vert[i].m_bone_weight, m_vert[j].m_bone_weight, alpha)));
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendQuad(int i1, int i2, int i3, int i4, int base)
{
    m_indices << base + i1;
    m_indices << base + i2;
    m_indices << base + i3;

    m_indices << base + i4;
    m_indices << base + i1;
    m_indices << base + i3;
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendQuadDuplicateVerts(int i1, int i2, int i3, int i4, int base)
{
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i1);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i2);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i3);

    m_indices << m_vert.Count(); AddDuplicateVertex(base + i4);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i1);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i3);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendTriangle(int i1, int i2, int i3, int base)
{
    m_indices << base + i1;
    m_indices << base + i2;
    m_indices << base + i3;
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendTriangleDuplicateVerts(int i1, int i2, int i3, int base)
{
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i1);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i2);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i3);
}

//-----------------------------------------------------------------------------
void EasyMesh::ComputeNormals(int start, int vcount)
{
    for (int i = 0; i < vcount; i += 3)
    {
        vec3 v0 = m_vert[m_indices[start + i + 2]].m_coord
                - m_vert[m_indices[start + i + 0]].m_coord;
        vec3 v1 = m_vert[m_indices[start + i + 1]].m_coord
                - m_vert[m_indices[start + i + 0]].m_coord;
        vec3 n = normalize(cross(v1, v0));

        for (int j = 0; j < 3; j++)
            m_vert[m_indices[start + i + j]].m_normal = n;
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::ComputeTexCoord(float uv_scale, int uv_offset)
{
    UNUSED(uv_scale, uv_offset);
#if 0
    VertexDictionnary vert_dict;
    Array<int> tri_list;

    tri_list.Reserve(m_indices.Count() - m_cursors.Last().m2);
    for (int i = m_cursors.Last().m2; i < m_indices.Count(); i++)
    {
        vert_dict.AddVertex(m_indices[i], m_vert[m_indices[i]].m_coord);
        tri_list << m_indices[i];
    }

    //full triangle count
    Array<int> tri_done;
    Array<int> tri_check;
    int tri_count = (m_indices.Count() - m_cursors.Last().m2) / 3;

    tri_check << tri_list[0];

    while (tri_check.Count())
    {
        int cur_tri = tri_check[0];
        int v[3]   = { tri_list[cur_tri + uv_offset % 3], tri_list[cur_tri + (1 + uv_offset) % 3], tri_list[cur_tri + (2 + uv_offset) % 3] };
        vec2 uv[3] = { m_vert[tri_list[cur_tri]].m_texcoord.xy, m_vert[tri_list[cur_tri + 1]].m_texcoord.xy, m_vert[tri_list[cur_tri + 2]].m_texcoord.xy };
        for (int j = 0; j < 3; j++)
        {
            if (uv[j] != vec2(-1.0f) && uv[j] == uv[(j + 1) % 3])
            {
                uv[0] = vec2(-1.0f);
                uv[1] = vec2(-1.0f);
                uv[2] = vec2(-1.0f);
                break;
            }
        }
        int uv_set = 0;
        for (int j = 0; j < 3; j++)
            uv_set += (uv[j].x < 0.f)?(0):(1);

        //this case shouldn't happen.
        if (uv_set == 1)
        {
            /*
            for (int j = 0; j < 3; j++)
            {
                if (uv[j] != vec2(-1.0f))
                {
                    uv[(j + 1) % 2] = uv[j] + vec2(.0f, uv_scale * length(m_vert[v[j]].m1 - m_vert[v[(j + 1) % 3]].m1));
                    uv_set = 2;
                    break;
                }
            }
            */
        }
        //No UV is set, let's do the arbitrary set and use the basic method.
        if (uv_set == 0)
        {
            float new_dot = FLT_MAX;
            int base_i = 0;
            for (int j = 0; j < 3; j++)
            {
                float tmp_dot = abs(dot(normalize(m_vert[v[(j + 1) % 3]].m_coord - m_vert[v[j]].m_coord),
                                        normalize(m_vert[v[(j + 2) % 3]].m_coord - m_vert[v[j]].m_coord)));
                if (tmp_dot < new_dot)
                {
                    base_i = j;
                    new_dot = tmp_dot;
                }
            }
            uv[base_i] = vec2(.0f);
            uv[(base_i + 1) % 3] = vec2(.0f, uv_scale * length(m_vert[v[base_i]].m_coord - m_vert[v[(base_i + 1) % 3]].m_coord));
            uv_set = 2;
        }
        //2 points have been set, let's figure the third
        if (uv_set == 2)
        {
            {
                //invert values so the two set uv are in [0, 1] slots.
                int new_v[3];
                vec2 new_uv[3];
                bool ignore_set = false;
                if (uv[0].x >= 0.f && uv[1].x < 0.f)
                {
                    new_v[0] = v[2]; new_v[1] = v[0]; new_v[2] = v[1];
                    new_uv[0] = uv[2]; new_uv[1] = uv[0]; new_uv[2] = uv[1];
                }
                else if (uv[0].x < 0.f && uv[1].x >= 0.f)
                {
                    new_v[0] = v[1]; new_v[1] = v[2]; new_v[2] = v[0];
                    new_uv[0] = uv[1]; new_uv[1] = uv[2]; new_uv[2] = uv[0];
                }
                else
                    ignore_set = true;
                if (!ignore_set)
                {
                    v[0]  = new_v[0];  v[1]  = new_v[1];  v[2]  = new_v[2];
                    uv[0] = new_uv[0]; uv[1] = new_uv[1]; uv[2] = new_uv[2];
                }
            }

            //Do this to be sure the normal is OK.
            ComputeNormals(cur_tri, 3);
            vec3 v01 = normalize(m_vert[v[1]].m_coord - m_vert[v[0]].m_coord);
            vec3 v02 = m_vert[v[2]].m_coord - m_vert[v[0]].m_coord;
            vec3 v_dir = normalize(cross(m_vert[m_indices[cur_tri]].m_normal, v01));
            vec2 texu_dir = uv[1] - uv[0];
            vec2 texv_dir = vec2(texu_dir.y, texu_dir.x);
            //Final calculations
            uv[2] = texu_dir * dot(v01, v02) + texv_dir * dot(v_dir, v02);

            //Set UV on ALL matching vertices!
            Array<int> matching_vert;
            for (int i = 0; i < 3; i++)
            {
#if 1
                //This marks all same position UV to the same values
                //Deactivation is a test.
                matching_vert << v[i];
                vert_dict.FindMatchingVertices(v[i], matching_vert);
                for (int j = 0; j < matching_vert.Count(); j++)
                    if (m_vert[matching_vert[j]].m_texcoord.xy == vec2(-1.0f))
                        m_vert[matching_vert[j]].m_texcoord = vec4(abs(uv[i]), m_vert[matching_vert[j]].m_texcoord.zw);
#else
                m_vert[v[i]].m_texcoord = abs(uv[i]);
#endif
            }

            tri_done << cur_tri;
            tri_check.Remove(0);

            //Get connected triangles and go from there.
            for (int j = 0; j < 3; j++)
            {
#if 1
                //This finds triangle that are connected to this triangle
                vert_dict.FindConnectedTriangles(ivec2(v[j], v[(j + 1) % 3]), tri_list, tri_check, &tri_done);
#else
                //This finds triangle that are connected to the vertices of this triangle
                vert_dict.FindConnectedTriangles(v[j], tri_list, tri_check, &tri_done);
#endif
            }
        }
        else if (uv_set == 3)
        {
            for (int j = 0; j < 3; j++)
            {
                m_vert[tri_list[cur_tri]].m_texcoord     = vec4(vec2(-1.0f), m_vert[tri_list[cur_tri]].m_texcoord.zw);
                m_vert[tri_list[cur_tri + 1]].m_texcoord = vec4(vec2(-1.0f), m_vert[tri_list[cur_tri + 1]].m_texcoord.zw);
                m_vert[tri_list[cur_tri + 2]].m_texcoord = vec4(vec2(-1.0f), m_vert[tri_list[cur_tri + 2]].m_texcoord.zw);
            }

            //uv[0] = vec2(-1.0f);
            //uv[1] = vec2(-1.0f);
            //uv[2] = vec2(-1.0f);
            /*
            bool tri_present = false;
            for (int j = 0; j < tri_done.Count(); j++)
                if (cur_tri == tri_done[j])
                    tri_present = true;
            if (!tri_present)
                tri_done << cur_tri;
            tri_check.Remove(0);
            */
        }

        if (tri_check.Count() == 0 && tri_done.Count() != tri_count)
        {
            //look for unset triangle
            for (int i = 0; !tri_check.Count() && i < tri_list.Count(); i += 3)
            {
                bool tri_present = false;
                for (int j = 0; j < tri_done.Count(); j++)
                    if (i == tri_done[j])
                        tri_present = true;
                if (!tri_present)
                    tri_check << i;
            }
        }
    }
#endif
}

//-----------------------------------------------------------------------------
void EasyMesh::SetVertColor(vec4 const &color)
{
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        m_vert[i].m_color = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetTexCoordData(vec2 const &new_offset, vec2 const &new_scale)
{
    BD()->TexCoordOffset() = new_offset;
    BD()->TexCoordScale() = new_scale;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetTexCoordData2(vec2 const &new_offset, vec2 const &new_scale)
{
    BD()->TexCoordOffset2() = new_offset;
    BD()->TexCoordScale2() = new_scale;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurVertNormal(vec3 const &normal)
{
    m_vert[m_vert.Count() - 1].m_normal = normal;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurVertColor(vec4 const &color)
{
    m_vert[m_vert.Count() - 1].m_color = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurVertTexCoord(vec2 const &texcoord)
{
    m_vert[m_vert.Count() - 1].m_texcoord = vec4(texcoord, m_vert[m_vert.Count() - 1].m_texcoord.zw);
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurVertTexCoord2(vec2 const &texcoord)
{
    m_vert[m_vert.Count() - 1].m_texcoord = vec4(m_vert[m_vert.Count() - 1].m_texcoord.xy, texcoord);
}

//-----------------------------------------------------------------------------
void EasyMesh::Translate(vec3 const &v)
{
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        m_vert[i].m_coord += v;
}

//-----------------------------------------------------------------------------
void EasyMesh::RotateX(float angle) { Rotate(angle, vec3(1, 0, 0)); }
void EasyMesh::RotateY(float angle) { Rotate(angle, vec3(0, 1, 0)); }
void EasyMesh::RotateZ(float angle) { Rotate(angle, vec3(0, 0, 1)); }

//-----------------------------------------------------------------------------
void EasyMesh::Rotate(float angle, vec3 const &axis)
{
    mat3 m = mat3::rotate(angle, axis);
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        m_vert[i].m_coord  = m * m_vert[i].m_coord;
        m_vert[i].m_normal = m * m_vert[i].m_normal;
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::RadialJitter(float r)
{
    Array<int> Welded;
    Welded.Push(-1);
    for (int i = m_cursors.Last().m1 + 1; i < m_vert.Count(); i++)
    {
        int j, k;
        for (j = m_cursors.Last().m1, k = 0; j < i; j++, k++)
        {
            if(Welded[k] < 0)
            {
                vec3 diff = m_vert[i].m_coord - m_vert[j].m_coord;

                if(diff.x > 0.1f || diff.x < -0.1f)
                    continue;

                if(diff.y > 0.1f || diff.y < -0.1f)
                    continue;

                if(diff.z > 0.1f || diff.z < -0.1f)
                    continue;

                break;
            }
        }

        if(j == i)
            Welded.Push(-1);
        else
            Welded.Push(j);
    }

    int i, j;
    for (i = m_cursors.Last().m1, j = 0; i < m_vert.Count(); i++, j++)
    {
        if(Welded[j] == -1)
            m_vert[i].m_coord *= 1.0f + rand(r);
        else
            m_vert[i].m_coord = m_vert[Welded[j]].m_coord;
    }

    ComputeNormals(m_cursors.Last().m2, m_indices.Count() - m_cursors.Last().m2);
}

//-----------------------------------------------------------------------------
void EasyMesh::TaperX(float ny, float nz, float xoff, bool absolute) { DoMeshTransform(MeshTransform::Taper, Axis::X, Axis::X, ny, nz, xoff, absolute); }
void EasyMesh::TaperY(float nx, float nz, float yoff, bool absolute) { DoMeshTransform(MeshTransform::Taper, Axis::Y, Axis::Y, nz, nx, yoff, absolute); }
void EasyMesh::TaperZ(float nx, float ny, float zoff, bool absolute) { DoMeshTransform(MeshTransform::Taper, Axis::Z, Axis::Z, nx, ny, zoff, absolute); }

//-----------------------------------------------------------------------------
void EasyMesh::TwistX(float t, float toff) { DoMeshTransform(MeshTransform::Twist, Axis::X, Axis::X, t, t, toff); }
void EasyMesh::TwistY(float t, float toff) { DoMeshTransform(MeshTransform::Twist, Axis::Y, Axis::Y, t, t, toff); }
void EasyMesh::TwistZ(float t, float toff) { DoMeshTransform(MeshTransform::Twist, Axis::Z, Axis::Z, t, t, toff); }

//-----------------------------------------------------------------------------
void EasyMesh::ShearX(float ny, float nz, float xoff, bool absolute) { DoMeshTransform(MeshTransform::Shear, Axis::X, Axis::X, ny, nz, xoff, absolute); }
void EasyMesh::ShearY(float nx, float nz, float yoff, bool absolute) { DoMeshTransform(MeshTransform::Shear, Axis::Y, Axis::Y, nz, nx, yoff, absolute); }
void EasyMesh::ShearZ(float nx, float ny, float zoff, bool absolute) { DoMeshTransform(MeshTransform::Shear, Axis::Z, Axis::Z, nx, ny, zoff, absolute); }

//-----------------------------------------------------------------------------
void EasyMesh::StretchX(float ny, float nz, float xoff) { DoMeshTransform(MeshTransform::Stretch, Axis::X, Axis::X, ny, nz, xoff); }
void EasyMesh::StretchY(float nx, float nz, float yoff) { DoMeshTransform(MeshTransform::Stretch, Axis::Y, Axis::Y, nz, nx, yoff); }
void EasyMesh::StretchZ(float nx, float ny, float zoff) { DoMeshTransform(MeshTransform::Stretch, Axis::Z, Axis::Z, nx, ny, zoff); }

//-----------------------------------------------------------------------------
void EasyMesh::BendXY(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::X, Axis::Y, t, t, toff); }
void EasyMesh::BendXZ(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::X, Axis::Z, t, t, toff); }
void EasyMesh::BendYX(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::Y, Axis::X, t, t, toff); }
void EasyMesh::BendYZ(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::Y, Axis::Z, t, t, toff); }
void EasyMesh::BendZX(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::Z, Axis::X, t, t, toff); }
void EasyMesh::BendZY(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::Z, Axis::Y, t, t, toff); }

//-----------------------------------------------------------------------------
void EasyMesh::DoMeshTransform(MeshTransform ct, Axis axis0, Axis axis1, float n0, float n1, float noff, bool absolute)
{
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        switch (ct)
        {
            case MeshTransform::Taper:
            {
                float value = m_vert[i].m_coord[axis0];
                if (absolute) value = abs(value);
                m_vert[i].m_coord[(axis0 + 1) % 3] *= max(0.f, 1.f + (n0 * value + noff));
                m_vert[i].m_coord[(axis0 + 2) % 3] *= max(0.f, 1.f + (n1 * value + noff));
                break;
            }
            case MeshTransform::Twist:
            {
                vec3 rotaxis = vec3(1.f); rotaxis[(axis0 + 1) % 3] = .0f; rotaxis[(axis0 + 2) % 3] = .0f;
                m_vert[i].m_coord = mat3::rotate(m_vert[i].m_coord[axis0] * n0 + noff, rotaxis) * m_vert[i].m_coord;
                break;
            }
            case MeshTransform::Shear:
            {
                float value = m_vert[i].m_coord[axis0];
                if (absolute) value = abs(value);
                m_vert[i].m_coord[(axis0 + 1) % 3] += (n0 * value + noff);
                m_vert[i].m_coord[(axis0 + 2) % 3] += (n1 * value + noff);
                break;
            }
            case MeshTransform::Stretch:
            {
                //float value = abs(m_vert[i].m1[axis0]);
                //m_vert[i].m1[(axis0 + 1) % 3] += (lol::pow(value, n0) + noff);
                //m_vert[i].m1[(axis0 + 2) % 3] += (lol::pow(value, n1) + noff);
                break;
            }
            case MeshTransform::Bend:
            {
                vec3 rotaxis = vec3(1.f); rotaxis[(axis1 + 1) % 3] = .0f; rotaxis[(axis1 + 2) % 3] = .0f;
                m_vert[i].m_coord = mat3::rotate(m_vert[i].m_coord[axis0] * n0 + noff, rotaxis) * m_vert[i].m_coord;
                break;
            }
        }
    }
    ComputeNormals(m_cursors.Last().m2, m_indices.Count() - m_cursors.Last().m2);
}

//-----------------------------------------------------------------------------
void EasyMesh::Scale(vec3 const &s)
{
    vec3 const invs = vec3(1) / s;

    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        m_vert[i].m_coord *= s;
        m_vert[i].m_normal = normalize(m_vert[i].m_normal * invs);
    }

    /* Flip winding if the scaling involves mirroring */
    if (!BD()->IsEnabled(MeshBuildOperation::Scale_Winding) && s.x * s.y * s.z < 0)
    {
        for (int i = m_cursors.Last().m2; i < m_indices.Count(); i += 3)
        {
            uint16_t tmp = m_indices[i + 0];
            m_indices[i + 0] = m_indices[i + 1];
            m_indices[i + 1] = tmp;
        }
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::MirrorX() { DupAndScale(vec3(-1, 1, 1)); }
void EasyMesh::MirrorY() { DupAndScale(vec3(1, -1, 1)); }
void EasyMesh::MirrorZ() { DupAndScale(vec3(1, 1, -1)); }

//-----------------------------------------------------------------------------
void EasyMesh::DupAndScale(vec3 const &s)
{
    int vlen = m_vert.Count() - m_cursors.Last().m1;
    int tlen = m_indices.Count() - m_cursors.Last().m2;

    for (int i = 0; i < vlen; i++)
        AddDuplicateVertex(m_cursors.Last().m1++);

    for (int i = 0; i < tlen; i++)
        m_indices << m_indices[m_cursors.Last().m2++] + vlen;

    Scale(s);

    m_cursors.Last().m1 -= vlen;
    m_cursors.Last().m2 -= tlen;
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendCylinder(int nsides, float h, float d1, float d2,
                              bool dualside, bool smooth, bool close)
{
    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r1 = d1 * .5f;
    float r2 = d2 * .5f;

    //SAVE
    vec4 Saved_Color = BD()->Color();
    vec4 Saved_Color2 = BD()->Color2();
    vec2 Save_texcoord_offset = BD()->TexCoordOffset();
    vec2 Save_texcoord_scale = BD()->TexCoordScale();

    int vbase = m_vert.Count();

    mat3 rotmat = mat3::rotate(360.0f / (float)nsides, 0.f, 1.f, 0.f);
    vec3 p1(r1, -h * .5f, 0.f), p2(r2, h * .5f, 0.f), n;
    vec2 uv1(.0f, .0f), uv2(.0f, 1.0f), uvadd(1.0f / (float)nsides, .0f);
    if (close)
        SetTexCoordData(vec2(.0f), vec2(1.0f, .5f));

    /* Construct normal */
    if (r2 != .0f)
        n = vec3(r2, h * .5f, 0.f);
    else
        n = vec3(r1, h * .5f, 0.f);
    n.y = r1 * (r1 - r2) / h;
    if (!smooth)
        n = mat3::rotate(180.0f / nsides, 0.f, 1.f, 0.f) * n;
    n = normalize(n);

    /* FIXME: normals should be flipped in two-sided mode, but that
     * means duplicating the vertices again... */
    for (int i = 0; i < nsides; i++)
    {
        AddVertex(p1); SetCurVertNormal(n); SetCurVertTexCoord(uv1); SetCurVertTexCoord2(uv1);
        AddVertex(p2); SetCurVertNormal(n); SetCurVertTexCoord(uv2); SetCurVertTexCoord2(uv2);
        SetCurVertColor(BD()->Color2());

        if (smooth)
        {
            int j = (i + 1) % nsides;
            AppendQuad(j * 2, j * 2 + 1, i * 2 + 1, i * 2, vbase);
            if (dualside)
                AppendQuad(i * 2, i * 2 + 1, j * 2 + 1, j * 2, vbase);
        }

        p1 = rotmat * p1; uv1 += uvadd;
        p2 = rotmat * p2; uv2 += uvadd;

        if (!smooth)
        {
            AddVertex(p1); SetCurVertNormal(n); SetCurVertTexCoord(uv1); SetCurVertTexCoord2(uv1);
            AddVertex(p2); SetCurVertNormal(n); SetCurVertTexCoord(uv2); SetCurVertTexCoord2(uv2);
            SetCurVertColor(BD()->Color2());

            AppendQuad(i * 4 + 2, i * 4 + 3, i * 4 + 1, i * 4, vbase);
            if (dualside)
                AppendQuad(i * 4, i * 4 + 1, i * 4 + 3, i * 4 + 2, vbase);
        }

        n = rotmat * n;
    }

    if (close)
    {
        //START
        OpenBrace();
        //LOWER DISC
        SetTexCoordData(vec2(.0f, .5f), vec2(.5f, .5f));
        SetCurColor(BD()->Color());
        AppendDisc(nsides, d1);
        Translate(vec3(.0f, h, .0f));
        RotateX(180.0f);
        //UPPER DISC
        SetTexCoordData(vec2(.5f, .5f), vec2(.5f, .5f));
        SetCurColor(BD()->Color2());
        AppendDisc(nsides, d2);
        Translate(vec3(.0f, h * .5f, .0f));
        CloseBrace();
    }
    //RESTORE
    SetCurColor(Saved_Color);
    SetCurColor2(Saved_Color2);
    SetTexCoordData(Save_texcoord_offset, Save_texcoord_scale);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSphere(int ndivisions, float d)
{
    AppendCapsule(ndivisions, 0.f, d);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendCapsule(int ndivisions, float h, float d)
{
    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r = d * .5f;

    int ibase = m_indices.Count();

    Array<vec3> vertices;
    float uv_h = 0;
    float uv_r = 0;

    /* FIXME: we don't know how to handle even-divided capsules, so we
     * force the count to be odd. */
    if (h)
    {
        ndivisions |= 1;
        //calculate uv h&r percents
        uv_h = (float)h / (float)(h + r * 2);
        uv_r = (float)r / (float)(h + r * 2);
    }

    /* Fill in the icosahedron vertices, rotating them so that there
     * is a vertex at [0 1 0] and [0 -1 0] after normalisation. */
    float phi = 0.5f + 0.5f * sqrt(5.f);
    mat3 mat = mat3::rotate(asin(1.f / sqrt(2.f + phi)) * (180.f / F_PI),
                            vec3(0.f, 0.f, 1.f));
    for (int i = 0; i < 4; i++)
    {
        float x = (i & 1) ? 0.5f : -0.5f;
        float y = (i & 2) ? phi * 0.5f : phi * -0.5f;
        vertices << mat * vec3(x, y, 0.f);
        vertices << mat * vec3(0.f, x, y);
        vertices << mat * vec3(y, 0.f, x);
    }

    static int const trilist[] =
    {
        0, 1, 2, 2, 4, 6, 3, 8, 1, 9, 4, 8,
        7, 0, 5, 7, 11, 3, 10, 5, 6, 10, 9, 11,

        0, 3, 1, 7, 3, 0, 1, 4, 2, 8, 4, 1,
        2, 5, 0, 6, 5, 2, 6, 9, 10, 4, 9, 6,
        7, 10, 11, 5, 10, 7, 8, 11, 9, 3, 11, 8
    };

    for (unsigned i = 0; i < sizeof(trilist) / sizeof(*trilist); i += 3)
    {
        vec3 const &a = vertices[trilist[i]];
        vec3 const &b = vertices[trilist[i + 1]];
        vec3 const &c = vertices[trilist[i + 2]];

        vec3 const vb = 1.f / ndivisions * (b - a);
        vec3 const vc = 1.f / ndivisions * (c - a);

        int line = ndivisions + 1;

        for (int v = 0, x = 0, y = 0; x < ndivisions + 1; v++)
        {
            vec3 p[] = { a + (float)x * vb + (float)y * vc,
                         p[0] + vb,
                         p[0] + vc,
                         p[0] + vb + vc };
            vec2 uv[4];

            /* FIXME: when we normalise here, we get a volume that is slightly
             * smaller than the sphere of radius 1, since we are not using
             * the midradius. */
            for (int k = 0; k < 4; k++)
            {
                //keep normalized until the end of the UV calculations
                p[k] = normalize(p[k]);

                uv[k].x = (lol::atan2(p[k].z, p[k].x) + F_PI) / (F_PI * 2.f);
                if (abs(p[k].y) >= 1.0f)
                    uv[k].x = -1.f;
                uv[k].y = lol::atan2(p[k].y, dot(p[k], normalize(p[k] * vec3(1.f,0.f,1.f)))) / F_PI + 0.5f;
                if (h)
                {
                    if (uv[k].y > .5f)
                        uv[k].y = uv_r + uv_h + (uv[k].y - .5f) * uv_r * 2.f;
                    else
                        uv[k].y *= uv_r * 2.f;
                }
                p[k] *= r;
            }

            /* If this is a capsule, grow in the Y direction */
            if (h > 0.f)
            {
                for (int k = 0; k < 4; k++)
                    p[k].y += (p[k].y > 0.f) ? 0.5f * h : -0.5f * h;
            }

            /* Add zero, one or two triangles */
            int id[] = { 0, 1, 2,
                         1, 3 ,2 };
            int l = 6;
            while ((l -= 3) >= 0)
            {
                if ((l == 0 && y < line - 1) || (l == 3 && y < line - 2))
                {
                    int k = -1;
                    while (++k < 3)
                    {
                        int rid[] = { id[k + l], id[(k + 1) % 3 + l] };
                        if (uv[rid[0]].x >= .0f &&
                            uv[rid[1]].x >= .0f &&
                            abs(uv[rid[0]].x - uv[rid[1]].x) > .5f)
                        {
                            if (uv[rid[0]].x < uv[rid[1]].x)
                                uv[rid[0]].x += 1.0f;
                            else
                                uv[rid[1]].x += 1.0f;
                        }
                    }
                    k = -1;
                    while (++k < 3)
                    {
                        int rid[] = { id[k + l], id[(k + 1) % 3 + l], id[(k + 2) % 3 + l] };
                        AddVertex(p[rid[0]]);
                        vec2 new_uv;
                        if (uv[rid[0]].x < .0f)
                            new_uv = vec2((uv[rid[1]].x + uv[rid[2]].x) * .5f, uv[rid[0]].y);
                        else
                            new_uv = uv[rid[0]];
                        SetCurVertTexCoord(vec2(0.f, 1.f) - new_uv);
                        SetCurVertTexCoord2(vec2(0.f, 1.f) - new_uv);
                    }
                    AppendTriangle(0, 2, 1, m_vert.Count() - 3);
                }
            }

            y++;
            if (y == line)
            {
                x++;
                y = 0;
                line--;
            }
        }
    }

    ComputeNormals(ibase, m_indices.Count() - ibase);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendTorus(int ndivisions, float d1, float d2)
{
    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r1 = d1 * .5f;
    float r2 = d2 * .5f;

    int ibase = m_indices.Count();
    int nidiv = ndivisions; /* Cross-section */
    int njdiv = ndivisions; /* Full circumference */

    for (int j = 0; j < njdiv; j++)
    for (int i = 0; i < 2 * nidiv; i++)
    {
        for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
        {
            int i2 = (i + di) % nidiv;
            int j2 = (j + dj) % njdiv;

            //Location on the donut
            float x = 0.5f * (r2 - r1) * (float)lol::cos(2.f * F_PI * i2 / nidiv) + 0.5f * (r1 + r2);
            float y = 0.5f * (r2 - r1) * (float)lol::sin(2.f * F_PI * i2 / nidiv);
            float z = 0.0f;

            //Center circle
            float ca = (float)lol::cos(2.f * F_PI * j2 / njdiv);
            float sa = (float)lol::sin(2.f * F_PI * j2 / njdiv);

            //Actual location
            float x2 = x * ca - z * sa;
            float z2 = z * ca + x * sa;

            AddVertex(vec3(x2, y, z2));
            SetCurVertTexCoord(vec2((float)(i + di) / (float)nidiv, (float)(j + dj) / (float)nidiv));
            SetCurVertTexCoord2(vec2((float)(i + di) / (float)nidiv, (float)(j + dj) / (float)nidiv));
        }

        AppendTriangle(0, 2, 3, m_vert.Count() - 4);
        AppendTriangle(0, 3, 1, m_vert.Count() - 4);
    }

    ComputeNormals(ibase, m_indices.Count() - ibase);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendBox(vec3 const &size, float chamf)
{
    AppendBox(size, chamf, false);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSmoothChamfBox(vec3 const &size, float chamf)
{
    AppendBox(size, chamf, true);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendFlatChamfBox(vec3 const &size, float chamf)
{
    AppendBox(size, chamf, false);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendBox(vec3 const &size, float chamf, bool smooth)
{
    if (chamf < 0.0f)
    {
        AppendBox(size + vec3(chamf * 2.0f), -chamf, smooth);
        return;
    }

    int vbase = m_vert.Count();
    int ibase = m_indices.Count();

    vec3 d = size * 0.5f;

    MeshType mt = MeshType::Box;
    TexCoordPos bl = TexCoordPos::BL;
    TexCoordPos br = TexCoordPos::BR;
    TexCoordPos tl = TexCoordPos::TL;
    TexCoordPos tr = TexCoordPos::TR;

    //--
    //Side vertices
    //--
    MeshFaceType mft = MeshFaceType::BoxFront;
    AddVertex(vec3(-d.x, -d.y, -d.z - chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, +d.y, -d.z - chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, +d.y, -d.z - chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, -d.y, -d.z - chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    mft = MeshFaceType::BoxLeft;
    AddVertex(vec3(-d.x - chamf, -d.y, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x - chamf, +d.y, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x - chamf, +d.y, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x - chamf, -d.y, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    mft = MeshFaceType::BoxBack;
    AddVertex(vec3(+d.x, -d.y, +d.z + chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, +d.y, +d.z + chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, +d.y, +d.z + chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, -d.y, +d.z + chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    mft = MeshFaceType::BoxRight;
    AddVertex(vec3(+d.x + chamf, -d.y, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x + chamf, +d.y, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x + chamf, +d.y, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x + chamf, -d.y, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    //Bottom vertices
    //--
    mft = MeshFaceType::BoxBottom;
    AddVertex(vec3(-d.x, -d.y - chamf, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, -d.y - chamf, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, -d.y - chamf, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, -d.y - chamf, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    //Top vertices
    //--
    mft = MeshFaceType::BoxTop;
    AddVertex(vec3(-d.x, +d.y + chamf, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, +d.y + chamf, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, +d.y + chamf, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, +d.y + chamf, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    /* The 6 quads on each side of the box */
    for (int i = 0; i < 24; i += 4)
        AppendQuad(i, i + 1, i + 2, i + 3, vbase);

    ComputeNormals(ibase, m_indices.Count() - ibase);
    ibase = m_indices.Count();

    /* The 8 quads at each edge of the box */
    if (chamf)
    {
        static int const quadlist[48] =
        {
            0, 3, 18, 17, 4, 7, 17, 16, 8, 11, 16, 19, 12, 15, 19, 18,
            2, 1, 20, 23, 6, 5, 21, 20, 10, 9, 22, 21, 14, 13, 23, 22,
            1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 3, 2,
        };

        for (int i = 0; i < 48; i += 4)
        {
            if (smooth)
                AppendQuad(quadlist[i], quadlist[i + 1],
                           quadlist[i + 2], quadlist[i + 3], vbase);
            else
                AppendQuadDuplicateVerts(quadlist[i], quadlist[i + 1],
                                 quadlist[i + 2], quadlist[i + 3], vbase);
        }
    }

    /* The 8 triangles at each corner of the box */
    if (chamf)
    {
        static int const trilist[24] =
        {
            3, 12, 18, 15, 8, 19, 11, 4, 16, 7, 0, 17,
            2, 23, 13, 14, 22, 9, 10, 21, 5, 6, 20, 1,
        };

        for (int i = 0; i < 24; i += 3)
        {
            if (smooth)
                AppendTriangle(trilist[i], trilist[i + 1],
                               trilist[i + 2], vbase);
            else
                AppendTriangleDuplicateVerts(trilist[i], trilist[i + 1],
                                             trilist[i + 2], vbase);
        }
    }

    if (!smooth)
        ComputeNormals(ibase, m_indices.Count() - ibase);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendStar(int nbranches, float d1, float d2,
                          bool fade, bool fade2)
{
    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r1 = d1 * .5f;
    float r2 = d2 * .5f;

    //TODO: It would probably be good to think of another way of UV painting this, like "branch repeating"
    int vbase = m_vert.Count();
    float maxr = max(r1, r2);

    AddVertex(vec3(0.f, 0.f, 0.f)); SetCurVertTexCoord(vec2(.5f, .5f)); SetCurVertTexCoord2(vec2(.5f, .5f));

    mat3 rotmat = mat3::rotate(180.0f / nbranches, 0.f, 1.f, 0.f);
    vec3 p1(r1, 0.f, 0.f), p2(r2, 0.f, 0.f);
    vec3 uv1(0.f, 0.f, -.5f * ((float)r1 / maxr)),
         uv2(0.f, 0.f, -.5f * ((float)r2 / maxr));

    p2 = rotmat * p2; uv2 = rotmat * uv2;
    rotmat = rotmat * rotmat;

    for (int i = 0; i < nbranches; i++)
    {
        AddVertex(p1); SetCurVertTexCoord(uv1.xz + vec2(.5f)); SetCurVertTexCoord2(uv1.xz + vec2(.5f));
        if (fade2)
            SetCurVertColor(BD()->Color2());

        AddVertex(p2); SetCurVertTexCoord(uv2.xz + vec2(.5f)); SetCurVertTexCoord2(uv2.xz + vec2(.5f));
        if (fade)
            SetCurVertColor(BD()->Color2());

        AppendQuad(0, 2 * i + 1, 2 * i + 2, (2 * i + 3) % (2 * nbranches),
                   vbase);

        p1 = rotmat * p1; uv1 = rotmat * uv1;
        p2 = rotmat * p2; uv2 = rotmat * uv2;
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendExpandedStar(int nbranches, float d1, float d2, float extrad)
{
    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r1 = d1 * .5f;
    float r2 = d2 * .5f;
    float extrar = extrad * .5f;

    int vbase = m_vert.Count();
    float maxr = (float)max(max(r1, r2), max(r1 + extrar, r2 + extrar));

    AddVertex(vec3(0.f, 0.f, 0.f)); SetCurVertTexCoord(vec2(.5f, .5f)); SetCurVertTexCoord2(vec2(.5f, .5f));

    mat3 rotmat = mat3::rotate(180.0f / nbranches, 0.f, 1.f, 0.f);
    vec3 p1(r1, 0.f, 0.f), p2(r2, 0.f, 0.f),
         p3(r1 + extrar, 0.f, 0.f), p4(r2 + extrar, 0.f, 0.f);;
    vec3 uv1(0.f, 0.f, -.5f * ((float)r1 / maxr)),
         uv2(0.f, 0.f, -.5f * ((float)r2 / maxr)),
         uv3(0.f, 0.f, -.5f * ((float)(r1 + extrar) / maxr)),
         uv4(0.f, 0.f, -.5f * ((float)(r2 + extrar) / maxr));

    p2 = rotmat * p2; uv2 = rotmat * uv2;
    p4 = rotmat * p4; uv4 = rotmat * uv4;
    rotmat = rotmat * rotmat;

    for (int i = 0; i < nbranches; i++)
    {
        AddVertex(p1); SetCurVertTexCoord(uv1.xz + vec2(.5f)); SetCurVertTexCoord2(uv1.xz + vec2(.5f));
        AddVertex(p2); SetCurVertTexCoord(uv2.xz + vec2(.5f)); SetCurVertTexCoord2(uv2.xz + vec2(.5f));
        AddVertex(p3); SetCurVertTexCoord(uv3.xz + vec2(.5f)); SetCurVertTexCoord2(uv3.xz + vec2(.5f)); SetCurVertColor(BD()->Color2());
        AddVertex(p4); SetCurVertTexCoord(uv4.xz + vec2(.5f)); SetCurVertTexCoord2(uv4.xz + vec2(.5f)); SetCurVertColor(BD()->Color2());

        int j = (i + 1) % nbranches;
        AppendQuad(0, 4 * i + 1, 4 * i + 2, 4 * j + 1, vbase);
        AppendQuad(4 * i + 1, 4 * i + 3, 4 * i + 4, 4 * i + 2, vbase);
        AppendQuad(4 * j + 1, 4 * i + 2, 4 * i + 4, 4 * j + 3, vbase);

        p1 = rotmat * p1; uv1 = rotmat * uv1;
        p2 = rotmat * p2; uv2 = rotmat * uv2;
        p3 = rotmat * p3; uv3 = rotmat * uv3;
        p4 = rotmat * p4; uv4 = rotmat * uv4;
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendDisc(int nsides, float d, bool fade)
{
    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r = d * .5f;

    int vbase = m_vert.Count();

    AddVertex(vec3(0.f, 0.f, 0.f)); SetCurVertTexCoord(vec2(.5f, .5f)); SetCurVertTexCoord2(vec2(.5f, .5f));

    mat3 rotmat = mat3::rotate(360.0f / nsides, 0.f, 1.f, 0.f);
    vec3 p1(r, 0.f, 0.f);
    vec3 uv(.5f, .0f, .0f);

    for (int i = 0; i < nsides; i++)
    {
        AddVertex(p1); SetCurVertTexCoord(uv.xz + vec2(.5f, .5f)); SetCurVertTexCoord2(uv.xz + vec2(.5f, .5f));
        if (fade)
            SetCurVertColor(BD()->Color2());
        AppendTriangle(0, i + 1, ((i + 1) % nsides) + 1, vbase);
        p1 = rotmat * p1;
        uv = rotmat * uv;
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSimpleTriangle(float d, bool fade)
{
    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float size = d * .5f;

    mat3 m = mat3::rotate(120.f, 0.f, 1.f, 0.f);
    vec3 p(0.f, 0.f, size);

    AddVertex(p); SetCurVertTexCoord(vec2(.5f, 0.133975f)); SetCurVertTexCoord2(vec2(.5f, 0.133975f));
    p = m * p;
    AddVertex(p); SetCurVertTexCoord(vec2(0.f, 1.f)); SetCurVertTexCoord2(vec2(0.f, 1.f));
    if (fade)
        SetCurVertColor(BD()->Color2());
    p = m * p;
    AddVertex(p); SetCurVertTexCoord(vec2(1.f, 1.f)); SetCurVertTexCoord2(vec2(1.f, 1.f));
    if (fade)
        SetCurVertColor(BD()->Color2());

    AppendTriangle(0, 1, 2, m_vert.Count() - 3);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSimpleQuad(float size, bool fade)
{
    AppendSimpleQuad(vec2(size * .5f), vec2(size * -.5f), 0.f, fade);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSimpleQuad(vec2 p1, vec2 p2, float z, bool fade)
{
    MeshType mt = MeshType::Quad;
    MeshFaceType mft = MeshFaceType::QuadDefault;

    //--
    AddVertex(vec3(p2.x, z, -p1.y));
    TexCoordPos br = TexCoordPos::BR;
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, br, mft));
    //--
    AddVertex(vec3(p2.x, z, -p2.y));
    TexCoordPos bl = TexCoordPos::BL;
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, bl, mft));
    //--
    AddVertex(vec3(p1.x, z, -p2.y));
    TexCoordPos tl = TexCoordPos::TL;
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tl, mft));
    if (fade) SetCurVertColor(BD()->Color2());
    //--
    AddVertex(vec3(p1.x, z, -p1.y));
    TexCoordPos tr = TexCoordPos::TR;
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    if (fade) SetCurVertColor(BD()->Color2());

    AppendQuad(0, 1, 2, 3, m_vert.Count() - 4);
    ComputeNormals(m_indices.Count() - 6, 6);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendCog(int nbsides, float h, float d10, float d20,
                         float d1, float d2, float d12, float d22,
                         float sidemul, bool offset)
{
    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r10 = d10 * .5f;
    float r20 = d20 * .5f;
    float r1  = d1  * .5f;
    float r2  = d2  * .5f;
    float r12 = d12 * .5f;
    float r22 = d22 * .5f;

    int ibase = m_indices.Count();
    int vbase = m_vert.Count();

    /* FIXME: enforce this some other way */
    if (r12 < 0)
        h = -h;

    mat3 rotmat = mat3::rotate(180.0f / nbsides, 0.f, 1.f, 0.f);
    mat3 smat1 = mat3::rotate(sidemul * 180.0f / nbsides, 0.f, 1.f, 0.f);
    mat3 smat2 = mat3::rotate(sidemul * -360.0f / nbsides, 0.f, 1.f, 0.f);

    vec3 p[12];

    //Upper points
    p[0] = vec3(r10, h * .5f, 0.f);
    p[1] = rotmat * p[0];
    p[2] = vec3(r1, h * .5f, 0.f);
    p[3] = rotmat * p[2];
    p[4] = smat1 * (rotmat * vec3(r1 + r12, h * .5f, 0.f));
    p[5] = smat2 * (rotmat * p[4]);

    //Lower points
    p[6] = vec3(r20, h * -.5f, 0.f);
    p[7] = rotmat * p[6];
    p[8] = vec3(r2, h * -.5f, 0.f);
    p[9] = rotmat * p[8];
    p[10] = smat1 * (rotmat * vec3(r2 + r22, h * -.5f, 0.f));
    p[11] = smat2 * (rotmat * p[10]);

    if (offset)
        for (int n = 0; n < 12; n++)
            p[n] = rotmat * p[n];

    rotmat = rotmat * rotmat;

    //UV base computation
    float maxr = max(max(r1 + r12, r2 + r22), max(r10, r20));
    float InLn = length(p[1] - p[0]);
    float CogLn[8] = { .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0, k = 2; j < 8 && k < 12; j += 4, k += 6)
        {
            CogLn[j + i] = length(p[k + i + 1] - p[k + i]);
            CogLn[j + 3] += CogLn[j + i];
            if (i == 1) //Add 3to4 twice since it's automatically completed by +1 loop.
                CogLn[j + 3] += CogLn[j + i];
        }
    }

    //Choose the biggest cog length
    int CogSrc = (CogLn[7] > CogLn[3])?(4):(0);
    CogLn[3] = CogLn[CogSrc + 3];
    for (int i = 0; i < 3; i++)
        CogLn[i] = CogLn[CogSrc + i] / CogLn[CogSrc + 3];

    //Calculate Cog Modifiers
    vec2 InUV[2] = { vec2(.0f), vec2(.5f) };
    vec2 CogUV[2] = { vec2(.0f), vec2(.5f) };
    vec2 upadd = vec2(.25f, .75f);
    vec2 lowadd = vec2(.75f, .75f);
    {
        if (h < InLn)
        {
            InUV[0].x  = 1.0f;
            InUV[0].y  = h / InLn;
            InUV[1].x  = .0f;
            InUV[1].y -= InUV[0].y * .5f;
        }
        else
        {
            InUV[0].x  = InLn / h;
            InUV[0].y  = 1.0f;
            InUV[1].x -= InUV[0].x * .5f;
            InUV[1].y = .0f;
        }
        if (h < CogLn[3])
        {
            CogUV[0].x  = 1.0f;
            CogUV[0].y  = h / CogLn[3];
            CogUV[1].x  = .0f;
            CogUV[1].y -= CogUV[0].y * .5f;
        }
        else
        {
            CogUV[0].x  = CogLn[3] / h;
            CogUV[0].y  = 1.0f;
            CogUV[1].x -= CogUV[0].x * .5f;
            CogUV[1].y  = .0f;
        }
        if (InUV[0].x + CogUV[0].x < .5f)
        {
            InUV[1].x = .0f;
            CogUV[1].x = .5f - CogUV[0].x;
            upadd  = vec2(.75f, .25f);
            lowadd = vec2(.75f, .75f);
        }
        else if (InUV[0].y + CogUV[0].y < .5f)
        {
            InUV[1].y = .0f;
            CogUV[1].y = .5f - CogUV[0].y;
        }
        else
        {
            InUV[0] *= .5f;
            InUV[1] *= .5f;
            CogUV[0] *= .5f;
            CogUV[1] *= .5f;
            InUV[1] += vec2(.5f, .0f);
        }
    }

    //Build UV tab
    vec2 uv[12]; float CogSz;
    //Upper points
    CogSz = 1.0f - CogLn[1];
    uv[0]  = vec2(0.f,   0.f) * InUV[0]  + InUV[1];
    uv[1]  = vec2(1.f,   0.f) * InUV[0]  + InUV[1];
    uv[5]  = vec2(CogSz, 0.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[2];
    uv[4]  = vec2(CogSz, 0.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[1];
    uv[3]  = vec2(CogSz, 0.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[0];
    uv[2]  = vec2(0.f,   0.f) * CogUV[0] + CogUV[1];

    //Lower points
    CogSz = 1.0f - CogLn[1];
    uv[6]  = vec2(0.f,   1.f) * InUV[0]  + InUV[1];
    uv[7]  = vec2(1.f,   1.f) * InUV[0]  + InUV[1];
    uv[11] = vec2(CogSz, 1.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[2];
    uv[10] = vec2(CogSz, 1.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[1];
    uv[ 9] = vec2(CogSz, 1.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[0];
    uv[ 8] = vec2(0.f,   1.f) * CogUV[0] + CogUV[1];

    //Gear generation loop
    for (int i = 0; i < nbsides; i++)
    {
        int j = 3 * 12 * i,
            k = 3 * 12 * ((i + 1) % nbsides);

        int q[] = { /* The top and bottom faces */
                    j, j, j, j,
                    j, j, j, j,
                    j, j, k, k,
                    k, k, j, j,
                    j, j, j, k,
                    k, j, j, j,
                    /* The inner side quads */
                    j, j, j, j,
                    j, k, k, j,
                    /* The outer side quads */
                    j, j, j, j,
                    j, j, j, j,
                    j, j, j, j,
                    k, j, j, k
                    };
        int m[] = { /* The top and bottom faces */
                    0,  2,  3,  1,
                    7,  9,  8,  6,
                    1,  3,  2,  0,
                    6,  8,  9,  7,
                    3,  4,  5,  2,
                    8, 11, 10,  9,
                    /* The inner side quads */
                    0,  1,  7,  6,
                    1,  0,  6,  7,
                    /* The outer side quads */
                    3,  2,  8,  9,
                    4,  3,  9, 10,
                    5,  4, 10, 11,
                    2,  5, 11, 8
                    };
        int a[] = { /* The top and bottom faces */
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    /* The inner side quads */
                    1, 1, 1, 1,
                    2, 2, 2, 2,
                    /* The outer side quads */
                    1, 1, 1, 1,
                    1, 2, 2, 1,
                    1, 2, 2, 1,
                    2, 2, 2, 2
                    };

        /* Each vertex will share three faces, so three different
         * normals, therefore we add each vertex three times. */
        for (int n = 0; n < 3 * 12; n++)
        {
            int d = n / 3;
            int e = d % 6;
            AddVertex(p[d]);
            if (n % 3 == 0) //Top-Bottom logic
            {
                vec2 tmp = (p[d].xz / maxr);
                vec2 add;
                if (d >= 6)
                {
                    tmp *= -1.0f;
                    add = lowadd;
                }
                else
                    add = upadd;
                SetCurVertTexCoord(tmp * vec2(.25f) + add);
                SetCurVertTexCoord2(tmp * vec2(.25f) + add);
            }
            else if (e == 0 || e == 1) //inner Logic
            {
                SetCurVertTexCoord(uv[d]);
                SetCurVertTexCoord2(uv[d]);
            }
            else //Cog logic
            {
                if (e == 2 && n % 3 == 2)
                {
                    SetCurVertTexCoord(vec2(1.f, (d == 2)?(0.f):(1.f)) * CogUV[0] + CogUV[1]);
                    SetCurVertTexCoord2(vec2(1.f, (d == 2)?(0.f):(1.f)) * CogUV[0] + CogUV[1]);
                }
                else
                {
                    SetCurVertTexCoord(uv[d]);
                    SetCurVertTexCoord2(uv[d]);
                }
            }
            if (d >= 6)
                SetCurVertColor(BD()->Color2());
        }

        int l = -4;
        while ((l += 4) < 48)
            AppendQuad(q[l + 0] + m[l + 0] * 3 + a[l + 0],
                       q[l + 1] + m[l + 1] * 3 + a[l + 1],
                       q[l + 2] + m[l + 2] * 3 + a[l + 2],
                       q[l + 3] + m[l + 3] * 3 + a[l + 3],
                       vbase);

        for (int n = 0; n < 12; n++)
            p[n] = rotmat * p[n];
    }

    ComputeNormals(ibase, m_indices.Count() - ibase);
}

//-----------------------------------------------------------------------------
void EasyMesh::Chamfer(float f)
{
    int vlen = m_vert.Count() - m_cursors.Last().m1;
    int ilen = m_indices.Count() - m_cursors.Last().m2;

    /* Step 1: enumerate all faces. This is done by merging triangles
     * that are coplanar and share an edge. */
    int *triangle_classes = new int[ilen / 3];
    for (int i = 0; i < ilen / 3; i++)
        triangle_classes[i] = -1;

    for (int i = 0; i < ilen / 3; i++)
    {

    }

    /* Fun shit: reduce all triangles */
    int *vertices = new int[vlen];
    memset(vertices, 0, vlen * sizeof(int));
    for (int i = 0; i < ilen; i++)
        vertices[m_indices[i]]++;

    for (int i = 0; i < ilen / 3; i++)

    {
    #if 0
        if (vertices[m_indices[i * 3]] > 1)
            continue;
        if (vertices[m_indices[i * 3 + 1]] > 1)
            continue;
        if (vertices[m_indices[i * 3 + 2]] > 1)
            continue;
    #endif

        vec3 bary = 1.f / 3.f * (m_vert[m_indices[i * 3]].m_coord +
                                 m_vert[m_indices[i * 3 + 1]].m_coord +
                                 m_vert[m_indices[i * 3 + 2]].m_coord);
        for (int k = 0; k < 3; k++)
        {
            vec3 &p = m_vert[m_indices[i * 3 + k]].m_coord;
            p -= normalize(p - bary) * f;
        }
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::SplitTriangles(int pass) { SplitTriangles(pass, nullptr); }

//-----------------------------------------------------------------------------
void EasyMesh::SplitTriangles(int pass, VertexDictionnary *vert_dict)
{
    while (pass--)
    {
        int trimax = m_indices.Count();
        for (int i = m_cursors.Last().m2; i < trimax; i += 3)
        {
            int vbase = m_vert.Count();
            int j = -1;
            while (++j < 3)
            {
                AddLerpVertex(m_indices[i + j], m_indices[i + (j + 1) % 3], .5f);
                if (vert_dict)
                    vert_dict->AddVertex(vbase + j, m_vert[vbase + j].m_coord);
            }
            //Add new triangles
            AppendTriangle(vbase, m_indices[i + 1], vbase + 1, 0);
            AppendTriangle(vbase + 2, vbase + 1, m_indices[i + 2], 0);
            AppendTriangle(vbase, vbase + 1, vbase + 2, 0);
            //Change current triangle
            m_indices[i + 1] = vbase;
            m_indices[i + 2] = vbase + 2;
        }
    }
    ComputeNormals(m_cursors.Last().m2, m_indices.Count() - m_cursors.Last().m2);
}

//-----------------------------------------------------------------------------
//TODO : Add an half-edges implementation to refine smooth.
//TODO : Smooth should only use connected vertices that are on edges of the mesh (See box).
void EasyMesh::SmoothMesh(int main_pass, int split_per_main_pass, int smooth_per_main_pass)
{
    VertexDictionnary vert_dict;
    Array<vec3> smooth_buf[2];
    Array<int> master_list;
    Array<int> matching_ids;
    Array<int> connected_vert;
    int smbuf = 0;

    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        vert_dict.AddVertex(i, m_vert[i].m_coord);

    while (main_pass--)
    {
        int split_pass = split_per_main_pass;
        int smooth_pass = smooth_per_main_pass;

        SplitTriangles(split_pass, &vert_dict);

        matching_ids.Reserve(m_vert.Count() - m_cursors.Last().m1);
        connected_vert.Reserve(m_vert.Count() - m_cursors.Last().m1);
        smooth_buf[0].Resize(m_vert.Count() - m_cursors.Last().m1);
        smooth_buf[1].Resize(m_vert.Count() - m_cursors.Last().m1);

        for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
            smooth_buf[smbuf][i - m_cursors.Last().m1] = m_vert[i].m_coord;

        while (smooth_pass--)
        {
            master_list.Empty();
            if (vert_dict.GetMasterList(master_list))
            {
                for (int i = 0; i < master_list.Count(); i++)
                {
                    connected_vert.Empty();
                    if (vert_dict.FindConnectedVertices(master_list[i], m_indices, m_cursors.Last().m2, connected_vert))
                    {
                        //Calculate vertices sum
                        vec3 vert_sum = vec3(.0f);
                        for (int j = 0; j < connected_vert.Count(); j++)
                            vert_sum += smooth_buf[smbuf][connected_vert[j] - m_cursors.Last().m1];

                        //Calculate new master vertex
                        float n = (float)connected_vert.Count();
                        //b(n) = 5/4 - pow(3 + 2 * cos(2.f * F_PI / n), 2) / 32
                        float beta = 3.f + 2.f * cos(2.f * F_PI / n);
                        beta = 5.f / 4.f - beta * beta / 32.f;
                        //a(n) = n * (1 - b(n)) / b(n)
                        float alpha = (n * (1 - beta)) / beta;
                        //V = (a(n) * v + v1 + ... + vn) / (a(n) + n)
                        vec3 new_vert = (alpha * smooth_buf[smbuf][master_list[i] - m_cursors.Last().m1] + vert_sum) / (alpha + n);

                        //Set all matching vertices to new value
                        matching_ids.Empty();
                        matching_ids << master_list[i];
                        vert_dict.FindMatchingVertices(master_list[i], matching_ids);
                        for (int j = 0; j < matching_ids.Count(); j++)
                            smooth_buf[1 - smbuf][matching_ids[j] - m_cursors.Last().m1] = new_vert;
                    }
                }
            }
            smbuf = 1 - smbuf;
        }

        for (int i = 0; i < smooth_buf[smbuf].Count(); i++)
            m_vert[i + m_cursors.Last().m1].m_coord = smooth_buf[smbuf][i];
    }
}

} /* namespace lol */
