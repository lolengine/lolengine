//
//  Lol Engine — Input tutorial
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
#include "loldebug.h"

using namespace lol;

LOLFX_RESOURCE_DECLARE(07_input);

class InputTutorial : public WorldEntity
{
public:
    InputTutorial()
    {
        m_pitch_angle = 0;
        m_yaw_angle = 0;
        m_autorot = true;

        m_text = new Text("", "data/font/ascii.png");
        m_text->SetPos(vec3(5, 30, 1));
        Ticker::Ref(m_text);
    }

    ~InputTutorial()
    {
        Ticker::Unref(m_text);
    }

    virtual void tick_game(float seconds) override
    {
        WorldEntity::tick_game(seconds);

        auto mouse = input::mouse();
        auto keyboard = input::keyboard();

        /* Handle keyboard */
        if (keyboard->key_pressed(input::key::SC_Space))
            m_autorot = !m_autorot;

        /* Handle joystick */
        auto joystick = input::joystick(0);
        if ((bool)joystick)
        {
            if (lol::fabs(joystick->axis(input::axis::LeftY)) > 0.2f)
                m_pitch_angle += joystick->axis(input::axis::LeftY) * seconds;
            if (lol::fabs(joystick->axis(input::axis::LeftX)) > 0.2f)
                m_yaw_angle += joystick->axis(input::axis::LeftX) * seconds;
        }

        /* Handle mouse */
        if (true)
        {
            if (mouse->button(input::button::BTN_Left))
            {
                mouse->capture(true);
                m_pitch_angle += mouse->axis(input::axis::MoveY) * seconds * 0.1f;
                m_yaw_angle += mouse->axis(input::axis::MoveX) * seconds * 0.1f;
            }
            else
            {
                mouse->capture(false);
                if (m_autorot)
                    m_yaw_angle += seconds * 0.2f;
            }

            m_text->SetText(lol::format(
                "cursor: (%0.3f,%0.3f) - pixel (%d,%d)\n"
                "  move: (%0.3f,%0.3f) - pixel (%d,%d)",
                mouse->axis(input::axis::X), mouse->axis(input::axis::Y),
                (int)mouse->axis(input::axis::ScreenX),
                (int)mouse->axis(input::axis::ScreenY),
                mouse->axis(input::axis::MoveX), mouse->axis(input::axis::MoveY),
                (int)mouse->axis(input::axis::ScreenMoveX),
                (int)mouse->axis(input::axis::ScreenMoveY)));
        }
        else
        {
            m_text->SetText("no mouse detected");
        }

        mat4 anim = mat4::fromeuler_yxz(m_yaw_angle, m_pitch_angle, 0.f);
        mat4 model = mat4::translate(vec3(0, 0, -4.5));
        mat4 view = mat4::lookat(vec3(0, 2, 0), vec3(0, 0, -4), vec3(0, 1, 0));
        mat4 proj = mat4::perspective(radians(45.0f), 640.0f, 480.0f, 0.1f, 10.0f);

        m_matrix = proj * view * model * anim;
    }

    struct vertex { vec3 pos, color; };

    virtual bool init_draw() override
    {
        array<vertex> mesh
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

        array<uint16_t> faces_indices
        {
            0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
            7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
            4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3,
        };

        array<uint16_t> lines_indices
        {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7,
        };

        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(07_input));

        m_mvp = m_shader->GetUniformLocation("u_matrix");
        m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
        m_color = m_shader->GetAttribLocation(VertexUsage::Color, 0);

        m_vdecl = std::make_shared<VertexDeclaration>(
                                VertexStream<vec3,vec3>(VertexUsage::Position,
                                                        VertexUsage::Color));

        m_vbo = std::make_shared<VertexBuffer>(mesh.bytes());
        m_vbo->set_data(mesh.data(), mesh.bytes());

        m_lines_ibo = std::make_shared<IndexBuffer>(lines_indices.bytes());
        m_lines_ibo->set_data(lines_indices.data(), lines_indices.bytes());

        m_faces_ibo = std::make_shared<IndexBuffer>(faces_indices.bytes());
        m_faces_ibo->set_data(faces_indices.data(), faces_indices.bytes());

        return WorldEntity::init_draw();
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
    bool m_autorot;
    float m_pitch_angle;
    float m_yaw_angle;
    mat4 m_matrix;

    std::shared_ptr<Shader> m_shader;
    ShaderAttrib m_coord, m_color;
    ShaderUniform m_mvp;
    std::shared_ptr<VertexDeclaration> m_vdecl;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<IndexBuffer> m_lines_ibo, m_faces_ibo;

    Text *m_text;
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    Application app("Tutorial 7: Input", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new InputTutorial();

    app.Run();

    return EXIT_SUCCESS;
}

