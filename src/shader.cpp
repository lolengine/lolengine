//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cmath>
#include <cstdio>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#include "core.h"
#include "lolgl.h"

namespace lol
{

/*
 * Shader implementation class
 */

class ShaderData
{
    friend class Shader;

private:
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    GLuint prog_id, vert_id, frag_id;
#endif
    uint32_t vert_crc, frag_crc;

    /* Global shader cache */
    static Shader *shaders[];
    static int nshaders;
};

Shader *ShaderData::shaders[256];
int ShaderData::nshaders = 0;

/*
 * Public Shader class
 */

Shader *Shader::Create(char const *vert, char const *frag)
{
    uint32_t new_vert_crc = Hash::Crc32(vert);
    uint32_t new_frag_crc = Hash::Crc32(frag);

    for (int n = 0; n < ShaderData::nshaders; n++)
    {
        if (ShaderData::shaders[n]->data->vert_crc == new_vert_crc
             && ShaderData::shaders[n]->data->frag_crc == new_frag_crc)
            return ShaderData::shaders[n];
    }

    Shader *ret = new Shader(vert, frag);
    ShaderData::shaders[ShaderData::nshaders] = ret;
    ShaderData::nshaders++;
    return ret;
}

void Shader::Destroy(Shader *shader)
{
    /* XXX: do nothing! the shader should remain in cache */
    (void)shader;
}

Shader::Shader(char const *vert, char const *frag)
  : data(new ShaderData())
{
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    char buf[4096];
    GLsizei len;
#endif

    data->vert_crc = Hash::Crc32(vert);
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    data->vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(data->vert_id, 1, &vert, NULL);
    glCompileShader(data->vert_id);

    glGetShaderInfoLog(data->vert_id, sizeof(buf), &len, buf);
    if (len > 0)
        Log::Error("failed to compile vertex shader: %s", buf);
#endif

    data->frag_crc = Hash::Crc32(frag);
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    data->frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(data->frag_id, 1, &frag, NULL);
    glCompileShader(data->frag_id);

    glGetShaderInfoLog(data->frag_id, sizeof(buf), &len, buf);
    if (len > 0)
        Log::Error("failed to compile fragment shader: %s", buf);

    data->prog_id = glCreateProgram();
    glAttachShader(data->prog_id, data->vert_id);
    glAttachShader(data->prog_id, data->frag_id);

    glLinkProgram(data->prog_id);
    glValidateProgram(data->prog_id);
#endif
}

int Shader::GetAttribLocation(char const *attr) const
{
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    return glGetAttribLocation(data->prog_id, attr);
#else
    (void)attr;
    return -1;
#endif
}

int Shader::GetUniformLocation(char const *uni) const
{
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    return glGetUniformLocation(data->prog_id, uni);
#else
    (void)uni;
    return -1;
#endif
}

void Shader::Bind() const
{
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    glUseProgram(data->prog_id);
#endif
}

Shader::~Shader()
{
#if defined HAVE_GL_2X || defined HAVE_GLES_2X
    glDetachShader(data->prog_id, data->vert_id);
    glDetachShader(data->prog_id, data->frag_id);
    glDeleteShader(data->vert_id);
    glDeleteShader(data->frag_id);
    glDeleteProgram(data->prog_id);
#endif
    delete data;
}

} /* namespace lol */

