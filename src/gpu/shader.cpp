//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
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
#   if defined USE_D3D9
#       include <algorithm>
        using std::min;
        using std::max;
#       include <d3d9.h>
#       include <d3dx9shader.h>
#   endif
#endif

#include "core.h"
#include "lolgl.h"

using namespace std;

#if defined USE_D3D9
extern IDirect3DDevice9 *g_d3ddevice;
#elif defined _XBOX
extern D3DDevice *g_d3ddevice;
#endif

namespace lol
{

/*
 * Shader implementation class
 */

class ShaderData
{
    friend class Shader;

private:
#if defined USE_D3D9
    IDirect3DVertexShader9 *vert_shader;
    IDirect3DPixelShader9 *frag_shader;
    ID3DXConstantTable *vert_table, *frag_table;
#elif defined _XBOX
    D3DVertexShader *vert_shader;
    D3DPixelShader *frag_shader;
    ID3DXConstantTable *vert_table, *frag_table;
#elif !defined __CELLOS_LV2__
    GLuint prog_id, vert_id, frag_id;
#else
    CGprogram vert_id, frag_id;
#endif
    uint32_t vert_crc, frag_crc;

    /* Shader patcher */
    static int GetVersion();
    static void Patch(char *dst, char const *vert, char const *frag);

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
#if defined USE_D3D9 || defined _XBOX
    ID3DXBuffer *shader_code, *error_msg;
    HRESULT hr;
#elif !defined __CELLOS_LV2__
    char buf[4096], errbuf[4096];
    char const *shader = buf;
    GLint status;
    GLsizei len;
#else
    /* Initialise the runtime shader compiler. FIXME: this needs only
     * to be done once. */
    cgRTCgcInit();
#endif

    /* Compile vertex shader */
    data->vert_crc = Hash::Crc32(vert);
#if defined USE_D3D9 || defined _XBOX
    hr = D3DXCompileShader(vert, (UINT)strlen(vert), NULL, NULL, "main",
                           "vs_2_0", 0, &shader_code, &error_msg,
                           &data->vert_table);
    if (FAILED(hr))
    {
        Log::Error("failed to compile vertex shader: %s",
                   error_msg ? error_msg->GetBufferPointer() : "error");
        Log::Error("shader source:\n%s\n", vert);
    }
    g_d3ddevice->CreateVertexShader((DWORD *)shader_code->GetBufferPointer(),
                                    &data->vert_shader);
    shader_code->Release();
#elif !defined __CELLOS_LV2__
    ShaderData::Patch(buf, vert, NULL);
    data->vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(data->vert_id, 1, &shader, NULL);
    glCompileShader(data->vert_id);

    glGetShaderiv(data->vert_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        glGetShaderInfoLog(data->vert_id, sizeof(errbuf), &len, errbuf);
        Log::Error("failed to compile vertex shader: %s", errbuf);
        Log::Error("shader source:\n%s\n", buf);
    }
#else
    data->vert_id = cgCreateProgram(cgCreateContext(), CG_SOURCE, vert,
                                    cgGLGetLatestProfile(CG_GL_VERTEX),
                                    NULL, NULL);
    if (data->vert_id == NULL)
    {
        Log::Error("failed to compile vertex shader");
        Log::Error("shader source:\n%s\n", vert);
    }
#endif

    /* Compile fragment shader */
    data->frag_crc = Hash::Crc32(frag);
#if defined USE_D3D9 || defined _XBOX
    hr = D3DXCompileShader(frag, (UINT)strlen(frag), NULL, NULL, "main",
                           "ps_2_0", 0, &shader_code, &error_msg,
                           &data->frag_table);
    if (FAILED(hr))
    {
        Log::Error("failed to compile fragment shader: %s",
                   error_msg ? error_msg->GetBufferPointer() : "error");
        Log::Error("shader source:\n%s\n", frag);
    }
    g_d3ddevice->CreatePixelShader((DWORD *)shader_code->GetBufferPointer(),
                                   &data->frag_shader);
    shader_code->Release();
#elif !defined __CELLOS_LV2__
    ShaderData::Patch(buf, NULL, frag);
    data->frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(data->frag_id, 1, &shader, NULL);
    glCompileShader(data->frag_id);

    glGetShaderiv(data->frag_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        glGetShaderInfoLog(data->frag_id, sizeof(errbuf), &len, errbuf);
        Log::Error("failed to compile fragment shader: %s", errbuf);
        Log::Error("shader source:\n%s\n", buf);
    }
#else
    data->frag_id = cgCreateProgram(cgCreateContext(), CG_SOURCE, frag,
                                    cgGLGetLatestProfile(CG_GL_FRAGMENT),
                                    NULL, NULL);
    if (data->frag_id == NULL)
    {
        Log::Error("failed to compile fragment shader");
        Log::Error("shader source:\n%s\n", frag);
    }
#endif

#if defined USE_D3D9 || defined _XBOX
    /* FIXME: this is only debug code, we don't need it. */
    D3DXCONSTANTTABLE_DESC desc;
    data->frag_table->GetDesc(&desc);
    for (int i = 0; i < desc.Constants; i++)
    {
        D3DXCONSTANT_DESC cdesc;
        UINT count = 1;
        D3DXHANDLE h = data->frag_table->GetConstant(NULL, i);
        data->frag_table->GetConstantDesc(h, &cdesc, &count);
    }
    data->vert_table->GetDesc(&desc);
    for (int i = 0; i < desc.Constants; i++)
    {
        D3DXCONSTANT_DESC cdesc;
        UINT count = 1;
        D3DXHANDLE h = data->vert_table->GetConstant(NULL, i);
        data->frag_table->GetConstantDesc(h, &cdesc, &count);
    }
#elif !defined __CELLOS_LV2__
    /* Create program */
    data->prog_id = glCreateProgram();
    glAttachShader(data->prog_id, data->vert_id);
    glAttachShader(data->prog_id, data->frag_id);

    glLinkProgram(data->prog_id);
    glValidateProgram(data->prog_id);
#endif
}

ShaderAttrib Shader::GetAttribLocation(char const *attr,
                                       VertexUsage usage, int index) const
{
    ShaderAttrib ret;
#if defined USE_D3D9 || defined _XBOX
    ret.m_flags = (uint32_t)index << 16;
    ret.m_flags |= (uint32_t)usage;
#elif !defined __CELLOS_LV2__
    ret.m_flags = glGetAttribLocation(data->prog_id, attr);
#else
    /* FIXME: can we do this at all? */
#endif
    return ret;
}

ShaderUniform Shader::GetUniformLocation(char const *uni) const
{
    ShaderUniform ret;
#if defined USE_D3D9 || defined _XBOX
    /* Global variables are prefixed with "$" */
    char tmpname[128];
    sprintf(tmpname, "$%s", uni);
    D3DXCONSTANT_DESC cdesc;
    D3DXHANDLE hr;
    UINT count;

    count = 0;
    hr = data->frag_table->GetConstantByName(NULL, tmpname);
    if (hr)
        data->frag_table->GetConstantDesc(hr, &cdesc, &count);
    if (count)
    {
        ret.frag = cdesc.RegisterIndex;
        ret.flags |= 1;
    }

    count = 0;
    hr = data->vert_table->GetConstantByName(NULL, tmpname);
    if (hr)
        data->vert_table->GetConstantDesc(hr, &cdesc, &count);
    if (count)
    {
        ret.vert = cdesc.RegisterIndex;
        ret.flags |= 2;
    }
#elif !defined __CELLOS_LV2__
    ret.frag = (uintptr_t)glGetUniformLocation(data->prog_id, uni);
    ret.vert = 0;
#else
    ret.frag = (uintptr_t)cgGetNamedParameter(data->frag_id, uni);
    ret.vert = (uintptr_t)cgGetNamedParameter(data->vert_id, uni);
#endif
    return ret;
}

void Shader::SetUniform(ShaderUniform const &uni, int i)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, ivec4(i, 0, 0, 0));
#elif !defined __CELLOS_LV2__
    glUniform1i(uni.frag, i);
#else
    /* FIXME: does this exist at all? */
    //cgGLSetParameter1i((CGparameter)uni.frag, i);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, ivec2 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, ivec4(v, 0, 0));
#elif !defined __CELLOS_LV2__
    glUniform2i(uni.frag, v.x, v.y);
#else
    /* FIXME: does this exist at all? */
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, ivec3 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, ivec4(v, 0));
#elif !defined __CELLOS_LV2__
    glUniform3i(uni.frag, v.x, v.y, v.z);
#else
    /* FIXME: does this exist at all? */
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, ivec4 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        g_d3ddevice->SetPixelShaderConstantI((UINT)uni.frag, &v[0], 1);
    if (uni.flags & 2)
        g_d3ddevice->SetVertexShaderConstantI((UINT)uni.vert, &v[0], 1);
#elif !defined __CELLOS_LV2__
    glUniform4i(uni.frag, v.x, v.y, v.z, v.w);
#else
    /* FIXME: does this exist at all? */
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, float f)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, vec4(f, 0, 0, 0));
#elif !defined __CELLOS_LV2__
    glUniform1f(uni.frag, f);
#else
    if (uni.frag)
        cgGLSetParameter1f((CGparameter)uni.frag, f);
    if (uni.vert)
        cgGLSetParameter1f((CGparameter)uni.vert, f);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, vec2 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, vec4(v, 0, 0));
#elif !defined __CELLOS_LV2__
    glUniform2f(uni.frag, v.x, v.y);
#else
    if (uni.frag)
        cgGLSetParameter2f((CGparameter)uni.frag, v.x, v.y);
    if (uni.vert)
        cgGLSetParameter2f((CGparameter)uni.vert, v.x, v.y);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, vec3 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, vec4(v, 0));
#elif !defined __CELLOS_LV2__
    glUniform3f(uni.frag, v.x, v.y, v.z);
#else
    if (uni.frag)
        cgGLSetParameter3f((CGparameter)uni.frag, v.x, v.y, v.z);
    if (uni.vert)
        cgGLSetParameter3f((CGparameter)uni.vert, v.x, v.y, v.z);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, vec4 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        g_d3ddevice->SetPixelShaderConstantF((UINT)uni.frag, &v[0], 1);
    if (uni.flags & 2)
        g_d3ddevice->SetVertexShaderConstantF((UINT)uni.vert, &v[0], 1);
#elif !defined __CELLOS_LV2__
    glUniform4f(uni.frag, v.x, v.y, v.z, v.w);
#else
    /* FIXME: use the array versions of these functions */
    if (uni.frag)
        cgGLSetParameter4f((CGparameter)uni.frag, v.x, v.y, v.z, v.w);
    if (uni.vert)
        cgGLSetParameter4f((CGparameter)uni.vert, v.x, v.y, v.z, v.w);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, mat4 const &m)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        g_d3ddevice->SetPixelShaderConstantF((UINT)uni.frag, &m[0][0], 4);
    if (uni.flags & 2)
        g_d3ddevice->SetVertexShaderConstantF((UINT)uni.vert, &m[0][0], 4);
#elif !defined __CELLOS_LV2__
    glUniformMatrix4fv(uni.frag, 1, GL_FALSE, &m[0][0]);
#else
    if (uni.frag)
        cgGLSetMatrixParameterfc((CGparameter)uni.frag, &m[0][0]);
    if (uni.vert)
        cgGLSetMatrixParameterfc((CGparameter)uni.vert, &m[0][0]);
#endif
}

void Shader::Bind() const
{
#if defined USE_D3D9 || defined _XBOX
    HRESULT hr;
    hr = g_d3ddevice->SetVertexShader(data->vert_shader);
    hr = g_d3ddevice->SetPixelShader(data->frag_shader);
#elif !defined __CELLOS_LV2__
    glUseProgram(data->prog_id);
#else
    cgGLEnableProfile(cgGLGetLatestProfile(CG_GL_VERTEX));
    cgGLBindProgram(data->vert_id);
    cgGLEnableProfile(cgGLGetLatestProfile(CG_GL_FRAGMENT));
    cgGLBindProgram(data->frag_id);
#endif
}

Shader::~Shader()
{
#if defined USE_D3D9 || defined _XBOX
    data->vert_shader->Release();
    data->vert_table->Release();
    data->frag_shader->Release();
    data->frag_table->Release();
#elif !defined __CELLOS_LV2__
    glDetachShader(data->prog_id, data->vert_id);
    glDetachShader(data->prog_id, data->frag_id);
    glDeleteShader(data->vert_id);
    glDeleteShader(data->frag_id);
    glDeleteProgram(data->prog_id);
#else
    cgDestroyProgram(data->vert_id);
    cgDestroyProgram(data->frag_id);
#endif
    delete data;
}

/* Try to detect shader compiler features */
int ShaderData::GetVersion()
{
    static int version = 0;

#if !defined USE_D3D9 && !defined _XBOX && !defined __CELLOS_LV2__
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
#endif

    return version;
}

/* Simple shader source patching for old GLSL versions.
 * If supported version is 1.30, do nothing.
 * If supported version is 1.20:
 *  - replace "#version 130" with "#version 120"
 */
void ShaderData::Patch(char *dst, char const *vert, char const *frag)
{
    int ver_driver = GetVersion();

    strcpy(dst, vert ? vert : frag);
    if (ver_driver >= 130)
        return;

    int ver_shader = 110;
    char *parser = strstr(dst, "#version");
    if (parser)
        ver_shader = atoi(parser + strlen("#version"));

    if (ver_shader > 120 && ver_driver <= 120)
    {
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
        char *main = parser + 1;

        /* Perform main() replaces */
        char const * const main_replaces[] =
        {
#if 0
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
#endif

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
        char const * const fast_replaces[] =
        {
            "#version 130", "#version 120",
            "in vec2", vert ? "attribute vec2" : "varying vec2",
            "in vec3", vert ? "attribute vec3" : "varying vec3",
            "in vec4", vert ? "attribute vec4" : "varying vec4",
            "in mat4", vert ? "attribute mat4" : "varying mat4",
            "out vec2", "varying vec2",
            "out vec3", "varying vec3",
            "out vec4", "varying vec4",
            "out mat4", "varying mat4",
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

