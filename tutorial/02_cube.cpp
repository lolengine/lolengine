//
// Lol Engine - Cube tutorial
//
// Copyright: (c) 2011-2013 Sam Hocevar <sam@hocevar.net>
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

LOLFX_RESOURCE_DECLARE(02_cube);

class Cube : public WorldEntity
{
public:
    Cube()
    {
        m_angle = 0;

        /* Front vertices/colors */
        m_mesh.Push(vec3(-1.0, -1.0,  1.0), vec3(1.0, 0.0, 1.0));
        m_mesh.Push(vec3( 1.0, -1.0,  1.0), vec3(0.0, 1.0, 0.0));
        m_mesh.Push(vec3( 1.0,  1.0,  1.0), vec3(1.0, 0.5, 0.0));
        m_mesh.Push(vec3(-1.0,  1.0,  1.0), vec3(1.0, 1.0, 0.0));
        /* Back */
        m_mesh.Push(vec3(-1.0, -1.0, -1.0), vec3(1.0, 0.0, 0.0));
        m_mesh.Push(vec3( 1.0, -1.0, -1.0), vec3(0.0, 0.5, 0.0));
        m_mesh.Push(vec3( 1.0,  1.0, -1.0), vec3(0.0, 0.5, 1.0));
        m_mesh.Push(vec3(-1.0,  1.0, -1.0), vec3(0.0, 0.0, 1.0));

        m_faces_indices << 0 << 1 << 2 << 2 << 3 << 0;
        m_faces_indices << 1 << 5 << 6 << 6 << 2 << 1;
        m_faces_indices << 7 << 6 << 5 << 5 << 4 << 7;
        m_faces_indices << 4 << 0 << 3 << 3 << 7 << 4;
        m_faces_indices << 4 << 5 << 1 << 1 << 0 << 4;
        m_faces_indices << 3 << 2 << 6 << 6 << 7 << 3;

        m_lines_indices << 0 << 1 << 1 << 2 << 2 << 3 << 3 << 0;
        m_lines_indices << 4 << 5 << 5 << 6 << 6 << 7 << 7 << 4;
        m_lines_indices << 0 << 4 << 1 << 5 << 2 << 6 << 3 << 7;

        m_ready = false;
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        m_angle += seconds * 45.0f;

        mat4 anim = mat4::rotate(m_angle, vec3(0, 1, 0));
        mat4 model = mat4::translate(vec3(0, 0, -4.5));
        mat4 view = mat4::lookat(vec3(0, 2, 0), vec3(0, 0, -4), vec3(0, 1, 0));
        mat4 proj = mat4::perspective(45.0f, 640.0f, 480.0f, 0.1f, 10.0f);

        m_matrix = proj * view * model * anim;
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        if (!m_ready)
        {
            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(02_cube));

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

            m_lines_ibo = new IndexBuffer(m_lines_indices.Bytes());
            void *indices = m_lines_ibo->Lock(0, 0);
            memcpy(indices, &m_lines_indices[0], m_lines_indices.Bytes());
            m_lines_ibo->Unlock();

            m_faces_ibo = new IndexBuffer(m_faces_indices.Bytes());
            indices = m_faces_ibo->Lock(0, 0);
            memcpy(indices, &m_faces_indices[0], m_faces_indices.Bytes());
            m_faces_ibo->Unlock();

            /* FIXME: this object never cleans up */
            m_ready = true;
        }

        Video::SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        m_shader->Bind();
        m_vdecl->SetStream(m_vbo, m_coord, m_color);
        m_vdecl->Bind();

        m_shader->SetUniform(m_mvp, m_matrix);
        m_lines_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Lines, 0, 0,
                                     m_mesh.Count(), 0, m_lines_indices.Count());
        m_lines_ibo->Unbind();

        m_shader->SetUniform(m_mvp, m_matrix * mat4::scale(0.5f));
        m_faces_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, 0, 0,
                                     m_mesh.Count(), 0, m_faces_indices.Count());
        m_faces_ibo->Unbind();

        m_vdecl->Unbind();
    }

private:
    float m_angle;
    mat4 m_matrix;
    Array<vec3,vec3> m_mesh;
    Array<uint16_t> m_lines_indices, m_faces_indices;

    Shader *m_shader;
    ShaderAttrib m_coord, m_color;
    ShaderUniform m_mvp;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    IndexBuffer *m_lines_ibo, *m_faces_ibo;

    bool m_ready;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 2: Cube", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new Cube();

    app.Run();

    return EXIT_SUCCESS;
}

