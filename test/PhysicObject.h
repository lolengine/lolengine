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
#include "easymesh/easymesh.h"
#include "Physics/EasyPhysics.h"

using namespace lol;
using namespace lol::phys;

class PhysicsObject : public WorldEntity
{
public:
	PhysicsObject(Simulation* new_sim, const vec3 &base_location, const quat &base_rotation)
		: m_ready(false), m_should_render(true)
	{
		m_mesh.Compile("[sc#ddd afcb60 1 60 -.1]");
		vec3 BoxSize = vec3(60.f, 1.f, 60.f);
		m_physics.SetShapeToBox(BoxSize);
		m_physics.SetMass(.0f);
		m_physics.SetTransform(base_location, base_rotation);
		m_physics.InitBodyToRigid();
		m_physics.AddToSimulation(new_sim);
	}

	PhysicsObject(Simulation* new_sim, float base_mass, const vec3 &base_location)
		: m_ready(false), m_should_render(true)
	{
		Array<char *> MeshRand;

		MeshRand << "[sc#add afcb2 2 2 -.1]";
		MeshRand << "[sc#dad afcb2 2 2 -.1]";
		MeshRand << "[sc#dda afcb2 2 2 -.1]";
		MeshRand << "[sc#daa afcb2 2 2 -.1]";
		MeshRand << "[sc#ada afcb2 2 2 -.1]";
		MeshRand << "[sc#aad afcb2 2 2 -.1]";

		int SphereLimit = MeshRand.Count();

		MeshRand << "[sc#add asph6 2 2 2]";
		MeshRand << "[sc#dad asph6 2 2 2]";
		MeshRand << "[sc#dda asph6 2 2 2]";
		MeshRand << "[sc#daa asph6 2 2 2]";
		MeshRand << "[sc#ada asph6 2 2 2]";
		MeshRand << "[sc#aad asph6 2 2 2]";

		int ConeLimit = MeshRand.Count();

		MeshRand << "[sc#add scb#add ad16 2 0 rx180 ty-1 ac16 2 2 0 0 0]";
		MeshRand << "[sc#dad scb#dad ad16 2 0 rx180 ty-1 ac16 2 2 0 0 0]";
		MeshRand << "[sc#dda scb#dda ad16 2 0 rx180 ty-1 ac16 2 2 0 0 0]";
		MeshRand << "[sc#daa scb#daa ad16 2 0 rx180 ty-1 ac16 2 2 0 0 0]";
		MeshRand << "[sc#ada scb#ada ad16 2 0 rx180 ty-1 ac16 2 2 0 0 0]";
		MeshRand << "[sc#aad scb#aad ad16 2 0 rx180 ty-1 ac16 2 2 0 0 0]";

		int CylLimit = MeshRand.Count();

		MeshRand << "[sc#add scb#add ad16 2 0 rx180 ty-1 my ac16 2 2 2 0 0]";
		MeshRand << "[sc#dad scb#dad ad16 2 0 rx180 ty-1 my ac16 2 2 2 0 0]";
		MeshRand << "[sc#dda scb#dda ad16 2 0 rx180 ty-1 my ac16 2 2 2 0 0]";
		MeshRand << "[sc#daa scb#daa ad16 2 0 rx180 ty-1 my ac16 2 2 2 0 0]";
		MeshRand << "[sc#ada scb#ada ad16 2 0 rx180 ty-1 my ac16 2 2 2 0 0]";
		MeshRand << "[sc#aad scb#aad ad16 2 0 rx180 ty-1 my ac16 2 2 2 0 0]";

		int CapsLimit = MeshRand.Count();

		MeshRand << "[sc#add scb#add asph6 1 1 1 ty-1 my ac16 2 1 1 0 0]";
		MeshRand << "[sc#dad scb#dad asph6 1 1 1 ty-1 my ac16 2 1 1 0 0]";
		MeshRand << "[sc#dda scb#dda asph6 1 1 1 ty-1 my ac16 2 1 1 0 0]";
		MeshRand << "[sc#daa scb#daa asph6 1 1 1 ty-1 my ac16 2 1 1 0 0]";
		MeshRand << "[sc#ada scb#ada asph6 1 1 1 ty-1 my ac16 2 1 1 0 0]";
		MeshRand << "[sc#aad scb#aad asph6 1 1 1 ty-1 my ac16 2 1 1 0 0]";

		int RandValue = (int)(lol::RandF() * (MeshRand.Count() - 1));

		m_mesh.Compile(MeshRand[RandValue]);
		vec3 BoxSize = vec3(2.0f);
		if (RandValue < SphereLimit)
			m_physics.SetShapeToBox(BoxSize);
		else if (RandValue < ConeLimit)
			m_physics.SetShapeToSphere(BoxSize.x * 2.f);
		else if (RandValue < CylLimit)
			m_physics.SetShapeToCone(BoxSize.x, BoxSize.y);
		else if (RandValue < CapsLimit)
			m_physics.SetShapeToCylinder(BoxSize);
		else
			m_physics.SetShapeToCapsule(BoxSize.x, BoxSize.y);

		m_physics.SetMass(base_mass);
		m_physics.SetTransform(base_location);
		m_physics.InitBodyToRigid();
		m_physics.AddToSimulation(new_sim);
	}

	void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f)))
	{
		m_physics.SetTransform(base_location, base_rotation);
	}

	lol::mat4 GetTransform()
	{
		return m_physics.GetTransform();
	}

	void SetRender(bool should_render)
	{
		m_should_render = should_render;
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

		if (m_should_render)
			m_mesh.Render(m_physics.GetTransform());
	}

private:
	//Base datas
	EasyMesh		m_mesh;
	EasyPhysics		m_physics;

	bool			m_ready;
	bool			m_should_render;
};

#endif /* __PHYSICOBJECT_H__ */

