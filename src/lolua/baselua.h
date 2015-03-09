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
typedef struct ClassVar
{
    const char *name;
    lua_CFunction get;
    lua_CFunction set;
} ClassVar;
typedef lua_State State;

//-----------------------------------------------------------------------------
struct ObjectLib
{
    typedef struct ClassVarStr
    {
        ClassVarStr() { }
        ClassVarStr(String var_name, lua_CFunction get, lua_CFunction set)
        {
            m_get_name = String("Get") + var_name;
            m_set_name = String("Set") + var_name;
            m_get = get;
            m_set = set;
        }
        String m_get_name = "";
        String m_set_name = "";
        lua_CFunction m_get = nullptr;
        lua_CFunction m_set = nullptr;
    } ClassVarStr;

    ObjectLib(String class_name,
        array<ClassMethod> statics,
        array<ClassMethod> methods,
        array<ClassVar> variables)
    {
        m_class_name = class_name;
        m_static_name = class_name + "_lib";
        m_method_name = class_name + "_inst";

        m_statics = statics;
        if (m_statics.Count() == 0
            || m_statics.Last().name != nullptr
            || m_statics.Last().func != nullptr)
            m_statics.Push({ nullptr, nullptr });

        m_methods = methods;
        if (m_methods.Count() == 0
            || m_methods.Last().name != nullptr
            || m_methods.Last().func != nullptr)
            m_methods.Push({ nullptr, nullptr });

        for (ClassVar& cv : variables)
        {
            if (cv.name && cv.get && cv.set)
            {
                m_variables.Push({ cv.name, cv.get, cv.set });
            }
        }
        if (m_variables.Count() == 0
            || variables.Last().name != nullptr
            || variables.Last().get != nullptr
            || variables.Last().set != nullptr)
            m_variables.Push(ClassVarStr());
    }
    String m_class_name = "";
    String m_static_name = "";
    String m_method_name = "";
    array<ClassMethod> m_statics;
    array<ClassMethod> m_methods;
    array<ClassVarStr> m_variables;
};

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
    static const ObjectLib* GetLib() { ASSERT(false); return nullptr; }
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
            { "New", New<TLuaClass> },
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
        luaL_newmetatable(l, GetStaticName<TLuaClass>());
        //Register default statics and template one
        luaL_setfuncs(l, default_statics, 0);
        luaL_setfuncs(l, GetStaticMethods<TLuaClass>(), 0);
        //Push metatable on stack
        lua_pushvalue(l, -1);
        //Set the "__index" field of the metatable to point to itself, pops the stack
        lua_setfield(l, -1, "__index");
        //Set it global to validate the operation
        lua_setglobal(l, GetObjectName<TLuaClass>());

        //Repeat all the operations for instance metatable
        luaL_newmetatable(l, GetMethodName<TLuaClass>());
        luaL_setfuncs(l, GetInstanceMethods<TLuaClass>(), 0);
        lua_pushvalue(l, -1);
        lua_setfield(l, -1, "__index");

        //Create variables Get/Set
        const array<ObjectLib::ClassVarStr>& variables = GetVariables<TLuaClass>();
        for (const ObjectLib::ClassVarStr& var : variables)
        {
            if (!var.m_get || !var.m_set)
                continue;

            //Add getter
            lua_pushcfunction(l, var.m_get);
            lua_setfield(l, -2, var.m_get_name.C());

            //Add setter
            lua_pushcfunction(l, var.m_set);
            lua_setfield(l, -2, var.m_set_name.C());
        }

        //Don't set it to global, but pop the stack to hide the metatable
        lua_pop(l, 1);
    }

private:
    template <typename TLuaClass>
    static const ObjectLib* GetLib()
    {
        const ObjectLib* lib = TLuaClass::GetLib();
        ASSERT(lib);
        return lib;
    }

public:
    template <typename TLuaClass>
    static const char* GetObjectName() { return GetLib<TLuaClass>()->m_class_name.C(); }
    template <typename TLuaClass>
    static const char* GetStaticName() { return GetLib<TLuaClass>()->m_static_name.C(); }
    template <typename TLuaClass>
    static const char* GetMethodName() { return GetLib<TLuaClass>()->m_method_name.C(); }
    template <typename TLuaClass>
    static const ClassMethod* GetStaticMethods() { return GetLib<TLuaClass>()->m_statics.Data(); }
    template <typename TLuaClass>
    static const ClassMethod* GetInstanceMethods() { return GetLib<TLuaClass>()->m_methods.Data(); }
    template <typename TLuaClass>
    static const array<ObjectLib::ClassVarStr>& GetVariables() { return GetLib<TLuaClass>()->m_variables; }

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
        luaL_getmetatable(l, GetMethodName<TLuaClass>());
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
class Function
{
public:
    Function(State* l, const char* name, int(*function)(State*))
    {
        lua_pushcfunction(l, function);
        lua_setglobal(l, name);
    }
};

//-----------------------------------------------------------------------------
template<typename T>
class VarPtr
{
protected:
    T* m_value = nullptr;

public:
    VarPtr()                        { }
    VarPtr(T* value)                { m_value = value; }
    VarPtr(State* l, int index)     { InnerGet(l, index); }
    inline T* V()                   { return m_value; }
    inline T* operator=(T* value)   { m_value = value; }
    inline int Return(State* l)     { InnerPush(l); return 1; }

protected:
    virtual void InnerGet(State* l, int index)
    {
        T** obj = static_cast<T**>(luaL_checkudata(l, index, Object::GetMethodName<T>()));
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
class VarPtrLight
{
public:
    VarPtrLight() : VarPtr() { }
    VarPtrLight(T* value) : VarPtr(T* value) { }
    VarPtrLight(State* l, int index) : VarPtr(State* l, int index) { }
protected:
    virtual void InnerGet(State* l, int index)
    {
        T** obj = static_cast<T**>(luaL_testudata(l, index, Object::GetMethodName<T>()));
        m_value = obj ? *obj : nullptr;
    }
};

//-----------------------------------------------------------------------------
template<typename T>
class Var
{
private:
    T m_value;

public:
    Var()                       { InnerInit(); }
    Var(T value)                { m_value = value; }
    Var(State* l, int index)    { InnerGet(l, index); }
    inline T& V()               { return m_value; }
    inline int Return(State* l) { InnerPush(l); return 1; }
    inline Var<T>&  operator-(const T& value)  { m_value - value; return *this; }
    inline Var<T>&  operator+(const T& value)  { m_value + value; return *this; }
    inline Var<T>&  operator*(const T& value)  { m_value  * value; return *this; }
    inline Var<T>&  operator/(const T& value)  { m_value / value; return *this; }
    inline Var<T>&  operator=(const T& value)  { m_value = value; return *this; }
    inline Var<T>& operator-=(const T& value)  { m_value -= value; return *this; }
    inline Var<T>& operator+=(const T& value)  { m_value += value; return *this; }
    inline Var<T>& operator*=(const T& value)  { m_value *= value; return *this; }
    inline Var<T>& operator/=(const T& value)  { m_value /= value; return *this; }
    inline Var<T>&  operator-(const Var<T>& o) { m_value - o.m_value; return *this; }
    inline Var<T>&  operator+(const Var<T>& o) { m_value + o.m_value; return *this; }
    inline Var<T>&  operator*(const Var<T>& o) { m_value  * o.m_value; return *this; }
    inline Var<T>&  operator/(const Var<T>& o) { m_value / o.m_value; return *this; }
    inline Var<T>&  operator=(const Var<T>& o) { m_value = o.m_value; return *this; }
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
