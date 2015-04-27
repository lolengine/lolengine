//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

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

#include "pegtl.hh"

#include "lolgl.h"

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
#else
    GLuint prog_id, vert_id, frag_id;
    // Benlitz: using a simple array could be faster since there is never more than a few attribute locations to store
    map<uint64_t, GLint> attrib_locations;
    map<uint64_t, bool> attrib_errors;
#endif
    uint32_t vert_crc, frag_crc;

    /* Shader patcher */
    static int GetVersion();
    static String Patch(String const &code, ShaderType type);

    /* Global shader cache */
    static Shader *shaders[];
    static hash<char const *> Hash;
    static int nshaders;
};

Shader *ShaderData::shaders[256];
hash<char const *> ShaderData::Hash;
int ShaderData::nshaders = 0;

/*
 * LolFx parser
 */

using namespace pegtl;

struct lolfx_parser
{
public:
    lolfx_parser(String const &code)
      : m_section("header")
    {
        basic_parse_string<struct lolfx>(std::string(code.C()), this);
    }

public:
    String m_section;
    map<String, String> m_programs;

private:
    struct do_title : action_base<do_title>
    {
        static void apply(std::string const &ctx, lolfx_parser *that)
        {
            that->m_section = ctx.c_str();
        }
    };

    struct do_code : action_base<do_code>
    {
        static void apply(std::string const &ctx, lolfx_parser *that)
        {
            that->m_programs[that->m_section] = ctx.c_str();
        }
    };

    // title <- '[' (!']')+ ']' .{eol}
    struct title
      : seq<one<'['>,
            ifapply<plus<not_one<']'>>, do_title>,
            one<']'>,
            until<eol, any>> {};

    // FIXME: I’m using this rule because the ifapply<> above also
    // gets triggered when using at<> which is non-consuming.
    struct title_ignore
      : seq<one<'['>,
            plus<not_one<']'>>,
            one<']'>,
            until<eol, any>> {};

    // code_line <- .{eol}
    struct code_line
      : until<eol, any> {};

    // code_section < code_line{&(title / eof)}
    struct code_section
      : ifapply<until<at<sor<title_ignore, pegtl::eof>>, code_line>, do_code> {};

    // shader < title code_section
    struct shader
      : seq<title, code_section> {};

    // header < code_section
    struct header
      : code_section {};

    // lolfx < header code_section*
    struct lolfx
      : seq<header, star<shader>> {};
};

/*
 * Public Shader class
 */

Shader *Shader::Create(String const &name, String const &code)
{
    lolfx_parser p(code);

    ASSERT(p.m_programs.has_key("vert.glsl"),
           "no vertex shader in %s", name.C());

    ASSERT(p.m_programs.has_key("frag.glsl"),
           "no fragment shader in %s", name.C());

    String vert = p.m_programs["vert.glsl"];
    String frag = p.m_programs["frag.glsl"];

    uint32_t new_vert_crc = ShaderData::Hash(vert);
    uint32_t new_frag_crc = ShaderData::Hash(frag);

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
               String const &vert, String const &frag)
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
#else
    char errbuf[4096];
    String shader_code;
    GLchar const *gl_code;
    GLint status;
    GLsizei len;
#endif

    /* Compile vertex shader */
    data->vert_crc = ShaderData::Hash(vert);
#if defined USE_D3D9 || defined _XBOX
#   if defined USE_D3D9
    data->m_dev = (IDirect3DDevice9 *)Renderer::Get()->GetDevice();
#   elif defined _XBOX
    data->m_dev = (D3DDevice *)Renderer::Get()->GetDevice();
#   endif

    hr = D3DXCompileShader(vert, (UINT)strlen(vert), macros, nullptr, "main",
                           "vs_3_0", 0, &shader_code, &error_msg,
                           &data->vert_table);
    if (FAILED(hr))
    {
        msg::error("failed to compile vertex shader %s: %s\n", name.C(),
                   error_msg ? error_msg->GetBufferPointer() : "error");
        msg::error("shader source:\n%s\n", vert);
    }
    data->m_dev->CreateVertexShader((DWORD *)shader_code->GetBufferPointer(),
                                    &data->vert_shader);
    shader_code->Release();
#else
    shader_code = ShaderData::Patch(vert, ShaderType::Vertex);
    data->vert_id = glCreateShader(GL_VERTEX_SHADER);
    gl_code = shader_code.C();
    glShaderSource(data->vert_id, 1, &gl_code, nullptr);
    glCompileShader(data->vert_id);

    glGetShaderInfoLog(data->vert_id, sizeof(errbuf), &len, errbuf);
    glGetShaderiv(data->vert_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        msg::error("failed to compile vertex shader %s: %s\n",
                   name.C(), errbuf);
        msg::error("shader source:\n%s\n", shader_code.C());
    }
    else if (len > 16)
    {
        msg::debug("compile log for vertex shader %s: %s\n", name.C(), errbuf);
        msg::debug("shader source:\n%s\n", shader_code.C());
    }
#endif

    /* Compile fragment shader */
    data->frag_crc = ShaderData::Hash(frag);
#if defined USE_D3D9 || defined _XBOX
    hr = D3DXCompileShader(frag, (UINT)strlen(frag), macros, nullptr, "main",
                           "ps_3_0", 0, &shader_code, &error_msg,
                           &data->frag_table);
    if (FAILED(hr))
    {
        msg::error("failed to compile fragment shader %s: %s\n", name.C(),
                   error_msg ? error_msg->GetBufferPointer() : "error");
        msg::error("shader source:\n%s\n", frag);
    }
    data->m_dev->CreatePixelShader((DWORD *)shader_code->GetBufferPointer(),
                                   &data->frag_shader);
    shader_code->Release();
#else
    shader_code = ShaderData::Patch(frag, ShaderType::Fragment);
    data->frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    gl_code = shader_code.C();
    glShaderSource(data->frag_id, 1, &gl_code, nullptr);
    glCompileShader(data->frag_id);

    glGetShaderInfoLog(data->frag_id, sizeof(errbuf), &len, errbuf);
    glGetShaderiv(data->frag_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        msg::error("failed to compile fragment shader %s: %s\n",
                   name.C(), errbuf);
        msg::error("shader source:\n%s\n", shader_code.C());
    }
    else if (len > 16)
    {
        msg::debug("compile log for fragment shader %s: %s\n",
                   name.C(), errbuf);
        msg::debug("shader source:\n%s\n", shader_code.C());
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
#else
    /* Create program */
    data->prog_id = glCreateProgram();
    glAttachShader(data->prog_id, data->vert_id);
    glAttachShader(data->prog_id, data->frag_id);

    glLinkProgram(data->prog_id);
    glGetProgramInfoLog(data->prog_id, sizeof(errbuf), &len, errbuf);
    glGetProgramiv(data->prog_id, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        msg::error("failed to link program %s: %s\n", name.C(), errbuf);
    }
    else if (len > 16)
    {
        msg::debug("link log for program %s: %s\n", name.C(), errbuf);
    }

    GLint validated;
    glValidateProgram(data->prog_id);
    glGetProgramiv(data->prog_id, GL_VALIDATE_STATUS, &validated);
    if (validated != GL_TRUE)
    {
        msg::error("failed to validate program %s\n", name.C());
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
            if (name.starts_with(attribute_names[j]) ||
                name.starts_with(String(attribute_names[j]).to_lower()))
            {
                usage = VertexUsage(j);
                char* idx_ptr = name.C() + strlen(attribute_names[j]);
                index = strtol(idx_ptr, nullptr, 10);
                break;
            }
        }

        if (usage == VertexUsage::MAX || index == -1)
        {
            msg::error("unable to parse attribute semantic from name: %s\n",
                       name_buffer);
        }
        else
        {
            GLint location = glGetAttribLocation(data->prog_id, name_buffer);
            uint64_t flags = (uint64_t)(uint16_t)usage.ToScalar() << 16;
            flags |= (uint64_t)(uint16_t)index;
            // TODO: this is here just in case. Remove this once everything has been correctly tested
#if _DEBUG
            if (data->attrib_locations.has_key(flags))
            {
                msg::error("error while parsing attribute semantics in %s\n",
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
    return data->attrib_locations.count();
}

ShaderAttrib Shader::GetAttribLocation(VertexUsage usage, int index) const
{
    ShaderAttrib ret;
    ret.m_flags = (uint64_t)(uint16_t)usage.ToScalar() << 16;
    ret.m_flags |= (uint64_t)(uint16_t)index;
#if defined USE_D3D9 || defined _XBOX
#else
    GLint l = -1;

    if (!data->attrib_locations.try_get(ret.m_flags, l))
    {
        /* Only spit an error once, we don’t need to flood the console. */
        if (!data->attrib_errors.has_key(ret.m_flags))
        {
            msg::error("attribute %s not found in shader %s\n",
                       usage.ToString().C(), data->m_name.C());
            data->attrib_errors[ret.m_flags] = true;
        }
    }
    ret.m_flags |= (uint64_t)(uint32_t)l << 32;
#endif
    return ret;
}

ShaderUniform Shader::GetUniformLocation(String const& uni) const
{
    return GetUniformLocation(uni.C());
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
#else
    ret.frag = (uintptr_t)glGetUniformLocation(data->prog_id, uni);
    ret.vert = 0;
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
#else
    glUniform1i((GLint)uni.frag, i);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, ivec2 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, ivec4(v, 0, 0));
#else
    glUniform2i((GLint)uni.frag, v.x, v.y);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, ivec3 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, ivec4(v, 0));
#else
    glUniform3i((GLint)uni.frag, v.x, v.y, v.z);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, ivec4 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantI((UINT)uni.frag, &v[0], 1);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantI((UINT)uni.vert, &v[0], 1);
#else
    glUniform4i((GLint)uni.frag, v.x, v.y, v.z, v.w);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, float f)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, vec4(f, 0, 0, 0));
#else
    glUniform1f((GLint)uni.frag, f);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, vec2 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, vec4(v, 0, 0));
#else
    glUniform2fv((GLint)uni.frag, 1, &v[0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, vec3 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    SetUniform(uni, vec4(v, 0));
#else
    glUniform3fv((GLint)uni.frag, 1, &v[0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, vec4 const &v)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag, &v[0], 1);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert, &v[0], 1);
#else
    glUniform4fv((GLint)uni.frag, 1, &v[0]);
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
#else
    glUniformMatrix2fv((GLint)uni.frag, 1, GL_FALSE, &m[0][0]);
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
#else
    glUniformMatrix3fv((GLint)uni.frag, 1, GL_FALSE, &m[0][0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, mat4 const &m)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag, &m[0][0], 4);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert, &m[0][0], 4);
#else
    glUniformMatrix4fv((GLint)uni.frag, 1, GL_FALSE, &m[0][0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, TextureUniform tex, int index)
{
#if defined USE_D3D9 || defined _XBOX
    data->m_dev->SetTexture(index, (LPDIRECT3DTEXTURE9)tex.m_flags);
    data->m_dev->SetSamplerState(index, D3DSAMP_MAGFILTER, tex.m_attrib & 0xff);
    data->m_dev->SetSamplerState(index, D3DSAMP_MINFILTER, (tex.m_attrib >> 8) & 0xff);
    data->m_dev->SetSamplerState(index, D3DSAMP_MIPFILTER, (tex.m_attrib >> 16) & 0xff);
#else
    glActiveTexture(GL_TEXTURE0 + index);
    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (int)tex.m_flags);
    SetUniform(uni, index);
#endif
}

/*
 * Uniform setters for arrays
 */

void Shader::SetUniform(ShaderUniform const &uni, array<float> const &v)
{
#if defined USE_D3D9 || defined _XBOX
    /* FIXME: this will not work properly because we don't know how tell DX9
     * it's a bunch of floats instead of vec4. */
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag,
                                             &v[0], v.count() / 4);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert,
                                              &v[0], v.count() / 4);
#else
    glUniform1fv((GLint)uni.frag, (GLsizei)v.count(), &v[0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, array<vec2> const &v)
{
#if defined USE_D3D9 || defined _XBOX
    /* FIXME: this will not work properly because we don't know how tell DX9
     * it's a bunch of vec2 instead of vec4. */
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag,
                                             &v[0][0], v.count() / 2);
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert,
                                              &v[0][0], v.count() / 2);
#else
    glUniform2fv((GLint)uni.frag, (GLsizei)v.count(), &v[0][0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, array<vec3> const &v)
{
#if defined USE_D3D9 || defined _XBOX
    /* FIXME: this will not work properly because we don't know how tell DX9
     * it's a bunch of vec3 instead of vec4. */
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag,
                                             &v[0][0], v.count());
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert,
                                              &v[0][0], v.count());
#else
    glUniform3fv((GLint)uni.frag, (GLsizei)v.count(), &v[0][0]);
#endif
}

void Shader::SetUniform(ShaderUniform const &uni, array<vec4> const &v)
{
#if defined USE_D3D9 || defined _XBOX
    if (uni.flags & 1)
        data->m_dev->SetPixelShaderConstantF((UINT)uni.frag,
                                             &v[0][0], v.count());
    if (uni.flags & 2)
        data->m_dev->SetVertexShaderConstantF((UINT)uni.vert,
                                              &v[0][0], v.count());
#else
    glUniform4fv((GLint)uni.frag, (GLsizei)v.count(), &v[0][0]);
#endif
}

void Shader::Bind() const
{
#if defined USE_D3D9 || defined _XBOX
    HRESULT hr;
    hr = data->m_dev->SetVertexShader(data->vert_shader);
    hr = data->m_dev->SetPixelShader(data->frag_shader);
#else
    glUseProgram(data->prog_id);
#endif
}

void Shader::Unbind() const
{
#if defined USE_D3D9 || defined _XBOX
    HRESULT hr;
    hr = data->m_dev->SetVertexShader(nullptr);
    hr = data->m_dev->SetPixelShader(nullptr);
#else
    /* FIXME: untested */
    glUseProgram(0);
#endif
}

Shader::~Shader()
{
#if defined USE_D3D9 || defined _XBOX
    data->vert_shader->Release();
    data->vert_table->Release();
    data->frag_shader->Release();
    data->frag_table->Release();
#else
    glDetachShader(data->prog_id, data->vert_id);
    glDetachShader(data->prog_id, data->frag_id);
    glDeleteShader(data->vert_id);
    glDeleteShader(data->frag_id);
    glDeleteProgram(data->prog_id);
#endif
    delete data;
}

/* Try to detect shader compiler features */
int ShaderData::GetVersion()
{
    static int version = 0;

#if !defined USE_D3D9 && !defined _XBOX
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
        char const *end = patched_code.C() + patched_code.count() + 1;

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

    patched_code.resize(strlen(patched_code.C()));

    return patched_code;
}

static const String g_ret = "\n";
static const String g_eol = ";";
static const String g_bop = "{";
static const String g_bcl = "}";
static const String g_tab = "    ";

//----
String Shader::GetVariablePrefix(const ShaderVariable variable)
{
    switch (variable.ToScalar())
    {
        case ShaderVariable::Attribute:  return String("in_");
        case ShaderVariable::Uniform:    return String("u_");
        case ShaderVariable::Varying:    return String("pass_");
        case ShaderVariable::InOut:
        default: return String();
    }
}

//----
String Shader::GetVariableQualifier(const ShaderVariable variable)
{
    switch (variable.ToScalar())
    {
        case ShaderVariable::Attribute: return String("attribute");
        case ShaderVariable::Uniform:   return String("uniform");
        case ShaderVariable::Varying:   return String("varying");
        case ShaderVariable::InOut:
        default: return String();
    }
}

//----
String Shader::GetFunctionQualifier(const ShaderVariable variable, const ShaderProgram program)
{
    switch (program.ToScalar())
    {
        case ShaderProgram::Geometry:
        {
            //TODO : L O L ----------------
            return String();
        }
        case ShaderProgram::Vertex:
        {
            switch (variable.ToScalar())
            {
                case ShaderVariable::Attribute:  return String("in");
                case ShaderVariable::Uniform:    return String("in");
                case ShaderVariable::Varying:    return String("inout");
                case ShaderVariable::InOut:      return String("inout");
                default: return String();
            }
            return String();
        }
        case ShaderProgram::Pixel:
        {
            switch (variable.ToScalar())
            {
                case ShaderVariable::Attribute:  return String("in");
                case ShaderVariable::Uniform:    return String("in");
                case ShaderVariable::Varying:    return String("in");
                case ShaderVariable::InOut:      return String("inout");
                default: return String();
            }
            return String();
        }
        default:
        {
            return String();
        }
    }
}

//----
String Shader::GetProgramQualifier(const ShaderProgram program)
{
    switch (program.ToScalar())
    {
        case ShaderProgram::Geometry: return String(); //TODO : L O L ---------
        case ShaderProgram::Vertex:   return String("[vert.glsl]");
        case ShaderProgram::Pixel:    return String("[frag.glsl]");
        default: return String();
    }
}

//----
String Shader::GetProgramOutVariable(const ShaderProgram program)
{
    switch (program.ToScalar())
    {
        case ShaderProgram::Geometry: return String(); //TODO : L O L ---------
        case ShaderProgram::Vertex:   return String("gl_Position");
        case ShaderProgram::Pixel:    return String("gl_FragColor");
        default: return String();
    }
}

//----
String Shader::GetProgramOutVariableLocal(const ShaderProgram program)
{
    switch (program.ToScalar())
    {
        case ShaderProgram::Geometry: return String(); //TODO : L O L ---------
        case ShaderProgram::Vertex:   return String("out_position");
        case ShaderProgram::Pixel:    return String("out_frag_color");
        default: return String();
    }
}

//ShaderVar -------------------------------------------------------------------
ShaderVar ShaderVar::GetShaderOut(ShaderProgram program)
{
    switch (program.ToScalar())
    {
    case ShaderProgram::Geometry: //TODO : L O L ------------------------------
    default: ASSERT(false); return ShaderVar();
    case ShaderProgram::Vertex:   return ShaderVar(ShaderVariable::InOut, ShaderVariableType::Vec4, Shader::GetProgramOutVariableLocal(program));
    case ShaderProgram::Pixel:    return ShaderVar(ShaderVariable::InOut, ShaderVariableType::Vec4, Shader::GetProgramOutVariableLocal(program));
    }
}

//Shader Block implementation class -------------------------------------------
void ShaderBlock::AddVar(ShaderVar const& var)
{
    ShaderVariable qualifier = var.GetQualifier();
    String type = var.GetType();
    String name = Shader::GetVariablePrefix(qualifier) + var.m_name;
    ASSERT(!m_parameters[qualifier.ToScalar()].has_key(name));
    m_parameters[qualifier.ToScalar()][name] = type;
}

//----
void ShaderBlock::AddCallParameters(map<String, String> const& variables, String& result)
{
    array<String> keys = variables.keys();
    for (String key : keys)
    {
        if (result.count() > 0)
            result += ", ";
        result += key;
    }
}

//----
void ShaderBlock::AddDefinitionParameters(const ShaderVariable type, const ShaderProgram program, map<String, String>& variables, String& result)
{
    array<String> keys = variables.keys();
    for (String key : keys)
    {
        if (result.count() > 0)
            result += ", ";
        result += Shader::GetFunctionQualifier(type, program) + " ";
        result += variables[key];
        result += String(" ");
        result += key;
    }
}

//----
void ShaderBlock::Build(const ShaderProgram program, String& call, String& function)
{
    ASSERT(m_name.count());
    ASSERT(m_parameters[ShaderVariable::InOut].count());

    //Build call in main
    String call_name = String("Call_") + m_name;
    call = call_name + "(";
    String call_parameters;
    for (int i = 0; i < ShaderVariable::MAX; i++)
        AddCallParameters(/*(ShaderVariable)i, */m_parameters[i], call_parameters);
    call += call_parameters + ");";

    //Build function declaration
    function = String("void ") + call_name + "(";
    String def_parameters;
    for (int i = 0; i < ShaderVariable::MAX; i++)
        AddDefinitionParameters((ShaderVariable)i, program, m_parameters[i], def_parameters);
    function += def_parameters + ")" + g_ret +
        "{" + g_ret +
        m_code_main + ((m_code_main.ends_with(g_ret)) ? (String()) : (g_ret)) +
        "}";
}

//Shader Builder implementation class -----------------------------------------
ShaderBuilder::ShaderBuilder(String const& name, String const& version)
    : m_name(name), m_version(version)
{
    ASSERT(name.count());
    ASSERT(version.count());
}

//----
ShaderBuilder::~ShaderBuilder()
{
}

//----
String const& ShaderBuilder::GetName()
{
    return m_name;
}

//----
ShaderBuilder& ShaderBuilder::operator<<(const ShaderProgram program)
{
    m_current_program = program;
    return *this;
}

//----
ShaderBuilder& ShaderBuilder::operator<<(ShaderBlock* block)
{
    ASSERT(m_current_program != ShaderProgram::MAX);
    m_blocks[m_current_program.ToScalar()].push_unique(block);
    return *this;
}

//----
ShaderBuilder& ShaderBuilder::operator<<(ShaderBlock const& block)
{
    ASSERT(m_current_program != ShaderProgram::MAX);
    m_blocks[m_current_program.ToScalar()].push_unique(new ShaderBlock(block));
    return *this;
}

//----
String ShaderBuilder::AddSlotOutVariableLocal(const ShaderProgram program)
{
    ShaderVariable var = ShaderVariable::InOut;
    String result = Shader::GetProgramOutVariableLocal(program);
    switch (program.ToScalar())
    {
        case ShaderProgram::Geometry:
        {
            //TODO : L O L ----------------
            break;
        }
        case ShaderProgram::Vertex:
        {
            m_parameters[program.ToScalar()][var.ToScalar()][result] = "vec4";
            break;
        }
        case ShaderProgram::Pixel:
        {
            m_parameters[program.ToScalar()][var.ToScalar()][result] = "vec4";
            break;
        }
        default:
        {
            break;
        }
    }
    return result;
}

//----
void ShaderBuilder::MergeParameters(map<String, String>& variables, map<String, String>& merged)
{
    array<String> keys = variables.keys();
    for (String key : keys)
    {
        bool has_key = merged.has_key(key);

        //Key exists, check the type to make sure it's the same
        ASSERT(!has_key || (has_key && merged[key] == variables[key]),
            "has_key=%d, key=%s merged[key]=%s, variables[key]=%s\n",
            (int)has_key, key.C(), merged[key].C(), variables[key].C());

        //does not exist, had it
        if (!has_key)
            merged[key] = variables[key];
    }
}

//----
void ShaderBuilder::Build(String& code)
{
    //Cleanup first
    for (int prog = 0; prog < ShaderProgram::MAX; prog++)
        for (int var = 0; var < ShaderVariable::MAX; var++)
            m_parameters[prog][var].empty();

    //Start building
    for (int prog = 0; prog < ShaderProgram::MAX; prog++)
    {
        //Add default local out in merged variables
        String out_local_var = AddSlotOutVariableLocal((ShaderProgram)prog);

        if (!out_local_var.count())
            continue;

        //Merge all variables
        for (int var = 0; var < ShaderVariable::MAX; var++)
            for (int block = 0; block < m_blocks[prog].count(); block++)
                MergeParameters(m_blocks[prog][block]->m_parameters[var], m_parameters[prog][var]);

        //Actually write code
        code += Shader::GetProgramQualifier((ShaderProgram)prog) + g_ret;

        //Add actual code
        code += String("#version ") + m_version + g_ret + g_ret;

        //Added shader variables
        for (int var = 0; var < ShaderVariable::InOut; var++)
        {
            array<String> keys = m_parameters[prog][var].keys();
            if (keys.count())
            {
                code += String("//- ") + Shader::GetVariableQualifier((ShaderVariable)var) + " ----" + g_ret;
                for (String key : keys)
                {
                    code += Shader::GetVariableQualifier((ShaderVariable)var) + " ";
                    code += m_parameters[prog][var][key] + " " + key + ";" + g_ret;
                }
                if (var + 1 < ShaderVariable::InOut)
                    code += g_ret;
            }
        }
        code += g_ret;

        //Build Blocks code and add it
        array<String> calls;
        for (int block = 0; block < m_blocks[prog].count(); block++)
        {
            String call;
            String function;
            m_blocks[prog][block]->Build(ShaderProgram(prog), call, function);
            calls << call;
            if (m_blocks[prog][block]->m_code_custom.count())
            {
                code += String("//- ") + m_blocks[prog][block]->GetName() + " custom code ----" + g_ret;
                code += m_blocks[prog][block]->m_code_custom + g_ret + g_ret;
            }
            code += String("//- ") + m_blocks[prog][block]->GetName() + " main code ----" + g_ret;
            code += function + g_ret + g_ret;
        }

        //Added main definition
        code += String("//- Main ----") + g_ret +
            String("void main(void)") + g_ret + "{" + g_ret;

        //Add local variables
        int var = ShaderVariable::InOut;
        array<String> keys = m_parameters[prog][var].keys();
        for (String key : keys)
        {
            if (keys.count())
            {
                code += g_tab + m_parameters[prog][var][key] + " " + key + ";" + g_ret;
            }
        }
        code += g_ret;

        //Add calls
        code += g_tab + String("//- Calls ----") + g_ret;
        for (String call : calls)
            code += g_tab + call + g_ret;
        code += g_ret;

        code += g_tab + Shader::GetProgramOutVariable((ShaderProgram)prog) + " = " + out_local_var + ";" + g_ret +
            String("}") + g_ret + g_ret;
    }
}

} /* namespace lol */

