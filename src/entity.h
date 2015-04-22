//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Entity class
// ---------------
// Entities are objects that can be ticked by the game loop and/or the render
// loop. Entities are implemented as one or several linked lists. See the
// Ticker class for the ticking logic and the linked list implementation.
//

#include <stdint.h>

namespace lol
{

struct InitState
{
    enum Value
    {
        Unknown,
        Error,
        NeedInitDraw,
        NeedInitGame,
        Ready,
    }
    m_value;

    inline InitState(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

class Entity
{
    friend class Scene;
    friend class Ticker;
    friend class TickerData;
    friend class Dict;
    friend class Emcee;

public:
    virtual char const *GetName();

    inline bool IsTicked() { return !!m_ref && !m_autorelease; }
protected:
    Entity();
    virtual ~Entity();

    inline int  IsDestroying() { return m_destroy; }

    virtual void InitGame();
    virtual void InitDraw();

    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, class Scene &scene);

    enum
    {
        GAMEGROUP_BEGIN = 0, //Must be the first element

        GAMEGROUP_INPUT,    //Input should be polled before everything else
        GAMEGROUP_IMGUI,    //Debug update needs to be called before the rest for init purposes
        GAMEGROUP_APP,      //Main application update
        GAMEGROUP_ENTITY,   //Default entity update
        //------------------//Split entity update:
        GAMEGROUP_PLAYER,   //Player updates before AI to ensure player actions is prevalent
        GAMEGROUP_AI,       //AI update
        GAMEGROUP_OTHER_0,  //Other misc updates here
        GAMEGROUP_OTHER_1,  //Same ------------------
        GAMEGROUP_OTHER_2,  //Same ------------------
        GAMEGROUP_OTHER_3,  //Same ------------------
        //------------------//Primitives updates
        GAMEGROUP_MESH,     //Update Mesh/Animation to ensure correct sync with PLY/AI
        GAMEGROUP_FX,       //Update FX/other to ensure correct sync with WorldPos and Meshes
        GAMEGROUP_LIGHT,    //Update after FX because it could some
        GAMEGROUP_CAMERA,   //Update camera at the end of the frame, once everything is settled
        GAMEGROUP_STATS,    //Stats updates

        GAMEGROUP_END       //Must be the last element
    }
    m_gamegroup;

    enum
    {
        DRAWGROUP_BEGIN = GAMEGROUP_END,

        DRAWGROUP_CAMERA,   //Update camera first for rendering
        DRAWGROUP_TEXTURE,  //Texture
        DRAWGROUP_LIGHT,    //
        DRAWGROUP_WORLD,    //Other misc updates here
        DRAWGROUP_ENTITY,   //
        DRAWGROUP_FX,       //
        DRAWGROUP_OTHER_0,  //Other misc updates here
        DRAWGROUP_OTHER_1,  //Same ------------------
        DRAWGROUP_OTHER_2,  //Same ------------------
        DRAWGROUP_OTHER_3,  //Same ------------------
        DRAWGROUP_APP,      //Main application Draw
        DRAWGROUP_HUD,
        DRAWGROUP_IMGUI,
        DRAWGROUP_CAPTURE,

        DRAWGROUP_END,      //Must be the next-to-last element
        DRAWGROUP_NONE      //This group is for non draw-ticked
    }
    m_drawgroup;

    //static int const GAMEGROUP_BEGIN = 0;
    //static int const DRAWGROUP_BEGIN = GAMEGROUP_END;
    static int const ALLGROUP_END = DRAWGROUP_END;

    /* The initialisation state */
    InitState m_initstate;

#if !LOL_BUILD_RELEASE
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
    int m_ref, m_autorelease, m_destroy;
    uint64_t m_scene_mask = 0;
};

} /* namespace lol */

