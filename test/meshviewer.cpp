
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

using namespace std;
using namespace lol;

static int const TEXTURE_WIDTH = 256;

#define     R_M                 2.f
#define     WIDTH               (770.f * R_M)
#define     HEIGHT              (200.f * R_M)
#define     SCREEN_W            (10.f / WIDTH)
#define     SCREEN_LIMIT        1.1f
#define     RATIO_HW            (HEIGHT / WIDTH)
#define     RATIO_WH            (WIDTH / HEIGHT)

#define     ROT_SPEED           vec2(50.f)
#define     ROT_CLAMP           89.f
#define     POS_SPEED           vec2(1.2f)
#define     POS_CLAMP           1.f
#define     FOV_SPEED           20.f
#define     FOV_CLAMP           120.f
#define     ZOM_SPEED           3.f
#define     ZOM_CLAMP           20.f

#define     WITH_TEXTURE        0

LOLFX_RESOURCE_DECLARE(shinyfur);
LOLFX_RESOURCE_DECLARE(shinymvtexture);

enum
{
    KEY_CAM_RESET,
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

    KEY_MAX,
};

enum
{
    MSG_IN,
    MSG_OUT,

    MSG_MAX
} MessageType;

class MeshViewer : public WorldEntity
{
public:
    MeshViewer(char const *file_name = "data/mesh-buffer.txt")
      : m_file_name(file_name)
    {
        /* Register an input controller for the keyboard */
        m_controller = new Controller("Default", KEY_MAX, 0);

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

        // Message Service
        MessageService::Setup(MSG_MAX);

        // Mesh Setup
        m_mesh_id = 0;
        m_mesh_id1 = 0.f;
        m_default_texture = NULL;

        //Camera Setup
        m_fov = -100.f;
        m_fov_mesh = 0.f;
        m_fov_speed = 0.f;
        m_zoom = -100.f;
        m_zoom_mesh = 0.f;
        m_zoom_speed = 0.f;
        m_rot = vec2(0.f);
        m_rot_mesh = vec2(0.f);
        m_rot_speed = vec2(0.f);
        m_pos = vec2(0.f);
        m_pos_mesh = vec2(0.f);
        m_pos_speed = vec2(0.f);
        m_screen_offset = vec2(0.f);

        m_camera = new Camera();
        m_camera->SetView(vec3(0.f, 0.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));
        m_camera->SetProjection(0.f, .0001f, 2000.f, WIDTH * SCREEN_W, RATIO_HW);
        //m_camera->SetScreenScale(vec2(10.f));
        m_camera->UseShift(true);
        g_scene->PushCamera(m_camera);

        //Lights setup
        m_lights << new Light();
        m_lights.Last()->SetPosition(vec4(4.f, -1.f, -4.f, 1.f));
        m_lights.Last()->SetColor(vec4(.0f, .2f, .5f, 1.f));
        Ticker::Ref(m_lights.Last());

        m_lights << new Light();
        m_lights.Last()->SetPosition(vec4(8.f, 2.f, 6.f, 1.f));
        m_lights.Last()->SetColor(vec4(1.f, 1.f, 1.f, 1.f));
        Ticker::Ref(m_lights.Last());

        //stream update
        m_stream_update_time = 2.0f;
        m_stream_update_timer = 1.0f;
    }

    ~MeshViewer()
    {
        g_scene->PopCamera(m_camera);
        for (int i = 0; i < m_lights.Count(); ++i)
            Ticker::Unref(m_lights[i]);
        MessageService::Destroy();
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        //TODO : This should probably be "standard LoL behaviour"
        {
            //Shutdown logic
            if (m_controller->GetKey(KEY_ESC).IsReleased())
                Ticker::Shutdown();
        }

        //Mesh Change
        m_mesh_id = clamp(m_mesh_id + ((int)m_controller->GetKey(KEY_MESH_PREV).IsPressed() - (int)m_controller->GetKey(KEY_MESH_NEXT).IsPressed()), 0, m_meshes.Count() - 1);
        m_mesh_id1 = damp(m_mesh_id1, (float)m_mesh_id, .2f, seconds);

        //Camera update
        bool is_pos = m_controller->GetKey(KEY_CAM_POS).IsDown();
        bool is_fov = m_controller->GetKey(KEY_CAM_FOV).IsDown();

        vec2 tmp = vec2((float)m_controller->GetKey(KEY_CAM_UP      ).IsDown() - (float)m_controller->GetKey(KEY_CAM_DOWN).IsDown(),
                       ((float)m_controller->GetKey(KEY_CAM_RIGHT   ).IsDown() - (float)m_controller->GetKey(KEY_CAM_LEFT).IsDown()));

        //Base data
        vec2 rot = (!is_pos && !is_fov)?(tmp):(vec2(.0f)); rot = vec2(rot.x, rot.y);
        vec2 pos = (is_pos && !is_fov)?(tmp):(vec2(.0f)); pos = -vec2(pos.y, pos.x);
        vec2 fov = (!is_pos && is_fov)?(tmp):(vec2(.0f)); fov = vec2(-fov.x, fov.y);

        //speed
        m_rot_speed = damp(m_rot_speed, rot * ROT_SPEED, .2f, seconds);
        float pos_factor = 1.f / (1.f + m_zoom_mesh * .5f);
        m_pos_speed = damp(m_pos_speed, pos * POS_SPEED * pos_factor, .2f, seconds);
        float fov_factor = 1.f + lol::pow((m_fov_mesh / FOV_CLAMP) * 1.f, 2.f);
        m_fov_speed = damp(m_fov_speed, fov.x * FOV_SPEED * fov_factor, .2f, seconds);
        float zom_factor = 1.f + lol::pow((m_zoom_mesh / ZOM_CLAMP) * 1.f, 2.f);
        m_zoom_speed = damp(m_zoom_speed, fov.y * ZOM_SPEED, .2f, seconds);

        m_rot += m_rot_speed * seconds;

        //Transform update
        if (!m_controller->GetKey(KEY_CAM_RESET).IsDown())
        {
            m_pos += m_pos_speed * seconds;
            m_fov += m_fov_speed * seconds;
            m_zoom += m_zoom_speed * seconds;
        }

        //clamp
        vec2 rot_mesh = vec2(SmoothClamp(m_rot.x, -ROT_CLAMP, ROT_CLAMP, ROT_CLAMP * .1f), m_rot.y);
        vec2 pos_mesh = vec2(SmoothClamp(m_pos.x, -POS_CLAMP, POS_CLAMP, POS_CLAMP * .1f),
                             SmoothClamp(m_pos.y, -POS_CLAMP, POS_CLAMP, POS_CLAMP * .1f));
        float fov_mesh = SmoothClamp(m_fov, 0.f, FOV_CLAMP, FOV_CLAMP * .1f);
        float zoom_mesh = SmoothClamp(m_zoom, 0.f, ZOM_CLAMP, ZOM_CLAMP * .1f);

        if (m_controller->GetKey(KEY_CAM_RESET).IsDown())
        {
            pos_mesh = vec2(0.f);
            zoom_mesh = 0.f;
        }

        m_rot_mesh = vec2(damp(m_rot_mesh.x, rot_mesh.x, .2f, seconds), damp(m_rot_mesh.y, rot_mesh.y, .2f, seconds));
        m_pos_mesh = vec2(damp(m_pos_mesh.x, pos_mesh.x, .2f, seconds), damp(m_pos_mesh.y, pos_mesh.y, .2f, seconds));
        m_fov_mesh = damp(m_fov_mesh, fov_mesh, .2f, seconds);
        m_zoom_mesh = damp(m_zoom_mesh, zoom_mesh, .2f, seconds);

        //Mesh mat calculation
        m_mat = mat4(quat::fromeuler_xyz(vec3(m_rot_mesh, .0f)));

        //Target List Setup
        Array<vec3> target_list;
        if (m_meshes.Count() && m_mesh_id >= 0)
            for (int i = 0; i < m_meshes[m_mesh_id].m1.GetVertexCount(); i++)
                target_list << (m_mat * mat4::translate(m_meshes[m_mesh_id].m1.GetVertexLocation(i))).v3.xyz;

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
        while (u-- > 0 && MessageService::FetchFirst(MSG_IN, mesh))
        {
            int o = 1;
            while (o-- > 0)
            {
                if (m_mesh_id == m_meshes.Count() - 1)
                    m_mesh_id++;
                //Create a new mesh
                m_meshes.Push(EasyMesh(), false);
                if (!m_meshes.Last().m1.Compile(mesh.C()))
                    m_meshes.Pop();
                //else
                //    m_meshes.Last().m1.ComputeTexCoord(0.2f, 2);
            }
        }

#if __native_client__
        if (m_stream_update_time > .0f)
        {
            m_stream_update_time = -1.f;
            MessageService::Send(MSG_IN, "[sc#f8f afcb 1 1 1 0]");
            MessageService::Send(MSG_IN, "[sc#8ff afcb 1 1 1 0]");
            MessageService::Send(MSG_IN, "[sc#ff8 afcb 1 1 1 0]");
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

            for (int i = 0; i < cmd.Count() - 1; i++)
            {
                if (cmd[i] == '/' && cmd[i + 1] == '/')
                {
                    int j = i;
                    for (; j < cmd.Count(); j++)
                    {
                        if (cmd[j] == '\r' || cmd[j] == '\n')
                            break;
                    }
                    String new_cmd = cmd.Sub(0, i);
                    if (j < cmd.Count())
                        new_cmd += cmd.Sub(j, cmd.Count() - j);
                    cmd = new_cmd;
                    i--;
                }
            }

            if (cmd.Count()
                 && (!m_cmdlist.Count() || cmd != m_cmdlist.Last()))
            {
                m_cmdlist << cmd;
                MessageService::Send(MSG_IN, cmd);
            }
        }
#endif //WINDOWS
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        //TODO : This should probably be "standard LoL behaviour"
        {
            if (m_controller->GetKey(KEY_F1).IsReleased())
                Video::SetDebugRenderMode(DebugRenderMode::Default);
            if (m_controller->GetKey(KEY_F2).IsReleased())
                Video::SetDebugRenderMode(DebugRenderMode::Wireframe);
            if (m_controller->GetKey(KEY_F3).IsReleased())
                Video::SetDebugRenderMode(DebugRenderMode::Lighting);
            if (m_controller->GetKey(KEY_F4).IsReleased())
                Video::SetDebugRenderMode(DebugRenderMode::Normal);
            if (m_controller->GetKey(KEY_F5).IsReleased())
                Video::SetDebugRenderMode(DebugRenderMode::UV);
        }

        if (!m_default_texture)
        {
            m_texture_shader = Shader::Create(LOLFX_RESOURCE_NAME(shinymvtexture));
            m_texture_uni = m_texture_shader->GetUniformLocation("u_Texture");
            m_default_texture = Tiler::Register("data/test-texture.png", ivec2(0), ivec2(0,1));
        }
        else if (m_texture && m_default_texture)
            m_texture_shader->SetUniform(m_texture_uni, m_default_texture->GetTexture(), 0);

        for (int i = 0; i < m_meshes.Count(); i++)
        {
            if (!m_meshes[i].m2)
            {
#if WITH_TEXTURE
                m_meshes[i].m1.MeshConvert(new DefaultShaderData(((1 << VertexUsage::Position) | (1 << VertexUsage::Normal) |
                                                                  (1 << VertexUsage::Color)    | (1 << VertexUsage::TexCoord)),
                                                                  m_texture_shader, true));
#else
                m_meshes[i].m1.MeshConvert();
#endif
                m_meshes[i].m2 = true;
            }
        }

        g_renderer->SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        vec3 x = vec3(1.f,0.f,0.f);
        vec3 y = vec3(0.f,1.f,0.f);
        for (int i = 0; i < m_meshes.Count(); i++)
        {
            mat4 save_proj = m_camera->GetProjection();
            float j = -(float)(m_meshes.Count() - (i + 1)) + (-m_mesh_id1 + (float)(m_meshes.Count() - 1));
            if (m_meshes[i].m2)
            {
                mat4 new_proj =
                    //Y object Offset
                    mat4::translate(x * m_screen_offset.x + y * m_screen_offset.y) *
                    //Mesh Pos Offset
                    mat4::translate((x * m_pos_mesh.x * RATIO_HW + y * m_pos_mesh.y) * 2.f * (1.f + .5f * m_zoom_mesh / SCREEN_LIMIT)) *
                    //Mesh count offset
                    mat4::translate(x * RATIO_HW * 2.f * j) *
                    //Align right meshes
                    mat4::translate(x - x * RATIO_HW) *
                    //Mesh count scale
                    //mat4::scale(1.f - .2f * j * (1.f / (float)m_meshes.Count())) *
                    //Camera projection
                    save_proj;
                m_camera->SetProjection(new_proj);
                m_meshes[i].m1.Render(m_mat);
                g_renderer->Clear(ClearMask::Depth);
            }
            m_camera->SetProjection(save_proj);
        }
    }

private:
    Array<Light *> m_lights;
    Controller *m_controller;
    mat4 m_mat;

    //Camera Setup
    Camera *m_camera;
    float   m_fov;
    float   m_fov_mesh;
    float   m_fov_speed;
    float   m_zoom;
    float   m_zoom_mesh;
    float   m_zoom_speed;
    vec2    m_rot;
    vec2    m_rot_mesh;
    vec2    m_rot_speed;
    vec2    m_pos;
    vec2    m_pos_mesh;
    vec2    m_pos_speed;
    vec2    m_screen_offset;

    //Mesh infos
    int     m_mesh_id;
    float   m_mesh_id1;
    Array<EasyMesh, bool> m_meshes;

    //File data
    String        m_file_name;
    Array<String> m_cmdlist;
    float         m_stream_update_time;
    float         m_stream_update_timer;

    //misc datas
    Shader *        m_texture_shader;
    TileSet *       m_default_texture;
    Texture *       m_texture;
    ShaderUniform   m_texture_uni;
    Image *         m_image;
};

//The basic main :
int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("MeshViewer", ivec2((int)WIDTH, (int)HEIGHT), 60.0f);
    if (argc > 1)
        new MeshViewer(argv[1]);
    else
        new MeshViewer();
    app.Run();

    return EXIT_SUCCESS;
}

