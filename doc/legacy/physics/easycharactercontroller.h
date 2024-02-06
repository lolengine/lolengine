//
//  Lol Engine — Bullet physics test
//
//  Copyright © 2009—2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2012—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The EasyCharacterController class
// ------------------
//

//Should try to to make a btKinematicCharacterController for real.
//

#include <lol/engine.h>
#include "easyphysics.h"
#include "bulletcharactercontroller.h"
#include <BulletDynamics/Character/btKinematicCharacterController.h>

namespace lol
{

namespace phys
{

class EasyCharacterController : public EasyPhysic,
                                public entity
{

    friend class Simulation;
    friend class EasyPhysic;

public:
    EasyCharacterController(WorldEntity* NewOwnerEntity) :
        EasyPhysic(NewOwnerEntity),
        m_pair_caching_object(nullptr),
        m_character(nullptr),
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
    virtual std::string GetName();

public:
    virtual void tick_game(float seconds);

protected:

    virtual btGhostObject* GetGhostObjectInstance();

    btPairCachingGhostObject*        m_pair_caching_object;
    //btKinematicCharacterController*    m_character;
    BulletKinematicCharacterController* m_character;

    float                            m_step_height;
    int                                m_up_axis;
    bool                            m_base_is_updating;
    vec3                            m_base_cached_movement;
    vec3                            m_frame_cached_movement;

    //----
    float                            m_walk_velocity_damping;

    //----
    vec3                            m_gravity;

    //----
    vec3                            m_walk_velocity;
    vec3                            m_current_velocity;
};

} /* namespace phys */

} /* namespace lol */

