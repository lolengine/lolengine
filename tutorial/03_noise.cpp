//
// Lol Engine - Noise tutorial
//
// Copyright: (c) 2012-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "loldebug.h"

using namespace std;
using namespace lol;

#if defined _WIN32
#   include <direct.h>
#endif

extern char const *lolfx_03_noise;

class NoiseDemo : public WorldEntity
{
public:
    NoiseDemo() :
        m_time(0.0),
        m_ready(false)
    {
        m_vertices << vec2(-1.0,  1.0);
        m_vertices << vec2(-1.0, -1.0);
        m_vertices << vec2( 1.0, -1.0);
        m_vertices << vec2(-1.0,  1.0);
        m_vertices << vec2( 1.0, -1.0);
        m_vertices << vec2( 1.0,  1.0);
        m_ready = false;
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        m_time += seconds;

        if (!m_ready)
        {
            m_shader = Shader::Create(lolfx_03_noise);
            m_coord = m_shader->GetAttribLocation("in_Position", VertexUsage::Position, 0);
            m_time_uni = m_shader->GetUniformLocation("u_Time");

            m_vdecl = new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position));

            m_vbo = new VertexBuffer(m_vertices.Bytes());
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, &m_vertices[0], m_vertices.Bytes());
            m_vbo->Unlock();

            m_ready = true;

            /* FIXME: this object never cleans up */
        }

        m_shader->Bind();
        m_shader->SetUniform(m_time_uni, m_time);
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
    }

private:
    Array<vec2> m_vertices;
    Shader *m_shader;
    ShaderAttrib m_coord;
    ShaderUniform m_time_uni;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    float m_time;
    bool m_ready;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 3: Noise", ivec2(1280, 720), 60.0f);

#if defined _MSC_VER && !defined _XBOX
    _chdir("..");
#elif defined _WIN32 && !defined _XBOX
    _chdir("../..");
#endif

    new NoiseDemo();

    app.Run();
    return EXIT_SUCCESS;
}

