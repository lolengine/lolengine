//
//  Lol Engine — Noise tutorial
//
//  Copyright © 2012—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>
#include "loldebug.h"

using namespace lol;

LOLFX_RESOURCE_DECLARE(03_noise);

class NoiseDemo : public WorldEntity
{
public:
    NoiseDemo()
      : m_vertices({ vec2(-1.0,  1.0),
                     vec2(-1.0, -1.0),
                     vec2( 1.0, -1.0),
                     vec2(-1.0,  1.0),
                     vec2( 1.0, -1.0),
                     vec2( 1.0,  1.0), }),
        m_time(0.0),
        m_ready(false)
    {
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        m_time += seconds;

        if (!m_ready)
        {
            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(03_noise));
            m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
            m_time_uni = m_shader->GetUniformLocation("u_time");

            m_vdecl = new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position));

            m_vbo = new VertexBuffer(m_vertices.bytes());
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, &m_vertices[0], m_vertices.bytes());
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
    array<vec2> m_vertices;
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

    new NoiseDemo();

    app.Run();
    return EXIT_SUCCESS;
}

