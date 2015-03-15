//
// Lol Engine - Input tutorial
//
// Copyright: (c) 2011-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
        m_controller = new Controller("Default");
        m_controller->SetInputCount(KEY_MAX, AXIS_MAX);

        m_keyboard = InputDevice::Get("Keyboard");
        if (m_keyboard)
            m_controller->GetKey(KEY_MANUAL_ROTATION).Bind("Keyboard", "Space");

        m_mouse = InputDevice::Get("Mouse");
        if (m_mouse)
        {
            m_controller->GetKey(KEY_DRAG_MESH).Bind("Mouse", "Left");
            m_controller->GetAxis(AXIS_DRAG_PITCH).Bind("Mouse", "Y");
            m_controller->GetAxis(AXIS_DRAG_YAW).Bind("Mouse", "X");
        }

        m_joystick = InputDevice::Get("Joystick1");
        if (m_joystick)
        {
            m_controller->GetAxis(AXIS_PITCH).Bind("Joystick1", "Axis2");
            m_controller->GetAxis(AXIS_YAW).Bind("Joystick1", "Axis1");
        }

        m_pitch_angle = 0;
        m_yaw_angle = 0;
        m_autorot = true;

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

        m_text = new Text("", "data/font/ascii.png");
        m_text->SetPos(vec3(5, 5, 1));
        Ticker::Ref(m_text);

        m_ready = false;
    }

    ~InputTutorial()
    {
        Ticker::Unref(m_text);
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        /* Handle keyboard */
        if (m_keyboard)
        {
            if (m_controller->GetKey(KEY_MANUAL_ROTATION).IsPressed())
                m_autorot = !m_autorot;
        }

        /* Handle joystick */
        if (m_joystick)
        {
            if (lol::abs(m_controller->GetAxis(AXIS_PITCH).GetValue()) > 0.2f)
                m_pitch_angle += m_controller->GetAxis(AXIS_PITCH).GetValue() * seconds * 100;
            if (lol::abs(m_controller->GetAxis(AXIS_YAW).GetValue()) > 0.2f)
                m_yaw_angle += m_controller->GetAxis(AXIS_YAW).GetValue() * seconds * 100;
        }

        /* Handle mouse */
        if (m_mouse)
        {
            if (m_controller->GetKey(KEY_DRAG_MESH).IsDown())
            {
                InputDevice::CaptureMouse(true);
                m_pitch_angle -= m_controller->GetAxis(AXIS_DRAG_PITCH).GetValue() * seconds * 100;
                m_yaw_angle += m_controller->GetAxis(AXIS_DRAG_YAW).GetValue() * seconds * 100;
            }
            else
            {
                InputDevice::CaptureMouse(false);
                if (m_autorot)
                    m_yaw_angle += seconds * 20;
            }

            m_text->SetText(String::Printf(
                "cursor: (%0.3f, %0.3f) - pixel (%d, %d)",
                m_mouse->GetCursor(0).x, m_mouse->GetCursor(0).y,
                m_mouse->GetCursorPixel(0).x, m_mouse->GetCursorPixel(0).y));
        }
        else
        {
            m_text->SetText("no mouse detected");
        }

        mat4 anim = mat4::fromeuler_yxz(m_yaw_angle, m_pitch_angle, 0.f);
        mat4 model = mat4::translate(vec3(0, 0, -4.5));
        mat4 view = mat4::lookat(vec3(0, 2, 0), vec3(0, 0, -4), vec3(0, 1, 0));
        mat4 proj = mat4::perspective(45.0f, 640.0f, 480.0f, 0.1f, 10.0f);

        m_matrix = proj * view * model * anim;
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        if (!m_ready)
        {
            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(07_input));

            m_mvp = m_shader->GetUniformLocation("in_Matrix");
            m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
            m_color = m_shader->GetAttribLocation(VertexUsage::Color, 0);

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

        g_renderer->SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        m_shader->Bind();
        m_vdecl->SetStream(m_vbo, m_coord, m_color);
        m_vdecl->Bind();

        m_shader->SetUniform(m_mvp, m_matrix);
        m_lines_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Lines, 0, 0,
                                    (int)m_mesh.Count(), 0, (int)m_lines_indices.Count());
        m_lines_ibo->Unbind();

        m_shader->SetUniform(m_mvp, m_matrix * mat4::scale(0.5f));
        m_faces_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, 0, 0,
                                    (int)m_mesh.Count(), 0, (int)m_faces_indices.Count());
        m_faces_ibo->Unbind();

        m_vdecl->Unbind();
    }

private:
    enum
    {
        KEY_MANUAL_ROTATION,
        KEY_DRAG_MESH,
        KEY_MAX
    };

    enum
    {
        AXIS_DRAG_PITCH,
        AXIS_DRAG_YAW,
        AXIS_PITCH,
        AXIS_YAW,
        AXIS_MAX
    };

    InputDevice *m_keyboard, *m_mouse, *m_joystick;
    Controller *m_controller;

    bool m_autorot;
    float m_pitch_angle;
    float m_yaw_angle;
    mat4 m_matrix;
    array<vec3,vec3> m_mesh;
    array<uint16_t> m_lines_indices, m_faces_indices;

    Shader *m_shader;
    ShaderAttrib m_coord, m_color;
    ShaderUniform m_mvp;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    IndexBuffer *m_lines_ibo, *m_faces_ibo;

    Text *m_text;
    bool m_ready;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 7: Input", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new InputTutorial();

    app.Run();

    return EXIT_SUCCESS;
}

