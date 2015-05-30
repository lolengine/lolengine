//
//  Lol Engine — Graphing tutorial
//
//  Copyright © 2012—2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
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
    TextureDemo()
      : m_vertices({ vec2(-1.0,  1.0),
                     vec2(-1.0, -1.0),
                     vec2( 1.0, -1.0),
                     vec2(-1.0,  1.0),
                     vec2( 1.0, -1.0),
                     vec2( 1.0,  1.0), }),
        m_frames(0),
        m_ready(false)
    {
        m_heightmap.resize(TEXTURE_WIDTH * 1);
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        /* Generate a new heightmap at the beginning */
        if (m_frames == 0)
            memset(m_heightmap.data(), 255, m_heightmap.bytes());

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

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        /* Initialise GPU data */
        if (!m_ready)
        {
            m_texture = new Texture(ivec2(TEXTURE_WIDTH, 1), PixelFormat::Y_8);

            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(04_texture));
            m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
            m_texture_uni = m_shader->GetUniformLocation("u_Texture");

            m_vdecl = new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position));

            m_vbo = new VertexBuffer(m_vertices.bytes());
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, &m_vertices[0], m_vertices.bytes());
            m_vbo->Unlock();

            m_ready = true;

            /* FIXME: this object never cleans up */
        }

        /* Send new heightmap to GPU */
        m_texture->SetData(m_heightmap.data());

        m_shader->Bind();
        m_shader->SetUniform(m_texture_uni, m_texture->GetTextureUniform(), 0);
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
    }

private:
    array<vec2> m_vertices;
    Texture *m_texture;
    Shader *m_shader;
    ShaderAttrib m_coord;
    ShaderUniform m_texture_uni;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    array<uint8_t> m_heightmap;
    int m_frames;
    bool m_ready;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 4: Texture", ivec2(1280, 720), 60.0f);

//new DebugFps(50, 50);
    new TextureDemo();

    app.Run();
    return EXIT_SUCCESS;
}

