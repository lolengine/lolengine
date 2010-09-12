//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Entity class
// ---------------
// Entities are objects that can be ticked by the game loop and/or the render
// loop. Entities are implemented as one or several linked lists. See the
// Ticker class for the ticking logic and the linked list implementation.
//

#if !defined __DH_ENTITY_H__
#define __DH_ENTITY_H__

#include <stdint.h>

class Entity
{
    friend class Ticker;
    friend class TickerData;
    friend class Dict;

protected:
    Entity();
    virtual ~Entity();

    virtual char const *GetName();

    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

    Entity *gamenext, *drawnext, *autonext;
    int ref, autorelease, destroy;

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

#if !FINAL_RELEASE
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
};

#endif // __DH_ENTITY_H__

