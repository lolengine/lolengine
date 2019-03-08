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

#include <lol/engine-internal.h>

#include <cstdlib>

namespace lol
{

/*
 * Public entity class
 */

entity::entity()
{
#if !LOL_BUILD_RELEASE
    m_tickstate = tickable::state::idle;
#endif
    m_gamegroup = tickable::group::game::entity;
    m_drawgroup = tickable::group::draw::entity;
    /* FIXME: is this a problem? because the object can
     * be ticked before the constructor is finished! */
    Ticker::Register(this);
}

entity::~entity()
{
#if !LOL_BUILD_RELEASE
    if (!has_flags(flags::destroying))
        msg::error("entity destructor called directly\n");
#endif
}

std::string entity::GetName() const
{
    return "<entity>";
}

void entity::tick_game(float seconds)
{
    UNUSED(seconds);
#if !LOL_BUILD_RELEASE
    if (m_tickstate != tickable::state::pre_game)
        msg::error("invalid entity game tick\n");
    m_tickstate = tickable::state::post_game;
#endif
}

void entity::tick_draw(float seconds, Scene &scene)
{
    UNUSED(seconds, scene);
#if !LOL_BUILD_RELEASE
    if (m_tickstate != tickable::state::pre_draw)
        msg::error("invalid entity draw tick\n");
    m_tickstate = tickable::state::post_draw;
#endif
}

} /* namespace lol */

