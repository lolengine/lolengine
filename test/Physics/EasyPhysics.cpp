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

//
// The EasyMesh class
// ------------------
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "LolBtPhysicsIntegration.h"
#include "LolPhysics.h"

namespace lol
{

namespace phys
{

#ifdef HAVE_PHYS_USE_BULLET

EasyPhysics::EasyPhysics() : 
	m_collision_object(NULL),
	m_rigid_body(NULL),
	m_collision_shape(NULL),
	m_motion_state(NULL),
	m_mass(.0f),
	m_local_inertia(btVector3(.0f, .0f, .0f))
{
}

EasyPhysics::~EasyPhysics()
{
	m_rigid_body = NULL;
	delete m_collision_object;
	delete m_collision_shape;
	delete m_motion_state;
}

//-------------------------------------------------------------------------
//Set Shape functions
//--

void EasyPhysics::SetShapeTo(btCollisionShape* collision_shape)
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
void EasyPhysics::SetShapeToBox(lol::vec3& box_size)
{
	vec3 new_box_size = box_size * LOL2BT_UNIT * LOL2BT_SIZE;
	SetShapeTo(new btBoxShape(LOL2BT_VEC3(new_box_size)));
}

void EasyPhysics::SetShapeToSphere(float radius)
{
	SetShapeTo(new btSphereShape(radius * LOL2BT_UNIT * LOL2BT_SIZE));
}

void EasyPhysics::SetShapeToCone(float radius, float height)
{
	SetShapeTo(new btConeShape(	radius * LOL2BT_UNIT,
								height * LOL2BT_UNIT));
}

void EasyPhysics::SetShapeToCylinder(lol::vec3& cyl_size)
{
	vec3 new_cyl_size = cyl_size * LOL2BT_UNIT;
	new_cyl_size.y *= LOL2BT_SIZE;
	SetShapeTo(new btCylinderShape(LOL2BT_VEC3(new_cyl_size)));
}

void EasyPhysics::SetShapeToCapsule(float radius, float height)
{
	SetShapeTo(new btCapsuleShape(	radius * LOL2BT_UNIT * LOL2BT_SIZE,
									height * LOL2BT_UNIT * LOL2BT_SIZE));
}

//-------------------------------------------------------------------------
//Base Location/Rotation setup
//--
void EasyPhysics::SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation)
{
	if (m_motion_state)
	{
		if (m_mass != .0f)
			m_motion_state->setWorldTransform(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(base_location)));
		else
		{
			m_rigid_body->setWorldTransform(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(base_location)));
			m_motion_state->setWorldTransform(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(base_location)));
		}
	}
	else
		m_motion_state = new btDefaultMotionState(btTransform(LOL2BT_QUAT(base_rotation), LOL2BT_VEC3(base_location)));
}

//-------------------------------------------------------------------------
//Mass related functions
//--
//Set Shape functions
void EasyPhysics::SetMass(float mass)
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
void EasyPhysics::InitBodyToRigid()
{
	if (m_collision_object)
		delete m_collision_object;

	SetLocalInertia(m_mass);
	if (!m_motion_state)
		SetTransform(vec3(.0f));
	btRigidBody::btRigidBodyConstructionInfo NewInfos(m_mass, m_motion_state, m_collision_shape, m_local_inertia);
	m_rigid_body = new btRigidBody(NewInfos);
	m_collision_object = m_rigid_body;
}

void EasyPhysics::AddToSimulation(class Simulation* current_simulation)
{
	btDiscreteDynamicsWorld* dynamics_world = current_simulation->GetWorld();
	if (m_rigid_body)
	{
		dynamics_world->addRigidBody(m_rigid_body);
		if (m_mass != .0f)
			current_simulation->AddToDynamic(this);
		else
			current_simulation->AddToStatic(this);
	}
	else
		dynamics_world->addCollisionObject(m_collision_object);
}

//-------------------------------------------------------------------------
//Getter functons
//--

mat4 EasyPhysics::GetTransform()
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
void EasyPhysics::SetLocalInertia(float mass)
{
	if (mass != .0f)
		m_collision_shape->calculateLocalInertia(mass, m_local_inertia);
	else
		m_local_inertia = btVector3(.0f, .0f, .0f);
}
#endif

} /* namespace phys */

} /* namespace lol */
