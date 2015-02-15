//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Shader class
// ----------------
//

#include <stdint.h>

#include "entity.h"

/* External declaration for LolFx files. */
#define LOLFX_RESOURCE_DECLARE(name) \
    extern "C" char const *lolfx_resource_##name
#define LOLFX_RESOURCE_HELPER(name) #name ".lolfx"
#define LOLFX_RESOURCE_NAME(name) \
    LOLFX_RESOURCE_HELPER(name), lolfx_resource_##name

namespace lol
{

/* A safe enum to indicate how a vertex stream is going to be used. For
 * now there is only TexCoord and not TexCoord0 TexCoord1 etc. because
 * we can always reorganise the vertex declaration for the indices to
 * match. If the need arises these enums will be added. */
//VertexUsageBase -------------------------------------------------------------
struct VertexUsageBase : public StructSafeEnum
{

    enum Type
    {
        Position,
        BlendWeight,
        BlendIndices,
        Normal,
        PointSize,
        TexCoord,
        TexCoordExt,
        Tangent,
        Binormal,
        TessFactor,
        PositionT,
        Color,
        Fog,
        Depth,
        Sample,
        MAX,
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[Position] = "Position";
        enum_map[BlendWeight] = "BlendWeight";
        enum_map[BlendIndices] = "BlendIndices";
        enum_map[Normal] = "Normal";
        enum_map[PointSize] = "PointSize";
        enum_map[TexCoord] = "TexCoord";
        enum_map[TexCoordExt] = "TexCoordExt";
        enum_map[Tangent] = "Tangent";
        enum_map[Binormal] = "Binormal";
        enum_map[TessFactor] = "TessFactor";
        enum_map[PositionT] = "PositionT";
        enum_map[Color] = "Color";
        enum_map[Fog] = "Fog";
        enum_map[Depth] = "Depth";
        enum_map[Sample] = "Sample";
        enum_map[MAX] = "MAX";
        return true;
    }
};
typedef SafeEnum<VertexUsageBase> VertexUsage;

//-----------------------------------------------------------------------------
//Enum definitions
//-----------------------------------------------------------------------------
//ShaderVariableBase ----------------------------------------------------------
struct ShaderVariableBase
{
    enum Type
    {
        //Main shader parameters
        Attribute = 0,
        Uniform,
        Varying,

        //Passed variables, defined as local main() variables
        InOut,

        MAX
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[Attribute] = "Attribute";
        enum_map[Uniform] = "Uniform";
        enum_map[Varying] = "Varying";
        enum_map[InOut] = "InOut";
        enum_map[MAX] = "MAX";
        return true;
    }
};
typedef SafeEnum<ShaderVariableBase> ShaderVariable;

//ShaderProgramBase -----------------------------------------------------------
struct ShaderProgramBase
{
    enum Type
    {
        Geometry = 0,
        Vertex,
        Pixel,

        MAX
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[Geometry] = "Geometry";
        enum_map[Vertex] = "Vertex";
        enum_map[Pixel] = "Pixel";
        enum_map[MAX] = "MAX";
        return true;
    }
};
typedef SafeEnum<ShaderProgramBase> ShaderProgram;

//ShaderUniform ---------------------------------------------------------------
struct ShaderUniform
{
    friend class Shader;

public:
    inline ShaderUniform() : flags(0) {}

private:
    uintptr_t frag, vert;
    /* FIXME: do we really need this to indicate which locations are valid? */
    uint32_t flags;
};

//ShaderAttrib ----------------------------------------------------------------
struct ShaderAttrib
{
    friend class Shader;
    friend class VertexDeclaration;

public:
    inline ShaderAttrib() : m_flags((uint64_t)0 - 1) {}
    inline bool IsValid() { return m_flags != (uint64_t)0 - 1; }
    inline VertexUsage GetUsage() { return VertexUsage((int)(m_flags >> 16) & 0xffff); }
    inline int GetIndex() { return (int)(m_flags & 0xffff); }

private:
    uint64_t m_flags;
};

//TextureUniform --------------------------------------------------------------
struct TextureUniform
{
    friend class Shader;
    friend class Framebuffer;
    friend class Texture;

public:
    inline TextureUniform() : m_flags(0) {}

private:
    uint64_t m_flags;
#if defined USE_D3D9 || defined _XBOX
    uint32_t m_attrib;
#endif
};

class ShaderData;

//Shader ----------------------------------------------------------------------
class Shader : public Entity
{
public:
    static Shader *Create(String const &name, String const &code);
    static void Destroy(Shader *shader);

    int GetAttribCount() const;
    ShaderAttrib GetAttribLocation(VertexUsage usage, int index) const;

    ShaderUniform GetUniformLocation(char const *uni) const;
    void SetUniform(ShaderUniform const &uni, int i);
    void SetUniform(ShaderUniform const &uni, ivec2 const &v);
    void SetUniform(ShaderUniform const &uni, ivec3 const &v);
    void SetUniform(ShaderUniform const &uni, ivec4 const &v);
    void SetUniform(ShaderUniform const &uni, float f);
    void SetUniform(ShaderUniform const &uni, vec2 const &v);
    void SetUniform(ShaderUniform const &uni, vec3 const &v);
    void SetUniform(ShaderUniform const &uni, vec4 const &v);
    void SetUniform(ShaderUniform const &uni, mat2 const &m);
    void SetUniform(ShaderUniform const &uni, mat3 const &m);
    void SetUniform(ShaderUniform const &uni, mat4 const &m);
    void SetUniform(ShaderUniform const &uni, TextureUniform tex, int index);

    void SetUniform(ShaderUniform const &uni, array<float> const &v);
    void SetUniform(ShaderUniform const &uni, array<vec2> const &v);
    void SetUniform(ShaderUniform const &uni, array<vec3> const &v);
    void SetUniform(ShaderUniform const &uni, array<vec4> const &v);

    void Bind() const;
    void Unbind() const;

protected:
    Shader(String const &name, String const &vert, String const &frag);
    ~Shader();

private:
    ShaderData *data;

public:
    static String GetVariablePrefix(const ShaderVariable variable);
    static String GetVariableQualifier(const ShaderVariable variable);
    static String GetFunctionQualifier(const ShaderVariable variable, const ShaderProgram program);
    static String GetProgramQualifier(const ShaderProgram program);
    static String GetProgramOutVariable(const ShaderProgram program);
    static String GetProgramOutVariableLocal(const ShaderProgram program);
};

//ShaderBlock -----------------------------------------------------------------
class ShaderBlock
{
    friend class ShaderBuilder;

protected:
    String m_name;

    //--------------------------
    //map : <var_name, var_type>
    //--------------------------

    //Main shader parameters
    map<String, String> m_parameters[ShaderVariable::MAX];

    //Actual code
    String m_code_main;
    String m_code_custom;

public:
    ShaderBlock(String const& name) : m_name(name) { }
    ~ShaderBlock() { }

    String const& GetName() { return m_name; }
    //Sets code that will be used in the main
    void SetMainCode(String const& code_main) { m_code_main = code_main; }
    //Sets custom code that will be put before the main -so functions-
    void SetCustomCode(String const& code_custom) { m_code_custom = code_custom; }
    //Add parameter to the block
    void Add(const ShaderVariable variable, String const& type, String const& name);

protected:
    void AddCallParameters(const ShaderVariable variable, map<String, String> const& variables, String& result);
    void AddDefinitionParameters(const ShaderVariable variable, const ShaderProgram program, map<String, String>& variables, String& result);
    void Build(const ShaderProgram program, String& call, String& function);
};

//Shaderbuilder ---------------------------------------------------------------
class ShaderBuilder
{
protected:
    String m_name;
    String m_version;
    ShaderProgram m_current_program = ShaderProgram::MAX;

    //Blocks
    array<ShaderBlock*> m_blocks[ShaderProgram::MAX];

    //Final shader parameters
    map<String, String> m_parameters[ShaderProgram::MAX][ShaderVariable::MAX];

public:
    ShaderBuilder(String const& name, String const& version);
    ~ShaderBuilder();

    String const& GetName();
    ShaderBuilder& operator<<(const ShaderProgram program);
    ShaderBuilder& operator<<(ShaderBlock* block);

protected:
    String AddSlotOutVariableLocal(const ShaderProgram program);
    void MergeParameters(map<String, String>& variables, map<String, String>& merged);

public:
    void Build(String& code);
};

} /* namespace lol */

