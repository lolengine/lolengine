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
// The BulletCharacterController class
// ------------------
// This class is a equivalent of btKinematicCharacterController, but more useful for Lol.
//

#if !defined __BULLETCHARACTERCONTROLLER_BULLETCHARACTERCONTROLLER_H__
#define __BULLETCHARACTERCONTROLLER_BULLETCHARACTERCONTROLLER_H__

#include <lol/main.h>
#include "easyphysics.h"
//#include "BulletDynamics\Character\btCharacterControllerInterface.h"

#define USE_LOL_CTRLR_CHARAC

namespace lol
{

    namespace phys
    {

#ifdef USE_LOL_CTRLR_CHARAC

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
        btCollisionObject*    m_me;
        const vec3            m_up;
        float                m_min_slope_dot;
    };

        ///BulletKinematicCharacterController is an object that supports a sliding motion in a world.
        ///It uses a ghost object and convex sweep test to test for upcoming collisions. This is combined with discrete collision detection to recover from penetrations.
        ///Interaction between btKinematicCharacterController and dynamic rigid bodies needs to be explicity implemented by the user.
        class BulletKinematicCharacterController : public btActionInterface
        {
        public:
            BulletKinematicCharacterController(btPairCachingGhostObject* NewGhostObject, btConvexShape* NewConvexShape, float NewStepHeight, int NewUpAxis=1)
            {
                m_convex_shape = NewConvexShape;
                m_i_up_axis = NewUpAxis;
                m_ghost_object = NewGhostObject;
                m_step_height = NewStepHeight;

                m_added_margin = 0.02f;
                m_walk_direction = vec3(.0f, .0f, .0f);
                m_do_gobject_sweep_test = true;
                m_turn_angle = .0f;
                m_use_walk_direction = false; // Should remove walk direction, this doesn't work correctly.
                m_velocity_time_interval = .0f;
                m_vertical_velocity = .0f;
                m_vertical_offset = .0f;
                m_f_gravity = 9.8f * 3.f; // 3G acceleration.
                m_fall_speed = 55.f; // Terminal velocity of a sky diver in m/s.
                m_jump_speed = 10.f; // ?
                m_was_on_ground = false;
                m_was_jumping = false;
                SetMaxSlope(45.f);
            }
            ~BulletKinematicCharacterController() { }

        protected:

            static vec3* GetUpAxisDirections()
            {
                static vec3 sUpAxisDirection[3] = { vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f) };

                return sUpAxisDirection;
            }

            //--------------------------
            //CONVENIENCE FUNCTIONS
            //--

            //Returns the reflection Direction of a ray going 'Direction' hitting a surface with Normal 'Normal' from: http://www-cs-students.stanford.edu/~adityagp/final/node3.html
            vec3 GetReflectedDir(const vec3& Direction, const vec3& Normal)
            {
                return Direction - (2.f * dot(Direction, Normal) * Normal);
            }
            //Returns the portion of 'direction' that is parallel to 'normal'
            vec3 ProjectDirOnNorm(const vec3& Direction, const vec3& Normal)
            {
                return Normal * dot(Direction, Normal);
            }
            //Returns the portion of 'Direction' that is perpindicular to 'Normal'
            vec3 ProjectDirOnNormPerpindicular(const vec3& Direction, const vec3& Normal)
            {
                return Direction - ProjectDirOnNorm(Direction, Normal);
            }
            //Returns Ghost Object. -duh-
            btPairCachingGhostObject* GetGhostObject()
            {
                return m_ghost_object;
            }

            //"Real" war functions
            bool RecoverFromPenetration(btCollisionWorld* CollisionWorld);
            void UpdateTargetOnHit(const vec3& hit_normal, float TangentMag = .0f, float NormalMag = 1.f);
            void DoMove(btCollisionWorld* CollisionWorld, const vec3& MoveStep, float DeltaTime);

        public:
            ///btActionInterface interface : KEEP IN camelCase
            virtual void updateAction(btCollisionWorld* CollisionWorld, float deltaTime)
            {
                PreStep(CollisionWorld);
                PlayerStep(CollisionWorld, deltaTime);
            }

            //not in the interface, but called above
            void PreStep(btCollisionWorld* CollisionWorld);
            void PlayerStep(btCollisionWorld* CollisionWorld, float DeltaTime);

            ///btActionInterface interface : KEEP IN camelCase
            void debugDraw(btIDebugDraw* debugDrawer) { }

            void SetUpAxis(int NewAxis)
            {
                if (NewAxis < 0)
                    NewAxis = 0;
                if (NewAxis > 2)
                    NewAxis = 2;
                m_i_up_axis = NewAxis;
            }

            //!!!!!! SHOULD DITCH THAT !!!!!!
            //This should probably be called setPositionIncrementPerSimulatorStep.
            //This is neither a Direction nor a velocity, but the amount to
            //increment the position each simulation iteration, regardless
            //of DeltaTime.
            //This call will Reset any velocity set by SetVelocityForTimeInterval().
            virtual void SetWalkDirection(const vec3& walkDirection)
            {
                m_use_walk_direction = true;
                m_walk_direction = walkDirection;
                m_normalized_direction = normalize(m_walk_direction);
            }

            //Caller provides a velocity with which the character should MoveStep for
            //the given time period.  After the time period, velocity is Reset
            //to zero.
            //This call will Reset any walk Direction set by SetWalkDirection().
            //Negative time intervals will result in no motion.
            virtual void SetVelocityForTimeInterval(const vec3& velocity, float timeInterval)
            {
                m_use_walk_direction = false;
                m_walk_direction = velocity;
                m_normalized_direction = normalize(m_walk_direction);
                m_velocity_time_interval = timeInterval;
            }

            //Usefulness ?
            void Reset() { }
            void Warp(const vec3& NewOrigin)
            {
                btTransform NewTransform;
                NewTransform.setIdentity();
                NewTransform.setOrigin(LOL2BTU_VEC3(NewOrigin));
                m_ghost_object->setWorldTransform(NewTransform);
            }

            //External Setup
            //--

            void SetFallSpeed(float NewFallSpeed)            { m_fall_speed = NewFallSpeed; }
            void SetJumpSpeed(float NewJumpSpeed)            { m_jump_speed = NewJumpSpeed; }
            void SetMaxJumpHeight(float NewMaxJumpHeight)    { m_max_jump_height = NewMaxJumpHeight; }

            //Jump logic will go in EasyCC
            bool CanJump() const                            { return OnGround(); }
            void Jump();

            //NewGravity functions
            void SetGravity(float NewGravity)                { m_f_gravity = NewGravity; }
            float GetGravity() const                        { return m_f_gravity; }

            //The max slope determines the maximum angle that the controller can walk up.
            //The slope angle is measured in radians.
            void SetMaxSlope(float NewSlopeRadians)            { m_max_slope_radians = NewSlopeRadians; m_max_slope_cosine = lol::cos(NewSlopeRadians); }
            float GetMaxSlope() const                        { return m_max_slope_radians; }

            void SetUseGhostSweepTest(bool UseGObjectSweepTest) { m_do_gobject_sweep_test = UseGObjectSweepTest; }

            bool OnGround() const                            { return m_vertical_velocity == .0f && m_vertical_offset == .0f; }

        private:

            btPairCachingGhostObject*    m_ghost_object;
            btConvexShape*                m_convex_shape; //is also in m_ghost_object, but it needs to be convex, so we store it here to avoid upcast

            //keep track of the contact manifolds
            btManifoldArray                m_manifold_array;

            float                        m_half_height;
            float                        m_velocity_time_interval;
            float                        m_vertical_velocity;
            float                        m_vertical_offset;
            float                        m_fall_speed;
            float                        m_jump_speed;
            float                        m_max_jump_height;
            float                        m_max_slope_radians; // Slope angle that is set (used for returning the exact value)
            float                        m_max_slope_cosine;  // Cosine equivalent of m_max_slope_radians (calculated once when set, for optimization)
            float                        m_f_gravity;
            float                        m_turn_angle;
            float                        m_step_height;
            float                        m_added_margin;//@todo: remove this and fix the code

            ///this is the desired walk Direction, set by the user
            vec3                        m_walk_direction;
            vec3                        m_normalized_direction;

            //some internal variables
            vec3                        m_current_position;
            float                         m_current_step_offset;
            vec3                        m_target_position;

            vec3                        m_touching_normal;
            bool                        m_touching_contact;

            bool                        m_was_on_ground;
            bool                         m_was_jumping;
            bool                        m_do_gobject_sweep_test;
            bool                        m_use_walk_direction;
            int                            m_i_up_axis;

            //---------------------------------------------------------------------
            //NEW INTERNAL VARS
            //---------------------------------------------------------------------

            //Gravity in vec3
            vec3                        m_gravity;

            //Current Velocity
            vec3                        m_velocity;
        };

#endif // USE_LOL_CTRLR_CHARAC

    } /* namespace phys */

} /* namespace lol */

#endif /* __BULLETCHARACTERCONTROLLER_BULLETCHARACTERCONTROLLER_H__ */

