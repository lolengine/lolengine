//
//  Lol Engine
//
//  Copyright: © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://www.wtfpl.net/ for more details.
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
    m_tickstate = STATE_IDLE;
#endif
    m_gamegroup = GAMEGROUP_ENTITY;
    m_drawgroup = DRAWGROUP_ENTITY;
    Ticker::Register(this);
}

Entity::~Entity()
{
#if !LOL_BUILD_RELEASE
    if (!m_destroy)
        msg::error("entity destructor called directly\n");
#endif
}

char const *Entity::GetName()
{
    return "<entity>";
}

void Entity::InitGame()
{
}

void Entity::InitDraw()
{
}

void Entity::TickGame(float seconds)
{
    UNUSED(seconds);
#if !LOL_BUILD_RELEASE
    if (m_tickstate != STATE_PRETICK_GAME)
        msg::error("invalid entity game tick\n");
    m_tickstate = STATE_POSTTICK_GAME;
#endif
}

void Entity::TickDraw(float seconds, Scene &scene)
{
    (void)seconds;
#if !LOL_BUILD_RELEASE
    if (m_tickstate != STATE_PRETICK_DRAW)
        msg::error("invalid entity draw tick\n");
    m_tickstate = STATE_POSTTICK_DRAW;
#endif
}

void Entity::SetState(uint32_t state)
{
    Ticker::SetState(this, state);
}

void Entity::SetStateWhenMatch(uint32_t state,
                               Entity *other_entity, uint32_t other_state)
{
    Ticker::SetStateWhenMatch(this, state, other_entity, other_state);
}

} /* namespace lol */

