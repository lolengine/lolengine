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
		: m_ready(false), m_should_render(true), m_is_character(false)
	{
		m_mesh.Compile("[sc#ddd afcb60 1 60 -.1]");
		vec3 BoxSize = vec3(60.f, 1.f, 60.f);
		m_physics.SetCollisionChannel(0, 0xFF);
		m_physics.SetShapeToBox(BoxSize);
		m_physics.SetMass(.0f);
		m_physics.SetTransform(base_location, base_rotation);
		m_physics.InitBodyToRigid(true);
		m_physics.AddToSimulation(new_sim);
	}

	PhysicsObject(Simulation* new_sim, const vec3 &base_location, const quat &base_rotation, int dummy)
		: m_ready(false), m_should_render(true), m_is_character(false)
	{
		if (dummy == 1) //for Rope purpose
		{
			m_mesh.Compile("[sc#ddd afcb20 1 20 -.1]");
			//m_mesh.Compile("[sc#f00 afcb10 10 10 -.1]");

			vec3 BoxSize = vec3(20.f, 1.f, 20.f);
			m_physics.SetCollisionChannel(0, 0xFF);
			m_physics.SetShapeToBox(BoxSize);
			m_physics.SetMass(.0f);
			m_physics.SetTransform(base_location, base_rotation);
			m_physics.InitBodyToRigid(true);
			m_physics.AddToSimulation(new_sim);
		}
		else if (dummy == 2) //for character purpose
		{
			m_is_character = true;
			m_mesh.Compile("[sc#f00 afcb10 10 10 -.1]");
			//m_mesh.Compile("[sc#fff scb#fff ac1 2 2 2 0 0]");
			vec3 BoxSize = vec3(2.f, 2.f, 2.f);
			m_character.SetCollisionChannel(0, 0xFF);
			m_character.SetShapeToCapsule(BoxSize.x, BoxSize.y);
			m_character.SetMass(.0f);
			m_character.SetTransform(base_location, base_rotation);
			m_character.InitBodyToGhost();
			m_character.AddToSimulation(new_sim);
		}
	}

	PhysicsObject(Simulation* new_sim, float base_mass, const vec3 &base_location, int RandValue = -1)
		: m_ready(false), m_should_render(true), m_is_character(false)
	{
		Array<char const *> MeshRand;

		MeshRand << "[sc#add afcb2 2 2 -.1]";
		MeshRand << "[sc#dad afcb2 2 2 -.1]";
		MeshRand << "[sc#dda afcb2 2 2 -.1]";
		MeshRand << "[sc#daa afcb2 2 2 -.1]";
		MeshRand << "[sc#ada afcb2 2 2 -.1]";
		MeshRand << "[sc#aad afcb2 2 2 -.1]";

		int SphereLimit = MeshRand.Count();

		MeshRand << "[sc#add asph1 2 2 2]";
		MeshRand << "[sc#dad asph1 2 2 2]";
		MeshRand << "[sc#dda asph1 2 2 2]";
		MeshRand << "[sc#daa asph1 2 2 2]";
		MeshRand << "[sc#ada asph1 2 2 2]";
		MeshRand << "[sc#aad asph1 2 2 2]";

		int ConeLimit = MeshRand.Count();

		MeshRand << "[sc#add scb#add ad1 2 0 rx180 ty-1 ac1 2 2 0 0 0]";
		MeshRand << "[sc#dad scb#dad ad1 2 0 rx180 ty-1 ac1 2 2 0 0 0]";
		MeshRand << "[sc#dda scb#dda ad1 2 0 rx180 ty-1 ac1 2 2 0 0 0]";
		MeshRand << "[sc#daa scb#daa ad1 2 0 rx180 ty-1 ac1 2 2 0 0 0]";
		MeshRand << "[sc#ada scb#ada ad1 2 0 rx180 ty-1 ac1 2 2 0 0 0]";
		MeshRand << "[sc#aad scb#aad ad1 2 0 rx180 ty-1 ac1 2 2 0 0 0]";

		int CylLimit = MeshRand.Count();

		MeshRand << "[sc#add scb#add ad1 2 0 rx180 ty-1 my ac1 2 2 2 0 0]";
		MeshRand << "[sc#dad scb#dad ad1 2 0 rx180 ty-1 my ac1 2 2 2 0 0]";
		MeshRand << "[sc#dda scb#dda ad1 2 0 rx180 ty-1 my ac1 2 2 2 0 0]";
		MeshRand << "[sc#daa scb#daa ad1 2 0 rx180 ty-1 my ac1 2 2 2 0 0]";
		MeshRand << "[sc#ada scb#ada ad1 2 0 rx180 ty-1 my ac1 2 2 2 0 0]";
		MeshRand << "[sc#aad scb#aad ad1 2 0 rx180 ty-1 my ac1 2 2 2 0 0]";

		int CapsLimit = MeshRand.Count();

		MeshRand << "[sc#add scb#add acap1 2 1]";
		MeshRand << "[sc#dad scb#dad acap1 2 1]";
		MeshRand << "[sc#dda scb#dda acap1 2 1]";
		MeshRand << "[sc#daa scb#daa acap1 2 1]";
		MeshRand << "[sc#ada scb#ada acap1 2 1]";
		MeshRand << "[sc#aad scb#aad acap1 2 1]";

		switch (RandValue)
		{
			case 0:
			{
				RandValue = (int)(lol::RandF() * (SphereLimit - 1));
				break;
			}
			case 1:
			{
				RandValue = SphereLimit + (int)(lol::RandF() * ((ConeLimit - SphereLimit) - 1));
				break;
			}
			case 2:
			{
				RandValue = ConeLimit + (int)(lol::RandF() * ((CylLimit - ConeLimit) - 1));
				break;
			}
			case 3:
			{
				RandValue = CylLimit + (int)(lol::RandF() * ((CapsLimit - CylLimit) - 1));
				break;
			}
			case 4:
			{
				RandValue = CapsLimit + (int)(lol::RandF() * ((MeshRand.Count() - CapsLimit) - 1));
				break;
			}
			default:
			{
				RandValue = (int)(lol::RandF() * (MeshRand.Count() - 1));
			}
		}

		m_mesh.Compile(MeshRand[RandValue]);
		vec3 BoxSize = vec3(2.0f);
		int ColGroup = 1;
		if (RandValue < SphereLimit)
		{
			m_physics.SetShapeToBox(BoxSize);
			ColGroup += 0;
		}
		else if (RandValue < ConeLimit)
		{
			m_physics.SetShapeToSphere(BoxSize.x * 2.f);
			ColGroup += 1;
		}
		else if (RandValue < CylLimit)
		{
			m_physics.SetShapeToCone(BoxSize.x, BoxSize.y);
			ColGroup += 2;
		}
		else if (RandValue < CapsLimit)
		{
			m_physics.SetShapeToCylinder(BoxSize);
			ColGroup += 3;
		}
		else
		{
			m_physics.SetShapeToCapsule(BoxSize.x, BoxSize.y);
			ColGroup += 4;
		}

		m_physics.SetCollisionChannel(0, 0xFF);
		//m_physics.SetCollisionChannel(ColGroup, (1<<ColGroup)|(1));
		m_physics.SetMass(base_mass);
		m_physics.SetTransform(base_location);
		m_physics.InitBodyToRigid();
		m_physics.AddToSimulation(new_sim);
	}

	void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation=lol::quat(lol::mat4(1.0f)))
	{
		if (m_is_character)
			m_character.SetTransform(base_location, base_rotation);
		else
			m_physics.SetTransform(base_location, base_rotation);
	}

	lol::mat4 GetTransform()
	{
		if (m_is_character)
			return m_character.GetTransform();
		else
			return m_physics.GetTransform();
	}

	void SetRender(bool should_render)
	{
		m_should_render = should_render;
	}

	EasyMesh *GetMesh() { return &m_mesh; }
	EasyPhysic *GetPhysic() { return &m_physics; }
	EasyPhysic *GetCharacter() { return &m_character; }

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
		{
			if (m_is_character)
				m_mesh.Render(m_character.GetTransform());
			else
				m_mesh.Render(m_physics.GetTransform());
		}
	}

private:
	//Base datas
	EasyMesh				m_mesh;
	EasyPhysic				m_physics;
	EasyCharacterController	m_character;

	bool			m_ready;
	bool			m_should_render;
	bool			m_is_character;
};

#endif /* __PHYSICOBJECT_H__ */

