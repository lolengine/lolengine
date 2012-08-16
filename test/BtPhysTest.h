//
// BtPhysTest
//
// Copyright: (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
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
    Camera*								m_camera;
    bool								m_ready;

	lol::phys::Simulation*				m_simulation;
	Array<EasyConstraint*>				m_constraint_list;
	Array<PhysicsObject*>				m_physobj_list;
	Array<PhysicsObject*>				m_ground_list;
	Array<PhysicsObject*>				m_platform_list;
	Array<PhysicsObject*>				m_character_list;

	float								m_loop_value;
};

#endif // __BTPHYSTEST_H__

