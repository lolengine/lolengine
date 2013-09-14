//
// Lol Engine - Graphing tutorial
//
// Copyright: (c) 2012-2013 Sam Hocevar <sam@hocevar.net>
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

static int const TEXTURE_WIDTH = 256;

LOLFX_RESOURCE_DECLARE(04_texture);

class TextureDemo : public WorldEntity
{
public:
    TextureDemo() :
        m_frames(0),
        m_ready(false)
    {
        m_vertices << vec2(-1.0,  1.0);
        m_vertices << vec2(-1.0, -1.0);
        m_vertices << vec2( 1.0, -1.0);
        m_vertices << vec2(-1.0,  1.0);
        m_vertices << vec2( 1.0, -1.0);
        m_vertices << vec2( 1.0,  1.0);

        m_heightmap = new uint8_t[TEXTURE_WIDTH * 1];
    }

    virtual ~TextureDemo()
    {
        delete m_heightmap;
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        /* Generate a new heightmap at the beginning */
        if (m_frames == 0)
            memset(m_heightmap, 255, TEXTURE_WIDTH);

        /* Scroll left */
        for (int i = 0; i < TEXTURE_WIDTH - 1; i++)
            m_heightmap[i] = m_heightmap[i + 1];

        int height = m_heightmap[TEXTURE_WIDTH - 1];
        height = (height + 127 + 40 * lol::sin(m_frames * 0.03) + rand() % 97 - 38) / 2;
        height = std::max(15, std::min(height, 240));
        m_heightmap[TEXTURE_WIDTH - 1] = height;

        /* Update frame counter */
        ++m_frames;
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        /* Initialise GPU data */
        if (!m_ready)
        {
            m_texture = new Texture(ivec2(TEXTURE_WIDTH, 1), PixelFormat::Y_8);

            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(04_texture));
            m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
            m_texture_uni = m_shader->GetUniformLocation("u_Texture");

            m_vdecl = new VertexDeclaration(VertexStream<vec2>(VertexUsage::Position));

            m_vbo = new VertexBuffer(m_vertices.Bytes());
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, &m_vertices[0], m_vertices.Bytes());
            m_vbo->Unlock();

            m_ready = true;

            /* FIXME: this object never cleans up */
        }

        /* Send new heightmap to GPU */
        m_texture->SetData(m_heightmap);

        m_shader->Bind();
        m_shader->SetUniform(m_texture_uni, m_texture->GetTexture(), 0);
        m_vdecl->SetStream(m_vbo, m_coord);
        m_vdecl->Bind();
        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 6);
        m_vdecl->Unbind();
    }

private:
    Array<vec2> m_vertices;
    Texture *m_texture;
    Shader *m_shader;
    ShaderAttrib m_coord;
    ShaderUniform m_texture_uni;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    uint8_t *m_heightmap;
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

