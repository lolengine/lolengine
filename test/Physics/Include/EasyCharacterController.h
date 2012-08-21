//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The EasyCharacterController class
// ------------------
//

//Should try to to make a btKinematicCharacterController for real.
//

#if !defined __EASYCHARACTERCONTROLLER_EASYCHARACTERCONTROLLER_H__
#define __EASYCHARACTERCONTROLLER_EASYCHARACTERCONTROLLER_H__

#ifdef HAVE_PHYS_USE_BULLET
#include "core.h"
#include "EasyPhysics.h"
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#endif

namespace lol
{

namespace phys
{

class EasyCharacterController : public EasyPhysic,
								public Entity
{

	friend class Simulation;
	friend class EasyPhysic;

#ifdef HAVE_PHYS_USE_BULLET

public:
	EasyCharacterController(WorldEntity* NewOwnerEntity) :
		EasyPhysic(NewOwnerEntity),
		m_pair_caching_object(NULL),
		m_character(NULL),
		m_step_height(.0f),
		m_base_is_updating(false),
		m_base_cached_movement(vec3(0.f)),
		m_frame_cached_movement(vec3(0.f)),
		m_walk_velocity(vec3(0.f)),
		m_current_velocity(vec3(0.f))
	{
		m_gamegroup = GAMEGROUP_EZP_CHAR_CTRLR;
		m_up_axis = 1;
		m_gravity = vec3(.0f, -9.81f, .0f);
		m_walk_velocity_damping = 0.2f;
	}
	~EasyCharacterController()
	{
		delete m_character;
	}

	virtual void InitBodyToRigid(bool ZeroMassIsKinematic=false);
	virtual void InitBodyToGhost();
	virtual void AddToSimulation(class Simulation* current_simulation);
	virtual void RemoveFromSimulation(class Simulation* current_simulation);
	virtual void SetMovementForFrame(vec3 const &MoveQuantity);
	virtual void Jump();

	virtual void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation);
protected:
	virtual void BaseTransformChanged(const lol::mat4& PreviousMatrix, const lol::mat4& NewMatrix);
public:
	virtual void TickGame(float seconds);

protected:

	virtual btGhostObject* GetGhostObjectInstance();

	btPairCachingGhostObject*		m_pair_caching_object;
	btKinematicCharacterController*	m_character;

	float							m_step_height;
	int								m_up_axis;
	bool							m_base_is_updating;
	vec3							m_base_cached_movement;
	vec3							m_frame_cached_movement;

	//----
	float							m_walk_velocity_damping;

	//----
	vec3							m_gravity;

	//----
	vec3							m_walk_velocity;
	vec3							m_current_velocity;

#else  // NO PHYSIC IMPLEMENTATION

	virtual void InitBodyToRigid(bool ZeroMassIsKinematic=false) { }
	virtual void InitBodyToGhost() { }
	virtual void AddToSimulation(class Simulation* current_simulation) { }
	virtual void RemoveFromSimulation(class Simulation* current_simulation) { }
	virtual void SetMovementForFrame(vec3 const &MoveQuantity) { }

#endif // PHYSIC IMPLEMENTATION

};

} /* namespace phys */

} /* namespace lol */

#endif /* __EASYCHARACTERCONTROLLER_EASYCHARACTERCONTROLLER_H__ */

