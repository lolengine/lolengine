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

#include <lol/engine-internal.h>

namespace lol
{

LOLFX_RESOURCE_DECLARE(shiny);
LOLFX_RESOURCE_DECLARE(shinyflat);
LOLFX_RESOURCE_DECLARE(shinydebugwireframe);
LOLFX_RESOURCE_DECLARE(shinydebuglighting);
LOLFX_RESOURCE_DECLARE(shinydebugnormal);
LOLFX_RESOURCE_DECLARE(shinydebugUV);
LOLFX_RESOURCE_DECLARE(shiny_SK);

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
    else if (render_mode == DebugRenderMode::Flat)
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(shinyflat));
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
    StoreUniformNames();
    SetupDefaultData(with_UV);
}

//-----------------------------------------------------------------------------
DefaultShaderData::DefaultShaderData(uint16_t vert_decl_flags, Shader* shader, bool with_UV)
    : GpuShaderData(vert_decl_flags, shader, DebugRenderMode::Default)
{
    StoreUniformNames();
    SetupDefaultData(with_UV);
}

static const String DefaultUniforms[8] =
{
    String("u_lights"),
    String("u_model"),
    String("u_modelview"),
    String("u_view"),
    String("u_inv_view"),
    String("u_proj"),
    String("u_normalmat"),
    String("in_Damage")
};
//-----------------------------------------------------------------------------
void DefaultShaderData::StoreUniformNames()
{
}

//-----------------------------------------------------------------------------
void DefaultShaderData::SetupDefaultData(bool with_UV)
{
    UNUSED(with_UV);
    for (int i = 0; i < 7; i++)
        AddUniform(DefaultUniforms[i]);
}

//-----------------------------------------------------------------------------
void DefaultShaderData::SetupShaderDatas(mat4 const &model)
{
    mat4 proj = g_scene->GetCamera()->GetProjection();
    mat4 view = g_scene->GetCamera()->GetView();
    mat4 modelview = view * model;
    mat3 normalmat = transpose(inverse(mat3(modelview)));
    /* FIXME: this should be hidden in the shader */
    array<Light *> const &lights = g_scene->GetLights();
    array<vec4> light_data;
    //This is not very nice, but necessary for emscripten WebGL generation.
    float f = 0.f;

    /* FIXME: the 4th component of the position can be used for other things */
    /* FIXME: GetUniform("blabla") is costly */
    for (int i = 0; i < lights.Count(); ++i)
        light_data << vec4(lights[i]->GetPosition(), lights[i]->GetType()) << lights[i]->GetColor();
    while (light_data.Count() < LOL_MAX_LIGHT_COUNT)
        light_data << vec4::zero << vec4::zero;

    int i = 0;
    m_shader->SetUniform(*GetUniform(DefaultUniforms[i++]), light_data);

    m_shader->SetUniform(*GetUniform(DefaultUniforms[i++]), model);
    m_shader->SetUniform(*GetUniform(DefaultUniforms[i++]), modelview);
    m_shader->SetUniform(*GetUniform(DefaultUniforms[i++]), view);
    m_shader->SetUniform(*GetUniform(DefaultUniforms[i++]), inverse(view));
    m_shader->SetUniform(*GetUniform(DefaultUniforms[i++]), proj);
    m_shader->SetUniform(*GetUniform(DefaultUniforms[i++]), normalmat);
    m_shader->SetUniform(*GetUniform(DefaultUniforms[i++]), f);
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
    ASSERT(!vflags, "no Vertex Usage setup for 0x%04x", vflags);

    if (has_position)   gpudata->AddAttribute(VertexUsage::Position, 0);
    if (has_normal)     gpudata->AddAttribute(VertexUsage::Normal, 0);
    if (has_color)      gpudata->AddAttribute(VertexUsage::Color, 0);
    if (has_texcoord)   gpudata->AddAttribute(VertexUsage::TexCoord, 0);

    SetupVertexData(gpudata->m_vert_decl_flags, src_mesh);

    if (!m_ibo)
    {
        array<uint16_t> indexlist;
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

    //init to a minimum of gpudata->m_render_mode size
    if (m_gpudatas.Count() <= gpudata->m_render_mode)
    {
        int i = m_gpudatas.Count();
        int max = gpudata->m_render_mode + 1;
        m_gpudatas.Reserve(max);
        for (; i < max; i++)
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
    vbo_data = &vertexlist[0]; \
    vbo_bytes = vertexlist.Bytes(); \
    m_vertexcount = vertexlist.Count(); \
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
    ASSERT(!saveflags, "no Vertex Declaration setup for 0x%04x", vflags);

    if (flagnb == 5 && has_position && has_normal && has_color && has_texcoord && has_texcoordExt)
    {
        new_vdecl = new VertexDeclaration(
                         VertexStream<vec3,vec3,u8vec4,vec4>(
                          VertexUsage::Position,
                          VertexUsage::Normal,
                          VertexUsage::Color,
                          VertexUsage::TexCoord));

        array<vec3, vec3, u8vec4, vec4> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord,
                            src_mesh->m_vert[i].m_normal,
                            (u8vec4)(src_mesh->m_vert[i].m_color * 255.f),
                            src_mesh->m_vert[i].m_texcoord);
        COPY_VBO;
    }
    else if (flagnb == 4 && has_position && has_normal && has_color && has_texcoord)
    {
        new_vdecl = new VertexDeclaration(
                         VertexStream<vec3,vec3,u8vec4,vec2>(
                          VertexUsage::Position,
                          VertexUsage::Normal,
                          VertexUsage::Color,
                          VertexUsage::TexCoord));

        array<vec3, vec3, u8vec4, vec2> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord,
                            src_mesh->m_vert[i].m_normal,
                            (u8vec4)(src_mesh->m_vert[i].m_color * 255.f),
                            src_mesh->m_vert[i].m_texcoord.xy);
        COPY_VBO;
    }
    else if (flagnb == 4 && has_position && has_color && has_texcoord && has_texcoordExt)
    {
        new_vdecl = new VertexDeclaration(VertexStream<vec3,vec4,vec4>(VertexUsage::Position, VertexUsage::Color, VertexUsage::TexCoord));

        array<vec3, vec4, vec4> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord, src_mesh->m_vert[i].m_color, src_mesh->m_vert[i].m_texcoord);
        COPY_VBO;
    }
    else if (flagnb == 3 && has_position && has_normal && has_color)
    {
        new_vdecl = new VertexDeclaration(
                         VertexStream<vec3,vec3,u8vec4>(
                          VertexUsage::Position,
                          VertexUsage::Normal,
                          VertexUsage::Color));

        array<vec3,vec3,u8vec4> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord,
                            src_mesh->m_vert[i].m_normal,
                            (u8vec4)(src_mesh->m_vert[i].m_color * 255.f));
        COPY_VBO;
    }
    else if (flagnb == 3 && has_position && has_texcoord && has_texcoordExt)
    {
        new_vdecl = new VertexDeclaration(VertexStream<vec3,vec4>(VertexUsage::Position, VertexUsage::TexCoord));

        array<vec3, vec4> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord, src_mesh->m_vert[i].m_texcoord);
        COPY_VBO;
    }
    else if (flagnb == 2 && has_position && has_texcoord)
    {
        new_vdecl = new VertexDeclaration(VertexStream<vec3,vec2>(VertexUsage::Position, VertexUsage::TexCoord));

        array<vec3, vec2> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord, src_mesh->m_vert[i].m_texcoord.xy);
        COPY_VBO;
    }
    else if (flagnb == 2 && has_position && has_color)
    {
        new_vdecl = new VertexDeclaration(VertexStream<vec3,u8vec4>(VertexUsage::Position, VertexUsage::Color));

        array<vec3, u8vec4> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.Count(); i++)
            vertexlist.Push(src_mesh->m_vert[i].m_coord, (u8vec4)(src_mesh->m_vert[i].m_color * 255.f));
        COPY_VBO;
    }
    else
        ASSERT(0, "no Vertex Declaration combination for 0x%04x", vflags);

    m_vdatas.Push(vflags, new_vdecl, new_vbo);
}

//-----------------------------------------------------------------------------
void GpuEasyMeshData::RenderMeshData(mat4 const &model, int render_mode)
{
    ASSERT(0 <= render_mode && render_mode < m_gpudatas.Count(), "render mode is not in the defined range");
    ASSERT(m_gpudatas[render_mode], "gpu datas for this render mode don't exist");
    GpuShaderData& gpu_sd = *(m_gpudatas[render_mode]);

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
    ASSERT(!vflags, "no Vertex Stream setup for 0x%04x", vflags);

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

} /* namespace lol */
