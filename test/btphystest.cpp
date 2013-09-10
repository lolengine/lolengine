//
// BtPhysTest
//
// Copyright: (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2012-2013 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "loldebug.h"

using namespace lol;

#ifndef HAVE_PHYS_USE_BULLET
#define HAVE_PHYS_USE_BULLET
#endif /* HAVE_PHYS_USE_BULLET */

#include "physics/lolphysics.h"
#include "physics/easyphysics.h"

#define CAT_MODE        1
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

int gNumObjects = 64;

#if CAT_MODE
#define USE_WALL        1
#define USE_BODIES      1
#else
#define USE_WALL        1
#define USE_PLATFORM    1
#define USE_ROPE        0
#define USE_BODIES      1
#define USE_ROTATION    0
#define USE_CHARACTER   0
#define USE_STAIRS      0
#endif

LOLFX_RESOURCE_DECLARE(front_camera_sprite);

BtPhysTest::BtPhysTest(bool editor)
{
    m_loop_value = .0f;

#if CAT_MODE
    /* cat datas setup */
    m_cat_texture = Tiler::Register("data/CatsSheet.png", ivec2(0), ivec2(0,1));
#endif //CAT_MODE

    /* Register an input controller for the keyboard */
    m_controller = new Controller(KEY_MAX, 0);
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

#if CAT_MODE
    m_camera->SetView(vec3(70.f, 50.f, 0.f),
                      vec3(0.f, 0.f, 0.f),
                      vec3(0, 1, 0));
    m_camera->SetProjection(mat4::perspective(60.f, (float)Video::GetSize().x, (float)Video::GetSize().y, .1f, 1000.f));
#else
    m_camera->SetView(vec3(50.f, 50.f, 0.f),
                      vec3(0.f, 0.f, 0.f),
                      vec3(0, 1, 0));
    m_camera->SetProjection(mat4::perspective(45.f, (float)Video::GetSize().x, (float)Video::GetSize().y, .1f, 1000.f));
#endif
    g_scene->PushCamera(m_camera);

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
    m_light1->SetPosition(vec4(0.2f, 0.2f, 0.f, 0.f));
    m_light1->SetColor(vec4(0.5f, 0.5f, 0.5f, 1.f));
    Ticker::Ref(m_light1);

    /* Add an orangeish point light */
    m_light2 = new Light();
    m_light2->SetPosition(vec4(-15.f, 15.f, 15.f, 1.f));
    m_light2->SetColor(vec4(0.4f, 0.3f, 0.2f, 1.f));
    Ticker::Ref(m_light2);

    float offset = 29.5f;
    vec3 pos_offset = vec3(.0f, 30.f, .0f);
#if USE_STAIRS
    {
        vec3 new_offset = vec3(1.0f, .125f, .0f);
        quat NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
        vec3 NewPosition = pos_offset + vec3(5.0f, -29.f, 15.0f);
        {
            NewRotation = quat::fromeuler_xyz(0.f, 0.f, 30.f);
            NewPosition += vec3(4.0f, .0f, -4.0f);

            PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 3);
            Ticker::Ref(NewPhyobj);
            m_stairs_list << NewPhyobj;
        }
        {
            NewRotation = quat::fromeuler_xyz(0.f, 0.f, 40.f);
            NewPosition += vec3(4.0f, .0f, -4.0f);

            PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 3);
            Ticker::Ref(NewPhyobj);
            m_stairs_list << NewPhyobj;
        }
        NewPosition = pos_offset + vec3(5.0f, -29.5f, 15.0f);
        NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
        for (int i=0; i < 15; i++)
        {
            NewPosition += new_offset;

            PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 3);
            Ticker::Ref(NewPhyobj);
            m_stairs_list << NewPhyobj;
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
                NewRotation = quat::rotate(90.f, NewAxis);
            }

            NewPhyobj->SetTransform(NewPosition, NewRotation);
            Ticker::Ref(NewPhyobj);
            m_ground_list << NewPhyobj;
        }
    }
#endif //USE_WALL

    PhysicsObject* BasePhyobj = NULL;
#if USE_PLATFORM
    {
        quat NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
        vec3 NewPosition = pos_offset + vec3(5.0f, -25.0f, -15.0f);

        PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

        m_platform_list << NewPhyobj;
        Ticker::Ref(NewPhyobj);

        NewPosition = pos_offset + vec3(-15.0f, -25.0f, 5.0f);

        NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);
        BasePhyobj = NewPhyobj;

        m_platform_list << NewPhyobj;
        Ticker::Ref(NewPhyobj);

        NewRotation = quat::fromeuler_xyz(0.f, 0.f, 90.f);
        NewPosition = pos_offset + vec3(-20.0f, -25.0f, 5.0f);

        NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

        NewPhyobj->GetPhysic()->AttachTo(BasePhyobj->GetPhysic(), true, true);
        m_platform_list << NewPhyobj;
        Ticker::Ref(NewPhyobj);

        //NewPosition += vec3(-0.0f, .0f, .0f);
        //NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

        //NewPhyobj->GetPhysic()->AttachTo(BasePhyobj->GetPhysic(), true, false);
        //m_platform_list << NewPhyobj;
        //Ticker::Ref(NewPhyobj);

        //NewPosition += vec3(-2.0f, .0f, .0f);
        //NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

        //NewPhyobj->GetPhysic()->AttachTo(BasePhyobj->GetPhysic(), false, false);
        //m_platform_list << NewPhyobj;
        //Ticker::Ref(NewPhyobj);
    }
#endif //USE_PLATFORM

#if USE_CHARACTER
    {
        quat NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
        vec3 NewPosition = pos_offset + vec3(-5.0f, -10.0f, 15.0f);

        PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 2);

        m_character_list << NewPhyobj;
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
                        vec3(8.f * (float)x, 8.f * (float)y, 8.f * (float)z));
                    m_physobj_list.Push(new_physobj, ZERO_TIME);
                    Ticker::Ref(new_physobj);
                }
            }
        }
    }
#endif //USE_BODIES

#if USE_ROPE
    {
        Array<PhysicsObject*> RopeElements;
        for (int i = 0; i < 14; i++)
        {
            PhysicsObject* new_physobj = new PhysicsObject(m_simulation, 1000.f,
                vec3(0.f, 15.f, -20.f) +
                vec3(0.f, 0.f, 2.f * (float)i), 1);
            RopeElements << new_physobj;
            m_physobj_list.Push(new_physobj, ZERO_TIME);
            Ticker::Ref(new_physobj);
            if (RopeElements.Count() > 1)
            {
                EasyConstraint* new_constraint = new EasyConstraint();

                vec3 A2B = .5f * (RopeElements[i]->GetPhysic()->GetTransform().v3.xyz -
                            RopeElements[i - 1]->GetPhysic()->GetTransform().v3.xyz);
                new_constraint->SetPhysObjA(RopeElements[i - 1]->GetPhysic(), lol::mat4::translate(A2B));
                new_constraint->SetPhysObjB(RopeElements[i]->GetPhysic(), lol::mat4::translate(-A2B));
                new_constraint->InitConstraintToPoint2Point();
                new_constraint->DisableCollisionBetweenObjs(true);
                new_constraint->AddToSimulation(m_simulation);
                m_constraint_list << new_constraint;
            }
        }
    }
#endif //USE_ROPE
}

void BtPhysTest::TickGame(float seconds)
{
    WorldEntity::TickGame(seconds);

    if (m_controller->GetKey(KEY_QUIT).IsReleased())
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
    vec3 cam_min_max[2] = { vec3(FLT_MAX), vec3(-FLT_MAX) };
    mat4 world_cam = g_scene->GetCamera()->GetView();
    mat4 cam_screen = g_scene->GetCamera()->GetProjection();

    for (int i = 0; i < m_physobj_list.Count(); i++)
    {
        PhysicsObject* PhysObj = m_physobj_list[i].m1;
        float &Timer = m_physobj_list[i].m2;

        vec3 obj_loc = PhysObj->GetPhysic()->GetTransform().v3.xyz;

        cam_center += obj_loc;
        cam_factor += 1.f;

        mat4 LocalPos = mat4::translate(obj_loc);
        vec3 vpos;

        LocalPos = world_cam * LocalPos;
        vpos = LocalPos.v3.xyz;
        cam_min_max[0] = min(vpos.xyz, cam_min_max[0]);
        cam_min_max[1] = max(vpos.xyz, cam_min_max[1]);

        LocalPos = cam_screen * LocalPos;
        vpos = (LocalPos.v3 / LocalPos.v3.w).xyz;
        screen_min_max[0] = min(vpos.xy, screen_min_max[0]);
        screen_min_max[1] = max(vpos.xy, screen_min_max[1]);

        //if (length(PhysObj->GetPhysic()->GetLinearVelocity()) < ZERO_SPEED)
        if (lol::abs(PhysObj->GetPhysic()->GetLinearVelocity().y) < ZERO_SPEED)
            Timer -= seconds;

        if (Timer < .0f)
        {
            PhysObj->GetPhysic()->AddImpulse(JUMP_HEIGHT *
                                             vec3(JUMP_STRAFE, 1.f, JUMP_STRAFE) *
                                             vec3(rand(-1.f, 1.f), 1.0f, rand(-1.f, 1.f)) *
                                             PhysObj->GetPhysic()->GetMass());
            Timer = ZERO_TIME;
        }
    }

    vec3 min_max_diff = (cam_min_max[1] - cam_min_max[0]);
    float screen_size = max(max(lol::abs(min_max_diff.x), lol::abs(min_max_diff.y)),
                        max(    lol::abs(min_max_diff.x), lol::abs(min_max_diff.y)));
    float fov_ratio = max(max(lol::abs(screen_min_max[0].x), lol::abs(screen_min_max[0].y)),
                          max(lol::abs(screen_min_max[1].x), lol::abs(screen_min_max[1].y)));

    //m_camera->SetProjection(mat4::perspective(30.f * fov_ratio * 1.1f, 1280.f, 960.f, .1f, 1000.f));
    m_camera->SetView((mat4::rotate(10.f * seconds, vec3(.0f, 1.f, .0f)) * vec4(m_camera->GetPosition(), 1.0f)).xyz,
                      //vec3(70.f, 30.f, 0.f),
                      cam_center / cam_factor, vec3(0, 1, 0));
#endif //USE_BODIES
#endif //CAT_MODE

#if USE_WALL
    {
        for (int i = 0; i < m_ground_list.Count(); i++)
        {
            PhysicsObject* PhysObj = m_ground_list[i];
            mat4 GroundMat = PhysObj->GetTransform();

            GroundBarycenter += GroundMat.v3.xyz;
            factor += 1.f;
        }

        GroundBarycenter /= factor;

        for (int i = 0; i < m_ground_list.Count(); i++)
        {
            PhysicsObject* PhysObj = m_ground_list[i];

            mat4 GroundMat = PhysObj->GetTransform();
            vec3 CenterToGround = GroundMat.v3.xyz - GroundBarycenter;
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
        for (int i = 0; i < m_ground_list.Count(); i++)
        {
            PhysicsObject* PhysObj = m_ground_list[i];

            mat4 GroundMat = PhysObj->GetTransform();
            mat4 CenterMx = mat4::translate(GroundBarycenter);
            GroundMat = inverse(CenterMx) * GroundMat;
            GroundMat = CenterMx *
                        mat4(quat::fromeuler_xyz(vec3(.0f, 20.f, 20.0f) * seconds))
                        * GroundMat;
            PhysObj->SetTransform(GroundMat.v3.xyz, quat(GroundMat));
        }
    }
#endif //USE_ROTATION

#if USE_PLATFORM
    {
        for (int i = 0; i < m_platform_list.Count(); i++)
        {
            PhysicsObject* PhysObj = m_platform_list[i];

            mat4 GroundMat = PhysObj->GetTransform();
            if (i == 0)
            {
                GroundMat = GroundMat * mat4(quat::fromeuler_xyz(vec3(20.f, .0f, .0f) * seconds));
                PhysObj->SetTransform(GroundMat.v3.xyz, quat(GroundMat));
            }
            else if (i == 1)
            {
                GroundMat =
                    mat4::translate(vec3(-15.0f, 5.0f, lol::cos(m_loop_value) * 8.f)) *
                    mat4(quat::fromeuler_xyz(vec3(.0f, lol::cos(m_loop_value) * 20.f, .0f)));
                PhysObj->SetTransform(GroundMat.v3.xyz, quat(GroundMat));
            }
        }
    }
#endif //USE_PLATFORM

#if USE_CHARACTER
    {
        for (int i = 0; i < m_character_list.Count(); i++)
        {
            PhysicsObject* PhysObj = m_character_list[i];
            EasyCharacterController* Character = (EasyCharacterController*)PhysObj->GetCharacter();
            mat4 CtlrMx = Character->GetTransform();

            vec3 movement(0.f);
            movement.z = (m_controller->GetKey(KEY_MOVE_RIGHT).IsDown() ? 1.f : 0.f)
                       - (m_controller->GetKey(KEY_MOVE_LEFT).IsDown() ? 1.f : 0.f);
            movement.x = (m_controller->GetKey(KEY_MOVE_FORWARD).IsDown() ? 1.f : 0.f)
                       - (m_controller->GetKey(KEY_MOVE_BACK).IsDown() ? 1.f : 0.f);
            movement.y = (m_controller->GetKey(KEY_MOVE_UP).IsDown() ? 1.f : 0.f)
                       - (m_controller->GetKey(KEY_MOVE_DOWN).IsDown() ? 1.f : 0.f);
            vec3 CharMove = movement * seconds * vec3(4.f, 10.f, 4.f);

            if (m_controller->GetKey(KEY_MOVE_JUMP).IsReleased())
                Character->Jump();
            Character->SetMovementForFrame(CharMove);

            RayCastResult HitResult;
            if (m_simulation->RayHits(HitResult, ERT_Closest, Character->GetTransform().v3.xyz, (Character->GetTransform().v3.xyz + vec3(.0f, -1.f, .0f)), Character))
                Character->AttachTo(HitResult.m_collider_list[0], true, true);
            else
                Character->AttachTo(NULL);
        }
    }
#endif //USE_CHARACTER

#if USE_CHARACTER
    {
        PhysObjBarycenter = vec3(.0f);
        factor = .0f;

        for (int i = 0; i < m_character_list.Count(); i++)
        {
            PhysicsObject* PhysObj = m_character_list[i];
            mat4 GroundMat = PhysObj->GetTransform();

            PhysObjBarycenter += GroundMat.v3.xyz;
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
        for (int i = 0; i < m_physobj_list.Count(); i++)
        {
            PhysicsObject* PhysObj = m_physobj_list[i].m1;
            mat4 GroundMat = PhysObj->GetTransform();

            PhysObjBarycenter += GroundMat.v3.xyz;
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

void BtPhysTest::TickDraw(float seconds)
{
    WorldEntity::TickDraw(seconds);

    if (!m_ready)
    {
#if CAT_MODE
        /* cat datas setup */
        m_cat_shader = Shader::Create(LOLFX_RESOURCE_NAME(front_camera_sprite));
#if USE_BODIES
        for (int i = 0; i < m_physobj_list.Count(); i++)
        {
            PhysicsObject* PhysObj = m_physobj_list[i].m1;
            m_cat_sdata = new CatShaderData(((1 << VertexUsage::Position) |
                                                (1 << VertexUsage::Color) |
                                                (1 << VertexUsage::TexCoord) |
                                                (1 << VertexUsage::TexCoordExt)),
                                                m_cat_shader);
            m_cat_sdata->m_shader_texture = m_cat_texture->GetTexture();
            m_cat_sdata->m_sprite_flip = ((rand(2) == 1)?(1.f):(0.f)) / (float)(NB_SPRITE * PARTICLE_SIZE);
            PhysObj->SetCustomShaderData(m_cat_sdata);
            m_cat_sdata = NULL;
        }
#endif //USE_BODIES
#endif //CAT_MODE

        /* FIXME: this object never cleans up */
        m_ready = true;
    }
    else
    {
#if CAT_MODE
        for (int i = 0; i < m_physobj_list.Count(); i++)
        {
            PhysicsObject* PhysObj = m_physobj_list[i].m1;
            CatShaderData* ShaderData = (CatShaderData*)PhysObj->GetCustomShaderData();

            ShaderData->m_sprite_orientation = damp(ShaderData->m_sprite_orientation,
                                                    F_PI_4 * ((ShaderData->m_sprite_flip * 2.f * (float)(NB_SPRITE * PARTICLE_SIZE)) - 1.f) *
                                                    clamp(PhysObj->GetPhysic()->GetLinearVelocity().y / 20.0f, -1.f, 1.f),
                                                    0.1f, seconds);
        }
#endif //CAT_MODE
    }

    //Video::SetClearColor(vec4(0.0f, 0.0f, 0.12f, 1.0f));

}

BtPhysTest::~BtPhysTest()
{
    g_scene->PopCamera(m_camera);
    Ticker::Unref(m_light1);
    Ticker::Unref(m_light2);

#if CAT_MODE
    /* cat datas setup */
    Shader::Destroy(m_cat_shader);
    Tiler::Deregister(m_cat_texture);
#endif //CAT_MODE

    while (m_constraint_list.Count())
    {
        EasyConstraint* CurPop = m_constraint_list.Last();
        m_constraint_list.Pop();
        CurPop->RemoveFromSimulation(m_simulation);
        delete CurPop;
    }
    while (m_ground_list.Count())
    {
        PhysicsObject* CurPop = m_ground_list.Last();
        m_ground_list.Pop();
        CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
        Ticker::Unref(CurPop);
    }
    while (m_stairs_list.Count())
    {
        PhysicsObject* CurPop = m_stairs_list.Last();
        m_stairs_list.Pop();
        CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
        Ticker::Unref(CurPop);
    }
    while (m_character_list.Count())
    {
        PhysicsObject* CurPop = m_character_list.Last();
        m_character_list.Pop();
        CurPop->GetCharacter()->RemoveFromSimulation(m_simulation);
        Ticker::Unref(CurPop);
    }
    while (m_platform_list.Count())
    {
        PhysicsObject* CurPop = m_platform_list.Last();
        m_platform_list.Pop();
        CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
        Ticker::Unref(CurPop);
    }
    while (m_physobj_list.Count())
    {
        PhysicsObject* CurPop = m_physobj_list.Last().m1;
        m_physobj_list.Pop();
        CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
        Ticker::Unref(CurPop);
    }
    Ticker::Unref(m_simulation);

}

//-----------------------------------------------------------------------------
// CShaderData
//-----------------------------------------------------------------------------
CatShaderData::CatShaderData(uint32_t vert_decl_flags, Shader* shader)
    : GpuShaderData(vert_decl_flags, shader, DebugRenderMode::Default)
{
    m_sprite_orientation = .0f;
    m_sprite_flip = .0f;
    SetupDefaultData();
}

//-----------------------------------------------------------------------------
void CatShaderData::SetupDefaultData()
{
    AddUniform("in_model_view");
    AddUniform("in_normal_mat");
    AddUniform("in_proj");
    AddUniform("in_texture");
    AddUniform("in_sprite_orientation");
    AddUniform("in_sprite_flip");
}

//-----------------------------------------------------------------------------
void CatShaderData::SetupShaderDatas(mat4 const &model)
{
    mat4 proj = g_scene->GetCamera()->GetProjection();
    mat4 view = g_scene->GetCamera()->GetView();

    mat4 modelview = view * model;
    mat3 normalmat = transpose(inverse(mat3(view)));

    m_shader->SetUniform(*GetUniform("in_model_view"), modelview);
    m_shader->SetUniform(*GetUniform("in_normal_mat"), normalmat);
    m_shader->SetUniform(*GetUniform("in_proj"), proj);
    m_shader->SetUniform(*GetUniform("in_texture"), m_shader_texture, 0);
    m_shader->SetUniform(*GetUniform("in_sprite_orientation"), m_sprite_orientation);
    m_shader->SetUniform(*GetUniform("in_sprite_flip"), m_sprite_flip);
}

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("BtPhysTest", ivec2(1280, 960), 60.0f);

    new BtPhysTest(argc > 1);
    app.ShowPointer(false);

    app.Run();

    return EXIT_SUCCESS;
}

