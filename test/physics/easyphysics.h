//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#endif //HAVE_PHYS_USE_BULLET

namespace lol
{

namespace phys
{

class EasyPhysic
{

    friend class Simulation;
    friend class EasyConstraint;

#ifdef HAVE_PHYS_USE_BULLET

public:
    EasyPhysic(WorldEntity* NewOwnerEntity);
    ~EasyPhysic();

    virtual void SetShapeToBox(lol::vec3& box_size);
    virtual void SetShapeToSphere(float radius);
    virtual void SetShapeToCone(float radius, float height);
    virtual void SetShapeToCylinder(lol::vec3& cyl_size);
    virtual void SetShapeToCapsule(float radius, float height);

    virtual bool CanChangeCollisionChannel() { return (m_rigid_body == NULL); }
    virtual mat4 GetTransform();
    virtual void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f)));
protected:
    virtual void BaseTransformChanged(const lol::mat4& PreviousMatrix, const lol::mat4& NewMatrix);
public:
    virtual void SetMass(float mass);
    virtual float GetMass() { return m_mass; }
    virtual void SetHitRestitution(float hit_restitution);
    virtual void InitBodyToRigid(bool ZeroMassIsKinematic=false);
    virtual void InitBodyToGhost();
    virtual void AddToSimulation(class Simulation* current_simulation);
    virtual void RemoveFromSimulation(class Simulation* current_simulation);

    //Force/Impulse functions
	virtual void AddImpulse(const lol::vec3& impulse);
    virtual void AddImpulse(const lol::vec3& impulse, const lol::vec3& rel_pos);
  	virtual void AddImpulseTorque(const lol::vec3& torque);
	virtual void AddForce(const lol::vec3& force);
	virtual void AddForce(const lol::vec3& force, const lol::vec3& rel_pos);
    virtual void AddForceTorque(const lol::vec3& torque);

    //Movements getter
	lol::vec3 GetLinearVelocity() const;
	lol::vec3 GetLinearForce() const;
	lol::vec3 GetAngularVelocity() const;
	lol::vec3 GetAngularForce() const;

protected:
    virtual void SetLocalInertia(float mass);
    virtual void SetShapeTo(btCollisionShape* collision_shape);

    virtual btGhostObject* GetGhostObjectInstance();

    btCollisionObject*                          m_collision_object;

    btGhostObject*                              m_ghost_object;

    btRigidBody*                                m_rigid_body;
    btVector3                                   m_local_inertia;

    btCollisionShape*                           m_collision_shape;
    btConvexShape*                              m_convex_shape;
    btMotionState*                              m_motion_state;

#else  // NO PHYSIC IMPLEMENTATION

public:
    EasyPhysic(WorldEntity* NewOwnerEntity) { m_owner_entity = NewOwnerEntity; }

    virtual void SetShapeToBox(lol::vec3& BoxSize) { }
    virtual void SetShapeToSphere(float radius) { }
    virtual void SetShapeToCone(float radius, float height) { }
    virtual void SetShapeToCylinder(lol::vec3& cyl_size) { }
    virtual void SetShapeToCapsule(float radius, float height) { }

    virtual bool CanChangeCollisionChannel() { return true; }
    virtual mat4 GetTransform() { return mat4(1.0f); }
    virtual void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f))) { }
private:
    virtual void BaseTransformChanged(const lol::mat4& PreviousMatrix, const lol::mat4& NewMatrix) { }
public:
    virtual void SetMass(float mass) { }
    virtual float GetMass() { return .0f; }
    virtual void SetHitRestitution(float hit_restitution) { }
    virtual void InitBodyToRigid() { }
    virtual void InitBodyToGhost() { }
    virtual void AddToSimulation(class Simulation* current_simulation) { }
    virtual void RemoveFromSimulation(class Simulation* current_simulation) { }

    //Force/Impulse functions
	virtual void AddImpulse(const lol::vec3& impulse) { }
    virtual void AddImpulse(const lol::vec3& impulse, const lol::vec3& rel_pos) { }
  	virtual void AddImpulseTorque(const lol::vec3& torque) { }
	virtual void AddForce(const lol::vec3& force) { }
	virtual void AddForce(const lol::vec3& force, const lol::vec3& rel_pos) { }
    virtual void AddForceTorque(const lol::vec3& torque) { }

    //Movements getter
    lol::vec3 GetLinearVelocity()    const { return lol::vec3(.0f); }
	lol::vec3 GetLinearForce()       const { return lol::vec3(.0f); }
	lol::vec3 GetAngularVelocity()   const { return lol::vec3(.0f); }
	lol::vec3 GetAngularForce()      const { return lol::vec3(.0f); }

    virtual void InitBodyToGhost() { }

#endif // PHYSIC IMPLEMENTATION

public:
    //Sets the collision Group & Mask.
    //Mask can change at runtime, not group !
    virtual bool SetCollisionChannel(int NewGroup, int NewMask)
    {
        if (CanChangeCollisionChannel())
        {
            m_collision_group = (1 << NewGroup);
            m_collision_mask = NewMask;
            return true;
        }
        return false;
    }
    int GetCollisionGroup() { return m_collision_group; }
    int GetCollisionMask()    { return m_collision_mask; }

    //Base/Attachment logic
    virtual void AttachTo(EasyPhysic* NewBase, bool NewBaseLockLocation = true, bool NewBaseLockRotation = true)
    {
        if (NewBase == this || (NewBase && NewBase->m_base_physic == this))
            return;

        if (NewBase)
        {
            bool bAlreadyExists = false;
            for (int i = 0; i < NewBase->m_based_physic_list.Count(); ++i)
                if (NewBase->m_based_physic_list[i] == this)
                    bAlreadyExists = true;
            if (!bAlreadyExists)
                NewBase->m_based_physic_list << this;
            m_base_physic = NewBase;
            m_base_lock_location = NewBaseLockLocation;
            m_base_lock_rotation = NewBaseLockRotation;
        }
        else if (m_base_physic)
        {
            for (int i = 0; i < m_base_physic->m_based_physic_list.Count(); ++i)
                if (m_base_physic->m_based_physic_list[i] == this)
                    m_base_physic->m_based_physic_list.Remove(i--);
            m_base_physic = NULL;
        }
    }

protected:
    lol::mat4                                       m_local_to_world;
    float                                           m_mass;
    float                                           m_hit_restitution;
    int                                             m_collision_group;
    int                                             m_collision_mask;
    WorldEntity*                                    m_owner_entity;
    Simulation*                                     m_owner_simulation;

    //Base/Attachment logic
    Array<EasyPhysic*>                              m_based_physic_list;     //List of objects based on this : this object moves, its based object MoveStep with it.
    EasyPhysic*                                     m_base_physic;           //Base for this object : The base moves, the object moves with it.
    bool                                            m_base_lock_location;    //when this is TRUE, location moves with rotation change.
    bool                                            m_base_lock_rotation;    //when this is TRUE, rotation moves with rotation change.

    //Touch logic
    Array<EasyPhysic*>                              m_touching_physic;        //Maintained by ghost objects
};

} /* namespace phys */

} /* namespace lol */

#endif /* __EASYPHYSICS_EASYPHYSICS_H__ */

