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

class Simulation : public Entity
{
	friend class EasyPhysics;

public:
	Simulation() :
		m_broadphase(0),
		m_collision_configuration(0),
		m_dispatcher(0),
		m_solver(0),
		m_dynamics_world(0),
		m_timestep(1.f/60.f)
	{
	}
	~Simulation()
	{
		Exit();
	}

	char const *GetName() { return "<Simulation>"; }

#ifdef HAVE_PHYS_USE_BULLET
public:
	void Init()
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

	virtual void TickGame(float seconds)
	{
		//step the simulation
		if (m_dynamics_world)
		{
			//the "+1" is to have at least one Timestep and to ensure float to int .5f conversion.
			int steps = (int)(seconds / m_timestep) + 1;
			m_dynamics_world->stepSimulation(seconds, steps, m_timestep);
		}
	}

	void Exit()
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

private:
	void CustomSetContinuousDetection(bool ShouldUseCCD)
	{
		if (m_dynamics_world)
			m_dynamics_world->getDispatchInfo().m_useContinuous = ShouldUseCCD;
	}

	void CustomSetGravity(vec3 &NewGravity)
	{
		if (m_dynamics_world)
			m_dynamics_world->setGravity(LOL2BT_VEC3(NewGravity));
	}

	void CustomSetTimestep(float NewTimestep) { }

	//broadphase
	btBroadphaseInterface*					m_broadphase;
	// Set up the collision configuration and dispatc
	btDefaultCollisionConfiguration*		m_collision_configuration;
	btCollisionDispatcher*					m_dispatcher;
	// The actual physics solver
	btSequentialImpulseConstraintSolver*	m_solver;
	// The world.
	btDiscreteDynamicsWorld*				m_dynamics_world;
#else
public:
	void Init() { }
	void TickGame(float seconds) { }
	void Exit() { }
private:
	void CustomSetContinuousDetection(bool ShouldUseCCD) { }
	void CustomSetGravity(vec3 &NewGravity) { }
	void CustomSetTimestep(float NewTimestep) { }
#endif //HAVE_PHYS_USE_BULLET

public:
	//Main logic :
	//The Set*() functions do the all-lib-independent data storage.
	//And then it calls the CustomSet*() which are the specialized versions.

	//Sets the continuous collision detection flag.
	void SetContinuousDetection(bool ShouldUseCCD)
	{
		m_using_CCD = ShouldUseCCD;
		CustomSetContinuousDetection(ShouldUseCCD);
	}

	//Sets the simulation gravity.
	void SetGravity(vec3 &NewGravity)
	{
		m_gravity = NewGravity;
		CustomSetGravity(NewGravity);
	}

	//Sets the simulation fixed timestep.
	void SetTimestep(float NewTimestep)
	{
		if (NewTimestep > .0f)
		{
			m_timestep = NewTimestep;
			CustomSetTimestep(NewTimestep);
		}
	}

private:
	//Adds the given EasyPhysics to the correct list.
	void AddToDynamic(EasyPhysics* dynamic_EP)	{ m_dynamic_list << dynamic_EP; }
	void AddToStatic(EasyPhysics* static_EP)	{ m_static_list	<< static_EP; }

	//Easy Physics body List
	Array<EasyPhysics*>						m_dynamic_list;
	Array<EasyPhysics*>						m_static_list;

	//Easy Physics data storage
	float									m_timestep;
	bool									m_using_CCD;
	vec3									m_gravity;
};

} /* namespace phys */

} /* namespace lol */

#endif // __LOLPHYSICS_H__

