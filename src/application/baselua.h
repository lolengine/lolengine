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
template <typename T>
class ObjectDef
{
public:
    ObjectDef() { }
    virtual ~ObjectDef() { }
    static ObjectDef<T>* New(State* l, int arg_nb)
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

protected:
    T* m_instance = nullptr;
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
        LuaPtr<T> var(m_lua_state, -1);
        lua_pop(m_lua_state, 1);
        return var.V();
    }

protected:
    Lolua::State* GetLuaState();

private:
    Lolua::State* m_lua_state;
};

//-----------------------------------------------------------------------------
//class T definition
//class T
//{
//public:
//	int Ctor(lua_State* l) { return 0; }
//	static const char*                  m_class_name;
//	static const Class<T>::Variable*    m_variables;
//	static const Class<T>::Method* 	    m_methods;
//};
/*
//-----------------------------------------------------------------------------
template<typename T>
class ClassTest
{
public:
    //-----------------------------------------------------------------------------
    struct Variable
    {
        const char*		m_name;
        int(*Getter)(lua_State*);
        int(*Setter)(lua_State*);
    };

    //-----------------------------------------------------------------------------
    struct Method
    {
        const char*		m_name;
        int(*Func)(lua_State*);
    };

    //Register: Register class in Lua. Leave library empty to store class in global scope.
    static void Register(lua_State* l, const char *library = NULL)
    {
        if (library && strlen(library))
        {
            lua_getglobal(l, library);
            //Create library if not present
            if (lua_isnil(l, -1))
            {
                lua_newtable(l);
                //Duplicate table pointer since setglobal pops the value
                lua_pushvalue(l, -1);
                lua_setglobal(l, library);
            }
            //lua_pushcfunction(l, &Class<T>::Ctor);
            //lua_setfield(l, -2, T::m_class_name);
            //lua_pop(l, 1);
        }
        else
        {
            //lua_pushcfunction(l, &Class<T>::Ctor);
            //lua_setglobal(l, T::m_class_name);
        }

        luaL_newmetatable(l, T::m_class_name);
        int metatable = lua_gettop(l);

        //Register all m_methods: Register Name for fun_call and number for choice
        for (int i = 0; T::m_methods[i].m_name; i++)
        {
            lua_pushstring(l, T::m_methods[i].m_name);
            lua_pushcfunction(l, T::m_methods[i].Func);
            lua_settable(l, metatable);
        }

        //lua_pushstring(l, "__gc");
        //lua_pushcfunction(l, &Class<T>::Dtor);
        //lua_settable(l, metatable);

        //lua_pushstring(l, "__tostring");
        //lua_pushcfunction(l, &Class<T>::ToString);
        //lua_settable(l, metatable);

        //lua_pushstring(l, "__eq");
        //lua_pushcfunction(l, &Class<T>::Equals);
        //lua_settable(l, metatable);

        //lua_pushstring(l, "__index");
        //lua_pushcfunction(l, &Class<T>::Getter);
        //lua_settable(l, metatable);

        //lua_pushstring(l, "__newindex");
        //lua_pushcfunction(l, &Class<T>::Setter);
        //lua_settable(l, metatable);

        ////Register all properties: Register Name for fun_call and number for choice
        //for (int i = 0; T::m_variables[i].m_name; i++)
        //{
        //    lua_pushstring(l, T::m_variables[i].m_name);
        //    lua_pushnumber(l, i);
        //    lua_settable(l, metatable);
        //}

        ////Register all m_methods: Register Name for fun_call and number for choice
        //for (int i = 0; T::m_methods[i].m_name; i++)
        //{
        //    lua_pushstring(l, T::m_methods[i].m_name);
        //    //Add (1 << 8) to identify function on call
        //    lua_pushnumber(l, i | (1 << 8));
        //    lua_settable(l, metatable);
        //}
    }

};

//-----------------------------------------------------------------------------
template<typename T>
class Class
{
public:
    //-----------------------------------------------------------------------------
    struct Variable
    {
        const char*		m_name;
        int             (T::*Getter)(lua_State*);
        int             (T::*Setter)(lua_State*);
    };

    //-----------------------------------------------------------------------------
    struct Method
    {
        const char*		m_name;
        int             (T::*Func)(lua_State*);
    };

    //-----------------------------------------------------------------------------
    //Check: Raise an exception if FAIL
    static T* Check(lua_State * l, int narg)
    {
        T** obj = static_cast <T **>(luaL_checkudata(l, narg, T::m_class_name));
        return !obj ? nullptr : *obj;
    }

    //LightCheck: Returns nullptr if FAIL
    static T* LightCheck(lua_State* l, int num_args)
    {
        T** obj = static_cast <T **>(luaL_testudata(l, num_args, T::m_class_name));
        return !obj ? nullptr : *obj;
    }

    //Register: Register class in Lua. Leave library empty to store class in global scope.
    static void Register(lua_State* l, const char *library = NULL)
    {
        if (library && strlen(library))
        {
            lua_getglobal(l, library);
            //Create library if not present
            if (lua_isnil(l, -1))
            {
                lua_newtable(l);
                //Duplicate table pointer since setglobal pops the value
                lua_pushvalue(l, -1);
                lua_setglobal(l, library);
            }
            lua_pushcfunction(l, &Class<T>::Ctor);
            lua_setfield(l, -2, T::m_class_name);
            lua_pop(l, 1);
        }
        else
        {
            lua_pushcfunction(l, &Class<T>::Ctor);
            lua_setglobal(l, T::m_class_name);
        }

        luaL_newmetatable(l, T::m_class_name);
        int metatable = lua_gettop(l);

        //lua_pushstring(l, "__gc");
        //lua_pushcfunction(l, &Class<T>::Dtor);
        //lua_settable(l, metatable);

        //lua_pushstring(l, "__tostring");
        //lua_pushcfunction(l, &Class<T>::ToString);
        //lua_settable(l, metatable);

        //lua_pushstring(l, "__eq");
        //lua_pushcfunction(l, &Class<T>::Equals);
        //lua_settable(l, metatable);

        lua_pushstring(l, "__index");
        lua_pushcfunction(l, &Class<T>::Getter);
        lua_settable(l, metatable);

        lua_pushstring(l, "__newindex");
        lua_pushcfunction(l, &Class<T>::Setter);
        lua_settable(l, metatable);

        //Register all properties: Register Name for fun_call and number for choice
        for (int i = 0; T::m_variables[i].m_name; i++)
        {
            lua_pushstring(l, T::m_variables[i].m_name);
            lua_pushnumber(l, i);
            lua_settable(l, metatable);
        }

        //Register all m_methods: Register Name for fun_call and number for choice
        for (int i = 0; T::m_methods[i].m_name; i++)
        {
            lua_pushstring(l, T::m_methods[i].m_name);
            //Add (1 << 8) to identify function on call
            lua_pushnumber(l, i | (1 << 8));
            lua_settable(l, metatable);
        }
    }

private:
    //Ctor
    static int Ctor(lua_State* l)
    {
        T*  obj = new T(l);
        T** lua_obj = static_cast<T**>(lua_newuserdata(l, sizeof(T*))); // Push value = userdata
        *lua_obj = obj;

        luaL_getmetatable(l, T::m_class_name); 		// Fetch global metatable T::classname
        lua_setmetatable(l, -2);
        return 1;
    }

    //Dtor
    static int Dtor(lua_State* l)
    {
        T** lua_obj = static_cast<T**>(lua_touserdata(l, -1));
        if (lua_obj && *lua_obj)
            delete(*lua_obj);
        return 0;
    }

    //AddInstance
    static void AddInstance(lua_State* l, T* instance)
    {
        T** a = (T**)lua_newuserdata(l, sizeof(T*)); // Create userdata
        *a = instance;

        luaL_getmetatable(l, T::m_class_name);
        lua_setmetatable(l, -2);
    }

    //----
    static void GetVarIndex(lua_State* l)
    {
        lua_getmetatable(l, 1); //Look up the index of a m_name
        lua_pushvalue(l, 2); 	//Push the m_name
        lua_rawget(l, -2);		//Get the index
    }

    //----
    static void PrepareGetSet(lua_State* l)
    {
        lua_pop(l, 2);    // Pop metatable and idx
        lua_remove(l, 1); // Remove userdata
        lua_remove(l, 1); // Remove [key]
    }

    //Getter: Call a func or a getter var
    static int Getter(lua_State* l)
    {
        GetVarIndex(l);
        //Check if we got a valid index
        if (lua_isnumber(l, -1))
        {
            int idx = lua_tonumber(l, -1);
            T** obj = static_cast<T**>(lua_touserdata(l, 1));
            lua_pushvalue(l, 3);

            //It's a method
            if (idx & (1 << 8))
            {
                //Push the Method index and call the function
                lua_pushnumber(l, idx ^ (1 << 8));
                lua_pushlightuserdata(l, obj);
                lua_pushcclosure(l, &Class<T>::CallMethod, 2);
                // Return a Method
                return 1;
            }
            else
            {
                PrepareGetSet(l);
                return ((*obj)->*(T::m_variables[idx].Getter))(l);
            }
        }
        return 1;
    }

    //Getter: Class a setter var or error if func
    static int Setter(lua_State* l)
    {
        GetVarIndex(l);
        //Check if we got a valid index
        if (lua_isnumber(l, -1))
        {
            int idx = lua_tonumber(l, -1);
            T** obj = static_cast<T**>(lua_touserdata(l, 1));
            if (!obj || !*obj)
            {
                luaL_error(l, "Internal error, no object given!");
                return 0;
            }

            //Try to set a Method - FAIL
            if (idx & (1 << 8))
            {
                String s = String::Printf("Trying to set the method [%s] of class [%s]", (*obj)->T::m_methods[idx ^ (1 << 8)].m_name, T::m_class_name);
                luaL_error(l, s.C());
                return 0;
            }
            else
            {
                PrepareGetSet(l);
                return ((*obj)->*(T::m_variables[idx].Setter))(l);
            }
        }
        return 0;
    }

    //CallMethod: Actually calls the method on the object
    static int CallMethod(lua_State* l)
    {
        int i = (int)lua_tonumber(l, lua_upvalueindex(1));
        T** obj = static_cast <T**>(lua_touserdata(l, lua_upvalueindex(2)));
        return ((*obj)->*(T::m_methods[i].Func))(l);
    }

    //ToString:
    static int ToString(lua_State* l)
    {
        T** obj = static_cast<T**>(lua_touserdata(l, -1));
        if (obj)
            lua_pushfstring(l, "%s (%p)", T::m_class_name, (void*)*obj);
        else
            lua_pushstring(l, "Empty object");
        return 1;
    }

    //Equals:
    static int Equals(lua_State* l)
    {
        T** obj1 = static_cast<T**>(lua_touserdata(l, -1));
        T** obj2 = static_cast<T**>(lua_touserdata(l, 1));
        lua_pushboolean(l, *obj1 == *obj2);
        return 1;
    }
};
*/

} /* namespace Lolua */

} /* namespace lol */
