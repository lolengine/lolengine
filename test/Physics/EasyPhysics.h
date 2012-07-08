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
// The EasyPhysics class
// ------------------
//

#if !defined __EASYPHYSICS_EASYPHYSICS_H__
#define __EASYPHYSICS_EASYPHYSICS_H__

#ifdef HAVE_PHYS_USE_BULLET
#include "core.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#endif

namespace lol
{

namespace phys
{

class EasyPhysics
{
#ifdef HAVE_PHYS_USE_BULLET
public:
	EasyPhysics();
	~EasyPhysics();

	void SetShapeToBox(lol::vec3& box_size);
	void SetShapeToSphere(float radius);
	void SetShapeToCone(float radius, float height);
	void SetShapeToCylinder(lol::vec3& cyl_size);
	void SetShapeToCapsule(float radius, float height);

	void SetBaseTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f)));
	void SetMass(float mass);
	void InitBodyToRigid();
	void AddToSimulation(class Simulation* current_simulation);
	mat4 GetTransform();

protected:
	void SetLocalInertia(float mass);
	void SetShapeTo(btCollisionShape* collision_shape);

	btCollisionObject*							m_collision_object;

	btRigidBody*								m_rigid_body;
	btScalar									m_mass;
	btVector3									m_local_inertia;

	btCollisionShape*							m_collision_shape;
	btMotionState*								m_motion_state;

	lol::mat4									m_local_to_world;
#else
public:
	EasyPhysics() { }

	void SetShapeToBox(lol::vec3& BoxSize) { }
	void SetShapeToSphere(float radius) { }
	void SetShapeToCone(float radius, float height) { }
	void SetShapeToCylinder(lol::vec3& cyl_size) { }
	void SetShapeToCapsule(float radius, float height) { }

	void SetBaseTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f))) { }
	void SetMass(float mass) { }
	void InitBodyToRigid() { }
	void AddToSimulation(class Simulation* current_simulation) { }
	mat4 GetTransform() { return mat4(1.0f); }
#endif
};

} /* namespace phys */

} /* namespace lol */

#endif /* __EASYPHYSICS_EASYPHYSICS_H__ */

