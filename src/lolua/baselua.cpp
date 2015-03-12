//
//  Base Lua class for Lua script loading
//
//  Copyright: (c) 2009-2015 Sam Hocevar <sam@hocevar.net>
//                 2009-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This program is free software. It comes without any warranty, to
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

    static int LuaPanic(LuaState* l)
    {
        char const *message = lua_tostring(l, -1);
        Log::Error("%s\n", message);
        DebugAbort();
        return 0;
    }

    static int LuaDoFile(LuaState *l)
    {
        if (lua_isnoneornil(l, 1))
            return LUA_ERRFILE;

        LuaCharPtr var; var.Get(l, 1);
        char const *filename = var;// lua_tostring(l, 1);
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
                status = luaL_dostring(l, s.C());
                break;
            }
        }

        if (status == LUA_ERRFILE)
            Log::Error("could not find Lua file %s\n", filename);
        else if (status == 1)
        {
            LuaString error; error.Get(l, -1);
            Log::Error("Lua error %s\n", error().C());
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
}

//-----------------------------------------------------------------------------
Loader::~Loader()
{
    lua_close(m_lua_state);
}

//-----------------------------------------------------------------------------
bool Loader::ExecLua(String const &lua)
{
    const char* c = lua_pushstring(m_lua_state, lua.C());
    int status = LuaBaseData::LuaDoFile(m_lua_state);
    return status == 0;
}

//-----------------------------------------------------------------------------
LuaState* Loader::GetLuaState()
{
    return m_lua_state;
}

} /* namespace Lolua */

} /* namespace lol */

