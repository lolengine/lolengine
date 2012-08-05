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

#include "Physics/LolPhysics.h"
#include "Physics/EasyPhysics.h"
#include "PhysicObject.h"
#include "BtPhysTest.h"

using namespace lol::phys;

#define CUBE_HALF_EXTENTS .5f
#define EXTRA_HEIGHT 1.f

int gNumObjects = 64;

BtPhysTest::BtPhysTest(bool editor)
{
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
	m_simulation->Init();
	vec3 NewGravity = vec3(.0f, -10.0f, .0f);
	m_simulation->SetGravity(NewGravity);
	m_simulation->SetContinuousDetection(true);
	m_simulation->SetTimestep(1.f / 120.f);
    Ticker::Ref(m_simulation);

	float offset = 29.5f;
	vec3 pos_offset = vec3(.0f, 30.f, .0f);
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
			vec3 axis = vec3(.0f);
			axis[2 - idx] = 1;
			NewRotation = quat::rotate(90.f, axis);
		}

		NewPhyobj->SetTransform(NewPosition, NewRotation);
		Ticker::Ref(NewPhyobj);
		m_ground_list << NewPhyobj;
	}

	{
		quat NewRotation = quat(1.f);
		vec3 NewPosition = pos_offset + vec3(5.0f, -20.0f, -15.0f);

		PhysicsObject* NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 0);

		m_platform_list << NewPhyobj;
		Ticker::Ref(NewPhyobj);

		NewPosition = pos_offset + vec3(-20.0f, -25.0f, 5.0f);

		NewPhyobj = new PhysicsObject(m_simulation, NewPosition, NewRotation, 0);

		m_platform_list << NewPhyobj;
		Ticker::Ref(NewPhyobj);
	}

	if (1)
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

	if (1)
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

#if 0
	//init Physics
	{
		m_bt_ccd_mode = USE_CCD;

		//collision configuration contains default setup for memory, collision setup
		m_bt_collision_config = new btDefaultCollisionConfiguration();

		//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_bt_dispatcher = new btCollisionDispatcher(m_bt_collision_config);
		m_bt_dispatcher->registerCollisionCreateFunc(BOX_SHAPE_PROXYTYPE,
													BOX_SHAPE_PROXYTYPE,
													m_bt_collision_config->getCollisionAlgorithmCreateFunc(CONVEX_SHAPE_PROXYTYPE,
																											CONVEX_SHAPE_PROXYTYPE));

		m_bt_broadphase = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		m_bt_solver = new btSequentialImpulseConstraintSolver;

		m_bt_world = new btDiscreteDynamicsWorld(m_bt_dispatcher, m_bt_broadphase, m_bt_solver, m_bt_collision_config);
		//m_bt_world->setDebugDrawer(&sDebugDrawer);
		m_bt_world->getSolverInfo().m_splitImpulse = true;
		m_bt_world->getSolverInfo().m_numIterations = 20;

		m_bt_world->getDispatchInfo().m_useContinuous = (m_bt_ccd_mode == USE_CCD);
		m_bt_world->setGravity(btVector3(0,-10,0));

		///create a few basic rigid bodies
		btBoxShape* box = new btBoxShape(btVector3(btScalar(110.),btScalar(1.),btScalar(110.)));
		btCollisionShape* groundShape = box;
		m_bt_collision_shapes << groundShape;
        m_ground_mesh.Compile("[sc#ddd afcb220 2 220 -1]");

		//m_bt_collision_shapes << new btCylinderShape(btVector3(.5f,.5f,.5f));

		btTransform groundTransform;
		groundTransform.setIdentity();

		//We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
		{
			btScalar mass(0.);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0,0,0);
			if (isDynamic)
				groundShape->calculateLocalInertia(mass,localInertia);

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			m_bt_world->addRigidBody(body);
		}

		//Adding Shapes
		{
			//create a few dynamic rigidbodies
			// Re-using the same collision is better for memory usage and performance
			btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
			m_rigid_mesh[0].Compile("[sc#add afcb2 2 2 -.1]");
			m_rigid_mesh[1].Compile("[sc#dad afcb2 2 2 -.1]");
			m_rigid_mesh[2].Compile("[sc#dda afcb2 2 2 -.1]");
			m_rigid_mesh[3].Compile("[sc#daa afcb2 2 2 -.1]");
			m_rigid_mesh[4].Compile("[sc#ada afcb2 2 2 -.1]");
			m_rigid_mesh[5].Compile("[sc#aad afcb2 2 2 -.1]");

			m_bt_collision_shapes << colShape;
			m_bt_dynamic_shapes << colShape;

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();
			btScalar mass(1.f);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0,0,0);
			if (isDynamic)
				colShape->calculateLocalInertia(mass,localInertia);

			int i;
			for (i=0;i<gNumObjects;i++)
			{
				btCollisionShape* shape = colShape;
				btTransform trans;
				trans.setIdentity();

				//stack them
				int colsize = 10;
				int row = int(((float)i*CUBE_HALF_EXTENTS*2.0f)/((float)colsize*2.0f*CUBE_HALF_EXTENTS));
				int row2 = row;
				int col = (i)%(colsize)-colsize/2;

				if (col>3)
				{
					col=11;
					row2 |=1;
				}

				btVector3 pos(((row+col+row2) % 4)*CUBE_HALF_EXTENTS,
				              20.0f + row*8*CUBE_HALF_EXTENTS+CUBE_HALF_EXTENTS+EXTRA_HEIGHT,
				              col*8*CUBE_HALF_EXTENTS + 2 * (row2%2)*CUBE_HALF_EXTENTS);

				trans.setOrigin(pos);
	
				float mass = 1.f;


				btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

				//rigidbody is dynamic if and only if mass is non zero, otherwise static
				bool isDynamic = (mass != 0.f);

				btVector3 localInertia(0,0,0);
				if (isDynamic)
					shape->calculateLocalInertia(mass,localInertia);

				//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

				btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);

				btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);

				btRigidBody* body = new btRigidBody(cInfo);
				body->setContactProcessingThreshold(BT_LARGE_FLOAT);

				m_bt_world->addRigidBody(body);

				///when using m_ccdMode
				if (m_bt_ccd_mode == USE_CCD)
				{
					body->setCcdMotionThreshold(CUBE_HALF_EXTENTS);
					body->setCcdSweptSphereRadius(0.9*CUBE_HALF_EXTENTS);
				}
			}
		}
	}
#endif
}

void BtPhysTest::TickGame(float seconds)
{
    WorldEntity::TickGame(seconds);

    if (Input::GetButtonState(27 /*SDLK_ESCAPE*/))
        Ticker::Shutdown();

	vec3 GroundBarycenter = vec3(.0f);
	vec3 PhysObjBarycenter = vec3(.0f);
	float factor = .0f;
	
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

	if (0)
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
			else
			{
				GroundMat = GroundMat * mat4::translate(vec3(.0f, .0f, 10.0f) * seconds);
				if (GroundMat.v3.z > 40.0f)
					GroundMat = GroundMat * mat4::translate(vec3(.0f, .0f, -80.0f));
				PhysObj->SetTransform(GroundMat.v3.xyz, quat(GroundMat));
			}
		}
	}

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

#if 0
	///step the simulation
	if (m_bt_world)
	{
		//int steps = (int)(seconds / 0.005f);
		//for (int i = 0; i < steps; i++)
			m_bt_world->stepSimulation(seconds /*/ steps*/);
		//optional but useful: debug drawing
		//m_bt_world->debugDrawWorld();
	}
#endif
}

void BtPhysTest::TickDraw(float seconds)
{
    WorldEntity::TickDraw(seconds);

    if (!m_ready)
    {
#if 0
		m_ground_mesh.MeshConvert();
		m_rigid_mesh[0].MeshConvert();
		m_rigid_mesh[1].MeshConvert();
		m_rigid_mesh[2].MeshConvert();
		m_rigid_mesh[3].MeshConvert();
		m_rigid_mesh[4].MeshConvert();
		m_rigid_mesh[5].MeshConvert();
#endif
        /* FIXME: this object never cleans up */
        m_ready = true;
    }

    Video::SetClearColor(vec4(0.0f, 0.0f, 0.12f, 1.0f));

#if 0
	vec3 BarycenterLocation = vec3(.0f);
	float BarycenterFactor = 0.0f;
	for(int i=0;i<gNumObjects;i++)
	{
		mat4 m(1.0f);
		btMatrix3x3	rot; rot.setIdentity();
		btCollisionObject*	colObj = m_bt_world->getCollisionObjectArray()[i];
		btRigidBody*		body = btRigidBody::upcast(colObj);
		if(body && body->getMotionState())
		{
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(&m[0][0]);
			rot = myMotionState->m_graphicsWorldTrans.getBasis();
		}
		else
		{
			colObj->getWorldTransform().getOpenGLMatrix(&m[0][0]);
			rot = colObj->getWorldTransform().getBasis();
		}
		if (i > 0)
		{
			BarycenterLocation += m.v3.xyz;
			BarycenterFactor += 1.0f;
		}
		if (i == 0)
			m_ground_mesh.Render(m);
		else 
			m_rigid_mesh[i % 6].Render(m);
	}
	if (BarycenterFactor > .0f)
	{
		BarycenterLocation /= BarycenterFactor;

		m_camera->SetTarget(BarycenterLocation);
		m_camera->SetPosition(BarycenterLocation + vec3(-20.0f, 8.0f, .0f));
	}
#endif
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

#if 0
	//Exit Physics
	{
		//cleanup in the reverse order of creation/initialization
		//remove the rigidbodies from the dynamics world and delete them
		for (int i = m_bt_world->getNumCollisionObjects() - 1; i >= 0 ;i--)
		{
			btCollisionObject* obj = m_bt_world->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
				delete body->getMotionState();

			m_bt_world->removeCollisionObject(obj);
			delete obj;
		}

		//delete collision shapes
		for (int j = 0; j < m_bt_collision_shapes.Count(); j++)
		{
			btCollisionShape* shape = m_bt_collision_shapes[j];
			delete shape;
		}
		m_bt_collision_shapes.Empty();

		delete m_bt_world;
		delete m_bt_solver;
		delete m_bt_broadphase;
		delete m_bt_dispatcher;
		delete m_bt_collision_config;
	}
#endif
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

