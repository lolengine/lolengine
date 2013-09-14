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
struct VertexUsage
{
    enum Value
    {
        Position = 0,
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

        Max
    }
    m_value;

private:
    static String GetName(Value v, bool use_simple)
    {
        String tmp = String("");
        if (!use_simple) tmp += "<";
        switch (v)
        {
            case Position:      { tmp += "Position";    break; }
            case BlendWeight:   { tmp += "BlendWeight"; break; }
            case BlendIndices:  { tmp += "BlendIndices";break; }
            case Normal:        { tmp += "Normal";      break; }
            case PointSize:     { tmp += "PointSize";   break; }
            case TexCoord:      { tmp += "TexCoord";    break; }
            case TexCoordExt:   { tmp += "TexCoordExt"; break; }
            case Tangent:       { tmp += "Tangent";     break; }
            case Binormal:      { tmp += "Binormal";    break; }
            case TessFactor:    { tmp += "TessFactor";  break; }
            case PositionT:     { tmp += "PositionT";   break; }
            case Color:         { tmp += "Color";       break; }
            case Fog:           { tmp += "Fog";         break; }
            case Depth:         { tmp += "Depth";       break; }
            case Sample:        { tmp += "Sample";      break; }
            default:            { tmp += "UNDEFINED";   break; }
        }
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
        for (int i = 0; i < Max; ++i)
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

    inline VertexUsage(Value v) : m_value(v) {}
    inline VertexUsage(int v) : m_value((Value)v) {}
    inline operator Value() { return m_value; }
};

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

struct ShaderTexture
{
    friend class Shader;
    friend class Framebuffer;
    friend class Texture;

public:
    inline ShaderTexture() : m_flags(0) {}

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
    void SetUniform(ShaderUniform const &uni, ShaderTexture tex, int index);

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

