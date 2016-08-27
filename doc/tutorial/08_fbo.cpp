//
//  Lol Engine — Framebuffer Object tutorial
//
//  Copyright © 2012—2015 Sam Hocevar <sam@hocevar.net>
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

#include <lol/engine.h>

using namespace lol;

LOLFX_RESOURCE_DECLARE(08_fbo);

class FBO : public WorldEntity
{
public:
    FBO()
      : m_vertices { vec2( 1.0,  1.0),
                     vec2(-1.0, -1.0),
                     vec2( 1.0, -1.0),
                     vec2(-1.0, -1.0),
                     vec2( 1.0,  1.0),
                     vec2(-1.0,  1.0), },
        m_time(0.f),
        m_ready(false)
    {
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        m_time += seconds;
        m_hotspot = 0.4f * vec3(
            lol::sin((float)m_time * 4.f) + lol::cos((float)m_time * 5.3f),
            lol::sin((float)m_time * 5.7f) + lol::cos((float)m_time * 4.4f),
            lol::sin((float)m_time * 5.f));
        m_color = 0.25f * vec3(1.1f + lol::sin((float)m_time * 2.5f + 1.f),
                               1.1f + lol::sin((float)m_time * 2.8f + 1.3f),
                               1.1f + lol::sin((float)m_time * 2.7f));
        /* Saturate dot color */
        float x = std::max(m_color.x, std::max(m_color.y, m_color.z));
        m_color /= x;
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        if (!m_ready)
        {
            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(08_fbo));
            m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
            m_uni_flag = m_shader->GetUniformLocation("in_Flag");
            m_uni_point = m_shader->GetUniformLocation("in_Point");
            m_uni_color = m_shader->GetUniformLocation("in_Color");
            m_uni_texture = m_shader->GetUniformLocation("u_texture");

            m_vdecl = new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position));

            m_vbo = new VertexBuffer(m_vertices.bytes());
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, &m_vertices[0], m_vertices.bytes());
            m_vbo->Unlock();

            m_fbo = new Framebuffer(Video::GetSize());
            m_fbo->Bind();

            {
                RenderContext rc;
                rc.SetClearColor(vec4(0.f, 0.f, 0.f, 1.f));
                rc.SetClearDepth(1.f);
                Renderer::Get()->Clear(ClearMask::Color | ClearMask::Depth);
            }

            m_fbo->Unbind();

            m_ready = true;

            /* FIXME: this object never cleans up */
        }

        /* FIXME: we should just disable depth test in the shader */
        RenderContext rc;
        rc.SetDepthFunc(DepthFunc::Disabled);

        m_fbo->Bind();
        m_shader->Bind();

        m_shader->SetUniform(m_uni_flag, 0.f);
        m_shader->SetUniform(m_uni_point, m_hotspot);
        m_shader->SetUniform(m_uni_color, m_color);
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
        m_shader->Unbind();
        m_fbo->Unbind();

        m_shader->Bind();
        m_shader->SetUniform(m_uni_flag, 1.f);
        m_shader->SetUniform(m_uni_texture, m_fbo->GetTextureUniform(), 0);
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
        m_shader->Unbind();
    }

private:
    array<vec2> m_vertices;
    Shader *m_shader;
    ShaderAttrib m_coord;
    ShaderUniform m_uni_flag, m_uni_point, m_uni_color, m_uni_texture;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    Framebuffer *m_fbo;
    double m_time;
    vec3 m_hotspot, m_color;
    bool m_ready;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 08: Framebuffer Object", ivec2(512, 512), 60.0f);

    new FBO();

    app.Run();
    return EXIT_SUCCESS;
}

