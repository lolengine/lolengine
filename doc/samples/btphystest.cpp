//
//  Lol Engine — BtPhys tutorial
//
//  Copyright © 2012—2020 Sam Hocevar <sam@hocevar.net>
//            © 2009—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
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
#include <lol/color>
#include "loldebug.h"

using namespace lol;

#include "physics/lolphysics.h"
#include "physics/easyphysics.h"

#define CAT_MODE        0
#define OBJ_SIZE        2.f
#define NB_SPRITE       4
#define PARTICLE_SIZE   4
#include "physicobject.h"

#include "btphystest.h"

using namespace lol::phys;

#define CUBE_HALF_EXTENTS   .5f
#define EXTRA_HEIGHT        1.f
#define BASE_TIME           2.f
#define ZERO_TIME (BASE_TIME + rand(-BASE_TIME * .4f, BASE_TIME * .4f))
#define ZERO_SPEED          3.5f
#define JUMP_HEIGHT         30.f
#define JUMP_STRAFE         .5f
#define TARGET_TIMER        10.f + (rand(4.f) - 2.f)

int gNumObjects = 16;

#if CAT_MODE
#define USE_WALL        1
#define USE_BODIES      1
#else
#define USE_WALL        1
#define USE_PLATFORM    0
#define USE_ROPE        0
#define USE_BODIES      1
#define USE_ROTATION    1
#define USE_CHARACTER   0
#define USE_STAIRS      0
#endif

LOLFX_RESOURCE_DECLARE(front_camera_sprite);

//Damp for float
template <typename T1, typename T2, typename Tf>
static inline T1 damp(const T1 &a, const T2 &b, const Tf &x, const Tf &dt)
{
    if (dt <= .0f)
        return a;
    return lol::lerp(a, b, dt / (dt + x));
}

BtPhysTest::BtPhysTest(bool editor)
{
    m_init_status = 0;
}

void BtPhysTest::InitApp()
{
    m_init_status = 1;

    m_loop_value = .0f;

#if CAT_MODE
    /* cat datas setup */
    m_cat_texture = Tiler::Register("data/CatsSheet.png", ivec2::zero, ivec2(0,1));
    m_fov_dp = .0f;
    m_loc_dp = .0f;
#endif //CAT_MODE

    /* Create a camera that matches the settings of XNA BtPhysTest */
    m_camera = new Camera();

#if CAT_MODE
    m_camera->SetView(vec3(70.f, 50.f, 0.f),
                      vec3(0.f, 0.f, 0.f),
                      vec3(0, 1, 0));
    m_camera->SetProjection(radians(60.f), .1f, 1000.f, (float)video::size().x, (float)video::size().y / (float)video::size().x);
    m_target_timer = TARGET_TIMER;
    m_cam_target = -1;
#else
    m_camera->SetView(vec3(50.f, 50.f, 0.f),
                      vec3(0.f, 0.f, 0.f),
                      vec3(0, 1, 0));
    m_camera->SetProjection(radians(45.f), .1f, 1000.f, (float)video::size().x, (float)video::size().y / (float)video::size().x);
#endif
    Scene& scene = Scene::GetScene();
    scene.PushCamera(m_camera);

    m_ready = false;

    m_simulation = new Simulation();
    m_simulation->SetWorldLimit(vec3(-1000.0f, -1000.0f, -1000.0f), vec3(1000.0f, 1000.0f, 1000.0f));
    m_simulation->Init();
    vec3 NewGravity = vec3(.0f, -10.0f, .0f);
    m_simulation->SetGravity(NewGravity);
    m_simulation->SetContinuousDetection(true);
    m_simulation->SetTimestep(1.f / 120.f);
    Ticker::Ref(m_simulation);

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

    float offset = 29.5f;
    vec3 pos_offset = vec3(.0f, 30.f, .0f);
#if USE_STAIRS
    {
        vec3 new_offset = vec3(1.0f, .125f, .0f);
        quat NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
        vec3 NewPosition = pos_offset + vec3(5.0f, -29.f, 15.0f);
        {
            NewRotation = quat::fromeuler_xyz(0.f, 0.f, radians(30.f));
            NewPosition += vec3(4.0f, .0f, -4.0f);

            PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 3);
            Ticker::Ref(NewPhyobj);
            m_stairs_list.push_back(NewPhyobj);
        }
        {
            NewRotation = quat::fromeuler_xyz(0.f, 0.f, radians(40.f));
            NewPosition += vec3(4.0f, .0f, -4.0f);

            PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 3);
            Ticker::Ref(NewPhyobj);
            m_stairs_list.push_back(NewPhyobj);
        }
        NewPosition = pos_offset + vec3(5.0f, -29.5f, 15.0f);
        NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
        for (int i=0; i < 15; i++)
        {
            NewPosition += new_offset;

            PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 3);
            Ticker::Ref(NewPhyobj);
            m_stairs_list.push_back(NewPhyobj);
        }
    }
#endif //USE_STAIRS

#if USE_WALL
    {
        for (int i=0; i < 6; i++)
        {
            vec3 NewPosition = vec3(.0f);
            quat NewRotation = quat(1.f);

            PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation);

            int idx = i/2;
            NewPosition = pos_offset;
            NewPosition[idx] += offset;
            offset *= -1.f;

            if (idx != 1)
            {
                vec3 NewAxis = vec3(.0f);
                NewAxis[2 - idx] = 1;
                NewRotation = quat::rotate(radians(90.f), NewAxis);
            }

            NewPhyobj->SetTransform(NewPosition, NewRotation);
            Ticker::Ref(NewPhyobj);
            m_ground_list.push_back(NewPhyobj);
        }
    }
#endif //USE_WALL

    PhysicsObject* BasePhyobj = nullptr;
#if USE_PLATFORM
    {
        quat NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
        vec3 NewPosition = pos_offset + vec3(5.0f, -25.0f, -15.0f);

        PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

        m_platform_list.push_back(NewPhyobj);
        Ticker::Ref(NewPhyobj);

        NewPosition = pos_offset + vec3(-15.0f, -25.0f, 5.0f);

        NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);
        BasePhyobj = NewPhyobj;

        m_platform_list.push_back(NewPhyobj);
        Ticker::Ref(NewPhyobj);

        NewRotation = quat::fromeuler_xyz(0.f, 0.f, radians(90.f));
        NewPosition = pos_offset + vec3(-20.0f, -25.0f, 5.0f);

        NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

        NewPhyobj->GetPhysic()->AttachTo(BasePhyobj->GetPhysic(), true, true);
        m_platform_list.push_back(NewPhyobj);
        Ticker::Ref(NewPhyobj);

        //NewPosition += vec3(-0.0f, .0f, .0f);
        //NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

        //NewPhyobj->GetPhysic()->AttachTo(BasePhyobj->GetPhysic(), true, false);
        //m_platform_list.push_back(NewPhyobj);
        //Ticker::Ref(NewPhyobj);

        //NewPosition += vec3(-2.0f, .0f, .0f);
        //NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

        //NewPhyobj->GetPhysic()->AttachTo(BasePhyobj->GetPhysic(), false, false);
        //m_platform_list.push_back(NewPhyobj);
        //Ticker::Ref(NewPhyobj);
    }
#endif //USE_PLATFORM

#if USE_CHARACTER
    {
        quat NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
        vec3 NewPosition = pos_offset + vec3(-5.0f, -10.0f, 15.0f);

        PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 2);

        m_character_list.push_back(NewPhyobj);
        Ticker::Ref(NewPhyobj);

        //NewPhyobj->GetCharacter()->AttachTo(BasePhyobj->GetPhysic(), true, true);
    }
#endif //USE_CHARACTER

#if USE_BODIES
    {
        for (int x=0; x < 6; x++)
        {
            for (int y=0; y < 2; y++)
            {
                for (int z=0; z < 5; z++)
                {
                    PhysicsObject* new_physobj = new PhysicsObject(m_simulation, 1000.f,
                        vec3(-20.f, 15.f, -20.f) +
#if CAT_MODE
                        vec3(rand(4.f), rand(2.f), rand(4.f)) -
                        vec3(2.f       , 1.f      , 2.f) +
#endif //CAT_MODE
                        vec3(8.f * (float)x, 8.f * (float)y, 8.f * (float)z));
                    m_physobj_list.push_back(animated_object { new_physobj, ZERO_TIME });
                    Ticker::Ref(new_physobj);
                }
            }
        }
    }
#endif //USE_BODIES

#if USE_ROPE
    {
        std::vector<PhysicsObject*> RopeElements;
        for (int i = 0; i < 14; i++)
        {
            PhysicsObject* new_physobj = new PhysicsObject(m_simulation, 1000.f,
                vec3(0.f, 15.f, -20.f) +
                vec3(0.f, 0.f, 2.f * (float)i), 1);
            RopeElements.push_back(new_physobj);
            m_physobj_list.push_back(animated_object { new_physobj, ZERO_TIME });
            Ticker::Ref(new_physobj);
            if (RopeElements.size() > 1)
            {
                EasyConstraint* new_constraint = new EasyConstraint();

                vec3 A2B = .5f * (RopeElements[i]->GetPhysic()->GetTransform()[3].xyz -
                            RopeElements[i - 1]->GetPhysic()->GetTransform()[3].xyz);
                new_constraint->SetPhysObjA(RopeElements[i - 1]->GetPhysic(), lol::mat4::translate(A2B));
                new_constraint->SetPhysObjB(RopeElements[i]->GetPhysic(), lol::mat4::translate(-A2B));
                new_constraint->InitConstraintToPoint2Point();
                new_constraint->DisableCollisionBetweenObjs(true);
                new_constraint->AddToSimulation(m_simulation);
                m_constraint_list.push_back(new_constraint);
            }
        }
    }
#endif //USE_ROPE
}

BtPhysTest::~BtPhysTest()
{
    Scene& scene = Scene::GetScene();
    scene.PopCamera(m_camera);
    Ticker::Unref(m_light1);
    Ticker::Unref(m_light2);

#if CAT_MODE
    /* cat datas setup */
    Shader::Destroy(m_cat_shader);
    Tiler::Deregister(m_cat_texture);
#endif //CAT_MODE

    while (m_constraint_list.size())
    {
        EasyConstraint* CurPop = m_constraint_list.back();
        m_constraint_list.pop_back();
        CurPop->RemoveFromSimulation(m_simulation);
        delete CurPop;
    }
    std::vector<PhysicsObject*> objects;
    for (auto *obj : m_ground_list)
        objects.push_back(obj);
    for (auto *obj : m_stairs_list)
        objects.push_back(obj);
    for (auto *obj : m_character_list)
        objects.push_back(obj);
    for (auto *obj : m_platform_list)
        objects.push_back(obj);
    // Back insertion; is this really necessary?
    while (m_physobj_list.size())
    {
        objects.push_back(m_physobj_list.back().obj);
        m_physobj_list.pop_back();
    }
    m_ground_list.clear();
    m_stairs_list.clear();
    m_character_list.clear();
    m_platform_list.clear();

    while (objects.size())
    {
        PhysicsObject* CurPop = objects.back();
        objects.pop_back();
        CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
        Ticker::Unref(CurPop);
    }

    //while (m_ground_list.size())
    //{
    //    PhysicsObject* CurPop = m_ground_list.back();
    //    m_ground_list.pop_back();
    //    CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
    //    Ticker::Unref(CurPop);
    //}
    //while (m_stairs_list.size())
    //{
    //    PhysicsObject* CurPop = m_stairs_list.back();
    //    m_stairs_list.pop_back();
    //    CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
    //    Ticker::Unref(CurPop);
    //}
    //while (m_character_list.size())
    //{
    //    PhysicsObject* CurPop = m_character_list.back();
    //    m_character_list.pop_back();
    //    CurPop->GetCharacter()->RemoveFromSimulation(m_simulation);
    //    Ticker::Unref(CurPop);
    //}
    //while (m_platform_list.size())
    //{
    //    PhysicsObject* CurPop = m_platform_list.back();
    //    m_platform_list.pop_back();
    //    CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
    //    Ticker::Unref(CurPop);
    //}
    //while (m_physobj_list.size())
    //{
    //    PhysicsObject* CurPop = m_physobj_list.back().obj;
    //    m_physobj_list.pop_back();
    //    CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
    //    Ticker::Unref(CurPop);
    //}
    Ticker::Unref(m_simulation);

}

void BtPhysTest::tick_game(float seconds)
{
    WorldEntity::tick_game(seconds);

    if (!m_init_status)
    {
        if (Scene::GetCount())
            InitApp();
        return;
    }
    else if (m_init_status == 1)
    {
        m_init_status++;
        return;
    }

    auto context = Debug::DrawContext::New(color::white, 1.f);
    Debug::DrawGrid(vec3::zero, vec3::axis_x, vec3::axis_y, vec3::axis_z, 10.f);

    if (input::keyboard()->key_released(input::key::SC_Escape))
        Ticker::Shutdown();

    m_loop_value += seconds;
    if (m_loop_value > F_PI * 2.0f)
        m_loop_value -= F_PI * 2.0f;

    vec3 GroundBarycenter = vec3(.0f);
    vec3 PhysObjBarycenter = vec3(.0f);
    float factor = .0f;
#if CAT_MODE
#if USE_BODIES
    vec3 cam_center(0.f);
    float cam_factor = .0f;
    vec2 screen_min_max[2] = { vec2(FLT_MAX), vec2(-FLT_MAX) };
    Scene& scene = Scene::GetScene();
    mat4 world_cam = scene.GetCamera()->GetView();
    mat4 cam_screen = scene.GetCamera()->GetProjection();

    m_target_timer -= seconds;
    if (m_target_timer < .0f)
    {
        m_target_timer = TARGET_TIMER;
        if (m_cam_target == -1)
            m_cam_target = rand(m_physobj_list.size());
        else
            m_cam_target = -1;
    }

    for (size_t i = 0; i < m_physobj_list.size(); i++)
    {
        PhysicsObject* PhysObj = m_physobj_list[i].obj;
        float &obj_timer = m_physobj_list[i].anim;

        vec3 obj_loc = PhysObj->GetPhysic()->GetTransform()[3].xyz;

        if (m_cam_target == -1 || m_cam_target == i)
        {
            cam_center += obj_loc;
            cam_factor += 1.f;

            mat4 LocalPos = mat4::translate(obj_loc);
            vec3 vpos;

            LocalPos = world_cam * LocalPos;
            mat4 LocalPos0 = LocalPos;

            int j = 2;
            while (j-- > 0)
            {
                if (j == 1)
                    LocalPos = mat4::translate(vec3(-4.f)) * LocalPos0;
                else
                    LocalPos = mat4::translate(vec3(4.f)) * LocalPos0;

                LocalPos = cam_screen * LocalPos;
                vpos = (LocalPos[3] / LocalPos[3].w).xyz;
                screen_min_max[0] = min(vpos.xy, screen_min_max[0]);
                screen_min_max[1] = max(vpos.xy, screen_min_max[1]);
            }
        }

        //Jump handling
        //if (length(PhysObj->GetPhysic()->GetLinearVelocity()) < ZERO_SPEED)
        if (lol::fabs(PhysObj->GetPhysic()->GetLinearVelocity().y) < ZERO_SPEED)
            obj_timer -= seconds;

        if (obj_timer < .0f)
        {
            PhysObj->GetPhysic()->AddImpulse(JUMP_HEIGHT *
                                             vec3(JUMP_STRAFE, 1.f, JUMP_STRAFE) *
                                             vec3(rand(-1.f, 1.f), 1.0f, rand(-1.f, 1.f)) *
                                             PhysObj->GetPhysic()->GetMass());
            obj_timer = ZERO_TIME;
        }
    }

    float fov_ratio = max(max(lol::fabs(screen_min_max[0].x), lol::fabs(screen_min_max[0].y)),
                          max(lol::fabs(screen_min_max[1].x), lol::fabs(screen_min_max[1].y)));

    vec3 new_target = cam_center / cam_factor;
    float fov_dp = .0f;
    float loc_dp = .0f;

    //ideally fov is on the target
    if (lol::fabs(fov_ratio - 1.f) < .2f)
        fov_dp = ((m_cam_target == -1)?(.7f):(.2f));
    else
        fov_dp = ((m_cam_target == -1)?(1.7f):(.9f));

    //ideally loc is on the target
    if (length(new_target - m_camera->GetTarget()) < 6.f)
        loc_dp = ((m_cam_target == -1)?(.5f):(.03f));
    else
        loc_dp = ((m_cam_target == -1)?(.9f):(.5f));

    m_fov_dp = damp(m_fov_dp, fov_dp, 0.08f, seconds);
    m_loc_dp = damp(m_loc_dp, loc_dp, 0.08f, seconds);
    m_camera->SetFov(damp(m_camera->GetFov(), m_camera->GetFov() * fov_ratio * 1.1f, m_fov_dp, seconds));
    vec3 tmp = damp(m_camera->GetTarget(), new_target, m_loc_dp, seconds);
    m_camera->SetView((mat4::rotate(radians(10.f) * seconds, vec3(.0f, 1.f, .0f)) * vec4(m_camera->GetPosition(), 1.0f)).xyz,
                      tmp, vec3(0, 1, 0));
#endif //USE_BODIES
#endif //CAT_MODE

#if USE_WALL
    {
        for (size_t i = 0; i < m_ground_list.size(); i++)
        {
            PhysicsObject* PhysObj = m_ground_list[i];
            mat4 GroundMat = PhysObj->GetTransform();

            GroundBarycenter += GroundMat[3].xyz;
            factor += 1.f;
        }

        GroundBarycenter /= factor;

        for (size_t i = 0; i < m_ground_list.size(); i++)
        {
            PhysicsObject* PhysObj = m_ground_list[i];

            mat4 GroundMat = PhysObj->GetTransform();
            vec3 CenterToGround = GroundMat[3].xyz - GroundBarycenter;
            vec3 CenterToCam = m_camera->GetPosition() - GroundBarycenter;

            if (dot(normalize(CenterToCam - CenterToGround),
                    normalize(CenterToGround)) > 0.f)
                PhysObj->SetRender(false);
            else
                PhysObj->SetRender(true);
        }
    }
#endif //USE_WALL

#if USE_ROTATION
    {
        for (size_t i = 0; i < m_ground_list.size(); i++)
        {
            PhysicsObject* PhysObj = m_ground_list[i];

            mat4 GroundMat = PhysObj->GetTransform();
            mat4 CenterMx = mat4::translate(GroundBarycenter);
            GroundMat = inverse(CenterMx) * GroundMat;
            GroundMat = CenterMx *
                        mat4(quat::fromeuler_xyz(vec3(.0f, radians(20.f), radians(20.0f)) * seconds))
                        * GroundMat;
            PhysObj->SetTransform(GroundMat[3].xyz, quat(mat3(GroundMat)));
        }
    }
#endif //USE_ROTATION

#if USE_PLATFORM
    {
        for (size_t i = 0; i < m_platform_list.size(); i++)
        {
            PhysicsObject* PhysObj = m_platform_list[i];

            mat4 GroundMat = PhysObj->GetTransform();
            if (i == 0)
            {
                GroundMat = GroundMat * mat4(quat::fromeuler_xyz(vec3(radians(20.f), .0f, .0f) * seconds));
                PhysObj->SetTransform(GroundMat[3].xyz, quat(mat3(GroundMat)));
            }
            else if (i == 1)
            {
                GroundMat =
                    mat4::translate(vec3(-15.0f, 5.0f, lol::cos(m_loop_value) * 8.f)) *
                    mat4(quat::fromeuler_xyz(vec3(.0f, lol::cos(m_loop_value) * radians(20.f), .0f)));
                PhysObj->SetTransform(GroundMat[3].xyz, quat(mat3(GroundMat)));
            }
        }
    }
#endif //USE_PLATFORM

#if USE_CHARACTER
    {
        auto keyboard = input::keyboard();

        for (size_t i = 0; i < m_character_list.size(); i++)
        {
            PhysicsObject* PhysObj = m_character_list[i];
            EasyCharacterController* Character = (EasyCharacterController*)PhysObj->GetCharacter();
            mat4 CtlrMx = Character->GetTransform();

            vec3 movement(0.f);
            movement.z = (keyboard->key(input::key::SC_Right) ? 1.f : 0.f)
                - (keyboard->key(input::key::SC_Left) ? 1.f : 0.f);
            movement.x = (keyboard->key(input::key::SC_Up) ? 1.f : 0.f)
                - (keyboard->key(input::key::SC_Down) ? 1.f : 0.f);
            movement.y = (keyboard->key(input::key::SC_PageUp) ? 1.f : 0.f)
                - (keyboard->key(input::key::SC_PageDown) ? 1.f : 0.f);
            vec3 CharMove = movement * seconds * vec3(4.f, 10.f, 4.f);

            if (input::keyboard()->key_released(input::key::SC_Space))
                Character->Jump();
            Character->SetMovementForFrame(CharMove);

            RayCastResult HitResult;
            if (m_simulation->RayHits(HitResult, ERT_Closest, Character->GetTransform()[3].xyz, (Character->GetTransform()[3].xyz + vec3(.0f, -1.f, .0f)), Character))
                Character->AttachTo(HitResult.m_collider_list[0], true, true);
            else
                Character->AttachTo(nullptr);
        }
    }
#endif //USE_CHARACTER

#if USE_CHARACTER
    {
        PhysObjBarycenter = vec3(.0f);
        factor = .0f;

        for (size_t i = 0; i < m_character_list.size(); i++)
        {
            PhysicsObject* PhysObj = m_character_list[i];
            mat4 GroundMat = PhysObj->GetTransform();

            PhysObjBarycenter += GroundMat[3].xyz;
            factor += 1.f;
        }

        PhysObjBarycenter /= factor;

#if 0
        m_camera->SetTarget(m_camera->GetTarget() + (seconds / (seconds + 0.18f)) * (PhysObjBarycenter - m_camera->GetTarget()));
        vec3 CamPosCenter = m_camera->GetTarget() + vec3(.0f, 5.0f, .0f);
        m_camera->SetPosition(CamPosCenter + normalize(m_camera->GetPosition() - CamPosCenter) * 20.0f);
#endif
    }
#else
    {
        PhysObjBarycenter = vec3(.0f);
        for (size_t i = 0; i < m_physobj_list.size(); i++)
        {
            PhysicsObject* PhysObj = m_physobj_list[i].obj;
            mat4 GroundMat = PhysObj->GetTransform();

            PhysObjBarycenter += GroundMat[3].xyz;
            factor += 1.f;
        }

        PhysObjBarycenter /= factor;

#if 0
        m_camera->SetTarget(PhysObjBarycenter);
        m_camera->SetPosition(GroundBarycenter + normalize(GroundBarycenter - PhysObjBarycenter) * 60.0f);
#endif
    }
#endif //USE_CHARACTER
}

void BtPhysTest::tick_draw(float seconds, Scene &scene)
{
    WorldEntity::tick_draw(seconds, scene);

    if (m_init_status != 2)
        return;

    if (!m_ready)
    {
#if CAT_MODE
        /* cat datas setup */
        m_cat_shader = Shader::Create(LOLFX_RESOURCE_NAME(front_camera_sprite));
#if USE_BODIES
        for (size_t i = 0; i < m_physobj_list.size(); i++)
        {
            PhysicsObject* PhysObj = m_physobj_list[i].obj;
            m_cat_sdata = new CatShaderData(((1 << VertexUsage::Position) |
                                                (1 << VertexUsage::Color) |
                                                (1 << VertexUsage::TexCoord) |
                                                (1 << VertexUsage::TexCoordExt)),
                                                m_cat_shader);
            m_cat_sdata->m_tex_uniform = m_cat_texture->GetTexture()->GetTextureUniform();
            m_cat_sdata->m_sprite_flip = ((rand(2) == 1)?(1.f):(0.f)) / (float)(NB_SPRITE * PARTICLE_SIZE);
            PhysObj->SetCustomShaderData(m_cat_sdata);
            m_cat_sdata = nullptr;
        }
#endif //USE_BODIES
#endif //CAT_MODE

        /* FIXME: this object never cleans up */
        m_ready = true;
    }
    else
    {
#if CAT_MODE
        for (size_t i = 0; i < m_physobj_list.size(); i++)
        {
            PhysicsObject* PhysObj = m_physobj_list[i].obj;
            CatShaderData* ShaderData = (CatShaderData*)PhysObj->GetCustomShaderData();

            ShaderData->m_sprite_orientation = damp(ShaderData->m_sprite_orientation,
                                                    F_PI_4 * ((ShaderData->m_sprite_flip * 2.f * (float)(NB_SPRITE * PARTICLE_SIZE)) - 1.f) *
                                                    clamp(PhysObj->GetPhysic()->GetLinearVelocity().y / 20.0f, -1.f, 1.f),
                                                    0.1f, seconds);
        }
#endif //CAT_MODE
    }

    //Video::set_clear_color(vec4(0.0f, 0.0f, 0.12f, 1.0f));

}

//-----------------------------------------------------------------------------
// CShaderData
//-----------------------------------------------------------------------------
CatShaderData::CatShaderData(uint32_t vert_decl_flags, std::shared_ptr<Shader> shader)
    : GpuShaderData(vert_decl_flags, shader, DebugRenderMode::Default)
{
    m_sprite_orientation = .0f;
    m_sprite_flip = .0f;
    SetupDefaultData();
}

//-----------------------------------------------------------------------------
void CatShaderData::SetupDefaultData()
{
    AddUniform("u_model_view");
    AddUniform("u_normal_mat");
    AddUniform("u_proj");
    AddUniform("u_texture");
    AddUniform("u_sprite_orientation");
    AddUniform("u_sprite_flip");
}

//-----------------------------------------------------------------------------
void CatShaderData::SetupShaderDatas(mat4 const &model)
{
    Scene& scene = Scene::GetScene();
    mat4 proj = scene.GetCamera()->GetProjection();
    mat4 view = scene.GetCamera()->GetView();

    mat4 modelview = view * model;
    mat3 normalmat = transpose(inverse(mat3(view)));

    m_shader->SetUniform(*GetUniform("u_model_view"), modelview);
    m_shader->SetUniform(*GetUniform("u_normal_mat"), normalmat);
    m_shader->SetUniform(*GetUniform("u_proj"), proj);
    m_shader->SetUniform(*GetUniform("u_texture"), m_tex_uniform, 0);
    m_shader->SetUniform(*GetUniform("u_sprite_orientation"), m_sprite_orientation);
    m_shader->SetUniform(*GetUniform("u_sprite_flip"), m_sprite_flip);
}

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    auto app = app::init("BtPhysTest", ivec2(1280, 960), 60.0f);

    new BtPhysTest(argc > 1);
    app->show_pointer(false);
    app->run();

    return EXIT_SUCCESS;
}

