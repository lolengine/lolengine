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
//#include "lua/luawrapper.hpp"

#pragma once

namespace lol
{

//-----------------------------------------------------------------------------
namespace Lolua
{

//-----------------------------------------------------------------------------
typedef luaL_Reg ClassMethod;
typedef lua_State State;

//-----------------------------------------------------------------------------
class ObjectDef
{
public:
    ObjectDef() { }
    virtual ~ObjectDef() { }
    static ObjectDef* New(State* l, int arg_nb)
    {
        UNUSED(l);
        UNUSED(arg_nb);
        ASSERT(false);
        return nullptr;
    }
    static const char* GetClassName()               { ASSERT(false); return nullptr; }
    static const char* GetClassLibName()            { ASSERT(false); return nullptr;; }
    static const char* GetClassInstName()           { ASSERT(false); return nullptr; }
    static const ClassMethod* GetStaticMethods()    { ASSERT(false); return nullptr; }
    static const ClassMethod* GetInstanceMethods()  { ASSERT(false); return nullptr; }
};

//-----------------------------------------------------------------------------
// Class available to link C++ class to Lua methods
//--
class Object
{
public:
    //-------------------------------------------------------------------------
    Object() { }
    virtual ~Object() { }

    //-------------------------------------------------------------------------
    template <typename TLuaClass>
    static void Register(State *l)
    {
        //Default statics
        static const luaL_Reg default_statics[]
        {
            { "New",  New<TLuaClass> },
            { "__gc", Del<TLuaClass> },
            { NULL, NULL }
        };
        //TODO: Touky: Implement that
        //__tostring : ToString
        //__add : Addition(+)
        //__sub : Subtraction(-)
        //__mul : Multiplication(*)
        //__div : Division(/ )
        //__mod : Modulos(%)
        //__unm : Unary - , used for negation on numbers
        //__concat : Concatenation(..)
        //__eq : Equality(== )
        //__lt : Less than(<)
        //__le : Less than or equal to(<= )

        //Create Static metatable
        luaL_newmetatable(l, TLuaClass::GetClassLibName());
        //Register default statics and template one
        luaL_setfuncs(l, default_statics, 0);
        luaL_setfuncs(l, TLuaClass::GetStaticMethods(), 0);
        //Push funcs on metatable
        lua_pushvalue(l, -1);
        //Set the "__index" field of the metatable to point to itself, pops the stack
        lua_setfield(l, -1, "__index");
        //Set it global to validate the operation
        lua_setglobal(l, TLuaClass::GetClassName());

        //Repeat all the operations for instance metatable
        luaL_newmetatable(l, TLuaClass::GetClassInstName());
        luaL_setfuncs(l, TLuaClass::GetInstanceMethods(), 0);
        lua_pushvalue(l, -1);
        lua_setfield(l, -1, "__index");
        //Don't set it to global, but pop the stack to hide the metatable
        lua_pop(l, 1);
    }

protected:
    //-------------------------------------------------------------------------
    template <typename TLuaClass>
    static int New(State* l)
    {
        //Number of arguments
        int n_args = lua_gettop(l);

        //Create user data
        TLuaClass** data = (TLuaClass**)lua_newuserdata(l, sizeof(TLuaClass*));
        *data = TLuaClass::New(l, n_args);

        //Retrieve instance table
        luaL_getmetatable(l, TLuaClass::GetClassInstName());
        //Set metatable to instance
        lua_setmetatable(l, -2);
        //Return 1 so Lua will get the UserData and clean the stack.
        return 1;
    }

    //-------------------------------------------------------------------------
    template <typename TLuaClass>
    static int Del(State * l)
    {
        VarPtr<TLuaClass> obj(l, 1);
        ASSERT(obj.V());
        delete obj.V();
        return 0;
    }
};

//-----------------------------------------------------------------------------
//
//--
struct Function
{
    Function(State* l, const char* name, int(*function)(State*))
    {
        lua_pushcfunction(l, function);
        lua_setglobal(l, name);
    }
};

//-----------------------------------------------------------------------------
template<typename T>
struct VarPtr
{
protected:
    T* m_value = nullptr;

public:
    VarPtr()                        { }
    VarPtr(T* value)                { m_value = value; }
    VarPtr(State* l, int index) { InnerGet(l, index); }
    inline T* V()                   { return m_value; }
    inline T* operator=(T* value)   { m_value = value; }
    inline int Return(State* l) { InnerPush(l); return 1; }

protected:
    virtual void InnerGet(State* l, int index)
    {
        T** obj = static_cast<T**>(luaL_checkudata(l, index, T::GetClassInstName()));
        m_value = obj ? *obj : nullptr;
    }
    void InnerPush(State* l)
    {
        T** data = (T**)lua_newuserdata(l, sizeof(T*));
        *data = m_value;
    }
};
//-----------------------------------------------------------------------------
template<typename T>
struct VarPtrLight
{
public:
    VarPtrLight() : VarPtr() { }
    VarPtrLight(T* value) : VarPtr(T* value) { }
    VarPtrLight(State* l, int index) : VarPtr(State* l, int index) { }
protected:
    virtual void InnerGet(State* l, int index)
    {
        T** obj = static_cast<T**>(luaL_testudata(l, index, T::GetClassInstName()));
        m_value = obj ? *obj : nullptr;
    }
};

//-----------------------------------------------------------------------------
template<typename T>
struct Var
{
private:
    T m_value;

public:
    Var()                       { InnerInit(); }
    Var(T value)                { m_value = value; }
    Var(State* l, int index)    { InnerGet(l, index); }
    inline T& V()               { return m_value; }
    inline int Return(State* l) { InnerPush(l); return 1; }
    inline Var<T>&  operator-(const T& value)  { m_value  - value; return *this; }
    inline Var<T>&  operator+(const T& value)  { m_value  + value; return *this; }
    inline Var<T>&  operator*(const T& value)  { m_value  * value; return *this; }
    inline Var<T>&  operator/(const T& value)  { m_value  / value; return *this; }
    inline Var<T>&  operator=(const T& value)  { m_value  = value; return *this; }
    inline Var<T>& operator-=(const T& value)  { m_value -= value; return *this; }
    inline Var<T>& operator+=(const T& value)  { m_value += value; return *this; }
    inline Var<T>& operator*=(const T& value)  { m_value *= value; return *this; }
    inline Var<T>& operator/=(const T& value)  { m_value /= value; return *this; }
    inline Var<T>&  operator-(const Var<T>& o) { m_value  - o.m_value; return *this; }
    inline Var<T>&  operator+(const Var<T>& o) { m_value  + o.m_value; return *this; }
    inline Var<T>&  operator*(const Var<T>& o) { m_value  * o.m_value; return *this; }
    inline Var<T>&  operator/(const Var<T>& o) { m_value  / o.m_value; return *this; }
    inline Var<T>&  operator=(const Var<T>& o) { m_value  = o.m_value; return *this; }
    inline Var<T>& operator-=(const Var<T>& o) { m_value -= o.m_value; return *this; }
    inline Var<T>& operator+=(const Var<T>& o) { m_value += o.m_value; return *this; }
    inline Var<T>& operator*=(const Var<T>& o) { m_value *= o.m_value; return *this; }
    inline Var<T>& operator/=(const Var<T>& o) { m_value /= o.m_value; return *this; }
    inline bool operator==(const T& value)     { return m_value == value; }
    inline bool operator!=(const T& value)     { return m_value != value; }
    inline bool operator==(const Var<T>& o) { return m_value == o.m_value; }
    inline bool operator!=(const Var<T>& o) { return m_value != o.m_value; }

private:
    void InnerInit()                   { m_value = T(0); }
    void InnerGet(State* l, int index) { ASSERT(false); }
    void InnerPush(State* l)           { ASSERT(false); }
};

//-----------------------------------------------------------------------------
template<> inline void Var<String>     ::InnerInit() { m_value = String(); }
template<> inline void Var<String>     ::InnerGet(State* l, int index) { m_value = lua_tostring(l, index); }
template<> inline void Var<char const*>::InnerGet(State* l, int index) { m_value = lua_tostring(l, index); }
template<> inline void Var<double>     ::InnerGet(State* l, int index) { m_value = lua_tonumber(l, index); }
template<> inline void Var<float>      ::InnerGet(State* l, int index) { m_value = (float)lua_tonumber(l, index); }
template<> inline void Var<int32_t>    ::InnerGet(State* l, int index) { m_value = (int32_t)lua_tointeger(l, index); }
template<> inline void Var<int64_t>    ::InnerGet(State* l, int index) { m_value = lua_tointeger(l, index); }
template<> inline void Var<uint32_t>   ::InnerGet(State* l, int index) { m_value = lua_tounsigned(l, index); }

template<> inline void Var<String>     ::InnerPush(State* l) { lua_pushstring(l, m_value.C()); }
template<> inline void Var<char const*>::InnerPush(State* l) { lua_pushstring(l, m_value); }
template<> inline void Var<double>     ::InnerPush(State* l) { lua_pushnumber(l, m_value); }
template<> inline void Var<float>      ::InnerPush(State* l) { lua_pushnumber(l, m_value); }
template<> inline void Var<int32_t>    ::InnerPush(State* l) { lua_pushinteger(l, m_value); }
template<> inline void Var<int64_t>    ::InnerPush(State* l) { lua_pushinteger(l, m_value); }
template<> inline void Var<uint32_t>   ::InnerPush(State* l) { lua_pushunsigned(l, m_value); }

//-----------------------------------------------------------------------------
class Loader
{
public:
    Loader();
    virtual ~Loader();

    bool ExecLua(String const &lua);

    template<typename T>
    T GetVar(String const &name)
    {
        lua_getglobal(m_lua_state, name.C());
        Var<T> var(m_lua_state, -1);
        lua_pop(m_lua_state, 1);
        return var.V();
    }
    template<typename T>
    T* GetPtr(String const &name)
    {
        lua_getglobal(m_lua_state, name.C());
        VarPtr<T> var(m_lua_state, -1);
        lua_pop(m_lua_state, 1);
        return var.V();
    }

protected:
    Lolua::State* GetLuaState();

private:
    Lolua::State* m_lua_state;
};

} /* namespace Lolua */

} /* namespace lol */
