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

#if USE_SDL && defined __APPLE__
#   include <SDL_main.h>
#endif

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

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
    m_camera->SetPerspective(90.f, 1280.f, 960.f, .1f, 1000.f);
	//m_camera->SetOrtho(1280.f / 6, 960.f / 6, -1000.f, 1000.f);
    Ticker::Ref(m_camera);

    m_ready = false;

	m_simulation = new Simulation();
	m_simulation->InitContext();
	vec3 NewGravity = vec3(.0f, -10.0f, .0f);
	m_simulation->SetGravity(NewGravity);

	m_ground_object = new PhysicsObject(m_simulation);
	Ticker::Ref(m_ground_object);

	for (int x=0; x < 10; x++)
	{
		for (int y=0; y < 10; y++)
		{
			PhysicsObject* new_physobj = new PhysicsObject(m_simulation, 10.f, vec3(0.f, 20.f, -20.0f) + vec3(.0f, 4.f * (float)y, 4.f * (float)x));
			m_physobj_list << new_physobj;
			Ticker::Ref(new_physobj);
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

	m_simulation->TickContext(seconds);

	m_camera->SetTarget(vec3(.0f));
	m_camera->SetPosition(vec3(-30.0f, 10.0f, .0f));

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

