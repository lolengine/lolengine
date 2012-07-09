//
// LolPhysics
//
// Copyright: (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//            (c) 2012 Sam Hocevar <sam@hocevar.net>
//

#if !defined __LOLPHYSICS_H__
#define __LOLPHYSICS_H__

#ifdef HAVE_PHYS_USE_BULLET
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include "LolBtPhysicsIntegration.h"
#include "EasyPhysics.h"
#endif

namespace lol
{

namespace phys
{

class Simulation
{
public:
	Simulation() :
		m_broadphase(0),
		m_collision_configuration(0),
		m_dispatcher(0),
		m_solver(0),
		m_dynamics_world(0)
	{
	}
	~Simulation()
	{
		ExitContext();
	}

	char const *GetName() { return "<Simulation>"; }

#ifdef HAVE_PHYS_USE_BULLET
	void InitContext()
	{
		// Build the broadphase
		m_broadphase = new btDbvtBroadphase();
 
		// Set up the collision configuration and dispatcher
		m_collision_configuration = new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_collision_configuration);
 
		// The actual physics solver
		m_solver = new btSequentialImpulseConstraintSolver;
 
		// The world.
		m_dynamics_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collision_configuration);
	}

	void SetContinuousDetection(bool ShouldUseCCD)
	{
		if (m_dynamics_world)
			m_dynamics_world->getDispatchInfo().m_useContinuous = ShouldUseCCD;
	}

	void SetGravity(vec3 &NewGravity)
	{
		if (m_dynamics_world)
			m_dynamics_world->setGravity(LOL2BT_VEC3(NewGravity));
	}

	void TickContext(float seconds)
	{
		//step the simulation
		if (m_dynamics_world)
		{
			int steps = (int)(seconds / 0.005f);
			for (int i = 0; i < steps; i++)
				m_dynamics_world->stepSimulation(seconds / steps);
		}
	}

	void ExitContext()
	{
		delete m_dynamics_world;
		delete m_solver;
		delete m_dispatcher;
		delete m_collision_configuration;
		delete m_broadphase;
	}

	btDiscreteDynamicsWorld* GetWorld()
	{
		return m_dynamics_world;
	}
	void AddToDynamic(EasyPhysics* dynamic_EP)
	{
		m_dynamic_list << dynamic_EP;
	}
	void AddToStatic(EasyPhysics* static_EP)
	{
		m_static_list << static_EP;
	}

private:

	//broadphase
	btBroadphaseInterface*					m_broadphase;
	// Set up the collision configuration and dispatc
	btDefaultCollisionConfiguration*		m_collision_configuration;
	btCollisionDispatcher*					m_dispatcher;
	// The actual physics solver
	btSequentialImpulseConstraintSolver*	m_solver;
	// The world.
	btDiscreteDynamicsWorld*				m_dynamics_world;

	//Easy Physics body List
	Array<EasyPhysics*>						m_dynamic_list;
	Array<EasyPhysics*>						m_static_list;
#else
	void InitContext() { }
	void TickContext(float seconds) { }
	void ExitContext() { }
#endif //HAVE_PHYS_USE_BULLET
};

} /* namespace phys */

} /* namespace lol */

#endif // __LOLPHYSICS_H__

