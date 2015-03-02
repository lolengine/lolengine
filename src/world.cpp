//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>
#include <cstdlib>
#include <ctype.h>

//#include "lua/lua.hpp"

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

const luaL_Reg test1Lua::m_statics[] = { { "getTest", test1Lua::getTest }, { NULL, NULL } };
const luaL_Reg test1Lua::m_methods[] = { { NULL, NULL } };
const char     test1Lua::m_class[]   = "test1";

World::World()
    : LuaLoader()
{
    g_world_data.m_lua_state = GetLuaState();
    //------ DEBUG TEST
    //m_test1.LoadTo(GetLuaState());
    //luaL_loadfile(GetLuaState(), "lua/init.lua");
    //LuaVar<int32_t> var(GetLuaState(), 1);
    //test1Lua::Library m_test1(GetLuaState());
    //------ DEBUG TEST

}

World::~World()
{
}

} /* namespace lol */

