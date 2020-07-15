//
//  Lol Engine — Framebuffer Object tutorial
//
//  Copyright © 2012—2020 Sam Hocevar <sam@hocevar.net>
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

#include <memory>

using namespace lol;

LOLFX_RESOURCE_DECLARE(08_fbo);

class FBO : public WorldEntity
{
public:
    virtual void tick_game(float seconds) override
    {
        WorldEntity::tick_game(seconds);

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

    virtual bool init_draw() override
    {
        std::vector<vec2> vertices
        {
            vec2( 1.0,  1.0),
            vec2(-1.0, -1.0),
            vec2( 1.0, -1.0),
            vec2(-1.0, -1.0),
            vec2( 1.0,  1.0),
            vec2(-1.0,  1.0),
        };

        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(08_fbo));
        m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
        m_uni_flag = m_shader->GetUniformLocation("u_flag");
        m_uni_point = m_shader->GetUniformLocation("u_point");
        m_uni_color = m_shader->GetUniformLocation("u_color");
        m_uni_texture = m_shader->GetUniformLocation("u_texture");

        m_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec2>(VertexUsage::Position));

        m_vbo = std::make_shared<VertexBuffer>(vertices.size() * sizeof(vertices[0]));
        m_vbo->set_data(vertices.data(), vertices.size() * sizeof(vertices[0]));

        // Create the back buffer and clear it
        m_backbuffer = std::make_shared<Framebuffer>(Video::GetSize());
        m_backbuffer->Bind();
        {
            Scene& scene = Scene::GetScene();
            render_context rc(scene.get_renderer());
            rc.clear_color(vec4(0.f, 0.f, 0.f, 1.f));
            rc.clear_depth(1.f);
            scene.get_renderer()->clear(ClearMask::Color | ClearMask::Depth);
        }
        m_backbuffer->Unbind();

        return true;
    }

    virtual void tick_draw(float seconds, Scene &scene) override
    {
        WorldEntity::tick_draw(seconds, scene);

        /* FIXME: we should just disable depth test in the shader */
        render_context rc(scene.get_renderer());
        rc.depth_func(DepthFunc::Disabled);

        /* FIXME: this no longer works because we don’t restore the
         * actually bound framebuffer. */
        m_backbuffer->Bind();
        m_shader->Bind();
        m_shader->SetUniform(m_uni_flag, 0.f);
        m_shader->SetUniform(m_uni_point, m_hotspot);
        m_shader->SetUniform(m_uni_color, m_color);
        m_vdecl->Bind();
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
        m_shader->Unbind();
        m_backbuffer->Unbind();

        m_shader->Bind();
        m_shader->SetUniform(m_uni_flag, 1.f);
        m_shader->SetUniform(m_uni_texture, m_backbuffer->GetTextureUniform(), 0);
        m_vdecl->Bind();
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
        m_shader->Unbind();
    }

    virtual bool release_draw() override
    {
        m_shader.reset();
        m_vdecl.reset();
        m_vbo.reset();
        m_backbuffer.reset();
        return true;
    }

private:
    std::shared_ptr<Shader> m_shader;
    ShaderAttrib m_coord;
    ShaderUniform m_uni_flag, m_uni_point, m_uni_color, m_uni_texture;
    std::shared_ptr<VertexDeclaration> m_vdecl;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<Framebuffer> m_backbuffer;
    double m_time = 0.0f;
    vec3 m_hotspot, m_color;
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    app app("Tutorial 08: Framebuffer Object", ivec2(512, 512), 60.0f);
    new FBO();
    app.run();

    return EXIT_SUCCESS;
}
