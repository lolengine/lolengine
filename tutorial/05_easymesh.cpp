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
    EasyMeshTutorial()
    {
        m_gears.Push(EasyMesh(), mat4(1.0f), 0.0f);
        m_gears.Push(EasyMesh(), mat4(1.0f), 0.0f);
        m_gears.Push(EasyMesh(), mat4(1.0f), 180.0f / 18);
        m_gears.Push(EasyMesh(), mat4(1.0f), 180.0f / 18);
        m_gears.Push(EasyMesh(), mat4(1.0f), 180.0f / 18);

        m_gears[0].m1.Compile("sc#f9f scb#f9f acg 12 10 5 5 20 20 5 5 0.1 0 s .1 .1 .1");
        m_gears[1].m1.Compile("sc#ff9 scb#ff9 acg 54 10 95 95 90 90 -5 -5 0.1 0 s .1 .1 .1");
        m_gears[2].m1.Compile("sc#9ff scb#9ff acg 18 10 5 5 30 30 5 5 0.1 0 s .1 .1 .1");
        m_gears[3].m1.Compile("sc#9ff scb#9ff acg 18 10 5 5 30 30 5 5 0.1 0 s .1 .1 .1");
        m_gears[4].m1.Compile("sc#9ff scb#9ff acg 18 10 5 5 30 30 5 5 0.1 0 s .1 .1 .1");

        m_angle = 0;

        m_camera = new Camera(vec3(0.f, 600.f, 0.f),
                              vec3(0.f, 0.f, 0.f),
                              vec3(0, 1, 0));
        m_camera->SetPerspective(70.f, 960.f, 600.f, .1f, 1000.f);
        m_camera->SetTarget(vec3(0.f, -1.f, 0.f));
        m_camera->SetPosition(vec3(-15.f, 10.f, 0.f));
        Ticker::Ref(m_camera);

        m_ready = false;
    }

    ~EasyMeshTutorial()
    {
        Ticker::Unref(m_camera);
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        m_angle += seconds * 70.0f;
        m_mat = mat4::rotate(10.0f, vec3(0, 0, 1))
              * mat4::rotate(m_angle, vec3(0, 1, 0));

        m_gears[0].m3 += seconds * 150.0f;
        m_gears[1].m3 += seconds * 150.0f * -2 / 9;
        m_gears[2].m3 += seconds * 150.0f * -2 / 3;
        m_gears[3].m3 += seconds * 150.0f * -2 / 3;
        m_gears[4].m3 += seconds * 150.0f * -2 / 3;

        m_gears[0].m2 = mat4::translate(vec3(0, 0, 0))
                      * mat4::rotate(m_gears[0].m3, vec3(0, 1, 0));
        m_gears[1].m2 = mat4::translate(vec3(0, 0, 0))
                      * mat4::rotate(m_gears[1].m3, vec3(0, 1, 0));
        m_gears[2].m2 = mat4::translate(vec3(0, 0, 5.5f))
                      * mat4::rotate(m_gears[2].m3, vec3(0, 1, 0));
        m_gears[3].m2 = mat4::translate(vec3(5.5f * lol::sqrt(3.f) * 0.5f, 0, -5.5f * 0.5f))
                      * mat4::rotate(m_gears[3].m3, vec3(0, 1, 0));
        m_gears[4].m2 = mat4::translate(vec3(-5.5f * lol::sqrt(3.f) * 0.5f, 0, -5.5f * 0.5f))
                      * mat4::rotate(m_gears[4].m3, vec3(0, 1, 0));
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

    bool m_ready;
};

int main(int argc, char **argv)
{
    Application app("Tutorial 5: EasyMesh", ivec2(960, 600), 60.0f);
    new EasyMeshTutorial();
    app.Run();

    return EXIT_SUCCESS;
}

