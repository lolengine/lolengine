//
// Lol Engine - Fractal tutorial
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

#include <cstring>
#include <cstdio>

#include <lol/main.h>
#include "loldebug.h"

using namespace lol;

LOLFX_RESOURCE_DECLARE(11_fractal);

class Fractal : public WorldEntity
{
public:
    Fractal(ivec2 const &size)
    {
        /* Ensure texture size is a multiple of 16 for better aligned
         * data access. Store the dimensions of a texel for our shader,
         * as well as the half-size of the screen. */
        m_size = size;
        m_size.x = (m_size.x + 15) & ~15;
        m_size.y = (m_size.y + 15) & ~15;
        m_texel_settings = vec4(1.0, 1.0, 2.0, 2.0) / (vec4)m_size.xyxy;
        m_screen_settings = vec4(1.0, 1.0, 0.5, 0.5) * (vec4)m_size.xyxy;

        /* Window size decides the world aspect ratio. For instance, 640×480
         * will be mapped to (-0.66,-0.5) - (0.66,0.5). */
#if !defined __native_client__
        m_window_size = Video::GetSize();
#else
        /* FIXME: it's illegal to call this on the game thread! */
        m_window_size = ivec2(640, 480);
#endif
        if (m_window_size.y < m_window_size.x)
            m_window2world = 0.5 / m_window_size.y;
        else
            m_window2world = 0.5 / m_window_size.x;
        m_texel2world = (dvec2)m_window_size / (dvec2)m_size * m_window2world;

        m_oldmouse = ivec2(0, 0);

        m_pixels.Resize(m_size.x * m_size.y);
        m_frame = -1;
        m_slices = 4;
        for (int i = 0; i < 4; i++)
        {
            m_deltashift[i] = real("0");
            m_deltascale[i] = real("1");
            m_dirty[i] = 2;
        }
#if defined __CELLOS_LV2__ || defined _XBOX
        //m_center = rcmplx(-.22815528839841, -1.11514249704382);
        //m_center = rcmplx(0.001643721971153, 0.822467633298876);
        m_center = rcmplx("-0.65823419062254", "0.50221777363480");
        m_zoom_speed = -0.025;
#else
        m_center = rcmplx(-0.75, 0.0);
        m_zoom_speed = 0.0;
#endif
        m_translate = rcmplx(0.0, 0.0);
        m_radius = 5.0;
        m_ready = false;
        m_drag = false;

        for (int i = 0; i < (MAX_ITERATIONS + 1) * PALETTE_STEP; i++)
        {
            double f = (double)i / PALETTE_STEP;

            double r = 0.5 * lol::sin(f * 0.27 + 2.0) + 0.5;
            double g = 0.5 * lol::sin(f * 0.17 - 1.8) + 0.5;
            double b = 0.5 * lol::sin(f * 0.21 - 2.6) + 0.5;

            if (f < 7.0)
            {
                f = f < 1.0 ? 0.0 : (f - 1.0) / 6.0;
                r *= f;
                g *= f;
                b *= f;
            }

            uint8_t red = r * 255.99f;
            uint8_t green = g * 255.99f;
            uint8_t blue = b * 255.99f;
#if defined __CELLOS_LV2__ || defined _XBOX
            m_palette.Push(u8vec4(255, red, green, blue));
#elif defined __native_client__
            m_palette.Push(u8vec4(red, green, blue, 255));
#else
            m_palette.Push(u8vec4(blue, green, red, 255));
#endif
        }

#if !defined __native_client__
        m_centertext = new Text(NULL, "data/font/ascii.png");
        m_centertext->SetPos(vec3(5, m_window_size.y - 15, 1));
        Ticker::Ref(m_centertext);

        m_mousetext = new Text(NULL, "data/font/ascii.png");
        m_mousetext->SetPos(vec3(5, m_window_size.y - 29, 1));
        Ticker::Ref(m_mousetext);

        m_zoomtext = new Text(NULL, "data/font/ascii.png");
        m_zoomtext->SetPos(vec3(5, m_window_size.y - 43, 1));
        Ticker::Ref(m_zoomtext);
#endif

        m_position = vec3::zero;
        m_bbox[0] = m_position;
        m_bbox[1] = vec3((vec2)m_window_size, 0);
        //Input::TrackMouse(this);

#if LOL_FEATURE_THREADS
        /* Spawn worker threads and wait for their readiness. */
        for (int i = 0; i < MAX_THREADS; i++)
            m_threads[i] = new Thread(DoWorkHelper, this);
        for (int i = 0; i < MAX_THREADS; i++)
            m_spawnqueue.Pop();
#endif
    }

    ~Fractal()
    {
#if LOL_FEATURE_THREADS
        /* Signal worker threads for completion and wait for
         * them to quit. */
        for (int i = 0; i < MAX_THREADS; i++)
            m_jobqueue.Push(-1);
        for (int i = 0; i < MAX_THREADS; i++)
            m_donequeue.Pop();
#endif

        //Input::UntrackMouse(this);
#if !defined __native_client__
        Ticker::Unref(m_centertext);
        Ticker::Unref(m_mousetext);
        Ticker::Unref(m_zoomtext);
#endif
    }

    inline dcmplx TexelToWorldOffset(vec2 texel)
    {
        double dx = (0.5 + texel.x - m_size.x / 2) * m_texel2world.x;
        double dy = (0.5 + m_size.y / 2 - texel.y) * m_texel2world.y;
        return m_radius * dcmplx(dx, dy);
    }

    inline dcmplx ScreenToWorldOffset(vec2 pixel)
    {
        /* No 0.5 offset here, because we want to be able to position the
         * mouse at (0,0) exactly. */
        double dx = pixel.x - m_window_size.x / 2;
        double dy = m_window_size.y / 2 - pixel.y;
        return m_radius * m_window2world * dcmplx(dx, dy);
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        ivec2 mousepos = ivec2::zero; /* FIXME: input */

        int prev_frame = (m_frame + 4) % 4;
        m_frame = (m_frame + 1) % 4;

        rcmplx worldmouse = m_center
                          + rcmplx(ScreenToWorldOffset((vec2)mousepos));

        uint32_t buttons = 0;
        //uint32_t buttons = Input::GetMouseButtons();
#if !defined __CELLOS_LV2__ && !defined _XBOX
        if (buttons & 0x2)
        {
            if (!m_drag)
            {
                m_oldmouse = mousepos;
                m_drag = true;
            }
            m_translate = rcmplx(ScreenToWorldOffset((vec2)m_oldmouse)
                                  - ScreenToWorldOffset((vec2)mousepos));
            /* XXX: the purpose of this hack is to avoid translating by
             * an exact number of pixels. If this were to happen, the step()
             * optimisation for i915 cards in our shader would behave
             * incorrectly because a quarter of the pixels in the image
             * would have tied rankings in the distance calculation. */
            m_translate *= real(1023.0 / 1024.0);
            m_oldmouse = mousepos;
        }
        else
        {
            m_drag = false;
            if (m_translate != rcmplx(0.0, 0.0))
            {
                m_translate *= real(std::pow(2.0, -seconds * 5.0));
                if ((double)norm(m_translate) < m_radius * 1e-4)
                    m_translate = rcmplx(0.0, 0.0);
            }
        }

        if (buttons & 0x5 && mousepos.x != -1)
        {
            double zoom = (buttons & 0x1) ? -0.5 : 0.5;
            m_zoom_speed += zoom * seconds;
            if (m_zoom_speed / zoom > 5e-3f)
                m_zoom_speed = zoom * 5e-3f;
        }
        else if (m_zoom_speed)
        {
            m_zoom_speed *= std::pow(2.0, -seconds * 5.0);
            if (lol::abs(m_zoom_speed) < 1e-5 || m_drag)
                m_zoom_speed = 0.0;
        }
#endif

        if (m_zoom_speed || m_translate != rcmplx(0.0, 0.0))
        {
            rcmplx oldcenter = m_center;
            double oldradius = m_radius;
            double zoom = std::pow(2.0, seconds * 1e3f * m_zoom_speed);
            if (m_radius * zoom > 8.0)
            {
                m_zoom_speed *= -1.0;
                zoom = 8.0 / m_radius;
            }
            else if (m_radius * zoom < 1e-14)
            {
                m_zoom_speed *= -1.0;
                zoom = 1e-14 / m_radius;
            }
            m_radius *= zoom;
#if !defined __CELLOS_LV2__ && !defined _XBOX
            m_center += m_translate;
            m_center = (m_center - worldmouse) * real(zoom) + worldmouse;
            worldmouse = m_center
                       + rcmplx(ScreenToWorldOffset((vec2)mousepos));
#endif

            /* Store the transformation properties to go from m_frame - 1
             * to m_frame. */
            m_deltashift[prev_frame] = (m_center - oldcenter) / real(oldradius);
            m_deltashift[prev_frame].x /= m_size.x * m_texel2world.x;
            m_deltashift[prev_frame].y /= m_size.y * m_texel2world.y;
            m_deltascale[prev_frame] = m_radius / oldradius;
            m_dirty[0] = m_dirty[1] = m_dirty[2] = m_dirty[3] = 2;
        }
        else
        {
            /* If settings didn't change, set transformation from previous
             * frame to identity. */
            m_deltashift[prev_frame] = real::R_0();
            m_deltascale[prev_frame] = real::R_1();
        }

        /* Transformation from current frame to current frame is always
         * identity. */
        m_zoom_settings[m_frame][0] = 0.0f;
        m_zoom_settings[m_frame][1] = 0.0f;
        m_zoom_settings[m_frame][2] = 1.0f;

        /* Compute transformation from other frames to current frame */
        for (int i = 0; i < 3; i++)
        {
            int prev_index = (m_frame + 4 - i) % 4;
            int cur_index = (m_frame + 3 - i) % 4;

            m_zoom_settings[cur_index][0] = (real)m_zoom_settings[prev_index][0] * m_deltascale[cur_index] + m_deltashift[cur_index].x;
            m_zoom_settings[cur_index][1] = (real)m_zoom_settings[prev_index][1] * m_deltascale[cur_index] + m_deltashift[cur_index].y;
            m_zoom_settings[cur_index][2] = (real)m_zoom_settings[prev_index][2] * m_deltascale[cur_index];
        }

        /* Precompute texture offset change instead of doing it in GLSL */
        for (int i = 0; i < 4; i++)
        {
            m_zoom_settings[i][0] += 0.5 * (1.0 - m_zoom_settings[i][2]);
            m_zoom_settings[i][1] -= 0.5 * (1.0 - m_zoom_settings[i][2]);
        }

#if !defined __native_client__
        char buf[256];
        std::sprintf(buf, "center: ");
        m_center.x.sprintf(buf + strlen(buf), 30);
        std::sprintf(buf + strlen(buf), " ");
        m_center.y.sprintf(buf + strlen(buf), 30);
        m_centertext->SetText(buf);
        std::sprintf(buf, " mouse: ");
        worldmouse.x.sprintf(buf + strlen(buf), 30);
        std::sprintf(buf + strlen(buf), " ");
        worldmouse.y.sprintf(buf + strlen(buf), 30);
        m_mousetext->SetText(buf);
        std::sprintf(buf, "  zoom: %g", 1.0 / m_radius);
        m_zoomtext->SetText(buf);
#endif

        if (m_dirty[m_frame])
        {
            m_dirty[m_frame]--;

            for (int i = 0; i < m_size.y; i += MAX_LINES * 2)
            {
#if LOL_FEATURE_THREADS
                m_jobqueue.Push(i);
#else
                DoWork(i);
#endif
            }
        }
    }

#if LOL_FEATURE_THREADS
    static void *DoWorkHelper(void *data)
    {
        Fractal *that = (Fractal *)data;
        that->m_spawnqueue.Push(0);
        for ( ; ; )
        {
            int line = that->m_jobqueue.Pop();
            if (line == -1)
                break;
            that->DoWork(line);
            that->m_donequeue.Push(0);
        }
        that->m_donequeue.Push(0);
        return NULL;
    };
#endif

    void DoWork(int line)
    {
        double const maxsqlen = 1024;
        double const k1 = 1.0 / (1 << 10) / (std::log(maxsqlen) / std::log(2.0));

        int jmin = ((m_frame + 1) % 4) / 2 + line;
        int jmax = jmin + MAX_LINES * 2;
        if (jmax > m_size.y)
            jmax = m_size.y;
        u8vec4 *m_pixelstart = &m_pixels[0]
                             + m_size.x * (m_size.y / 4 * m_frame + line / 4);

        dcmplx c = (dcmplx)m_center;

        for (int j = jmin; j < jmax; j += 2)
        for (int i = m_frame % 2; i < m_size.x; i += 2)
        {
            double xr, yr, x0, y0, x1, y1, x2, y2, x3, y3;
            dcmplx z0 = c + TexelToWorldOffset(vec2(i, j));
            //dcmplx r0(0.28693186889504513, 0.014286693904085048);
            //dcmplx r0(0.001643721971153, 0.822467633298876);
            //dcmplx r0(-1.207205434596, 0.315432814901);
            //dcmplx r0(-0.79192956889854, -0.14632423080102);
            //dcmplx r0(0.3245046418497685, 0.04855101129280834);
            dcmplx r0 = z0;

            x0 = z0.x; y0 = z0.y;
            xr = r0.x; yr = r0.y;

            int iter = MAX_ITERATIONS - 4;
            for (;;)
            {
                /* Unroll the loop: tests are more expensive to do at each
                 * iteration than the few extra multiplications. */
                x1 = x0 * x0 - y0 * y0 + xr;
                y1 = x0 * y0 + x0 * y0 + yr;
                x2 = x1 * x1 - y1 * y1 + xr;
                y2 = x1 * y1 + x1 * y1 + yr;
                x3 = x2 * x2 - y2 * y2 + xr;
                y3 = x2 * y2 + x2 * y2 + yr;
                x0 = x3 * x3 - y3 * y3 + xr;
                y0 = x3 * y3 + x3 * y3 + yr;

                if (x0 * x0 + y0 * y0 >= maxsqlen)
                    break;
                iter -= 4;
                if (iter < 4)
                    break;
            }

            if (iter)
            {
                double n = x0 * x0 + y0 * y0;

                if (x1 * x1 + y1 * y1 >= maxsqlen)
                {
                    iter += 3; n = x1 * x1 + y1 * y1;
                }
                else if (x2 * x2 + y2 * y2 >= maxsqlen)
                {
                    iter += 2; n = x2 * x2 + y2 * y2;
                }
                else if (x3 * x3 + y3 * y3 >= maxsqlen)
                {
                    iter += 1; n = x3 * x3 + y3 * y3;
                }

                if (n > maxsqlen * maxsqlen)
                    n = maxsqlen * maxsqlen;

                /* Approximate log(sqrt(n))/log(sqrt(maxsqlen)) */
                double f = iter;
                union { double n; uint64_t x; } u = { n };
                double k = (u.x >> 42) - (((1 << 10) - 1) << 10);
                k *= k1;

                /* Approximate log2(k) in [1,2]. */
                f += (- 0.344847817623168308695977510213252644185 * k
                      + 2.024664188044341212602376988171727038739) * k
                      - 1.674876738008591047163498125918330313237;

                *m_pixelstart++ = m_palette[(int)(f * PALETTE_STEP)];
            }
            else
            {
#if defined __CELLOS_LV2__ || defined _XBOX
                *m_pixelstart++ = u8vec4(255, 0, 0, 0);
#else
                *m_pixelstart++ = u8vec4(0, 0, 0, 255);
#endif
            }
        }
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

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
            m_texture = new Texture(ivec2(m_size.x / 2, m_size.y * 2),
                                    PixelFormat::RGBA_8);

            /* Ensure the texture data is complete at least once, otherwise
             * uploading subimages will not work. */
            m_texture->SetData(&m_pixels[0]);

            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(11_fractal));

            m_vertexattrib = m_shader->GetAttribLocation(VertexUsage::Position, 0);
            m_texattrib = m_shader->GetAttribLocation(VertexUsage::TexCoord, 0);
            m_texeluni = m_shader->GetUniformLocation("u_TexelSize");
            m_screenuni = m_shader->GetUniformLocation("u_ScreenSize");
            m_zoomuni = m_shader->GetUniformLocation("u_ZoomSettings");

            m_vdecl =
              new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position),
                                    VertexStream<vec2>(VertexUsage::TexCoord));
            m_vbo = new VertexBuffer(sizeof(vertices));
            m_tbo = new VertexBuffer(sizeof(texcoords));

            void *tmp = m_vbo->Lock(0, 0);
            memcpy(tmp, vertices, sizeof(vertices));
            m_vbo->Unlock();

            tmp = m_tbo->Lock(0, 0);
            memcpy(tmp, texcoords, sizeof(texcoords));
            m_tbo->Unlock();

            /* FIXME: this object never cleans up */
            m_ready = true;
        }

        m_texture->Bind();

        if (m_dirty[m_frame])
        {
#if LOL_FEATURE_THREADS
            for (int i = 0; i < m_size.y; i += MAX_LINES * 2)
                m_donequeue.Pop();
#endif

            m_dirty[m_frame]--;

#if defined __CELLOS_LV2__
            /* glTexSubImage2D is extremely slow on the PS3, to the point
             * that uploading the whole texture is 40 times faster. */
            m_texture->SetData(&m_pixels[0]);
#else
            m_texture->SetSubData(ivec2(0, m_frame * m_size.y / 2),
                                  m_size / 2,
                                  &m_pixels[m_size.x * m_size.y / 4 * m_frame]);
#endif
        }

        m_shader->Bind();
        m_shader->SetUniform(m_texeluni, m_texel_settings);
        m_shader->SetUniform(m_screenuni, m_screen_settings);
        m_shader->SetUniform(m_zoomuni, m_zoom_settings);
        m_vdecl->Bind();
        m_vdecl->SetStream(m_vbo, m_vertexattrib);
        m_vdecl->SetStream(m_tbo, m_texattrib);
        m_texture->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
    }

private:
    static int const MAX_ITERATIONS = 340;
    static int const PALETTE_STEP = 32;
    static int const MAX_THREADS = 8;
    static int const MAX_LINES = 8;

    ivec2 m_size, m_window_size, m_oldmouse;
    double m_window2world;
    dvec2 m_texel2world;
    array<u8vec4> m_pixels, m_palette;

    Shader *m_shader;
    ShaderAttrib m_vertexattrib, m_texattrib;
    ShaderUniform m_texeluni, m_screenuni, m_zoomuni;

    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo, *m_tbo;
    Texture *m_texture;

    int m_frame, m_slices, m_dirty[4];
    bool m_ready, m_drag;

    rcmplx m_deltashift[4], m_center, m_translate;
    real m_deltascale[4];
    double m_zoom_speed, m_radius;

    vec4 m_texel_settings, m_screen_settings;
    mat4 m_zoom_settings;

#if LOL_FEATURE_THREADS
    /* Worker threads */
    Thread *m_threads[MAX_THREADS];
    Queue<int> m_spawnqueue, m_jobqueue, m_donequeue;
#endif

#if !defined __native_client__
    /* Debug information */
    Text *m_centertext, *m_mousetext, *m_zoomtext;
#endif
};

int main(int argc, char **argv)
{
    ivec2 window_size(640, 480);

    System::Init(argc, argv);
    Application app("Tutorial 3: Fractal", window_size, 60.0f);

    new DebugFps(5, 5);
    new Fractal(window_size);
    //new DebugRecord("fractalol.ogm", 60.0f);

    app.Run();

    return EXIT_SUCCESS;
}

