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
#include "lolgl.h"
#include "loldebug.h"

using namespace std;
using namespace lol;

#if USE_SDL && defined __APPLE__
#   include <SDL_main.h>
#endif

#if defined _WIN32
#   undef main /* FIXME: still needed? */
#endif

class Cube : public WorldEntity
{
public:
    Cube()
    {
        /* Front */
        m_vertices[0] = vec3(-1.0, -1.0,  1.0);
        m_vertices[1] = vec3( 1.0, -1.0,  1.0);
        m_vertices[2] = vec3( 1.0,  1.0,  1.0);
        m_vertices[3] = vec3(-1.0,  1.0,  1.0);
        /* Back */
        m_vertices[4] = vec3(-1.0, -1.0, -1.0);
        m_vertices[5] = vec3( 1.0, -1.0, -1.0);
        m_vertices[6] = vec3( 1.0,  1.0, -1.0);
        m_vertices[7] = vec3(-1.0,  1.0, -1.0);

        m_colors[0] = vec3(1.0, 0.0, 0.0);
        m_colors[1] = vec3(0.0, 1.0, 0.0);
        m_colors[2] = vec3(0.0, 0.0, 1.0);
        m_colors[3] = vec3(1.0, 1.0, 1.0);
        m_colors[4] = vec3(1.0, 0.0, 0.0);
        m_colors[5] = vec3(0.0, 1.0, 0.0);
        m_colors[6] = vec3(0.0, 0.0, 1.0);
        m_colors[7] = vec3(1.0, 1.0, 1.0);

        m_indices[0] = i16vec3(0, 1, 2);
        m_indices[1] = i16vec3(2, 3, 0);
        m_indices[2] = i16vec3(1, 5, 6);
        m_indices[3] = i16vec3(6, 2, 1);
        m_indices[4] = i16vec3(7, 6, 5);
        m_indices[5] = i16vec3(5, 4, 7);
        m_indices[6] = i16vec3(4, 0, 3);
        m_indices[7] = i16vec3(3, 7, 4);
        m_indices[8] = i16vec3(4, 5, 1);
        m_indices[9] = i16vec3(1, 0, 4);
        m_indices[10] = i16vec3(3, 2, 6);
        m_indices[11] = i16vec3(6, 7, 3);
    }

    virtual void TickGame(float deltams)
    {
real x(4.08f); x.print();
        WorldEntity::TickGame(deltams);

        m_angle += deltams;

        vec3 axis_y(0, 1, 0);
        mat4 anim = mat4::rotate(m_angle * 0.0001f, axis_y);
        mat4 model = mat4::translate(vec3(0, 0, -4));
        mat4 view = mat4::lookat(vec3(0, 2, 0), vec3(0, 0, -4), vec3(0, 1, 0));
        mat4 proj = mat4::perspective(M_PI / 4, 640.0f, 480.0f, -10.0f, 10.0f);
anim = mat4(1.0f);
model = mat4(1.0f);
view = mat4(1.0f);
        m_matrix = proj * view * model * anim;
proj.printf();
view.printf();
model.printf();
anim.printf();
printf("\n");
m_matrix.printf();
printf("\n");
m_vertices[0].printf();
(m_matrix * vec4(m_vertices[0], 1)).printf();
printf("\n");
    }

    virtual void TickDraw(float deltams)
    {
        WorldEntity::TickDraw(deltams);

        if (!m_ready)
        {
            m_shader = Shader::Create(
                "#version 120\n"
                "attribute vec3 coord3d;"
                "attribute vec3 v_color;"
                "uniform mat4 mvp;"
                "varying vec3 f_color;"
                "void main(void) {"
                "    gl_Position = mvp * vec4(coord3d, 1.0);"
                "    f_color = v_color;"
                "}",

                "#version 120\n"
                "varying vec3 f_color;"
                "void main(void) {"
                "    gl_FragColor = vec4(f_color, 1.0);"
                "    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);"
                "}");
            m_coord = m_shader->GetAttribLocation("coord3d");
            m_color = m_shader->GetAttribLocation("v_color");
            m_mvp = m_shader->GetUniformLocation("mvp");
            m_ready = true;

#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
            /* Method 1: store vertex buffer on the GPU memory */
            glGenBuffers(1, &m_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices,
                         GL_STATIC_DRAW);
            glGenBuffers(1, &m_cbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors,
                         GL_STATIC_DRAW);
            glGenBuffers(1, &m_ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices,
                         GL_STATIC_DRAW);
#else
#endif

            /* FIXME: this object never cleans up */
        }

        m_shader->Bind();
#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        glEnableVertexAttribArray(m_coord);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexAttribPointer(m_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(m_color);
        glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
        glVertexAttribPointer(m_color, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        int size;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

        glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(m_coord);
        glDisableVertexAttribArray(m_color);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, m_vertices);
        glDisableClientState(GL_VERTEX_ARRAY);
#endif
    }

private:
    float m_angle;
    mat4 m_matrix;
    vec3 m_vertices[8];
    vec3 m_colors[8];
    i16vec3 m_indices[12];
    Shader *m_shader;
#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
    GLuint m_vbo, m_cbo, m_ibo;
#endif
    int m_coord, m_color, m_mvp;
    bool m_ready;
};

int main()
{
    Application app("Tutorial 2: Cube", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new Cube();

    app.Run();

    return EXIT_SUCCESS;
}

