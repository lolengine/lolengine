//
// Lol Engine - Triangle tutorial
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

LOLFX_RESOURCE_DECLARE(01_triangle);

class Triangle : public WorldEntity
{
public:
    Triangle()
    {
        m_vertices << vec2( 0.0,  0.8);
        m_vertices << vec2(-0.8, -0.8);
        m_vertices << vec2( 0.8, -0.8);
        m_ready = false;
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        if (!m_ready)
        {
            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(01_triangle));
            m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);

            m_vdecl = new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position));

            m_vbo = new VertexBuffer(m_vertices.Bytes());
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, &m_vertices[0], m_vertices.Bytes());
            m_vbo->Unlock();

            m_ready = true;

            /* FIXME: this object never cleans up */
        }

        m_shader->Bind();
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 3);
        m_vdecl->Unbind();
    }

private:
    Array<vec2> m_vertices;
    Shader *m_shader;
    ShaderAttrib m_coord;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    bool m_ready;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 1: Triangle", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new Triangle();

    app.Run();
    return EXIT_SUCCESS;
}

