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
#include <cstdio>

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
        fprintf(stderr, "ERROR: entity destructor called directly\n");
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
        fprintf(stderr, "ERROR: invalid entity game tick\n");
    state = STATE_POSTTICK_GAME;
#endif
}

void Entity::TickDraw(float deltams)
{
#if !LOL_RELEASE
    if (state != STATE_PRETICK_DRAW)
        fprintf(stderr, "ERROR: invalid entity draw tick\n");
    state = STATE_POSTTICK_DRAW;
#endif
}

} /* namespace lol */

