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
        m_angle = 0;
        m_mesh.Compile("sc#e94 scb#964 [acap11 6 6 tx8]");
        m_mesh.Compile("sc#94e scb#649 [acap3 6 6 tx-5 tz-6]");
        m_mesh.Compile("sc#49e scb#469 [acap31 6 6 tx-5 tz6]");

        m_camera = new Camera(vec3(0.f, 600.f, 0.f),
                              vec3(0.f, 0.f, 0.f),
                              vec3(0, 1, 0));
        m_camera->SetPerspective(70.f, 640.f, 480.f, .1f, 1000.f);
        m_camera->SetTarget(vec3(0.f));
        m_camera->SetPosition(vec3(-20.f, 20.f, 0.f));
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
    Application app("Tutorial 5: EasyMesh", ivec2(640, 480), 60.0f);
    new EasyMeshTutorial();
    app.Run();

    return EXIT_SUCCESS;
}

