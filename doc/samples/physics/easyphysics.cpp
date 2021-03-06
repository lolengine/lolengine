//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2009—2013 Cédric Lecacheur <jordx@free.fr>
//            © 2009—2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include "lolbtphysicsintegration.h"
#include "lolphysics.h"

namespace lol
{

namespace phys
{

//-------------------------------------------------------------------------
//EASY_PHYSIC
//--

EasyPhysic::EasyPhysic(WorldEntity* NewOwnerEntity) :
    m_collision_object(nullptr),
    m_ghost_object(nullptr),
    m_rigid_body(nullptr),
    m_local_inertia(btVector3(.0f, .0f, .0f)),
    m_collision_shape(nullptr),
    m_convex_shape(nullptr),
    m_motion_state(nullptr),
    m_mass(.0f),
    m_hit_restitution(.0f),
    m_collision_group(1),
    m_collision_mask(1),
    m_owner_entity(NewOwnerEntity),
    m_owner_simulation(nullptr),
    m_base_physic(nullptr)
{
}

EasyPhysic::~EasyPhysic()
{
    m_rigid_body = nullptr;
    delete m_collision_object;
    delete m_collision_shape;
    delete m_motion_state;
}

//-------------------------------------------------------------------------
//Set Shape functions
//--

void EasyPhysic::SetShapeTo(btCollisionShape* collision_shape)
{
    bool bReinitToRigidBody = false;
    if (m_rigid_body)
    {
        bReinitToRigidBody = true;
        delete m_rigid_body;
    }
    if (m_collision_shape)
        delete m_collision_shape;

    m_collision_shape = collision_shape;

    if (bReinitToRigidBody)
        InitBodyToRigid();
}

//Box Shape support
void EasyPhysic::SetShapeToBox(lol::vec3& box_size)
{
    vec3 new_box_size = box_size * LOL2BT_UNIT * LOL2BT_SIZE;
    m_convex_shape = new btBoxShape(LOL2BT_VEC3(new_box_size));
    SetShapeTo(m_convex_shape);
}

void EasyPhysic::SetShapeToSphere(float radius)
{
    m_convex_shape = new btSphereShape(radius * LOL2BT_UNIT * LOL2BT_SIZE);
    SetShapeTo(m_convex_shape);
}

void EasyPhysic::SetShapeToCone(float radius, float height)
{
    m_convex_shape = new btConeShape(    radius * LOL2BT_UNIT,
                                        height * LOL2BT_UNIT);
    SetShapeTo(m_convex_shape);
}

void EasyPhysic::SetShapeToCylinder(lol::vec3& cyl_size)
{
    vec3 new_cyl_size = cyl_size * LOL2BT_UNIT;
    new_cyl_size.y *= LOL2BT_SIZE;
    m_convex_shape = new btCylinderShape(LOL2BT_VEC3(new_cyl_size));
    SetShapeTo(m_convex_shape);
}

void EasyPhysic::SetShapeToCapsule(float radius, float height)
{
    m_convex_shape = new btCapsuleShape(radius * LOL2BT_UNIT * LOL2BT_SIZE,
                                        height * LOL2BT_UNIT * LOL2BT_SIZE);
    SetShapeTo(m_convex_shape);
}

//-------------------------------------------------------------------------
//Base Location/Rotation setup
//--

//Getter
mat4 EasyPhysic::GetTransform()
{
    m_local_to_world = lol::mat4(1.0f);
    if (m_rigid_body && m_motion_state)
    {
        btTransform CurTransform;
        m_motion_state->getWorldTransform(CurTransform);
        CurTransform.getOpenGLMatrix(&m_local_to_world[0][0]);
    }
    else if (m_collision_object)
        m_collision_object->getWorldTransform().getOpenGLMatrix(&m_local_to_world[0][0]);
    return m_local_to_world;
}

//Setter
void EasyPhysic::SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation)
{
    lol::mat4 PreviousMatrix = m_local_to_world;
    m_local_to_world = lol::mat4::translate(base_location) * lol::mat4(base_rotation);

    if (m_ghost_object)
        m_ghost_object->setWorldTransform(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(LOL2BT_UNIT * base_location)));
    else
    {
        if (m_motion_state)
            m_motion_state->setWorldTransform(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(LOL2BT_UNIT * base_location)));
        else
            m_motion_state = new btDefaultMotionState(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(LOL2BT_UNIT * base_location)));
    }

    for (size_t i = 0; i < m_based_physic_list.size(); i++)
    {
        if (m_based_physic_list[i])
            m_based_physic_list[i]->BaseTransformChanged(PreviousMatrix, m_local_to_world);
        else
            remove_at(m_based_physic_list, i--);
    }
}

//Internal callback when Base transform has changed.
void EasyPhysic::BaseTransformChanged(const lol::mat4& PreviousMatrix, const lol::mat4& NewMatrix)
{
    mat4 PreviousMatrixLoc = ((m_base_lock_location)?(PreviousMatrix):(lol::mat4::translate(PreviousMatrix[3].xyz)));
    mat4 PreviousMatrixRot = ((m_base_lock_rotation)?(lol::mat4(lol::quat(lol::mat3(PreviousMatrix)))):(lol::mat4(1.f)));
    mat4 NewMatrixLoc = ((m_base_lock_location)?(NewMatrix):(lol::mat4::translate(NewMatrix[3].xyz)));
    mat4 NewMatrixRot = ((m_base_lock_rotation)?(lol::mat4(lol::quat(lol::mat3(NewMatrix)))):(lol::mat4(1.f)));

    if (m_ghost_object || (m_rigid_body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT))
    {
        mat4 ThisMatrixLoc = NewMatrixLoc * inverse(PreviousMatrixLoc) * lol::mat4::translate(m_local_to_world[3].xyz);
        mat4 ThisMatrixRot = NewMatrixRot * inverse(PreviousMatrixRot) * lol::mat4(lol::quat(lol::mat3(m_local_to_world)));
        SetTransform(ThisMatrixLoc[3].xyz, lol::quat(lol::mat3(ThisMatrixRot)));
    }
}

//-------------------------------------------------------------------------
//Mass related functions
//--
//Set Mass functions
void EasyPhysic::SetMass(float mass)
{
    m_mass = mass;

    if (m_rigid_body)
    {
        SetLocalInertia(m_mass);
        m_rigid_body->setMassProps(m_mass, m_local_inertia);
    }
}

//-------------------------------------------------------------------------
//Hit restitution functions
//--
//Set Hit Restitution functions
void EasyPhysic::SetHitRestitution(float hit_restitution)
{
    m_hit_restitution = hit_restitution;

    if (m_rigid_body)
    {
        m_rigid_body->setRestitution(m_hit_restitution);
    }
}

//-------------------------------------------------------------------------
//Final conversion pass functions : Body related
//--

//Init to rigid body
void EasyPhysic::InitBodyToRigid(bool SetToKinematic)
{
    if (m_collision_object)
        delete m_collision_object;

    if (!m_motion_state)
        SetTransform(vec3(.0f));

    btRigidBody::btRigidBodyConstructionInfo NewInfos(m_mass, m_motion_state, m_collision_shape, m_local_inertia);
    NewInfos.m_restitution = m_hit_restitution;
    m_rigid_body = new btRigidBody(NewInfos);
    m_collision_object = m_rigid_body;
    m_collision_object->setUserPointer(this);

    if (m_mass == .0f)
    {
        if (SetToKinematic)
        {
            m_rigid_body->setActivationState(DISABLE_DEACTIVATION);
            m_rigid_body->setCollisionFlags(m_rigid_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        }
    }
    else
        SetMass(m_mass);
}

//Return correct Ghost Object
btGhostObject* EasyPhysic::GetGhostObjectInstance()
{
    return new btGhostObject();
}

//Init to Ghost object, for Overlap/Sweep Test/Touching logic
void EasyPhysic::InitBodyToGhost()
{
    if (m_collision_object)
        delete m_collision_object;

    m_ghost_object = GetGhostObjectInstance();
    m_ghost_object->setCollisionShape(m_collision_shape);
    m_collision_object = m_ghost_object;
    m_collision_object->setUserPointer(this);

    SetTransform(m_local_to_world[3].xyz, lol::quat(lol::mat3(m_local_to_world)));

    m_ghost_object->setCollisionFlags(m_ghost_object->getCollisionFlags());
}

//-------------
//Touch logic
//-------------
  //    btManifoldArray   manifoldArray;
  //    btBroadphasePairArray& pairArray = ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
  //    int numPairs = pairArray.size();

  //    for (int i=0;i<numPairs;i++)
  //    {
  //       manifoldArray.clear();

  //       const btBroadphasePair& pair = pairArray[i];
  //
  //       //unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
  //       btBroadphasePair* collisionPair = dynamicsWorld->getPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);
  //       if (!collisionPair)
  //          continue;

  //       if (collisionPair->m_algorithm)
  //          collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

  //       for (int j=0;j<manifoldArray.size();j++)
  //       {
  //          btPersistentManifold* manifold = manifoldArray[j];
  //          btScalar directionSign = manifold->getBody0() == m_ghostObject ? btScalar(-1.0) : btScalar(1.0);
  //          for (int p=0;p<manifold->getNumContacts();p++)
  //          {
  //               const btManifoldPoint&pt = manifold->getContactPoint(p);
  //              if (pt.getDistance()<0.f)
        //{
        //    const btVector3& ptA = pt.getPositionWorldOnA();
        //    const btVector3& ptB = pt.getPositionWorldOnB();
        //    const btVector3& normalOnB = pt.m_normalWorldOnB;
        //    /// work here
        //}
  //          }
  //       }
  //    }


//Add Physic object to the simulation
void EasyPhysic::AddToSimulation(class Simulation* current_simulation)
{
    btDiscreteDynamicsWorld* dynamics_world = current_simulation->GetWorld();
    if (dynamics_world)
    {
        if (m_ghost_object)
        {
            dynamics_world->addCollisionObject(m_ghost_object, m_collision_group, m_collision_mask);
            current_simulation->ObjectRegistration(true, this, Simulation::EEPT_Ghost);
        }
        else if (m_rigid_body)
        {
            dynamics_world->addRigidBody(m_rigid_body, m_collision_group, m_collision_mask);
            if (m_mass != .0f)
                current_simulation->ObjectRegistration(true, this, Simulation::EEPT_Dynamic);
            else
                current_simulation->ObjectRegistration(true, this, Simulation::EEPT_Static);
        }
        else
        {
            dynamics_world->addCollisionObject(m_collision_object, m_collision_group, m_collision_mask);
            current_simulation->ObjectRegistration(true, this, Simulation::EEPT_CollisionObject);
        }
    }
}

//Remove Physic object to the simulation
void EasyPhysic::RemoveFromSimulation(class Simulation* current_simulation)
{
    btDiscreteDynamicsWorld* dynamics_world = current_simulation->GetWorld();
    if (dynamics_world)
    {
        if (m_rigid_body)
        {
            dynamics_world->removeRigidBody(m_rigid_body);
            if (m_mass != .0f)
                current_simulation->ObjectRegistration(false, this, Simulation::EEPT_Dynamic);
            else
                current_simulation->ObjectRegistration(false, this, Simulation::EEPT_Static);
        }
        else
        {
            dynamics_world->removeCollisionObject(m_collision_object);
            if (m_ghost_object)
                current_simulation->ObjectRegistration(false, this, Simulation::EEPT_Ghost);
            current_simulation->ObjectRegistration(false, this, Simulation::EEPT_CollisionObject);
        }
    }
}

//-------------------------------------------------------------------------
//Force/Impulse functions
//--
void EasyPhysic::AddImpulse(const lol::vec3& impulse)
{
    if (m_rigid_body)
        m_rigid_body->applyCentralImpulse(LOL2BT_VEC3(impulse));
}

void EasyPhysic::AddImpulse(const lol::vec3& impulse, const lol::vec3& rel_pos)
{
    if (m_rigid_body)
        m_rigid_body->applyImpulse(LOL2BT_VEC3(impulse), LOL2BTU_VEC3(rel_pos));
}

void EasyPhysic::AddImpulseTorque(const lol::vec3& torque)
{
    if (m_rigid_body)
        m_rigid_body->applyTorqueImpulse(LOL2BT_VEC3(torque));
}

//--
void EasyPhysic::AddForce(const lol::vec3& force)
{
    if (m_rigid_body)
        m_rigid_body->applyCentralForce(LOL2BT_VEC3(force));
}

void EasyPhysic::AddForce(const lol::vec3& force, const lol::vec3& rel_pos)
{
    if (m_rigid_body)
        m_rigid_body->applyForce(LOL2BT_VEC3(force), LOL2BTU_VEC3(rel_pos));
}

void EasyPhysic::AddForceTorque(const lol::vec3& torque)
{
    if (m_rigid_body)
        m_rigid_body->applyTorque(LOL2BT_VEC3(torque));
}

//-------------------------------------------------------------------------
//Movements getter
//--
lol::vec3 EasyPhysic::GetLinearVelocity() const
{
    if (m_rigid_body)
        return BT2LOL_VEC3(m_rigid_body->getLinearVelocity());
    return lol::vec3(.0f);
}

lol::vec3 EasyPhysic::GetLinearForce() const
{
    if (m_rigid_body)
        return BT2LOL_VEC3(m_rigid_body->getTotalForce());
    return lol::vec3(.0f);
}

lol::vec3 EasyPhysic::GetAngularVelocity() const
{
    if (m_rigid_body)
        return BT2LOL_VEC3(m_rigid_body->getAngularVelocity());
    return lol::vec3(.0f);
}

lol::vec3 EasyPhysic::GetAngularForce() const
{
    if (m_rigid_body)
        return BT2LOL_VEC3(m_rigid_body->getTotalTorque());
    return lol::vec3(.0f);
}

//Set Local Inertia
void EasyPhysic::SetLocalInertia(float mass)
{
    if (mass != .0f)
        m_collision_shape->calculateLocalInertia(mass, m_local_inertia);
    else
        m_local_inertia = btVector3(.0f, .0f, .0f);
}

} /* namespace phys */

} /* namespace lol */
