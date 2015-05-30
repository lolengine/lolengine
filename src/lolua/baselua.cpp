//
//  Base Lua class for Lua script loading
//
//  Copyright © 2009—2015 Sam Hocevar <sam@hocevar.net>
//            © 2009—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>
#include <cstdlib>
#include <ctype.h>

namespace lol
{

//-----------------------------------------------------------------------------
class LuaBaseData
{
    friend class Lolua::Loader;

    //PANIC PANIC -------------------------------------------------------------
    static int LuaPanic(LuaState* l)
    {
        char const *message = lua_tostring(l, -1);
        msg::error("%s\n", message);
        DebugAbort();
        return 0;
    }

    //Exec lua code -----------------------------------------------------------
    static int LuaDoCode(LuaState *l, String const& s)
    {
        return luaL_dostring(l, s.C());
    }

    //Open a file and exec lua code -------------------------------------------
    static int LuaDoFile(LuaState *l)
    {
        if (lua_isnoneornil(l, 1))
            return LUA_ERRFILE;

        LuaCharPtr var; var.Get(l, 1);
        char const *filename = var;// lua_tostring(l, 1);
        int status = LUA_ERRFILE;

        File f;
        for (auto candidate : System::GetPathList(filename))
        {
            f.Open(candidate, FileAccess::Read);
            if (f.IsValid())
            {
                String s = f.ReadString();
                f.Close();

                msg::debug("loading Lua file %s\n", candidate.C());
                status = LuaDoCode(l, s);
                break;
            }
        }

        if (status == LUA_ERRFILE)
            msg::error("could not find Lua file %s\n", filename);
        else if (status == 1)
        {
            LuaString error; error.Get(l, -1);
            msg::error("Lua error %s\n", error().C());
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
    Loader::Store(m_lua_state, this);

    lua_close(m_lua_state);
}

//Store loader ------------------------------------------------------------
static array<LuaState*, Lolua::Loader*> g_loaders;
void Loader::Store(LuaState* l, Lolua::Loader* loader)
{
    g_loaders.push(l, loader);
}
void Loader::Release(LuaState* l, Lolua::Loader* loader)
{
    for (int i = 0; i < g_loaders.count(); ++i)
    {
        if (g_loaders[i].m1 == l && g_loaders[i].m2 == loader)
        {
            g_loaders.remove(i);
            return;
        }
    }
}

//Store lua object --------------------------------------------------------
void Loader::StoreObject(LuaState* l, Object* obj)
{
    for (auto loader : g_loaders)
    {
        if (loader.m1 == l)
        {
            loader.m2->Store(obj);
            return;
        }
    }
}

//-----------------------------------------------------------------------------
bool Loader::ExecLuaFile(String const &lua)
{
    const char* c = lua_pushstring(m_lua_state, lua.C());
    int status = LuaBaseData::LuaDoFile(m_lua_state);
    return status == 0;
}

//-----------------------------------------------------------------------------
bool Loader::ExecLuaCode(String const &lua)
{
    return 0 == LuaBaseData::LuaDoCode(m_lua_state, lua);
}

//-----------------------------------------------------------------------------
LuaState* Loader::GetLuaState()
{
    return m_lua_state;
}

} /* namespace Lolua */

} /* namespace lol */

