//
// Lol Engine - Fractal tutorial
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

#include <cstring>

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
#   include <direct.h>
#endif

class Fractal : public WorldEntity
{
public:
    Fractal(ivec2 const &size)
    {
        m_size = size;
        m_pixels = new u8vec4[size.x * size.y];
        m_angle = 0.0f;
        m_ready = false;
    }

    ~Fractal()
    {
        delete m_pixels;
    }

    virtual void TickGame(float deltams)
    {
        WorldEntity::TickGame(deltams);

        m_angle += deltams * 0.0005f;

        cmplx r0(cosf(m_angle), sinf(m_angle));
        for (int j = 0; j < m_size.y; j++)
        for (int i = 0; i < m_size.x; i++)
        {
            float const maxlen = 128.0f;
            int const maxiter = 20;

            cmplx x0(4.0f / m_size.x * i - 2.5f, 3.0f / m_size.y * j - 1.5f);
            cmplx r = x0;
            cmplx z;
            int iter = maxiter;
            for (z = r; iter && z.sqlen() < maxlen * maxlen; z = z * z + r)
                --iter;

            float f = iter;
            float n = z.len();
            f += logf(logf(n) / logf(maxlen)) / logf(2.0f);

            if (iter)
            {
                uint8_t red = 255 - f * 11;
                uint8_t green = 255 - f * 11;
                uint8_t blue = (f * 23 > 255) ? 511 - f * 23 : 255;
                m_pixels[j * m_size.x + i] = u8vec4(red, green, blue, 0);
            }
            else
            {
                m_pixels[j * m_size.x + i] = u8vec4(0, 0, 0, 0);
            }
        }
    }

    virtual void TickDraw(float deltams)
    {
        WorldEntity::TickDraw(deltams);

        static float const vertices[] =
        {
            -1.0f, -1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,
             1.0f,  1.0f,
            -1.0f,  1.0f,
            -1.0f, -1.0f,
        };

        static float const texcoords[] =
        {
             0.0f,  0.0f,
             1.0f,  0.0f,
             1.0f,  1.0f,
             1.0f,  1.0f,
             0.0f,  1.0f,
             0.0f,  0.0f,
        };

        if (!m_ready)
        {
            glGenTextures(1, &m_texid);
            glBindTexture(GL_TEXTURE_2D, m_texid);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            m_shader = Shader::Create(
                "#version 120\n"
                "attribute vec2 in_TexCoord;\n"
                "attribute vec2 in_Vertex;"
                "void main(void) {"
                "    gl_Position = vec4(in_Vertex, 0.0, 1.0);"
                "    gl_TexCoord[0] = vec4(in_TexCoord, 0.0, 0.0);\n"
                "}",

                "#version 120\n"
                "uniform sampler2D in_Texture;\n"
                "void main(void) {"
                "    gl_FragColor = texture2D(in_Texture, gl_TexCoord[0].xy);"
                "}");
            m_vertexattrib = m_shader->GetAttribLocation("in_Vertex");
            m_texattrib = m_shader->GetAttribLocation("in_TexCoord");
            m_ready = true;

#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
            /* Method 1: store vertex buffer on the GPU memory */
            glGenBuffers(1, &m_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                         GL_STATIC_DRAW);
            glGenBuffers(1, &m_tbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords,
                         GL_STATIC_DRAW);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
            /* Method 2: upload vertex information at each frame */
#else
#endif

            /* FIXME: this object never cleans up */
        }

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_texid);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size.x, m_size.y,
                        GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);

        m_shader->Bind();
#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glEnableVertexAttribArray(m_vertexattrib);
        glVertexAttribPointer(m_vertexattrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
        glEnableVertexAttribArray(m_texattrib);
        glVertexAttribPointer(m_texattrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        /* Never used for now */
        //glEnableVertexAttribArray(m_vertexattrib);
        //glVertexAttribPointer(m_vertexattrib, 2, GL_FLOAT, GL_FALSE, 0, vertices);
#else
        //glEnableClientState(GL_VERTEX_ARRAY);
        //glVertexPointer(2, GL_FLOAT, 0, vertices);
        //glEnableClientState(GL_VERTEX_ARRAY);
        //glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
#endif

        glDrawArrays(GL_TRIANGLES, 0, 6);

#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        glDisableVertexAttribArray(m_vertexattrib);
        glDisableVertexAttribArray(m_texattrib);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        /* Never used for now */
        //glDisableVertexAttribArray(m_vertexattrib);
        //glDisableVertexAttribArray(m_texattrib);
#else
        //glDisableClientState(GL_VERTEX_ARRAY);
        //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
    }

private:
    ivec2 m_size;
    u8vec4 *m_pixels;
    Shader *m_shader;
    GLuint m_texid;
#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
    GLuint m_vbo, m_tbo;
    GLuint m_tco;
#endif
    int m_vertexattrib, m_texattrib;
    float m_angle;
    bool m_ready;
};

int main()
{
#if defined _WIN32
    _chdir("../..");
#endif

    Application app("Tutorial 3: Fractal", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new Fractal(ivec2(640, 480));

    app.Run();

    return EXIT_SUCCESS;
}

