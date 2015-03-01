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

#include "lua/lua.hpp"
#include "lua/luawrapper.hpp"

#pragma once

namespace lol
{

//-----------------------------------------------------------------------------
class LuaObject
{
protected:
    template<typename T, const char* name, const luaL_Reg* statics, const luaL_Reg* methods, T* (*ctor)(lua_State*)>
    struct LuaLibrary
    {
        LuaLibrary() { }
        void LoadTo(lua_State* l) { luaW_register<T>(l, name, statics, methods, ctor); }
    };
};

//-----------------------------------------------------------------------------
struct LuaFunction
{
    LuaFunction(lua_State* l, const char* name, int (*function)(lua_State*))
    {
        lua_pushcfunction(l, function);
        lua_setglobal(l, name);
    }
};

//-----------------------------------------------------------------------------
template<typename T>
struct LuaVar
{
private:
    T m_value = T(0);

public:
    LuaVar()                            { }
    LuaVar(T value)                     { m_value = value; }
    LuaVar(lua_State* l, int index)     { InnerGet(l, index); }
    inline T& V()                       { return m_value; }
    inline T& operator=(const T& value) { m_value = value; }
    inline int Return(lua_State* l)     { InnerPush(l); return 1; }

private:
    void InnerGet(lua_State* l, int index) { ASSERT(false); }
    void InnerPush(lua_State* l)           { ASSERT(false); }
};

//-----------------------------------------------------------------------------
template<typename T>
struct LuaPtr
{
private:
    T* m_value = nullptr;

public:
    LuaPtr()                        { }
    LuaPtr(T* value)                { m_value = value; }
    LuaPtr(lua_State* l, int index) { InnerGet(l, index); }
    inline T* V()                   { return m_value; }
    inline T* operator=(T* value)   { m_value = value; }
    inline int Return(lua_State* l) { InnerPush(l); return 1; }

private:
    void InnerGet(lua_State* l, int index) { m_value = luaW_check<T>(l, index); }
    void InnerPush(lua_State* l)           { luaW_push<T>(l, m_value); }
};

//-----------------------------------------------------------------------------
template<> inline void LuaVar<String>     ::InnerGet(lua_State* l, int index) { m_value = lua_tostring(l, index); }
template<> inline void LuaVar<char const*>::InnerGet(lua_State* l, int index) { m_value = lua_tostring(l, index); }
template<> inline void LuaVar<double>     ::InnerGet(lua_State* l, int index) { m_value = lua_tonumber(l, index); }
template<> inline void LuaVar<float>      ::InnerGet(lua_State* l, int index) { m_value = (float)lua_tonumber(l, index); }
template<> inline void LuaVar<int32_t>    ::InnerGet(lua_State* l, int index) { m_value = (int32_t)lua_tointeger(l, index); }
template<> inline void LuaVar<int64_t>    ::InnerGet(lua_State* l, int index) { m_value = lua_tointeger(l, index); }
template<> inline void LuaVar<uint32_t>   ::InnerGet(lua_State* l, int index) { m_value = lua_tounsigned(l, index); }

template<> inline void LuaVar<String>     ::InnerPush(lua_State* l) { lua_pushstring(l, m_value.C()); }
template<> inline void LuaVar<char const*>::InnerPush(lua_State* l) { lua_pushstring(l, m_value); }
template<> inline void LuaVar<double>     ::InnerPush(lua_State* l) { lua_pushnumber(l, m_value); }
template<> inline void LuaVar<float>      ::InnerPush(lua_State* l) { lua_pushnumber(l, m_value); }
template<> inline void LuaVar<int32_t>    ::InnerPush(lua_State* l) { lua_pushinteger(l, m_value); }
template<> inline void LuaVar<int64_t>    ::InnerPush(lua_State* l) { lua_pushinteger(l, m_value); }
template<> inline void LuaVar<uint32_t>   ::InnerPush(lua_State* l) { lua_pushunsigned(l, m_value); }

//-----------------------------------------------------------------------------
class LuaLoader
{
public:
    LuaLoader();
    virtual ~LuaLoader();

    bool ExecLua(String const &lua);
    double GetLuaNumber(String const &var);

    template<typename T>
    T GetVar(String const &name)
    {
        lua_getglobal(m_lua_state, name.C());
        LuaVar<T> var(m_lua_state, -1);
        lua_pop(m_lua_state, 1);
        return var.V();
    }
    template<typename T>
    T* GetPtr(String const &name)
    {
        lua_getglobal(m_lua_state, name.C());
        LuaPtr<T> var(m_lua_state, -1);
        lua_pop(m_lua_state, 1);
        return var.V();
    }

protected:
    lua_State* GetLuaState();

private:
    lua_State* m_lua_state;
};

} /* namespace lol */
