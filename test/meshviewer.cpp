
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

#include <cfloat> /* for FLT_MAX */

#include "core.h"
#include "scenesetup.h"

using namespace std;
using namespace lol;

static int const TEXTURE_WIDTH = 256;

#define     R_M                 1.f
#define     DEFAULT_WIDTH       (770.f * R_M)
#define     DEFAULT_HEIGHT      (200.f * R_M)
#define     WIDTH               ((float)Video::GetSize().x)
#define     HEIGHT              ((float)Video::GetSize().y)
#define     SCREEN_W            (10.f / WIDTH)
#define     SCREEN_LIMIT        1.1f
#define     RATIO_HW            (HEIGHT / WIDTH)
#define     RATIO_WH            (WIDTH / HEIGHT)

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

#define     NO_NACL_EM          (!__native_client__ && !EMSCRIPTEN)
#define     NACL_EM             (__native_client__ || EMSCRIPTEN)

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

    MSE_MAX
};

enum MVMouseAxisList
{
    MSEX_CAM_Y = 0,
    MSEX_CAM_X,

    MSEX_MAX
};

#define     MAX_KEYS    MSE_MAX
#define     MAX_AXIS    MSEX_MAX

enum MessageType
{
    MSG_IN,
    MSG_OUT,

    MSG_MAX
};

class MeshViewer : public WorldEntity
{
public:
    MeshViewer(char const *file_name = "data/mesh-buffer.txt")
      : m_file_name(file_name)
    {
        m_init = false;
    }

    ~MeshViewer()
    {
        if (m_camera)
            g_scene->PopCamera(m_camera);
        if (m_ssetup)
            delete(m_ssetup);
        MessageService::Destroy();

        m_ssetup = nullptr;
        m_camera = nullptr;
    }

#if NO_NACL_EM
    bool  KeyReleased(MVKeyboardList index) { return (HAS_KBOARD && m_controller->GetKey(index).IsReleased()); }
    bool  KeyPressed(MVKeyboardList index)  { return (HAS_KBOARD && m_controller->GetKey(index).IsPressed()); }
    bool  KeyDown(MVKeyboardList index)     { return (HAS_KBOARD && m_controller->GetKey(index).IsDown()); }
    bool  KeyReleased(MVMouseKeyList index) { return (HAS_MOUSE  && m_controller->GetKey(index).IsReleased()); }
    bool  KeyPressed(MVMouseKeyList index)  { return (HAS_MOUSE  && m_controller->GetKey(index).IsPressed()); }
    bool  KeyDown(MVMouseKeyList index)     { return (HAS_MOUSE  && m_controller->GetKey(index).IsDown()); }
    float AxisValue(MVMouseAxisList index)  { return (HAS_MOUSE)?(m_controller->GetAxis(index).GetValue()):(0.f); }
#endif //NO_NACL_EM

    void Init()
    {
        m_init = true;
        m_input_usage = 0;

#if NO_NACL_EM
        /* Register an input controller for the keyboard */
        m_controller = new Controller("Default", MAX_KEYS, MAX_AXIS);

        if (InputDevice::Get("Mouse"))
        {
            m_input_usage |= (1<<IPT_MV_MOUSE);

            m_controller->GetKey(MSE_CAM_ROT).Bind("Mouse", "Left");
            m_controller->GetKey(MSE_CAM_POS).Bind("Mouse", "Right");
            m_controller->GetKey(MSE_CAM_FOV).Bind("Mouse", "Middle");
            m_controller->GetAxis(MSEX_CAM_Y).Bind("Mouse", "Y");
            m_controller->GetAxis(MSEX_CAM_X).Bind("Mouse", "X");
        }

        if (InputDevice::Get("Keyboard"))
        {
            m_input_usage |= (1<<IPT_MV_KBOARD);

            //Camera keyboard rotation
            m_controller->GetKey(KEY_CAM_UP   ).Bind("Keyboard", "Up");
            m_controller->GetKey(KEY_CAM_DOWN ).Bind("Keyboard", "Down");
            m_controller->GetKey(KEY_CAM_LEFT ).Bind("Keyboard", "Left");
            m_controller->GetKey(KEY_CAM_RIGHT).Bind("Keyboard", "Right");

            //Camera keyboard position switch
            m_controller->GetKey(KEY_CAM_POS  ).Bind("Keyboard", "LeftShift");
            m_controller->GetKey(KEY_CAM_FOV  ).Bind("Keyboard", "LeftCtrl");

            //Camera unzoom switch
            m_controller->GetKey(KEY_CAM_RESET).Bind("Keyboard", "Space");

            //Mesh change
            m_controller->GetKey(KEY_MESH_NEXT).Bind("Keyboard", "PageUp");
            m_controller->GetKey(KEY_MESH_PREV).Bind("Keyboard", "PageDown");

            //Base setup
            m_controller->GetKey(KEY_F1).Bind("Keyboard", "F1");
            m_controller->GetKey(KEY_F2).Bind("Keyboard", "F2");
            m_controller->GetKey(KEY_F3).Bind("Keyboard", "F3");
            m_controller->GetKey(KEY_F4).Bind("Keyboard", "F4");
            m_controller->GetKey(KEY_F5).Bind("Keyboard", "F5");
            m_controller->GetKey(KEY_ESC).Bind("Keyboard", "Escape");
        }
#endif //NO_NACL_EM

        // Message Service
        MessageService::Setup();

        // Mesh Setup
        m_render_max = vec2(-.9f, 6.1f);
        m_mesh_id = 0;
        m_mesh_id1 = 0.f;
        m_default_texture = NULL;

        //Camera Setup
        m_reset_timer = -1.f;
        m_fov = -100.f;
        m_fov_mesh = 0.f;
        m_fov_speed = 0.f;
        m_zoom = -100.f;
        m_zoom_mesh = 0.f;
        m_zoom_speed = 0.f;
        m_rot = vec2(45.f);
        m_rot_mesh = vec2::zero;
        m_rot_speed = vec2::zero;
        m_pos = vec2::zero;
        m_pos_mesh = vec2::zero;
        m_pos_speed = vec2::zero;
        m_screen_offset = vec2::zero;
        m_hist_scale = vec2(.13f, .03f);
        m_hist_scale_mesh = vec2(.0f);
        m_hist_scale_speed = vec2(.0f);

        m_camera = new Camera();
        m_camera->SetView(vec3(0.f, 0.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));
        m_camera->SetProjection(0.f, .0001f, 2000.f, WIDTH * SCREEN_W, RATIO_HW);
        m_camera->UseShift(true);
        g_scene->PushCamera(m_camera);

        //Lights setup
        m_ssetup = new SceneSetup();
        m_ssetup->Compile(" addlight 0.0 position (4 -1 -4) color (.0 .2 .5 1)"
                          " addlight 0.0 position (8 2 6) color #ffff"
                          " custom setmesh \"sc#fff ab 1\"");
        m_ssetup->Startup();

        //stream update
        m_stream_update_time = 2.0f;
        m_stream_update_timer = 1.0f;
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

        //TODO : This should probably be "standard LoL behaviour"
#if NO_NACL_EM
        {
            //Shutdown logic
            if (KeyReleased(KEY_ESC))
                Ticker::Shutdown();
        }
#endif //NO_NACL_EM

        //Mesh Change
#if NO_NACL_EM
        m_mesh_id = clamp(m_mesh_id + ((int)KeyPressed(KEY_MESH_PREV) - (int)KeyPressed(KEY_MESH_NEXT)), 0, m_meshes.Count() - 1);
#endif //NO_NACL_EM
        m_mesh_id1 = damp(m_mesh_id1, (float)m_mesh_id, .2f, seconds);

        //Camera update
        bool is_pos = false;
        bool is_fov = false;
        bool is_hsc = false;
        vec2 tmp = vec2::zero;

#if NO_NACL_EM
        is_pos = KeyDown(KEY_CAM_POS) || KeyDown(MSE_CAM_POS);
        is_fov = KeyDown(KEY_CAM_FOV) || KeyDown(MSE_CAM_FOV);

        if (KeyDown(MSE_CAM_ROT) || KeyDown(MSE_CAM_POS) || KeyDown(MSE_CAM_FOV))
        {
            tmp += vec2(AxisValue(MSEX_CAM_Y), AxisValue(MSEX_CAM_X));
            if (KeyDown(MSE_CAM_ROT))
                tmp *= 6.f;
            if (KeyDown(MSE_CAM_POS))
                tmp *= vec2(1.f, -1.f) * 3.f;
            if (KeyDown(MSE_CAM_FOV))
                tmp = vec2(tmp.y * 4.f, tmp.x * 6.f);
        }

        tmp += vec2((float)KeyDown(KEY_CAM_UP   ) - (float)KeyDown(KEY_CAM_DOWN),
                    (float)KeyDown(KEY_CAM_RIGHT) - (float)KeyDown(KEY_CAM_LEFT));
#endif //NO_NACL_EM

        //Base data
        vec2 rot = (!is_pos && !is_fov)?(tmp):(vec2(.0f)); rot = vec2(rot.x, rot.y);
        vec2 pos = ( is_pos && !is_fov)?(tmp):(vec2(.0f)); pos = -vec2(pos.y, pos.x);
        vec2 fov = (!is_pos && is_fov )?(tmp):(vec2(.0f)); fov = vec2(-fov.x, fov.y);
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

#if NO_NACL_EM
        if (m_reset_timer >= 0.f)
            m_reset_timer -= seconds;
        if (KeyPressed(KEY_CAM_RESET))
        {
            if (m_reset_timer >= 0.f)
            {
                m_pos = vec2(0.f);
                m_zoom = -100.f;
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
#endif //NO_NACL_EM

        //clamp
        vec2 rot_mesh = vec2(SmoothClamp(m_rot.x, -ROT_CLAMP, ROT_CLAMP, ROT_CLAMP * .1f), m_rot.y);
        vec2 pos_mesh = vec2(SmoothClamp(m_pos.x, -POS_CLAMP, POS_CLAMP, POS_CLAMP * .1f),
                             SmoothClamp(m_pos.y, -POS_CLAMP, POS_CLAMP, POS_CLAMP * .1f));
        float fov_mesh = SmoothClamp(m_fov, 0.f, FOV_CLAMP, FOV_CLAMP * .1f);
        float zoom_mesh = SmoothClamp(m_zoom, 0.f, ZOM_CLAMP, ZOM_CLAMP * .1f);
        vec2 hist_scale_mesh = vec2(SmoothClamp(m_hist_scale.x, 0.f, HST_CLAMP, HST_CLAMP * .1f),
                                    SmoothClamp(m_hist_scale.y, 0.f, HST_CLAMP, HST_CLAMP * .1f));

#if NO_NACL_EM
        if (KeyDown(KEY_CAM_RESET) && m_reset_timer < 0.f)
        {
            pos_mesh = vec2::zero;
            zoom_mesh = 0.f;
        }
#endif //NO_NACL_EM

        m_rot_mesh = vec2(damp(m_rot_mesh.x, rot_mesh.x, .2f, seconds), damp(m_rot_mesh.y, rot_mesh.y, .2f, seconds));
        m_pos_mesh = vec2(damp(m_pos_mesh.x, pos_mesh.x, .2f, seconds), damp(m_pos_mesh.y, pos_mesh.y, .2f, seconds));
        m_fov_mesh = damp(m_fov_mesh, fov_mesh, .2f, seconds);
        m_zoom_mesh = damp(m_zoom_mesh, zoom_mesh, .2f, seconds);
        m_hist_scale_mesh = damp(m_hist_scale_mesh, hist_scale_mesh, .2f, seconds);

        //Mesh mat calculation
        m_mat = mat4(quat::fromeuler_xyz(vec3(m_rot_mesh, .0f)));

        //Target List Setup
        Array<vec3> target_list;
        if (m_meshes.Count() && m_mesh_id >= 0)
            for (int i = 0; i < m_meshes[m_mesh_id]->GetVertexCount(); i++)
                target_list << (m_mat * mat4::translate(m_meshes[m_mesh_id]->GetVertexLocation(i))).v3.xyz;

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
        for (int i = 0; i < target_list.Count(); i++)
        {
            vec3 obj_loc = target_list[i];
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
        float screen_ratio = max(max(lol::abs(local_min_max[0].x), lol::abs(local_min_max[0].y)),
                                 max(lol::abs(local_min_max[1].x), lol::abs(local_min_max[1].y)));
        float scale_ratio = max(max(lol::abs(screen_min_max[0].x), lol::abs(screen_min_max[0].y)),
                                 max(lol::abs(screen_min_max[1].x), lol::abs(screen_min_max[1].y)));
        vec2 screen_offset = vec2(0.f, -(screen_min_max[1].y + screen_min_max[0].y) * .5f);
        m_screen_offset = damp(m_screen_offset, screen_offset, .9f, seconds);
        float z_pos = (inverse(world_cam) * mat4::translate(vec3(0.f, 0.f, max(local_min_max[0].z, local_min_max[1].z)))).v3.z;

        if (cam_factor > 0.f)
        {
            vec2 new_screen_scale = m_camera->GetScreenScale();
            m_camera->SetScreenScale(max(vec2(0.001f), new_screen_scale * ((1.0f + m_zoom_mesh) / (scale_ratio * SCREEN_LIMIT))));
            m_camera->m_position.z = damp(m_camera->m_position.z, z_pos + screen_ratio * 2.f, .1f, seconds);
            m_camera->SetFov(m_fov_mesh);
            m_camera->SetScreenInfos(damp(m_camera->GetScreenSize(), max(1.f, screen_ratio), 1.2f, seconds));
        }

        //--
        //Message Service
        //--
        String mesh("");
        int u = 4;
        while (u-- > 0 && MessageService::FetchFirst(MessageBucket::AppIn, mesh))
        {
            int o = 1;
            while (o-- > 0)
            {
                SceneSetup* new_ssetup = new SceneSetup();
                if (new_ssetup->Compile(mesh.C()))
                {
                    delete(m_ssetup);
                    m_ssetup = new_ssetup;
                    m_ssetup->Startup();
                    for (int i = 0; i < m_ssetup->m_custom_cmd.Count(); ++i)
                    {
                        if (m_ssetup->m_custom_cmd[i].m1 == "setmesh")
                        {
                            //Create a new mesh
                            EasyMesh* em = new EasyMesh();
                            if (em->Compile(m_ssetup->m_custom_cmd[i].m2.C()))
                            {
                                if (m_mesh_id == m_meshes.Count() - 1)
                                    m_mesh_id++;
                                m_meshes.Push(em);
                            }
                            else
                                delete(em);
                        }
                    }
                    m_ssetup->m_custom_cmd.Empty();
                }
            }
        }

#if NACL_EM
        if (m_stream_update_time > .0f)
        {
            m_stream_update_time = -1.f;
//            MessageService::Send(MessageBucket::AppIn, "[sc#f8f ab 1]");
//            MessageService::Send(MessageBucket::AppIn, "[sc#f8f ab 1 splt 4 twy 90]");
//            MessageService::Send(MessageBucket::AppIn, "[sc#8ff afcb 1 1 1 0]");
//            MessageService::Send(MessageBucket::AppIn, "[sc#ff8 afcb 1 1 1 0]");
        }
#elif WIN32
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
                cmd = String(" addlight 0.0 position (4 -1 -4) color (.0 .2 .5 1) \
                               addlight 0.0 position (8 2 6) color #ffff \
                               custom setmesh \"") + cmd + "\"";
                MessageService::Send(MessageBucket::AppIn, cmd);
            }
        }
#endif //WINDOWS
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        if (!m_init)
            return;

        //TODO : This should probably be "standard LoL behaviour"
#if NO_NACL_EM
        {
            if (KeyReleased(KEY_F1))
                Video::SetDebugRenderMode(DebugRenderMode::Default);
            if (KeyReleased(KEY_F2))
                Video::SetDebugRenderMode(DebugRenderMode::Wireframe);
            if (KeyReleased(KEY_F3))
                Video::SetDebugRenderMode(DebugRenderMode::Lighting);
            if (KeyReleased(KEY_F4))
                Video::SetDebugRenderMode(DebugRenderMode::Normal);
            if (KeyReleased(KEY_F5))
                Video::SetDebugRenderMode(DebugRenderMode::UV);
        }
#endif //NO_NACL_EM

#if NO_NACL_EM
        if (!m_default_texture)
        {
            m_texture_shader = Shader::Create(LOLFX_RESOURCE_NAME(shinymvtexture));
            m_texture_uni = m_texture_shader->GetUniformLocation("u_Texture");
            m_default_texture = Tiler::Register("data/test-texture.png", ivec2::zero, ivec2(0,1));
        }
        else if (m_texture && m_default_texture)
            m_texture_shader->SetUniform(m_texture_uni, m_default_texture->GetTexture(), 0);
#endif //NO_NACL_EM

        g_renderer->SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        vec3 x = vec3(1.f,0.f,0.f);
        vec3 y = vec3(0.f,1.f,0.f);
        for (int i = 0; i < m_meshes.Count(); i++)
        {
            {
                if (m_meshes[i]->GetMeshState() == MeshRender::NeedConvert)
                {
#if WITH_TEXTURE
                    m_meshes[i]->MeshConvert(new DefaultShaderData(((1 << VertexUsage::Position) | (1 << VertexUsage::Normal) |
                                                                    (1 << VertexUsage::Color)    | (1 << VertexUsage::TexCoord)),
                                                                    m_texture_shader, true));
#else
                    m_meshes[i]->MeshConvert();
#endif //WITH_TEXTURE
                }
                mat4 save_proj = m_camera->GetProjection();
                float j = -(float)(m_meshes.Count() - (i + 1)) + (-m_mesh_id1 + (float)(m_meshes.Count() - 1));

                if (m_mesh_id1 - m_render_max[0] > (float)i && m_mesh_id1 - m_render_max[1] < (float)i &&
                    m_meshes[i]->GetMeshState() > MeshRender::NeedConvert)
                {
                    float a_j = lol::abs(j);
                    float i_trans = (a_j * a_j * m_hist_scale_mesh.x + a_j * m_hist_scale_mesh.x) * .5f;
                    float i_scale = clamp(1.f - (m_hist_scale_mesh.y * (m_mesh_id1 - (float)i)), 0.f, 1.f);
                    mat4 new_proj =
                        //Y object Offset
                        mat4::translate(x * m_screen_offset.x + y * m_screen_offset.y) *
                        //Mesh Pos Offset
                        mat4::translate((x * m_pos_mesh.x * RATIO_HW + y * m_pos_mesh.y) * 2.f * (1.f + .5f * m_zoom_mesh / SCREEN_LIMIT)) *
                        //Mesh count offset
                        mat4::translate(x * RATIO_HW * 2.f * (j + i_trans)) *
                        //Align right meshes
                        mat4::translate(x - x * RATIO_HW) *
                        //Mesh count scale
                        mat4::scale(vec3(vec2(i_scale), 1.f)) *
                        //Camera projection
                        save_proj;
                    m_camera->SetProjection(new_proj);
//#if NO_NACL_EM
                    m_meshes[i]->Render(m_mat);
//#endif //NO_NACL_EM
                    g_renderer->Clear(ClearMask::Depth);
                }
                m_camera->SetProjection(save_proj);
            }
        }
    }

private:
    SceneSetup*         m_ssetup;
    short               m_input_usage;
    Controller*         m_controller;
    mat4                m_mat;
    bool                m_init;

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

    //Mesh infos
    vec2                m_render_max;
    int                 m_mesh_id;
    float               m_mesh_id1;
    Array<EasyMesh*>    m_meshes;

    //File data
    String              m_file_name;
    Array<String>       m_cmdlist;
    float               m_stream_update_time;
    float               m_stream_update_timer;

    //misc datas
    Shader *            m_texture_shader;
    TileSet *           m_default_texture;
    Texture *           m_texture;
    ShaderUniform       m_texture_uni;
    Image *             m_image;
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

