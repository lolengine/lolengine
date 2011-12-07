//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

#include "core.h"

namespace lol
{

/*
 * Public Entity class
 */

Entity::Entity() :
    gamenext(0),
    drawnext(0),
    ref(0),
    destroy(0)
{
#if !LOL_RELEASE
    state = STATE_IDLE;
#endif
    gamegroup = GAMEGROUP_DEFAULT;
    drawgroup = DRAWGROUP_DEFAULT;
    Ticker::Register(this);
}

Entity::~Entity()
{
#if !LOL_RELEASE
    if (!destroy)
        Log::Error("entity destructor called directly\n");
#endif
}

char const *Entity::GetName()
{
    return "<entity>";
}

void Entity::TickGame(float deltams)
{
#if !LOL_RELEASE
    if (state != STATE_PRETICK_GAME)
        Log::Error("invalid entity game tick\n");
    state = STATE_POSTTICK_GAME;
#endif
}

void Entity::TickDraw(float deltams)
{
#if !LOL_RELEASE
    if (state != STATE_PRETICK_DRAW)
        Log::Error("invalid entity draw tick\n");
    state = STATE_POSTTICK_DRAW;
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

