//
// BtPhysTest
//
// Copyright: (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2012 Sam Hocevar <sam@hocevar.net>
//

#if !defined __BTPHYSTEST_H__
#define __BTPHYSTEST_H__

class Nacl_PhysTest : public WorldEntity
{
public:
    Nacl_PhysTest(bool editor = false);
    virtual ~Nacl_PhysTest();

    char const *GetName() { return "<Nacl_PhysTest>"; }

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, Scene &scene);

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
    Camera*                         m_camera;
    Controller *                    m_controller;
    Light *                         m_light1;
    Light *                         m_light2;
    bool                            m_ready;

    //lol::phys::Simulation*          m_simulation;
    //array<EasyConstraint*>          m_constraint_list;
    //array<PhysicsObject*, float>    m_physobj_list;
    //array<PhysicsObject*>           m_ground_list;
    //array<PhysicsObject*>           m_platform_list;
    //array<PhysicsObject*>           m_character_list;
    //array<PhysicsObject*>           m_stairs_list;

    float                           m_loop_value;
    float                           m_target_timer;
    int                             m_cam_target;
    float                           m_fov_dp;
    float                           m_loc_dp;
};

#endif // __BTPHYSTEST_H__

