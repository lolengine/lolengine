//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdlib>

namespace lol
{

/*
 * Public Entity class
 */

Entity::Entity() :
    m_initstate(InitState::Ready),
    m_ref(0),
    m_destroy(0)
{
#if !LOL_BUILD_RELEASE
    m_tickstate = tickable::state::idle;
#endif
    m_gamegroup = tickable::group::game::entity;
    m_drawgroup = tickable::group::draw::entity;
    /* FIXME: is this a problem? because the object can
     * be ticked before the constructor is finished! */
    Ticker::Register(this);
}

Entity::~Entity()
{
#if !LOL_BUILD_RELEASE
    if (!m_destroy)
        msg::error("entity destructor called directly\n");
#endif
}

std::string Entity::GetName() const
{
    return "<entity>";
}

void Entity::InitGame()
{
}

void Entity::InitDraw()
{
}

void Entity::tick_game(float seconds)
{
    UNUSED(seconds);
#if !LOL_BUILD_RELEASE
    if (m_tickstate != tickable::state::pre_game)
        msg::error("invalid entity game tick\n");
    m_tickstate = tickable::state::post_game;
#endif
}

void Entity::tick_draw(float seconds, Scene &scene)
{
    UNUSED(seconds, scene);
#if !LOL_BUILD_RELEASE
    if (m_tickstate != tickable::state::pre_draw)
        msg::error("invalid entity draw tick\n");
    m_tickstate = tickable::state::post_draw;
#endif
}

} /* namespace lol */

