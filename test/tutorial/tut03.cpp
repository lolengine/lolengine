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
        m_frame = -1;
        m_dirty = 8;
        m_center = -0.75;
        //f64cmplx(0.001643721971153, 0.822467633298876);
        //f64cmplx(-1.207205434596, 0.315432814901);
        //f64cmplx(-0.79192956889854, -0.14632423080102);
        //f64cmplx(0.3245046418497685, 0.04855101129280834);
        //f64cmplx(0.28693186889504513, 0.014286693904085048);
        m_radius = 1.5;
        m_screenradius = 0.5 * (m_size.x < m_size.y ? m_size.x : m_size.y);
        m_ready = false;

        m_centertext = new Text(NULL, "gfx/font/ascii.png");
        m_centertext->SetPos(ivec3(5, m_size.y - 15, 1));
        Ticker::Ref(m_centertext);

        m_mousetext = new Text(NULL, "gfx/font/ascii.png");
        m_mousetext->SetPos(ivec3(5, m_size.y - 29, 1));
        Ticker::Ref(m_mousetext);

        position = ivec3(0, 0, 0);
        bbox[0] = position;
        bbox[1] = ivec3(size, 0);
        Input::TrackMouse(this);
    }

    ~Fractal()
    {
        Input::UntrackMouse(this);
        Ticker::Unref(m_centertext);
        Ticker::Unref(m_mousetext);
        delete m_pixels;
    }

    inline f64cmplx ScreenToWorldOffset(ivec2 pixel)
    {
        f64cmplx tmp = f64cmplx(pixel.x - m_size.x / 2,
                                m_size.y / 2 - pixel.y);
        return tmp * (m_radius / m_screenradius);
    }

    virtual void TickGame(float deltams)
    {
        WorldEntity::TickGame(deltams);

        m_frame = (m_frame + 1) % 4;

        f64cmplx worldmouse = m_center + ScreenToWorldOffset(mousepos);

        ivec3 buttons = Input::GetMouseButtons();
        if ((buttons[0] || buttons[2]) && mousepos.x != -1)
        {
            double zoom = pow(2.0, (buttons[0] ? -deltams : deltams) * 0.0015);
            if (m_radius * zoom > 1.5)
                zoom = 1.0;
            m_radius *= zoom;
            m_center = (m_center - worldmouse) * zoom + worldmouse;
            worldmouse = m_center + ScreenToWorldOffset(mousepos);
            m_dirty = 8;
        }

        char buf[128];
        sprintf(buf, "center: %+13.11f%+13.11fi", m_center.x, m_center.y);
        m_centertext->SetText(buf);
        sprintf(buf, " mouse: %+13.11f%+13.11fi", worldmouse.x, worldmouse.y);
        m_mousetext->SetText(buf);

        u8vec4 *m_pixelstart = m_pixels + m_size.x * m_size.y / 4 * m_frame;

        if (m_dirty)
        {
            m_dirty--;

            for (int j = ((m_frame + 1) % 4) / 2; j < m_size.y; j += 2)
            for (int i = m_frame % 2; i < m_size.x; i += 2)
            {
                double const maxlen = 32;
                int const maxiter = 170;

                f64cmplx z0 = m_center + ScreenToWorldOffset(ivec2(i, j));
                f64cmplx r0 = z0;
                //f64cmplx r0(0.28693186889504513, 0.014286693904085048);
                //f64cmplx r0(0.001643721971153, 0.822467633298876);
                f64cmplx z;
                int iter = maxiter;
                for (z = z0; iter && z.sqlen() < maxlen * maxlen; z = z * z + r0)
                    --iter;

                double f = iter;
                double n = z.sqlen();

                double k = log(n) * 0.5f / log(maxlen);
                /* Approximate log2(k) in [1,2]. */
                f += (- 0.344847817623168308695977510213252644185 * k
                      + 2.024664188044341212602376988171727038739) * k
                      - 1.674876738008591047163498125918330313237;

                if (iter)
                {
                    double r = 0.5 * sin(f * 0.27 - 1.5) + 0.5;
                    double g = 0.5 * sin(f * 0.13 + 1.3) + 0.5;
                    double b = 0.5 * sin(f * 0.21 + 0.4) + 0.5;

                    uint8_t red = r * 255.0f;
                    uint8_t green = g * 255.0f;
                    uint8_t blue = b * 255.0f;
                    *m_pixelstart++ = u8vec4(red, green, blue, 0);
                }
                else
                {
                    *m_pixelstart++ = u8vec4(0, 0, 0, 0);
                }
            }
        }
    }

    virtual void TickDraw(float deltams)
    {
        WorldEntity::TickDraw(deltams);

        static float const vertices[] =
        {
             1.0f,  1.0f,
            -1.0f,  1.0f,
            -1.0f, -1.0f,
            -1.0f, -1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,
        };

        static float const texcoords[] =
        {
             1.0f,  1.0f,
             0.0f,  1.0f,
             0.0f,  0.0f,
             0.0f,  0.0f,
             1.0f,  0.0f,
             1.0f,  1.0f,
        };

        if (!m_ready)
        {
            /* Create a texture of half the width and twice the height
             * so that we can upload four different subimages each frame. */
            glGenTextures(1, &m_texid);
            glBindTexture(GL_TEXTURE_2D, m_texid);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x / 2, m_size.y * 2,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            m_shader = Shader::Create(
#if !defined __CELLOS_LV2__
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
                "    vec2 coord = gl_TexCoord[0].xy;"
                /* gl_FragCoord is centered inside the pixel, so we remove
                 * 0.5 from gl_FragCoord.x. Also, (0,0) is at the bottom
                 * left whereas our images have (0,0) at the top left, so we
                 * _add_ 0.5 to gl_FragCoord.y. (XXX: this is no longer true
                 * but will be again when mouse coordinates are back to
                 * being top-left again). */
                "    float i = mod(gl_FragCoord.x - 0.5, 2.0);"
                "    float j = mod(gl_FragCoord.y - 0.5 + i, 2.0);"
                "    coord.y += i + j * 2;"
                "    coord.y *= 0.25;"
                "    vec4 p = texture2D(in_Texture, coord);"
                "    gl_FragColor = p;"
                "}"
#else
                "void main(float4 in_Position : POSITION,"
                "          float2 in_TexCoord : TEXCOORD0,"
                "          out float4 out_Position : POSITION,"
                "          out float2 out_TexCoord : TEXCOORD0)"
                "{"
                "    out_TexCoord = in_TexCoord;"
                "    out_Position = in_Position;"
                "}",

                "void main(float2 in_TexCoord : TEXCOORD0,"
                "          uniform sampler2D tex,"
                "          out float4 out_FragColor : COLOR)"
                "{"
                "    out_FragColor = tex2D(tex, in_TexCoord);"
                "}"
#endif
            );
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

        if (m_dirty)
        {
            m_dirty--;

            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, m_frame * m_size.y / 2,
                            m_size.x / 2, m_size.y / 2,
#if !defined __CELLOS_LV2__
                            GL_RGBA, GL_UNSIGNED_BYTE,
#else
                            /* The PS3 is big-endian */
                            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
#endif
                            m_pixels + m_size.x * m_size.y / 4 * m_frame);
        }

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
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
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
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
    int m_frame, m_dirty;
    bool m_ready;

    f64cmplx m_center;
    double m_radius, m_screenradius;

    /* Debug information */
    Text *m_centertext, *m_mousetext;
};

int main()
{
#if defined _WIN32
    _chdir("../..");
#endif

    Application app("Tutorial 3: Fractal", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new Fractal(ivec2(640, 480));
    //new DebugRecord("fractalol.ogm", 60.0f);

    app.Run();

    return EXIT_SUCCESS;
}

