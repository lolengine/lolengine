//
// Lol Engine - EasyMesh tutorial
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

#include "core.h"

using namespace std;
using namespace lol;

class EasyMeshTutorial : public WorldEntity
{
public:
    void AddPiece(int x, int y, int h, int dx, int dy, int dh)
    {
        m_mesh.OpenBrace();
        for (int j = 0; j < y; ++j)
            for (int i = 0; i < x; ++i)
            {
                m_mesh.OpenBrace();
                m_mesh.Compile("ad12,2.2,0 ty.1 ac12,.2,2.4,2.2,0,1 ty.8 ac12,1.7,2.4,2.4,0,1 ty2.5");
                m_mesh.Translate(vec3(i * 8.f, (h - 1) * 3.2f, j * 8.f));

				m_mesh.CloseBrace();
            }
        m_mesh.OpenBrace();
        m_mesh.AppendFlatChamfBox(vec3(x * 8.f, h * 3.2f, y * 8.f), -.1f);
        m_mesh.Translate(vec3((x - 1) * 4.f, (h - 1) * 1.6f, (y - 1) * 4.f));
        m_mesh.CloseBrace();
        m_mesh.Translate(vec3(dx * 8.f, dh * 3.2f, dy * 8.f));

        m_mesh.CloseBrace();
    }

    EasyMeshTutorial()
    {
        m_angle = 0;
        m_mesh.Compile("sc#800 [asph10 20 20 20]");
		m_mesh.RadialJitter(0.2f);		
        //m_mesh.Compile("sc#94e scb#649 [asph3 7 7 7 tx-6 tz-9]");
        //m_mesh.Compile("sc#49e scb#469 [asph31 7 7 7 tx-6 tz9]");
        m_mesh.Compile("sc#1c1 scb#1c1");
        AddPiece(16, 16, 1, -8, -8, -4);

        /* Flat white LOL */
        m_mesh.Compile("sc#ccc scb#ccc");

        AddPiece(1, 4, 1, -7, -7, -3);
        AddPiece(2, 1, 1, -6, -4, -3);

        AddPiece(1, 4, 1, -3, -7, -3);
        AddPiece(2, 1, 1, -2, -7, -3);
        AddPiece(2, 1, 1, -2, -4, -3);
        AddPiece(1, 2, 1, -1, -6, -3);

        AddPiece(1, 4, 1, 1, -7, -3);
        AddPiece(2, 1, 1, 2, -4, -3);

        /* High red LOL */
        m_mesh.Compile("sc#e33 scb#e33");

        AddPiece(3, 1, 3, 3, 4, -3);
        AddPiece(1, 1, 3, 5, 4, 0);
        AddPiece(1, 1, 3, 5, 4, 3);
        AddPiece(1, 1, 3, 5, 4, 6);

        AddPiece(3, 1, 3, -1, 4, -3);
        AddPiece(1, 1, 3, 1, 4, 0);
        AddPiece(1, 1, 3, 1, 4, 3);
        AddPiece(1, 1, 3, -1, 4, 0);
        AddPiece(1, 1, 3, -1, 4, 3);
        AddPiece(3, 1, 3, -1, 4, 6);

        AddPiece(3, 1, 3, -5, 4, -3);
        AddPiece(1, 1, 3, -3, 4, 0);
        AddPiece(1, 1, 3, -3, 4, 3);
        AddPiece(1, 1, 3, -3, 4, 6);

        /* Some random crap */
        m_mesh.Compile("sc#e0e scb#e0e");
        AddPiece(1, 1, 1, -1, 0, 2);
        m_mesh.Compile("sc#0ee scb#0ee");
        AddPiece(2, 1, 1, -1, 0, 1);
        m_mesh.Compile("sc#e94 scb#e94");
        AddPiece(1, 1, 1, 0, 0, 0);
        m_mesh.Compile("sc#94e scb#94e");
        AddPiece(2, 1, 1, 0, 0, -1);
        m_mesh.Compile("sc#9e4 scb#9e4");
        AddPiece(1, 2, 3, -1, -1, -2);
        m_mesh.Compile("sc#49e scb#49e");
        AddPiece(2, 3, 1, 0, -1, -2);
        m_mesh.Compile("sc#4e9 scb#4e9");
        AddPiece(6, 2, 1, -2, 0, -3);
        m_mesh.Compile("sc#e49 scb#e49");
        AddPiece(6, 2, 1, -2, -2, -3);

        /* Center everything -- is it needed? */
        m_mesh.Compile("tx4 tz4");

        m_camera = new Camera(vec3(0.f, 600.f, 0.f),
                              vec3(0.f, 0.f, 0.f),
                              vec3(0, 1, 0));
        m_camera->SetPerspective(70.f, 960.f, 600.f, .1f, 1000.f);
        m_camera->SetTarget(vec3(0.f, -10.f, 0.f));
        m_camera->SetPosition(vec3(-100.f, 60.f, 0.f));
        Ticker::Ref(m_camera);

        m_ready = false;
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        m_angle += seconds * 45.0f;

        mat4 anim = mat4::rotate(m_angle, vec3(0, 1, 0));
        mat4 model = mat4::translate(vec3(0, 0, 0));

        m_matrix = model * anim;
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        if (!m_ready)
        {
            m_mesh.MeshConvert();
            m_ready = true;
        }

        Video::SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        m_mesh.Render(m_matrix);
    }

private:
    float m_angle;
    mat4 m_matrix;
    EasyMesh m_mesh;
    Camera *m_camera;

    bool m_ready;
};

int main(int argc, char **argv)
{
    Application app("Tutorial 5: EasyMesh", ivec2(960, 600), 60.0f);
    new EasyMeshTutorial();
    app.Run();

    return EXIT_SUCCESS;
}

