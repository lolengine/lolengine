//
//  Copyright © 2009-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

namespace lol
{

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

//MeshViewerInput ---------------------------------------------------------
struct MeshViewerKeyInputBase : public StructSafeEnum
{
    enum Type
    {
        KBD_BEG = 0,

        Exit = KBD_BEG,

        KBD_END,

        MSE_BEG = KBD_END,

        LeftClick = MSE_BEG,
        RightClick,
        MiddleClick,
        Focus,

        MSE_END,

        MAX = MSE_END,
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[Exit] = g_name_key_Escape;

        enum_map[LeftClick] = g_name_mouse_key_left;
        enum_map[RightClick] = g_name_mouse_key_right;
        enum_map[MiddleClick] = g_name_mouse_key_middle;
        enum_map[Focus] = g_name_mouse_key_in_screen;

        return true;
    }
};
typedef SafeEnum<MeshViewerKeyInputBase> MeshViewerKeyInput;

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

//ViewerObject ----------------------------------------------------------------
class ViewerObject
{
public:
    ViewerObject() { }
    ViewerObject(String const& name) : m_name(name) { }
    virtual ~ViewerObject() { }

    virtual void TickDraw(float seconds, Scene &scene) { }
    String GetName() { return m_name; }

protected:
    String m_name;
};

//EasyMeshViewerObject --------------------------------------------------------
class EasyMeshViewerObject : public ViewerObject
{
    typedef ViewerObject super;

public:
    EasyMeshViewerObject()
        : ViewerObject() { }
    EasyMeshViewerObject(String const& name, EasyMesh const& mesh)
        : ViewerObject(name)
    {
        Init(name, mesh);
    }
    virtual ~EasyMeshViewerObject() { }

    virtual void TickDraw(float seconds, Scene &scene);

    void Init(String const& name, EasyMesh const& mesh)
    {
        m_name = name;
        m_mesh = mesh;
    }

protected:
    EasyMesh m_mesh;
};

//MeshViewerLoadJob -----------------------------------------------------------
class MeshViewerLoadJob : public ThreadJob
{
    friend class BaseThreadManager;
    typedef ThreadJob super;

public:
    inline MeshViewerLoadJob() : ThreadJob() { }
    inline MeshViewerLoadJob(String const& path)
        : ThreadJob(ThreadJobType::WORK_TODO), m_path(path) { }
    virtual ~MeshViewerLoadJob() { }

    virtual void RetrieveResult(class MeshViewer* app) { }

protected:
    virtual bool DoWork() { return super::DoWork(); }

protected:
    String m_path;
};

//EasyMeshLoadJob -------------------------------------------------------------
class EasyMeshLoadJob : public MeshViewerLoadJob
{
    friend class BaseThreadManager;
    typedef EasyMeshLoadJob super;

public:
    inline EasyMeshLoadJob() : MeshViewerLoadJob() { }
    inline EasyMeshLoadJob(String const& path)
        : MeshViewerLoadJob(path) { }
    virtual ~EasyMeshLoadJob() { }

    static MeshViewerLoadJob* GetInstance(String const& path);
    virtual void RetrieveResult(class MeshViewer* app);

protected:
    static bool Check(String const& path) { return path.contains(".easymesh"); }
    virtual bool DoWork();

protected:
    EasyMeshLuaLoader m_loader;
    array<EasyMeshViewerObject*> m_meshes;
};

//MeshViewer ------------------------------------------------------------------
class MeshViewer : public WorldEntity
{
    typedef WorldEntity super;
public:
    MeshViewer(char const *file_name = "../data/meshviewer.easymesh.lua");
    ~MeshViewer();

    void Start();
    void Stop();

    void UpdateSceneSetup(bool only_destroy = false);

    MeshViewerLoadJob* GetLoadJob(String const& path);
    void AddViewerObj(ViewerObject* obj) { m_objs << obj; }

    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, Scene &scene);

    bool KeyReleased(MVKeyboardList index);
    bool KeyPressed(MVKeyboardList index);
    bool KeyDown(MVKeyboardList index);
    bool KeyReleased(MVMouseKeyList index);
    bool KeyPressed(MVMouseKeyList index);
    bool KeyDown(MVMouseKeyList index);
    float AxisValue(MVMouseAxisList index);

    void Prepare();
    void Unprepare();
    void Update(float seconds);
    void Draw(float seconds, Scene &scene);

private:
    //Main stuff --------------------------------------------------------------
    bool                    m_init = false;
    bool                    m_first_tick = false;
    InputProfile            m_profile;
    Camera*                 m_camera = nullptr;
    Text*                   m_text = nullptr;

    //ImGui stuff
    bool                    m_menu_cam_useage = true;
    float                   m_menu_cam_fov = 40.f;
    vec3                    m_menu_cam_pos = vec3(20.f, 45.f, 45.f);
    int                     m_menu_mesh_idx = 0;
    array<char*>            m_menu_mesh_names_char;
    array<String>           m_menu_mesh_names_str;

    //Scene setup data
    SceneSetupLuaLoader     m_ssetup_loader;
    FileUpdateStatus*       m_ssetup_file_status = nullptr;
    String                  m_ssetup_file_name;
    String                  m_ssetup_name;
    map<String, SceneSetup*> m_ssetups;

    //File data
    String                  m_file_name;
    FileUpdateStatus*       m_file_status;

    //Object data
    array<ViewerObject*>    m_objs;

    //Entities listing
    array<Entity*>          m_entities;

    //Input
    Controller*             m_controller = nullptr;

    //Thread stuff
    FileUpdateTester*       m_file_check = nullptr;
    DefaultThreadManager*   m_file_loader = nullptr;

    //OLD ---------------------------------------------------------------------
    SceneSetup*             m_ssetup = nullptr;
    array<LightData>    m_light_datas;
    short               m_input_usage;
    mat4                m_mat;
    mat4                m_mat_prev;

    //Camera Setup
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
    array<String>       m_cmdlist;
    float               m_stream_update_time;
    float               m_stream_update_timer;

    //misc datas
    Shader *            m_texture_shader;
    TileSet *           m_default_texture;
    Texture *           m_texture;
    ShaderUniform       m_texture_uni;
};

} /* namespace lol */
