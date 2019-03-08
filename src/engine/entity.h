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
// The entity class
// ----------------
// Entities are objects that can be ticked by the game loop and/or the render
// loop. Entities are implemented as one or several linked lists. See the
// Ticker class for the ticking logic and the linked list implementation.
//

#include <cstdint>

#include <lol/engine/tickable.h>

namespace lol
{

class entity
{
    friend class Scene;
    friend class ticker;
    friend class ticker_data;

public:
    virtual std::string GetName() const;

    enum class flags : uint16_t
    {
        none = 0,
        init_game    = 1 << 0,
        init_draw    = 1 << 1,
        release_game = 1 << 2,
        release_draw = 1 << 4,
        destroying   = 1 << 5,
        autorelease  = 1 << 6,
    };

    inline void add_flags(flags f);
    inline void remove_flags(flags f);
    inline bool has_flags(flags f);

protected:
    entity();
    virtual ~entity();

    virtual bool init_game() { return true; }
    virtual bool init_draw() { return true; }
    virtual bool release_game() { return true; }
    virtual bool release_draw() { return true; }

    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, class Scene &scene);

#if !LOL_BUILD_RELEASE
    tickable::state m_tickstate;
#endif
    tickable::group::game m_gamegroup;
    tickable::group::draw m_drawgroup;

private:
    flags m_flags = flags::none;
    int m_ref = 0;
    uint64_t m_scene_mask = 0;
};

static inline entity::flags operator |(entity::flags a, entity::flags b)
{
    return (entity::flags)((uint16_t)a | (uint16_t)b);
}

static inline entity::flags operator &(entity::flags a, entity::flags b)
{
    return (entity::flags)((uint16_t)a & (uint16_t)b);
}

static inline entity::flags operator ^(entity::flags a, entity::flags b)
{
    return (entity::flags)((uint16_t)a ^ (uint16_t)b);
}

inline void entity::add_flags(entity::flags f) { m_flags = m_flags | f; }
inline void entity::remove_flags(entity::flags f) { m_flags = (m_flags | f) ^ f; }
inline bool entity::has_flags(entity::flags f) { return (m_flags & f) != flags::none; }

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

