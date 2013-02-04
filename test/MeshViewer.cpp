
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

#define    IPT_CAM_RESET          "Cam_Center"
#define    IPT_CAM_FORWARD        "Cam_Forward"
#define    IPT_CAM_BACKWARD       "Cam_Backward"
#define    IPT_CAM_ZOOM_OUT       "Cam_Zoom_In"
#define    IPT_CAM_ZOOM_IN        "Cam_Zoom_Out"

#define    IPT_MESH_UPDATE        "Mesh_Update"
#define    IPT_MESH_RESET         "Mesh_Reset"

#define    IPT_MESH_LEFT          "Mesh_Left"
#define    IPT_MESH_RIGHT         "Mesh_Right"
#define    IPT_MESH_UP            "Mesh_Up"
#define    IPT_MESH_DOWN          "Mesh_Down"
#define    IPT_MESH_SCALE_UP      "Mesh_Scale_Up"
#define    IPT_MESH_SCALE_DOWN    "Mesh_Scale_Down"
#define    IPT_MESH_OFFSET_UP     "Mesh_Offset_Up"
#define    IPT_MESH_OFFSET_DOWN   "Mesh_Offset_Down"
#define    IPT_MESH_ROT_LEFT      "Mesh_Rot_Left"
#define    IPT_MESH_ROT_RIGHT     "Mesh_Rot_Right"
#define    IPT_MESH_ROT_UP        "Mesh_Rot_Up"
#define    IPT_MESH_ROT_DOWN      "Mesh_Rot_Down"

class MeshViewer : public WorldEntity
{
public:
    void SetFov(float new_fov=60.0f)
    {
        //FOV compensation doesn't work
        ivec2 video_size = Video::GetSize();
        float near = -video_size.x - video_size.y;
        float far = video_size.x + video_size.y;
        float t1 = tanf(new_fov / 2);
        float dist = video_size.x / (2.0f * t1);
        //m_fov_compensation = mat4::translate(-0.5f * video_size.x, -0.5f * video_size.y, -dist);
        m_fov_compensation = mat4::translate(vec3(.0f));
        if (new_fov > 0.1f)
            m_camera->SetPerspective(new_fov, (float)video_size.x, (float)video_size.y, .1f, 1000.f);
        else
            m_camera->SetOrtho((float)video_size.x, (float)video_size.y, .1f, 1000.f);
    }

    MeshViewer(char *file_buffer = "MeshViewerBuffer.txt")
    {
        //Input setup
        Input::LinkActionToKey(IPT_CAM_RESET,           Key::Return);
        Input::LinkActionToKey(IPT_CAM_FORWARD,         Key::PageUp);
        Input::LinkActionToKey(IPT_CAM_BACKWARD,        Key::PageDown);
        Input::LinkActionToKey(IPT_CAM_ZOOM_IN,         Key::Home);
        Input::LinkActionToKey(IPT_CAM_ZOOM_OUT,        Key::End);

        Input::LinkActionToKey(IPT_MESH_LEFT,           Key::Left);
        Input::LinkActionToKey(IPT_MESH_RIGHT,          Key::Right);
        Input::LinkActionToKey(IPT_MESH_UP,             Key::Up);
        Input::LinkActionToKey(IPT_MESH_DOWN,           Key::Down);

        Input::LinkActionToKey(IPT_MESH_UPDATE,         Key::Space);
        Input::LinkActionToKey(IPT_MESH_RESET,          Key::KP0);

        Input::LinkActionToKey(IPT_MESH_OFFSET_DOWN,    Key::KP1);
        Input::LinkActionToKey(IPT_MESH_OFFSET_UP,      Key::KP3);
        Input::LinkActionToKey(IPT_MESH_SCALE_DOWN,     Key::KP7);
        Input::LinkActionToKey(IPT_MESH_SCALE_UP,       Key::KP9);

        Input::LinkActionToKey(IPT_MESH_ROT_LEFT,       Key::KP4);
        Input::LinkActionToKey(IPT_MESH_ROT_RIGHT,      Key::KP6);
        Input::LinkActionToKey(IPT_MESH_ROT_UP,         Key::KP8);
        Input::LinkActionToKey(IPT_MESH_ROT_DOWN,       Key::KP5);
        
        m_angle = 0;

        //Camera Setup
        m_fov_zoom_damp = .0f;
        m_fov_damp = 60.0f;
        m_fov = 60.0f;
        m_camera = new Camera(vec3(0.f, 600.f, 0.f),
                              vec3(0.f, 0.f, 0.f),
                              vec3(0, 1, 0));
        SetFov(m_fov_damp);
        m_camera->SetTarget(vec3(0.f, 0.f, 0.f));
        m_camera->SetPosition(vec3(0.f, 0.f, 10.f));
        m_camera->ForceSceneUpdate();
        Ticker::Ref(m_camera);

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

        //File management
#ifdef HAVE_STDIO_H
        m_file_buffer = fopen("MeshViewerBuffer.txt", "r");

        if (!m_file_buffer)
            Ticker::Shutdown();
#else //Full FAIL, kill the viewer
        Ticker::Shutdown();
#endif

        m_last_stream_size = 0;
        m_last_stream_cmd << 0;

        m_stream_update_time = 0.0f;
        m_stream_update_timer = 1.0f;
    }

    ~MeshViewer()
    {
        Ticker::Unref(m_camera);
        for (int i = 0; i < m_lights.Count(); ++i)
            Ticker::Unref(m_lights[i]);
#ifdef HAVE_STDIO_H
        fclose(m_file_buffer);
#endif
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

        //Shutdown logic
        if (Input::WasReleased(Key::Escape))
            Ticker::Shutdown();

        //Update Mesh BBox - Get the Min/Max needed
        vec3 min_max[2] = { vec3(FLT_MAX), vec3(-FLT_MAX) };
        int mesh_id = m_meshes.Count() - 1;
        for (; mesh_id >= 0; mesh_id--)
            if (m_meshes[mesh_id].m2)
                break;

        if (m_meshes.Count() && mesh_id >= 0)
        {
            for (int i = 0; i < m_meshes[mesh_id].m1.GetVertexCount(); i++)
            {
                vec3 vpos = m_meshes[mesh_id].m1.GetVertexLocation(i);
                min_max[0] = min(vpos.xyz, min_max[0]);
                min_max[1] = max(vpos.xyz, min_max[1]);
            }
        }
        else
        {
            min_max[0] = vec3(.0f);
            min_max[1] = vec3(.0f);
        }
        //[0] : center, [1] : size.
        vec3 BBox[2] = { vec3(.0f), vec3(.0f) };
        BBox[1] = min_max[1] - min_max[0];
        BBox[0] = min_max[0] + BBox[1] * .5f;
        vec3 BBox_mod = BBox[1];

        //--
        //Camera movement handling
        //--
        vec3 cam_move = BBox_mod * seconds *
                        vec3(.0f, .0f, (float)(Input::GetStatus(IPT_CAM_BACKWARD) - Input::GetStatus(IPT_CAM_FORWARD)));

        if (Input::WasReleased(IPT_CAM_RESET))
            SetFov();

        float fov_zoom = (float)(Input::GetStatus(IPT_CAM_ZOOM_OUT) - Input::GetStatus(IPT_CAM_ZOOM_IN));
        m_fov_zoom_damp = damp(m_fov_zoom_damp, fov_zoom, (fov_zoom == .0f)?(.15f):(0.5f), seconds);
        m_fov = max(.0f, m_fov + seconds * 10.0f * m_fov_zoom_damp);
        m_fov_damp = damp(m_fov_damp, m_fov, .2f, seconds);
        SetFov(m_fov_damp);

        m_camera->SetPosition(m_camera->GetPosition() + cam_move);
        m_camera->SetTarget(m_camera->GetPosition() + vec3(0, 0, -5.0f));

        //--
        //Mesh movement handling
        //--
        if (Input::WasReleased(IPT_MESH_RESET))
            SetDefaultMeshTransform();

        vec2 new_move = vec2(.0f);

        new_move = vec2((float)(Input::GetStatus(IPT_MESH_RIGHT) - Input::GetStatus(IPT_MESH_LEFT)),
                        (float)(Input::GetStatus(IPT_MESH_UP)    - Input::GetStatus(IPT_MESH_DOWN)));
        m_mesh_screen_move_damp = vec2(damp(m_mesh_screen_move_damp.x, new_move.x, (new_move.x == .0f)?(.15f):(0.5f), seconds),
                                         damp(m_mesh_screen_move_damp.y, new_move.y, (new_move.y == .0f)?(.15f):(0.5f), seconds));

        new_move = vec2((float)(Input::GetStatus(IPT_MESH_OFFSET_UP) - Input::GetStatus(IPT_MESH_OFFSET_DOWN)),
                        (float)(Input::GetStatus(IPT_MESH_SCALE_UP)  - Input::GetStatus(IPT_MESH_SCALE_DOWN)));
        m_mesh_move_damp = vec2(damp(m_mesh_move_damp.x, new_move.x, (new_move.x == .0f)?(.15f):(0.5f), seconds),
                                  damp(m_mesh_move_damp.y, new_move.y, (new_move.y == .0f)?(.15f):(0.5f), seconds));

        new_move = vec2((float)(Input::GetStatus(IPT_MESH_ROT_UP)    - Input::GetStatus(IPT_MESH_ROT_DOWN)),
                        (float)(Input::GetStatus(IPT_MESH_ROT_RIGHT) - Input::GetStatus(IPT_MESH_ROT_LEFT)));
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
        if (Input::WasReleased(IPT_MESH_UPDATE))
            m_stream_update_time = m_stream_update_timer + 1.0f;
        m_stream_update_time += seconds;

#ifdef HAVE_STDIO_H
        // obtain file size:
        fseek(m_file_buffer, 0 , SEEK_END);
        long new_stream_size = ftell(m_file_buffer);
        rewind(m_file_buffer);
#else
        long new_stream_size = 0;
#endif

        if (new_stream_size && (new_stream_size != m_last_stream_size || m_stream_update_time > m_stream_update_timer))
        {
            Array<char> new_stream_cmd;
            m_stream_update_time = .0f;

            //Reserve the needed datas & read the file
            new_stream_cmd.Resize((int)new_stream_size);
#ifdef HAVE_STDIO_H
            size_t result = fread(&new_stream_cmd[0], 1, new_stream_size, m_file_buffer);
#else
#endif

            //if the new cmd is different from the previous one
            if (new_stream_cmd.Count() != m_last_stream_cmd.Count() ||
                strcmp(&new_stream_cmd[0], &m_last_stream_cmd[0]))
            {
                //store the current datas
                m_last_stream_cmd  = new_stream_cmd;
                m_last_stream_size = new_stream_size;

                //Create a new mesh
                m_meshes.Push(EasyMesh(), false, .0f, vec3(.0f));
                m_meshes.Last().m1.Compile(&new_stream_cmd[0]);
            }
        }
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        for (int i = 0; i < m_meshes.Count(); i++)
        {
            if (!m_meshes[i].m2)
            {
                m_meshes[i].m1.MeshConvert();
                m_meshes[i].m2 = true;
            }
        }

        Video::SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        mat4 default_proj = Scene::GetDefault()->GetProjMatrix();
        for (int i = 0; i < m_meshes.Count(); i++)
        {
            float new_scale = max(.0f, 1.0f - (m_mesh_offset_damp.y * (float)(m_meshes.Count() - (i + 1))));
            m_meshes[i].m3 = damp(m_meshes[i].m3, new_scale, .35f, seconds);
            if (m_meshes[i].m3 > .0f)
            {
                vec3 new_mesh_offset = vec3(.0f);
                for (int j = m_meshes.Count() - 1; j > i; j--)
                {
                    float ofs_scale = max(.0f, 1.0f - (m_mesh_offset_damp.y * (float)(m_meshes.Count() - (j + 0))));
                    new_mesh_offset = new_mesh_offset + vec3(m_meshes[j].m3 * ofs_scale * ofs_scale * m_mesh_offset_damp.x, .0f, .0f);
                }
                m_meshes[i].m4 = damp(m_meshes[i].m4, new_mesh_offset, .35f, seconds);
                Scene::GetDefault()->SetProjMatrix(mat4::translate(m_meshes[i].m4) *
                                                   mat4::translate(vec3(m_mesh_screen_offset_damp, .0f)) *
                                                   mat4::scale(vec3(vec2(m_meshes[i].m3), 1.0f)) *
                                                   default_proj *
                                                   m_fov_compensation);
                m_meshes[i].m1.Render(m_mat);
                Video::Clear(ClearMask::Depth);
            }
        }
        Scene::GetDefault()->SetProjMatrix(default_proj);
    }

private:
    float           m_angle;
    mat4            m_mat;

    //Mesh infos
    //Move damping
    vec2            m_mesh_rotate_damp;
    vec2            m_mesh_screen_move_damp;
    vec2            m_mesh_move_damp;

    //Move transform damping
    vec2            m_mesh_rotation_damp;
    vec2            m_mesh_screen_offset_damp;
    vec2            m_mesh_offset_damp;  

    vec2            m_mesh_rotation;     //Meshes rotation
    vec2            m_mesh_screen_offset;//Meshes screen offset
    vec2            m_mesh_offset;       //Mesh Offset after first mesh (x: offset, y: scale)

    //File data
#ifdef HAVE_STDIO_H
    FILE *          m_file_buffer;
#endif
    long            m_last_stream_size;
    Array<char>     m_last_stream_cmd;
    float           m_stream_update_time;
    float           m_stream_update_timer;

    //misc datas
    Array<EasyMesh, bool, float, vec3> m_meshes;
    Array<Light *>  m_lights;
    Camera *        m_camera;
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

