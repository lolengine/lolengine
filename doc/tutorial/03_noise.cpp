//
//  Lol Engine — Noise tutorial
//
//  Copyright © 2012—2019 Sam Hocevar <sam@hocevar.net>
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

#include <lol/engine.h>
#include "loldebug.h"

using namespace lol;

LOLFX_RESOURCE_DECLARE(03_noise);

class NoiseDemo : public WorldEntity
{
public:
    virtual bool init_draw() override
    {
        array<vec2> vertices
        {
            vec2(-1.0,  1.0),
            vec2(-1.0, -1.0),
            vec2( 1.0, -1.0),
            vec2(-1.0,  1.0),
            vec2( 1.0, -1.0),
            vec2( 1.0,  1.0),
        };

        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(03_noise));
        m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
        m_time_uni = m_shader->GetUniformLocation("u_time");

        m_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec2>(VertexUsage::Position));

        m_vbo = std::make_shared<VertexBuffer>(vertices.bytes());
        m_vbo->set_data(vertices.data(), vertices.bytes());

        return true;
    }

    virtual void tick_draw(float seconds, Scene &scene) override
    {
        WorldEntity::tick_draw(seconds, scene);

        m_time += seconds;

        m_shader->Bind();
        m_shader->SetUniform(m_time_uni, m_time);
        m_vdecl->Bind();
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
    }

    virtual bool release_draw() override
    {
        m_shader.reset();
        m_vdecl.reset();
        m_vbo.reset();
        return true;
    }

private:
    std::shared_ptr<Shader> m_shader;
    ShaderAttrib m_coord;
    ShaderUniform m_time_uni;
    std::shared_ptr<VertexDeclaration> m_vdecl;
    std::shared_ptr<VertexBuffer> m_vbo;
    float m_time = 0.0;
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    Application app("Tutorial 3: Noise", ivec2(1280, 720), 60.0f);

    new NoiseDemo();

    app.Run();
    return EXIT_SUCCESS;
}

