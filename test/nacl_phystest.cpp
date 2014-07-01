//
// BtPhysTest
//
// Copyright: (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2012-2013 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/main.h>
#include "loldebug.h"

using namespace lol;

//#include "physics/lolphysics.h"
//#include "physics/easyphysics.h"

#include "nacl_phystest.h"

//using namespace lol::phys;

int gNumObjects = 64;

Nacl_PhysTest::Nacl_PhysTest(bool editor)
{
    /* Register an input controller for the keyboard */
    m_controller = new Controller("Default", KEY_MAX, 0);
    m_controller->GetKey(KEY_MOVE_FORWARD).Bind("Keyboard", "Up");
    m_controller->GetKey(KEY_MOVE_BACK).Bind("Keyboard", "Down");
    m_controller->GetKey(KEY_MOVE_LEFT).Bind("Keyboard", "Left");
    m_controller->GetKey(KEY_MOVE_RIGHT).Bind("Keyboard", "Right");
    m_controller->GetKey(KEY_MOVE_JUMP).Bind("Keyboard", "Space");
    m_controller->GetKey(KEY_MOVE_UP).Bind("Keyboard", "PageUp");
    m_controller->GetKey(KEY_MOVE_DOWN).Bind("Keyboard", "PageDown");
    m_controller->GetKey(KEY_QUIT).Bind("Keyboard", "Escape");

    /* Create a camera that matches the settings of XNA BtPhysTest */
    m_camera = new Camera();
    m_camera->SetView(vec3(50.f, 50.f, 0.f),
                      vec3(0.f, 0.f, 0.f),
                      vec3(0, 1, 0));
    m_camera->SetProjection(45.f, .1f, 1000.f, (float)Video::GetSize().x, (float)Video::GetSize().y / (float)Video::GetSize().x);
    g_scene->PushCamera(m_camera);

    m_ready = false;

    /*
    m_simulation = new Simulation();
    m_simulation->SetWorldLimit(vec3(-1000.0f, -1000.0f, -1000.0f), vec3(1000.0f, 1000.0f, 1000.0f));
    m_simulation->Init();
    vec3 NewGravity = vec3(.0f, -10.0f, .0f);
    m_simulation->SetGravity(NewGravity);
    m_simulation->SetContinuousDetection(true);
    m_simulation->SetTimestep(1.f / 120.f);
    Ticker::Ref(m_simulation);
    */

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
}

void Nacl_PhysTest::TickGame(float seconds)
{
    WorldEntity::TickGame(seconds);

    if (m_controller->GetKey(KEY_QUIT).IsReleased())
        Ticker::Shutdown();
}

void Nacl_PhysTest::TickDraw(float seconds, Scene &scene)
{
    WorldEntity::TickDraw(seconds, scene);

    if (!m_ready)
    {
        /* FIXME: this object never cleans up */
        m_ready = true;
    }
    else
    {
    }
}

Nacl_PhysTest::~Nacl_PhysTest()
{
    g_scene->PopCamera(m_camera);
    Ticker::Unref(m_light1);
    Ticker::Unref(m_light2);

    //Ticker::Unref(m_simulation);
}

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Nacl_PhysTest", ivec2(1280, 960), 60.0f);

    new Nacl_PhysTest(argc > 1);
    app.ShowPointer(false);

    app.Run();

    return EXIT_SUCCESS;
}

