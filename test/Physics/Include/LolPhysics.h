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
#include "EasyConstraint.h"
#endif

namespace lol
{

namespace phys
{

class Simulation : public Entity
{
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
		if (1)
		{
			m_Sweep_broadphase = new btAxisSweep3(LOL2BT_VEC3(m_world_min), LOL2BT_VEC3(m_world_max));
			m_Sweep_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
			m_broadphase = m_Sweep_broadphase;
		}
		else
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
		Entity::TickGame(seconds);

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
			m_dynamics_world->setGravity(LOL2BT_VEC3(NewGravity * LOL2BT_UNIT));
	}

	void CustomSetWorldLimit(vec3 const &NewWorldMin, vec3 const &NewWorldMax)
	{
	}

	void CustomSetTimestep(float NewTimestep) { }

	//broadphase
	btBroadphaseInterface*					m_broadphase;
	btAxisSweep3*							m_Sweep_broadphase;
	// Set up the collision configuration and dispatc
	btDefaultCollisionConfiguration*		m_collision_configuration;
	btCollisionDispatcher*					m_dispatcher;
	// The actual physics solver
	btSequentialImpulseConstraintSolver*	m_solver;
	// The world.
	btDiscreteDynamicsWorld*				m_dynamics_world;

#else  // NO PHYSIC IMPLEMENTATION

public:
	void Init() { }
	void TickGame(float seconds) { }
	void Exit() { }
private:
	void CustomSetContinuousDetection(bool ShouldUseCCD) { }
	void CustomSetGravity(vec3 &NewGravity) { }
	void CustomSetWorldLimit(vec3 &NewWorldMin, vec3 &NewWorldMax) { }
	void CustomSetTimestep(float NewTimestep) { }

#endif // PHYSIC IMPLEMENTATION

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

	//Sets the simulation gravity.
	void SetWorldLimit(vec3 const &NewWorldMin, vec3 const &NewWorldMax)
	{
		m_world_min = NewWorldMin;
		m_world_max = NewWorldMax;
		CustomSetWorldLimit(NewWorldMin, NewWorldMax);
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
	friend class EasyPhysic;
	friend class EasyConstraint;

	//Adds the given EasyPhysic to the correct list.
	void AddToDynamic(EasyPhysic* NewEPDynamic)	{ m_dynamic_list << NewEPDynamic; }
	void AddToStatic(EasyPhysic* NewEPStatic)	{ m_static_list	<< NewEPStatic; }
	void AddToGhost(EasyPhysic* NewEPGhost)		{ m_ghost_list << NewEPGhost; }
	void AddToConstraint(EasyConstraint* NewEC)	{ m_constraint_list	<< NewEC; }

	//Easy Physics body List
	Array<EasyPhysic*>						m_dynamic_list;
	Array<EasyPhysic*>						m_static_list;
	Array<EasyPhysic*>						m_ghost_list;
	Array<EasyConstraint*>					m_constraint_list;

	//Easy Physics data storage
	float									m_timestep;
	bool									m_using_CCD;
	vec3									m_gravity;
	vec3									m_world_min;
	vec3									m_world_max;
};

} /* namespace phys */

} /* namespace lol */

#endif // __LOLPHYSICS_H__

