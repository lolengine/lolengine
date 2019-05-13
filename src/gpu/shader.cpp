//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <string>
#include <memory>
#include <map>
#include <set>
#include <cstring>
#include <cstdio>

#if defined _WIN32
#   define WIN32_LEAN_AND_MEAN 1
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#endif

#include "tao/pegtl.hpp"

#include "lolgl.h"

// FIXME: fine-tune this define
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X

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
    std::string m_name;

    GLuint prog_id, vert_id, frag_id;
    // Benlitz: using a simple array could be faster since there is never more than a few attribute locations to store
    std::map<uint64_t, GLint> attrib_locations;
    std::map<uint64_t, bool> attrib_errors;
    size_t vert_crc, frag_crc;

    /* Shader patcher */
    static int GetVersion();
    static std::string Patch(std::string const &code, ShaderType type);
};

/* Global shader cache */
static std::set<std::shared_ptr<Shader>> g_shaders;

/*
 * LolFx parser
 */

using namespace tao;
using namespace pegtl;

struct lolfx_parser
{
public:
    std::string m_section;
    std::map<std::string, std::string> m_programs;

private:
    // title <- '[' (!']')+ ']' .{eol}
    struct do_title
      : plus<not_one<']'>> {};

    struct title
      : seq<one<'['>,
            do_title,
            one<']'>,
            until<eol, any>> {};

    // FIXME: I’m using this rule because the do_title above also
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
      : until<at<sor<title_ignore, pegtl::eof>>, code_line> {};

    // shader < title code_section
    struct shader
      : seq<title, code_section> {};

    // header < code_section
    struct header
      : code_section {};

    // lolfx < header code_section*
    struct lolfx
      : seq<header, star<shader>> {};

    // default action: nothing
    template<typename RULE>
    struct action : nothing<RULE> {};

public:
    lolfx_parser(std::string const &code)
      : m_section("header")
    {
        string_input<> in(code, "shader");
        pegtl::parse<lolfx, action>(in, this);
    }
};

template<>
struct lolfx_parser::action<lolfx_parser::do_title>
{
    template<typename INPUT>
    static void apply(INPUT const &in, lolfx_parser *that)
    {
        that->m_section = in.string();
    }
};

template<>
struct lolfx_parser::action<lolfx_parser::code_section>
{
    template<typename INPUT>
    static void apply(INPUT const &in, lolfx_parser *that)
    {
        that->m_programs[that->m_section] = in.string();
    }
};

/*
 * Public Shader class
 */

std::shared_ptr<Shader> Shader::Create(std::string const &name, std::string const &code)
{
    lolfx_parser p(code);

    ASSERT(has_key(p.m_programs, "vert.glsl"),
           "no vertex shader in %s", name.c_str());

    ASSERT(has_key(p.m_programs, "frag.glsl"),
           "no fragment shader in %s", name.c_str());

    std::string vert = p.m_programs["vert.glsl"];
    std::string frag = p.m_programs["frag.glsl"];

    size_t new_vert_crc = std::hash<std::string>{}(vert);
    size_t new_frag_crc = std::hash<std::string>{}(frag);

    for (auto shader : g_shaders)
    {
        if (shader->data->vert_crc == new_vert_crc
             && shader->data->frag_crc == new_frag_crc)
            return shader;
    }

    // FIXME: the cache never expires!
    auto ret = std::make_shared<Shader>(name, vert, frag);
    g_shaders.insert(ret);
    return ret;
}

Shader::Shader(std::string const &name,
               std::string const &vert, std::string const &frag)
  : data(std::make_unique<ShaderData>())
{
    data->m_name = name;

    char errbuf[4096];
    std::string shader_code;
    GLchar const *gl_code;
    GLint status;
    GLsizei len;

    /* Compile vertex shader */
    data->vert_crc = std::hash<std::string>{}(vert);

    shader_code = ShaderData::Patch(vert, ShaderType::Vertex);
    data->vert_id = glCreateShader(GL_VERTEX_SHADER);
    gl_code = shader_code.c_str();
    glShaderSource(data->vert_id, 1, &gl_code, nullptr);
    glCompileShader(data->vert_id);

    glGetShaderInfoLog(data->vert_id, sizeof(errbuf), &len, errbuf);
    glGetShaderiv(data->vert_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        msg::error("failed to compile vertex shader %s: %s\n",
                   name.c_str(), errbuf);
        msg::error("shader source:\n%s\n", shader_code.c_str());
    }
    else if (len > 16)
    {
        msg::debug("compile log for vertex shader %s: %s\n", name.c_str(), errbuf);
        msg::debug("shader source:\n%s\n", shader_code.c_str());
    }

    /* Compile fragment shader */
    data->frag_crc = std::hash<std::string>{}(frag);

    shader_code = ShaderData::Patch(frag, ShaderType::Fragment);
    data->frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    gl_code = shader_code.c_str();
    glShaderSource(data->frag_id, 1, &gl_code, nullptr);
    glCompileShader(data->frag_id);

    glGetShaderInfoLog(data->frag_id, sizeof(errbuf), &len, errbuf);
    glGetShaderiv(data->frag_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        msg::error("failed to compile fragment shader %s: %s\n",
                   name.c_str(), errbuf);
        msg::error("shader source:\n%s\n", shader_code.c_str());
    }
    else if (len > 16)
    {
        msg::debug("compile log for fragment shader %s: %s\n",
                   name.c_str(), errbuf);
        msg::debug("shader source:\n%s\n", shader_code.c_str());
    }

    /* Create program */
    data->prog_id = glCreateProgram();
    glAttachShader(data->prog_id, data->vert_id);
    glAttachShader(data->prog_id, data->frag_id);

    glLinkProgram(data->prog_id);
    glGetProgramInfoLog(data->prog_id, sizeof(errbuf), &len, errbuf);
    glGetProgramiv(data->prog_id, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        msg::error("failed to link program %s: %s\n", name.c_str(), errbuf);
    }
    else if (len > 16)
    {
        msg::debug("link log for program %s: %s\n", name.c_str(), errbuf);
    }

    GLint validated;
    glValidateProgram(data->prog_id);
    glGetProgramiv(data->prog_id, GL_VALIDATE_STATUS, &validated);
    if (validated != GL_TRUE)
    {
        msg::error("failed to validate program %s\n", name.c_str());
    }

    GLint num_attribs;
    glGetProgramiv(data->prog_id, GL_ACTIVE_ATTRIBUTES, &num_attribs);

#if __EMSCRIPTEN__ // WebGL doesn't support GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, so chose a default size value.
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

        std::string attr_name(name_buffer);
        int index = -1;
        VertexUsage usage = VertexUsage::MAX;
        for (int j = 0; j < (int)VertexUsage::MAX; ++j)
        {
            if (starts_with(attr_name, attribute_names[j]) ||
                starts_with(attr_name, tolower(attribute_names[j])))
            {
                usage = VertexUsage(j);
                char* idx_ptr = &attr_name[0] + strlen(attribute_names[j]);
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
#if LOL_BUILD_DEBUG
            if (has_key(data->attrib_locations, flags))
            {
                msg::error("error while parsing attribute semantics in %s\n",
                           attr_name.c_str());
            }
#endif
            data->attrib_locations[flags] = location;
        }
    }

    delete[] name_buffer;
}

int Shader::GetAttribCount() const
{
    return (int)data->attrib_locations.size();
}

ShaderAttrib Shader::GetAttribLocation(VertexUsage usage, int index) const
{
    ShaderAttrib ret;
    ret.m_flags = (uint64_t)(uint16_t)usage.ToScalar() << 16;
    ret.m_flags |= (uint64_t)(uint16_t)index;

    GLint l = -1;

    if (!try_get(data->attrib_locations, ret.m_flags, l))
    {
        /* Only spit an error once, we don’t need to flood the console. */
        if (!has_key(data->attrib_errors, ret.m_flags))
        {
            msg::error("attribute %s not found in shader %s\n",
                       usage.tostring().c_str(), data->m_name.c_str());
            data->attrib_errors[ret.m_flags] = true;
        }
    }
    ret.m_flags |= (uint64_t)(uint32_t)l << 32;
    return ret;
}

ShaderUniform Shader::GetUniformLocation(std::string const& uni) const
{
    return GetUniformLocation(uni.c_str());
}
ShaderUniform Shader::GetUniformLocation(char const *uni) const
{
    ShaderUniform ret;
    ret.frag = (uintptr_t)glGetUniformLocation(data->prog_id, uni);
    ret.vert = 0;
    return ret;
}

/*
 * Uniform setters for scalars
 */

void Shader::SetUniform(ShaderUniform const &uni, int i)
{
    glUniform1i((GLint)uni.frag, i);
}

void Shader::SetUniform(ShaderUniform const &uni, ivec2 const &v)
{
    glUniform2i((GLint)uni.frag, v.x, v.y);
}

void Shader::SetUniform(ShaderUniform const &uni, ivec3 const &v)
{
    glUniform3i((GLint)uni.frag, v.x, v.y, v.z);
}

void Shader::SetUniform(ShaderUniform const &uni, ivec4 const &v)
{
    glUniform4i((GLint)uni.frag, v.x, v.y, v.z, v.w);
}

void Shader::SetUniform(ShaderUniform const &uni, float f)
{
    glUniform1f((GLint)uni.frag, f);
}

void Shader::SetUniform(ShaderUniform const &uni, vec2 const &v)
{
    glUniform2fv((GLint)uni.frag, 1, &v[0]);
}

void Shader::SetUniform(ShaderUniform const &uni, vec3 const &v)
{
    glUniform3fv((GLint)uni.frag, 1, &v[0]);
}

void Shader::SetUniform(ShaderUniform const &uni, vec4 const &v)
{
    glUniform4fv((GLint)uni.frag, 1, &v[0]);
}

void Shader::SetUniform(ShaderUniform const &uni, mat2 const &m)
{
    glUniformMatrix2fv((GLint)uni.frag, 1, GL_FALSE, &m[0][0]);
}

void Shader::SetUniform(ShaderUniform const &uni, mat3 const &m)
{
    glUniformMatrix3fv((GLint)uni.frag, 1, GL_FALSE, &m[0][0]);
}

void Shader::SetUniform(ShaderUniform const &uni, mat4 const &m)
{
    glUniformMatrix4fv((GLint)uni.frag, 1, GL_FALSE, &m[0][0]);
}

void Shader::SetUniform(ShaderUniform const &uni, TextureUniform tex, int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (int)tex.m_flags);
    SetUniform(uni, index);
}

/*
 * Uniform setters for arrays
 */

void Shader::SetUniform(ShaderUniform const &uni, array<float> const &v)
{
    glUniform1fv((GLint)uni.frag, (GLsizei)v.count(), &v[0]);
}

void Shader::SetUniform(ShaderUniform const &uni, array<vec2> const &v)
{
    glUniform2fv((GLint)uni.frag, (GLsizei)v.count(), &v[0][0]);
}

void Shader::SetUniform(ShaderUniform const &uni, array<vec3> const &v)
{
    glUniform3fv((GLint)uni.frag, (GLsizei)v.count(), &v[0][0]);
}

void Shader::SetUniform(ShaderUniform const &uni, array<vec4> const &v)
{
    glUniform4fv((GLint)uni.frag, (GLsizei)v.count(), &v[0][0]);
}

void Shader::Bind() const
{
    glUseProgram(data->prog_id);
}

void Shader::Unbind() const
{
    /* FIXME: untested */
    glUseProgram(0);
}

Shader::~Shader()
{
    glDetachShader(data->prog_id, data->vert_id);
    glDetachShader(data->prog_id, data->frag_id);
    glDeleteShader(data->vert_id);
    glDeleteShader(data->frag_id);
    glDeleteProgram(data->prog_id);
}

/* Try to detect shader compiler features */
int ShaderData::GetVersion()
{
    static int version = 0;

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

    return version;
}

/*
 * Simple shader source patching for old GLSL versions.
 */
std::string ShaderData::Patch(std::string const &code, ShaderType type)
{
    int ver_driver = GetVersion();

    std::string patched_code = code;
    if (ver_driver >= 130)
        return patched_code;

    /* FIXME: use std::string instead of char * for parsing? */
    int ver_shader = 110;
    char *parser = strstr(&patched_code[0], "#version");
    if (parser)
        ver_shader = atoi(parser + strlen("#version"));

    /* This is GL ES, we only know version 100. */
    if (ver_shader > 100 && ver_driver == 100)
    {
        /* FIXME: this isn't elegant but honestly, we don't care, this
         * whole file is going to die soon. */
        char *p = strstr(&patched_code[0], "#version");
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
        char const *end = patched_code.c_str() + patched_code.length() + 1;

        /* Find main() */
        parser = strstr(&patched_code[0], "main");
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
            char *match = strstr(&patched_code[0], rep[0]);
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
            "out vec4 out_color;", " ",
            "out_color =", "gl_FragColor =",
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
            while (true)
            {
                size_t index = patched_code.find(rep[0]);
                if (index == std::string::npos)
                    break;

                size_t l0 = strlen(rep[0]);
                size_t l1 = strlen(rep[1]);
                UNUSED(l1);

                std::string left = patched_code.substr(0, index);
                std::string right = patched_code.substr(index + l0, patched_code.length() - (index + l0));

                patched_code = left + std::string(rep[1]) + right;
            }
        }
    }

    return patched_code;
}

//----
std::string Shader::GetVariablePrefix(const ShaderVariable variable)
{
    switch (variable.ToScalar())
    {
        case ShaderVariable::Attribute:  return "in_";
        case ShaderVariable::Uniform:    return "u_";
        case ShaderVariable::Varying:    return "pass_";
        case ShaderVariable::InOut:
        default: return "";
    }
}

//----
std::string Shader::GetVariableQualifier(const ShaderVariable variable)
{
    switch (variable.ToScalar())
    {
        case ShaderVariable::Attribute: return "attribute";
        case ShaderVariable::Uniform:   return "uniform";
        case ShaderVariable::Varying:   return "varying";
        case ShaderVariable::InOut:
        default: return "";
    }
}

//----
std::string Shader::GetFunctionQualifier(const ShaderVariable variable, const ShaderProgram program)
{
    switch (program.ToScalar())
    {
        case ShaderProgram::Geometry:
        {
            //TODO : L O L ----------------
            return "";
        }
        case ShaderProgram::Vertex:
        {
            switch (variable.ToScalar())
            {
                case ShaderVariable::Attribute:  return "in";
                case ShaderVariable::Uniform:    return "in";
                case ShaderVariable::Varying:    return "inout";
                case ShaderVariable::InOut:      return "inout";
                default: return "";
            }
            return "";
        }
        case ShaderProgram::Pixel:
        {
            switch (variable.ToScalar())
            {
                case ShaderVariable::Attribute:  return "in";
                case ShaderVariable::Uniform:    return "in";
                case ShaderVariable::Varying:    return "in";
                case ShaderVariable::InOut:      return "inout";
                default: return "";
            }
            return "";
        }
        default:
        {
            return "";
        }
    }
}

//----
std::string Shader::GetProgramQualifier(const ShaderProgram program)
{
    switch (program.ToScalar())
    {
        case ShaderProgram::Geometry: return ""; //TODO : L O L ---------
        case ShaderProgram::Vertex:   return "[vert.glsl]";
        case ShaderProgram::Pixel:    return "[frag.glsl]";
        default: return "";
    }
}

//----
std::string Shader::GetProgramOutVariable(const ShaderProgram program)
{
    switch (program.ToScalar())
    {
        case ShaderProgram::Geometry: return ""; //TODO : L O L ---------
        case ShaderProgram::Vertex:   return "gl_Position";
        case ShaderProgram::Pixel:    return "gl_FragColor";
        default: return "";
    }
}

//----
std::string Shader::GetProgramOutVariableLocal(const ShaderProgram program)
{
    switch (program.ToScalar())
    {
        case ShaderProgram::Geometry: return ""; //TODO : L O L ---------
        case ShaderProgram::Vertex:   return "out_position";
        case ShaderProgram::Pixel:    return "out_frag_color";
        default: return "";
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
    std::string type = var.GetType();
    std::string name = Shader::GetVariablePrefix(qualifier) + var.m_name;
    ASSERT(!has_key(m_parameters[qualifier.ToScalar()], name));
    m_parameters[qualifier.ToScalar()][name] = type;
}

//----
void ShaderBlock::AddCallParameters(std::map<std::string, std::string> const& variables, std::string& result)
{
    for (auto const &key : variables)
    {
        if (result.length() > 0)
            result += ", ";
        result += key.first;
    }
}

//----
void ShaderBlock::AddDefinitionParameters(const ShaderVariable type, const ShaderProgram program, std::map<std::string, std::string>& variables, std::string& result)
{
    for (auto const &key : variables)
    {
        if (result.length() > 0)
            result += ", ";
        result += Shader::GetFunctionQualifier(type, program) + " ";
        result += key.second;
        result += " ";
        result += key.first;
    }
}

//----
void ShaderBlock::Build(const ShaderProgram program, std::string& call, std::string& function)
{
    ASSERT(m_name.length());
    ASSERT(m_parameters[ShaderVariable::InOut].size());

    //Build call in main
    std::string call_name = std::string("Call_") + m_name;
    call = call_name + "(";
    std::string call_parameters;
    for (int i = 0; i < ShaderVariable::MAX; i++)
        AddCallParameters(/*(ShaderVariable)i, */m_parameters[i], call_parameters);
    call += call_parameters + ");";

    //Build function declaration
    function = std::string("void ") + call_name + "(";
    std::string def_parameters;
    for (int i = 0; i < ShaderVariable::MAX; i++)
        AddDefinitionParameters((ShaderVariable)i, program, m_parameters[i], def_parameters);
    function += def_parameters + ")\n{\n"
        + m_code_main + (ends_with(m_code_main, "\n") ? std::string() : "\n") +
        "}";
}

//Shader Builder implementation class -----------------------------------------
ShaderBuilder::ShaderBuilder(std::string const& name, std::string const& version)
    : m_name(name), m_version(version)
{
    ASSERT(name.length());
    ASSERT(version.length());
}

//----
ShaderBuilder::~ShaderBuilder()
{
}

//----
std::string const& ShaderBuilder::GetName()
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
std::string ShaderBuilder::AddSlotOutVariableLocal(const ShaderProgram program)
{
    ShaderVariable var = ShaderVariable::InOut;
    std::string result = Shader::GetProgramOutVariableLocal(program);
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
void ShaderBuilder::MergeParameters(std::map<std::string, std::string>& variables, std::map<std::string, std::string>& merged)
{
    for (auto const &key : variables)
    {
        bool has_param = has_key(merged, key.first);

        //Key exists, check the type to make sure it's the same
        ASSERT(!has_param || (has_param && merged[key.first] == variables[key.first]),
            "has_param=%d, key=%s merged[key]=%s, variables[key]=%s\n",
            (int)has_param, key.first.c_str(), merged[key.first].c_str(), key.second.c_str());

        //does not exist, had it
        if (!has_param)
            merged[key.first] = key.second;
    }
}

//----
std::string ShaderBuilder::Build()
{
    std::string code;

    //Cleanup first
    for (int prog = 0; prog < ShaderProgram::MAX; prog++)
        for (int var = 0; var < ShaderVariable::MAX; var++)
            m_parameters[prog][var].clear();

    //Start building
    for (int prog = 0; prog < ShaderProgram::MAX; prog++)
    {
        //Add default local out in merged variables
        std::string out_local_var = AddSlotOutVariableLocal((ShaderProgram)prog);

        if (!out_local_var.length())
            continue;

        //Merge all variables
        for (int var = 0; var < ShaderVariable::MAX; var++)
            for (int block = 0; block < m_blocks[prog].count(); block++)
                MergeParameters(m_blocks[prog][block]->m_parameters[var], m_parameters[prog][var]);

        //Actually write code
        code += Shader::GetProgramQualifier((ShaderProgram)prog) + "\n";

        //Add actual code
        code += std::string("#version ") + m_version + "\n\n";

        // Safety for GLES
        code += "#if defined GL_ES\nprecision mediump float;\n#endif\n";

        //Added shader variables
        for (int var = 0; var < ShaderVariable::InOut; var++)
        {
            array<std::string> all_keys = keys(m_parameters[prog][var]);
            if (all_keys.count())
            {
                code += std::string("//- ") + Shader::GetVariableQualifier((ShaderVariable)var) + " ----\n";
                for (auto const &key : all_keys)
                {
                    code += Shader::GetVariableQualifier((ShaderVariable)var) + " ";
                    code += m_parameters[prog][var][key] + " " + key + ";\n";
                }
                if (var + 1 < ShaderVariable::InOut)
                    code += "\n";
            }
        }
        code += "\n";

        //Build Blocks code and add it
        array<std::string> calls;
        for (int block = 0; block < m_blocks[prog].count(); block++)
        {
            std::string call;
            std::string function;
            m_blocks[prog][block]->Build(ShaderProgram(prog), call, function);
            calls << call;
            if (m_blocks[prog][block]->m_code_custom.length())
            {
                code += std::string("//- ") + m_blocks[prog][block]->GetName() + " custom code ----\n";
                code += m_blocks[prog][block]->m_code_custom + "\n\n";
            }
            code += std::string("//- ") + m_blocks[prog][block]->GetName() + " main code ----\n";
            code += function + "\n\n";
        }

        //Added main definition
        code += "//- Main ----\nvoid main(void)\n{\n";

        //Add local variables
        int var = ShaderVariable::InOut;
        array<std::string> all_keys = keys(m_parameters[prog][var]);
        for (auto const &key : all_keys)
        {
            if (all_keys.count())
            {
                code += "    " + m_parameters[prog][var][key] + " " + key + ";\n";
            }
        }
        code += "\n";

        //Add calls
        code += "//- Calls ----\n";
        for (auto const &call : calls)
            code += "    " + call + "\n";
        code += "\n";

        code += "    " + Shader::GetProgramOutVariable((ShaderProgram)prog) + " = " + out_local_var + ";\n}\n\n";
    }

    return code;
}

} /* namespace lol */

#endif

