//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Shader class
// ----------------
//

#if !defined __LOL_GPU_SHADER_H__
#define __LOL_GPU_SHADER_H__

#include <stdint.h>

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
LOL_SAFE_ENUM(VertexUsage,
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
);

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
    Shader(String const &name, char const *vert, char const *frag);
    ~Shader();

private:
    ShaderData *data;
};

} /* namespace lol */

#endif // __LOL_GPU_SHADER_H__

