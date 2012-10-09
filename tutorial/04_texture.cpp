//
// Lol Engine - Noise tutorial
//
// Copyright: (c) 2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "loldebug.h"

using namespace std;
using namespace lol;

extern char const *lolfx_04_texture;

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

        m_heightmap = new uint8_t[4 * 512 * 1];
    }

    virtual ~TextureDemo()
    {
        delete m_heightmap;
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        /* Generate a new heightmap every 400 frames */
        if (m_frames % 400 == 0)
        {
            for (int i = 0, height = 64; i < 512; i++)
            {
                m_heightmap[4 * i] = height;
                m_heightmap[4 * i + 1] = 255; /* unused */
                m_heightmap[4 * i + 2] = 255; /* unused */
                m_heightmap[4 * i + 3] = 255; /* unused */
    
                height += rand() % 17 - 8;
                height += rand() % 17 - 8;
                height = std::max(15, std::min(height, 240));
            }
        }

        /* Slightly disturb the terrain */
        for (int i = 1; i < 511; i++)
        {
            int delta = (rand() & 1) ? 1 : -1;

            if (rand() & 3)
                continue;

            uint8_t &center = m_heightmap[4 * i];
            uint8_t &side1 = m_heightmap[4 * (i - delta)];
            uint8_t &side2 = m_heightmap[4 * (i + delta)];

            if (center > side1)
            {
                center--;
                side1++;
            }
            else if (center > side2)
            {
                center--;
                side2++;
            }
        }

        /* Update frame counter */
        ++m_frames;
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        /* Initialise GPU data */
        if (!m_ready)
        {
            m_texture = new Texture(ivec2(512, 1), PixelFormat::A8R8G8B8);

            m_shader = Shader::Create(lolfx_04_texture);
            m_coord = m_shader->GetAttribLocation("in_Position", VertexUsage::Position, 0);

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
    Application app("Tutorial 4: Texture", ivec2(640, 480), 60.0f);

    new TextureDemo();

    app.Run();
    return EXIT_SUCCESS;
}

