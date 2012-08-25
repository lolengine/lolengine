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

#define USE_LOL_CTRLR_CHARAC

#ifdef HAVE_PHYS_USE_BULLET
#include "core.h"
#include <stdio.h>
#include "../Include/LolBtPhysicsIntegration.h"
#include "../Include/LolPhysics.h"
#include "../Include/EasyCharacterController.h"
#include "../Include/BulletCharacterController.h"
//#include "LinearMath/btIDebugDraw.h"
//#include "BulletCollision/CollisionDispatch/btGhostObject.h"
//#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
//#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
//#include "BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h"
//#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
//#include "LinearMath/btDefaultMotionState.h"
#endif //HAVE_PHYS_USE_BULLET


namespace lol
{

namespace phys
{

#ifdef USE_LOL_CTRLR_CHARAC
#ifdef HAVE_PHYS_USE_BULLET

//SweepCallback used for Swweep Tests.
class ClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	ClosestNotMeConvexResultCallback(btCollisionObject* NewMe, const vec3& NewUp, float MinSlopeDot) :
					btCollisionWorld::ClosestConvexResultCallback(LOL2BTU_VEC3(vec3(.0f)), LOL2BTU_VEC3(vec3(.0f))),
					m_me(NewMe),
					m_up(NewUp),
					m_min_slope_dot(MinSlopeDot) { }

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& ConvexResult, bool NormalInWorld)
	{
		//We hit ourselves, FAIL
		if (ConvexResult.m_hitCollisionObject == m_me)
			return btScalar(1.f);

		vec3 WorldHitNomal(.0f);
		if (NormalInWorld)
			WorldHitNomal = BT2LOL_VEC3(ConvexResult.m_hitNormalLocal);
		else //need to transform Normal into worldspace
		{
			btVector3 TmpWorldHitNormal = ConvexResult.m_hitCollisionObject->getWorldTransform().getBasis() * ConvexResult.m_hitNormalLocal;
			WorldHitNomal = BT2LOL_VEC3(TmpWorldHitNormal);
		}

		float DotUp = dot(m_up, WorldHitNomal);
		//We hit below the accepted slope_dot, FAIL
		if (DotUp < m_min_slope_dot)
			return btScalar(1.f);

		//Continue to next.
		return ClosestConvexResultCallback::addSingleResult(ConvexResult, NormalInWorld);
	}
protected:
	btCollisionObject*	m_me;
	const vec3			m_up;
	float				m_min_slope_dot;
};

//When called, will try to remove Character controller from its collision.
bool BulletKinematicCharacterController::RecoverFromPenetration(btCollisionWorld* CollisionWorld)
{
	bool HasPenetration = false;

	//Retrieve all pair with us colliding.
	CollisionWorld->getDispatcher()->dispatchAllCollisionPairs(m_ghost_object->getOverlappingPairCache(), CollisionWorld->getDispatchInfo(), CollisionWorld->getDispatcher());
	m_current_position = BT2LOLU_VEC3(m_ghost_object->getWorldTransform().getOrigin());
	
	float MaxPen = .0f;
	for (int i = 0; i < m_ghost_object->getOverlappingPairCache()->getNumOverlappingPairs(); i++)
	{
		m_manifold_array.resize(0);

		//this is the equivalent of the "Touch algorithm". Maybe refactor ?
		btBroadphasePair* CollisionPair = &m_ghost_object->getOverlappingPairCache()->getOverlappingPairArray()[i];
		if (CollisionPair->m_algorithm)
			CollisionPair->m_algorithm->getAllContactManifolds(m_manifold_array);
		
		for (int j = 0; j < m_manifold_array.size(); ++j)
		{
			btPersistentManifold* CurMfold = m_manifold_array[j];
			//Normal direction differs if we're Body0
			float DirSign = CurMfold->getBody0() == m_ghost_object ? -1.f : 1.f;

			for (int k = 0; k < CurMfold->getNumContacts(); k++)
			{
				const btManifoldPoint& MfPoint = CurMfold->getContactPoint(k);
				float Dist = MfPoint.getDistance();
				if (Dist < .0f)
				{
					if (Dist < MaxPen)
					{
						MaxPen = Dist;
						m_touching_normal = BT2LOL_VEC3(MfPoint.m_normalWorldOnB) * DirSign;
					}
					m_current_position += BT2LOL_VEC3(MfPoint.m_normalWorldOnB) * DirSign * Dist * .2f;
					HasPenetration = true;
				}
			}
		}
	}

	btTransform GObjMx = m_ghost_object->getWorldTransform();
	GObjMx.setOrigin(LOL2BTU_VEC3(m_current_position));
	m_ghost_object->setWorldTransform(GObjMx);

	return HasPenetration;
}

//When the Controller hits a wall, we modify the target so the controller will MoveStep along the wall.
void BulletKinematicCharacterController::UpdateTargetOnHit(const vec3& HitNormal, float TangentMag, float NormalMag)
{
	vec3 Movedir = m_target_position - m_current_position;
	float MoveLength = (float)length(Movedir);

	if (MoveLength > SIMD_EPSILON)
	{
		Movedir = normalize(Movedir);

		vec3 ReflectDir = normalize(GetReflectedDir(Movedir, HitNormal));
		vec3 ParallelDir = ProjectDirOnNorm(ReflectDir, HitNormal);
		vec3 PerpindicularDir = ProjectDirOnNormPerpindicular(ReflectDir, HitNormal);

		m_target_position = m_current_position;

		if (NormalMag != .0f)
			m_target_position += PerpindicularDir * NormalMag * MoveLength;
	}
}

//Handles the Step-Up : Currently taking into account Stair step & Jump.
void BulletKinematicCharacterController::StepUp(btCollisionWorld* world)
{
	// phase 1: up
	vec3 UpDir = GetUpAxisDirections()[m_up_axis];
	btTransform SweepStart, SweepEnd;
	SweepStart.setIdentity();
	SweepEnd.setIdentity();

	m_target_position = m_current_position + UpDir * (m_step_height + (m_vertical_offset > 0.f ? m_vertical_offset : 0.f));

	/* FIXME: Handle HasPenetration properly */
	SweepStart.setOrigin(LOL2BTU_VEC3(m_current_position + UpDir * (m_convex_shape->getMargin() + m_added_margin)));
	SweepEnd.setOrigin(LOL2BTU_VEC3(m_target_position));

	ClosestNotMeConvexResultCallback SweepCallback(m_ghost_object, -UpDir, float(0.7071));
	SweepCallback.m_collisionFilterGroup = GetGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
	SweepCallback.m_collisionFilterMask = GetGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;
	
	if (m_do_gobject_sweep_test)
		m_ghost_object->convexSweepTest(m_convex_shape, SweepStart, SweepEnd, SweepCallback, world->getDispatchInfo().m_allowedCcdPenetration);
	else
		world->convexSweepTest(m_convex_shape, SweepStart, SweepEnd, SweepCallback);
	
	if (SweepCallback.hasHit())
	{
		// Only modify the position if the hit was a slope and not a wall or ceiling.
		if(SweepCallback.m_hitNormalWorld.dot(LOL2BTU_VEC3(UpDir)) > .0f)
		{
			// we moved up only a Fraction of the step height
			m_current_step_offset = m_step_height * SweepCallback.m_closestHitFraction;
			btVector3 InterpolPos; //TODO : REPLACE BY INTERPOLATE3/LERP(VEC3)
			InterpolPos.setInterpolate3(LOL2BTU_VEC3(m_current_position), LOL2BTU_VEC3(m_target_position), SweepCallback.m_closestHitFraction);
			m_current_position = BT2LOLU_VEC3(InterpolPos);
		}
		m_vertical_velocity = .0f;
		m_vertical_offset = .0f;
	}
	else
	{
		m_current_step_offset = m_step_height;
		m_current_position = m_target_position;
	}
}

//Handles the actual Movement. It actually moves in the 3 dimensions, function name is confusing.
void BulletKinematicCharacterController::StepForwardAndStrafe(btCollisionWorld* CollisionWorld, const vec3& MoveStep)
{
	// phase 2: forward and strafe
	m_target_position = m_current_position + MoveStep;
	btTransform SweepStart, SweepEnd;
	SweepStart.setIdentity();
	SweepEnd.setIdentity();

	float Fraction = 1.f;
	float SqDist = .0f;

	if (m_touching_contact && dot(m_normalized_direction, m_touching_normal) > .0f)
		UpdateTargetOnHit(m_touching_normal);

	//Let's loop on movement, until Movement fraction if below 0.01, which means we've reached our destination.
	//Or until we'tried 10 times.
	int MaxMoveLoop = 10;
	while (Fraction > .01f && MaxMoveLoop-- > 0)
	{
		SweepStart.setOrigin(LOL2BTU_VEC3(m_current_position));
		SweepEnd.setOrigin(LOL2BTU_VEC3(m_target_position));
		vec3 SweepDirNeg(m_current_position - m_target_position);

		ClosestNotMeConvexResultCallback SweepCallback(m_ghost_object, SweepDirNeg, .0f);
		SweepCallback.m_collisionFilterGroup = GetGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
		SweepCallback.m_collisionFilterMask = GetGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

		//The sweep test is done with an added margin, so we use it and then discard it
		float SavedMargin = m_convex_shape->getMargin();
		m_convex_shape->setMargin(SavedMargin + m_added_margin); //Apply Added Margin
		if (m_do_gobject_sweep_test)
			m_ghost_object->convexSweepTest (m_convex_shape, SweepStart, SweepEnd, SweepCallback, CollisionWorld->getDispatchInfo().m_allowedCcdPenetration);
		else
			CollisionWorld->convexSweepTest (m_convex_shape, SweepStart, SweepEnd, SweepCallback, CollisionWorld->getDispatchInfo().m_allowedCcdPenetration);
		m_convex_shape->setMargin(SavedMargin); //Restore saved margin

		Fraction -= SweepCallback.m_closestHitFraction;

		if (SweepCallback.hasHit())
		{	
			//We moved only a Fraction
			float HitDist = (float)length(BT2LOLU_VEC3(SweepCallback.m_hitPointWorld) - m_current_position);

			UpdateTargetOnHit(BT2LOL_VEC3(SweepCallback.m_hitNormalWorld));
			vec3 NewDir = m_target_position - m_current_position;
			SqDist = sqlength(NewDir);
			if (SqDist > SIMD_EPSILON)
			{
				NewDir = normalize(NewDir);
				//See Quake2: "If velocity is against original velocity, stop ead to avoid tiny oscilations in sloping corners."
				if (dot(NewDir, m_normalized_direction) <= .0f)
					break;
			}
			else
				break;
		}
		else //We moved whole way
			m_current_position = m_target_position;
	}
}

//Handles the Step-down : We go back on the ground at the end of the MoveStep.
void BulletKinematicCharacterController::StepDown(btCollisionWorld* CollisionWorld, float DeltaTime)
{
	// phase 3: down
	vec3 UpDir = GetUpAxisDirections()[m_up_axis];
	btTransform SweepStart, SweepEnd;
	SweepStart.setIdentity();
	SweepEnd.setIdentity();

	float DownVel = (m_vertical_velocity < 0.f ? -m_vertical_velocity : 0.f) * DeltaTime;
	if (DownVel > .0f && DownVel < m_step_height && (m_was_on_ground || !m_was_jumping))
		DownVel = m_step_height;

	vec3 StepDrop = UpDir * (m_current_step_offset + DownVel);
	m_target_position -= StepDrop;

	SweepStart.setOrigin(LOL2BTU_VEC3(m_current_position));
	SweepEnd.setOrigin(LOL2BTU_VEC3(m_target_position));

	ClosestNotMeConvexResultCallback SweepCallback(m_ghost_object, UpDir, m_max_slope_cosine);
	SweepCallback.m_collisionFilterGroup = GetGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
	SweepCallback.m_collisionFilterMask = GetGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;
	
	if (m_do_gobject_sweep_test)
		m_ghost_object->convexSweepTest(m_convex_shape, SweepStart, SweepEnd, SweepCallback, CollisionWorld->getDispatchInfo().m_allowedCcdPenetration);
	else
		CollisionWorld->convexSweepTest(m_convex_shape, SweepStart, SweepEnd, SweepCallback, CollisionWorld->getDispatchInfo().m_allowedCcdPenetration);

	if (SweepCallback.hasHit())
	{
		// we dropped a Fraction of the height -> hit floor
		btVector3 InterpolPos; //TODO : REPLACE BY INTERPOLATE3/LERP(VEC3)
		InterpolPos.setInterpolate3(LOL2BTU_VEC3(m_current_position), LOL2BTU_VEC3(m_target_position), SweepCallback.m_closestHitFraction);
		m_current_position = BT2LOLU_VEC3(InterpolPos);
		m_vertical_velocity = .0f;
		m_vertical_offset = .0f;
		m_was_jumping = false;
	}
	else // we dropped the full height
		m_current_position = m_target_position;
}

//The PreStepis done in order to recover from any HasPenetration.
void BulletKinematicCharacterController::PreStep(btCollisionWorld* CollisionWorld)
{
	int MaxPenetrationLoop = 0;
	m_touching_contact = false;

	while (RecoverFromPenetration(CollisionWorld))
	{
		MaxPenetrationLoop++;
		m_touching_contact = true;
		if (MaxPenetrationLoop > 4)
			break;
	}

	m_current_position = BT2LOLU_VEC3(m_ghost_object->getWorldTransform().getOrigin());
	m_target_position = m_current_position;
}

//And so we step :
//StepUpfirst, then movement, then StepDownon the ground.
void BulletKinematicCharacterController::PlayerStep(btCollisionWorld* CollisionWorld, float DeltaTime)
{
	// quick check...
	if (!m_use_walk_direction && m_velocity_time_interval <= .0f)
		return;		// no motion

	m_was_on_ground = OnGround();

	// Update fall velocity.
	m_vertical_velocity -= m_gravity * DeltaTime;
	if(m_vertical_velocity > .0f && m_vertical_velocity > m_jump_speed)
		m_vertical_velocity = m_jump_speed;

	if(m_vertical_velocity < .0f && btFabs(m_vertical_velocity) > btFabs(m_fall_speed))
		m_vertical_velocity = -btFabs(m_fall_speed);
	m_vertical_offset = m_vertical_velocity * DeltaTime;

	btTransform NewTransform;
	NewTransform = m_ghost_object->getWorldTransform();

	vec3 MoveStep(.0f);
	if (m_use_walk_direction)
		MoveStep = m_walk_direction;
	else
	{
		//Still have some time left for moving!
		float dtMoving = (DeltaTime < m_velocity_time_interval) ? DeltaTime : m_velocity_time_interval;
		m_velocity_time_interval -= DeltaTime;

		// how far will we MoveStep while we are moving?
		MoveStep = m_walk_direction * dtMoving;
	}

	//Okay, step !
	StepUp(CollisionWorld);
	StepForwardAndStrafe(CollisionWorld, MoveStep);
	StepDown(CollisionWorld, DeltaTime);

	//Movement finished, update World transform
	NewTransform.setOrigin(LOL2BTU_VEC3(m_current_position));
	m_ghost_object->setWorldTransform(NewTransform);
}

//should MoveStep Jump logic in EasyCC
void BulletKinematicCharacterController::Jump()
{
	if (!CanJump())
		return;

	m_vertical_velocity = m_jump_speed;
	m_was_jumping = true;
}

#endif // HAVE_PHYS_USE_BULLET
#endif // USE_LOL_CTRLR_CHARAC

	} /* namespace phys */

} /* namespace lol */
