//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>
#include <cstdlib>
#include <ctype.h>

#include "core.h"

#include "lua/lua.hpp"

namespace lol
{

/*
 * World implementation class
 */

class WorldData
{
    friend class World;

    lua_State *m_lua_state;
};

static WorldData g_world_data;
World g_world;

/*
 * Public World class
 */

World::World()
{
    g_world_data.m_lua_state = luaL_newstate();
    luaL_openlibs(g_world_data.m_lua_state);
}

World::~World()
{
    lua_close(g_world_data.m_lua_state);
}

bool World::ExecLua(String const &lua)
{
    Array<String> pathlist = System::GetPathList(lua);
    for (int i = 0; i < pathlist.Count(); ++i)
    {
        luaL_dofile(g_world_data.m_lua_state, pathlist[i].C());
    }

    return true;
}

double World::GetLuaNumber(String const &var)
{
    lua_getglobal(g_world_data.m_lua_state, var.C());
    return lua_tonumber(g_world_data.m_lua_state, -1);
}

} /* namespace lol */

