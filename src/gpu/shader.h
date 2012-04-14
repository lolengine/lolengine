//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Shader class
// ----------------
//

#if !defined __LOL_SHADER_H__
#define __LOL_SHADER_H__

#include <stdint.h>

namespace lol
{

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

private:
    uint64_t m_flags;
};

class ShaderData;

class Shader
{
public:
    static Shader *Create(char const *vert, char const *frag);
    static void Destroy(Shader *shader);

    ShaderAttrib GetAttribLocation(char const *attr, struct VertexUsage usage,
                                   int index) const;

    ShaderUniform GetUniformLocation(char const *uni) const;
    void SetUniform(ShaderUniform const &uni, int i);
    void SetUniform(ShaderUniform const &uni, ivec2 const &v);
    void SetUniform(ShaderUniform const &uni, ivec3 const &v);
    void SetUniform(ShaderUniform const &uni, ivec4 const &v);
    void SetUniform(ShaderUniform const &uni, float f);
    void SetUniform(ShaderUniform const &uni, vec2 const &v);
    void SetUniform(ShaderUniform const &uni, vec3 const &v);
    void SetUniform(ShaderUniform const &uni, vec4 const &v);
    void SetUniform(ShaderUniform const &uni, mat4 const &m);

    void Bind() const;

protected:
    Shader(char const *vert, char const *frag);
    ~Shader();

private:
    ShaderData *data;
};

} /* namespace lol */

#endif // __LOL_SHADER_H__

