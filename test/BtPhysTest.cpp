//
// BtPhysTest
//
// Copyright: (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//            (c) 2012 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined _WIN32
#   include <direct.h>
#endif

#if defined _XBOX
#   define _USE_MATH_DEFINES /* for M_PI */
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#elif defined _WIN32
#   define _USE_MATH_DEFINES /* for M_PI */
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#else
#   include <cmath>
#endif

#include "core.h"
#include "loldebug.h"

using namespace lol;

#ifndef HAVE_PHYS_USE_BULLET
#define HAVE_PHYS_USE_BULLET
#endif /* HAVE_PHYS_USE_BULLET */

#include "Physics/Include/LolPhysics.h"
#include "Physics/Include/EasyPhysics.h"
#include "PhysicObject.h"
#include "BtPhysTest.h"

using namespace lol::phys;

#define CUBE_HALF_EXTENTS .5f
#define EXTRA_HEIGHT 1.f

int gNumObjects = 64;

#define USE_WALL		1
#define USE_PLATFORM	1
#define USE_ROPE		0
#define USE_BODIES		1
#define USE_ROTATION	0
#define USE_CHARACTER	1

#define	IPT_MOVE_FORWARD		"Move_Forward"
#define	IPT_MOVE_BACKWARD		"Move_Backward"
#define	IPT_MOVE_LEFT			"Move_Left"
#define	IPT_MOVE_RIGHT			"Move_Right"
#define	IPT_MOVE_UP				"Move_Up"
#define	IPT_MOVE_DOWN			"Move_Down"
#define	IPT_MOVE_JUMP			"Move_Jump"

BtPhysTest::BtPhysTest(bool editor)
{
	m_loop_value = .0f;

    /* Create a camera that matches the settings of XNA BtPhysTest */
    m_camera = new Camera(vec3(0.f, 600.f, 0.f),
                          vec3(0.f, 0.f, 0.f),
                          vec3(0, 1, 0));
    m_camera->SetRotation(quat::fromeuler_xyz(0.f, 0.f, 0.f));
    m_camera->SetPerspective(45.f, 1280.f, 960.f, .1f, 1000.f);
	//m_camera->SetOrtho(1280.f / 6, 960.f / 6, -1000.f, 1000.f);
    Ticker::Ref(m_camera);

    m_ready = false;

	m_simulation = new Simulation();
	m_simulation->SetWorldLimit(vec3(-1000.0f, -1000.0f, -1000.0f), vec3(1000.0f, 1000.0f, 1000.0f));
	m_simulation->Init();
	vec3 NewGravity = vec3(.0f, -10.0f, .0f);
	m_simulation->SetGravity(NewGravity);
	m_simulation->SetContinuousDetection(true);
	m_simulation->SetTimestep(1.f / 120.f);
    Ticker::Ref(m_simulation);

	float offset = 29.5f;
	vec3 pos_offset = vec3(.0f, 30.f, .0f);
	if (USE_WALL)
	{
		for (int i=0; i < 6; i++)
		{
			vec3 NewPosition = vec3(.0f);
			quat NewRotation = quat(1.f);

			PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation);

			int idx = i/2;
			NewPosition = pos_offset;
			NewPosition[idx] += offset;
			offset *= -1.f;

			if (idx != 1)
			{
				vec3 NewAxis = vec3(.0f);
				NewAxis[2 - idx] = 1;
				NewRotation = quat::rotate(90.f, NewAxis);
			}

			NewPhyobj->SetTransform(NewPosition, NewRotation);
			Ticker::Ref(NewPhyobj);
			m_ground_list << NewPhyobj;
		}
	}

	PhysicsObject* BasePhyobj = NULL;
	if (USE_PLATFORM)
	{
		quat NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
		vec3 NewPosition = pos_offset + vec3(5.0f, -25.0f, -15.0f);

		PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

		m_platform_list << NewPhyobj;
		Ticker::Ref(NewPhyobj);

		NewPosition = pos_offset + vec3(-15.0f, -25.0f, 5.0f);

		NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);
		BasePhyobj = NewPhyobj;

		m_platform_list << NewPhyobj;
		Ticker::Ref(NewPhyobj);

		NewRotation = quat::fromeuler_xyz(0.f, 0.f, 90.f);
		NewPosition = pos_offset + vec3(-20.0f, -25.0f, 5.0f);

		NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

		NewPhyobj->GetPhysic()->AttachTo(BasePhyobj->GetPhysic(), true, true);
		m_platform_list << NewPhyobj;
		Ticker::Ref(NewPhyobj);

		//NewPosition += vec3(-0.0f, .0f, .0f);
		//NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

		//NewPhyobj->GetPhysic()->AttachTo(BasePhyobj->GetPhysic(), true, false);
		//m_platform_list << NewPhyobj;
		//Ticker::Ref(NewPhyobj);

		//NewPosition += vec3(-2.0f, .0f, .0f);
		//NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 1);

		//NewPhyobj->GetPhysic()->AttachTo(BasePhyobj->GetPhysic(), false, false);
		//m_platform_list << NewPhyobj;
		//Ticker::Ref(NewPhyobj);
	}

	if (USE_CHARACTER)
	{
		quat NewRotation = quat::fromeuler_xyz(0.f, 0.f, 0.f);
		vec3 NewPosition = pos_offset + vec3(-15.0f, -10.0f, .0f);

		PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 2);

		m_character_list << NewPhyobj;
		Ticker::Ref(NewPhyobj);


		Input::LinkActionToKey(IPT_MOVE_FORWARD,		Key::Up);
		Input::LinkActionToKey(IPT_MOVE_BACKWARD,		Key::Down);
		Input::LinkActionToKey(IPT_MOVE_LEFT,			Key::Left);
		Input::LinkActionToKey(IPT_MOVE_RIGHT,			Key::Right);
		Input::LinkActionToKey(IPT_MOVE_JUMP,			Key::Space);
		Input::LinkActionToKey(IPT_MOVE_UP,				Key::PageUp);
		Input::LinkActionToKey(IPT_MOVE_DOWN,			Key::PageDown);

		//NewPhyobj->GetCharacter()->AttachTo(BasePhyobj->GetPhysic(), true, true);
	}

	if (USE_BODIES)
	{
		for (int x=0; x < 6; x++)
		{
			for (int y=0; y < 6; y++)
			{
				for (int z=0; z < 5; z++)
				{
					PhysicsObject* new_physobj = new PhysicsObject(m_simulation, 1000.f,
						vec3(-20.f, 15.f, -20.f) +
						vec3(8.f * (float)x, 8.f * (float)y, 8.f * (float)z));
					m_physobj_list << new_physobj;
					Ticker::Ref(new_physobj);
				}
			}
		}
	}

	if (USE_ROPE)
	{
		Array<PhysicsObject*> RopeElements;
		for (int i = 0; i < 14; i++)
		{
			PhysicsObject* new_physobj = new PhysicsObject(m_simulation, 1000.f,
				vec3(0.f, 15.f, -20.f) +
				vec3(0.f, 0.f, 2.f * (float)i), 1);
			RopeElements << new_physobj;
			m_physobj_list << new_physobj;
			Ticker::Ref(new_physobj);
			if (RopeElements.Count() > 1)
			{
				EasyConstraint* new_constraint = new EasyConstraint();

				vec3 A2B = .5f * (RopeElements[i]->GetPhysic()->GetTransform().v3.xyz - 
							RopeElements[i - 1]->GetPhysic()->GetTransform().v3.xyz);
				new_constraint->SetPhysObjA(RopeElements[i - 1]->GetPhysic(), lol::mat4::translate(A2B));
				new_constraint->SetPhysObjB(RopeElements[i]->GetPhysic(), lol::mat4::translate(-A2B));
				new_constraint->InitConstraintToPoint2Point();
				new_constraint->DisableCollisionBetweenObjs(true);
				new_constraint->AddToSimulation(m_simulation);
				m_constraint_list << new_constraint;
			}
		}
	}
}

void BtPhysTest::TickGame(float seconds)
{
    WorldEntity::TickGame(seconds);

    if (Input::WasReleased(Key::Escape))
        Ticker::Shutdown();

	m_loop_value += seconds;
	if (m_loop_value > M_PI * 2.0f)
		m_loop_value -= M_PI * 2.0f;

	vec3 GroundBarycenter = vec3(.0f);
	vec3 PhysObjBarycenter = vec3(.0f);
	float factor = .0f;

	if (USE_WALL)
	{
		for (int i = 0; i < m_ground_list.Count(); i++)
		{
			PhysicsObject* PhysObj = m_ground_list[i];
			mat4 GroundMat = PhysObj->GetTransform();

			GroundBarycenter += GroundMat.v3.xyz;
			factor += 1.f;
		}

		GroundBarycenter /= factor;

		for (int i = 0; i < m_ground_list.Count(); i++)
		{
			PhysicsObject* PhysObj = m_ground_list[i];

			mat4 GroundMat = PhysObj->GetTransform();
			vec3 CenterToGround = GroundMat.v3.xyz - GroundBarycenter;
			vec3 CenterToCam = m_camera->m_position - GroundBarycenter;

			if (dot(normalize(CenterToCam - CenterToGround),
					normalize(CenterToGround)) > 0.f)
				PhysObj->SetRender(false);
			else
				PhysObj->SetRender(true);
		}
	}

	if (USE_ROTATION)
	{
		for (int i = 0; i < m_ground_list.Count(); i++)
		{
			PhysicsObject* PhysObj = m_ground_list[i];

			mat4 GroundMat = PhysObj->GetTransform();
			mat4 CenterMx = mat4::translate(GroundBarycenter);
			GroundMat = inverse(CenterMx) * GroundMat;
			GroundMat = CenterMx *
						mat4(quat::fromeuler_xyz(vec3(.0f, 20.f, 20.0f) * seconds))
						* GroundMat;
			PhysObj->SetTransform(GroundMat.v3.xyz, quat(GroundMat));
		}
	}

	if (USE_PLATFORM)
	{
		for (int i = 0; i < m_platform_list.Count(); i++)
		{
			PhysicsObject* PhysObj = m_platform_list[i];

			mat4 GroundMat = PhysObj->GetTransform();
			if (i == 0)
			{
				GroundMat = GroundMat * mat4(quat::fromeuler_xyz(vec3(20.f, .0f, .0f) * seconds));
				PhysObj->SetTransform(GroundMat.v3.xyz, quat(GroundMat));
			}
			else if (i == 1)
			{
				GroundMat =
					mat4::translate(vec3(-15.0f, 5.0f, lol::cos(m_loop_value) * 8.f)) *
					mat4(quat::fromeuler_xyz(vec3(.0f, lol::cos(m_loop_value) * 20.f, .0f)));
				PhysObj->SetTransform(GroundMat.v3.xyz, quat(GroundMat));
			}
		}
	}

	if (USE_CHARACTER)
	{
		for (int i = 0; i < m_character_list.Count(); i++)
		{
			PhysicsObject* PhysObj = m_character_list[i];
			EasyCharacterController* Character = (EasyCharacterController*)PhysObj->GetCharacter();
			mat4 CtlrMx = Character->GetTransform();
			
			int HMovement = Input::GetStatus(IPT_MOVE_RIGHT) - Input::GetStatus(IPT_MOVE_LEFT);
			int VMovement = Input::GetStatus(IPT_MOVE_FORWARD) - Input::GetStatus(IPT_MOVE_BACKWARD);
			int RMovement = Input::GetStatus(IPT_MOVE_UP) - Input::GetStatus(IPT_MOVE_DOWN);
			vec3 CharMove = vec3((float)VMovement * seconds * 4.f, (float)RMovement * seconds * 10.f, (float)HMovement * seconds * 4.f);

			if (Input::WasReleased(IPT_MOVE_JUMP))
				Character->Jump();
			Character->SetMovementForFrame(CharMove);

			RayCastResult HitResult;
			if (m_simulation->RayHits(HitResult, ERT_Closest, Character->GetTransform().v3.xyz, (Character->GetTransform().v3.xyz + vec3(.0f, -1.f, .0f)), Character))
				Character->AttachTo(HitResult.m_collider_list[0], true, true);
			else
				Character->AttachTo(NULL);
		}
	}

	if (USE_CHARACTER)
	{
		PhysObjBarycenter = vec3(.0f);
		factor = .0f;

		for (int i = 0; i < m_character_list.Count(); i++)
		{
			PhysicsObject* PhysObj = m_character_list[i];
			mat4 GroundMat = PhysObj->GetTransform();

			PhysObjBarycenter += GroundMat.v3.xyz;
			factor += 1.f;
		}

		PhysObjBarycenter /= factor;

		m_camera->SetTarget(m_camera->GetTarget() + (seconds / (seconds + 0.18f)) * (PhysObjBarycenter - m_camera->GetTarget()));
		vec3 CamPosCenter = m_camera->GetTarget() + vec3(.0f, 5.0f, .0f);
		m_camera->SetPosition(CamPosCenter + normalize(m_camera->GetPosition() - CamPosCenter) * 20.0f);
	}
	else
	{
		PhysObjBarycenter = vec3(.0f);
		for (int i = 0; i < m_physobj_list.Count(); i++)
		{
			PhysicsObject* PhysObj = m_physobj_list[i];
			mat4 GroundMat = PhysObj->GetTransform();

			PhysObjBarycenter += GroundMat.v3.xyz;
			factor += 1.f;
		}

		PhysObjBarycenter /= factor;

		m_camera->SetTarget(PhysObjBarycenter);
		m_camera->SetPosition(GroundBarycenter + normalize(GroundBarycenter - PhysObjBarycenter) * 60.0f);
	}

}

void BtPhysTest::TickDraw(float seconds)
{
    WorldEntity::TickDraw(seconds);

    if (!m_ready)
    {
        /* FIXME: this object never cleans up */
        m_ready = true;
    }

    //Video::SetClearColor(vec4(0.0f, 0.0f, 0.12f, 1.0f));

}

BtPhysTest::~BtPhysTest()
{
	Ticker::Unref(m_camera);
	
	while (m_constraint_list.Count())
	{
		EasyConstraint* CurPop = m_constraint_list.Last();
		m_constraint_list.Pop();
		CurPop->RemoveFromSimulation(m_simulation);
		delete CurPop;
	}
	while (m_ground_list.Count())
	{
		PhysicsObject* CurPop = m_ground_list.Last();
		m_ground_list.Pop();
		CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
		Ticker::Unref(CurPop);
	}
	while (m_character_list.Count())
	{
		PhysicsObject* CurPop = m_character_list.Last();
		m_character_list.Pop();
		CurPop->GetCharacter()->RemoveFromSimulation(m_simulation);
		Ticker::Unref(CurPop);
	}
	while (m_platform_list.Count())
	{
		PhysicsObject* CurPop = m_platform_list.Last();
		m_platform_list.Pop();
		CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
		Ticker::Unref(CurPop);
	}
	while (m_physobj_list.Count())
	{
		PhysicsObject* CurPop = m_physobj_list.Last();
		m_physobj_list.Pop();
		CurPop->GetPhysic()->RemoveFromSimulation(m_simulation);
		Ticker::Unref(CurPop);
	}
    Ticker::Unref(m_simulation);

}

int main(int argc, char **argv)
{
    Application app("BtPhysTest", ivec2(1280, 720), 60.0f);

#if defined _MSC_VER && !defined _XBOX
    _chdir("..");
#elif defined _WIN32 && !defined _XBOX
    _chdir("../..");
#endif

    new BtPhysTest(argc > 1);
    app.ShowPointer(false);

    app.Run();

    return EXIT_SUCCESS;
}

