//
// Lol Engine - Framebuffer Object tutorial
//
// Copyright: (c) 2013-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2013-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "loldebug.h"

using namespace std;
using namespace lol;

LOLFX_RESOURCE_DECLARE(12_voronoi);
LOLFX_RESOURCE_DECLARE(12_voronoi_setup);
LOLFX_RESOURCE_DECLARE(12_voronoi_distance);
LOLFX_RESOURCE_DECLARE(12_distance);
LOLFX_RESOURCE_DECLARE(12_texture_to_screen);

enum FboType
{
    SrcVoronoiFbo,
    VoronoiFbo,
    DistanceVoronoiFbo,
    DistanceFbo,

    MaxFboType
};

class Voronoi : public WorldEntity
{
public:
    Voronoi()
    {
        m_vertices << vec2( 1.0,  1.0);
        m_vertices << vec2(-1.0, -1.0);
        m_vertices << vec2( 1.0, -1.0);
        m_vertices << vec2(-1.0, -1.0);
        m_vertices << vec2( 1.0,  1.0);
        m_vertices << vec2(-1.0,  1.0);
        m_ready = false;
        m_cur_fbo = 0;
        m_time = .0f;
        m_timer = -1.0f;
        mode = 0;
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        {
            //Shutdown logic
            if (Input::WasReleased(Key::Escape))
                Ticker::Shutdown();
        }

        m_time += seconds;
        m_hotspot = 0.4f * vec3(lol::sin(m_time * 4.f) + lol::cos(m_time * 5.3f),
                                lol::sin(m_time * 5.7f) + lol::cos(m_time * 4.4f),
                                lol::sin(m_time * 5.f));
        m_color = 0.25f * vec3(1.1f + lol::sin(m_time * 2.5f + 1.f),
                               1.1f + lol::sin(m_time * 2.8f + 1.3f),
                               1.1f + lol::sin(m_time * 2.7f));
        /* Saturate dot color */
        float x = std::max(m_color.x, std::max(m_color.y, m_color.z));
        m_color /= x;
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        if (!m_ready)
        {
            m_vdecl = new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position));

            m_vbo = new VertexBuffer(m_vertices.Bytes());
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, &m_vertices[0], m_vertices.Bytes());
            m_vbo->Unlock();

            m_screen_shader = Shader::Create(LOLFX_RESOURCE_NAME(12_texture_to_screen));
            m_screen_coord = m_screen_shader->GetAttribLocation("in_position", VertexUsage::Position, 0);
            m_screen_texture = m_screen_shader->GetUniformLocation("in_texture");

            for (int i = 0; i < MaxFboType; ++i)
            {
                m_fbos.Push(new Framebuffer(Video::GetSize()), 0, Array<ShaderUniform>(), Array<ShaderAttrib>() );

                if (i == SrcVoronoiFbo)
                {
                    m_fbos[i].m2 = Shader::Create(LOLFX_RESOURCE_NAME(12_voronoi_setup));
                    m_fbos[i].m3 << m_fbos[i].m2->GetUniformLocation("in_texture");
                    m_fbos[i].m3 << m_fbos[i].m2->GetUniformLocation("in_source_point");
                    m_fbos[i].m3 << m_fbos[i].m2->GetUniformLocation("in_screen_res");
                    m_fbos[i].m4 << m_fbos[i].m2->GetAttribLocation("in_position", VertexUsage::Position, 0);
                }
                else if (i == VoronoiFbo)
                {
                    m_fbos[i].m2 = Shader::Create(LOLFX_RESOURCE_NAME(12_voronoi));
                    m_fbos[i].m3 << m_fbos[i].m2->GetUniformLocation("in_texture");
                    m_fbos[i].m3 << m_fbos[i].m2->GetUniformLocation("in_step");
                    m_fbos[i].m3 << m_fbos[i].m2->GetUniformLocation("in_screen_res");
                    m_fbos[i].m4 << m_fbos[i].m2->GetAttribLocation("in_position", VertexUsage::Position, 0);
                }
                else if (i == DistanceVoronoiFbo)
                {
                    m_fbos[i].m2 = Shader::Create(LOLFX_RESOURCE_NAME(12_voronoi_distance));
                }
                else if (i == DistanceFbo)
                {
                    m_fbos[i].m2 = Shader::Create(LOLFX_RESOURCE_NAME(12_distance));
                }

                m_fbos.Last().m1->Bind();
                Video::SetClearColor(vec4(0.f, 0.f, 0.f, 1.f));
                Video::SetClearDepth(1.f);
                Video::Clear(ClearMask::Color | ClearMask::Depth);
                m_fbos.Last().m1->Unbind();
            }

            temp_buffer = new Framebuffer(Video::GetSize());
            temp_buffer->Bind();
            Video::SetClearColor(vec4(0.f, 0.f, 0.f, 1.f));
            Video::SetClearDepth(1.f);
            Video::Clear(ClearMask::Color | ClearMask::Depth);
            temp_buffer->Unbind();

            m_ready = true;
            /* FIXME: this object never cleans up */

            //SRC SETUP
            m_cur_fbo = VoronoiFbo;
        }

        {
            //Shutdown logic
            if (Input::WasReleased(Key::O))
                voronoi_points.Pop();
            else if (Input::WasReleased(Key::P))
                voronoi_points.Push(vec3(rand<float>(512.f), rand<float>(512.f), .0f),
                        vec2(64.f + rand<float>(64.f), 64.f + rand<float>(64.f)));
            else if (Input::WasReleased(Key::F1))
                m_cur_fbo = SrcVoronoiFbo;
            else if (Input::WasReleased(Key::F2))
                m_cur_fbo = VoronoiFbo;
            else if (Input::WasReleased(Key::F3))
            {
                voronoi_points.Empty();
                if (mode == 0)
                {
                    int i = 4;
                    while (i-- > 0)
                        voronoi_points.Push(vec3(rand<float>(512.f), rand<float>(512.f), .0f),
                                            vec2(64.f + rand<float>(64.f), 64.f + rand<float>(64.f))
                                            //vec2(0.f)
                                            );
                    mode = 1;
                }
                else
                {
                    mode = 0;
                }
            }
        }

        if (mode == 0)
        {
            voronoi_points.Empty();
            int maxi = 6;
            for (int i = 0; i < maxi; ++i)
            {
                voronoi_points.Push(vec3(256.f) + 196.f * vec3(lol::cos(m_time + i * 2.f * F_PI / maxi), lol::sin(m_time + i * 2.f * F_PI / maxi), .0f), vec2(.0f));
                voronoi_points.Push(vec3(256.f) + 128.f * vec3(lol::cos(-m_time + i * 2.f * F_PI / maxi), lol::sin(-m_time + i * 2.f * F_PI / maxi), .0f), vec2(.0f));
                voronoi_points.Push(vec3(256.f) + 64.f *  vec3(lol::cos(m_time + i * 2.f * F_PI / maxi), lol::sin(m_time + i * 2.f * F_PI / maxi), .0f), vec2(.0f));
                voronoi_points.Push(vec3(256.f) + 32.f *  vec3(lol::cos(-m_time + i * 2.f * F_PI / maxi), lol::sin(-m_time + i * 2.f * F_PI / maxi), .0f), vec2(.0f));
            }
            voronoi_points.Push(vec3(256.f), vec2(.0f));
        }

        temp_buffer->Bind();
        Video::SetClearColor(vec4(0.f, 0.f, 0.f, 1.f));
        Video::SetClearDepth(1.f);
        Video::Clear(ClearMask::Color | ClearMask::Depth);
        temp_buffer->Unbind();

        {
            vec2 limit(1.f, 511.f);
            //SRC SETUP
            for (int j = 0; j < voronoi_points.Count(); ++j)
            {
                voronoi_points[j].m1 = vec3(voronoi_points[j].m1.xy + voronoi_points[j].m2 * seconds, voronoi_points[j].m1.z);
                if (voronoi_points[j].m1.x >= limit.y || voronoi_points[j].m1.x <= limit.x)
                {
                    voronoi_points[j].m2.x *= -1.f;
                    voronoi_points[j].m1.x = clamp(voronoi_points[j].m1.x, limit.x, limit.y);
                }
                if (voronoi_points[j].m1.y >= limit.y || voronoi_points[j].m1.y <= limit.x)
                {
                    voronoi_points[j].m2.y *= -1.f;
                    voronoi_points[j].m1.y = clamp(voronoi_points[j].m1.y, limit.x, limit.y);
                }
                voronoi_points[j].m1.z = ((float)j + 1) / ((float)voronoi_points.Count());
            }

            int f = SrcVoronoiFbo;

            m_fbos[f].m1->Bind();
            Video::SetClearColor(vec4(0.f, 0.f, 0.f, 1.f));
            Video::SetClearDepth(1.f);
            Video::Clear(ClearMask::Color | ClearMask::Depth);
            m_fbos[f].m1->Unbind();

            int buf = voronoi_points.Count() % 2;
            for (int j = 0; j < voronoi_points.Count(); ++j)
            {
                Framebuffer *dst_buf;
                Framebuffer *src_buf;
                
                if (buf)
                {
                    dst_buf = m_fbos[f].m1;
                    src_buf = temp_buffer;
                }
                else
                {
                    src_buf = m_fbos[f].m1;
                    dst_buf = temp_buffer;
                }

                dst_buf->Bind();
                /* FIXME: we should just disable depth test in the shader */
                Video::Clear(ClearMask::Depth);
                m_fbos[f].m2->Bind();

                int i = 0;
                m_fbos[f].m2->SetUniform(m_fbos[f].m3[i++], src_buf->GetTexture(), 0); //"in_texture"
                m_fbos[f].m2->SetUniform(m_fbos[f].m3[i++], voronoi_points[j].m1); //"in_source_point"
                m_fbos[f].m2->SetUniform(m_fbos[f].m3[i++], vec2(512.f, 512.f)); //"in_screen_res"

                m_vdecl->SetStream(m_vbo, m_fbos[f].m4.Last());
                m_vdecl->Bind();
                m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
                m_vdecl->Unbind();
                m_fbos[f].m2->Unbind();
                dst_buf->Unbind();

                buf = 1 - buf;
            }
        }

        Video::Clear(ClearMask::Color | ClearMask::Depth);

        //FRAME BUFFER DRAW
        m_timer -= seconds;
        if (m_timer < .0f && m_cur_fbo != SrcVoronoiFbo)
        {
            //m_timer = 1.0f;
            m_fbos[m_cur_fbo].m1->Bind();
            Video::SetClearColor(vec4(0.f, 0.f, 0.f, 1.f));
            Video::SetClearDepth(1.f);
            Video::Clear(ClearMask::Color | ClearMask::Depth);
            m_fbos[m_cur_fbo].m1->Unbind();

            ivec2 curres = ivec2(512, 512) / 2;
            int buf = 0;
            while (1)
            {
                Framebuffer *dst_buf;
                Framebuffer *src_buf;
                Shader *shader;

                if (curres == ivec2(0))
                    shader = m_screen_shader;
                else
                    shader = m_fbos[m_cur_fbo].m2;

                if (curres.x == 256)
                    src_buf = m_fbos[SrcVoronoiFbo].m1;
                else if (buf)
                    src_buf = m_fbos[m_cur_fbo].m1;
                else
                    src_buf = temp_buffer;

                if (buf)
                    dst_buf = temp_buffer;
                else
                    dst_buf = m_fbos[m_cur_fbo].m1;

                dst_buf->Bind();
                /* FIXME: we should just disable depth test in the shader */
                Video::Clear(ClearMask::Depth);
                shader->Bind();

                //08_FBO ??
    #if _XBOX
                /* FIXME: the Xbox enforces full EDRAM clears on each frame, so
                * we cannot expect the render target contents to be preserved.
                * This code snippet should be moved inside the Framebuffer class. */
                //m_fbos[m_cur_fbo].m2->SetUniform(m_uni_flag, 1.f);
                //m_fbos[m_cur_fbo].m2->SetUniform(m_uni_texture, m_fbo->GetTexture(), 0);
                //m_vdecl->SetStream(m_vbo, m_fbos[m_cur_fbo].m4.Last());
                //m_vdecl->Bind();
                //m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
                //m_vdecl->Unbind();
    #endif

                int i = 0;
                if (curres == ivec2(0))
                    m_screen_shader->SetUniform(m_screen_texture, src_buf->GetTexture(), 0);
                else if (m_cur_fbo == VoronoiFbo)
                {
                    shader->SetUniform(m_fbos[m_cur_fbo].m3[i++], src_buf->GetTexture(), 0); //"in_texture"
                    shader->SetUniform(m_fbos[m_cur_fbo].m3[i++], ((float)curres.x) / 512.f); //"in_step"
                    shader->SetUniform(m_fbos[m_cur_fbo].m3[i++], vec2(512.f, 512.f)); //"in_screen_res"
                }

                m_vdecl->SetStream(m_vbo, m_fbos[m_cur_fbo].m4.Last());
                m_vdecl->Bind();
                m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
                m_vdecl->Unbind();
                m_fbos[m_cur_fbo].m2->Unbind();
                dst_buf->Unbind();

                if (curres == ivec2(0))
                    break;
                if (curres == ivec2(1))
                {
                    if (buf == 1)
                        curres = ivec2(0);
                    else
                        break;
                }
                buf = 1 - buf;
                curres /= 2;
            }
        }

        //SCREEN DRAW
        m_screen_shader->Bind();
        m_screen_shader->SetUniform(m_screen_texture, m_fbos[m_cur_fbo].m1->GetTexture(), 0);
        m_vdecl->SetStream(m_vbo, m_screen_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
        m_screen_shader->Unbind();
    }

private:
    Array<vec3, vec2> voronoi_points;
    Array<vec2> m_vertices;
    Shader *m_screen_shader;
    ShaderAttrib m_screen_coord;
    ShaderUniform m_screen_texture;

    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;

    Array<Framebuffer *, Shader *, Array<ShaderUniform>, Array<ShaderAttrib> > m_fbos;
    Framebuffer *temp_buffer;

    int mode;
    int m_cur_fbo;
    double m_time;
    vec3 m_hotspot, m_color;
    bool m_ready;
    float m_timer;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 12: Jump Flooding Algorithm & Voronoi", ivec2(512, 512), 60.0f);

    new Voronoi();

    app.Run();
    return EXIT_SUCCESS;
}

