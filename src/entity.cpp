//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cstdio>

#include "core.h"

/*
 * Public Entity class
 */

Entity::Entity() :
    next(0),
    ref(0),
    destroy(0)
{
#if !FINAL_RELEASE
    state = STATE_IDLE;
#endif
    Ticker::Register(this);
}

Entity::~Entity()
{
#if !FINAL_RELEASE
    if (!destroy)
        fprintf(stderr, "ERROR: entity destructor called directly\n");
#endif
}

char const *Entity::GetName()
{
    return "<entity>";
}

Entity::Group Entity::GetGroup()
{
    return GROUP_DEFAULT;
}

void Entity::TickGame(float deltams)
{
#if !FINAL_RELEASE
    if (state != STATE_PRETICK_GAME)
        fprintf(stderr, "ERROR: invalid entity game tick\n");
    state = STATE_POSTTICK_GAME;
#endif
}

void Entity::TickDraw(float deltams)
{
#if !FINAL_RELEASE
    if (state != STATE_PRETICK_DRAW)
        fprintf(stderr, "ERROR: invalid entity draw tick\n");
    state = STATE_POSTTICK_DRAW;
#endif
}

void Entity::Ref()
{
    ref++;
}

int Entity::Unref()
{
    return --ref;
}

