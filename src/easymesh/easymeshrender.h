//
// Lol Engine
//
// Copyright: (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Cédric Lecacheur <jordx@free.fr>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The EasyMesh class
// ------------------
//

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
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
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
    GpuShaderData(uint16_t vert_decl_flags, Shader* shader, DebugRenderMode render_mode);
    virtual ~GpuShaderData();
    //--
    void AddUniform(const lol::String &new_uniform);
    void AddAttribute(VertexUsage usage, int index);
    ShaderUniform const *GetUniform(const lol::String &uniform);
    ShaderAttrib const *GetAttribute(VertexUsage usage, int index);
    //--
    virtual void SetupShaderDatas(mat4 const &model) { UNUSED(model); }
    //--

protected:
    uint16_t                            m_vert_decl_flags;
    Shader*                             m_shader;
    int                                    m_render_mode;
    array<lol::String, ShaderUniform>   m_shader_uniform;
    array<ShaderAttrib>                 m_shader_attrib;
};

class DefaultShaderData : public GpuShaderData
{
public:
    //---
    DefaultShaderData(DebugRenderMode render_mode);
    DefaultShaderData(uint16_t vert_decl_flags, Shader* shader, bool with_UV);
    virtual ~DefaultShaderData() {}
    void StoreUniformNames();
    //---
    void SetupDefaultData(bool with_UV);
    virtual void SetupShaderDatas(mat4 const &model);
    //--
    array<String>   m_uniform_names;
};

class GpuEasyMeshData
{
    friend class EasyMesh;

public:
    //---
    GpuEasyMeshData();
    ~GpuEasyMeshData();
    //---
    void AddGpuData(GpuShaderData* gpudata, class EasyMesh* src_mesh);
    void RenderMeshData(mat4 const &model, int render_mode=Video::GetDebugRenderMode());
    bool HasData(int render_mode) { return (0 <= render_mode && render_mode < m_gpudatas.Count() && !!m_gpudatas[render_mode]); }

private:
    void SetupVertexData(uint16_t vdecl_flags, EasyMesh* src_mesh);

    array<GpuShaderData*>               m_gpudatas;
    //uint16_t are the vdecl/vbo flags to avoid copy same vdecl several times.
    array<uint16_t, VertexDeclaration*,
                    VertexBuffer*>      m_vdatas;
    int                                 m_vertexcount;
    //We only need only one ibo for the whole mesh
    IndexBuffer *                       m_ibo;
    int                                 m_indexcount;
};

} /* namespace lol */

