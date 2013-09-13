//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "lolbtphysicsintegration.h"
#include "lolphysics.h"
#include "easycharactercontroller.h"

namespace lol
{

namespace phys
{

//-------------------------------------------------------------------------
//EASY_CHARACTER_CONTROLLER
//--

//Deactivated for Character controller
void EasyCharacterController::InitBodyToRigid(bool ZeroMassIsKinematic)
{
}

//Return correct Ghost Object
btGhostObject* EasyCharacterController::GetGhostObjectInstance()
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

        //m_character = new btKinematicCharacterController(m_pair_caching_object, m_convex_shape, m_step_height, m_up_axis);
        m_character = new BulletKinematicCharacterController(m_pair_caching_object, m_convex_shape, m_step_height, m_up_axis);

        //Deactivate Character controller basic behaviour.
        //m_character->setGravity(.0f);
        //m_character->setFallSpeed(.0f);

        dynamics_world->addAction(m_character);
        current_simulation->ObjectRegistration(true, this, Simulation::EEPT_CharacterController);
        Ticker::Ref(this);
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
        {
            dynamics_world->removeAction(m_character);
            current_simulation->ObjectRegistration(false, this, Simulation::EEPT_CharacterController);
            Ticker::Unref(this);
        }
    }
}

void EasyCharacterController::Jump()
{
    m_character->Jump();
}

//Set movement for this frame
void EasyCharacterController::SetMovementForFrame(vec3 const &MoveQuantity)
{
    m_frame_cached_movement = MoveQuantity;
}

//-------------------------------------------------------------------------
//Base Location/Rotation setup
//--
void EasyCharacterController::SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation)
{
    if (m_base_is_updating)
    {
        m_base_cached_movement = base_location - m_local_to_world.v3.xyz;
        m_local_to_world = lol::mat4::translate(m_local_to_world.v3.xyz) * lol::mat4(base_rotation);
        if (m_ghost_object)
            m_ghost_object->setWorldTransform(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(LOL2BT_UNIT * m_local_to_world.v3.xyz)));
    }
    else
        EasyPhysic::SetTransform(base_location, base_rotation);
}

//Internal callback when Base transform has changed.
void EasyCharacterController::BaseTransformChanged(const lol::mat4& PreviousMatrix, const lol::mat4& NewMatrix)
{
    m_base_is_updating = true;
    EasyPhysic::BaseTransformChanged(PreviousMatrix, NewMatrix);
    m_base_is_updating = false;
}

//---
char const *EasyCharacterController::GetName()
{
    return "<EasyCharacterController>";
}

//Physic Tick
void EasyCharacterController::TickGame(float seconds)
{
    Entity::TickGame(seconds);

    //Send final velocity in Bullet
    {
        int IterationsNb = (int)(seconds / m_owner_simulation->m_timestep);
        float NewSeconds = IterationsNb * m_owner_simulation->m_timestep;
        m_character->SetVelocityForTimeInterval((m_base_cached_movement + m_frame_cached_movement) / NewSeconds, NewSeconds);
        m_base_cached_movement = vec3(.0f);
    }
}

} /* namespace phys */

} /* namespace lol */
