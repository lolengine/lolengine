//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>
#include <cstdio>

#if defined(_WIN32) && !defined(_XBOX)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   if defined USE_D3D9
#       include <algorithm>
        using std::min;
        using std::max;
#       include <d3d9.h>
#       include <d3dx9shader.h>
#   endif
#elif defined _XBOX
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

#include "core.h"
#include "lolgl.h"

using namespace std;

namespace lol
{

struct ShaderType
{
    enum Value
    {
        Vertex = 1,
        Fragment,
        Geometry,
        TessControl,
        TessEval,
    }
    m_value;

    inline ShaderType(Value v) : m_value(v) {}
    inline ShaderType(int v) : m_value((Value)v) {}
    inline operator Value() { return m_value; }
};

static const char* attribute_names[] =
{
    "in_Position",
    "in_BlendWeight",
    "in_BlendIndices",
    "in_Normal",
    "in_PointSize",
    "in_TexCoord",
    "in_TexCoordExt",
    "in_Tangent",
    "in_Binormal",
    "in_TessFactor",
    "in_PositionT",
    "in_Color",
    "in_Fog",
    "in_Depth",
    "in_Sample"
};

/*
 * Shader implementation class
 */

class ShaderData
{
    friend class Shader;

private:
    String m_name;

#if defined USE_D3D9
    IDirect3DDevice9 *m_dev;
    IDirect3DVertexShader9 *vert_shader;
    IDirect3DPixelShader9 *frag_shader;
    ID3DXConstantTable *vert_table, *frag_table;
#elif defined _XBOX
    D3DDevice *m_dev;
    D3DVertexShader *vert_shader;
    D3DPixelShader *frag_shader;
    ID3DXConstantTable *vert_table, *frag_table;
#elif !defined __CELLOS_LV2__
    GLuint prog_id, vert_id, frag_id;
    // Benlitz: using a simple array could be faster since there is never more than a few attribute locations to store
    Map<uint64_t, GLint> attrib_locations;
    Map<uint64_t, bool> attrib_errors;
#else
    CGprogram vert_id, frag_id;
#endif
    uint32_t vert_crc, frag_crc;

    /* Shader patcher */
    static int GetVersion();
    static String Patch(String const &code, ShaderType type);

    /* Global shader cache */
    static Shader *shaders[];
    static Hash<char const *> hash;
    static int nshaders;
};

Shader *ShaderData::shaders[256];
Hash<char const *> ShaderData::hash;
int ShaderData::nshaders = 0;

/*
 * Public Shader class
 */

Shader *Shader::Create(String const &name, String const &code)
{
    String src = String("\n") + code;

    /* Parse the crap */
    Array<char const *, char const *> sections;
    char *key = nullptr;
    for (char *parser = src.C(); *parser; )
    {
        if (key == nullptr && (parser[0] == '\n' || parser[0] == '\r')
             && parser[1] == '[')
        {
            *parser = '\0';
            parser += 2;
            key = parser;
        }
        else if (key && parser[0] == ']')
        {
            *parser++ = '\0';
        }
        else if (key && (parser[0] == '\n' || parser[0] == '\r'))
        {
            sections.Push(key, parser);
            parser++;
            key = nullptr;
        }
        else
        {
            parser++;
        }
    }

    char const *vert = nullptr, *frag = nullptr;
    for (int i = 0; i < sections.Count(); i++)
    {
#if !defined __CELLOS_LV2__ && !defined _XBOX && !defined USE_D3D9
        if (!strcmp(sections[i].m1, "vert.glsl"))
            vert = sections[i].m2;
        if (!strcmp(sections[i].m1, "frag.glsl"))
            frag = sections[i].m2;
#else
        if (!strcmp(sections[i].m1, "vert.hlsl"))
            vert = sections[i].m2;
        if (!strcmp(sections[i].m1, "frag.hlsl"))
            frag = sections[i].m2;
#endif
    }

    /* FIXME: we don’t know how to handle these yet. */
    if (!vert)
        Log::Error("no vertex shader found in %s… sorry, I’m gonna crash now.\n",
                   name.C());
    if (!frag)
        Log::Error("no fragment shader found in %s… sorry, I’m gonna crash now.\n",
                   name.C());

    uint32_t new_vert_crc = ShaderData::hash(vert);
    uint32_t new_frag_crc = ShaderData::hash(frag);

    for (int n = 0; n < ShaderData::nshaders; n++)
    {
        if (ShaderData::shaders[n]->data->vert_crc == new_vert_crc
             && ShaderData::shaders[n]->data->frag_crc == new_frag_crc)
        {
            return ShaderData::shaders[n];
        }
    }

    Shader *ret = new Shader(name, vert, frag);
    ShaderData::shaders[ShaderData::nshaders] = ret;
    ShaderData::nshaders++;

    return ret;
}

void Shader::Destroy(Shader *shader)
{
    /* XXX: do nothing! the shader should remain in cache */
    UNUSED(shader);
}

Shader::Shader(String const &name,
               char const *vert, char const *frag)
  : data(new ShaderData())
{
    data->m_name = name;

#if defined USE_D3D9 || defined _XBOX
    ID3DXBuffer *shader_code, *error_msg;
    HRESULT hr;
    D3DXMACRO macros[] =
    {
#if defined _XBOX
        { "_XBOX", "1" },
#endif
        { nullptr, nullptr }
    };
#elif !defined __CELLOS_LV2__
    char errbuf[4096];
    String shader_code;
    GLchar const *gl_code;
    GLint status;
    GLsizei len;
#else
    /* Initialise the runtime shader compiler. FIXME: this needs only
     * to be done once. */
    cgRTCgcInit();
#endif

    /* Compile vertex shader */
    data->vert_crc = ShaderData::hash(vert);
#if defined USE_D3D9 || defined _XBOX
#   if defined USE_D3D9
    data->m_dev = (IDirect3DDevice9 *)g_renderer->GetDevice();
#   elif defined _XBOX
    data->m_dev = (D3DDevice *)g_renderer->GetDevice();
#   endif

    hr = D3DXCompileShader(vert, (UINT)strlen(vert), macros, nullptr, "main",
                           "vs_3_0", 0, &shader_code, &error_msg,
                           &data->vert_table);
    if (FAILED(hr))
    {
        Log::Error("failed to compile vertex shader %s: %s\n", name.C(),
                   error_msg ? error_msg->GetBufferPointer() : "error");
        Log::Error("shader source:\n%s\n", vert);
    }
    data->m_dev->CreateVertexShader((DWORD *)shader_code->GetBufferPointer(),
                                    &data->vert_shader);
    shader_code->Release();
#elif !defined __CELLOS_LV2__
    shader_code = ShaderData::Patch(vert, ShaderType::Vertex);
    data->vert_id = glCreateShader(GL_VERTEX_SHADER);
    gl_code = shader_code.C();
    glShaderSource(data->vert_id, 1, &gl_code, nullptr);
    glCompileShader(data->vert_id);

    glGetShaderInfoLog(data->vert_id, sizeof(errbuf), &len, errbuf);
    glGetShaderiv(data->vert_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        Log::Error("failed to compile vertex shader %s: %s\n",
                   name.C(), errbuf);
        Log::Error("shader source:\n%s\n", shader_code.C());
    }
    else if (len > 16)
    {
        Log::Debug("compile log for vertex shader %s: %s\n", name.C(), errbuf);
        Log::Debug("shader source:\n%s\n", shader_code.C());
    }
#else
    data->vert_id = cgCreateProgram(cgCreateContext(), CG_SOURCE, vert,
                                    cgGLGetLatestProfile(CG_GL_VERTEX),
                                    nullptr, nullptr);
    if (data->vert_id == nullptr)
    {
        Log::Error("failed to compile vertex shader %s\n", name.C());
        Log::Error("shader source:\n%s\n", vert);
    }
#endif

    /* Compile fragment shader */
    data->frag_crc = ShaderData::hash(frag);
#if defined USE_D3D9 || defined _XBOX
    hr = D3DXCompileShader(frag, (UINT)strlen(frag), macros, nullptr, "main",
                           "ps_3_0", 0, &shader_code, &error_msg,
                           &data->frag_table);
    if (FAILED(hr))
    {
        Log::Error("failed to compile fragment shader %s: %s\n", name.C(),
                   error_msg ? error_msg->GetBufferPointer() : "error");
        Log::Error("shader source:\n%s\n", frag);
    }
    data->m_dev->CreatePixelShader((DWORD *)shader_code->GetBufferPointer(),
                                   &data->frag_shader);
    shader_code->Release();
#elif !defined __CELLOS_LV2__
    shader_code = ShaderData::Patch(frag, ShaderType::Fragment);
    data->frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    gl_code = shader_code.C();
    glShaderSource(data->frag_id, 1, &gl_code, nullptr);
    glCompileShader(data->frag_id);

    glGetShaderInfoLog(data->frag_id, sizeof(errbuf), &len, errbuf);
    glGetShaderiv(data->frag_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        Log::Error("failed to compile fragment shader %s: %s\n",
                   name.C(), errbuf);
        Log::Error("shader source:\n%s\n", shader_code.C());
    }
    else if (len > 16)
    {
        Log::Debug("compile log for fragment shader %s: %s\n",
                   name.C(), errbuf);
        Log::Debug("shader source:\n%s\n", shader_code.C());
    }
#else
    data->frag_id = cgCreateProgram(cgCreateContext(), CG_SOURCE, frag,
                                    cgGLGetLatestProfile(CG_GL_FRAGMENT),
                                    nullptr, nullptr);
    if (data->frag_id == nullptr)
    {
        Log::Error("failed to compile fragment shader %s\n", name.C());
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
        D3DXHANDLE h = data->frag_table->GetConstant(nullptr, i);
        data->frag_table->GetConstantDesc(h, &cdesc, &count);
    }
    data->vert_table->GetDesc(&desc);
    for (int i = 0; i < desc.Constants; i++)
    {
        D3DXCONSTANT_DESC cdesc;
        UINT count = 1;
        D3DXHANDLE h = data->vert_table->GetConstant(nullptr, i);
        data->frag_table->GetConstantDesc(h, &cdesc, &count);
    }
#elif !defined __CELLOS_LV2__
    /* Create program */
    data->prog_id = glCreateProgram();
    glAttachShader(data->prog_id, data->vert_id);
    glAttachShader(data->prog_id, data->frag_id);

    glLinkProgram(data->prog_id);
    glGetProgramInfoLog(data->prog_id, sizeof(errbuf), &len, errbuf);
    glGetProgramiv(data->prog_id, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        Log::Error("failed to link program %s: %s\n", name.C(), errbuf);
    }
    else if (len > 16)
    {
        Log::Debug("link log for program %s: %s\n", name.C(), errbuf);
    }

    GLint validated;
    glValidateProgram(data->prog_id);
    glGetProgramiv(data->prog_id, GL_VALIDATE_STATUS, &validated);
    if (validated != GL_TRUE)
    {
        Log::Error("failed to validate program %s\n", name.C());
    }

    GLint num_attribs;
    glGetProgramiv(data->prog_id, GL_ACTIVE_ATTRIBUTES, &num_attribs);

#if EMSCRIPTEN //WebGL doesn't support GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, so chose a default size value.
    GLint max_len = 256;
#else
    GLint max_len;
    glGetProgramiv(data->prog_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_len);
#endif

    char* name_buffer = new char[max_len];
    for (int i = 0; i < num_attribs; ++i)
    {
        int attrib_len;
        int attrib_size;
        int attrib_type;
        glGetActiveAttrib(data->prog_id, i, max_len, &attrib_len, (GLint*)&attrib_size, (GLenum*)&attrib_type, name_buffer);

        String name(name_buffer);
        int index = -1;
        VertexUsage usage = VertexUsage::MAX;
        for (int j = 0; j < (int)VertexUsage::MAX; ++j)
        {
            if (name.StartsWith(attribute_names[j]))
            {
                usage = VertexUsage(j);
                char* idx_ptr = name.C() + strlen(attribute_names[j]);
                index = strtol(idx_ptr, nullptr, 10);
                break;
            }
        }

        if (usage == VertexUsage::MAX || index == -1)
        {
            Log::Error("unable to parse attribute semantic from name: %s\n",
                       name_buffer);
        }
        else
        {
            GLint location = glGetAttribLocation(data->prog_id, name_buffer);
            uint64_t flags = (uint64_t)(uint16_t)usage.ToScalar() << 16;
            flags |= (uint64_t)(uint16_t)index;
            // TODO: this is here just in case. Remove this once everything has been correctly tested
#if _DEBUG
            if (data->attrib_locations.HasKey(flags))
            {
                Log::Error("error while parsing attribute semantics in %s\n",
                           name.C());
            }
#endif
            data->attrib_locations[flags] = location;
        }
    }

    delete[] name_buffer;
#endif
}

int Shader::GetAttribCount() const
{
#if !defined __CELLOS_LV2__
    return data->attrib_locations.Count();
#else
    // TODO
    return 0;
#endif
}

ShaderAttrib Shader::GetAttribLocation(VertexUsage usage, int index) const
{
    ShaderAttrib ret;
    ret.m_flags = (uint64_t)(uint16_t)usage.ToScalar() << 16;
    ret.m_flags |= (uint64_t)(uint16_t)index;
#if defined USE_D3D9 || defined _XBOX
#elif !defined __CELLOS_LV2__
    GLint l = -1;

    if (!data->attrib_locations.TryGetValue(ret.m_flags, l))
    {
        /* Only spit an error once, we don’t need to flood the console. */
        if (!data->attrib_errors.HasKey(ret.m_flags))
        {
            Log::Error("attribute %s not found in shader %s\n",
                       usage.ToString().C(), data->m_name.C());
            data->attrib_errors[ret.m_flags] = true;
        }
    }
    ret.m_flags |= (uint64_t)(uint32_t)l << 32;
#else
    /* FIXME: can we do this at all on the PS3? */
#endif
    return ret;
}

ShaderUniform Shader::GetUniformLocation(char const *uni) const
{
    ShaderUniform ret;
#if defined USE_D3D9 || defined _XBOX
    /* Global variables are prefixed with "$" */
    String tmpname = String("$") + uni;
    D3DXCONSTANT_DESC cdesc;
    D3DXHANDLE hr;
    UINT count;

    count = 0;
    hr = data->frag_table->GetConstantByName(nullptr, tmpname.C());
    if (hr)
        data->frag_table->GetConstantDesc(hr, &cdesc, &count);
    if (count)
    {
        ret.frag = cdesc.RegisterIndex;
        ret.flags |= 1;
    }

    count = 0;
    hr = data->vert_table->GetConstantByName(nullptr, tmpname.C());
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

/*
 * Uniform setters for scalars
 */

void Shader::SetUniform(ShaderUniform const &uni, int i)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, ivec4(i, 0, 0, 0));
#elif !defined __CELLOS_LV2__
    glUniform1i(uni.frag, i);
#else
    /* FIXME: does this exist at all? cgGLSetParameter1i doesn't. */
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
        data->m_dev->SetPixelShaderConstantI((UINT)uni.frag, &v[0], 1);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantI((UINT)uni.vert, &v[0], 1);
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
    glUniform2fv(uni.frag, 1, &v[0]);
#else
    if (uni.frag)
        cgGLSetParameter2fv((CGparameter)uni.frag, &v[0]);
    if (uni.vert)
        cgGLSetParameter2fv((CGparameter)uni.vert, &v[0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, vec3 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, vec4(v, 0));
#elif !defined __CELLOS_LV2__
    glUniform3fv(uni.frag, 1, &v[0]);
#else
    if (uni.frag)
        cgGLSetParameter3fv((CGparameter)uni.frag, &v[0]);
    if (uni.vert)
        cgGLSetParameter3fv((CGparameter)uni.vert, &v[0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, vec4 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag, &v[0], 1);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert, &v[0], 1);
#elif !defined __CELLOS_LV2__
    glUniform4fv(uni.frag, 1, &v[0]);
#else
    if (uni.frag)
        cgGLSetParameter4fv((CGparameter)uni.frag, &v[0]);
    if (uni.vert)
        cgGLSetParameter4fv((CGparameter)uni.vert, &v[0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, mat2 const &m)
{
#if defined USE_D3D9 || defined _XBOX
    /* FIXME: do we need padding here like for the mat3 version? */
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag, &m[0][0], 1);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert, &m[0][0], 1);
#elif !defined __CELLOS_LV2__
    glUniformMatrix2fv(uni.frag, 1, GL_FALSE, &m[0][0]);
#else
    mat4 tmp(m, 1.0f, 1.0f);
    if (uni.frag)
        cgGLSetMatrixParameterfc((CGparameter)uni.frag, &m[0][0]);
    if (uni.vert)
        cgGLSetMatrixParameterfc((CGparameter)uni.vert, &m[0][0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, mat3 const &m)
{
#if defined USE_D3D9 || defined _XBOX
    /* Padding matrix columns is necessary on DirectX. We need to create
     * a new data structure; a 4×4 matrix will do. */
    mat4 tmp(m, 1.0f);
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag, &tmp[0][0], 3);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert, &tmp[0][0], 3);
#elif !defined __CELLOS_LV2__
    glUniformMatrix3fv(uni.frag, 1, GL_FALSE, &m[0][0]);
#else
    /* FIXME: check it's the proper way to do this */
    mat4 tmp(m, 1.0f);
    if (uni.frag)
        cgGLSetMatrixParameterfc((CGparameter)uni.frag, &m[0][0]);
    if (uni.vert)
        cgGLSetMatrixParameterfc((CGparameter)uni.vert, &m[0][0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, mat4 const &m)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag, &m[0][0], 4);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert, &m[0][0], 4);
#elif !defined __CELLOS_LV2__
    glUniformMatrix4fv(uni.frag, 1, GL_FALSE, &m[0][0]);
#else
    if (uni.frag)
        cgGLSetMatrixParameterfc((CGparameter)uni.frag, &m[0][0]);
    if (uni.vert)
        cgGLSetMatrixParameterfc((CGparameter)uni.vert, &m[0][0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, TextureUniform tex, int index)
{
#if defined USE_D3D9 || defined _XBOX
    data->m_dev->SetTexture(index, (LPDIRECT3DTEXTURE9)tex.m_flags);
    data->m_dev->SetSamplerState(index, D3DSAMP_MAGFILTER, tex.m_attrib & 0xff);
    data->m_dev->SetSamplerState(index, D3DSAMP_MINFILTER, (tex.m_attrib >> 8) & 0xff);
    data->m_dev->SetSamplerState(index, D3DSAMP_MIPFILTER, (tex.m_attrib >> 16) & 0xff);
#elif !defined __CELLOS_LV2__
    glActiveTexture(GL_TEXTURE0 + index);
    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (int)tex.m_flags);
    SetUniform(uni, index);
#else
    /* FIXME: unimplemented */
#endif
}

/*
 * Uniform setters for arrays
 */

void Shader::SetUniform(ShaderUniform const &uni, Array<float> const &v)
{
#if defined USE_D3D9 || defined _XBOX
    /* FIXME: this will not work properly because we don't know how tell DX9
     * it's a bunch of floats instead of vec4. */
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag,
                                             &v[0], v.Count() / 4);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert,
                                              &v[0], v.Count() / 4);
#elif !defined __CELLOS_LV2__
    glUniform1fv(uni.frag, v.Count(), &v[0]);
#else
    if (uni.frag)
        cgGLSetParameterArray1f((CGparameter)uni.frag,
                                0, v.Count(), &v[0]);
    if (uni.vert)
        cgGLSetParameterArray1f((CGparameter)uni.vert,
                                0, v.Count(), &v[0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, Array<vec2> const &v)
{
#if defined USE_D3D9 || defined _XBOX
    /* FIXME: this will not work properly because we don't know how tell DX9
     * it's a bunch of vec2 instead of vec4. */
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag,
                                             &v[0][0], v.Count() / 2);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert,
                                              &v[0][0], v.Count() / 2);
#elif !defined __CELLOS_LV2__
    glUniform2fv(uni.frag, v.Count(), &v[0][0]);
#else
    if (uni.frag)
        cgGLSetParameterArray2f((CGparameter)uni.frag,
                                0, v.Count(), &v[0][0]);
    if (uni.vert)
        cgGLSetParameterArray2f((CGparameter)uni.vert,
                                0, v.Count(), &v[0][0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, Array<vec3> const &v)
{
#if defined USE_D3D9 || defined _XBOX
    /* FIXME: this will not work properly because we don't know how tell DX9
     * it's a bunch of vec3 instead of vec4. */
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag,
                                             &v[0][0], v.Count());
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert,
                                              &v[0][0], v.Count());
#elif !defined __CELLOS_LV2__
    glUniform3fv(uni.frag, v.Count(), &v[0][0]);
#else
    if (uni.frag)
        cgGLSetParameterArray3f((CGparameter)uni.frag,
                                0, v.Count(), &v[0][0]);
    if (uni.vert)
        cgGLSetParameterArray3f((CGparameter)uni.vert,
                                0, v.Count(), &v[0][0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, Array<vec4> const &v)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag,
                                             &v[0][0], v.Count());
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert,
                                              &v[0][0], v.Count());
#elif !defined __CELLOS_LV2__
    glUniform4fv(uni.frag, v.Count(), &v[0][0]);
#else
    if (uni.frag)
        cgGLSetParameterArray4f((CGparameter)uni.frag,
                                0, v.Count(), &v[0][0]);
    if (uni.vert)
        cgGLSetParameterArray4f((CGparameter)uni.vert,
                                0, v.Count(), &v[0][0]);
#endif
}

void Shader::Bind() const
{
#if defined USE_D3D9 || defined _XBOX
    HRESULT hr;
    hr = data->m_dev->SetVertexShader(data->vert_shader);
    hr = data->m_dev->SetPixelShader(data->frag_shader);
#elif !defined __CELLOS_LV2__
    glUseProgram(data->prog_id);
#else
    cgGLEnableProfile(cgGLGetLatestProfile(CG_GL_VERTEX));
    cgGLBindProgram(data->vert_id);
    cgGLEnableProfile(cgGLGetLatestProfile(CG_GL_FRAGMENT));
    cgGLBindProgram(data->frag_id);
#endif
}

void Shader::Unbind() const
{
#if defined USE_D3D9 || defined _XBOX
    HRESULT hr;
    hr = data->m_dev->SetVertexShader(nullptr);
    hr = data->m_dev->SetPixelShader(nullptr);
#elif !defined __CELLOS_LV2__
    /* FIXME: untested */
    glUseProgram(0);
#else
    /* FIXME: untested */
    cgGLDisableProfile(cgGLGetLatestProfile(CG_GL_VERTEX));
    cgGLDisableProfile(cgGLGetLatestProfile(CG_GL_FRAGMENT));
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
#if defined HAVE_GLES_2X
        /* GLES 2.x supports #version 100, that's all. */
        return 100;
#else
        char buf[4096];
        GLsizei len;

        int id = glCreateShader(GL_VERTEX_SHADER);

        /* Can we compile 1.30 shaders? */
        char const *test130 =
            "#version 130\n"
            "void main() { gl_Position = vec4(0.0, 0.0, 0.0, 0.0); }";
        glShaderSource(id, 1, &test130, nullptr);
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
            glShaderSource(id, 1, &test120, nullptr);
            glCompileShader(id);
            glGetShaderInfoLog(id, sizeof(buf), &len, buf);
            if (len <= 0)
                version = 120;
        }

        /* Otherwise, assume we can compile 1.10 shaders. */
        if (!version)
            version = 110;

        glDeleteShader(id);
#endif
    }
#endif

    return version;
}

/*
 * Simple shader source patching for old GLSL versions.
 */
String ShaderData::Patch(String const &code, ShaderType type)
{
    int ver_driver = GetVersion();

    String patched_code = code;
    if (ver_driver >= 130)
        return patched_code;

    int ver_shader = 110;
    char *parser = strstr(patched_code.C(), "#version");
    if (parser)
        ver_shader = atoi(parser + strlen("#version"));

    /* This is GL ES, we only know version 100. */
    if (ver_shader > 100 && ver_driver == 100)
    {
        /* FIXME: this isn't elegant but honestly, we don't care, this
         * whole file is going to die soon. */
        char *p = strstr(patched_code.C(), "#version");
        if (p)
        {
            p += 8;
            while (*p == ' ')
                p++;
            if (p[0] == '1' && p[1] && p[2])
                p[1] = p[2] = '0';
        }
    }

    if (ver_shader > 120 && ver_driver <= 120)
    {
        char const *end = patched_code.C() + patched_code.Count() + 1;

        /* Find main() */
        parser = strstr(patched_code.C(), "main");
        if (!parser) return patched_code;
        parser = strstr(parser, "(");
        if (!parser) return patched_code;
        parser = strstr(parser, ")");
        if (!parser) return patched_code;
        parser = strstr(parser, "{");
        if (!parser) return patched_code;
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

            nullptr
        };

        for (char const * const *rep = main_replaces; rep[0]; rep += 2)
        {
            char *match = strstr(patched_code.C(), rep[0]);
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
            "in vec2", type == ShaderType::Vertex ? "attribute vec2" : "varying vec2",
            "in vec3", type == ShaderType::Vertex ? "attribute vec3" : "varying vec3",
            "in vec4", type == ShaderType::Vertex ? "attribute vec4" : "varying vec4",
            "in mat4", type == ShaderType::Vertex ? "attribute mat4" : "varying mat4",
            "out vec2", "varying vec2",
            "out vec3", "varying vec3",
            "out vec4", "varying vec4",
            "out mat4", "varying mat4",
            nullptr
        };

        for (char const * const *rep = fast_replaces; rep[0]; rep += 2)
        {
            char *match;
            while ((match = strstr(patched_code.C(), rep[0])))
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

    patched_code.Resize(strlen(patched_code.C()));

    return patched_code;
}

} /* namespace lol */

