//
// Lol Engine - Triangle tutorial
//
// Copyright: (c) 2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "loldebug.h"

using namespace std;
using namespace lol;

#if USE_SDL && defined __APPLE__
#   include <SDL_main.h>
#endif

#if defined __native_client__
#   define main old_main
#endif

#if defined _WIN32
#   undef main /* FIXME: still needed? */
#   include <direct.h>
#endif

class Triangle : public WorldEntity
{
public:
    Triangle()
    {
        m_vertices[0] = vec2( 0.0,  0.8);
        m_vertices[1] = vec2(-0.8, -0.8);
        m_vertices[2] = vec2( 0.8, -0.8);
        m_ready = false;
    }

    virtual void TickDraw(float deltams)
    {
        WorldEntity::TickDraw(deltams);

        if (!m_ready)
        {
            m_shader = Shader::Create(
#if !defined __CELLOS_LV2__ && !defined _XBOX && !defined USE_D3D9
                "#version 120\n"
                "attribute vec2 in_Position;"
                "void main(void) {"
                "    gl_Position = vec4(in_Position, 0.0, 1.0);"
                "}",

                "#version 120\n"
                "void main(void) {"
                "    gl_FragColor = vec4(0.7, 0.5, 0.2, 1.0);"
                "}"
#else
                "void main(float2 in_Position : POSITION,"
                "          out float4 out_Position : POSITION) {"
                "    out_Position = float4(in_Position, 0.0, 1.0);"
                "}",

                "void main(out float4 out_FragColor : COLOR) {"
                "    out_FragColor = float4(0.7, 0.5, 0.2, 1.0);"
                "}"
#endif
            );
            m_coord = m_shader->GetAttribLocation("in_Position", VertexUsage::Position, 0);

            m_vdecl = new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position));

            m_vbo = new VertexBuffer(sizeof(m_vertices));
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, m_vertices, sizeof(m_vertices));
            m_vbo->Unlock();

            m_ready = true;

            /* FIXME: this object never cleans up */
        }

        m_shader->Bind();
        m_vdecl->Bind();
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 1);
        m_vdecl->Unbind();
    }

private:
    vec2 m_vertices[3];
    Shader *m_shader;
    ShaderAttrib m_coord;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    bool m_ready;
};

int main(int argc, char **argv)
{
    Application app("Tutorial 1: Triangle", ivec2(640, 480), 60.0f);

#if defined _MSC_VER && !defined _XBOX
    _chdir("..");
#elif defined _WIN32 && !defined _XBOX
    _chdir("../..");
#endif

    new DebugFps(5, 5);
    new Triangle();

    app.Run();
    return EXIT_SUCCESS;
}

