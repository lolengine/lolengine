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

#pragma once

#include <string>
#include <map>

namespace lol
{

//MeshRenderBase --------------------------------------------------------------
//Utility enum for renderers
struct MeshRenderBase : public StructSafeEnum
{
    enum Type
    {
        NeedData,
        NeedConvert,
        CanRender,
        IgnoreRender,
    };
protected:
    virtual bool BuildEnumMap(std::map<int64_t, std::string>& enum_map)
    {
        enum_map[NeedData] = "NeedData";
        enum_map[NeedConvert] = "NeedConvert";
        enum_map[CanRender] = "CanRender";
        enum_map[IgnoreRender] = "IgnoreRender";
        return true;
    }
};
typedef SafeEnum<MeshRenderBase> MeshRender;

//Vertex datas for easymesh vertex list.
//TODO : <COORD, NORM, COLOR, UV>
struct VertexData
{
    vec3    m_coord;
    vec3    m_normal;
    vec4    m_color;
    vec4    m_texcoord;
    ivec4   m_bone_id;
    vec4    m_bone_weight;

    VertexData(vec3  new_coord      = vec3(0.f),
               vec3  new_normal     = vec3(0.f, 1.f, 0.f),
               vec4  new_color      = vec4(0.f),
               vec4  new_texcoord   = vec4(0.f),
               ivec4 new_bone_id    = ivec4(0),
               vec4  new_bone_weight= vec4(0.f))
    {
        m_coord         = new_coord;
        m_normal        = new_normal;
        m_color         = new_color;
        m_texcoord      = new_texcoord;
        m_bone_id       = new_bone_id;
        m_bone_weight   = new_bone_weight;
    }
};

//Base class to declare shader datas
class GpuShaderData
{
    friend class GpuEasyMeshData;

protected:
    GpuShaderData();
public:
    //--
    GpuShaderData(uint16_t vert_decl_flags, std::shared_ptr<Shader> shader, DebugRenderMode render_mode);
    virtual ~GpuShaderData();
    //--
    void AddUniform(std::string const &new_uniform);
    void AddAttribute(VertexUsage usage, int index);
    ShaderUniform const *GetUniform(std::string const &uniform);
    ShaderAttrib const *GetAttribute(VertexUsage usage, int index);
    //--
    virtual void SetupShaderDatas(mat4 const &) { }
    //--

protected:
    std::shared_ptr<Shader> m_shader;
    int m_render_mode;
    uint16_t m_vert_decl_flags;
    easy_array<std::string, ShaderUniform> m_shader_uniform;
    easy_array<ShaderAttrib> m_shader_attrib;
};

class DefaultShaderData : public GpuShaderData
{
public:
    //---
    DefaultShaderData(DebugRenderMode render_mode);
    DefaultShaderData(uint16_t vert_decl_flags, std::shared_ptr<Shader> shader, bool with_UV);
    virtual ~DefaultShaderData() {}
    void StoreUniformNames();
    //---
    void SetupDefaultData(bool with_UV);
    virtual void SetupShaderDatas(mat4 const &model);
    //--
    easy_array<std::string> m_uniform_names;
};

class GpuEasyMeshData
{
    friend class EasyMesh;

public:
    //---
    GpuEasyMeshData();
    ~GpuEasyMeshData();
    //---
    void AddGpuData(std::shared_ptr<GpuShaderData> gpudata, std::shared_ptr<class EasyMesh> src_mesh);
    void RenderMeshData(mat4 const &model, int render_mode = video::GetDebugRenderMode());
    bool HasData(int render_mode) { return (0 <= render_mode && render_mode < m_gpudata.count() && !!m_gpudata[render_mode]); }

private:
    void SetupVertexData(uint16_t vdecl_flags, std::shared_ptr<EasyMesh> src_mesh);

    easy_array<std::shared_ptr<GpuShaderData>> m_gpudata;
    //uint16_t are the vdecl/vbo flags to avoid copy same vdecl several times.
    easy_array<uint16_t, std::shared_ptr<VertexDeclaration>, std::shared_ptr<VertexBuffer>> m_vdata;
    size_t m_vertexcount;
    //We only need only one ibo for the whole mesh
    std::shared_ptr<IndexBuffer> m_ibo;
    size_t m_indexcount;
};

} /* namespace lol */

