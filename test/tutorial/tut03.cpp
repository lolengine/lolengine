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
        /* Ensure size has even X and Y values */
        m_size = size;
        m_size.x = (m_size.x + 1) & ~1;
        m_size.y = (m_size.y + 1) & ~1;

        m_window_size = Video::GetSize();
        m_pixels = new u8vec4[m_size.x * m_size.y];
        m_tmppixels = new u8vec4[m_size.x / 2 * m_size.y / 2];
        m_frame = -1;
        for (int i = 0; i < 4; i++)
        {
            m_deltashift[i] = 0.0;
            m_deltascale[i] = 1.0;
            m_dirty[i] = 2;
        }
        m_center = -0.75;
        m_radius = 1.5;
        m_texture_radius = 0.5 * (m_size.x < m_size.y ? m_size.x : m_size.y);
        m_window_radius = 0.5 * (m_window_size.x < m_window_size.y ? m_window_size.x : m_window_size.y);
        m_pixel_delta = vec4(vec2(1.0, 1.0) / (vec2)m_size, m_size);
        m_ready = false;

        m_palette = new u8vec4[(MAX_ITERATIONS + 1) * PALETTE_STEP];
        for (int i = 0; i < (MAX_ITERATIONS + 1) * PALETTE_STEP; i++)
        {
            float f = i / (double)PALETTE_STEP;

            double r = 0.5 * sin(f * 0.27 - 2.5) + 0.5;
            double g = 0.5 * sin(f * 0.13 + 1.1) + 0.5;
            double b = 0.5 * sin(f * 0.21 + 0.4) + 0.5;

            uint8_t red = r * 255.0f;
            uint8_t green = g * 255.0f;
            uint8_t blue = b * 255.0f;
            m_palette[i] = u8vec4(blue, green, red, 0);
        }

        m_centertext = new Text(NULL, "gfx/font/ascii.png");
        m_centertext->SetPos(ivec3(5, m_window_size.y - 15, 1));
        Ticker::Ref(m_centertext);

        m_mousetext = new Text(NULL, "gfx/font/ascii.png");
        m_mousetext->SetPos(ivec3(5, m_window_size.y - 29, 1));
        Ticker::Ref(m_mousetext);

        m_zoomtext = new Text(NULL, "gfx/font/ascii.png");
        m_zoomtext->SetPos(ivec3(5, m_window_size.y - 43, 1));
        Ticker::Ref(m_zoomtext);

        position = ivec3(0, 0, 0);
        bbox[0] = position;
        bbox[1] = ivec3(m_window_size, 0);
        Input::TrackMouse(this);
    }

    ~Fractal()
    {
        Input::UntrackMouse(this);
        Ticker::Unref(m_centertext);
        Ticker::Unref(m_mousetext);
        Ticker::Unref(m_zoomtext);
        delete m_pixels;
        delete m_tmppixels;
        delete m_palette;
    }

    inline f64cmplx TexelToWorldOffset(ivec2 texel)
    {
        f64cmplx tmp = f64cmplx(0.5 + texel.x - m_size.x / 2,
                                0.5 + m_size.y / 2 - texel.y);
        return tmp * (m_radius / m_texture_radius);
    }

    inline f64cmplx ScreenToWorldOffset(ivec2 pixel)
    {
        /* No 0.5 offset here, because we want to be able to position the
         * mouse at (0,0) exactly. */
        f64cmplx tmp = f64cmplx(pixel.x - m_window_size.x / 2,
                                m_window_size.y / 2 - pixel.y);
        return tmp * (m_radius / m_window_radius);
    }

    virtual void TickGame(float deltams)
    {
        WorldEntity::TickGame(deltams);

        m_frame = (m_frame + 1) % 4;

        f64cmplx worldmouse = m_center + ScreenToWorldOffset(mousepos);

        ivec3 buttons = Input::GetMouseButtons();
#ifdef __CELLOS_LV2__
        if (true)
#else
        if ((buttons[0] || buttons[2]) && mousepos.x != -1)
#endif
        {
            f64cmplx oldcenter = m_center;
            double oldradius = m_radius;
#ifdef __CELLOS_LV2__
            m_radius *= pow(2.0, -deltams * 0.00015);
            m_center = f64cmplx(0.001643721971153, 0.822467633298876);
#else
            double zoom = pow(2.0, (buttons[0] ? -deltams : deltams) * 0.0025);
            if (m_radius * zoom > 8.0)
                zoom = 8.0 / m_radius;
            else if (m_radius * zoom < 1e-14)
                zoom = 1e-14 / m_radius;
            m_radius *= zoom;
            m_center = (m_center - worldmouse) * zoom + worldmouse;
            worldmouse = m_center + ScreenToWorldOffset(mousepos);
#endif

            /* Store the transformation properties to go from m_frame-1
             * to m_frame. */
            m_deltashift[m_frame] = (oldcenter - m_center) / m_radius;
            m_deltascale[m_frame] = oldradius / m_radius;
            m_dirty[0] = m_dirty[1] = m_dirty[2] = m_dirty[3] = 2;
        }
        else
        {
            /* If settings didn't change, set transformation from previous
             * frame to identity. */
            m_deltashift[m_frame] = 0.0;
            m_deltascale[m_frame] = 1.0;
        }

        if (buttons[1])
            m_dirty[0] = m_dirty[1] = m_dirty[2] = m_dirty[3] = 2;

        char buf[128];
        sprintf(buf, "center: %+16.14f%+16.14fi", m_center.x, m_center.y);
        m_centertext->SetText(buf);
        sprintf(buf, " mouse: %+16.14f%+16.14fi", worldmouse.x, worldmouse.y);
        m_mousetext->SetText(buf);
        sprintf(buf, "  zoom: %g", 1.0 / m_radius);
        m_zoomtext->SetText(buf);

        u8vec4 *m_pixelstart = m_pixels + m_size.x * m_size.y / 4 * m_frame;

        if (m_dirty[m_frame])
        {
            m_dirty[m_frame]--;

            for (int j = ((m_frame + 1) % 4) / 2; j < m_size.y; j += 2)
            for (int i = m_frame % 2; i < m_size.x; i += 2)
            {
                double const maxsqlen = 1024;

                f64cmplx z0 = m_center + TexelToWorldOffset(ivec2(i, j));
                f64cmplx r0 = z0;
                //f64cmplx r0(0.28693186889504513, 0.014286693904085048);
                //f64cmplx r0(0.001643721971153, 0.822467633298876);
                //f64cmplx r0(-1.207205434596, 0.315432814901);
                //f64cmplx r0(-0.79192956889854, -0.14632423080102);
                //f64cmplx r0(0.3245046418497685, 0.04855101129280834);
                f64cmplx z;
                int iter = MAX_ITERATIONS;
                for (z = z0; iter && z.sqlen() < maxsqlen; z = z * z + r0)
                    --iter;

                if (iter)
                {
                    double f = iter;
                    double n = z.sqlen();
                    if (n > maxsqlen * maxsqlen)
                        n = maxsqlen * maxsqlen;

                    /* Approximate log(sqrt(n))/log(sqrt(maxsqlen)) */
                    union { double n; uint64_t x; } u = { n };
                    double k = (u.x >> 42) - (((1 << 10) - 1) << 10);
                    k *= 1.0 / (1 << 10) / log2(maxsqlen);

                    /* Approximate log2(k) in [1,2]. */
                    f += (- 0.344847817623168308695977510213252644185 * k
                          + 2.024664188044341212602376988171727038739) * k
                          - 1.674876738008591047163498125918330313237;

                    *m_pixelstart++ = m_palette[(int)(f * PALETTE_STEP)];
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
                         0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_pixels);
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
                "uniform vec4 in_PixelDelta;\n"
                "uniform sampler2D in_Texture;\n"
                "void main(void) {"
                "    vec2 coord = gl_TexCoord[0].xy;"
                /* i is 0 or 1, depending on the current X coordinate within
                 * the current 2×2 texel. j is 0 or 1, depending on the Y
                 * coordinate _and_ the value of i, in order to stay on the
                 * Bayer dithering pattern. */
                "    int i = int(mod(coord.x, 2.0 * in_PixelDelta.x) * in_PixelDelta.z);"
                "    int j = int(mod(coord.y + i * in_PixelDelta.y, 2.0 * in_PixelDelta.y) * in_PixelDelta.w);"
                /* Choose the best slice depending on the value of i and j. */
                "    coord.y += i + j * 2;"
                "    coord.y *= 0.25;"
                /* Get a pixel from the best slice */
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

                "void main(float4 in_FragCoord : WPOS,"
                "          float2 in_TexCoord : TEXCOORD0,"
                "          uniform float4 in_PixelDelta,"
                "          uniform sampler2D in_Texture,"
                "          out float4 out_FragColor : COLOR)"
                "{"
                "    float2 coord = in_TexCoord.xy;"
                "    float i = frac((in_FragCoord.x - 0.5) * 0.5) * 2.0;"
                "    float j = frac((in_FragCoord.y - 0.5 + i) * 0.5) * 2.0;"
                "    coord.y += i + j * 2;"
                "    coord.y *= 0.25;"
                "    float4 p = tex2D(in_Texture, coord);"
                "    out_FragColor = p;"
                "}"
#endif
            );
            m_vertexattrib = m_shader->GetAttribLocation("in_Vertex");
            m_texattrib = m_shader->GetAttribLocation("in_TexCoord");
            m_pixeluni = m_shader->GetUniformLocation("in_PixelDelta");
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

        if (m_dirty[m_frame])
        {
            m_dirty[m_frame]--;

            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, m_frame * m_size.y / 2,
                            m_size.x / 2, m_size.y / 2,
                            GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
                            m_pixels + m_size.x * m_size.y / 4 * m_frame);
        }

/* If other frames are dirty, upload fake data for now */
if (0) for (int i = 0; i < 4; i++)
{
    if (m_dirty[i])
    {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i * m_size.y / 2,
                            m_size.x / 2, m_size.y / 2,
                            GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
                            m_pixels + m_size.x * m_size.y / 4 * m_frame);
    }
}

        m_shader->Bind();
        m_shader->SetUniform(m_pixeluni, m_pixel_delta);
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
    static int const MAX_ITERATIONS = 170;
    static int const PALETTE_STEP = 32;

    ivec2 m_size, m_window_size;
    u8vec4 *m_pixels, *m_tmppixels, *m_palette;
    Shader *m_shader;
    GLuint m_texid;
#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
    GLuint m_vbo, m_tbo;
    GLuint m_tco;
#endif
    int m_vertexattrib, m_texattrib, m_pixeluni;
    int m_frame, m_dirty[4];
    bool m_ready;

    f64cmplx m_center;
    double m_radius, m_texture_radius, m_window_radius;
    vec4 m_pixel_delta;
    f64cmplx m_deltashift[4];
    double m_deltascale[4];

    /* Debug information */
    Text *m_centertext, *m_mousetext, *m_zoomtext;
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

