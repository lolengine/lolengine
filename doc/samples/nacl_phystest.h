//
//  Lol Engine — Bullet physics test
//
//  Copyright © 2009—2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2012—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

class Nacl_PhysTest : public WorldEntity
{
public:
    Nacl_PhysTest(bool editor = false);
    virtual ~Nacl_PhysTest();

    std::string GetName() const { return "<Nacl_PhysTest>"; }

protected:
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);

private:
    enum
    {
        KEY_MOVE_FORWARD,
        KEY_MOVE_BACK,
        KEY_MOVE_LEFT,
        KEY_MOVE_RIGHT,
        KEY_MOVE_UP,
        KEY_MOVE_DOWN,
        KEY_MOVE_JUMP,
        KEY_QUIT,
        KEY_MAX,
    };

    TileSet*                        m_cat_texture;
    Shader*                         m_cat_shader;
    Camera*                         m_camera;
    Controller *                    m_controller;
    Light *                         m_light1;
    Light *                         m_light2;
    bool                            m_ready;

    //lol::phys::Simulation*          m_simulation;
    //array<EasyConstraint*>          m_constraint_list;
    //array<PhysicsObject*, float>    m_physobj_list;
    //array<PhysicsObject*>           m_ground_list;
    //array<PhysicsObject*>           m_platform_list;
    //array<PhysicsObject*>           m_character_list;
    //array<PhysicsObject*>           m_stairs_list;

    float                           m_loop_value;
    float                           m_target_timer;
    int                             m_cam_target;
    float                           m_fov_dp;
    float                           m_loc_dp;
};

