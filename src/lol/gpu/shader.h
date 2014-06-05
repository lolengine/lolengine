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

namespace lol
{

/* A safe enum to indicate how a vertex stream is going to be used. For
 * now there is only TexCoord and not TexCoord0 TexCoord1 etc. because
 * we can always reorganise the vertex declaration for the indices to
 * match. If the need arises these enums will be added. */
DEF_ENUM(VertexUsage)
    DEF_VALUE
        ADD_VALUE(Position)
        ADD_VALUE(BlendWeight)
        ADD_VALUE(BlendIndices)
        ADD_VALUE(Normal)
        ADD_VALUE(PointSize)
        ADD_VALUE(TexCoord)
        ADD_VALUE(TexCoordExt)
        ADD_VALUE(Tangent)
        ADD_VALUE(Binormal)
        ADD_VALUE(TessFactor)
        ADD_VALUE(PositionT)
        ADD_VALUE(Color)
        ADD_VALUE(Fog)
        ADD_VALUE(Depth)
        ADD_VALUE(Sample)
    END_E_VALUE
    DEF_TEXT
        ADD_TEXT(Position)
        ADD_TEXT(BlendWeight)
        ADD_TEXT(BlendIndices)
        ADD_TEXT(Normal)
        ADD_TEXT(PointSize)
        ADD_TEXT(TexCoord)
        ADD_TEXT(TexCoordExt)
        ADD_TEXT(Tangent)
        ADD_TEXT(Binormal)
        ADD_TEXT(TessFactor)
        ADD_TEXT(PositionT)
        ADD_TEXT(Color)
        ADD_TEXT(Fog)
        ADD_TEXT(Depth)
        ADD_TEXT(Sample)
    END_TEXT
private:
    static String GetName(Value v, bool use_simple)
    {
        String tmp = String("");
        if (!use_simple) tmp += "<";
        tmp += VertexUsage(v).C();
        if (!use_simple) tmp += ">";
        return tmp;
    }

public:
    static String GetName(Value v)
    {
        return GetName(v, false);
    }

    static String GetNameList(uint32_t v)
    {
        String tmp = String("<");
        int nb = 0;
        for (int i = 0; i < MAX; ++i)
        {
            if (v & (1<<i))
            {
                if (nb != 0)
                    tmp += ", ";
                tmp += GetName(Value(i), true);
                ++nb;
            }
        }
        return tmp + ">";
    }
END_ENUM(VertexUsage)

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

class Shader
{
public:
    static Shader *Create(char const *lolfx);
    static void Destroy(Shader *shader);

    int GetAttribCount() const;
    ShaderAttrib GetAttribLocation(struct VertexUsage usage, int index) const;

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

    void SetUniform(ShaderUniform const &uni, Array<float> const &v);
    void SetUniform(ShaderUniform const &uni, Array<vec2> const &v);
    void SetUniform(ShaderUniform const &uni, Array<vec3> const &v);
    void SetUniform(ShaderUniform const &uni, Array<vec4> const &v);

    void Bind() const;
    void Unbind() const;

protected:
    Shader(char const *vert, char const *frag);
    ~Shader();

private:
    ShaderData *data;
};

} /* namespace lol */

#endif // __LOL_GPU_SHADER_H__

