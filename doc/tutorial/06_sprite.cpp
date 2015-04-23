//
// Lol Engine - Sprite tutorial
//
// Copyright: (c) 2011-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2012 Daniel Stephens (artwork)
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>

using namespace lol;

class SpriteTutorial : public WorldEntity
{
public:
    SpriteTutorial()
    {
        m_camera = new Camera();
        m_camera->SetView(mat4(1.f));
        m_camera->SetProjection(mat4::ortho(0.f, 640.f, 0.f, 480.f, -100.f, 100.f));
        Scene& scene = Scene::GetScene();
        scene.PushCamera(m_camera);
        Ticker::Ref(m_camera);

        m_tileset = Tiler::Register("06_sprite.png");
        for (int i = 0; i < FRAME_COUNT; ++i)
            m_tileset->AddTile(ibox2(i * 24, 376, 24 + i * 24, 24 + 376));

        for (int i = 0; i < SPRITE_COUNT; ++i)
        {
            m_sprites.Push(vec3((float)rand(-96, 640), (float)rand(-96, 480), 0.f),
                           rand(0.f, 1.f));
        }

        m_ready = false;
    }

    ~SpriteTutorial()
    {
        Tiler::Deregister(m_tileset);

        Scene& scene = Scene::GetScene();
        scene.PopCamera(m_camera);
        Ticker::Unref(m_camera);
    }

    virtual void TickGame(float seconds)
    {
        for (int i = 0; i < SPRITE_COUNT; ++i)
        {
            m_sprites[i].m1.y += 50.f * seconds;
            m_sprites[i].m2 = lol::fmod(m_sprites[i].m2 + seconds, 1.f);
            if (m_sprites[i].m1.y > 480 + 48)
                m_sprites[i].m1.y = (float)rand(-96, -48);
        }

        WorldEntity::TickGame(seconds);
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        if (!m_ready)
        {
            g_renderer->SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
            m_ready = true;
        }

        for (int i = 0; i < SPRITE_COUNT; ++i)
        {
            int frame = (int)(m_sprites[i].m2 * FRAME_COUNT);
//            m_sprites[i].m1.z = frame;
            scene.AddTile(m_tileset, frame,
                          m_sprites[i].m1, 0, vec2(2.f), 0.f);
        }
    }

private:
    Camera *m_camera;
    TileSet *m_tileset;

    static int const SPRITE_COUNT = 192;
    static int const FRAME_COUNT = 16;
    array<vec3, float> m_sprites;

    bool m_ready;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 6: Sprite", ivec2(640, 480), 60.0f);
    new SpriteTutorial();
    app.Run();

    return EXIT_SUCCESS;
}

