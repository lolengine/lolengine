//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
    friend class Emcee;

public:
    virtual char const *GetName();

protected:
    Entity();
    virtual ~Entity();

    inline int IsDestroying() { return m_destroy; }

    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

    enum
    {
        GAMEGROUP_BEFORE = 0,
        GAMEGROUP_DEFAULT,
        GAMEGROUP_AFTER,
        GAMEGROUP_AFTER_0,
        GAMEGROUP_AFTER_1,

        // Must be the last element
        GAMEGROUP_END
    }
    m_gamegroup;

    enum
    {
        DRAWGROUP_BEFORE = GAMEGROUP_END,
        DRAWGROUP_CAMERA,
        DRAWGROUP_DEFAULT,
        DRAWGROUP_HUD,
        DRAWGROUP_CAPTURE,
        // Must be the last element
        DRAWGROUP_END
    }
    m_drawgroup;

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
    m_tickstate;
#endif

    // Emcee begin
private:
    void SetState(uint32_t newstate);
    void SetStateWhenMatch(uint32_t newstate,
                           Entity *other_entity, uint32_t other_state);
    virtual uint32_t OnStateChanged(uint32_t newstate)
    {
        return LOLm_state = newstate;
    }

    uint32_t LOLm_state;
    // Emcee end

private:
    Entity *m_gamenext, *m_drawnext, *m_autonext;
    int m_ref, m_autorelease, m_destroy;
};

} /* namespace lol */

#endif // __LOL_ENTITY_H__

