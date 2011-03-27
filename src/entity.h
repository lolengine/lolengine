//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Entity class
// ---------------
// Entities are objects that can be ticked by the game loop and/or the render
// loop. Entities are implemented as one or several linked lists. See the
// Ticker class for the ticking logic and the linked list implementation.
//

#if !defined __LOL_ENTITY_H__
#define __LOL_ENTITY_H__

#include <stdint.h>

namespace lol
{

class Entity
{
    friend class Ticker;
    friend class TickerData;
    friend class Dict;

protected:
    Entity();
    virtual ~Entity();

    virtual char const *GetName();
    inline int IsDestroying() { return destroy; }

    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

    enum
    {
        GAMEGROUP_BEFORE = 0,
        GAMEGROUP_DEFAULT,
        GAMEGROUP_AFTER,
        // Must be the last element
        GAMEGROUP_END
    }
    gamegroup;

    enum
    {
        DRAWGROUP_BEFORE = GAMEGROUP_END,
        DRAWGROUP_DEFAULT,
        DRAWGROUP_HUD,
        DRAWGROUP_CAPTURE,
        // Must be the last element
        DRAWGROUP_END
    }
    drawgroup;

    static int const GAMEGROUP_BEGIN = 0;
    static int const DRAWGROUP_BEGIN = GAMEGROUP_END;
    static int const ALLGROUP_END = DRAWGROUP_END;

#if !LOL_RELEASE
    enum
    {
        STATE_IDLE = 0,
        STATE_PRETICK_GAME,
        STATE_POSTTICK_GAME,
        STATE_PRETICK_DRAW,
        STATE_POSTTICK_DRAW,
    }
    state;
#endif

private:
    Entity *gamenext, *drawnext, *autonext;
    int ref, autorelease, destroy;
};

} /* namespace lol */

#endif // __LOL_ENTITY_H__

