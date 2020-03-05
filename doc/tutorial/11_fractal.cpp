//
//  Lol Engine — Fractal tutorial
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

#include <memory>
#include <sstream>

#include <lol/engine.h>
#include <lol/types/real.h>
#include <lol/math/vector.h>
#include <lol/math/transform.h>

#include "loldebug.h"

#define USE_REAL 0

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
        m_window_size = Video::GetSize();
        if (m_window_size.y < m_window_size.x)
            m_window2world = 0.5 / m_window_size.y;
        else
            m_window2world = 0.5 / m_window_size.x;
        m_texel2world = (dvec2)m_window_size / (dvec2)m_size * m_window2world;

        m_oldmouse = ivec2(0, 0);

        m_pixels.resize(m_size.x * m_size.y);
        for (int i = 0; i < 4; i++)
        {
            m_deltashift[i] = real("0");
            m_deltascale[i] = real("1");
            m_dirty[i] = 2;
        }
        m_view.center = rcmplx(-0.75, 0.0);
        m_zoom_speed = 0.0;
        m_view.translate = rcmplx(0.0, 0.0);
        m_view.radius = 5.0;

        for (int i = 0; i < (MAX_ITERATIONS + 1) * PALETTE_STEP; i++)
        {
            double f = (double)i / PALETTE_STEP;

            vec3 hsv(lol::fmod(i * 0.001f, 1.f),
                     0.3 * lol::sin(f * 0.27 + 2.0) + 0.3,
                     0.3 * lol::sin(f * 0.21 - 2.6) + 0.6);
            vec3 rgb = color::hsv_to_rgb(hsv);

            if (f < 7.0)
            {
                rgb *= f < 1.0 ? 0.0 : (f - 1.0) / 6.0;
            }

            uint8_t red   = (uint8_t)(rgb.r * 256);
            uint8_t green = (uint8_t)(rgb.g * 256);
            uint8_t blue  = (uint8_t)(rgb.b * 256);
            m_palette.push(u8vec4(blue, green, red, 255));
        }

        m_zoomtext = new Text("", "data/font/ascii.png");
        m_zoomtext->SetPos(vec3(5, (float)m_window_size.y - 15, 1));
        Ticker::Ref(m_zoomtext);

        m_centertext = new Text("", "data/font/ascii.png");
        m_centertext->SetPos(vec3(5, (float)m_window_size.y - 29, 1));
        Ticker::Ref(m_centertext);

        m_mousetext = new Text("", "data/font/ascii.png");
        m_mousetext->SetPos(vec3(5, (float)m_window_size.y - 43, 1));
        Ticker::Ref(m_mousetext);

        m_position = vec3::zero;
        m_aabb.aa = m_position;
        m_aabb.bb = vec3((vec2)m_window_size, 0);

        if (thread::has_threads())
        {
            // Spawn worker threads and wait for their readiness
            for (int i = 0; i < MAX_THREADS; i++)
                m_threads[i] = new thread(std::bind(&Fractal::DoWorkHelper, this, std::placeholders::_1));
            for (int i = 0; i < MAX_THREADS; i++)
                m_spawnqueue.pop();
        }
    }

    ~Fractal()
    {
        if (thread::has_threads())
        {
            // Signal worker threads for completion and wait for them to quit
            for (int i = 0; i < MAX_THREADS; i++)
                m_jobqueue.push(-1);
            for (int i = 0; i < MAX_THREADS; i++)
                m_donequeue.pop();
        }

        Ticker::Unref(m_centertext);
        Ticker::Unref(m_mousetext);
        Ticker::Unref(m_zoomtext);
    }

    inline f128cmplx TexelToWorldOffset(vec2 texel)
    {
        double dx = (0.5 + texel.x - m_size.x / 2) * m_texel2world.x;
        double dy = (0.5 + m_size.y / 2 - texel.y) * m_texel2world.y;
        return m_view.radius * f128cmplx(dx, dy);
    }

    inline f128cmplx ScreenToWorldOffset(vec2 pixel)
    {
        /* No 0.5 offset here, because we want to be able to position the
         * mouse at (0,0) exactly. */
        double dx = pixel.x - m_window_size.x / 2;
        double dy = m_window_size.y / 2 - pixel.y;
        return m_view.radius * m_window2world * f128cmplx(dx, dy);
    }

    virtual void tick_game(float seconds)
    {
        WorldEntity::tick_game(seconds);

        auto mouse = input::mouse();
        auto keyboard = input::keyboard();

        vec2 mousepos(mouse->axis(input::axis::ScreenX),
                      mouse->axis(input::axis::ScreenY));

        int prev_frame = (m_frame + 4) % 4;
        m_frame = (m_frame + 1) % 4;

        if (keyboard->key_pressed(input::key::SC_Space))
        {
            m_julia = !m_julia;
            if (m_julia)
            {
                m_saved_view = m_view;
                m_view.r0 = m_view.center + rcmplx(ScreenToWorldOffset(mousepos));
            }
            else
            {
                m_view = m_saved_view;
            }
            for (auto & flag : m_dirty)
                flag = 2;
        }

        rcmplx worldmouse = m_view.center + rcmplx(ScreenToWorldOffset(mousepos));

        if (mouse->button(input::button::BTN_Middle))
        {
            if (!m_drag)
            {
                m_oldmouse = (ivec2)mousepos;
                m_drag = true;
            }
            m_view.translate = rcmplx(ScreenToWorldOffset((vec2)m_oldmouse)
                                    - ScreenToWorldOffset(mousepos));
            /* XXX: the purpose of this hack is to avoid translating by
             * an exact number of pixels. If this were to happen, the step()
             * optimisation for i915 cards in our shader would behave
             * incorrectly because a quarter of the pixels in the image
             * would have tied rankings in the distance calculation. */
            m_view.translate *= real(1023.0 / 1024.0);
            m_oldmouse = (ivec2)mousepos;
        }
        else
        {
            m_drag = false;
            if (m_view.translate != rcmplx(0.0, 0.0))
            {
                m_view.translate *= real(std::pow(2.0, -seconds * 5.0));
                if ((double)norm(m_view.translate) < m_view.radius * 1e-4)
                    m_view.translate = rcmplx(0.0, 0.0);
            }
        }

        bool hold_right = mouse->button(input::button::BTN_Right);
        bool hold_left = mouse->button(input::button::BTN_Left);
        if ((hold_right || hold_left) && mousepos.x != -1)
        {
            double zoom = hold_right ? -0.5 : 0.5;
            m_zoom_speed += zoom * seconds;
            if (m_zoom_speed / zoom > 5e-3f)
                m_zoom_speed = zoom * 5e-3f;
        }
        else if (m_zoom_speed)
        {
            m_zoom_speed *= std::pow(2.0, -seconds * 5.0);
            if (lol::fabs(m_zoom_speed) < 1e-5 || m_drag)
                m_zoom_speed = 0.0;
        }

        if (m_zoom_speed || m_view.translate != rcmplx(0.0, 0.0))
        {
            rcmplx oldcenter = m_view.center;
            double oldradius = m_view.radius;
            double zoom = std::pow(2.0, seconds * 1e3f * m_zoom_speed);
            if (m_view.radius * zoom > 8.0)
            {
                m_zoom_speed *= -1.0;
                zoom = 8.0 / m_view.radius;
            }
            else if (m_view.radius * zoom < MAX_ZOOM)
            {
                m_zoom_speed *= -1.0;
                zoom = MAX_ZOOM / m_view.radius;
            }
            m_view.radius *= zoom;
            m_view.center += m_view.translate;
            m_view.center = (m_view.center - worldmouse) * real(zoom) + worldmouse;
            worldmouse = m_view.center
                          + rcmplx(ScreenToWorldOffset(mousepos));

            /* Store the transformation properties to go from m_frame - 1
             * to m_frame. */
            m_deltashift[prev_frame] = (m_view.center - oldcenter) / real(oldradius);
            m_deltashift[prev_frame].x /= m_size.x * m_texel2world.x;
            m_deltashift[prev_frame].y /= m_size.y * m_texel2world.y;
            m_deltascale[prev_frame] = m_view.radius / oldradius;
            for (auto & flag : m_dirty)
                flag = 2;
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

            auto t0 = (real)m_zoom_settings[prev_index][0] * m_deltascale[cur_index] + m_deltashift[cur_index].x;
            auto t1 = (real)m_zoom_settings[prev_index][1] * m_deltascale[cur_index] + m_deltashift[cur_index].y;
            auto t2 = (real)m_zoom_settings[prev_index][2] * m_deltascale[cur_index];

            m_zoom_settings[cur_index][0] = float(t0);
            m_zoom_settings[cur_index][1] = float(t1);
            m_zoom_settings[cur_index][2] = float(t2);
        }

        /* Precompute texture offset change instead of doing it in GLSL */
        for (int i = 0; i < 4; i++)
        {
            m_zoom_settings[i][0] += 0.5f * (1.0f - m_zoom_settings[i][2]);
            m_zoom_settings[i][1] -= 0.5f * (1.0f - m_zoom_settings[i][2]);
        }

        m_centertext->SetText("center: " + m_view.center.x.str(30) + " " + m_view.center.y.str(30));
        m_mousetext->SetText(" mouse: " + worldmouse.x.str(30) + " " + worldmouse.y.str(30));

        std::stringstream ss;
        ss << '[' << (m_julia ? "Julia" : "Mandelbrot") << "] zoom: " << 1.0 / m_view.radius;
        m_zoomtext->SetText(ss.str());

        if (m_dirty[m_frame])
        {
            m_dirty[m_frame]--;

            for (int i = 0; i < m_size.y; i += MAX_LINES * 2)
            {
                if (thread::has_threads())
                    m_jobqueue.push(i);
                else
                    DoWork(i);
            }
        }
    }

    void DoWorkHelper(thread *)
    {
        m_spawnqueue.push(0);
        for ( ; ; )
        {
            int line = m_jobqueue.pop();
            if (line == -1)
                break;
            DoWork(line);
            m_donequeue.push(0);
        }
        m_donequeue.push(0);
    };

    void DoWork(int line)
    {
        double const maxsqlen = 1024;
        double const k1 = 1.0 / (1 << 10) / (std::log(maxsqlen) / std::log(2.0));

        int jmin = ((m_frame + 1) % 4) / 2 + line;
        int jmax = jmin + MAX_LINES * 2;
        if (jmax > m_size.y)
            jmax = m_size.y;
        u8vec4 *pixelstart = m_pixels.data()
                           + m_size.x * (m_size.y / 4 * m_frame + line / 4);

#if USE_REAL
        rcmplx c = (rcmplx)m_view.center;
        rcmplx jr0 = (rcmplx)m_view.r0;
#else
        f128cmplx c = (f128cmplx)m_view.center;
        f128cmplx jr0 = (f128cmplx)m_view.r0;
#endif

        for (int j = jmin; j < jmax; j += 2)
        for (int i = m_frame % 2; i < m_size.x; i += 2)
        {
#if USE_REAL
            real xr, yr, x0, y0, x1, y1, x2, y2, x3, y3;
            real sqx0, sqy0, sqx1, sqy1, sqx2, sqy2, sqx3, sqy3;
            rcmplx z0 = c + rcmplx(TexelToWorldOffset(vec2(ivec2(i, j))));
            rcmplx r0 = m_julia ? jr0 : z0;
#else
            ldouble xr, yr, x0, y0, x1, y1, x2, y2, x3, y3;
            ldouble sqx0, sqy0, sqx1, sqy1, sqx2, sqy2, sqx3, sqy3;
            f128cmplx z0 = c + TexelToWorldOffset(vec2(ivec2(i, j)));
            f128cmplx r0 = m_julia ? jr0 : z0;
#endif

            x0 = z0.x; y0 = z0.y;
            xr = r0.x; yr = r0.y;
            sqx0 = x0 * x0; sqy0 = y0 * y0;

            int iter = MAX_ITERATIONS - 4;
            for (;;)
            {
                /* Unroll the loop: tests are more expensive to do at each
                 * iteration than the few extra multiplications, at least
                 * with floats/doubles. */
                x1 = sqx0 - sqy0 + xr; y1 = x0 * y0 + x0 * y0 + yr;
                sqx1 = x1 * x1; sqy1 = y1 * y1;
                x2 = sqx1 - sqy1 + xr; y2 = x1 * y1 + x1 * y1 + yr;
                sqx2 = x2 * x2; sqy2 = y2 * y2;
                x3 = sqx2 - sqy2 + xr; y3 = x2 * y2 + x2 * y2 + yr;
                sqx3 = x3 * x3; sqy3 = y3 * y3;
                x0 = sqx3 - sqy3 + xr; y0 = x3 * y3 + x3 * y3 + yr;
                sqx0 = x0 * x0; sqy0 = y0 * y0;

                if ((double)sqx0 + (double)sqy0 >= maxsqlen)
                    break;
                iter -= 4;
                if (iter < 4)
                    break;
            }

            if (iter)
            {
                double n = (double)sqx0 + (double)sqy0;

                if ((double)sqx1 + (double)sqy1 >= maxsqlen)
                {
                    iter += 3; n = (double)sqx1 + (double)sqy1;
                }
                else if ((double)sqx2 + (double)sqy2 >= maxsqlen)
                {
                    iter += 2; n = (double)sqx2 + (double)sqy2;
                }
                else if ((double)sqx3 + (double)sqy3 >= maxsqlen)
                {
                    iter += 1; n = (double)sqx3 + (double)sqy3;
                }

                if (n > maxsqlen * maxsqlen)
                    n = maxsqlen * maxsqlen;

                /* Approximate log(sqrt(n))/log(sqrt(maxsqlen)) */
                double f = iter;
                union { double n; uint64_t x; } u = { (double)n };
                double k = (double)(u.x >> 42) - (((1 << 10) - 1) << 10);
                k *= k1;

                /* Approximate log2(k) in [1,2]. */
                f += (- 0.344847817623168308695977510213252644185 * k
                      + 2.024664188044341212602376988171727038739) * k
                      - 1.674876738008591047163498125918330313237;

                *pixelstart++ = m_palette[(int)(f * PALETTE_STEP)];
            }
            else
            {
                *pixelstart++ = u8vec4(0, 0, 0, 255);
            }
        }
    }

    virtual bool init_draw() override
    {
        float const vertices[] =
        {
             1.0f,  1.0f,
            -1.0f,  1.0f,
            -1.0f, -1.0f,
            -1.0f, -1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,
        };

        float const texcoords[] =
        {
             1.0f,  1.0f,
             0.0f,  1.0f,
             0.0f,  0.0f,
             0.0f,  0.0f,
             1.0f,  0.0f,
             1.0f,  1.0f,
        };

        /* Create a texture of half the width and twice the height
         * so that we can upload four different subimages each frame. */
        m_texture = std::make_shared<Texture>(ivec2(m_size.x / 2, m_size.y * 2),
                                              PixelFormat::RGBA_8);

        /* Ensure the texture data is complete at least once, otherwise
         * uploading subimages will not work. */
        m_texture->SetData(m_pixels.data());

        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(11_fractal));

        m_vertexattrib = m_shader->GetAttribLocation(VertexUsage::Position, 0);
        m_texattrib = m_shader->GetAttribLocation(VertexUsage::TexCoord, 0);
        m_texuni = m_shader->GetUniformLocation("u_texture");
        m_texeluni = m_shader->GetUniformLocation("u_texel_size");
        m_screenuni = m_shader->GetUniformLocation("u_screen_size");
        m_zoomuni = m_shader->GetUniformLocation("u_zoom_settings");

        m_vdecl = std::make_shared<VertexDeclaration>(
                                VertexStream<vec2>(VertexUsage::Position),
                                VertexStream<vec2>(VertexUsage::TexCoord));
        m_vbo = std::make_shared<VertexBuffer>(sizeof(vertices));
        m_vbo->set_data(vertices, sizeof(vertices));
        m_tbo = std::make_shared<VertexBuffer>(sizeof(texcoords));
        m_tbo->set_data(texcoords, sizeof(texcoords));

        return true;
    }

    virtual void tick_draw(float seconds, Scene &scene) override
    {
        WorldEntity::tick_draw(seconds, scene);

        m_texture->Bind();

        if (m_dirty[m_frame])
        {
            if (thread::has_threads())
            {
                for (int i = 0; i < m_size.y; i += MAX_LINES * 2)
                    m_donequeue.pop();
            }

            m_dirty[m_frame]--;

            m_texture->SetSubData(ivec2(0, m_frame * m_size.y / 2),
                                  m_size / 2,
                                  &m_pixels[m_size.x * m_size.y / 4 * m_frame]);
        }

        m_shader->Bind();
        m_shader->SetUniform(m_texuni, m_texture->GetTextureUniform(), 0);
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

    virtual bool release_draw() override
    {
        m_shader.reset();
        m_vdecl.reset();
        m_vbo.reset();
        m_tbo.reset();
        m_texture.reset();
        return true;
    }

private:
    static int const MAX_ITERATIONS = 400;
    static int const PALETTE_STEP = 32;
    static int const MAX_THREADS = 8;
    static int const MAX_LINES = 8;

    // 1e-14 for doubles, 1e-17 for long doubles
    static double constexpr MAX_ZOOM = 1e-17;

    ivec2 m_size, m_window_size, m_oldmouse;
    double m_window2world;
    dvec2 m_texel2world;
    array<u8vec4> m_pixels, m_palette;

    std::shared_ptr<Shader> m_shader;
    ShaderAttrib m_vertexattrib, m_texattrib;
    ShaderUniform m_texuni, m_texeluni, m_screenuni, m_zoomuni;

    std::shared_ptr<VertexDeclaration> m_vdecl;
    std::shared_ptr<VertexBuffer> m_vbo, m_tbo;
    std::shared_ptr<Texture> m_texture;

    int m_frame = -1, m_slices = 4, m_dirty[4];
    bool m_drag = false;

    struct view_settings
    {
        rcmplx center, translate, r0;
        double radius;
    };

    view_settings m_view, m_saved_view;

    rcmplx m_deltashift[4];
    real m_deltascale[4];
    double m_zoom_speed;
    bool m_julia = false;

    vec4 m_texel_settings, m_screen_settings;
    mat4 m_zoom_settings;

    // Worker threads
    thread *m_threads[MAX_THREADS];
    queue<int> m_spawnqueue, m_jobqueue, m_donequeue;

    // Debug information
    Text *m_centertext, *m_mousetext, *m_zoomtext;
};

int main(int argc, char **argv)
{
    ivec2 window_size(640, 480);

    sys::init(argc, argv);
    Application app("Tutorial 11: Fractal", window_size, 60.0f);

    new DebugFps(5, 5);
    new Fractal(window_size);
    //new DebugRecord("fractalol.ogm", 60.0f);

    app.Run();

    return EXIT_SUCCESS;
}

