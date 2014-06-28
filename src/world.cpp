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

    static int LuaPanic(lua_State* L)
    {
        char const *message = lua_tostring(L, -1);
        Log::Error("%s\n", message);
        DebugAbort();
        return 0;
    }

    static int LuaDoFile(lua_State *L)
    {
        if (lua_isnoneornil(L, 1))
            return LUA_ERRFILE;

        char const *filename = lua_tostring(L, 1);
        int status = LUA_ERRFILE;

        array<String> pathlist = System::GetPathList(filename);
        File f;
        for (int i = 0; i < pathlist.Count(); ++i)
        {
            f.Open(pathlist[i], FileAccess::Read);
            if (f.IsValid())
            {
                String s = f.ReadString();
                f.Close();

                Log::Debug("loading Lua file %s\n", pathlist[i].C());
                status = luaL_dostring(L, s.C());
                break;
            }
        }

        if (status == LUA_ERRFILE)
           Log::Error("could not find Lua file %s\n", filename);

        lua_pop(L, 1);

        return status;
    }

    lua_State *m_lua_state;
};

static WorldData g_world_data;
World g_world;

/*
 * Public World class
 */

World::World()
{
    /* Initialise the Lua engine */
    g_world_data.m_lua_state = luaL_newstate();
    lua_atpanic(g_world_data.m_lua_state, WorldData::LuaPanic);
    luaL_openlibs(g_world_data.m_lua_state);

    /* Override dofile() */
    lua_pushcfunction(g_world_data.m_lua_state, WorldData::LuaDoFile);
    lua_setglobal(g_world_data.m_lua_state, "dofile");
}

World::~World()
{
    lua_close(g_world_data.m_lua_state);
}

bool World::ExecLua(String const &lua)
{
    lua_pushstring(g_world_data.m_lua_state, lua.C());
    int status = WorldData::LuaDoFile(g_world_data.m_lua_state);
    return status == 0;
}

double World::GetLuaNumber(String const &var)
{
    double ret;
    lua_getglobal(g_world_data.m_lua_state, var.C());
    ret = lua_tonumber(g_world_data.m_lua_state, -1);
    lua_pop(g_world_data.m_lua_state, 1);
    return ret;
}

} /* namespace lol */

