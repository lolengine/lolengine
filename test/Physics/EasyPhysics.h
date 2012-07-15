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

class EasyPhysic
{

#ifdef HAVE_PHYS_USE_BULLET

public:
	EasyPhysic();
	~EasyPhysic();

	void SetShapeToBox(lol::vec3& box_size);
	void SetShapeToSphere(float radius);
	void SetShapeToCone(float radius, float height);
	void SetShapeToCylinder(lol::vec3& cyl_size);
	void SetShapeToCapsule(float radius, float height);

	bool CanChangeCollisionChannel() { return (m_rigid_body == NULL); }
	void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f)));
	void SetMass(float mass);
	void InitBodyToRigid(bool ZeroMassIsKinematic=false);
	void AddToSimulation(class Simulation* current_simulation);
	mat4 GetTransform();

protected:
	void SetLocalInertia(float mass);
	void SetShapeTo(btCollisionShape* collision_shape);

	btCollisionObject*							m_collision_object;

	btRigidBody*								m_rigid_body;
	btVector3									m_local_inertia;

	btCollisionShape*							m_collision_shape;
	btMotionState*								m_motion_state;

#else  // NO PHYSIC IMPLEMENTATION

public:
	EasyPhysic() { }

	void SetShapeToBox(lol::vec3& BoxSize) { }
	void SetShapeToSphere(float radius) { }
	void SetShapeToCone(float radius, float height) { }
	void SetShapeToCylinder(lol::vec3& cyl_size) { }
	void SetShapeToCapsule(float radius, float height) { }

	bool CanChangeCollisionChannel() { return true; }
	void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f))) { }
	void SetMass(float mass) { }
	void InitBodyToRigid() { }
	void AddToSimulation(class Simulation* current_simulation) { }
	mat4 GetTransform() { return mat4(1.0f); }

#endif // PHYSIC IMPLEMENTATION

public:
	//Sets the collision Group & Mask.
	//Mask can change at runtime, not group !
	bool SetCollisionChannel(int NewGroup, int NewMask)
	{
		if (CanChangeCollisionChannel())
		{
			m_collision_group = (1<<NewGroup);
			m_collision_mask = NewMask;
			return true;
		}
		return false;
	}
	int GetCollisionGroup() { return m_collision_group; }
	int GetCollisionMask()	{ return m_collision_mask; }

protected:
	lol::mat4									m_local_to_world;
	float										m_mass;
	int											m_collision_group;
	int											m_collision_mask;
};

class EasyConstraint
{
	EasyConstraint()
	{
		//btPoint2PointConstraint(bA, bB, PivotA, PivotB)
		//btHingeConstraint(bA, bB, TransfA, TransfB, UseRefA)
		//btSliderConstraint(bA, bB, TransfA, TransfB, UseRefA)
		//btConeTwistConstraint(bA, bB, TransfA, TransfB)
		//btGeneric6DofConstraint(bA, bB, TransfA, TransfB, UseRefA)
	}

#ifdef HAVE_PHYS_USE_BULLET

	btTypedConstraint*			m_typed_constraint;

#else  // NO PHYSIC IMPLEMENTATION

#endif // PHYSIC IMPLEMENTATION

};

} /* namespace phys */

} /* namespace lol */

#endif /* __EASYPHYSICS_EASYPHYSICS_H__ */

