//
// Lol Engine - Triangle tutorial
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

class Triangle : public WorldEntity
{
public:
    Triangle()
    {
        m_vertices[0] = vec2( 0.0,  0.8);
        m_vertices[1] = vec2(-0.8, -0.8);
        m_vertices[2] = vec2( 0.8, -0.8);
    }

    virtual void TickDraw(float deltams)
    {
        WorldEntity::TickDraw(deltams);

        if (!m_ready)
        {
            m_shader = Shader::Create(
                "#version 120\n"
                "attribute vec2 coord2d;"
                "void main(void) {"
                "    gl_Position = vec4(coord2d, 0.0, 1.0);"
                "}",

                "#version 120\n"
                "void main(void) {"
                "    gl_FragColor[0] = 0.0;"
                "    gl_FragColor[1] = 0.0;"
                "    gl_FragColor[2] = 1.0;"
                "}");
            m_attrib = m_shader->GetAttribLocation("coord2d");
            m_ready = true;

#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
            /* Method 1: store vertex buffer on the GPU memory */
            glGenBuffers(1, &m_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices,
                         GL_STATIC_DRAW);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
            /* Method 2: upload vertex information at each frame */
#else
#endif

            /* FIXME: this object never cleans up */
        }

        m_shader->Bind();
#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glEnableVertexAttribArray(m_attrib);
        glVertexAttribPointer(m_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        /* Never used for now */
        glEnableVertexAttribArray(m_attrib);
        glVertexAttribPointer(m_attrib, 2, GL_FLOAT, GL_FALSE, 0, m_vertices);
#else
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, m_vertices);
#endif

        glDrawArrays(GL_TRIANGLES, 0, 3);

#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        glDisableVertexAttribArray(m_attrib);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        /* Never used for now */
        glDisableVertexAttribArray(m_attrib);
#else
        glDisableClientState(GL_VERTEX_ARRAY);
#endif
    }

private:
    vec2 m_vertices[3];
    Shader *m_shader;
#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
    GLuint m_vbo;
#endif
    int m_attrib;
    bool m_ready;
};

int main()
{
    Application app("Tutorial 1: Triangle", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new Triangle();

    app.Run();

    return EXIT_SUCCESS;
}

