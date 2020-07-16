//
//  Lol Engine — Cube tutorial
//
//  Copyright © 2011—2020 Sam Hocevar <sam@hocevar.net>
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
#include <lol/color>
#include "loldebug.h"

using namespace lol;

LOLFX_RESOURCE_DECLARE(02_cube);

class Cube : public WorldEntity
{
public:
    Cube()
    {
        m_camera = new Camera();
        m_camera->SetProjection(mat4::perspective(radians(30.f), 640.f, 480.f, .1f, 1000.f));
        m_camera->SetView(mat4::lookat(vec3(-15.f, 5.f, 0.f),
            vec3(0.f, -1.f, 0.f),
            vec3(0.f, 1.f, 0.f)));
        Scene& scene = Scene::GetScene();
        scene.PushCamera(m_camera);
        Ticker::Ref(m_camera);
    }

    ~Cube()
    {
        Scene& scene = Scene::GetScene();
        scene.PopCamera(m_camera);
        Ticker::Unref(m_camera);
    }

    struct vertex { vec3 pos, color; };

    virtual bool init_draw() override
    {
        std::vector<vertex> mesh
        {
            // Front vertices/colors
            { vec3(-1.0, -1.0,  1.0), vec3(1.0, 0.0, 1.0) },
            { vec3( 1.0, -1.0,  1.0), vec3(0.0, 1.0, 0.0) },
            { vec3( 1.0,  1.0,  1.0), vec3(1.0, 0.5, 0.0) },
            { vec3(-1.0,  1.0,  1.0), vec3(1.0, 1.0, 0.0) },
            // Back
            { vec3(-1.0, -1.0, -1.0), vec3(1.0, 0.0, 0.0) },
            { vec3( 1.0, -1.0, -1.0), vec3(0.0, 0.5, 0.0) },
            { vec3( 1.0,  1.0, -1.0), vec3(0.0, 0.5, 1.0) },
            { vec3(-1.0,  1.0, -1.0), vec3(0.0, 0.0, 1.0) },
        };

        std::vector<uint16_t> lines_indices
        {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7,
        };

        std::vector<uint16_t> faces_indices
        {
            0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
            7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
            4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3,
        };

        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(02_cube));

        m_mvp = m_shader->GetUniformLocation("u_matrix");
        m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
        m_color = m_shader->GetAttribLocation(VertexUsage::Color, 0);

        m_vdecl = std::make_shared<VertexDeclaration>(
                                VertexStream<vec3,vec3>(VertexUsage::Position,
                                                        VertexUsage::Color));

        m_vbo = std::make_shared<VertexBuffer>(mesh.size() * sizeof(mesh[0]));
        m_vbo->set_data(mesh.data(), mesh.size() * sizeof(mesh[0]));

        m_lines_ibo = std::make_shared<IndexBuffer>(lines_indices.size() * sizeof(lines_indices[0]));
        m_lines_ibo->set_data(lines_indices.data(), lines_indices.size() * sizeof(lines_indices[0]));

        m_faces_ibo = std::make_shared<IndexBuffer>(faces_indices.size() * sizeof(faces_indices[0]));
        m_faces_ibo->set_data(faces_indices.data(), faces_indices.size() * sizeof(faces_indices[0]));

        return true;
    }

    virtual void tick_game(float seconds) override
    {
        WorldEntity::tick_game(seconds);

        m_angle += seconds * radians(45.0f);

        mat4 anim = mat4::rotate(m_angle, vec3(0, 1, 0));
        mat4 model = mat4::translate(vec3(0, 0, -4.5));
        mat4 view = mat4::lookat(vec3(0, 2, 0), vec3(0, 0, -4), vec3(0, 1, 0));
        mat4 proj = mat4::perspective(radians(45.0f), 640.0f, 480.0f, 0.1f, 10.0f);

        m_matrix = proj * view * model * anim;

        {
            auto context0 = Debug::DrawContext::New(color::red);
            {
                auto context1 = Debug::DrawContext::New(color::blue);
                Debug::DrawBox(box3(vec3(0.f), vec3(1.2f)));
                Debug::DrawGrid(vec3(0.f), vec3::axis_x, vec3::axis_y, vec3::axis_z, 10.0f);
                {
                    auto context2 = Debug::DrawContext::New(context0);
                    Debug::DrawBox(box3(vec3(0.f), vec3(1.3f)));
                }
                {
                    auto context2 = Debug::DrawContext::New(context0);
                    context2.SetColor(color::yellow);
                    Debug::DrawBox(box3(vec3(-1.f), vec3(1.4f)));
                }
            }
            Debug::DrawBox(box3(vec3(0.f), vec3(1.1f)));
        }
    }

    virtual void tick_draw(float seconds, Scene &scene) override
    {
        WorldEntity::tick_draw(seconds, scene);

        scene.get_renderer()->clear_color(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        m_shader->Bind();
        m_vdecl->Bind();
        m_vdecl->SetStream(m_vbo, m_coord, m_color);

        m_shader->SetUniform(m_mvp, m_matrix);
        m_lines_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Lines, m_lines_ibo->size() / sizeof(uint16_t));
        m_lines_ibo->Unbind();

        m_shader->SetUniform(m_mvp, m_matrix * mat4::scale(0.5f));
        m_faces_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, m_faces_ibo->size() / sizeof(uint16_t));
        m_faces_ibo->Unbind();

        m_vdecl->Unbind();
    }

    virtual bool release_draw() override
    {
        m_shader.reset();
        m_vdecl.reset();
        m_vbo.reset();
        m_lines_ibo.reset();
        m_faces_ibo.reset();
        return true;
    }

private:
    Camera* m_camera = nullptr;
    float m_angle = 0.0f;
    mat4 m_matrix;

    std::shared_ptr<Shader> m_shader;
    ShaderAttrib m_coord, m_color;
    ShaderUniform m_mvp;
    std::shared_ptr<VertexDeclaration> m_vdecl;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<IndexBuffer> m_lines_ibo, m_faces_ibo;
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    auto app = app::init("Tutorial 2: Cube", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new Cube();

    app->run();

    return EXIT_SUCCESS;
}
