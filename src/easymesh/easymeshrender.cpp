//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2009—2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2009—2013 Cédric Lecacheur <jordx@free.fr>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{

LOLFX_RESOURCE_DECLARE(easymesh_shiny);
LOLFX_RESOURCE_DECLARE(easymesh_shinyflat);
LOLFX_RESOURCE_DECLARE(easymesh_shinydebugwireframe);
LOLFX_RESOURCE_DECLARE(easymesh_shinydebuglighting);
LOLFX_RESOURCE_DECLARE(easymesh_shinydebugnormal);
LOLFX_RESOURCE_DECLARE(easymesh_shinydebugUV);
LOLFX_RESOURCE_DECLARE(easymesh_shiny_SK);

//-----------------------------------------------------------------------------
void EasyMesh::MeshConvert()
{
    /* Default material */
    auto shader = Shader::Create(LOLFX_RESOURCE_NAME(easymesh_shiny));

    /* Push index buffer to GPU */
    auto ibo = std::make_shared<IndexBuffer>(m_indices.count() * sizeof(uint16_t));
    uint16_t *indices = (uint16_t *)ibo->lock(0, 0);
    for (int i = 0; i < m_indices.count(); ++i)
        indices[i] = m_indices[i];
    ibo->unlock();

    /* Push vertex buffer to GPU */
    struct Vertex
    {
        vec3 pos, normal;
        u8vec4 color;
        vec4 texcoord;
    };

    auto vdecl = std::make_shared<VertexDeclaration>(
        VertexStream<vec3, vec3, u8vec4, vec4>(VertexUsage::Position,
                                               VertexUsage::Normal,
                                               VertexUsage::Color,
                                               VertexUsage::TexCoord));

    auto vbo = std::make_shared<VertexBuffer>(m_vert.count() * sizeof(Vertex));
    Vertex *vert = (Vertex *)vbo->lock(0, 0);
    for (int i = 0; i < m_vert.count(); ++i)
    {
        vert[i].pos = m_vert[i].m_coord,
        vert[i].normal = m_vert[i].m_normal,
        vert[i].color = (u8vec4)(m_vert[i].m_color * 255.f);
        vert[i].texcoord = m_vert[i].m_texcoord;
    }
    vbo->unlock();

    /* Reference our new data in our submesh */
    m_submeshes.push_back(std::make_shared<SubMesh>(shader, vdecl));
    m_submeshes.back()->SetIndexBuffer(ibo);
    m_submeshes.back()->SetVertexBuffer(0, vbo);

    m_state = MeshRender::CanRender;
}

//-----------------------------------------------------------------------------
GpuShaderData::GpuShaderData()
{
    m_render_mode = DebugRenderMode::Default;
}

//-----------------------------------------------------------------------------
GpuShaderData::GpuShaderData(uint16_t vert_decl_flags, std::shared_ptr<Shader> shader, DebugRenderMode render_mode)
  : m_shader(shader),
    m_render_mode(render_mode),
    m_vert_decl_flags(vert_decl_flags)
{
}

//-----------------------------------------------------------------------------
GpuShaderData::~GpuShaderData()
{
    m_shader_uniform.clear();
    m_shader_attrib.clear();
}

//-----------------------------------------------------------------------------
void GpuShaderData::AddUniform(std::string const &new_uniform)
{
    m_shader_uniform.push(new_uniform, m_shader->GetUniformLocation(new_uniform));
}

//-----------------------------------------------------------------------------
void GpuShaderData::AddAttribute(VertexUsage usage, int index)
{
    m_shader_attrib.push(m_shader->GetAttribLocation(usage, index));
}

//-----------------------------------------------------------------------------
ShaderUniform const *GpuShaderData::GetUniform(std::string const &uniform)
{
    for (int i = 0; i < m_shader_uniform.count(); ++i)
        if (std::get<0>(m_shader_uniform[i]) == uniform)
            return &std::get<1>(m_shader_uniform[i]);
    return nullptr;
}

//-----------------------------------------------------------------------------
ShaderAttrib const *GpuShaderData::GetAttribute(VertexUsage usage, int index)
{
    for (int i = 0; i < m_shader_attrib.count(); ++i)
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
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(easymesh_shiny));
    else if (render_mode == DebugRenderMode::Flat)
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(easymesh_shinyflat));
    else if (render_mode == DebugRenderMode::Wireframe)
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(easymesh_shinydebugwireframe));
    else if (render_mode == DebugRenderMode::Lighting)
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(easymesh_shinydebuglighting));
    else if (render_mode == DebugRenderMode::Normal)
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(easymesh_shinydebugnormal));
    else if (render_mode == DebugRenderMode::UV)
    {
        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(easymesh_shinydebugUV));
        m_vert_decl_flags |= (1 << VertexUsage::TexCoord);
        with_UV = true;
    }
    StoreUniformNames();
    SetupDefaultData(with_UV);
}

//-----------------------------------------------------------------------------
DefaultShaderData::DefaultShaderData(uint16_t vert_decl_flags, std::shared_ptr<Shader> shader, bool with_UV)
    : GpuShaderData(vert_decl_flags, shader, DebugRenderMode::Default)
{
    StoreUniformNames();
    SetupDefaultData(with_UV);
}

static std::string const DefaultUniforms[8] =
{
    "u_lights",
    "u_model",
    "u_modelview",
    "u_view",
    "u_inv_view",
    "u_proj",
    "u_normalmat",
    "u_damage",
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
    Scene& scene = Scene::GetScene();
    mat4 proj = scene.GetCamera()->GetProjection();
    mat4 view = scene.GetCamera()->GetView();
    mat4 modelview = view * model;
    mat3 normalmat = transpose(inverse(mat3(modelview)));
    /* FIXME: this should be hidden in the shader */
    std::vector<Light *> const &lights = scene.GetLights();
    std::vector<vec4> light_data;
    //This is not very nice, but necessary for emscripten WebGL generation.
    float f = 0.f;

    /* FIXME: the 4th component of the position can be used for other things */
    /* FIXME: GetUniform("blabla") is costly */
    for (auto *l : lights)
    {
        light_data.push_back(vec4(l->GetPosition(), (float)l->GetType()));
        light_data.push_back(l->GetColor());
    }

    while (light_data.size() < LOL_MAX_LIGHT_COUNT)
    {
        light_data.push_back(vec4::zero);
        light_data.push_back(vec4::zero);
    }

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
}

//-----------------------------------------------------------------------------
GpuEasyMeshData::~GpuEasyMeshData()
{
    m_gpudata.clear();
    m_vdata.clear();
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
void GpuEasyMeshData::AddGpuData(std::shared_ptr<GpuShaderData> gpudata, std::shared_ptr<EasyMesh> src_mesh)
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
        std::vector<uint16_t> indexlist;
        for (int i = 0; i < src_mesh->m_indices.count(); i += 3)
        {
            indexlist.push_back(src_mesh->m_indices[i + 0]);
            indexlist.push_back(src_mesh->m_indices[i + 1]);
            indexlist.push_back(src_mesh->m_indices[i + 2]);
        }

        m_ibo = std::make_shared<IndexBuffer>(indexlist.size() * sizeof(indexlist[0]));
        m_ibo->set_data(indexlist.data(), indexlist.size() * sizeof(indexlist[0]));

        m_indexcount = indexlist.size();
    }

    //init to a minimum of gpudata->m_render_mode size
    if (m_gpudata.count() <= gpudata->m_render_mode)
    {
        int i = m_gpudata.count();
        int max = gpudata->m_render_mode + 1;
        m_gpudata.reserve(max);
        for (; i < max; i++)
            m_gpudata.push(nullptr);
    }
    m_gpudata[gpudata->m_render_mode] = gpudata;
}

//-----------------------------------------------------------------------------
void GpuEasyMeshData::SetupVertexData(uint16_t vflags, std::shared_ptr<EasyMesh> src_mesh)
{
    for (int i = 0; i < m_vdata.count(); ++i)
        if (std::get<0>(m_vdata[i]) == vflags)
            return;

    std::shared_ptr<VertexDeclaration> new_vdecl;
    std::shared_ptr<VertexBuffer> new_vbo;
    void *vbo_data = nullptr;
    int vbo_bytes = 0;

#define COPY_VBO \
    vbo_data = &vertexlist[0]; \
    vbo_bytes = vertexlist.size() * sizeof(vertexlist[0]); \
    m_vertexcount = vertexlist.size(); \
    new_vbo = std::make_shared<VertexBuffer>(vbo_bytes); \
    new_vbo->set_data(vbo_data, vbo_bytes);

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
        new_vdecl = std::make_shared<VertexDeclaration>(
                         VertexStream<vec3,vec3,u8vec4,vec4>(
                          VertexUsage::Position,
                          VertexUsage::Normal,
                          VertexUsage::Color,
                          VertexUsage::TexCoord));

        std::vector<std::tuple<vec3, vec3, u8vec4, vec4>> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.count(); i++)
            vertexlist.push_back(std::make_tuple(
                src_mesh->m_vert[i].m_coord,
                src_mesh->m_vert[i].m_normal,
                (u8vec4)(src_mesh->m_vert[i].m_color * 255.f),
                src_mesh->m_vert[i].m_texcoord));
        COPY_VBO;
    }
    else if (flagnb == 4 && has_position && has_normal && has_color && has_texcoord)
    {
        new_vdecl = std::make_shared<VertexDeclaration>(
                         VertexStream<vec3,vec3,u8vec4,vec2>(
                          VertexUsage::Position,
                          VertexUsage::Normal,
                          VertexUsage::Color,
                          VertexUsage::TexCoord));

        std::vector<std::tuple<vec3, vec3, u8vec4, vec2>> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.count(); i++)
            vertexlist.push_back(std::make_tuple(
                src_mesh->m_vert[i].m_coord,
                src_mesh->m_vert[i].m_normal,
                u8vec4(src_mesh->m_vert[i].m_color * 255.f),
                vec2(src_mesh->m_vert[i].m_texcoord.xy)));
        COPY_VBO;
    }
    else if (flagnb == 4 && has_position && has_color && has_texcoord && has_texcoordExt)
    {
        new_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec3,vec4,vec4>(VertexUsage::Position, VertexUsage::Color, VertexUsage::TexCoord));

        std::vector<std::tuple<vec3, vec4, vec4>> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.count(); i++)
            vertexlist.push_back(std::make_tuple(
                src_mesh->m_vert[i].m_coord,
                src_mesh->m_vert[i].m_color,
                src_mesh->m_vert[i].m_texcoord));
        COPY_VBO;
    }
    else if (flagnb == 3 && has_position && has_normal && has_color)
    {
        new_vdecl = std::make_shared<VertexDeclaration>(
                         VertexStream<vec3,vec3,u8vec4>(
                          VertexUsage::Position,
                          VertexUsage::Normal,
                          VertexUsage::Color));

        std::vector<std::tuple<vec3,vec3,u8vec4>> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.count(); i++)
            vertexlist.push_back(std::make_tuple(
                src_mesh->m_vert[i].m_coord,
                src_mesh->m_vert[i].m_normal,
                (u8vec4)(src_mesh->m_vert[i].m_color * 255.f)));
        COPY_VBO;
    }
    else if (flagnb == 3 && has_position && has_texcoord && has_texcoordExt)
    {
        new_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec3,vec4>(VertexUsage::Position, VertexUsage::TexCoord));

        std::vector<std::tuple<vec3, vec4>> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.count(); i++)
            vertexlist.push_back(std::make_tuple(
                src_mesh->m_vert[i].m_coord,
                src_mesh->m_vert[i].m_texcoord));
        COPY_VBO;
    }
    else if (flagnb == 2 && has_position && has_texcoord)
    {
        new_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec3,vec2>(VertexUsage::Position, VertexUsage::TexCoord));

        std::vector<std::tuple<vec3, vec2>> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.count(); i++)
            vertexlist.push_back(std::make_tuple(
                src_mesh->m_vert[i].m_coord,
                vec2(src_mesh->m_vert[i].m_texcoord.xy)));
        COPY_VBO;
    }
    else if (flagnb == 2 && has_position && has_color)
    {
        new_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec3,u8vec4>(VertexUsage::Position, VertexUsage::Color));

        std::vector<std::tuple<vec3, u8vec4>> vertexlist;
        for (int i = 0; i < src_mesh->m_vert.count(); i++)
            vertexlist.push_back(std::make_tuple(
                src_mesh->m_vert[i].m_coord,
                u8vec4(src_mesh->m_vert[i].m_color * 255.f)));
        COPY_VBO;
    }
    else
        ASSERT(0, "no Vertex Declaration combination for 0x%04x", vflags);

    m_vdata.push(vflags, new_vdecl, new_vbo);
}

//-----------------------------------------------------------------------------
void GpuEasyMeshData::RenderMeshData(mat4 const &model, int render_mode)
{
    ASSERT(0 <= render_mode && render_mode < m_gpudata.count(), "render mode is not in the defined range");
    ASSERT(m_gpudata[render_mode], "gpu datas for this render mode don't exist");
    GpuShaderData& gpu_sd = *(m_gpudata[render_mode]);

    int vdecl_idx = 0;
    for (; vdecl_idx < m_vdata.count(); ++vdecl_idx)
        if (std::get<0>(m_vdata[vdecl_idx]) == gpu_sd.m_vert_decl_flags)
            break;

    if (vdecl_idx >= m_vdata.count())
        return;

    uint16_t vflags = std::get<0>(m_vdata[vdecl_idx]);
    auto vdecl = std::get<1>(m_vdata[vdecl_idx]);
    auto vbo = std::get<2>(m_vdata[vdecl_idx]);

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
    vdecl->DrawIndexedElements(MeshPrimitive::Triangles, m_indexcount);
    m_ibo->Unbind();
    vdecl->Unbind();
}

} /* namespace lol */
