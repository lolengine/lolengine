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
#include <cstring>
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
    GLuint prog_id, vert_id, frag_id;
    uint32_t vert_crc, frag_crc;

    /* Shader patcher */
    static int GetVersion();
    static void Patch(char *dst, char const *src);

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
    char buf[4096];
    char const *shader = buf;
    GLsizei len;

#if !defined __CELLOS_LV2__

    /* Compile vertex shader and fragment shader */
    ShaderData::Patch(buf, vert);
    data->vert_crc = Hash::Crc32(buf);
    data->vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(data->vert_id, 1, &shader, NULL);
    glCompileShader(data->vert_id);

    glGetShaderInfoLog(data->vert_id, sizeof(buf), &len, buf);
    if (len > 0)
        Log::Error("failed to compile vertex shader: %s", buf);

    ShaderData::Patch(buf, frag);
    data->frag_crc = Hash::Crc32(buf);
    data->frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(data->frag_id, 1, &shader, NULL);
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
#if defined __CELLOS_LV2__
    return 0;
#else
    return glGetAttribLocation(data->prog_id, attr);
#endif
}

int Shader::GetUniformLocation(char const *uni) const
{
#if defined __CELLOS_LV2__
    return 0;
#else
    return glGetUniformLocation(data->prog_id, uni);
#endif
}

void Shader::Bind() const
{
#if defined __CELLOS_LV2__
    ;
#else
    glUseProgram(data->prog_id);
#endif
}

Shader::~Shader()
{
#if defined __CELLOS_LV2__
    ;
#else
    glDetachShader(data->prog_id, data->vert_id);
    glDetachShader(data->prog_id, data->frag_id);
    glDeleteShader(data->vert_id);
    glDeleteShader(data->frag_id);
    glDeleteProgram(data->prog_id);
    delete data;
#endif
}

/* Try to detect shader compiler features */
int ShaderData::GetVersion()
{
    static int version = 0;

    if (!version)
    {
        char buf[4096];
        GLsizei len;

        int id = glCreateShader(GL_VERTEX_SHADER);

        /* Can we compile 1.30 shaders? */
        char const *test130 =
            "#version 130\n"
            "void main() { gl_Position = vec4(0.0, 0.0, 0.0, 0.0); }";
        glShaderSource(id, 1, &test130, NULL);
        glCompileShader(id);
        glGetShaderInfoLog(id, sizeof(buf), &len, buf);
        if (len <= 0)
            version = 130;

        /* If not, can we compile 1.20 shaders? */
        if (!version)
        {
            char const *test120 =
                "#version 120\n"
                "void main() { gl_Position = vec4(0.0, 0.0, 0.0, 0.0); }";
            glShaderSource(id, 1, &test120, NULL);
            glCompileShader(id);
            glGetShaderInfoLog(id, sizeof(buf), &len, buf);
            if (len <= 0)
                version = 120;
        }

        /* Otherwise, assume we can compile 1.10 shaders. */
        if (!version)
            version = 110;

        glDeleteShader(id);
    }

    return version;
}

/* Simple shader source patching for old GLSL versions.
 * If supported version is 1.30, do nothing.
 * If supported version is 1.20:
 *  - replace "#version 130" with "#version 120"
 */
void ShaderData::Patch(char *dst, char const *src)
{
    int version = GetVersion();

    if (version >= 130)
    {
        strcpy(dst, src);
        return;
    }

    if (version == 120)
    {
        static char const * const replaces[] =
        {
            "#version 130", "#version 120",
            "\nin vec2", "\nvec2",
            "\nin vec3", "\nvec3",
            "\nin vec4", "\nvec4",
            "\nin mat4", "\nmat4",
            NULL
        };

        while (*src)
        {
            for (char const * const *rep = replaces; rep[0]; rep += 2)
            {
                size_t l0 = strlen(rep[0]);
                if (!strncmp(src, rep[0], l0))
                {
                    src += l0;
                    dst += sprintf(dst, "%s", rep[1]);
                    goto next;
                }
            }

            *dst++ = *src++;

            next: continue;
        }
        *dst = '\0';
    }
}

} /* namespace lol */

