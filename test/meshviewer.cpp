
//
// Lol Engine - EasyMesh tutorial
//
// Copyright: (c) 2011-2014 Sam Hocevar <sam@hocevar.net>
//            (c) 2012-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cfloat> /* for FLT_MAX */

#include <lol/main.h>
#include "scenesetup.h"

using namespace lol;

static int const TEXTURE_WIDTH = 256;

#define     NO_NACL_EM          (!__native_client__ && !EMSCRIPTEN)
#define     NACL_EM             (__native_client__ || EMSCRIPTEN)
#define     NO_NACL_EM_INPUT    (1)

#define     R_M                 1.f
#if NACL_EM
#define     DEFAULT_WIDTH       (800.f * R_M)
#define     DEFAULT_HEIGHT      (400.f * R_M)
#else
#define     DEFAULT_WIDTH       (1200.f * R_M)
#define     DEFAULT_HEIGHT      (400.f * R_M)
#endif //NACL_EM
#define     WIDTH               ((float)Video::GetSize().x)
#define     HEIGHT              ((float)Video::GetSize().y)
#define     SCREEN_W            (10.f / WIDTH)
#define     RATIO_HW            (HEIGHT / WIDTH)
#define     RATIO_WH            (WIDTH / HEIGHT)
#define     SCREEN_LIMIT        1.4f

#define     RESET_TIMER         .2f
#define     ROT_SPEED           vec2(50.f)
#define     ROT_CLAMP           89.f
#define     POS_SPEED           vec2(1.2f)
#define     POS_CLAMP           1.f
#define     FOV_SPEED           20.f
#define     FOV_CLAMP           120.f
#define     ZOM_SPEED           3.f
#define     ZOM_CLAMP           20.f
#define     HST_SPEED           .5f
#define     HST_CLAMP           1.f

#define     WITH_TEXTURE        0


#define     HAS_KBOARD          (m_input_usage & (1<<IPT_MV_KBOARD))
#define     HAS_MOUSE           (m_input_usage & (1<<IPT_MV_MOUSE))

LOLFX_RESOURCE_DECLARE(shinyfur);
LOLFX_RESOURCE_DECLARE(shinymvtexture);

enum
{
    IPT_MV_KBOARD = 0,
    IPT_MV_MOUSE,

    INPUT_MAX
};

enum MVKeyboardList
{
    KEY_CAM_RESET = 0,
    KEY_CAM_POS,
    KEY_CAM_FOV,

    KEY_CAM_UP,
    KEY_CAM_DOWN,
    KEY_CAM_LEFT,
    KEY_CAM_RIGHT,

    KEY_MESH_NEXT,
    KEY_MESH_PREV,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_ESC,

    KEY_MAX
};

enum MVMouseKeyList
{
    MSE_CAM_ROT = KEY_MAX,
    MSE_CAM_POS,
    MSE_CAM_FOV,
    MSE_FOCUS,

    MSE_MAX
};

enum MVMouseAxisList
{
    MSEX_CAM_Y = 0,
    MSEX_CAM_X,

    MSEX_MAX
};

#define     MAX_KEYS        MSE_MAX
#define     MAX_AXIS        MSEX_MAX
#define     ALL_FEATURES    1
#define     NO_SC_SETUP     0

enum GizmoType
{
    GZ_Editor = 0,
    GZ_LightPos,
    GZ_LightDir,

    GZ_MAX
};

struct LightData
{
    LightData(vec3 pos, vec4 col)
    {
        m_pos = pos;
        m_col = col;
    }

    vec3 m_pos;
    vec4 m_col;
};

class TargetCamera
{
public:
    void EmptyTargets()             { m_targets.Empty(); }
    void AddTarget(vec3 new_target) { m_targets << new_target; }
    //This considers the box usage A to B as top-left to bottom-right
    void AddTarget(box3 new_target)
    {
        vec3 base_off = .5f * (new_target.B - new_target.A);
        vec3 base_pos = new_target.A + base_off;
        int pass = 0;
        while (pass < 3)
        {
            int mask = 3 - max(0, pass - 1);
            while (mask-- > 0)
            {
                ivec3 A((pass == 1 || (pass == 2 && mask == 1))?(1):(0));
                ivec3 B((pass == 2)?(1):(0)); B[mask] = 1;
                vec3 offset = vec3(ivec3((int)(!A.x != !B.x), (int)(!A.y != !B.y), (int)(!A.z != !B.z)));
                AddTarget(base_pos + offset * base_off * 2.f - base_off);
            }
            pass++;
        }
    }

    array<vec3>     m_targets;
};

class MeshViewer : public WorldEntity
{
public:
    MeshViewer(char const *file_name = "data/mesh-buffer.txt")
      : m_file_name(file_name)
    {
        m_init = false;
        m_first_tick = false;

        // Message Service
        MessageService::Setup();

        m_ssetup = nullptr;
        m_camera = nullptr;
        m_controller = nullptr;

        //Compile ref meshes
        m_gizmos << new EasyMesh();
        m_gizmos.Last()->Compile("[sc#0f0 ac 3 .5 .4 0 ty .25 [ad 3 .4 sy -1] ty .5 ac 3 1 .075 ty .5 dup[rz 90 ry 90 scv#00f dup[ry 90 scv#f00]]][sc#fff ab .1]");
        m_gizmos << new EasyMesh();
        m_gizmos.Last()->Compile("[sc#666 acap 1 .5 .5 ty -.5 sc#fff asph 2 1]");
        m_gizmos << new EasyMesh();
        m_gizmos.Last()->Compile("[sc#fff ac 3 .5 .4 0 ty .25 [ad 3 .4 sy -1] ty .5 ac 3 1 .1 ty .5 [ad 3 .1 sy -1] ty 1 rz 90 ry 90]");

        // Mesh Setup
        m_render_max = vec2(-.9f, 4.1f);
        m_mesh_render = 0;
        m_mesh_id = 0;
        m_mesh_id1 = 0.f;
        m_default_texture = nullptr;
        m_texture_shader = nullptr;
        m_texture = nullptr;

        //Camera Setup
        m_reset_timer = -1.f;
        m_fov = -100.f;
        m_fov_mesh = 0.f;
        m_fov_speed = 0.f;
        m_zoom = 0.f;
        m_zoom_mesh = 0.f;
        m_zoom_speed = 0.f;
        m_rot = vec2(/*45.f*/0.f, -45.f);
        m_rot_mesh = vec2::zero;
        m_rot_speed = vec2::zero;
        m_pos = vec2::zero;
        m_pos_mesh = vec2::zero;
        m_pos_speed = vec2::zero;
        m_screen_offset = vec2::zero;
        m_hist_scale = vec2(.13f, .03f);
        m_hist_scale_mesh = vec2(.0f);
        m_hist_scale_speed = vec2(.0f);

        m_mat_prev = mat4(quat::fromeuler_xyz(vec3::zero));
        m_mat = mat4::translate(vec3(0.f));//mat4(quat::fromeuler_xyz(vec3(m_rot_mesh, .0f)));

        m_build_timer = 0.1f;
        m_build_time = -1.f;

        //stream update
        m_stream_update_time = 2.0f;
        m_stream_update_timer = 1.0f;
    }

    ~MeshViewer()
    {
        if (m_camera)
            g_scene->PopCamera(m_camera);
        if (m_ssetup)
            delete(m_ssetup);
        MessageService::Destroy();

        m_controller = nullptr;
        m_camera = nullptr;
        m_ssetup = nullptr;
    }

#if NO_NACL_EM_INPUT
    bool  KeyReleased(MVKeyboardList index) { return (HAS_KBOARD && m_controller->GetKey(index).IsReleased()); }
    bool  KeyPressed(MVKeyboardList index)  { return (HAS_KBOARD && m_controller->GetKey(index).IsPressed()); }
    bool  KeyDown(MVKeyboardList index)     { return (HAS_KBOARD && m_controller->GetKey(index).IsDown()); }
    bool  KeyReleased(MVMouseKeyList index) { return (HAS_MOUSE  && m_controller->GetKey(index).IsReleased()); }
    bool  KeyPressed(MVMouseKeyList index)  { return (HAS_MOUSE  && m_controller->GetKey(index).IsPressed()); }
    bool  KeyDown(MVMouseKeyList index)     { return (HAS_MOUSE  && m_controller->GetKey(index).IsDown()); }
    float AxisValue(MVMouseAxisList index)  { return (HAS_MOUSE)?(m_controller->GetAxis(index).GetValue()):(0.f); }
#endif //NO_NACL_EM_INPUT

    void Init()
    {
        m_init = true;
        m_input_usage = 0;

#if NO_NACL_EM_INPUT
        /* Register an input controller for the keyboard */
        m_controller = new Controller("Default", MAX_KEYS, MAX_AXIS);

        if (InputDevice::Get(g_name_mouse.C()))
        {
            m_input_usage |= (1<<IPT_MV_MOUSE);

            m_controller->GetKey(MSE_CAM_ROT).BindMouse("Left");
            m_controller->GetKey(MSE_CAM_POS).BindMouse("Right");
            m_controller->GetKey(MSE_CAM_FOV).BindMouse("Middle");
            m_controller->GetKey(MSE_FOCUS).BindMouse("InScreen");
            m_controller->GetAxis(MSEX_CAM_Y).BindMouse("Y");
            m_controller->GetAxis(MSEX_CAM_X).BindMouse("X");
        }

        if (InputDevice::Get(g_name_keyboard.C()))
        {
            m_input_usage |= (1<<IPT_MV_KBOARD);

            //Camera keyboard rotation
            m_controller->GetKey(KEY_CAM_UP   ).BindKeyboard("Up");
            m_controller->GetKey(KEY_CAM_DOWN ).BindKeyboard("Down");
            m_controller->GetKey(KEY_CAM_LEFT ).BindKeyboard("Left");
            m_controller->GetKey(KEY_CAM_RIGHT).BindKeyboard("Right");

            //Camera keyboard position switch
            m_controller->GetKey(KEY_CAM_POS  ).BindKeyboard("LeftShift");
            m_controller->GetKey(KEY_CAM_FOV  ).BindKeyboard("LeftCtrl");

            //Camera unzoom switch
            m_controller->GetKey(KEY_CAM_RESET).BindKeyboard("Space");

            //Mesh change
            m_controller->GetKey(KEY_MESH_NEXT).BindKeyboard("PageUp");
            m_controller->GetKey(KEY_MESH_PREV).BindKeyboard("PageDown");

            //Base setup
            m_controller->GetKey(KEY_F1).BindKeyboard("F1");
            m_controller->GetKey(KEY_F2).BindKeyboard("F2");
            m_controller->GetKey(KEY_F3).BindKeyboard("F3");
            m_controller->GetKey(KEY_F4).BindKeyboard("F4");
            m_controller->GetKey(KEY_F5).BindKeyboard("F5");
            m_controller->GetKey(KEY_ESC).BindKeyboard("Escape");
        }
#endif //NO_NACL_EM_INPUT


        m_camera = new Camera();
        m_camera->SetView(vec3(0.f, 0.f, 10.f), vec3::zero, vec3::axis_y);
        m_camera->SetProjection(0.f, .0001f, 2000.f, WIDTH * SCREEN_W, RATIO_HW);
        m_camera->UseShift(true);
        g_scene->PushCamera(m_camera);

        //Lights setup
        m_ssetup = new SceneSetup();
#if NO_SC_SETUP
        m_ssetup->m_lights << new Light();
        m_ssetup->m_lights.Last()->SetPosition(vec4(4.f, -1.f, -4.f, 0.f));
        m_ssetup->m_lights.Last()->SetColor(vec4(.0f, .2f, .5f, 1.f));
        Ticker::Ref(m_ssetup->m_lights.Last());
        m_ssetup->m_lights << new Light();
        m_ssetup->m_lights.Last()->SetPosition(vec4(8.f, 2.f, 6.f, 0.f));
        m_ssetup->m_lights.Last()->SetColor(vec4(1.f));
        Ticker::Ref(m_ssetup->m_lights.Last());
        EasyMesh* em = new EasyMesh();
        if (em->Compile("sc#fff ab 1"))
        {
            if (m_mesh_id == m_meshes.Count() - 1)
                m_mesh_id++;
            m_meshes.Push(em, nullptr);
        }
#else
        m_ssetup->Compile("addlight 0.0 position (4 -1 -4) color (.0 .2 .5 1) "
                          "addlight 0.0 position (8 2 6) color #ffff "
                          "showgizmo true ");
        m_ssetup->Startup();
#endif //NO_SC_SETUP
        for (int i = 0; i < m_ssetup->m_lights.Count(); ++i)
        {
            m_light_datas << LightData(m_ssetup->m_lights[i]->GetPosition().xyz, m_ssetup->m_lights[i]->GetColor());
            m_ssetup->m_lights[i]->SetPosition(vec3::zero);
            m_ssetup->m_lights[i]->SetColor(vec4::zero);
        }
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        if (!m_init && g_scene)
        {
            Init();
            return;
        }

        if (!m_init)
            return;

        m_first_tick = true;

        //TODO : This should probably be "standard LoL behaviour"
#if NO_NACL_EM_INPUT
        {
            //Shutdown logic
            if (KeyReleased(KEY_ESC))
                Ticker::Shutdown();
        }
#endif //NO_NACL_EM_INPUT

        //Compute render mesh count
        float a_j = lol::abs(m_render_max[1]);
        float i_m = m_hist_scale_mesh.x;
        float i_trans = a_j - ((a_j * a_j * i_m * i_m + a_j * i_m) * .5f);
        m_render_max[1] = a_j * ((RATIO_WH * 1.f) / ((i_trans != 0.f)?(i_trans):(RATIO_WH))) - RATIO_HW * .3f;

        //Mesh Change
#if NO_NACL_EM_INPUT
        m_mesh_id = clamp(m_mesh_id + ((int)KeyPressed(KEY_MESH_PREV) - (int)KeyPressed(KEY_MESH_NEXT)), 0, m_meshes.Count() - 1);
#endif //NO_NACL_EM_INPUT
        m_mesh_id1 = damp(m_mesh_id1, (float)m_mesh_id, .2f, seconds);

#if ALL_FEATURES

        //Update light position & damping
        for (int i = 0; i < m_ssetup->m_lights.Count(); ++i)
        {
            vec3 pos = (m_mat * inverse(m_mat_prev) * vec4(m_ssetup->m_lights[i]->GetPosition(), 1.f)).xyz;
            vec3 tgt = (m_mat * vec4(m_light_datas[i].m_pos, 1.f)).xyz;

            vec3 new_pos = damp(pos, tgt, .3f, seconds);
            vec4 new_col = damp(m_ssetup->m_lights[i]->GetColor(), m_light_datas[i].m_col, .3f, seconds);

            m_ssetup->m_lights[i]->SetPosition(new_pos);
            m_ssetup->m_lights[i]->SetColor(new_col);
        }

        //Camera update
        bool is_pos = false;
        bool is_fov = false;
        bool is_hsc = false;
        vec2 tmpv = vec2::zero;

#if NO_NACL_EM_INPUT
        is_pos = KeyDown(KEY_CAM_POS) || KeyDown(MSE_CAM_POS);
        is_fov = KeyDown(KEY_CAM_FOV) || KeyDown(MSE_CAM_FOV);

        if (KeyDown(MSE_FOCUS) && (KeyDown(MSE_CAM_ROT) || KeyDown(MSE_CAM_POS) || KeyDown(MSE_CAM_FOV)))
        {
            tmpv += vec2(AxisValue(MSEX_CAM_Y), AxisValue(MSEX_CAM_X));
            if (KeyDown(MSE_CAM_ROT))
                tmpv *= vec2(1.f, 1.f) * 6.f;
            if (KeyDown(MSE_CAM_POS))
                tmpv *= vec2(1.f, -1.f) * 3.f;
            if (KeyDown(MSE_CAM_FOV))
                tmpv = vec2(tmpv.y * 4.f, tmpv.x * 6.f);
        }

        tmpv += vec2((float)KeyDown(KEY_CAM_UP   ) - (float)KeyDown(KEY_CAM_DOWN),
                     (float)KeyDown(KEY_CAM_RIGHT) - (float)KeyDown(KEY_CAM_LEFT));
#endif //NO_NACL_EM_INPUT

        //Base data
        vec2 rot = (!is_pos && !is_fov)?(tmpv):(vec2(.0f)); rot = vec2(rot.x, -rot.y);
        vec2 pos = ( is_pos && !is_fov)?(tmpv):(vec2(.0f)); pos = -vec2(pos.y, pos.x);
        vec2 fov = (!is_pos && is_fov )?(tmpv):(vec2(.0f)); fov = vec2(-fov.x, fov.y);
        vec2 hsc = (is_hsc)?(vec2(0.f)):(vec2(0.f));

        //speed
        m_rot_speed = damp(m_rot_speed, rot * ROT_SPEED, .2f, seconds);
        float pos_factor = 1.f / (1.f + m_zoom_mesh * .5f);
        m_pos_speed = damp(m_pos_speed, pos * POS_SPEED * pos_factor, .2f, seconds);
        float fov_factor = 1.f + lol::pow((m_fov_mesh / FOV_CLAMP) * 1.f, 2.f);
        m_fov_speed = damp(m_fov_speed, fov.x * FOV_SPEED * fov_factor, .2f, seconds);
        float zom_factor = 1.f + lol::pow((m_zoom_mesh / ZOM_CLAMP) * 1.f, 2.f);
        m_zoom_speed = damp(m_zoom_speed, fov.y * ZOM_SPEED * zom_factor, .2f, seconds);
        m_hist_scale_speed = damp(m_hist_scale_speed, hsc * HST_SPEED, .2f, seconds);

        m_rot += m_rot_speed * seconds;

#if NO_NACL_EM_INPUT
        if (m_reset_timer >= 0.f)
            m_reset_timer -= seconds;
        if (KeyPressed(KEY_CAM_RESET))
        {
            if (m_reset_timer >= 0.f)
            {
                m_pos = vec2(0.f);
                m_zoom = 0.f;
            }
            else
                m_reset_timer = RESET_TIMER;
        }

        //Transform update
        if (!KeyDown(KEY_CAM_RESET))
        {
            m_pos += m_pos_speed * seconds;
            m_fov += m_fov_speed * seconds;
            m_zoom += m_zoom_speed * seconds;
            m_hist_scale += m_hist_scale_speed * seconds;
        }
#endif //NO_NACL_EM_INPUT

        //clamp
        vec2 rot_mesh = vec2(SmoothClamp(m_rot.x, -ROT_CLAMP, ROT_CLAMP, ROT_CLAMP * .1f), m_rot.y);
        vec2 pos_mesh = vec2(SmoothClamp(m_pos.x, -POS_CLAMP, POS_CLAMP, POS_CLAMP * .1f),
                             SmoothClamp(m_pos.y, -POS_CLAMP, POS_CLAMP, POS_CLAMP * .1f));
        float fov_mesh = SmoothClamp(m_fov, 0.f, FOV_CLAMP, FOV_CLAMP * .1f);
        float zoom_mesh = SmoothClamp(m_zoom, -ZOM_CLAMP, ZOM_CLAMP, ZOM_CLAMP * .1f);
        vec2 hist_scale_mesh = vec2(SmoothClamp(m_hist_scale.x, 0.f, HST_CLAMP, HST_CLAMP * .1f),
                                    SmoothClamp(m_hist_scale.y, 0.f, HST_CLAMP, HST_CLAMP * .1f));

#if NO_NACL_EM_INPUT
        if (KeyDown(KEY_CAM_RESET) && m_reset_timer < 0.f)
        {
            pos_mesh = vec2::zero;
            zoom_mesh = 0.f;
        }
#endif //NO_NACL_EM_INPUT

        m_rot_mesh = vec2(damp(m_rot_mesh.x, rot_mesh.x, .2f, seconds), damp(m_rot_mesh.y, rot_mesh.y, .2f, seconds));
        m_pos_mesh = vec2(damp(m_pos_mesh.x, pos_mesh.x, .2f, seconds), damp(m_pos_mesh.y, pos_mesh.y, .2f, seconds));
        m_fov_mesh = damp(m_fov_mesh, fov_mesh, .2f, seconds);
        m_zoom_mesh = damp(m_zoom_mesh, zoom_mesh, .2f, seconds);
        m_hist_scale_mesh = damp(m_hist_scale_mesh, hist_scale_mesh, .2f, seconds);

        //Mesh mat calculation
        m_mat_prev = m_mat;
        m_mat = mat4::translate(vec3(0.f));

        //Target List Setup
        TargetCamera tc;
        if (m_meshes.Count() && m_mesh_id >= 0)
            for (int i = 0; i < m_meshes[m_mesh_id].m1->GetVertexCount(); i++)
                tc.AddTarget((m_mat * mat4::translate(m_meshes[m_mesh_id].m1->GetVertexLocation(i))).v3.xyz);
        tc.AddTarget(box3(vec3(0.f), vec3(1.f)));
        for (int k = 0; k < m_ssetup->m_lights.Count() && m_ssetup->m_show_lights; ++k)
        {
            vec3 light_pos = m_ssetup->m_lights[k]->GetPosition();
            mat4 world_cam = m_camera->GetView();
            light_pos = (inverse(world_cam) * vec4((world_cam * vec4(light_pos, 1.0f)).xyz * vec3::axis_z, 1.0f)).xyz;
            tc.AddTarget(box3(vec3(-1.f), vec3(1.f)) + light_pos *
                         ((m_ssetup->m_lights[k]->GetType() == LightType::Directional)?(-1.f):(1.f)));
        }

        //--
        //Update mesh screen location - Get the Min/Max needed
        //--
        vec2 cam_center(0.f);
        float cam_factor = .0f;
        vec3 local_min_max[2] = { vec3(FLT_MAX), vec3(-FLT_MAX) };
        vec2 screen_min_max[2] = { vec2(FLT_MAX), vec2(-FLT_MAX) };
        mat4 world_cam = m_camera->GetView();
        mat4 cam_screen = m_camera->GetProjection();

        //target on-screen computation
        for (int i = 0; i < tc.m_targets.Count(); i++)
        {
            vec3 obj_loc = tc.m_targets[i];
            {
                //Debug::DrawBox(obj_loc - vec3(4.f), obj_loc + vec3(4.f), vec4(1.f, 0.f, 0.f, 1.f));
                mat4 target_mx = mat4::translate(obj_loc);
                vec3 vpos;

                //Get location in cam coordinates
                target_mx = world_cam * target_mx;
                vpos = target_mx.v3.xyz;
                local_min_max[0] = min(vpos.xyz, local_min_max[0]);
                local_min_max[1] = max(vpos.xyz, local_min_max[1]);

                //Get location in screen coordinates
                target_mx = cam_screen * target_mx;
                vpos = (target_mx.v3 / target_mx.v3.w).xyz;
                screen_min_max[0] = min(screen_min_max[0], vpos.xy * vec2(RATIO_WH, 1.f));
                screen_min_max[1] = max(screen_min_max[1], vpos.xy * vec2(RATIO_WH, 1.f));

                //Build Barycenter
                cam_center += vpos.xy;
                cam_factor += 1.f;
            }
        }
        float screen_ratio = max(max(lol::abs(screen_min_max[0].x), lol::abs(screen_min_max[0].y)),
                                 max(lol::abs(screen_min_max[1].x), lol::abs(screen_min_max[1].y)));
        float z_dist = //m_camera->m_target_distance
            length(m_camera->m_position)
            + max(local_min_max[0].z, local_min_max[1].z);

        vec2 screen_offset = vec2(0.f, -(screen_min_max[1].y + screen_min_max[0].y) * .5f);
        m_screen_offset = damp(m_screen_offset, screen_offset, .9f, seconds);

        float forced_zoom = m_zoom_mesh;

        if (cam_factor > 0.f)
        {
            vec2 old_sscale = m_camera->GetScreenScale();
            float old_ssize = m_camera->GetScreenSize();
            float zoom_in  = 1.f + lol::max(0.f, forced_zoom);
            float zoom_out = 1.f + lol::max(0.f, -forced_zoom);
            m_camera->SetScreenScale(max(vec2(0.001f), ((old_sscale * zoom_in) / (screen_ratio * zoom_out * SCREEN_LIMIT))));
            m_camera->SetFov(m_fov_mesh);
            m_camera->SetScreenInfos(damp(old_ssize, max(1.f, screen_ratio * zoom_out), 1.2f, seconds));

            vec3 posz = ((mat4::rotate(m_rot_mesh.y, vec3::axis_y) * mat4::rotate(-m_rot_mesh.x, vec3::axis_x) * vec4::axis_z)).xyz;
            vec3 newpos = posz * damp(length(m_camera->m_position), z_dist * 1.2f, .1f, seconds);
            m_camera->SetView(newpos, vec3(0.f), vec3::axis_y);
        }

        //--
        //Message Service
        //--
        String mesh("");
        int u = 1;
        while (u-- > 0 && MessageService::FetchFirst(MessageBucket::AppIn, mesh))
        {
            int o = 1;
            while (o-- > 0)
            {
                SceneSetup* new_ssetup = new SceneSetup();
                if (new_ssetup->Compile(mesh.C()) && new_ssetup->m_lights.Count())
                {
                    //Store current light datas, in World
                    array<LightData> light_datas;
                    for (int i = 0; i < m_ssetup->m_lights.Count(); ++i)
                        light_datas << LightData(m_ssetup->m_lights[i]->GetPosition(), m_ssetup->m_lights[i]->GetColor());

                    if (m_ssetup)
                        delete(m_ssetup);
                    m_ssetup = new_ssetup;
                    m_ssetup->Startup();

                    //Restore all light datas so blend can occur
                    mat4 light_mat = m_mat * inverse(mat4(quat::fromeuler_xyz(vec3::zero)));
                    for (int i = 0; i < m_ssetup->m_lights.Count(); ++i)
                    {
                        //Store local dst in current m_ld
                        LightData ltmp = LightData(m_ssetup->m_lights[i]->GetPosition(), m_ssetup->m_lights[i]->GetColor());
                        if (i < m_light_datas.Count())
                            m_light_datas[i] = ltmp;
                        else
                            m_light_datas << ltmp;

                        vec3 loc = vec3::zero;
                        vec4 col = vec4::zero;
                        if (i < light_datas.Count())
                        {
                            loc = light_datas[i].m_pos;
                            col = light_datas[i].m_col;
                        }

                        //Restore old light datas in new lights
                        m_ssetup->m_lights[i]->SetPosition(loc);
                        m_ssetup->m_lights[i]->SetColor(col);
                    }
                }
                else
                {
                    m_ssetup->m_custom_cmd += new_ssetup->m_custom_cmd;
                    delete(new_ssetup);
                }
            }
        }

        //Check the custom cmd even if we don't have new messages.
        int o = 1;
        while (o-- > 0)
        {
            for (int i = 0; m_ssetup && i < m_ssetup->m_custom_cmd.Count(); ++i)
            {
                if (m_ssetup->m_custom_cmd[i].m1 == "setmesh")
                {
                    //Create a new mesh
                    EasyMesh* em = new EasyMesh();
                    if (em->Compile(m_ssetup->m_custom_cmd[i].m2.C(), false))
                    {
                        em->BD()->Cmdi() = 0;
                        if (m_mesh_id == m_meshes.Count() - 1)
                            m_mesh_id++;
                        m_meshes.Push(em, nullptr);
                    }
                    else
                        delete(em);
                }
            }
        }
        m_ssetup->m_custom_cmd.Empty();
#endif //ALL_FEATURES

#if NACL_EM
/*
        if (m_stream_update_time > .0f)
        {
            m_stream_update_time = -1.f;
            MessageService::Send(MessageBucket::AppIn,
                             " addlight 0.0 position (4 -1 -4) color (.0 .2 .5 1) \
                               addlight 0.0 position (8 2 6) color #ffff \
                               custom setmesh \"[sc#f8f ab 1]\"");
//            MessageService::Send(MessageBucket::AppIn, "[sc#f8f ab 1]");
//            MessageService::Send(MessageBucket::AppIn, "[sc#f8f ab 1 splt 4 twy 90]");
//            MessageService::Send(MessageBucket::AppIn, "[sc#8ff afcb 1 1 1 0]");
//            MessageService::Send(MessageBucket::AppIn, "[sc#ff8 afcb 1 1 1 0]");
        }
*/
#elif defined(_WIN32)
        //--
        //File management
        //--
        m_stream_update_time += seconds;
        if (m_stream_update_time > m_stream_update_timer)
        {
            m_stream_update_time = 0.f;

            File f;
            f.Open(m_file_name.C(), FileAccess::Read);
            String cmd = f.ReadString();
            f.Close();

            if (cmd.Count()
                 && (!m_cmdlist.Count() || cmd != m_cmdlist.Last()))
            {
                m_cmdlist << cmd;
                MessageService::Send(MessageBucket::AppIn, cmd);
            }
        }
#endif //WINDOWS
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        if (!m_init || !m_first_tick)
            return;

        //TODO : This should probably be "standard LoL behaviour"
#if NO_NACL_EM_INPUT
        {
            if (KeyReleased(KEY_F2))
                Video::SetDebugRenderMode((Video::GetDebugRenderMode() + 1) % DebugRenderMode::Max);
            else if (KeyReleased(KEY_F1))
                Video::SetDebugRenderMode((Video::GetDebugRenderMode() + DebugRenderMode::Max - 1) % DebugRenderMode::Max);
        }
#endif //NO_NACL_EM_INPUT

#if NO_NACL_EM && WITH_TEXTURE
        if (!m_default_texture)
        {
            m_texture_shader = Shader::Create(LOLFX_RESOURCE_NAME(shinymvtexture));
            m_texture_uni = m_texture_shader->GetUniformLocation("u_Texture");
            m_default_texture = Tiler::Register("data/test-texture.png", ivec2::zero, ivec2(0,1));
        }
        else if (m_texture && m_default_texture)
            m_texture_shader->SetUniform(m_texture_uni, m_default_texture->GetTexture(), 0);
#endif //NO_NACL_EM

        g_renderer->SetClearColor(m_ssetup->m_clear_color);

        for (int i = 0; i < m_gizmos.Count(); ++i)
        {
            if (m_gizmos[i]->GetMeshState() == MeshRender::NeedConvert)
                m_gizmos[i]->MeshConvert();
            else
                break;
        }

        if (m_build_timer > .0f)
        {
            if (m_build_time < .0f)
            {
                m_build_time = m_build_timer;
                for (int i = 0; i < m_meshes.Count(); ++i)
                {
                    if (m_meshes[i].m1 && m_meshes[i].m1->BD()->Cmdi() < m_meshes[i].m1->BD()->CmdStack().GetCmdNb())
                    {
                        EasyMesh* tmp = m_meshes[i].m1;
                        EasyMesh* newtmp = new EasyMesh(*tmp);
                        int ii = 1;
#if 1
                        bool stop = false;
                        while (!stop)
                        {
                            int cmdi = newtmp->BD()->Cmdi() + ii;
                            if (cmdi < newtmp->BD()->CmdStack().GetCmdNb())
                            {
                                switch (newtmp->BD()->CmdStack().GetCmd(cmdi))
                                {
                                    case EasyMeshCmdType::LoopStart:
                                    case EasyMeshCmdType::LoopEnd:
                                    case EasyMeshCmdType::OpenBrace:
                                    case EasyMeshCmdType::CloseBrace:
                                    case EasyMeshCmdType::ScaleWinding:
                                    case EasyMeshCmdType::QuadWeighting:
                                    case EasyMeshCmdType::PostBuildNormal:
                                    case EasyMeshCmdType::PreventVertCleanup:
                                    case EasyMeshCmdType::SetColorA:
                                    case EasyMeshCmdType::SetColorB:
                                    {
                                        ii++;
                                        break;
                                    }
                                    default:
                                    {
                                        stop = true;
                                        break;
                                    }
                                }
                            }
                            else
                                stop = true;
                        }
#endif
                        newtmp->BD()->CmdExecNb() = ii;
                        newtmp->ExecuteCmdStack(false);

                        m_meshes[i].m1 = newtmp;
                        delete(tmp);
                    }
                }
            }
            m_build_time -= seconds;
        }

#define NORMAL_USAGE 1
#if NORMAL_USAGE
        vec3 x = vec3(1.f,0.f,0.f);
        vec3 y = vec3(0.f,1.f,0.f);
        mat4 save_proj = m_camera->GetProjection();
                              //Y object Offset
        mat4 mat_obj_offset = mat4::translate(x * m_screen_offset.x + y * m_screen_offset.y) *
                              //Mesh Pos Offset
                              mat4::translate((x * m_pos_mesh.x * RATIO_HW + y * m_pos_mesh.y) * 2.f * (1.f + .5f * m_zoom_mesh / SCREEN_LIMIT));
                              //Align right meshes
        mat4 mat_align =      mat4::translate(x - x * RATIO_HW);
        mat4 mat_gizmo = mat_obj_offset * mat_align * save_proj;
        for (int i = 0; i < m_meshes.Count(); i++)
        {
            {
                if (m_meshes[i].m1->GetMeshState() == MeshRender::NeedConvert)
                {
#if WITH_TEXTURE
                    m_meshes[i].m1->MeshConvert(new DefaultShaderData(((1 << VertexUsage::Position) | (1 << VertexUsage::Normal) |
                                                                       (1 << VertexUsage::Color)    | (1 << VertexUsage::TexCoord)),
                                                                       m_texture_shader, true));
#else
                    m_meshes[i].m1->MeshConvert();
#endif //WITH_TEXTURE
                }
#if ALL_FEATURES
                float j = -(float)(m_meshes.Count() - (i + 1)) + (-m_mesh_id1 + (float)(m_meshes.Count() - 1));

                if (m_mesh_id1 - m_render_max[0] > (float)i && m_mesh_id1 - m_render_max[1] < (float)i &&
                    m_meshes[i].m1->GetMeshState() > MeshRender::NeedConvert)
                {
                    float a_j = lol::abs(j);
                    float i_trans = (a_j * a_j * m_hist_scale_mesh.x + a_j * m_hist_scale_mesh.x) * .5f;
                    float i_scale = clamp(1.f - (m_hist_scale_mesh.y * (m_mesh_id1 - (float)i)), 0.f, 1.f);

                    //Mesh count offset
                    mat4 mat_count_offset = mat4::translate(x * RATIO_HW * 2.f * (j + i_trans));
                    //Mesh count scale
                    mat4 mat_count_scale = mat4::scale(vec3(vec2(i_scale), 1.f));

                    //Camera projection
                    mat4 new_proj = mat_obj_offset * mat_count_offset * mat_align * mat_count_scale * save_proj;
                    m_camera->SetProjection(new_proj);
                    scene.AddPrimitive(*m_meshes[i].m1, m_mat);
                    g_renderer->Clear(ClearMask::Depth);
                }
                m_camera->SetProjection(save_proj);
#else
                scene.AddPrimitive(*m_meshes[i].m1, m_mat);
#endif //ALL_FEATURES
            }
        }

        //Scene setup update
        if (m_ssetup)
        {
            m_camera->SetProjection(mat_gizmo);
            if (m_ssetup->m_show_gizmo)
                scene.AddPrimitive(*m_gizmos[GZ_Editor], m_mat);

            if (m_ssetup->m_show_lights)
            {
                for (int k = 0; k < m_ssetup->m_lights.Count(); ++k)
                {
                    Light* ltmp = m_ssetup->m_lights[k];
                    mat4 world = mat4::translate(ltmp->GetPosition());
                    mat4 local = mat4::translate((inverse(m_mat) * world).v3.xyz);
                    //dir light
                    if (ltmp->GetType() == LightType::Directional)
                    {
                        scene.AddPrimitive(*m_gizmos[GZ_LightPos], m_mat * inverse(local));
                        scene.AddPrimitive(*m_gizmos[GZ_LightDir], inverse(world) * inverse(mat4::lookat(vec3::zero, -ltmp->GetPosition(), vec3::axis_y)));
                    }
                    else //point light
                    {
                        scene.AddPrimitive(*m_gizmos[GZ_LightPos], m_mat * local);
                    }
                }
            }
            m_camera->SetProjection(save_proj);
        }
#endif //NORMAL_USAGE

#if 0 //Debug normal draw
        for (int i = m_meshes.Count() - 1; 0 <= i && i < m_meshes.Count(); i++)
        {
            for (int j = 0; j < m_meshes[i].m1->m_indices.Count(); j += 3)
            {
                VertexData v[3] = { m_meshes[i].m1->m_vert[m_meshes[i].m1->m_indices[j  ]],
                                    m_meshes[i].m1->m_vert[m_meshes[i].m1->m_indices[j+1]],
                                    m_meshes[i].m1->m_vert[m_meshes[i].m1->m_indices[j+2]]
                                    };
                for (int k = 0; k < 3; k++)
                    Debug::DrawLine((m_mat * mat4::translate(v[k].m_coord)).v3.xyz,
                                    (m_mat * mat4::translate(v[(k+1)%3].m_coord)).v3.xyz, vec4(vec3((v[k].m_coord.z + 1.f)*.5f),1.f));
            }
            for (int j = 0; j < m_meshes[i].m1->m_vert.Count(); j++)
            {
                VertexData &v = m_meshes[i].m1->m_vert[m_meshes[i].m1->m_indices[j]];
                Debug::DrawLine((m_mat * mat4::translate(v.m_coord)).v3.xyz,
                                (m_mat * mat4::translate(v.m_coord)).v3.xyz +
                                (m_mat * vec4(v.m_normal * 5.f, 0.f)).xyz, vec4(lol::abs(v.m_normal), 1.f));
            }
        }
#endif
    }

private:
    SceneSetup*         m_ssetup;
    array<LightData>    m_light_datas;
    Controller*         m_controller;
    short               m_input_usage;
    mat4                m_mat;
    mat4                m_mat_prev;
    bool                m_init;
    bool                m_first_tick;

    //Camera Setup
    Camera *            m_camera;
    float               m_reset_timer;
    float               m_fov;
    float               m_fov_mesh;
    float               m_fov_speed;
    float               m_zoom;
    float               m_zoom_mesh;
    float               m_zoom_speed;
    vec2                m_rot;
    vec2                m_rot_mesh;
    vec2                m_rot_speed;
    vec2                m_pos;
    vec2                m_pos_mesh;
    vec2                m_pos_speed;
    vec2                m_hist_scale;
    vec2                m_hist_scale_mesh;
    vec2                m_hist_scale_speed;
    vec2                m_screen_offset;

    //Mesh update timer
    float               m_build_timer;
    float               m_build_time;

    //Mesh infos
    vec2                m_render_max;
    int                 m_mesh_render;
    int                 m_mesh_id;
    float               m_mesh_id1;
    array<EasyMesh*, EasyMesh*> m_meshes;
    array<EasyMesh*>    m_gizmos;

    //File data
    String              m_file_name;
    array<String>       m_cmdlist;
    float               m_stream_update_time;
    float               m_stream_update_timer;

    //misc datas
    Shader *            m_texture_shader;
    TileSet *           m_default_texture;
    Texture *           m_texture;
    ShaderUniform       m_texture_uni;
};

//The basic main :
int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("MeshViewer", ivec2((int)DEFAULT_WIDTH, (int)DEFAULT_HEIGHT), 60.0f);
    if (argc > 1)
        new MeshViewer(argv[1]);
    else
        new MeshViewer();
    app.Run();

    return EXIT_SUCCESS;
}

