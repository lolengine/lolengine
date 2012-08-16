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
// The EasyPhysic class
// ------------------
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

class EasyCharacterController : public EasyPhysic
{

#ifdef HAVE_PHYS_USE_BULLET

public:
	EasyCharacterController(WorldEntity* NewOwnerEntity) :
		EasyPhysic(NewOwnerEntity),
		m_character(NULL)
	{
		m_up_axis = 1;
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

protected:

	virtual btGhostObject* GetGhostObject();

	btPairCachingGhostObject*		m_pair_caching_object;
	btKinematicCharacterController*	m_character;

	float							m_step_height;
	int								m_up_axis;

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

