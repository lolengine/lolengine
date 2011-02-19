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
#if defined __APPLE__ && defined __MACH__
#   include <OpenGL/gl.h>
#else
#   define GL_GLEXT_PROTOTYPES
#   include <GL/gl.h>
#endif

#include "core.h"

/*
 * Shader implementation class
 */

class ShaderData
{
    friend class Shader;

private:
    GLuint prog_id, vert_id, frag_id;
};

/*
 * Public Shader class
 */

Shader *Shader::Create(char const *vert, char const *frag)
{
    return new Shader(vert, frag);
}

void Shader::Destroy(Shader *shader)
{
    delete shader;
}

Shader::Shader(char const *vert, char const *frag)
  : data(new ShaderData())
{
    char buf[4096];
    GLsizei len;

    data->vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(data->vert_id, 1, &vert, NULL);
    glCompileShader(data->vert_id);

    glGetShaderInfoLog(data->vert_id, sizeof(buf), &len, buf);
    if (len > 0)
        fprintf(stderr, "ERROR: failed to compile vertex shader: %s", buf);

    data->frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(data->frag_id, 1, &frag, NULL);
    glCompileShader(data->frag_id);

    glGetShaderInfoLog(data->frag_id, sizeof(buf), &len, buf);
    if (len > 0)
        fprintf(stderr, "ERROR: failed to compile fragment shader: %s", buf);

    data->prog_id = glCreateProgram();
    glAttachShader(data->prog_id, data->vert_id);
    glAttachShader(data->prog_id, data->frag_id);

    glLinkProgram(data->prog_id);
    glValidateProgram(data->prog_id);
}

int Shader::GetAttribLocation(char const *attr) const
{
    return glGetAttribLocation(data->prog_id, attr);
}

int Shader::GetUniformLocation(char const *uni) const
{
    return glGetUniformLocation(data->prog_id, uni);
}

void Shader::Bind() const
{
    glUseProgram(data->prog_id);
}

Shader::~Shader()
{
    glDetachShader(data->prog_id, data->vert_id);
    glDetachShader(data->prog_id, data->frag_id);
    glDeleteShader(data->vert_id);
    glDeleteShader(data->frag_id);
    glDeleteProgram(data->prog_id);
    delete data;
}

