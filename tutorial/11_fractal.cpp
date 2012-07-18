//
// Lol Engine - Fractal tutorial
//
// Copyright: (c) 2011-2012 Sam Hocevar <sam@hocevar.net>
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

using namespace lol;

#if defined _WIN32
#   include <direct.h>
#   if defined USE_D3D9
#       define FAR
#       define NEAR
#       include <d3d9.h>
#   endif
#endif

extern char const *lolfx_11_fractal;

#if defined USE_D3D9
extern IDirect3DDevice9 *g_d3ddevice;
#elif defined _XBOX
extern D3DDevice *g_d3ddevice;
#elif __CELLOS_LV2__
static GLint const INTERNAL_FORMAT = GL_ARGB_SCE;
static GLenum const TEXTURE_FORMAT = GL_BGRA;
static GLenum const TEXTURE_TYPE = GL_UNSIGNED_INT_8_8_8_8_REV;
#elif defined __native_client__
static GLint const INTERNAL_FORMAT = GL_RGBA;
static GLenum const TEXTURE_FORMAT = GL_RGBA;
static GLenum const TEXTURE_TYPE = GL_UNSIGNED_BYTE;
#else
/* Seems efficient for little endian textures */
static GLint const INTERNAL_FORMAT = GL_RGBA;
static GLenum const TEXTURE_FORMAT = GL_BGRA;
static GLenum const TEXTURE_TYPE = GL_UNSIGNED_INT_8_8_8_8_REV;
#endif

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
        m_texel_settings = vec4(1.0, 1.0, 2.0, 2.0) / m_size.xyxy;
        m_screen_settings = vec4(1.0, 1.0, 0.5, 0.5) * m_size.xyxy;

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
        m_texel2world = (vec2)m_window_size / m_size * m_window2world;

        m_oldmouse = ivec2(0, 0);

        m_pixels = new u8vec4[m_size.x * m_size.y];
        m_tmppixels = new u8vec4[m_size.x / 2 * m_size.y / 2];
        m_frame = -1;
        m_slices = 4;
        for (int i = 0; i < 4; i++)
        {
            m_deltashift[i] = 0.0;
            m_deltascale[i] = 1.0;
            m_dirty[i] = 2;
        }
#if defined __CELLOS_LV2__ || defined _XBOX
        //m_center = dcmplx(-.22815528839841, -1.11514249704382);
        //m_center = dcmplx(0.001643721971153, 0.822467633298876);
        m_center = dcmplx(-0.65823419062254, 0.50221777363480);
        m_zoom_speed = -0.025;
#else
        m_center = -0.75;
        m_zoom_speed = 0.0;
#endif
        m_translate = 0;
        m_radius = 5.0;
        m_ready = false;
        m_drag = false;

        m_palette = new u8vec4[(MAX_ITERATIONS + 1) * PALETTE_STEP];
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
            m_palette[i] = u8vec4(255, red, green, blue);
#elif defined __native_client__
            m_palette[i] = u8vec4(red, green, blue, 255);
#else
            m_palette[i] = u8vec4(blue, green, red, 255);
#endif
        }

#if !defined __native_client__
        m_centertext = new Text(NULL, "src/data/font/ascii.png");
        m_centertext->SetPos(ivec3(5, m_window_size.y - 15, 1));
        Ticker::Ref(m_centertext);

        m_mousetext = new Text(NULL, "src/data/font/ascii.png");
        m_mousetext->SetPos(ivec3(5, m_window_size.y - 29, 1));
        Ticker::Ref(m_mousetext);

        m_zoomtext = new Text(NULL, "src/data/font/ascii.png");
        m_zoomtext->SetPos(ivec3(5, m_window_size.y - 43, 1));
        Ticker::Ref(m_zoomtext);
#endif

        m_position = ivec3(0, 0, 0);
        m_bbox[0] = m_position;
        m_bbox[1] = ivec3(m_window_size, 0);
        Input::TrackMouse(this);

        /* Spawn worker threads and wait for their readiness. */
        for (int i = 0; i < MAX_THREADS; i++)
            m_threads[i] = new Thread(DoWorkHelper, this);
        for (int i = 0; i < MAX_THREADS; i++)
            m_spawnqueue.Pop();
    }

    ~Fractal()
    {
        /* Signal worker threads for completion and wait for
         * them to quit. */
        for (int i = 0; i < MAX_THREADS; i++)
            m_jobqueue.Push(-1);
        for (int i = 0; i < MAX_THREADS; i++)
            m_donequeue.Pop();

        Input::UntrackMouse(this);
#if !defined __native_client__
        Ticker::Unref(m_centertext);
        Ticker::Unref(m_mousetext);
        Ticker::Unref(m_zoomtext);
#endif
        delete m_pixels;
        delete m_tmppixels;
        delete m_palette;
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

        int prev_frame = m_frame;
        m_frame = (m_frame + 1) % 4;

        dcmplx worldmouse = m_center + ScreenToWorldOffset(m_mousepos);

        ivec3 buttons = Input::GetMouseButtons();
#if !defined __CELLOS_LV2__ && !defined _XBOX
        if (buttons[1])
        {
            if (!m_drag)
            {
                m_oldmouse = m_mousepos;
                m_drag = true;
            }
            m_translate = ScreenToWorldOffset(m_oldmouse)
                        - ScreenToWorldOffset(m_mousepos);
            /* XXX: the purpose of this hack is to avoid translating by
             * an exact number of pixels. If this were to happen, the step()
             * optimisation for i915 cards in our shader would behave
             * incorrectly because a quarter of the pixels in the image
             * would have tie rankings in the distance calculation. */
            m_translate *= 1023.0 / 1024.0;
            m_oldmouse = m_mousepos;
        }
        else
        {
            m_drag = false;
            if (m_translate != 0.0)
            {
                m_translate *= std::pow(2.0, -seconds * 5.0);
                if (m_translate.norm() / m_radius < 1e-4)
                    m_translate = 0.0;
            }
        }

        if ((buttons[0] || buttons[2]) && m_mousepos.x != -1)
        {
            double zoom = buttons[0] ? -0.5 : 0.5;
            m_zoom_speed += seconds * zoom;
            if (m_zoom_speed / zoom > 5e-3f)
                m_zoom_speed = 5e-3f * zoom;
        }
        else if (m_zoom_speed)
        {
            m_zoom_speed *= std::pow(2.0, -seconds * 5.0);
            if (abs(m_zoom_speed) < 1e-5 || m_drag)
                m_zoom_speed = 0.0;
        }
#endif

        if (m_zoom_speed || m_translate != 0.0)
        {
            dcmplx oldcenter = m_center;
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
            m_center = (m_center - worldmouse) * zoom + worldmouse;
            worldmouse = m_center + ScreenToWorldOffset(m_mousepos);
#endif

            /* Store the transformation properties to go from m_frame - 1
             * to m_frame. */
            m_deltashift[prev_frame] = (m_center - oldcenter) / oldradius;
            m_deltashift[prev_frame].x /= m_size.x * m_texel2world.x;
            m_deltashift[prev_frame].y /= m_size.y * m_texel2world.y;
            m_deltascale[prev_frame] = m_radius / oldradius;
            m_dirty[0] = m_dirty[1] = m_dirty[2] = m_dirty[3] = 2;
        }
        else
        {
            /* If settings didn't change, set transformation from previous
             * frame to identity. */
            m_deltashift[prev_frame] = 0.0;
            m_deltascale[prev_frame] = 1.0;
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

            m_zoom_settings[cur_index][0] = m_zoom_settings[prev_index][0] * m_deltascale[cur_index] + m_deltashift[cur_index].x;
            m_zoom_settings[cur_index][1] = m_zoom_settings[prev_index][1] * m_deltascale[cur_index] + m_deltashift[cur_index].y;
            m_zoom_settings[cur_index][2] = m_zoom_settings[prev_index][2] * m_deltascale[cur_index];
        }

        /* Precompute texture offset change instead of doing it in GLSL */
        for (int i = 0; i < 4; i++)
        {
            m_zoom_settings[i][0] += 0.5 * (1.0 - m_zoom_settings[i][2]);
            m_zoom_settings[i][1] -= 0.5 * (1.0 - m_zoom_settings[i][2]);
        }

#if !defined __native_client__
        char buf[128];
        sprintf(buf, "center: %+16.14f%+16.14fi", m_center.x, m_center.y);
        m_centertext->SetText(buf);
        sprintf(buf, " mouse: %+16.14f%+16.14fi", worldmouse.x, worldmouse.y);
        m_mousetext->SetText(buf);
        sprintf(buf, "  zoom: %g", 1.0 / m_radius);
        m_zoomtext->SetText(buf);
#endif

        if (m_dirty[m_frame])
        {
            m_dirty[m_frame]--;

            for (int i = 0; i < m_size.y; i += MAX_LINES * 2)
                m_jobqueue.Push(i);
        }
    }

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

    void DoWork(int line)
    {
        double const maxsqlen = 1024;
        double const k1 = 1.0 / (1 << 10) / (std::log(maxsqlen) / std::log(2.0));

        int jmin = ((m_frame + 1) % 4) / 2 + line;
        int jmax = jmin + MAX_LINES * 2;
        if (jmax > m_size.y)
            jmax = m_size.y;
        u8vec4 *m_pixelstart = m_pixels
                             + m_size.x * (m_size.y / 4 * m_frame + line / 4);

        for (int j = jmin; j < jmax; j += 2)
        for (int i = m_frame % 2; i < m_size.x; i += 2)
        {
            dcmplx z0 = m_center + TexelToWorldOffset(ivec2(i, j));
            dcmplx z1, z2, z3, r0 = z0;
            //dcmplx r0(0.28693186889504513, 0.014286693904085048);
            //dcmplx r0(0.001643721971153, 0.822467633298876);
            //dcmplx r0(-1.207205434596, 0.315432814901);
            //dcmplx r0(-0.79192956889854, -0.14632423080102);
            //dcmplx r0(0.3245046418497685, 0.04855101129280834);
            int iter = MAX_ITERATIONS - 4;
            for (;;)
            {
                /* Unroll the loop: tests are more expensive to do at each
                 * iteration than the few extra multiplications. */
                z1 = z0 * z0 + r0;
                z2 = z1 * z1 + r0;
                z3 = z2 * z2 + r0;
                z0 = z3 * z3 + r0;
                if (sqlength(z0) >= maxsqlen)
                    break;
                iter -= 4;
                if (iter < 4)
                    break;
            }

            if (iter)
            {
                double n = sqlength(z0);

                if (sqlength(z1) >= maxsqlen) { iter += 3; n = sqlength(z1); }
                else if (sqlength(z2) >= maxsqlen) { iter += 2; n = sqlength(z2); }
                else if (sqlength(z3) >= maxsqlen) { iter += 1; n = sqlength(z3); }

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

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

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
#if !defined _XBOX && !defined USE_D3D9
            /* Create a texture of half the width and twice the height
             * so that we can upload four different subimages each frame. */
            glGenTextures(1, &m_texid);
            glBindTexture(GL_TEXTURE_2D, m_texid);
            glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_FORMAT,
                         m_size.x / 2, m_size.y * 2, 0,
                         TEXTURE_FORMAT, TEXTURE_TYPE, m_pixels);
#   if defined __CELLOS_LV2__
            /* We need this hint because by default the storage type is
             * GL_TEXTURE_SWIZZLED_GPU_SCE. */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_ALLOCATION_HINT_SCE,
                            GL_TEXTURE_TILED_GPU_SCE);
#   endif
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#elif defined _XBOX
            /* By default the X360 will swizzle the texture. Ask for linear. */
            g_d3ddevice->CreateTexture(m_size.x / 2, m_size.y * 2, 1,
                                       D3DUSAGE_WRITEONLY, D3DFMT_LIN_A8R8G8B8,
                                       D3DPOOL_DEFAULT, &m_tex, NULL);
#else
            g_d3ddevice->CreateTexture(m_size.x / 2, m_size.y * 2, 1,
                                       D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8,
                                       D3DPOOL_SYSTEMMEM, &m_tex, NULL);
#endif

            m_shader = Shader::Create(lolfx_11_fractal);

            m_vertexattrib = m_shader->GetAttribLocation("a_Vertex", VertexUsage::Position, 0);
            m_texattrib = m_shader->GetAttribLocation("a_TexCoord", VertexUsage::TexCoord, 0);
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

#if defined _XBOX || defined USE_D3D9

#else
#   if !defined HAVE_GLES_2X
        glEnable(GL_TEXTURE_2D);
#   endif
        glBindTexture(GL_TEXTURE_2D, m_texid);
#endif

        if (m_dirty[m_frame])
        {
            for (int i = 0; i < m_size.y; i += MAX_LINES * 2)
                m_donequeue.Pop();

            m_dirty[m_frame]--;

#if defined _XBOX || defined USE_D3D9
            D3DLOCKED_RECT rect;
#   if defined _XBOX
            m_tex->LockRect(0, &rect, NULL, D3DLOCK_NOOVERWRITE);
#   else
            m_tex->LockRect(0, &rect, NULL,
                            D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
#   endif
            for (int j = 0; j < m_size.y * 2; j++)
            {
                u8vec4 *line = (u8vec4 *)rect.pBits + j * rect.Pitch / 4;
                for (int i = 0; i < m_size.x / 2; i++)
                    line[i] = m_pixels[m_size.x / 2 * j + i];
            }
            m_tex->UnlockRect(0);
#elif defined __CELLOS_LV2__
            /* glTexSubImage2D is extremely slow on the PS3, to the point
             * that uploading the whole texture is 40 times faster. */
            glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_FORMAT,
                         m_size.x / 2, m_size.y * 2, 0,
                         TEXTURE_FORMAT, TEXTURE_TYPE, m_pixels);
#else
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, m_frame * m_size.y / 2,
                            m_size.x / 2, m_size.y / 2,
                            TEXTURE_FORMAT, TEXTURE_TYPE,
                            m_pixels + m_size.x * m_size.y / 4 * m_frame);
#endif
        }

        m_shader->Bind();
        m_shader->SetUniform(m_texeluni, m_texel_settings);
        m_shader->SetUniform(m_screenuni, m_screen_settings);
        m_shader->SetUniform(m_zoomuni, m_zoom_settings);
        m_vdecl->Bind();
        m_vdecl->SetStream(m_vbo, m_vertexattrib);
        m_vdecl->SetStream(m_tbo, m_texattrib);
#if defined _XBOX || defined USE_D3D9
        g_d3ddevice->SetTexture(0, m_tex);
        g_d3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__
#else
#endif
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 2);
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
    u8vec4 *m_pixels, *m_tmppixels, *m_palette;

    Shader *m_shader;
    ShaderAttrib m_vertexattrib, m_texattrib;
    ShaderUniform m_texeluni, m_screenuni, m_zoomuni;

    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo, *m_tbo;
#if defined USE_D3D9
    IDirect3DTexture9 *m_tex;
#elif defined _XBOX
    D3DTexture *m_tex;
#else
    GLuint m_texid;
#endif
    int m_frame, m_slices, m_dirty[4];
    bool m_ready, m_drag;

    dcmplx m_center, m_translate;
    double m_zoom_speed, m_radius;
    vec4 m_texel_settings, m_screen_settings;
    mat4 m_zoom_settings;
    dcmplx m_deltashift[4];
    double m_deltascale[4];

    /* Worker threads */
    Thread *m_threads[MAX_THREADS];
    Queue<int> m_spawnqueue, m_jobqueue, m_donequeue;

    /* Debug information */
#if !defined __native_client__
    Text *m_centertext, *m_mousetext, *m_zoomtext;
#endif
};

int main(int argc, char **argv)
{
    Application app("Tutorial 3: Fractal", ivec2(640, 480), 60.0f);

#if defined _MSC_VER && !defined _XBOX
    _chdir("..");
#elif defined _WIN32 && !defined _XBOX
    _chdir("../..");
#endif

    new DebugFps(5, 5);
    new Fractal(ivec2(640, 480));
    //new DebugRecord("fractalol.ogm", 60.0f);

    app.Run();

    return EXIT_SUCCESS;
}

