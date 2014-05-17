//
// BtPhysTest
//
// Copyright: (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2012 Sam Hocevar <sam@hocevar.net>
//

#if !defined __BTPHYSTEST_H__
#define __BTPHYSTEST_H__

class CatShaderData : public GpuShaderData
{
    friend class CRenderer;

public:
    //---
    CatShaderData(uint32_t vert_decl_flags, Shader* shader);
    //---
    void SetupDefaultData();
    virtual void SetupShaderDatas(mat4 const &model);
    //--
    virtual lol::String GetInVertexName()   { return lol::String("in_vertex");   }
    virtual lol::String GetInNormalName()   { return lol::String("in_normal");   }
    virtual lol::String GetInColorName()    { return lol::String("in_color");    }
    virtual lol::String GetInTexCoordName() { return lol::String("in_texcoord"); }

    TextureUniform   m_tex_uniform;
    float           m_sprite_orientation;
    float           m_sprite_flip;
};

class BtPhysTest : public WorldEntity
{
public:
    BtPhysTest(bool editor = false);
    virtual ~BtPhysTest();

    char const *GetName() { return "<BtPhysTest>"; }

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

    void InitApp();

private:
    enum
    {
        KEY_MOVE_FORWARD,
        KEY_MOVE_BACK,
        KEY_MOVE_LEFT,
        KEY_MOVE_RIGHT,
        KEY_MOVE_UP,
        KEY_MOVE_DOWN,
        KEY_MOVE_JUMP,
        KEY_QUIT,
        KEY_MAX,
    };

    TileSet*                        m_cat_texture;
    Shader*                         m_cat_shader;
    CatShaderData*                  m_cat_sdata;
    Camera*                         m_camera;
    Controller *                    m_controller;
    Light *                         m_light1;
    Light *                         m_light2;
    int                             m_init_status;
    bool                            m_ready;

    lol::phys::Simulation*          m_simulation;
    Array<EasyConstraint*>          m_constraint_list;
    Array<PhysicsObject*, float>    m_physobj_list;
    Array<PhysicsObject*>           m_ground_list;
    Array<PhysicsObject*>           m_platform_list;
    Array<PhysicsObject*>           m_character_list;
    Array<PhysicsObject*>           m_stairs_list;

    float                           m_loop_value;
    float                           m_target_timer;
    int                             m_cam_target;
    float                           m_fov_dp;
    float                           m_loc_dp;
};

#endif // __BTPHYSTEST_H__

