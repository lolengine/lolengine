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
	EasyMesh							m_ground_mesh;
	EasyMesh							m_rigid_mesh[6];

	//Bullet Physics Datas
	enum
	{
		USE_CCD=1,
		USE_NO_CCD
	};
	int 									m_bt_ccd_mode;
	btDefaultCollisionConfiguration*		m_bt_collision_config;
	btCollisionDispatcher*					m_bt_dispatcher;
	btDbvtBroadphase*						m_bt_broadphase;
	btSequentialImpulseConstraintSolver*	m_bt_solver;
	btDiscreteDynamicsWorld*				m_bt_world;
	Array<btCollisionShape*>				m_bt_collision_shapes;
	Array<btCollisionShape*>				m_bt_dynamic_shapes;
};

#endif // __BTPHYSTEST_H__

