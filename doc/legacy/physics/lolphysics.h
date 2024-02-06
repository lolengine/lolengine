//
//  Lol Engine — Bullet physics test
//
//  Copyright © 2012—2020 Sam Hocevar <sam@hocevar.net>
//            © 2009—2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <cstring>
#include <cassert>
#include <vector>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "lolbtphysicsintegration.h"
#include "easyphysics.h"
#include "easyconstraint.h"

namespace lol
{

namespace phys
{

enum eRaycastType
{
    ERT_Closest,
    ERT_AllHit,
    ERT_AnyHit, //Will stop at the first hit. Hit data are supposed to be irrelevant

    ERT_MAX
};

struct RayCastResult
{
    RayCastResult(int CollisionFilterGroup=1, int CollisionFilterMask=(0xFF))
      : m_collision_filter_group(CollisionFilterGroup),
        m_collision_filter_mask(CollisionFilterMask)
    {
    }

    void Reset()
    {
        m_collider_list.clear();
        m_hit_normal_list.clear();
        m_hit_point_list.clear();
        m_hit_fraction_list.clear();
    }

    std::vector<EasyPhysic*> m_collider_list;
    std::vector<vec3> m_hit_normal_list;
    std::vector<vec3> m_hit_point_list;
    std::vector<float> m_hit_fraction_list;

    int16_t m_collision_filter_group;
    int16_t m_collision_filter_mask;
    uint32_t m_flags = 0; //???
};

class Simulation : public entity
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
        m_gamegroup = GAMEGROUP_SIMULATION;
    }
    ~Simulation()
    {
        Exit();
    }

    std::string GetName() const { return "<Simulation>"; }

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

    virtual void tick_game(float seconds)
    {
        entity::tick_game(seconds);

        //step the simulation
        if (m_dynamics_world)
        {
            //the "+1" is to have at least one Timestep and to ensure float to int .5f conversion.
            int steps = (int)(seconds / m_timestep) + 1;
            m_dynamics_world->stepSimulation(seconds, steps, m_timestep);
        }
    }

    //Rip-Off of the btKinematicClosestNotMeRayResultCallback
    class ClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
    {
    public:
        ClosestNotMeRayResultCallback(btCollisionObject* Me, const btVector3& rayFromWorld, const btVector3& rayToWorld) :
          btCollisionWorld::ClosestRayResultCallback(rayFromWorld, rayToWorld)
        {
            m_me = Me;
        }

        virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
        {
            if (rayResult.m_collisionObject == m_me)
                return 1.0;

            return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
        }
    protected:
        btCollisionObject* m_me;
    };

    //Will stop at the first hit. Hit data are supposed to be irrelevant
    class AnyHitRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
    {
    public:
        AnyHitRayResultCallback(const btVector3& rayFromWorld, const btVector3& rayToWorld) :
          btCollisionWorld::ClosestRayResultCallback(rayFromWorld, rayToWorld)
        {
        }

        virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &, bool)
        {
            return .0f;
        }
    };

    //Returns true when hitting something. If SourceCaster is set, it will be ignored by Raycast.
    bool RayHits(RayCastResult& HitResult, eRaycastType RaycastType, const vec3& RayFrom, const vec3& RayTo, EasyPhysic* SourceCaster = nullptr)
    {
        bool bResult = false;

        btCollisionWorld::RayResultCallback* BtRayResult = nullptr;
        btCollisionWorld::ClosestRayResultCallback* BtRayResult_Closest;
        btCollisionWorld::AllHitsRayResultCallback* BtRayResult_AllHits;

        switch (RaycastType)
        {
            case ERT_Closest:
            {
                if (SourceCaster)
                    BtRayResult_Closest = new ClosestNotMeRayResultCallback(SourceCaster->m_collision_object, LOL2BTU_VEC3(RayFrom), LOL2BTU_VEC3(RayTo));
                else
                    BtRayResult_Closest = new btCollisionWorld::ClosestRayResultCallback(LOL2BTU_VEC3(RayFrom), LOL2BTU_VEC3(RayTo));
                BtRayResult = BtRayResult_Closest;
                break;
            }
            case ERT_AllHit:
            {
                BtRayResult_AllHits = new btCollisionWorld::AllHitsRayResultCallback(LOL2BTU_VEC3(RayFrom), LOL2BTU_VEC3(RayTo));
                BtRayResult = BtRayResult_AllHits;
                break;
            }
            case ERT_AnyHit:
            {
                BtRayResult_Closest = new AnyHitRayResultCallback(LOL2BTU_VEC3(RayFrom), LOL2BTU_VEC3(RayTo));
                BtRayResult = BtRayResult_Closest;
                break;
            }
            default:
            {
                assert(false); // Raycast not handled
            }
        }

        m_dynamics_world->rayTest(LOL2BTU_VEC3(RayFrom), LOL2BTU_VEC3(RayTo), *BtRayResult);
        if (BtRayResult->hasHit())
        {
            bResult = true;

            switch (RaycastType)
            {
                case ERT_Closest:
                {
                    HitResult.m_collider_list.push_back((EasyPhysic*)BtRayResult_Closest->m_collisionObject->getUserPointer());
                    HitResult.m_hit_normal_list.push_back(BT2LOLU_VEC3(BtRayResult_Closest->m_hitNormalWorld));
                    HitResult.m_hit_point_list.push_back(BT2LOLU_VEC3(BtRayResult_Closest->m_hitPointWorld));
                    HitResult.m_hit_fraction_list.push_back(BtRayResult_Closest->m_closestHitFraction);
                    break;
                }
                case ERT_AllHit:
                {
                    for (int i = 0; i < BtRayResult_AllHits->m_collisionObjects.size(); i++)
                    {
                        HitResult.m_collider_list.push_back((EasyPhysic*)BtRayResult_AllHits->m_collisionObjects[i]->getUserPointer());
                        HitResult.m_hit_normal_list.push_back(BT2LOLU_VEC3(BtRayResult_AllHits->m_hitNormalWorld[i]));
                        HitResult.m_hit_point_list.push_back(BT2LOLU_VEC3(BtRayResult_AllHits->m_hitPointWorld[i]));
                        HitResult.m_hit_fraction_list.push_back(BtRayResult_AllHits->m_hitFractions[i]);
                    }
                    break;
                }
                default:
                {
                    assert(false); // Raycast not handled
                }
            }
        }

        delete BtRayResult;

        return bResult;
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

    void CustomSetWorldLimit(vec3 const &, vec3 const &)
    {
    }

    void CustomSetTimestep(float NewTimestep) { }

    //broadphase
    btBroadphaseInterface *m_broadphase;
    btAxisSweep3 *m_Sweep_broadphase;
    // Set up the collision configuration and dispatc
    btDefaultCollisionConfiguration *m_collision_configuration;
    btCollisionDispatcher *m_dispatcher;
    // The actual physics solver
    btSequentialImpulseConstraintSolver *m_solver;
    // The world.
    btDiscreteDynamicsWorld *m_dynamics_world;

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
    friend class EasyCharacterController;
    friend class EasyConstraint;

    enum eEasyPhysicType
    {
        EEPT_Dynamic,
        EEPT_Static,
        EEPT_Ghost,
        EEPT_CollisionObject,
        EEPT_CharacterController,

        EEPT_MAX
    };

    //m_owner_simulation
    //Adds the given EasyPhysic to the correct list.
    void ObjectRegistration(bool AddObject, EasyPhysic* NewEP, eEasyPhysicType CurType)
    {
        std::vector<EasyPhysic*>* SearchList = nullptr;
        switch(CurType)
        {
            case EEPT_Dynamic:
            {
                SearchList = &m_dynamic_list;
                break;
            }
            case EEPT_Static:
            {
                SearchList = &m_static_list;
                break;
            }
            case EEPT_Ghost:
            {
                SearchList = &m_ghost_list;
                break;
            }
            case EEPT_CollisionObject:
            {
                SearchList = &m_collision_object_list;
                break;
            }
            case EEPT_CharacterController:
            {
                SearchList = &m_character_controller_list;
                break;
            }
            default:
            {
                assert(false); // Physic type does not exist
            }
        }

        if (AddObject)
        {
            NewEP->m_owner_simulation = this;
            SearchList->push_back(NewEP);
        }
        else
        {
            NewEP->m_owner_simulation = nullptr;
            remove_item(*SearchList, NewEP);
        }
    }
    void ObjectRegistration(bool AddObject, EasyConstraint* NewEC)
    {
        std::vector<EasyConstraint*>* SearchList = nullptr;
        SearchList = &m_constraint_list;

        if (AddObject)
        {
            NewEC->m_owner_simulation = this;
            SearchList->push_back(NewEC);
        }
        else
        {
            NewEC->m_owner_simulation = nullptr;
            remove_item(*SearchList, NewEC);
        }
    }

    //Easy Physics body List
    std::vector<EasyPhysic*> m_dynamic_list;
    std::vector<EasyPhysic*> m_static_list;
    std::vector<EasyPhysic*> m_ghost_list;
    std::vector<EasyPhysic*> m_collision_object_list;
    std::vector<EasyPhysic*> m_character_controller_list;
    std::vector<EasyConstraint*> m_constraint_list;

    //Easy Physics data storage
    float m_timestep;
    bool m_using_CCD;
    vec3 m_gravity;
    vec3 m_world_min;
    vec3 m_world_max;
};

} /* namespace phys */

} /* namespace lol */

