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
#include "LolPhysics.h"
#endif

namespace lol
{

namespace phys
{

class EasyPhysic
{

	friend class EasyConstraint;

#ifdef HAVE_PHYS_USE_BULLET

public:
	EasyPhysic();
	~EasyPhysic();

	virtual void SetShapeToBox(lol::vec3& box_size);
	virtual void SetShapeToSphere(float radius);
	virtual void SetShapeToCone(float radius, float height);
	virtual void SetShapeToCylinder(lol::vec3& cyl_size);
	virtual void SetShapeToCapsule(float radius, float height);

	virtual bool CanChangeCollisionChannel() { return (m_rigid_body == NULL); }
	virtual void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f)));
	virtual void SetMass(float mass);
	virtual void InitBodyToRigid(bool ZeroMassIsKinematic=false);
	virtual void InitBodyToGhost();
	virtual void AddToSimulation(class Simulation* current_simulation);
	virtual void RemoveFromSimulation(class Simulation* current_simulation);
	virtual mat4 GetTransform();

protected:
	virtual void SetLocalInertia(float mass);
	virtual void SetShapeTo(btCollisionShape* collision_shape);

	virtual btGhostObject* GetGhostObject();

	btCollisionObject*							m_collision_object;

	btGhostObject*								m_ghost_object;

	btRigidBody*								m_rigid_body;
	btVector3									m_local_inertia;

	btCollisionShape*							m_collision_shape;
	btConvexShape*								m_convex_shape;
	btMotionState*								m_motion_state;

#else  // NO PHYSIC IMPLEMENTATION

public:
	EasyPhysic() { }

	virtual void SetShapeToBox(lol::vec3& BoxSize) { }
	virtual void SetShapeToSphere(float radius) { }
	virtual void SetShapeToCone(float radius, float height) { }
	virtual void SetShapeToCylinder(lol::vec3& cyl_size) { }
	virtual void SetShapeToCapsule(float radius, float height) { }

	virtual bool CanChangeCollisionChannel() { return true; }
	virtual void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f))) { }
	virtual void SetMass(float mass) { }
	virtual void InitBodyToRigid() { }
	virtual void InitBodyToGhost() { }
	virtual void AddToSimulation(class Simulation* current_simulation) { }
	virtual void RemoveFromSimulation(class Simulation* current_simulation) { }
	virtual mat4 GetTransform() { return mat4(1.0f); }

	virtual void InitBodyToGhost() { }

#endif // PHYSIC IMPLEMENTATION

public:
	//Sets the collision Group & Mask.
	//Mask can change at runtime, not group !
	virtual bool SetCollisionChannel(int NewGroup, int NewMask)
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

} /* namespace phys */

} /* namespace lol */

#endif /* __EASYPHYSICS_EASYPHYSICS_H__ */

