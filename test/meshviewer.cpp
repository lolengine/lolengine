
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

LOLFX_RESOURCE_DECLARE(shinyfur);
LOLFX_RESOURCE_DECLARE(shinymvtexture);

enum
{
    KEY_CAM_RESET,
    KEY_CAM_FORWARD,
    KEY_CAM_BACKWARD,
    KEY_CAM_ZOOM_OUT,
    KEY_CAM_ZOOM_IN,

    KEY_MESH_UPDATE,
    KEY_MESH_RESET,
    KEY_MESH_PREV,
    KEY_MESH_NEXT,

    KEY_MESH_LEFT,
    KEY_MESH_RIGHT,
    KEY_MESH_UP,
    KEY_MESH_DOWN,
    KEY_MESH_SCALE_UP,
    KEY_MESH_SCALE_DOWN,
    KEY_MESH_OFFSET_UP,
    KEY_MESH_OFFSET_DOWN,
    KEY_MESH_ROT_LEFT,
    KEY_MESH_ROT_RIGHT,
    KEY_MESH_ROT_UP,
    KEY_MESH_ROT_DOWN,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_ESC,

    KEY_MAX,
};

#define    MIN_FOV                0.1f

#define    WITH_FUR               0
#define    WITH_TEXTURE           0

class MeshViewer : public WorldEntity
{
public:
    void SetFov(float new_fov=60.0f, vec2 video_size = vec2(Video::GetSize()))
    {
        if (new_fov > MIN_FOV)
            g_scene->GetCamera()->SetProjection(mat4::perspective(new_fov, video_size.x, video_size.y, .1f, 1000.f));
        else
            g_scene->GetCamera()->SetProjection(mat4::ortho(video_size.x, video_size.y, .1f, 1000.f));
    }

    MeshViewer(char const *file_name = "data/mesh-buffer.txt")
      : m_file_name(file_name)
    {
        /* Register an input controller for the keyboard */
        m_controller = new Controller("Default", KEY_MAX, 0);

        m_controller->GetKey(KEY_CAM_RESET).Bind("Keyboard", "Return");
        m_controller->GetKey(KEY_CAM_ZOOM_IN).Bind("Keyboard", "PageUp");
        m_controller->GetKey(KEY_CAM_ZOOM_OUT).Bind("Keyboard", "PageDown");

        m_controller->GetKey(KEY_MESH_LEFT).Bind("Keyboard", "Left");
        m_controller->GetKey(KEY_MESH_RIGHT).Bind("Keyboard", "Right");
        m_controller->GetKey(KEY_MESH_UP).Bind("Keyboard", "Up");
        m_controller->GetKey(KEY_MESH_DOWN).Bind("Keyboard", "Down");

        m_controller->GetKey(KEY_MESH_UPDATE).Bind("Keyboard", "Space");
        m_controller->GetKey(KEY_MESH_RESET).Bind("Keyboard", "KP0");
        m_controller->GetKey(KEY_MESH_PREV).Bind("Keyboard", "KPPlus");
        m_controller->GetKey(KEY_MESH_NEXT).Bind("Keyboard", "KPMinus");

        m_controller->GetKey(KEY_MESH_OFFSET_DOWN).Bind("Keyboard", "KP1");
        m_controller->GetKey(KEY_MESH_OFFSET_UP).Bind("Keyboard", "KP3");
        m_controller->GetKey(KEY_MESH_SCALE_DOWN).Bind("Keyboard", "KP7");
        m_controller->GetKey(KEY_MESH_SCALE_UP).Bind("Keyboard", "KP9");

        m_controller->GetKey(KEY_MESH_ROT_LEFT).Bind("Keyboard", "KP4");
        m_controller->GetKey(KEY_MESH_ROT_RIGHT).Bind("Keyboard", "KP6");
        m_controller->GetKey(KEY_MESH_ROT_UP).Bind("Keyboard", "KP8");
        m_controller->GetKey(KEY_MESH_ROT_DOWN).Bind("Keyboard", "KP5");

        m_controller->GetKey(KEY_F1).Bind("Keyboard", "F1");
        m_controller->GetKey(KEY_F2).Bind("Keyboard", "F2");
        m_controller->GetKey(KEY_F3).Bind("Keyboard", "F3");
        m_controller->GetKey(KEY_F4).Bind("Keyboard", "F4");
        m_controller->GetKey(KEY_F5).Bind("Keyboard", "F5");
        m_controller->GetKey(KEY_ESC).Bind("Keyboard", "Escape");

        // State
        m_mesh_shown = 0;
        m_angle = 0;
        m_default_texture = NULL;

        //Camera Setup
        m_fov_zoom_damp = .0f;
        m_fov_damp = 60.0f;
        m_fov = 60.0f;
        m_camera = new Camera();
        SetFov(m_fov_damp);
        m_camera->SetView(vec3(0.f, 0.f, 10.f),
                          vec3(0.f, 0.f, 0.f),
                          vec3(0.f, 1.f, 0.f));
        g_scene->PushCamera(m_camera);

        //Lights setup
        m_lights << new Light();
        m_lights.Last()->SetPosition(vec4(4.f, -1.f, -4.f, 0.f));
        m_lights.Last()->SetColor(vec4(.0f, .2f, .5f, 1.f));
        Ticker::Ref(m_lights.Last());

        m_lights << new Light();
        m_lights.Last()->SetPosition(vec4(8.f, 2.f, 6.f, 1.f));
        m_lights.Last()->SetColor(vec4(.5f, .3f, .0f, 1.f));
        Ticker::Ref(m_lights.Last());

        //Speed damp
        m_mesh_rotate_damp = vec2(.0f);
        m_mesh_screen_move_damp = vec2(.0f);
        m_mesh_move_damp = vec2(.0f);

        //Actual values
        SetDefaultMeshTransform();

        //Actual values damp
        m_mesh_rotation_damp = vec2(.0f);
        m_mesh_screen_offset_damp = vec2(.0f);
        m_mesh_offset_damp = vec2(.0f);


        m_mat = mat4::rotate(m_mesh_rotation.x, vec3(1, 0, 0)) *
                mat4::rotate(m_angle, vec3(0, 1, 0)) *
                mat4::rotate(m_mesh_rotation.y, vec3(0, 1, 0));

        m_stream_update_time = 2.0f;
        m_stream_update_timer = 1.0f;
    }

    ~MeshViewer()
    {
        g_scene->PopCamera(m_camera);
        for (int i = 0; i < m_lights.Count(); ++i)
            Ticker::Unref(m_lights[i]);
    }

    void SetDefaultMeshTransform()
    {
        m_mesh_rotation = vec2(25.0f, .0f);
        m_mesh_screen_offset = vec2(.54f, .0f);
        m_mesh_offset = vec2(-.64f, .07f);
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

        //--
        //Update Mesh BBox - Get the Min/Max needed
        //--
        vec2 screen_min_max[2] = { vec2(FLT_MAX), vec2(-FLT_MAX) };
        vec3 cam_min_max[2] = { vec3(FLT_MAX), vec3(-FLT_MAX) };
        vec3 world_min_max[2] = { vec3(FLT_MAX), vec3(-FLT_MAX) };
        int mesh_id = m_meshes.Count() - 1;
        for (; mesh_id >= 0; mesh_id--)
            if (m_meshes[mesh_id].m2)
                break;

        mat4 world_cam = g_scene->GetCamera()->GetView();
        mat4 cam_screen = g_scene->GetCamera()->GetProjection();

        if (m_meshes.Count() && mesh_id >= 0)
        {
            for (int i = 0; i < m_meshes[mesh_id].m1.GetVertexCount(); i++)
            {
                //--
                mat4 LocalPos = m_mat * mat4::translate(m_meshes[mesh_id].m1.GetVertexLocation(i));
                vec3 vpos = LocalPos.v3.xyz;

                world_min_max[0] = min(vpos.xyz, world_min_max[0]);
                world_min_max[1] = max(vpos.xyz, world_min_max[1]);

                //--
                LocalPos = world_cam * LocalPos;
                vpos = LocalPos.v3.xyz;

                cam_min_max[0] = min(vpos.xyz, cam_min_max[0]);
                cam_min_max[1] = max(vpos.xyz, cam_min_max[1]);

                //--
                LocalPos = cam_screen * LocalPos;
                vpos = (LocalPos.v3 / LocalPos.v3.w).xyz;

                screen_min_max[0] = min(vpos.xy, screen_min_max[0]);
                screen_min_max[1] = max(vpos.xy, screen_min_max[1]);
            }
        }
        else
        {
            world_min_max[1] = vec3(.0f);
            world_min_max[0] = vec3(.0f);
            cam_min_max[1] = vec3(.0f);
            cam_min_max[0] = vec3(.0f);
            screen_min_max[0] = vec2(.0f);
            screen_min_max[1] = vec2(.0f);
        }
        //[0] : center, [1] : size.
        vec3 BBox[2] = { vec3(.0f), vec3(.0f) };
        BBox[1] = world_min_max[1] - world_min_max[0];
        BBox[0] = world_min_max[0] + BBox[1] * .5f;
        vec3 BBox_mod = BBox[1];
#if 0

        //--
        //Camera movement handling
        //--
        if (m_controller->GetKey(KEY_CAM_RESET).IsReleased())
            SetFov();

        //Auto Fov
        float local_max = max(max(lol::abs(world_min_max[0].x), lol::abs(world_min_max[0].y)),
                          max(    lol::abs(world_min_max[1].x), lol::abs(world_min_max[1].y)));
        float fov_ratio = max(max(lol::abs(screen_min_max[0].x), lol::abs(screen_min_max[0].y)),
                              max(lol::abs(screen_min_max[1].x), lol::abs(screen_min_max[1].y)));

        //Fov modification
        float fov_zoom = (float)(Input::GetStatus(IPT_CAM_ZOOM_OUT) - Input::GetStatus(IPT_CAM_ZOOM_IN));
        m_fov_zoom_damp = damp(m_fov_zoom_damp, fov_zoom, (fov_zoom == .0f)?(.15f):(0.5f), seconds);
        m_fov = max(.0f, m_fov + seconds * 10.0f * m_fov_zoom_damp);
        m_fov_damp = damp(m_fov_damp, m_fov, .2f, seconds);

        if (m_fov_damp < MIN_FOV)
        {
            vec2 tmp = vec2(Video::GetSize());
            SetFov(0, vec2(local_max * 2.2f) * (tmp / vec2(tmp.y)));
        }
        else
            SetFov(m_fov_damp);

        //Move modification
        vec3 campos = g_scene->GetCamera()->GetPosition();
        if (m_fov_damp < MIN_FOV)
            g_scene->GetCamera()->SetView(vec3(campos.xy, 10.f), quat(1.f));
        else if (fov_ratio > .0f)
            g_scene->GetCamera()->SetView(vec3(campos.xy, campos.z * fov_ratio * 1.1f), quat(1.f));
#else
        Camera* cur_cam = g_scene->GetCamera();
        vec3 min_max_diff = (cam_min_max[1] - cam_min_max[0]);
        float screen_size = max(max(lol::abs(min_max_diff.x), lol::abs(min_max_diff.y)),
                            max(    lol::abs(min_max_diff.x), lol::abs(min_max_diff.y)));
        float fov_ratio = max(max(lol::abs(screen_min_max[0].x), lol::abs(screen_min_max[0].y)),
                              max(lol::abs(screen_min_max[1].x), lol::abs(screen_min_max[1].y)));

        float fov_zoom = 0.f;
        fov_zoom += m_controller->GetKey(KEY_CAM_ZOOM_OUT).IsDown() ? 1.f : 0.f;
        fov_zoom -= m_controller->GetKey(KEY_CAM_ZOOM_IN).IsDown() ? 1.f : 0.f;
        m_fov_zoom_damp = damp(m_fov_zoom_damp, fov_zoom, (fov_zoom == .0f)?(.15f):(0.5f), seconds);
        m_fov = max(.0f, m_fov + seconds * 10.0f * m_fov_zoom_damp);
        m_fov_damp = damp(m_fov_damp, m_fov, .2f, seconds);

        if (m_fov_damp < MIN_FOV)
            cur_cam->SetProjection(mat4::ortho(screen_size * fov_ratio * 1.1f, 1600.f / 600.f, 1000.f));
        else if (fov_ratio > .0f)
            cur_cam->SetProjection(mat4::shifted_perspective(m_fov_damp, screen_size * fov_ratio * 1.1f, 1600.f / 600.f, 1000.f));

        vec3 cam_center = cam_min_max[0] + min_max_diff * .5f;

        vec4 test = inverse(world_cam) * vec4(.0f,.0f,-1.0f,1.f);
        test = test;
        test = inverse(world_cam) * vec4(.0f,.0f,.0f,1.f);
        test = inverse(world_cam) * vec4(.0f,.0f,1.0f,1.f);

        vec3 eye = (inverse(world_cam) * vec4(vec3(cam_center.xy, cam_min_max[1].z), 1.f)).xyz;
        vec3 target = (inverse(world_cam) * vec4(vec3(cam_center.xy, cam_min_max[0].z), 1.f)).xyz;
        if (eye == target)
            cur_cam->SetView(vec3(.0f), vec3(.0f, .0f, -1.f), vec3(0.f, 1.f, 0.f));
        else
            cur_cam->SetView(eye, target, vec3(0,1,0));
#endif

        //--
        //Mesh movement handling
        //--
        if (m_controller->GetKey(KEY_MESH_RESET).IsReleased())
            SetDefaultMeshTransform();

        m_mesh_shown += m_controller->GetKey(KEY_MESH_NEXT).IsReleased() ? 1 : 0;
        m_mesh_shown -= m_controller->GetKey(KEY_MESH_PREV).IsReleased() ? 1 : 0;
        m_mesh_shown = clamp(m_mesh_shown, 0, max(m_meshes.Count(), 1) - 1);

        vec2 new_move = vec2(.0f);
        new_move.x += m_controller->GetKey(KEY_MESH_RIGHT).IsDown() ? 1.f : 0.f;
        new_move.x -= m_controller->GetKey(KEY_MESH_LEFT).IsDown() ? 1.f : 0.f;
        new_move.y += m_controller->GetKey(KEY_MESH_UP).IsDown() ? 1.f : 0.f;
        new_move.y -= m_controller->GetKey(KEY_MESH_DOWN).IsDown() ? 1.f : 0.f;

        m_mesh_screen_move_damp = vec2(damp(m_mesh_screen_move_damp.x, new_move.x, (new_move.x == .0f)?(.15f):(0.5f), seconds),
                                         damp(m_mesh_screen_move_damp.y, new_move.y, (new_move.y == .0f)?(.15f):(0.5f), seconds));

        new_move.x = m_controller->GetKey(KEY_MESH_OFFSET_UP).IsDown() ? 1.f : 0.f;
        new_move.x -= m_controller->GetKey(KEY_MESH_OFFSET_DOWN).IsDown() ? 1.f : 0.f;
        new_move.y = m_controller->GetKey(KEY_MESH_SCALE_UP).IsDown() ? 1.f : 0.f;
        new_move.y -= m_controller->GetKey(KEY_MESH_SCALE_DOWN).IsDown() ? 1.f : 0.f;
        m_mesh_move_damp = vec2(damp(m_mesh_move_damp.x, new_move.x, (new_move.x == .0f)?(.15f):(0.5f), seconds),
                                  damp(m_mesh_move_damp.y, new_move.y, (new_move.y == .0f)?(.15f):(0.5f), seconds));

        new_move.x = m_controller->GetKey(KEY_MESH_ROT_UP).IsDown() ? 1.f : 0.f;
        new_move.x -= m_controller->GetKey(KEY_MESH_ROT_DOWN).IsDown() ? 1.f : 0.f;
        new_move.y = m_controller->GetKey(KEY_MESH_ROT_RIGHT).IsDown() ? 1.f : 0.f;
        new_move.y -= m_controller->GetKey(KEY_MESH_ROT_LEFT).IsDown() ? 1.f : 0.f;
        m_mesh_rotate_damp = vec2(damp(m_mesh_rotate_damp.x, new_move.x, (new_move.x == .0f)?(.15f):(0.5f), seconds),
                                    damp(m_mesh_rotate_damp.y, new_move.y, (new_move.y == .0f)?(.15f):(0.5f), seconds));

        vec2 mesh_screen_move = seconds * 0.6f * m_mesh_screen_move_damp;
        vec2 mesh_offset_move = seconds * vec2(.6f, .2f) * m_mesh_move_damp;
        vec2 mesh_rotate = seconds * vec2(40.0f, 60.0f) * m_mesh_rotate_damp;

        //Add movement
        m_mesh_screen_offset += mesh_screen_move;
        m_mesh_offset += mesh_offset_move;
        m_mesh_rotation += mesh_rotate;

        //Compute damp
        m_mesh_rotation_damp = damp(m_mesh_rotation_damp, m_mesh_rotation, .2f, seconds);
        m_mesh_screen_offset_damp = damp(m_mesh_screen_offset_damp, m_mesh_screen_offset, .2f, seconds);
        m_mesh_offset_damp = damp(m_mesh_offset_damp, m_mesh_offset, .2f, seconds);

        //Clamp necessary
        m_mesh_rotation.x = clamp(m_mesh_rotation.x, -90.0f, 90.0f);
        m_mesh_offset.y = max(.0f, m_mesh_offset.y);

        //m_angle += seconds * 70.0f;
        m_mat = mat4::rotate(m_mesh_rotation.x, vec3(1, 0, 0)) *
                mat4::rotate(m_angle, vec3(0, 1, 0)) *
                mat4::rotate(m_mesh_rotation.y, vec3(0, 1, 0));

        //--
        //File management
        //--
        if (m_controller->GetKey(KEY_MESH_UPDATE).IsReleased())
            m_stream_update_time = m_stream_update_timer + 1.0f;
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

                //Create a new mesh
                m_meshes.Push(EasyMesh(), false, .0f, vec3(.0f));
                if (!m_meshes.Last().m1.Compile(cmd.C()))
                    m_meshes.Pop();
                //else
                //    m_meshes.Last().m1.ComputeTexCoord(0.2f, 2);
            }
        }
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
            //m_image = new Image("data/test-texture.png");
            m_default_texture = Tiler::Register("data/test-texture.png", ivec2(0), ivec2(0,1));

            //ivec2 size = m_image->GetSize();
            //// m_image->GetFormat()
            //m_texture = new Texture(m_image->GetSize(), PixelFormat::ABGR_8);
            //m_texture->SetData(m_image->GetData());
            // PixelFormat::ABGR_8
        }
        else if (m_texture && m_default_texture)
            m_texture_shader->SetUniform(m_texture_uni, m_default_texture->GetTexture(), 0);

        for (int i = 0; i < m_meshes.Count(); i++)
        {
            if (!m_meshes[i].m2)
            {
                //Fur support
#if WITH_FUR
                m_meshes[i].m1.MeshConvert(Shader::Create(LOLFX_RESOURCE_NAME(shinyfur)));
#elif WITH_TEXTURE
                //m_meshes[i].m1.MeshConvert(m_texture_shader);
                //m_meshes[i].m1.MeshConvert();
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

        mat4 default_proj = g_scene->GetCamera()->GetProjection();
        int max_drawn = m_meshes.Count() - m_mesh_shown;
        for (int i = max_drawn; i < m_meshes.Count(); i++)
            m_meshes[i].m4 = vec3(.0f);
        for (int i = 0; i < max_drawn; i++)
        {
            float new_scale = max(.0f, 1.0f - (m_mesh_offset_damp.y * (float)(max_drawn - (i + 1))));
            m_meshes[i].m3 = damp(m_meshes[i].m3, new_scale, .35f, seconds);
            if (m_meshes[i].m3 > .0f)
            {
                vec3 new_mesh_offset = vec3(.0f);
                //damping calculations
                for (int j = max_drawn - 1; j > i; j--)
                {
                    float ofs_scale = max(.0f, 1.0f - (m_mesh_offset_damp.y * (float)(max_drawn - (j + 0))));
                    new_mesh_offset = new_mesh_offset + vec3(m_meshes[j].m3 * ofs_scale * ofs_scale * m_mesh_offset_damp.x, .0f, .0f);
                }
                m_meshes[i].m4 = damp(m_meshes[i].m4, new_mesh_offset, .35f, seconds);

                g_scene->GetCamera()->SetProjection(
                                                                mat4::translate(m_meshes[i].m4) *
                                                                mat4::translate(vec3(m_mesh_screen_offset_damp, .0f)) *
                                                                mat4::scale(vec3(vec2(m_meshes[i].m3), 1.0f)) *
                                                                default_proj);
#if WITH_FUR
                for (int j=0; j < 40; j++)
                    m_meshes[i].m1.Render(m_mat, 0.1 * j);
#else
                m_meshes[i].m1.Render(m_mat);
#endif
                g_renderer->Clear(ClearMask::Depth);
            }
        }
        g_scene->GetCamera()->SetProjection(default_proj);
    }

private:
    Controller *m_controller;
    Camera *m_camera;
    float m_angle;
    mat4 m_mat;

    //Mesh infos
    //Move damping
    vec2 m_mesh_rotate_damp;
    vec2 m_mesh_screen_move_damp;
    vec2 m_mesh_move_damp;

    //Move transform damping
    vec2 m_mesh_rotation_damp;
    vec2 m_mesh_screen_offset_damp;
    vec2 m_mesh_offset_damp;

    vec2 m_mesh_rotation;     //Meshes rotation
    vec2 m_mesh_screen_offset;//Meshes screen offset
    vec2 m_mesh_offset;       //Mesh Offset after first mesh (x: offset, y: scale)
    int  m_mesh_shown;

    //File data
    String        m_file_name;
    Array<String> m_cmdlist;
    float         m_stream_update_time;
    float         m_stream_update_timer;

    //misc datas
    Array<EasyMesh, bool, float, vec3> m_meshes;
    Array<Light *>  m_lights;
    Shader *        m_texture_shader;
    TileSet *       m_default_texture;
    Texture *       m_texture;
    ShaderUniform   m_texture_uni;
    Image *         m_image;
    float           m_fov;
    float           m_fov_damp;
    float           m_fov_zoom_damp;
    mat4            m_fov_compensation;
};

//The basic main :
int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("MeshViewer", ivec2(1600, 600), 60.0f);
    if (argc > 1)
        new MeshViewer(argv[1]);
    else
        new MeshViewer();
    app.Run();

    return EXIT_SUCCESS;
}

