//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>
#include <cstdio>

#if defined _WIN32
#   define WIN32_LEAN_AND_MEAN 1
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
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

    GLuint prog_id, vert_id, frag_id;
    // Benlitz: using a simple array could be faster since there is never more than a few attribute locations to store
    map<uint64_t, GLint> attrib_locations;
    map<uint64_t, bool> attrib_errors;
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
    String m_section;
    map<String, String> m_programs;

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
    lolfx_parser(String const &code)
      : m_section("header")
    {
        pegtl::parse_string<lolfx, action>(code.C(), "shader", this);
    }
};

template<>
struct lolfx_parser::action<lolfx_parser::do_title>
{
    static void apply(action_input const &in, lolfx_parser *that)
    {
        that->m_section = in.string().c_str();
    }
};

template<>
struct lolfx_parser::action<lolfx_parser::code_section>
{
    static void apply(action_input const &in, lolfx_parser *that)
    {
        that->m_programs[that->m_section] = in.string().c_str();
    }
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

    char errbuf[4096];
    String shader_code;
    GLchar const *gl_code;
    GLint status;
    GLsizei len;

    /* Compile vertex shader */
    data->vert_crc = ShaderData::Hash(vert);

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

    /* Compile fragment shader */
    data->frag_crc = ShaderData::Hash(frag);

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
    return ret;
}

ShaderUniform Shader::GetUniformLocation(String const& uni) const
{
    return GetUniformLocation(uni.C());
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

    delete data;
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

    patched_code.resize((int)strlen(patched_code.C()));

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

