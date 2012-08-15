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

#include "../Include/LolBtPhysicsIntegration.h"
#include "../Include/LolPhysics.h"

namespace lol
{

namespace phys
{

#ifdef HAVE_PHYS_USE_BULLET

//-------------------------------------------------------------------------
//EASY_PHYSIC
//--

EasyPhysic::EasyPhysic() : 
	m_collision_object(NULL),
	m_rigid_body(NULL),
	m_ghost_object(NULL),
	m_collision_shape(NULL),
	m_motion_state(NULL),
	m_mass(.0f),
	m_local_inertia(btVector3(.0f, .0f, .0f)),
	m_collision_group(1),
	m_collision_mask(1)
{
}

EasyPhysic::~EasyPhysic()
{
	m_rigid_body = NULL;
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
	m_convex_shape = new btConeShape(	radius * LOL2BT_UNIT,
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
void EasyPhysic::SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation)
{
	m_local_to_world = lol::mat4::translate(base_location) * mat4(base_rotation);

	if (m_ghost_object)
		m_ghost_object->setWorldTransform(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(base_location * LOL2BT_UNIT)));
	else
	{
		if (m_motion_state)
			m_motion_state->setWorldTransform(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(base_location * LOL2BT_UNIT)));
		else
			m_motion_state = new btDefaultMotionState(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(base_location * LOL2BT_UNIT)));
	}
}

//-------------------------------------------------------------------------
//Mass related functions
//--
//Set Shape functions
void EasyPhysic::SetMass(float mass)
{
	m_mass = mass;

	if (m_rigid_body)
	{
		SetLocalInertia(m_mass);
		m_rigid_body->setMassProps(mass, m_local_inertia);
	}
}

//-------------------------------------------------------------------------
//Final conversion pass functons : Body related
//--

//Init to rigid body
void EasyPhysic::InitBodyToRigid(bool SetToKinematic)
{
	if (m_collision_object)
		delete m_collision_object;

	if (!m_motion_state)
		SetTransform(vec3(.0f));

	btRigidBody::btRigidBodyConstructionInfo NewInfos(m_mass, m_motion_state, m_collision_shape, m_local_inertia);
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
btGhostObject* EasyPhysic::GetGhostObject()
{
	return new btGhostObject();
}

//Init to Ghost object, for Overlap/Sweep Test/Touching logic
void EasyPhysic::InitBodyToGhost()
{
	if (m_collision_object)
		delete m_collision_object;

	m_ghost_object = GetGhostObject();
	m_ghost_object->setCollisionShape(m_collision_shape);
	m_collision_object = m_ghost_object;
	m_collision_object->setUserPointer(this);

	SetTransform(m_local_to_world.v3.xyz, lol::quat(m_local_to_world));

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
  //           	const btManifoldPoint&pt = manifold->getContactPoint(p);
  //              if (pt.getDistance()<0.f)
		//{
		//	const btVector3& ptA = pt.getPositionWorldOnA();
		//	const btVector3& ptB = pt.getPositionWorldOnB();
		//	const btVector3& normalOnB = pt.m_normalWorldOnB;
		//	/// work here
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
			current_simulation->AddToGhost(this);
		}
		else if (m_rigid_body)
		{
			dynamics_world->addRigidBody(m_rigid_body, m_collision_group, m_collision_mask);
			if (m_mass != .0f)
				current_simulation->AddToDynamic(this);
			else
				current_simulation->AddToStatic(this);
		}
		else
			dynamics_world->addCollisionObject(m_collision_object, m_collision_group, m_collision_mask);
	}
}

//Remove Physic object to the simulation
void EasyPhysic::RemoveFromSimulation(class Simulation* current_simulation)
{
	btDiscreteDynamicsWorld* dynamics_world = current_simulation->GetWorld();
	if (dynamics_world)
	{
		if (m_rigid_body)
			dynamics_world->removeRigidBody(m_rigid_body);
		else if (m_collision_object)
			dynamics_world->removeCollisionObject(m_collision_object);
	}
}

//-------------------------------------------------------------------------
//Getter functons
//--

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

//Set Local Inertia
void EasyPhysic::SetLocalInertia(float mass)
{
	if (mass != .0f)
		m_collision_shape->calculateLocalInertia(mass, m_local_inertia);
	else
		m_local_inertia = btVector3(.0f, .0f, .0f);
}

#endif // HAVE_PHYS_USE_BULLET

} /* namespace phys */

} /* namespace lol */
