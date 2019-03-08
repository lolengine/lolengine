//
//  Lol Engine — Graphing tutorial
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

#include <lol/engine.h>
#include "loldebug.h"

using namespace lol;

static int const TEXTURE_WIDTH = 256;

LOLFX_RESOURCE_DECLARE(04_texture);

class TextureDemo : public WorldEntity
{
public:
    virtual bool init_game() override
    {
        /* Generate a new heightmap at the beginning */
        m_heightmap.resize(TEXTURE_WIDTH * 1);
        memset(m_heightmap.data(), 255, m_heightmap.bytes());

        return true;
    }

    virtual void tick_game(float seconds) override
    {
        WorldEntity::tick_game(seconds);

        /* Scroll left */
        for (int i = 0; i < m_heightmap.count() - 1; i++)
            m_heightmap[i] = m_heightmap[i + 1];

        int height = m_heightmap.last();
        height = (int)(height + 127 + 40 * lol::sin(m_frames * 0.03) + rand() % 97 - 38) / 2;
        height = std::max(15, std::min(height, 240));
        m_heightmap.last() = height;

        /* Update frame counter */
        ++m_frames;
    }

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

        m_texture = std::make_shared<Texture>(ivec2(TEXTURE_WIDTH, 1), PixelFormat::Y_8);

        m_shader = Shader::Create(LOLFX_RESOURCE_NAME(04_texture));
        m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
        m_texture_uni = m_shader->GetUniformLocation("u_texture");

        m_vdecl = std::make_shared<VertexDeclaration>(VertexStream<vec2>(VertexUsage::Position));

        m_vbo = std::make_shared<VertexBuffer>(vertices.bytes());
        void *data = m_vbo->Lock(0, 0);
        memcpy(data, vertices.data(), vertices.bytes());
        m_vbo->Unlock();

        return true;
    }

    virtual void tick_draw(float seconds, Scene &scene)
    {
        WorldEntity::tick_draw(seconds, scene);

        /* Send new heightmap to GPU */
        m_texture->Bind();
        m_texture->SetData(m_heightmap.data());

        m_shader->Bind();
        m_shader->SetUniform(m_texture_uni, m_texture->GetTextureUniform(), 0);
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
    }

    virtual bool release_draw() override
    {
        m_texture.reset();
        m_shader.reset();
        m_vdecl.reset();
        m_vbo.reset();
        return true;
    }

private:
    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<Shader> m_shader;
    ShaderAttrib m_coord;
    ShaderUniform m_texture_uni;
    std::shared_ptr<VertexDeclaration> m_vdecl;
    std::shared_ptr<VertexBuffer> m_vbo;
    array<uint8_t> m_heightmap;
    int m_frames = 0;
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    Application app("Tutorial 4: Texture", ivec2(1280, 720), 60.0f);

    new TextureDemo();

    app.Run();
    return EXIT_SUCCESS;
}

