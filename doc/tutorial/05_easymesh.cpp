//
//  Lol Engine — EasyMesh tutorial
//
//  Copyright © 2011—2015 Sam Hocevar <sam@hocevar.net>
//            © 2012—2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
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

using namespace lol;

#define USE_CUSTOM_SHADER 1
#if USE_CUSTOM_SHADER
LOLFX_RESOURCE_DECLARE(shiny);
#endif //USE_CUSTOM_SHADER

class EasyMeshTutorial : public WorldEntity
{
public:
    EasyMeshTutorial()
    {
        EasyMeshLuaLoader EzMhLoader;
        EzMhLoader.ExecLuaFile("05_easymesh.lua");

        EasyMeshLuaObject* gears0 = EzMhLoader.GetPtr<EasyMeshLuaObject>("g0");
        EasyMeshLuaObject* gears1 = EzMhLoader.GetPtr<EasyMeshLuaObject>("g1");
        EasyMeshLuaObject* gears2 = EzMhLoader.GetPtr<EasyMeshLuaObject>("g2");
        EasyMeshLuaObject* gears3 = EzMhLoader.GetPtr<EasyMeshLuaObject>("g3");
        EasyMeshLuaObject* gears4 = EzMhLoader.GetPtr<EasyMeshLuaObject>("g4");

        m_gears.push(gears0->GetMesh(), mat4(1.0f), 0.0f);
        m_gears.push(gears1->GetMesh(), mat4(1.0f), 0.0f);
        m_gears.push(gears2->GetMesh(), mat4(1.0f), 180.0f / 18);
        m_gears.push(gears3->GetMesh(), mat4(1.0f), 180.0f / 18);
        m_gears.push(gears4->GetMesh(), mat4(1.0f), 180.0f / 18);

        /*
        m_gears[0].m1.Compile("[sc#00f ab 8 1 8 ty -.25]"
                              "[sc#f9f scb#f9f acg 12 10 5 5 20 20 5 5 0.1 0 s .1 .1 .1 ty -.1 csgu]"
                              "[sc#fff scb#000 acg 12 10 10 10 20 20 5 5 0.1 0 s .05 .05 .05 tx -1.5 ty .3 csgu]"
                              "[sc#00f ab 5 3 9 tx 2.5 csgs]"
                              "[[ sc#fff ab 3   1.4 2   tx -2 tz -2 "
                              "[sc#fff ab 2.1  .7 1.1 ty .5 tx -1.4 tz -1.4 csgs] mz] csgu]");
        m_gears[1].m1.Compile("sc#ff9 scb#ff9 acg 54 10 95 95 90 90 -5 -5 0.1 0 s .1 .1 .1");
        m_gears[2].m1.Compile("[sc#0f0 ab 2 2 2 t .8 .8 .8 rx 20 ry 20 [sc#00f ab 2 2 2 tx 0 csgu]]");
        m_gears[3].m1.Compile("[sc#0f0 ab 2 2 2 t .8 .8 .8 rx 20 ry 20 [sc#00f ab 2 2 2 tx 0 csgs]]");
        m_gears[4].m1.Compile("[sc#0f0 ab 2 2 2 t .8 .8 .8 rx 20 ry 20 [sc#00f ab 2 2 2 tx 0 csga]]");
        */

        m_angle = 0;

        m_camera = new Camera();
        m_camera->SetProjection(mat4::perspective(30.f, 960.f, 600.f, .1f, 1000.f));
        m_camera->SetView(mat4::lookat(vec3(-15.f, 5.f, 0.f),
                                       vec3(0.f, -1.f, 0.f),
                                       vec3(0.f, 1.f, 0.f)));
        Scene& scene = Scene::GetScene();
        scene.PushCamera(m_camera);

        /* Add a white directional light */
        m_light1 = new Light();
        m_light1->SetPosition(vec3(0.2f, 0.2f, 0.f));
        m_light1->SetColor(vec4(0.5f, 0.5f, 0.5f, 1.f));
        m_light1->SetType(LightType::Directional);
        Ticker::Ref(m_light1);

        /* Add an orangeish point light */
        m_light2 = new Light();
        m_light2->SetPosition(vec3(-15.f, 15.f, 15.f));
        m_light2->SetColor(vec4(0.4f, 0.3f, 0.2f, 1.f));
        m_light2->SetType(LightType::Point);
        Ticker::Ref(m_light2);

        m_ready = false;
    }

    ~EasyMeshTutorial()
    {
        Scene& scene = Scene::GetScene();
        scene.PopCamera(m_camera);
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

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        if (!m_ready)
        {
            Renderer::Get()->SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

            /* Upload vertex data to GPU */
            for (int i = 0; i < m_gears.count(); i++)
                m_gears[i].m1.MeshConvert();

#if USE_CUSTOM_SHADER
            /* Custom Shader: Init the shader */
            Shader *custom_shader = Shader::Create(LOLFX_RESOURCE_NAME(shiny));
            // any other shader stuf here (Get uniform, mostly, and set texture)

            for (int i = 0; i < m_gears.count(); i++)
                m_gears[i].m1.SetMaterial(custom_shader);
#endif

            m_ready = true;
        }

        for (int i = 0; i < m_gears.count(); i++)
        {
            m_gears[i].m1.Render(scene, m_mat * m_gears[i].m2);
        }
    }

private:
    Shader* m_custom_shader;
    array<EasyMesh, mat4, float> m_gears;
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

