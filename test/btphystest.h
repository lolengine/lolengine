//
// BtPhysTest
//
// Copyright: (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2012 Sam Hocevar <sam@hocevar.net>
//

#if !defined __BTPHYSTEST_H__
#define __BTPHYSTEST_H__

class BtPhysTest : public WorldEntity
{
public:
    BtPhysTest(bool editor = false);
    virtual ~BtPhysTest();

    char const *GetName() { return "<BtPhysTest>"; }

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

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

    Camera*                     m_camera;
    Controller *                m_controller;
    Light *                     m_light1;
    Light *                     m_light2;
    bool                        m_ready;

    lol::phys::Simulation*      m_simulation;
    Array<EasyConstraint*>      m_constraint_list;
    Array<PhysicsObject*>       m_physobj_list;
    Array<PhysicsObject*>       m_ground_list;
    Array<PhysicsObject*>       m_platform_list;
    Array<PhysicsObject*>       m_character_list;
    Array<PhysicsObject*>       m_stairs_list;

    float                       m_loop_value;
};

#endif // __BTPHYSTEST_H__

