//
// Orbital
//
// Copyright: (c) 2009-2012 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//            (c) 2012 Sam Hocevar <sam@hocevar.net>
//

/* FIXME: this file is pure crap; it's only a test. */

#if !defined __PHYSICOBJECT_H__
#define __PHYSICOBJECT_H__

#include "core.h"
#include "easymesh\easymesh.h"
#include "Physics\EasyPhysics.h"

using namespace lol;
using namespace lol::phys;

class PhysicsObject : public WorldEntity
{
public:
	PhysicsObject(Simulation* new_sim)
		: m_ready(false)
	{
		m_mesh.Compile("[sc#add afcb110 1 110 -.1]");
		m_physics.SetShapeToBox(vec3(110.f, 1.f, 110.f));
		m_physics.SetMass(.0f);
		m_physics.InitBodyToRigid();
		m_physics.AddToSimulation(new_sim);
	}

	PhysicsObject(Simulation* new_sim, float base_mass, vec3 &base_location)
		: m_ready(false)
	{
		Array<char *> MeshRand;

		MeshRand << "[sc#add afcb2 2 2 -.1]";
		MeshRand << "[sc#dad afcb2 2 2 -.1]";
		MeshRand << "[sc#dda afcb2 2 2 -.1]";
		MeshRand << "[sc#daa afcb2 2 2 -.1]";
		MeshRand << "[sc#ada afcb2 2 2 -.1]";
		MeshRand << "[sc#aad afcb2 2 2 -.1]";

		int RandValue = (int)(lol::RandF() * (MeshRand.Count() - 1));

		m_mesh.Compile(MeshRand[RandValue]);
		m_physics.SetShapeToBox(vec3(2.0f));
		m_physics.SetMass(base_mass);
		m_physics.SetBaseTransform(base_location);
		m_physics.InitBodyToRigid();
		m_physics.AddToSimulation(new_sim);
	}

	~PhysicsObject()
	{
	}

	char const *GetName() { return "<PhysicsObject>"; }

protected:
	virtual void TickGame(float seconds)
	{
		WorldEntity::TickGame(seconds);
	}

	virtual void TickDraw(float seconds)
	{
		WorldEntity::TickDraw(seconds);

		if (!m_ready)
		{
			m_mesh.MeshConvert();
			m_ready = true;
		}

		m_mesh.Render(m_physics.GetTransform());
	}

private:
	//Base datas
	EasyMesh		m_mesh;
	EasyPhysics		m_physics;

	bool			m_ready;
};

#endif /* __PHYSICOBJECT_H__ */

