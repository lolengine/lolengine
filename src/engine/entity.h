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

#pragma once

//
// The Entity class
// ----------------
// Entities are objects that can be ticked by the game loop and/or the render
// loop. Entities are implemented as one or several linked lists. See the
// Ticker class for the ticking logic and the linked list implementation.
//

#include <cstdint>

#include <lol/engine/tickable.h>

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
    friend class ticker;
    friend class ticker_data;

public:
    virtual std::string GetName() const;

    inline bool IsTicked() { return !!m_ref && !m_autorelease; }
protected:
    Entity();
    virtual ~Entity();

    inline int  IsDestroying() { return m_destroy; }

    virtual void InitGame();
    virtual void InitDraw();

    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, class Scene &scene);

    /* The initialisation state */
    InitState m_initstate;

#if !LOL_BUILD_RELEASE
    tickable::state m_tickstate;
#endif

    tickable::group::game m_gamegroup;
    tickable::group::draw m_drawgroup;

private:
    int m_ref, m_autorelease, m_destroy;
    uint64_t m_scene_mask = 0;
};

template<typename T> struct entity_dict
{
    T *get(std::string const &key)
    {
        auto it = m_cache1.find(key);
        return it != m_cache1.end() ? it->second : nullptr;
    }

    T *set(std::string const &key, T *entity)
    {
        m_cache1[key] = entity;
        m_cache2[entity] = key;
        return entity;
    }

    void erase(T *entity)
    {
        // FIXME: temporary; we need Ticker::Ref etc.
        m_cache1.erase(m_cache2[entity]);
        m_cache2.erase(entity);
    }

    std::map<std::string, T*> m_cache1;
    std::map<T*, std::string> m_cache2;
};

} /* namespace lol */

