//
// Lol Engine - Cube tutorial
//
// Copyright: (c) 2011 Sam Hocevar <sam@hocevar.net>
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

#if defined _WIN32
#   undef main /* FIXME: still needed? */
#   include <direct.h>
#endif

class Cube : public WorldEntity
{
public:
    Cube()
    {
        m_angle = 0;

        /* Front vertices/colors */
        m_mesh += vec3(-1.0, -1.0,  1.0); m_mesh += vec3(1.0, 0.0, 1.0);
        m_mesh += vec3( 1.0, -1.0,  1.0); m_mesh += vec3(0.0, 1.0, 0.0);
        m_mesh += vec3( 1.0,  1.0,  1.0); m_mesh += vec3(1.0, 0.5, 0.0);
        m_mesh += vec3(-1.0,  1.0,  1.0); m_mesh += vec3(1.0, 1.0, 0.0);
        /* Back */
        m_mesh += vec3(-1.0, -1.0, -1.0); m_mesh += vec3(1.0, 0.0, 0.0);
        m_mesh += vec3( 1.0, -1.0, -1.0); m_mesh += vec3(0.0, 0.5, 0.0);
        m_mesh += vec3( 1.0,  1.0, -1.0); m_mesh += vec3(0.0, 0.5, 1.0);
        m_mesh += vec3(-1.0,  1.0, -1.0); m_mesh += vec3(0.0, 0.0, 1.0);

        m_indices += i16vec3(0, 1, 2);
        m_indices += i16vec3(2, 3, 0);
        m_indices += i16vec3(1, 5, 6);
        m_indices += i16vec3(6, 2, 1);
        m_indices += i16vec3(7, 6, 5);
        m_indices += i16vec3(5, 4, 7);
        m_indices += i16vec3(4, 0, 3);
        m_indices += i16vec3(3, 7, 4);
        m_indices += i16vec3(4, 5, 1);
        m_indices += i16vec3(1, 0, 4);
        m_indices += i16vec3(3, 2, 6);
        m_indices += i16vec3(6, 7, 3);

        m_ready = false;
    }

    virtual void TickGame(float deltams)
    {
        WorldEntity::TickGame(deltams);

        m_angle += deltams / 1000.0f * 45.0f;

        mat4 anim = mat4::rotate(m_angle, vec3(0, 1, 0));
        mat4 model = mat4::translate(vec3(0, 0, -4.5));
        mat4 view = mat4::lookat(vec3(0, 2, 0), vec3(0, 0, -4), vec3(0, 1, 0));
        mat4 proj = mat4::perspective(45.0f, 640.0f, 480.0f, 0.1f, 10.0f);

        m_matrix = proj * view * model * anim;
    }

    virtual void TickDraw(float deltams)
    {
        WorldEntity::TickDraw(deltams);

        if (!m_ready)
        {
            m_shader = Shader::Create(
#if !defined __CELLOS_LV2__ && !defined _XBOX && !defined USE_D3D9
                "#version 120\n"
                "attribute vec3 in_Vertex;"
                "attribute vec3 in_Color;"
                "uniform mat4 in_Matrix;"
                "varying vec3 pass_Color;"
                ""
                "void main(void) {"
                "    gl_Position = in_Matrix * vec4(in_Vertex, 1.0);"
                "    pass_Color = in_Color;"
                "}",

                "#version 120\n"
                "varying vec3 pass_Color;"
                ""
                "void main(void) {"
                "    gl_FragColor = vec4(pass_Color, 1.0);"
                "}"
#else
                "void main(float3 in_Vertex : POSITION,"
                "          float3 in_Color : COLOR,"
                "          uniform float4x4 in_Matrix,"
                "          out float4 out_Position : POSITION,"
                "          out float3 pass_Color : COLOR) {"
                "    pass_Color = in_Color;"
                "    out_Position = mul(in_Matrix, float4(in_Vertex, 1.0));"
                "}",

                "void main(float3 pass_Color : COLOR,"
                "          out float4 out_FragColor : COLOR) {"
                "    out_FragColor = float4(pass_Color, 1.0);"
                "}"
#endif
            );
            m_mvp = m_shader->GetUniformLocation("in_Matrix");
            m_coord = m_shader->GetAttribLocation("in_Vertex",
                                                  VertexUsage::Position, 0);
            m_color = m_shader->GetAttribLocation("in_Color",
                                                  VertexUsage::Color, 0);

            m_vdecl =
              new VertexDeclaration(VertexStream<vec3,vec3>(VertexUsage::Position,
                                                            VertexUsage::Color));

            m_vbo = new VertexBuffer(m_mesh.Bytes());
            void *mesh = m_vbo->Lock(0, 0);
            memcpy(mesh, &m_mesh[0], m_mesh.Bytes());
            m_vbo->Unlock();

            m_ibo = new IndexBuffer(m_indices.Bytes());
            void *indices = m_ibo->Lock(0, 0);
            memcpy(indices, &m_indices[0], m_indices.Bytes());
            m_ibo->Unlock();

            /* FIXME: this object never cleans up */
            m_ready = true;
        }

        Video::SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        m_shader->Bind();
        m_shader->SetUniform(m_mvp, m_matrix);
        m_vdecl->SetStream(m_vbo, m_coord, m_color);
        m_vdecl->Bind();
        m_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, 0, 0,
                                     m_mesh.Count(), 0, m_indices.Count());
        m_ibo->Unbind();
        m_vdecl->Unbind();
    }

private:
    float m_angle;
    mat4 m_matrix;
    Array<vec3> m_mesh;
    Array<i16vec3> m_indices;

    Shader *m_shader;
    ShaderAttrib m_coord, m_color;
    ShaderUniform m_mvp;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    IndexBuffer *m_ibo;

    bool m_ready;
};

int main(int argc, char **argv)
{
    Application app("Tutorial 2: Cube", ivec2(640, 480), 60.0f);

#if defined _MSC_VER && !defined _XBOX
    _chdir("..");
#elif defined _WIN32 && !defined _XBOX
    _chdir("../..");
#endif

    new DebugFps(5, 5);
    new Cube();

    app.Run();

    return EXIT_SUCCESS;
}

