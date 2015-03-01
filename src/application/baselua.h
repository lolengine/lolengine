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
#define LOLUA_CLASS_STATIC(ClassType, FuncName) Static ## ClassType ## FuncName
#define LOLUA_CLASS_METHOD(ClassType, FuncName) Method ## ClassType ## FuncName
#define LOLUA_CLASS_STATIC_C(ClassType, FuncName) Static ## #ClassType ## #FuncName
#define LOLUA_CLASS_METHOD_C(ClassType, FuncName) Method ## #ClassType ## #FuncName

#define LOLUA_BEGIN_CTOR(ClassType) \
    ClassType* ClassType##New(lua_State* L) \
    { \
    int arg_idx = 1; \

#define LOLUA_END_CTOR(ClassType, ...) \
    return new ClassType(__VA_ARGS__); \
} \

#define LOLUA_BEGIN_STATIC(FuncName) \
    int FuncName(lua_State* L) \
    { \
    int arg_idx = 1; \

#define LOLUA_BEGIN_CLASS_STATIC(ClassType, FuncName) \
    int LOLUA_CLASS_STATIC(ClassType, FuncName)(lua_State* L) \
    { \
    int arg_idx = 1; \

#define LOLUA_BEGIN_CLASS_METHOD(ClassType, FuncName) \
    int LOLUA_CLASS_METHOD(ClassType, FuncName)(lua_State* L) \
    { \
    int arg_idx = 1; \
    ClassType* this_ptr = luaW_check<ClassType>(L, arg_idx); \

#define LOLUA_CALL_FUNC(FuncName, ...)                  FuncName(__VA_ARGS__);
#define LOLUA_CALL_FUNC_RET(FuncName, ...)              auto result = FuncName(__VA_ARGS__);
#define LOLUA_CALL_STATIC(ClassType, FuncName, ...)     ClassType::FuncName(__VA_ARGS__);
#define LOLUA_CALL_STATIC_RET(ClassType, FuncName, ...) auto result = ClassType::FuncName(__VA_ARGS__);
#define LOLUA_CALL_METHOD(FuncName, ...)                this_ptr->FuncName(__VA_ARGS__);
#define LOLUA_CALL_METHOD_RET(FuncName, ...)            auto result = this_ptr->FuncName(__VA_ARGS__);

#define LOLUA_ARG_STRING(VarName)       String VarName = luaL_checkstring(L, arg_idx++);
#define LOLUA_ARG_CHAR(VarName)         const char* VarName = luaL_checkstring(L, arg_idx++);
#define LOLUA_ARG_DOUBLE(VarName)       double VarName = luaL_checknumber(L, arg_idx++);
#define LOLUA_ARG_INT(VarName)          int32_t VarName = luaL_checkinteger(L, arg_idx++);
#define LOLUA_ARG_UINT(VarName)         uint32_t VarName = luaL_checkunsigned(L, arg_idx++);
#define LOLUA_ARG_OBJ(VarType, VarName) VarType* VarName = luaW_check<VarType>(L, arg_idx++);

#define LOLUA_END_FUNC_STRING(VarName)  lua_pushstring(L, VarName.C()); return 1; }
#define LOLUA_END_FUNC_CHAR(VarName)    lua_pushstring(L, VarName); return 1; }
#define LOLUA_END_FUNC_FLOAT(VarName)   lua_pushnumber(L, VarName); return 1; }
#define LOLUA_END_FUNC_INT(VarName)     lua_pushinteger(L, VarName); return 1; }
#define LOLUA_END_FUNC_UINT(VarName)    lua_pushunsigned(L, VarName); return 1; }
#define LOLUA_END_FUNC_OBJ(VarName)     luaW_push<VarType>(L, VarName); return 1; }
#define LOLUA_END_FUNC_STRING_RESULT    LOLUA_END_FUNC_STRING(result)
#define LOLUA_END_FUNC_CHAR_RESULT      LOLUA_END_FUNC_CHAR(result)
#define LOLUA_END_FUNC_FLOAT_RESULT     LOLUA_END_FUNC_FLOAT(result)
#define LOLUA_END_FUNC_INT_RESULT       LOLUA_END_FUNC_INT(result)
#define LOLUA_END_FUNC_UINT_RESULT      LOLUA_END_FUNC_UINT(result)
#define LOLUA_END_FUNC_OBJ_RESULT       LOLUA_END_FUNC_OBJ(result)
#define LOLUA_END_FUNC_VOID             return 0; }

#define LOLUA_BEGIN_TABLE_STATIC(ClassType)         static luaL_Reg ClassType##StaticTable[] = {
#define LOLUA_BEGIN_TABLE_METHOD(ClassType)         static luaL_Reg ClassType##MethodTable[] = {
#define LOLUA_ADD_TABLE_STATIC(ClassType, FuncName) { LOLUA_CLASS_STATIC_C(ClassType, FuncName), LOLUA_CLASS_STATIC(ClassType, FuncName) },
#define LOLUA_ADD_TABLE_METHOD(ClassType, FuncName) { LOLUA_CLASS_METHOD_C(ClassType, FuncName), LOLUA_CLASS_METHOD(ClassType, FuncName) },
#define LOLUA_END_TABLE                             { NULL, NULL } };

#define LOLUA_DEFINE_CLASS_LIBRARY(ClassType) \
    static int luaopen_##ClassType(lua_State* L) \
    { luaW_register<ClassType>(L, #ClassType, ClassType##StaticTable, ClassType##MethodTable, ClassType##New); return 1; }\

#define LOLUA_NEW_STATE(VarName) lua_State* L = (VarName = luaL_newstate());
#define LOLUA_USE_STATE(VarName) lua_State* L = VarName;
#define LOLUA_REGISTER_CLASS_LIBRARY(ClassType) luaopen_##ClassType(L);

#define LOLUA_REGISTER_STATIC(LuaFuncName, CppFuncName) lua_pushcfunction(L, CppFuncName); lua_setglobal(L, #LuaFuncName);

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
