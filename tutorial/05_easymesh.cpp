//
// Lol Engine - EasyMesh tutorial
//
// Copyright: (c) 2011-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2012-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
    EasyMeshTutorial()
    {
        m_gears.Push(EasyMesh(), mat4(1.0f), 0.0f);
        m_gears.Push(EasyMesh(), mat4(1.0f), 0.0f);
        m_gears.Push(EasyMesh(), mat4(1.0f), 180.0f / 18);
        m_gears.Push(EasyMesh(), mat4(1.0f), 180.0f / 18);
        m_gears.Push(EasyMesh(), mat4(1.0f), 180.0f / 18);

        m_gears[0].m1.Compile("[sc#00f ab 8 1 8 ty -.25]"
                              "[sc#f9f scb#f9f acg 12 10 5 5 20 20 5 5 0.1 0 s .1 .1 .1 ty -.1 csgu]"
                              "[sc#fff scb#000 acg 12 10 10 10 20 20 5 5 0.1 0 s .05 .05 .05 tx -1.5 ty .3 csgu]"
                              "[sc#00f ab 5 3 9 tx 2.5 csgs]"
                              "[[ sc#fff ab 3   1.4 2   tx -2 tz -2 "
                              "[sc#fff ab 2.1  .7 1.1 ty .5 tx -1.4 tz -1.4 csgs] mz] csgu]");
        //m_gears[0].m1.Compile("[sc#f9f scb#f9f acg 12 10 5 5 20 20 5 5 0.1 0 s .1 .1 .1 [sc#00f ab 3 1 2 ty .25 tx 1 csgs]]");
        m_gears[1].m1.Compile("sc#ff9 scb#ff9 acg 54 10 95 95 90 90 -5 -5 0.1 0 s .1 .1 .1");
        //m_gears[2].m1.Compile("sc#9ff scb#9ff acg 18 10 5 5 30 30 5 5 0.1 0 s .1 .1 .1 [sc#00f scb#00f ab 2 2 2 tx 1.5]");
        //m_gears[3].m1.Compile("sc#9ff scb#9ff acg 18 10 5 5 30 30 5 5 0.1 0 s .1 .1 .1 [sc#00f scb#00f ab 2 2 2 tx 1.5]");
        //m_gears[4].m1.Compile("sc#9ff scb#9ff acg 18 10 5 5 30 30 5 5 0.1 0 s .1 .1 .1 [sc#00f scb#00f ab 2 2 2 tx 1.5]");
        m_gears[2].m1.Compile("[sc#0f0 ab 2 2 2 t .8 .8 .8 rx 20 ry 20 [sc#00f ab 2 2 2 tx 0 csgu]]");
        m_gears[3].m1.Compile("[sc#0f0 ab 2 2 2 t .8 .8 .8 rx 20 ry 20 [sc#00f ab 2 2 2 tx 0 csgs]]");
        m_gears[4].m1.Compile("[sc#0f0 ab 2 2 2 t .8 .8 .8 rx 20 ry 20 [sc#00f ab 2 2 2 tx 0 csga]]");

        m_angle = 0;

        m_camera = new Camera();
        m_camera->SetProjection(mat4::perspective(30.f, 960.f, 600.f, .1f, 1000.f));
        m_camera->SetView(mat4::lookat(vec3(-15.f, 5.f, 0.f),
                                       vec3(0.f, -1.f, 0.f),
                                       vec3(0.f, 1.f, 0.f)));
        Scene::GetDefault()->PushCamera(m_camera);

        /* Add a white directional light */
        m_light1 = new Light();
        m_light1->SetPosition(vec4(0.2f, 0.2f, 0.f, 0.f));
        m_light1->SetColor(vec4(0.5f, 0.5f, 0.5f, 1.f));
        Ticker::Ref(m_light1);

        /* Add an orangeish point light */
        m_light2 = new Light();
        m_light2->SetPosition(vec4(-15.f, 15.f, 15.f, 1.f));
        m_light2->SetColor(vec4(0.4f, 0.3f, 0.2f, 1.f));
        Ticker::Ref(m_light2);

        m_ready = false;
    }

    ~EasyMeshTutorial()
    {
        Scene::GetDefault()->PopCamera(m_camera);
        Ticker::Unref(m_light1);
        Ticker::Unref(m_light2);
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        m_angle += seconds * 70.0f;
        m_mat = mat4::rotate(10.0f, vec3(0, 0, 1))
                * mat4::rotate(100, vec3(0, 1, 0));
        //      * mat4::rotate(m_angle, vec3(0, 1, 0));

        m_gears[0].m3 += seconds * 20.0f;
        m_gears[1].m3 += seconds * 20.0f * -2 / 9;
        m_gears[2].m3 += seconds * 20.0f * -2 / 3;
        m_gears[3].m3 += seconds * 20.0f * -2 / 3;
        m_gears[4].m3 += seconds * 20.0f * -2 / 3;

        m_gears[0].m2 = mat4::translate(vec3(0, -1, 0))
                      * mat4::rotate(m_gears[0].m3 - 130.0f, vec3(0, 1, 0))
                      * mat4::rotate(40.0f, vec3(0, 0, 1));
        m_gears[1].m2 = mat4::translate(vec3(0, 0, 0))
                      * mat4::rotate(m_gears[1].m3, vec3(0, 1, 0));
        m_gears[2].m2 = mat4::translate(vec3(0, 0, 5.5f))
                      * mat4::rotate(m_gears[2].m3 - 40.0f, vec3(0, 1, 0))
                      * mat4::rotate(90.0f, vec3(0, 0, 1));
        m_gears[3].m2 = mat4::translate(vec3(5.5f * lol::sqrt(3.f) * 0.5f, 0, -5.5f * 0.5f))
                      * mat4::rotate(m_gears[3].m3 - 140.0f, vec3(0, 1, 0))
                      * mat4::rotate(-70.0f, vec3(0, 0, 1));
        m_gears[4].m2 = mat4::translate(vec3(-5.5f * lol::sqrt(3.f) * 0.5f, 0, -5.5f * 0.5f))
                      * mat4::rotate(m_gears[4].m3 - 80.0f, vec3(0, 1, 0));
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        if (!m_ready)
        {
            for (int i = 0; i < m_gears.Count(); i++)
                m_gears[i].m1.MeshConvert();
            m_ready = true;
        }

        Video::SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        for (int i = 0; i < m_gears.Count(); i++)
            m_gears[i].m1.Render(m_mat * m_gears[i].m2);
    }

private:
    Array<EasyMesh, mat4, float> m_gears;
    float m_angle;
    mat4 m_mat;
    Camera *m_camera;
    Light *m_light1, *m_light2;

    bool m_ready;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 5: EasyMesh", ivec2(960, 600), 60.0f);
    new EasyMeshTutorial();
    app.Run();

    return EXIT_SUCCESS;
}

