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
    char buf[4096], errbuf[4096];
    char const *shader = buf;
    GLsizei len;

#if !defined __CELLOS_LV2__
    /* Compile vertex shader */
    ShaderData::Patch(buf, vert);
    data->vert_crc = Hash::Crc32(buf);
    data->vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(data->vert_id, 1, &shader, NULL);
    glCompileShader(data->vert_id);

    glGetShaderInfoLog(data->vert_id, sizeof(errbuf), &len, errbuf);
    if (len > 0)
    {
        Log::Error("failed to compile vertex shader: %s", errbuf);
        Log::Error("shader source:\n%s\n", buf);
    }

    /* Compile fragment shader */
    ShaderData::Patch(buf, frag);
    data->frag_crc = Hash::Crc32(buf);
    data->frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(data->frag_id, 1, &shader, NULL);
    glCompileShader(data->frag_id);

    glGetShaderInfoLog(data->frag_id, sizeof(errbuf), &len, errbuf);
    if (len > 0)
    {
        Log::Error("failed to compile fragment shader: %s", errbuf);
        Log::Error("shader source:\n%s\n", buf);
    }

    /* Create program */
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

    strcpy(dst, src);
    if (version >= 130)
        return;

    if (version >= 110)
    {
        char *parser, *main;
        char const *end = dst + strlen(dst) + 1;

        /* Find main() */
        parser = strstr(dst, "main");
        if (!parser) return;
        parser = strstr(parser, "(");
        if (!parser) return;
        parser = strstr(parser, ")");
        if (!parser) return;
        parser = strstr(parser, "{");
        if (!parser) return;
        main = parser + 1;

        /* Perform main() replaces */
        static char const * const main_replaces[] =
        {
            "in vec2 in_Vertex;", "vec2 in_Vertex = gl_Vertex.xy;",
            "in vec3 in_Vertex;", "vec3 in_Vertex = gl_Vertex.xyz;",
            "in vec4 in_Vertex;", "vec4 in_Vertex = gl_Vertex.xyzw;",

            "in vec2 in_Color;", "vec2 in_Color = gl_Color.xy;",
            "in vec3 in_Color;", "vec3 in_Color = gl_Color.xyz;",
            "in vec4 in_Color;", "vec4 in_Color = gl_Color.xyzw;",

            "in vec2 in_MultiTexCoord0;",
               "vec2 in_MultiTexCoord0 = gl_MultiTexCoord0.xy;",
            "in vec2 in_MultiTexCoord1;",
               "vec2 in_MultiTexCoord1 = gl_MultiTexCoord1.xy;",
            "in vec2 in_MultiTexCoord2;",
               "vec2 in_MultiTexCoord2 = gl_MultiTexCoord2.xy;",
            "in vec2 in_MultiTexCoord3;",
               "vec2 in_MultiTexCoord3 = gl_MultiTexCoord3.xy;",
            "in vec2 in_MultiTexCoord4;",
               "vec2 in_MultiTexCoord4 = gl_MultiTexCoord4.xy;",
            "in vec2 in_MultiTexCoord5;",
               "vec2 in_MultiTexCoord5 = gl_MultiTexCoord5.xy;",
            "in vec2 in_MultiTexCoord6;",
               "vec2 in_MultiTexCoord6 = gl_MultiTexCoord6.xy;",
            "in vec2 in_MultiTexCoord7;",
               "vec2 in_MultiTexCoord7 = gl_MultiTexCoord7.xy;",

            NULL
        };

        for (char const * const *rep = main_replaces; rep[0]; rep += 2)
        {
            char *match = strstr(dst, rep[0]);
            if (match && match < main)
            {
                size_t l0 = strlen(rep[0]);
                size_t l1 = strlen(rep[1]);
                memmove(main + l1, main, end - main);
                memcpy(main, rep[1], l1);
                memset(match, ' ', l0);
                main += l1;
                end += l1;
            }
        }

        /* Perform small replaces */
        static char const * const fast_replaces[] =
        {
            "#version 130", "#version 120",
            "\nin vec2", "\n   vec2",
            "\nin vec3", "\n   vec3",
            "\nin vec4", "\n   vec4",
            "\nin mat4", "\n   mat4",
            NULL
        };

        for (char const * const *rep = fast_replaces; rep[0]; rep += 2)
        {
            char *match;
            while ((match = strstr(dst, rep[0])))
            {
                size_t l0 = strlen(rep[0]);
                size_t l1 = strlen(rep[1]);

                if (l1 > l0)
                    memmove(match + l1, match + l0, (end - match) - l0);
                memcpy(match, rep[1], l1);
                if (l1 < l0)
                    memset(match + l0, ' ', l1 - l0);
                end += l1 - l0;
            }
        }
    }
}

} /* namespace lol */

