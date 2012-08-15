//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2012 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#ifdef HAVE_PHYS_USE_BULLET
#include "../Include/EasyCharacterController.h"
#endif //HAVE_PHYS_USE_BULLET

namespace lol
{

namespace phys
{

#ifdef HAVE_PHYS_USE_BULLET

//-------------------------------------------------------------------------
//EASY_CHARACTER_CONTROLLER
//--

//Deactivated for Character controller
void EasyCharacterController::InitBodyToRigid(bool ZeroMassIsKinematic)
{
}

//Return correct Ghost Object
btGhostObject* EasyCharacterController::GetGhostObject()
{
	return new btPairCachingGhostObject();
}

//Init to Pair caching ghost object, since Character uses that one.
void EasyCharacterController::InitBodyToGhost()
{
	EasyPhysic::InitBodyToGhost();

	m_pair_caching_object = (btPairCachingGhostObject*)m_ghost_object;
	m_ghost_object->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT | m_ghost_object->getCollisionFlags());
}

//Add Physic object to the simulation
void EasyCharacterController::AddToSimulation(class Simulation* current_simulation)
{
	EasyPhysic::AddToSimulation(current_simulation);

	btDiscreteDynamicsWorld* dynamics_world = current_simulation->GetWorld();
	if (dynamics_world)
	{
		if (m_character)
			delete m_character;

		m_character = new btKinematicCharacterController(m_pair_caching_object, m_convex_shape, m_step_height, m_up_axis);
		dynamics_world->addAction(m_character);
	}
}

//Remove Physic object to the simulation
void EasyCharacterController::RemoveFromSimulation(class Simulation* current_simulation)
{
	EasyPhysic::RemoveFromSimulation(current_simulation);

	btDiscreteDynamicsWorld* dynamics_world = current_simulation->GetWorld();
	if (dynamics_world)
	{
		if (m_character)
			dynamics_world->removeAction(m_character);
	}
}

//Set movement for this frame
void EasyCharacterController::SetMovementForFrame(vec3 const &MoveQuantity)
{
	m_character->setWalkDirection(LOL2BT_VEC3(MoveQuantity));
}

#endif // HAVE_PHYS_USE_BULLET

} /* namespace phys */

} /* namespace lol */
