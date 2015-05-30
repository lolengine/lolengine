//
//  Lol Engine — Input tutorial
//
//  Copyright © 2011—2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
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
        m_controller = new Controller("Default");

#       ifdef OLD_SCHOOL
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
#       else
        m_profile
            << InputProfile::Keyboard(KEY_MANUAL_ROTATION, "Space")
            << InputProfile::MouseKey(KEY_DRAG_MESH, "Left")
            << InputProfile::JoystickAxis(1, AXIS_PITCH, "Axis2")
            << InputProfile::JoystickAxis(1, AXIS_YAW, "Axis1")
            << InputProfile::MouseAxis(AXIS_DRAG_PITCH, "Y")
            << InputProfile::MouseAxis(AXIS_DRAG_YAW, "X");

        m_controller->Init(m_profile);
        m_keyboard = InputDevice::GetKeyboard();
        m_mouse = InputDevice::GetMouse();
        m_joystick = InputDevice::GetJoystick(1);
#       endif //OLD_SCHOOL

        m_pitch_angle = 0;
        m_yaw_angle = 0;
        m_autorot = true;

        /* Front vertices/colors */
        m_mesh.push(vec3(-1.0, -1.0,  1.0), vec3(1.0, 0.0, 1.0));
        m_mesh.push(vec3( 1.0, -1.0,  1.0), vec3(0.0, 1.0, 0.0));
        m_mesh.push(vec3( 1.0,  1.0,  1.0), vec3(1.0, 0.5, 0.0));
        m_mesh.push(vec3(-1.0,  1.0,  1.0), vec3(1.0, 1.0, 0.0));
        /* Back */
        m_mesh.push(vec3(-1.0, -1.0, -1.0), vec3(1.0, 0.0, 0.0));
        m_mesh.push(vec3( 1.0, -1.0, -1.0), vec3(0.0, 0.5, 0.0));
        m_mesh.push(vec3( 1.0,  1.0, -1.0), vec3(0.0, 0.5, 1.0));
        m_mesh.push(vec3(-1.0,  1.0, -1.0), vec3(0.0, 0.0, 1.0));

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
            if (m_controller->WasKeyPressedThisFrame(KEY_MANUAL_ROTATION))
                m_autorot = !m_autorot;
        }

        /* Handle joystick */
        if (m_joystick)
        {
            if (lol::abs(m_controller->GetAxisValue(AXIS_PITCH)) > 0.2f)
                m_pitch_angle += m_controller->GetAxisValue(AXIS_PITCH) * seconds * 100;
            if (lol::abs(m_controller->GetAxisValue(AXIS_YAW)) > 0.2f)
                m_yaw_angle += m_controller->GetAxisValue(AXIS_YAW) * seconds * 100;
        }

        /* Handle mouse */
        if (m_mouse)
        {
            if (m_controller->IsKeyPressed(KEY_DRAG_MESH))
            {
                InputDevice::CaptureMouse(true);
                m_pitch_angle -= m_controller->GetAxisValue(AXIS_DRAG_PITCH) * seconds * 100;
                m_yaw_angle += m_controller->GetAxisValue(AXIS_DRAG_YAW) * seconds * 100;
            }
            else
            {
                InputDevice::CaptureMouse(false);
                if (m_autorot)
                    m_yaw_angle += seconds * 20;
            }

            m_text->SetText(String::format(
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

            m_vbo = new VertexBuffer(m_mesh.bytes());
            void *mesh = m_vbo->Lock(0, 0);
            memcpy(mesh, &m_mesh[0], m_mesh.bytes());
            m_vbo->Unlock();

            m_lines_ibo = new IndexBuffer(m_lines_indices.bytes());
            void *indices = m_lines_ibo->Lock(0, 0);
            memcpy(indices, &m_lines_indices[0], m_lines_indices.bytes());
            m_lines_ibo->Unlock();

            m_faces_ibo = new IndexBuffer(m_faces_indices.bytes());
            indices = m_faces_ibo->Lock(0, 0);
            memcpy(indices, &m_faces_indices[0], m_faces_indices.bytes());
            m_faces_ibo->Unlock();

            /* FIXME: this object never cleans up */
            m_ready = true;
        }

        Renderer::Get()->SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        m_shader->Bind();
        m_vdecl->SetStream(m_vbo, m_coord, m_color);
        m_vdecl->Bind();

        m_shader->SetUniform(m_mvp, m_matrix);
        m_lines_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Lines, 0, 0,
                                    m_mesh.count(), 0, m_lines_indices.count());
        m_lines_ibo->Unbind();

        m_shader->SetUniform(m_mvp, m_matrix * mat4::scale(0.5f));
        m_faces_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, 0, 0,
                                    m_mesh.count(), 0, m_faces_indices.count());
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
    InputProfile m_profile;

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

