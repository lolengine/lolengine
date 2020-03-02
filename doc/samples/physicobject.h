//
//  Lol Engine — Bullet physics test
//
//  Copyright © 2012—2020 Sam Hocevar <sam@hocevar.net>
//            © 2009—2013 Cédric Lecacheur <jordx@free.fr>
//            © 2009—2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

/* FIXME: this file is pure crap; it's only a test. */

#include <lol/engine.h>

#include "easymesh/easymesh.h"
#include "physics/easyphysics.h"
#include "physics/easycharactercontroller.h"
#include "physics/easyconstraint.h"

using namespace lol;
using namespace lol::phys;

#if CAT_MODE
#define USE_SPHERE          1
#else
#define USE_BOX             1
#define USE_SPHERE          1
#define USE_CONE            1
#define USE_CYLINDER        1
#define USE_CAPSULE         1
#endif

static vec4 hex_to_color(std::string const &s)
{
    std::string c = s[0] == '#' ? &s[1] : s;
    uint32_t tmp = std::strtol(c.c_str(), nullptr, 16);
    if (c.length() == 3)
    {
        tmp = 0x11000000u * (tmp >> 8)
            | 0x00110000u * ((tmp >> 4) & 0xf)
            | 0x00001100u * (tmp & 0xf)
            | 0x000000ffu;
    }
    else if (c.length() == 4)
    {
        tmp = 0x11000000u * (tmp >> 12)
            | 0x00110000u * ((tmp >> 8) & 0xf)
            | 0x00001100u * ((tmp >> 4) & 0xf)
            | 0x00000011u * (tmp & 0xf);
    }
    else if (c.length() == 6)
    {
        tmp = 0xffu | 0x100u * tmp;
    }
    ivec4 v(tmp >> 24, (tmp >> 16) & 0xff, (tmp >> 8) & 0xff, tmp & 0xff);
    return vec4(v) * (1.f / 255.f);
}

class PhysicsObject : public WorldEntity
{
public:
    PhysicsObject(Simulation* new_sim, const vec3 &base_location, const quat &base_rotation)
      : m_custom_shader(0),
        m_ready(false),
        m_should_render(true),
        m_is_character(false),
        m_is_phys(false)
    {
        m_physics = new EasyPhysic(this);

        //m_mesh.Compile("[sc#ddd afcb 60 1 60 -.1]");
        m_mesh.SetCurColor(color::white);
        m_mesh.AppendFlatChamfBox(vec3(60.f, 1.f, 60.f), -.1f);

        vec3 BoxSize = vec3(60.f, 1.f, 60.f);
        m_physics->SetCollisionChannel(0, 0xFF);
        m_physics->SetShapeToBox(BoxSize);
        m_physics->SetMass(.0f);
        m_physics->SetTransform(base_location, base_rotation);
        m_physics->InitBodyToRigid(true);
        m_physics->AddToSimulation(new_sim);
    }

    PhysicsObject(Simulation* new_sim, const vec3 &base_location, const quat &base_rotation, int dummy)
      : m_custom_shader(0),
        m_ready(false),
        m_should_render(true),
        m_is_character(false),
        m_is_phys(false)
    {
        if (dummy == 1) //for platform purpose
        {
            m_physics = new EasyPhysic(this);

            m_mesh.Compile("[sc#ddd afcb 20 1 20 -.1]");
            vec3 BoxSize = vec3(20.f, 1.f, 20.f);
            m_physics->SetCollisionChannel(0, 0xFF);
            m_physics->SetShapeToBox(BoxSize);
            m_physics->SetMass(.0f);
            m_physics->SetTransform(base_location, base_rotation);
            m_physics->InitBodyToRigid(true);
            m_physics->AddToSimulation(new_sim);
        }
        else if (dummy == 2) //for character purpose
        {
            m_character = new EasyCharacterController(this);
            m_is_character = true;
            //m_mesh.Compile("[sc#f00 afcb10 10 10 -.1]");
            m_mesh.Compile(
                "[sc#000 scb#000"
                //"[sc#aaa scb#aaa"
                "[ad8 2 0 rx180 ty-1]"
                "[asph8 .5 ty1]"
                "[ac32 2 .5 .5 0 0]"
                "[asph6 .1 ty.9 tx.5 tz.15]"
                "[asph6 .1 ty.9 tx.5 tz-.15]"
                "[asph8 .05 sy10 ty.6 tz.5]"
                "[asph8 .05 sy10 ty.6 tz-.5]"
                "]"
                "[sc#fd0 scb#fd0"
                "[ac8 .4 .1 0 0 0 ty.25 rz-90 ty.7 tx.5]"
                "]"
                "["
                "[sc#fff scb#fff"
                "[ad8 2 0 rx180 ty-1]"
                "[asph8 .5 ty1]"
                "[ac32 1.9 .5 .5 0 0]"
                "]"
                " ty-.1 tx.05]"
                );
            vec3 BoxSize = vec3(1.f, 2.f, 1.f);
            m_character->SetCollisionChannel(0, 0xFF);
            m_character->SetShapeToCapsule(BoxSize.x, BoxSize.y);
            m_character->SetMass(.0f);
            //m_character->SetStepHeight(1.f);
            m_character->SetTransform(base_location, base_rotation);
            m_character->InitBodyToGhost();
            m_character->AddToSimulation(new_sim);
        }
        else if (dummy == 3) //for Stairs purpose
        {
            m_physics = new EasyPhysic(this);

            m_mesh.Compile("[sc#aae afcb4 .25 4 -.01]");
            vec3 BoxSize = vec3(4.f, .25f, 4.f);
            m_physics->SetCollisionChannel(0, 0xFF);
            m_physics->SetShapeToBox(BoxSize);
            m_physics->SetMass(.0f);
            m_physics->SetTransform(base_location, base_rotation);
            m_physics->InitBodyToRigid(true);
            m_physics->AddToSimulation(new_sim);
        }
    }

    PhysicsObject(Simulation* new_sim, float base_mass, const vec3 &base_location, int RandValue = -1)
      : m_custom_shader(0),
        m_ready(false),
        m_should_render(true),
        m_is_character(false),
        m_is_phys(false)
    {
        static array<EasyMesh> MeshRand;
        static array<int> MeshLimit;
        static array<int> MeshType;

        if (!MeshRand.count())
        {
            array<std::string> colors_base = { "#add", "#dad", "#dda", "#daa", "#ada", "#aad" };

            MeshLimit << 0;

#if USE_BOX
            {
                array<std::string> colors = colors_base;
                while (colors.count())
                {
                    EasyMesh m;
                    vec4 color = hex_to_color(colors.pop());
                    m.SetCurColor(color);
                    m.AppendFlatChamfBox(vec3(1.7f), .4f);
                    m.OpenBrace();
                    {
                        m.SetCurColor(color::black);
                        m.ToggleScaleWinding();
                        m.AppendFlatChamfBox(vec3(1.9f), .4f);
                        m.ScaleX(-1.f);
                        m.ScaleY(-1.f);
                        m.ScaleZ(-1.f);
                    }
                    m.CloseBrace();
                    MeshRand << m;
                }
            }
            //MeshRand << "[sc#add afcb1.7 1.7 1.7 0.4][sc#000 tsw afcb1.9 1.9 1.9 0.4 sx-1 sy-1 sz-1]";
            //MeshRand << "[sc#dad afcb1.7 1.7 1.7 0.4][sc#000 tsw afcb1.9 1.9 1.9 0.4 sx-1 sy-1 sz-1]";
            //MeshRand << "[sc#dda afcb1.7 1.7 1.7 0.4][sc#000 tsw afcb1.9 1.9 1.9 0.4 sx-1 sy-1 sz-1]";
            //MeshRand << "[sc#daa afcb1.7 1.7 1.7 0.4][sc#000 tsw afcb1.9 1.9 1.9 0.4 sx-1 sy-1 sz-1]";
            //MeshRand << "[sc#ada afcb1.7 1.7 1.7 0.4][sc#000 tsw afcb1.9 1.9 1.9 0.4 sx-1 sy-1 sz-1]";
            //MeshRand << "[sc#aad afcb1.7 1.7 1.7 0.4][sc#000 tsw afcb1.9 1.9 1.9 0.4 sx-1 sy-1 sz-1]";

            MeshLimit << MeshRand.count();
            MeshType << 0;
#endif //USE_BOX


#if USE_SPHERE
#if CAT_MODE
            int nb_sprite = NB_SPRITE;
            //SPRITE
            vec2 start_point = vec2((float)rand(nb_sprite), (float)rand(nb_sprite)) / vec2((float)nb_sprite);
            //vec2(0.f, .0f) / vec2((float)nb_sprite);
            vec2 size = vec2(1.f) / vec2((float)nb_sprite);
            m_mesh.BD()->SetTexCoordCustomBuild(MeshType::Quad, MeshFaceType::QuadDefault,
                start_point, start_point + size);
            m_mesh.BD()->SetTexCoordCustomBuild2(MeshType::Quad, MeshFaceType::QuadDefault,
                vec2(-PARTICLE_SIZE), vec2(PARTICLE_SIZE));
            MeshRand << "[tpbn tvnc sc#ffff aq 0 0]";
            MeshRand << "[tpbn tvnc sc#faaf aq 0 0]";
            MeshRand << "[tpbn tvnc sc#afaf aq 0 0]";
            MeshRand << "[tpbn tvnc sc#aaff aq 0 0]";
#else
            {
                array<std::string> colors = colors_base;
                while (colors.count())
                {
                    EasyMesh m;
                    vec4 color = hex_to_color(colors.pop());
                    m.SetCurColor(color);
                    m.AppendSphere(1, 2.f);
                    MeshRand << m;
                }
            }
            //MeshRand << "[sc#add asph1 2]";
            //MeshRand << "[sc#dad asph1 2]";
            //MeshRand << "[sc#dda asph1 2]";
            //MeshRand << "[sc#daa asph1 2]";
            //MeshRand << "[sc#ada asph1 2]";
            //MeshRand << "[sc#aad asph1 2]";
#endif

            MeshLimit << MeshRand.count();
            MeshType << 1;
#endif //USE_SPHERE

#if USE_CONE
            {
                array<std::string> colors = colors_base;
                while (colors.count())
                {
                    EasyMesh m;
                    vec4 color = hex_to_color(colors.pop());
                    m.SetCurColor(color);
                    m.SetCurColorB(color);
                    m.AppendDisc(8, 2.f);
                    m.RotateX(180.f);
                    m.TranslateY(-1.f);
                    m.AppendCylinder(8, 2.f, 2.f, 0.f);
                    MeshRand << m;
                }
            }
            //MeshRand << "[sc#add scb#add ad8 2 0 rx180 ty-1 ac8 2 2 0 0 0]";
            //MeshRand << "[sc#dad scb#dad ad8 2 0 rx180 ty-1 ac8 2 2 0 0 0]";
            //MeshRand << "[sc#dda scb#dda ad8 2 0 rx180 ty-1 ac8 2 2 0 0 0]";
            //MeshRand << "[sc#daa scb#daa ad8 2 0 rx180 ty-1 ac8 2 2 0 0 0]";
            //MeshRand << "[sc#ada scb#ada ad8 2 0 rx180 ty-1 ac8 2 2 0 0 0]";
            //MeshRand << "[sc#aad scb#aad ad8 2 0 rx180 ty-1 ac8 2 2 0 0 0]";

            MeshLimit << MeshRand.count();
            MeshType << 2;
#endif //USE_CONE

#if USE_CYLINDER
            {
                array<std::string> colors = colors_base;
                while (colors.count())
                {
                    EasyMesh m;
                    vec4 color = hex_to_color(colors.pop());
                    m.SetCurColor(color);
                    m.SetCurColorB(color);
                    m.AppendDisc(8, 2.f);
                    m.RotateX(180.f);
                    m.TranslateY(-1.f);
                    m.MirrorY();
                    m.AppendCylinder(8.f, 2.f, 2.f, 2.f);
                    MeshRand << m;
                }
            }
            //MeshRand << "[sc#add scb#add ad8 2 0 rx180 ty-1 my ac8 2 2 2 0 0]";
            //MeshRand << "[sc#dad scb#dad ad8 2 0 rx180 ty-1 my ac8 2 2 2 0 0]";
            //MeshRand << "[sc#dda scb#dda ad8 2 0 rx180 ty-1 my ac8 2 2 2 0 0]";
            //MeshRand << "[sc#daa scb#daa ad8 2 0 rx180 ty-1 my ac8 2 2 2 0 0]";
            //MeshRand << "[sc#ada scb#ada ad8 2 0 rx180 ty-1 my ac8 2 2 2 0 0]";
            //MeshRand << "[sc#aad scb#aad ad8 2 0 rx180 ty-1 my ac8 2 2 2 0 0]";

            MeshLimit << MeshRand.count();
            MeshType << 3;
#endif //USE_CYLINDER

#if USE_CAPSULE
            {
                array<std::string> colors = colors_base;
                while (colors.count())
                {
                    EasyMesh m;
                    vec4 color = hex_to_color(colors.pop());
                    m.SetCurColor(color);
                    m.SetCurColorB(color);
                    m.AppendCapsule(1, 2.f, 1.f);
                    MeshRand << m;
                }
            }
            //MeshRand << "[sc#add scb#add acap1 2 1]";
            //MeshRand << "[sc#dad scb#dad acap1 2 1]";
            //MeshRand << "[sc#dda scb#dda acap1 2 1]";
            //MeshRand << "[sc#daa scb#daa acap1 2 1]";
            //MeshRand << "[sc#ada scb#ada acap1 2 1]";
            //MeshRand << "[sc#aad scb#aad acap1 2 1]";

            MeshLimit << MeshRand.count();
            MeshType << 4;
#endif //USE_CAPSULE
        }

        int RandLimit = RandValue;
        if (MeshLimit.count() <= RandValue || RandValue < 0)
            RandLimit = rand((int)MeshLimit.count() - 1);
        RandValue = rand(MeshLimit[RandLimit], MeshLimit[RandLimit + 1]);

        m_physics = new EasyPhysic(this);

        //m_mesh.Compile(MeshRand[RandValue]);
        m_mesh = MeshRand[RandValue];
        m_mesh.Scale(vec3(OBJ_SIZE));
        vec3 BoxSize = vec3(2.0f) * OBJ_SIZE;
        int ColGroup = 1;

        switch (MeshType[RandLimit])
        {
            case 0:
            {
                m_physics->SetShapeToBox(BoxSize);
                ColGroup += 0;
                break;
            }
            case 1:
            {
                m_physics->SetShapeToSphere(BoxSize.x);
                ColGroup += 1;
                break;
            }
            case 2:
            {
                m_physics->SetShapeToCone(BoxSize.x, BoxSize.y);
                ColGroup += 2;
                break;
            }
            case 3:
            {
                m_physics->SetShapeToCylinder(BoxSize);
                ColGroup += 3;
                break;
            }
            case 4:
            {
                m_physics->SetShapeToCapsule(BoxSize.x, BoxSize.y);
                ColGroup += 4;
                break;
            }
            default:
            {
            }
        }

        m_physics->SetHitRestitution(1.0f);
        m_physics->SetCollisionChannel(0, 0xFF);
        //m_physics->SetCollisionChannel(ColGroup, (1 << ColGroup)|(1));
        m_physics->SetMass(base_mass);
        m_physics->SetTransform(base_location);
        m_physics->InitBodyToRigid();
        m_physics->AddToSimulation(new_sim);
    }

    void SetTransform(const lol::vec3& base_location, const lol::quat& base_rotation = lol::quat(lol::mat3(1.0f)))
    {
        if (m_is_character)
            m_character->SetTransform(base_location, base_rotation);
        else
            m_physics->SetTransform(base_location, base_rotation);
    }

    lol::mat4 GetTransform()
    {
        if (m_is_character)
            return m_character->GetTransform();
        else
            return m_physics->GetTransform();
    }

    void SetRender(bool should_render)
    {
        m_should_render = should_render;
    }

    void SetCustomShaderData(GpuShaderData* custom_shader)
    {
        m_custom_shader = custom_shader;
    }
    GpuShaderData* GetCustomShaderData()
    {
        return m_custom_shader;
    }

    EasyMesh *GetMesh() { return &m_mesh; }
    EasyPhysic *GetPhysic() { return m_physics; }
    EasyCharacterController *GetCharacter() { return m_character; }

    ~PhysicsObject()
    {
    }

    std::string GetName() const { return "<PhysicsObject>"; }

protected:
    virtual void tick_game(float seconds)
    {
        WorldEntity::tick_game(seconds);
    }

    virtual void tick_draw(float seconds, Scene &scene)
    {
        WorldEntity::tick_draw(seconds, scene);

#if CAT_MODE
        if (!m_is_phys || m_custom_shader)
#endif //CAT_MODE
        {
            if (!m_ready)
            {
                m_mesh.MeshConvert();
                /* FIXME: m_custom_shader is ignored */
                m_ready = true;
            }
            else if (m_should_render)
            {
                if (m_is_character)
                    m_mesh.Render(scene, m_character->GetTransform());
                else
                    m_mesh.Render(scene, m_physics->GetTransform());
            }
        }
    }

private:
    // Base data
    EasyMesh                 m_mesh;
    EasyPhysic*              m_physics;
    EasyCharacterController* m_character;
    GpuShaderData*           m_custom_shader;

    bool m_ready;
    bool m_should_render;
    bool m_is_character;
    bool m_is_phys; /* Only used in CAT_MODE */
};

