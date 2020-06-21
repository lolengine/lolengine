//
//  Lol Engine
//
//  Copyright © 2017—2020 Sam Hocevar <sam@hocevar.net>
//            © 2009—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>
#include <lol/lua.h>

#include <lol/msg>
#include <cassert>
#include <string>
#include <cstdlib>
#include <cctype>

//
// Base Lua class for Lua script loading
//

namespace lol
{

//-----------------------------------------------------------------------------
class LuaBaseData
{
    friend class Lolua::Loader;

    //PANIC PANIC -------------------------------------------------------------
    static int LuaPanic(lua_State* l)
    {
        char const *message = lua_tostring(l, -1);
        msg::error("%s\n", message);
        assert(false);
        return 0;
    }

    //Exec lua code -----------------------------------------------------------
    static int LuaDoCode(lua_State *l, std::string const& s)
    {
        int status = luaL_dostring(l, s.c_str());
        if (status == 1)
        {
            auto stack = LuaStack::Begin(l, -1);
            auto error = stack.Get<std::string>();
            msg::error("Lua error %s\n", error.c_str());
            lua_pop(l, 1);
        }
        return status;
    }

    //Open a file and exec lua code -------------------------------------------
    static int LuaDoFile(lua_State *l)
    {
        if (lua_isnoneornil(l, 1))
            return LUA_ERRFILE;

        auto stack = LuaStack::Begin(l);
        std::string filename = stack.Get<std::string>();
        int status = LUA_ERRFILE;

        File f;
        f.Open(sys::get_data_path(filename), FileAccess::Read);
        if (!f.IsValid())
        {
            msg::error("could not find Lua file %s\n", filename.c_str());
            return LUA_ERRFILE;
        }

        std::string s = f.ReadString();
        f.Close();

        msg::debug("loading Lua file %s\n", filename.c_str());
        status = LuaDoCode(l, s);

        if (status == 1)
        {
            stack.SetIndex(-1);
            auto error = stack.Get<std::string>();
            msg::error("Lua error %s\n", error.c_str());
            lua_pop(l, 1);
        }

        lua_pop(l, 1);

        return status;
    }
};

namespace Lolua
{

//-----------------------------------------------------------------------------
Loader::Loader()
{
    m_lua_state = luaL_newstate();
    lua_atpanic(m_lua_state, LuaBaseData::LuaPanic);
    luaL_openlibs(m_lua_state);

    /* Override dofile() */
    LuaFunction do_file(m_lua_state, "dofile", LuaBaseData::LuaDoFile);

    //Store this instance
    Loader::Store(m_lua_state, this);
}

//-----------------------------------------------------------------------------
Loader::~Loader()
{
    //Release this instance
    Loader::Release(m_lua_state, this);

    lua_close(m_lua_state);
}

//Store loader ------------------------------------------------------------
// FIXME: change this to a map?
static std::vector<std::tuple<lua_State*, Lolua::Loader*>> g_loaders;

void Loader::Store(lua_State* l, Lolua::Loader* loader)
{
    g_loaders.push_back(std::make_tuple(l, loader));
}

void Loader::Release(lua_State* l, Lolua::Loader* loader)
{
    for (size_t i = 0; i < g_loaders.size(); ++i)
    {
        if (std::get<0>(g_loaders[i]) == l && std::get<1>(g_loaders[i]) == loader)
        {
            remove_at(g_loaders, i);
            return;
        }
    }
}

//Store lua object --------------------------------------------------------
void Loader::StoreObject(lua_State* l, Object* obj)
{
    for (auto loader : g_loaders)
    {
        if (std::get<0>(loader) == l)
        {
            std::get<1>(loader)->Store(obj);
            return;
        }
    }
}

//-----------------------------------------------------------------------------
bool Loader::ExecLuaFile(std::string const &lua)
{
    lua_pushstring(m_lua_state, lua.c_str());
    int status = LuaBaseData::LuaDoFile(m_lua_state);
    return status == 0;
}

//-----------------------------------------------------------------------------
bool Loader::ExecLuaCode(std::string const &lua)
{
    return 0 == LuaBaseData::LuaDoCode(m_lua_state, lua);
}

//-----------------------------------------------------------------------------
lua_State* Loader::GetLuaState()
{
    return m_lua_state;
}

} /* namespace Lolua */

} /* namespace lol */

