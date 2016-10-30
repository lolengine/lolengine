//
//  Lol Engine — base class for Lua script loading
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

#include "3rdparty/lua/src/lua.hpp"
//#include "lua/luawrapper.hpp"

#pragma once

#define OLD_VAR_SYSTEM true
#define NEW_VAR_SYSTEM true

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

//-----------------------------------------------------------------------------
class Object
{
public:
    //-----------------------------------------------------------------------------
    struct Library
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

        Library(String class_name,
            array<ClassMethod> const& statics,
            array<ClassMethod> const& methods,
            array<ClassVar> const& variables)
        {
            m_class_name = class_name;
            m_static_name = class_name + "_lib";
            m_method_name = class_name + "_inst";

            m_statics = statics;
            if (m_statics.count() == 0
                || m_statics.last().name != nullptr
                || m_statics.last().func != nullptr)
                m_statics.push({ nullptr, nullptr });

            m_methods = methods;
            if (m_methods.count() == 0
                || m_methods.last().name != nullptr
                || m_methods.last().func != nullptr)
                m_methods.push({ nullptr, nullptr });

            for (ClassVar const& cv : variables)
            {
                if (cv.name && cv.get && cv.set)
                {
                    m_variables.push({ cv.name, cv.get, cv.set });
                }
            }
            if (m_variables.count() == 0
                || variables.last().name != nullptr
                || variables.last().get != nullptr
                || variables.last().set != nullptr)
                m_variables.push(ClassVarStr());
        }
        String m_class_name = "";
        String m_static_name = "";
        String m_method_name = "";
        array<ClassMethod> m_statics;
        array<ClassMethod> m_methods;
        array<ClassVarStr> m_variables;
    };

public:
    Object() { }
    virtual ~Object() { }
    static Object* New(lua_State* l, int arg_nb)
    {
        UNUSED(l);
        UNUSED(arg_nb);
        ASSERT(false);
        return nullptr;
    }
    static const Library* GetLib() { ASSERT(false); return nullptr; }
};

//-----------------------------------------------------------------------------
// Class available to link C++ class to Lua methods
//--
class ObjectHelper
{
private:
    //-------------------------------------------------------------------------
    ObjectHelper() { }
    virtual ~ObjectHelper() { }

public:
    //-------------------------------------------------------------------------
    template <typename TLuaClass>
    static void Register(lua_State *l)
    {
        //Default statics
        static const luaL_Reg default_statics[]
        {
            { "New", New<TLuaClass> },
            { "Store", Store<TLuaClass> },
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
        const array<Object::Library::ClassVarStr>& variables = GetVariables<TLuaClass>();
        for (const Object::Library::ClassVarStr& var : variables)
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
    //-------------------------------------------------------------------------
    template <typename TLuaClass>
    static const Object::Library* GetLibrary()
    {
        const Object::Library* lib = TLuaClass::GetLib();
        ASSERT(lib);
        return lib;
    }

public:
    //-------------------------------------------------------------------------
    template <typename TLuaClass>
    static const char* GetObjectName() { return GetLibrary<TLuaClass>()->m_class_name.C(); }
    template <typename TLuaClass>
    static const char* GetStaticName() { return GetLibrary<TLuaClass>()->m_static_name.C(); }
    template <typename TLuaClass>
    static const char* GetMethodName() { return GetLibrary<TLuaClass>()->m_method_name.C(); }
    template <typename TLuaClass>
    static const ClassMethod* GetStaticMethods() { return GetLibrary<TLuaClass>()->m_statics.data(); }
    template <typename TLuaClass>
    static const ClassMethod* GetInstanceMethods() { return GetLibrary<TLuaClass>()->m_methods.data(); }
    template <typename TLuaClass>
    static const array<Object::Library::ClassVarStr>& GetVariables() { return GetLibrary<TLuaClass>()->m_variables; }

protected:
    //-------------------------------------------------------------------------
    template <typename TLuaClass>
    static int New(lua_State* l)
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
    template <typename TLuaClass> static int Store(lua_State * l);
    template <typename TLuaClass> static int Del(lua_State * l);
};

//-----------------------------------------------------------------------------
class Function
{
public:
    Function(lua_State* l, const char* name, int(*function)(lua_State*))
    {
        lua_pushcfunction(l, function);
        lua_setglobal(l, name);
    }
};

//-----------------------------------------------------------------------------
#if OLD_VAR_SYSTEM

//-----------------------------------------------------------------------------
template<typename T>
class VarPtr
{
protected:
    T* m_value = nullptr;
    bool m_optional = false;

public:
    VarPtr(bool optional = false)
    {
        m_optional = optional;
    }
    VarPtr(T* value, bool optional = false)
        : VarPtr(optional)
    {
        m_value = value;
    }
    VarPtr(lua_State* l, int& index, bool optional = false)
        : VarPtr(optional)
    {
        GetInc(l, index);
    }
    VarPtr(T* value, lua_State* l, int& index, bool optional = false)
        : VarPtr(value, optional)
    {
        GetInc(l, index);
    }

    inline T* operator ()()       { return m_value; }
    inline T* operator ->()       { return m_value; }
    inline T* operator=(T* value) { m_value = value; }
    inline T* GetValue()          { return m_value; }
    inline bool IsValid(lua_State* l, int index)
    {
        return InnerIsValid(l, index);
    }
    inline bool IsOptional()
    {
        return m_optional;
    }
private:
    inline void GetInc(lua_State* l, int& index)
    {
        bool is_nil = lua_isnil(l, index);
        if (!m_optional || (!is_nil && InnerIsValid(l, index)))
        {
            ASSERT(!is_nil);
            InnerGet(l, index);
        }
    }
public:
    inline void Get(lua_State* l, int index)
    {
        int idx = index;
        GetInc(l, idx);
    }
    inline int Return(lua_State* l)
    {
        InnerPush(l);
        return 1;
    }

protected:
    virtual bool InnerIsValid(lua_State* l, int index)
    {
        return !!lua_isuserdata(l, index);
    }
    virtual void InnerGet(lua_State* l, int& index)
    {
        T** obj = static_cast<T**>(luaL_checkudata(l, index++, ObjectHelper::GetMethodName<T>()));
        m_value = obj ? *obj : nullptr;
    }
    void InnerPush(lua_State* l)
    {
        T** data = (T**)lua_newuserdata(l, sizeof(T*));
        *data = m_value;
    }
};

//-----------------------------------------------------------------------------
/* TODO: FIX THAT TOUKY !!
template<typename T>
class VarPtrLight
{
public:
    VarPtrLight(bool optional = false) : VarPtr(optional) { }
    VarPtrLight(T* value, bool optional = false) : VarPtr(value, optional) { }
    VarPtrLight(lua_State* l, int& index, bool optional = false) : VarPtr(l, index, optional) { }
    VarPtrLight(T* value, lua_State* l, int& index, bool optional = false) : VarPtr(value, l, index, optional) { }
protected:
    virtual void InnerGet(lua_State* l, int& index)
    {
        T** obj = static_cast<T**>(luaL_testudata(l, index++, ObjectDef::GetMethodName<T>()));
        m_value = obj ? *obj : nullptr;
    }
};
*/

//-----------------------------------------------------------------------------
template<typename T>
class Var
{
protected:
    bool m_optional = false;
    T m_value;

public:
    Var(bool optional = false)
    {
        m_optional = optional;
        InnerInit();
    }
    Var(T value, bool optional = false)
    {
        m_optional = optional;
        m_value = value;
    }
    Var(lua_State* l, int& index, bool optional = false)
    {
        m_optional = optional;
        GetInc(l, index);
    }
    Var(T value, lua_State* l, int& index, bool optional = false)
    {
        m_optional = optional;
        m_value = value;
        GetInc(l, index);
    }
    inline operator T()     { return m_value; }
    inline T& operator ()() { return m_value; }
    inline T& GetValue()    { return m_value; }
    inline bool IsValid(lua_State* l, int index)
    {
        return InnerIsValid(l, index);
    }
    inline bool IsOptional()
    {
        return m_optional;
    }
private:
    void GetInc(lua_State* l, int& index)
    {
        bool is_nil = lua_isnil(l, index);
        if (!m_optional || (!is_nil && InnerIsValid(l, index)))
        {
            ASSERT(!is_nil);
            InnerGet(l, index);
        }
    }
public:
    inline void Get(lua_State* l, int index)
    {
        int idx = index;
        GetInc(l, idx);
    }
    inline int Return(lua_State* l)
    {
        return InnerPush(l);
    }
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
    inline bool operator==(const Var<T>& o)    { return m_value == o.m_value; }
    inline bool operator!=(const Var<T>& o)    { return m_value != o.m_value; }

protected:
    void InnerInit()                          { m_value = T(0); }
    bool InnerIsValid(lua_State* l, int index) { UNUSED(l); UNUSED(index); ASSERT(false); return false; }
    void InnerGet(lua_State* l, int& index)    { UNUSED(l); UNUSED(index); ASSERT(false); }
    int InnerPush(lua_State* l)                { UNUSED(l); ASSERT(false); return 0; }
};

//-----------------------------------------------------------------------------
#ifndef BASE_TYPES
template<> inline bool Var<bool>::InnerIsValid(lua_State* l, int index)
{
    return lua_isboolean(l, index);
}
template<> inline void Var<bool>::InnerGet(lua_State* l, int& index)
{
    m_value = !!lua_toboolean(l, index++);
}
template<> inline int Var<bool>::InnerPush(lua_State* l)
{
    lua_pushboolean(l, m_value);
    return 1;
}
//-----------------------------------------------------------------------------
template<> inline bool Var<char const*>::InnerIsValid(lua_State* l, int index)
{
    return !!lua_isstring(l, index);
}
template<> inline void Var<char const*>::InnerGet(lua_State* l, int& index)
{
    m_value = lua_tostring(l, index++);
}
template<> inline int Var<char const*>::InnerPush(lua_State* l)
{
    lua_pushstring(l, m_value);
    return 1;
}
//-----------------------------------------------------------------------------
template<> inline void Var<String>::InnerInit()
{
    m_value = String();
}
template<> inline bool Var<String>::InnerIsValid(lua_State* l, int index)
{
    Var<char const*> v;
    return v.IsValid(l, index);
}
template<> inline void Var<String>::InnerGet(lua_State* l, int& index)
{
    Var<char const*> v(l, index);
    m_value = v();
}
template<> inline int Var<String>::InnerPush(lua_State* l)
{
    Var<char const*> v;
    v = m_value.C();
    return v.Return(l);
}
//-----------------------------------------------------------------------------
template<> inline bool Var<double>::InnerIsValid(lua_State* l, int index)
{
    return !!lua_isnumber(l, index);
}
template<> inline void Var<double>::InnerGet(lua_State* l, int& index)
{
    m_value = lua_tonumber(l, index++);
}
template<> inline int Var<double>::InnerPush(lua_State* l)
{
    lua_pushnumber(l, m_value);
    return 1;
}
//-----------------------------------------------------------------------------
template<> inline bool Var<float>::InnerIsValid(lua_State* l, int index)
{
    Var<double> v;
    return v.IsValid(l, index);
}
template<> inline void Var<float>::InnerGet(lua_State* l, int& index)
{
    Var<double> v(l, index);
    m_value = (float)v();
}
template<> inline int Var<float>::InnerPush(lua_State* l)
{
    Var<double> v = (double)m_value;
    return v.Return(l);
}

#if 0
//-----------------------------------------------------------------------------
template<> inline bool Var<int64_t>::InnerIsValid(lua_State* l, int index)
{
    return !!lua_isnumber(l, index);
}
template<> inline void Var<int64_t>::InnerGet(lua_State* l, int& index)
{
    m_value = lua_tointeger(l, index++);
}
template<> inline int Var<int64_t>::InnerPush(lua_State* l)
{
    lua_pushinteger(l, m_value);
    return 1;
}
#endif

//-----------------------------------------------------------------------------
template<> inline bool Var<int32_t>::InnerIsValid(lua_State* l, int index)
{
    Var<int64_t> v;
    return v.IsValid(l, index);
}
template<> inline void Var<int32_t>::InnerGet(lua_State* l, int& index)
{
    Var<int64_t> v(l, index);
    m_value = (int32_t)v();
}
template<> inline int Var<int32_t>::InnerPush(lua_State* l)
{
    Var<int64_t> v = (int64_t)m_value;
    return v.Return(l);
}

//-----------------------------------------------------------------------------
template<> inline bool Var<uint32_t>::InnerIsValid(lua_State* l, int index)
{
    return !!lua_isnumber(l, index);
}
template<> inline void Var<uint32_t>::InnerGet(lua_State* l, int& index)
{
    m_value = (uint32_t)(lua_Unsigned)lua_tointeger(l, index++);
}
template<> inline int Var<uint32_t>::InnerPush(lua_State* l)
{
    lua_pushinteger(l, (lua_Integer)m_value);
    return 1;
}

#if 0
//-----------------------------------------------------------------------------
template<> inline bool Var<uint64_t>::InnerIsValid(lua_State* l, int index)
{
    Var<uint32_t> v;
    return v.IsValid(l, index);
}
template<> inline void Var<uint64_t>::InnerGet(lua_State* l, int& index)
{
    Var<uint32_t> v(l, index);
    m_value = (uint64_t)v();
}
template<> inline int Var<uint64_t>::InnerPush(lua_State* l)
{
    Var<uint32_t> v = (uint32_t)m_value;
    return v.Return(l);
}
#endif

//-----------------------------------------------------------------------------
template<> inline bool Var<vec2>::InnerIsValid(lua_State* l, int index)
{
    Var<float> x;
    return x.IsValid(l, index);
}
template<> inline void Var<vec2>::InnerGet(lua_State* l, int& index)
{
    Var<float> x(l, index);
    Var<float> y(x(), l, index, true);
    m_value = vec2(x, y);
}
template<> inline int Var<vec2>::InnerPush(lua_State* l)
{
    Var<float> x = m_value.x;
    Var<float> y = m_value.y;
    return (x.Return(l) + y.Return(l));
}
//-----------------------------------------------------------------------------
template<> inline bool Var<vec3>::InnerIsValid(lua_State* l, int index)
{
    Var<float> x;
    return x.IsValid(l, index);
}
template<> inline void Var<vec3>::InnerGet(lua_State* l, int& index)
{
    Var<float> x(l, index);
    Var<float> y(x(), l, index, true);
    Var<float> z(x(), l, index, true);
    m_value = vec3(x, y, z);
}
template<> inline int Var<vec3>::InnerPush(lua_State* l)
{
    Var<float> x = m_value.x;
    Var<float> y = m_value.y;
    Var<float> z = m_value.z;
    return (x.Return(l) + y.Return(l) + z.Return(l));
}
//-----------------------------------------------------------------------------
template<> inline bool Var<vec4>::InnerIsValid(lua_State* l, int index)
{
    Var<float> x;
    return x.IsValid(l, index);
}
template<> inline void Var<vec4>::InnerGet(lua_State* l, int& index)
{
    Var<float> x(l, index);
    Var<float> y(x(), l, index, true);
    Var<float> z(x(), l, index, true);
    Var<float> w(x(), l, index, true);
    m_value = vec4(x, y, z, w);
}
template<> inline int Var<vec4>::InnerPush(lua_State* l)
{
    Var<float> x = m_value.x;
    Var<float> y = m_value.y;
    Var<float> z = m_value.z;
    Var<float> w = m_value.w;
    return (x.Return(l) + y.Return(l) + z.Return(l) + w.Return(l));
}
#endif //BASE_TYPES

//-----------------------------------------------------------------------------
#ifndef CUSTOM_TYPES
class VarColor
{
protected:
    Var<vec4> m_value;

public:
    VarColor(bool optional = false)
    {
        m_value = Var<vec4>(optional);
        InnerInit();
    }
    VarColor(vec4 value, bool optional = false)
    {
        m_value = Var<vec4>(value, optional);
    }
    VarColor(lua_State* l, int& index, bool optional = false)
        : VarColor(optional)
    {
        GetInc(l, index);
    }
    VarColor(vec4 value, lua_State* l, int& index, bool optional = false)
        : VarColor(value, optional)
    {
        GetInc(l, index);
    }

    inline operator vec4()     { return m_value; }
    inline vec4& operator ()() { return m_value(); }
    inline vec4& GetValue()    { return m_value.GetValue(); }
    inline bool IsValid(lua_State* l, int index)
    {
        return InnerIsValid(l, index);
    }
    inline bool IsOptional()
    {
        return m_value.IsOptional();
    }
private:
    void GetInc(lua_State* l, int& index)
    {
        bool is_nil = lua_isnil(l, index);
        if (!m_value.IsOptional() || (!is_nil && InnerIsValid(l, index)))
        {
            ASSERT(!is_nil);
            InnerGet(l, index);
        }
    }
public:
    inline void Get(lua_State* l, int index)
    {
        int idx = index;
        GetInc(l, idx);
    }
    inline int Return(lua_State* l)
    {
        return InnerPush(l);
    }
    inline VarColor&  operator-(const vec4& value)  { m_value - value; return *this; }
    inline VarColor&  operator+(const vec4& value)  { m_value + value; return *this; }
    inline VarColor&  operator*(const vec4& value)  { m_value  * value; return *this; }
    inline VarColor&  operator/(const vec4& value)  { m_value / value; return *this; }
    inline VarColor&  operator=(const vec4& value)  { m_value = value; return *this; }
    inline VarColor& operator-=(const vec4& value)  { m_value -= value; return *this; }
    inline VarColor& operator+=(const vec4& value)  { m_value += value; return *this; }
    inline VarColor& operator*=(const vec4& value)  { m_value *= value; return *this; }
    inline VarColor& operator/=(const vec4& value)  { m_value /= value; return *this; }
    inline VarColor&  operator-(const Var<vec4>& o) { m_value - o; return *this; }
    inline VarColor&  operator+(const Var<vec4>& o) { m_value + o; return *this; }
    inline VarColor&  operator*(const Var<vec4>& o) { m_value  * o; return *this; }
    inline VarColor&  operator/(const Var<vec4>& o) { m_value / o; return *this; }
    inline VarColor&  operator=(const Var<vec4>& o) { m_value = o; return *this; }
    inline VarColor& operator-=(const Var<vec4>& o) { m_value -= o; return *this; }
    inline VarColor& operator+=(const Var<vec4>& o) { m_value += o; return *this; }
    inline VarColor& operator*=(const Var<vec4>& o) { m_value *= o; return *this; }
    inline VarColor& operator/=(const Var<vec4>& o) { m_value /= o; return *this; }
    inline bool operator==(const vec4& value)     { return m_value == value; }
    inline bool operator!=(const vec4& value)     { return m_value != value; }
    inline bool operator==(const Var<vec4>& o)    { return m_value == o; }
    inline bool operator!=(const Var<vec4>& o)    { return m_value != o; }
    inline VarColor&  operator-(const VarColor& v)  { m_value -  v.m_value; return *this; }
    inline VarColor&  operator+(const VarColor& v)  { m_value +  v.m_value; return *this; }
    inline VarColor&  operator*(const VarColor& v)  { m_value  * v.m_value; return *this; }
    inline VarColor&  operator/(const VarColor& v)  { m_value /  v.m_value; return *this; }
    inline VarColor&  operator=(const VarColor& v)  { m_value =  v.m_value; return *this; }
    inline VarColor& operator-=(const VarColor& v)  { m_value -= v.m_value; return *this; }
    inline VarColor& operator+=(const VarColor& v)  { m_value += v.m_value; return *this; }
    inline VarColor& operator*=(const VarColor& v)  { m_value *= v.m_value; return *this; }
    inline VarColor& operator/=(const VarColor& v)  { m_value /= v.m_value; return *this; }
    inline bool operator==(const VarColor& v)     { return m_value == v.m_value; }
    inline bool operator!=(const VarColor& v)     { return m_value != v.m_value; }

protected:
    void InnerInit()
    {
        m_value = vec4::zero;
    }
    bool InnerIsValid(lua_State* l, int index)
    {
        Var<String> s;
        return m_value.IsValid(l, index) || s.IsValid(l, index);
    }
    void InnerGet(lua_State* l, int& index)
    {
        //Try vec4 first
        if (m_value.IsValid(l, index))
        {
            m_value.Get(l, index);
        }
        else
        {
            Var<String> c(l, index);
            *this = Color::C8BppHexString(c);
        }
    }
    int InnerPush(lua_State* l)
    {
        Var<String> c = Color::HexString8Bpp(m_value);
        return c.Return(l);
    }
};

//-----------------------------------------------------------------------------
template<typename E>
class VarEnum
{
protected:
    SafeEnum<E> m_value;
    bool m_optional = false;

public:
    VarEnum(bool optional = false)
    {
        m_optional = optional;
        InnerInit();
    }
    VarEnum(SafeEnum<E> value, bool optional = false)
        : VarEnum(optional)
    {
        m_value = value;
    }
    VarEnum(lua_State* l, int& index, bool optional = false)
        : VarEnum(optional)
    {
        GetInc(l, index);
    }
    VarEnum(SafeEnum<E> value, lua_State* l, int& index, bool optional = false)
        : VarEnum(value, optional)
    {
        GetInc(l, index);
    }

    inline operator SafeEnum<E>()     { return m_value; }
    inline SafeEnum<E>& operator ()() { return m_value; }
    inline SafeEnum<E>& GetValue()    { return m_value; }
    inline bool IsValid(lua_State* l, int index)
    {
        return InnerIsValid(l, index);
    }
    inline bool IsOptional()
    {
        return m_optional;
    }
private:
    void GetInc(lua_State* l, int& index)
    {
        bool is_nil = lua_isnil(l, index);
        if (!m_optional || (!is_nil && InnerIsValid(l, index)))
        {
            ASSERT(!is_nil);
            InnerGet(l, index);
        }
    }
public:
    inline void Get(lua_State* l, int index)
    {
        int idx = index;
        GetInc(l, idx);
    }
    inline int Return(lua_State* l)
    {
        return InnerPush(l);
    }
    inline VarEnum<E>&  operator-(const SafeEnum<E>& value)  { m_value - value; return *this; }
    inline VarEnum<E>&  operator+(const SafeEnum<E>& value)  { m_value + value; return *this; }
    inline VarEnum<E>&  operator*(const SafeEnum<E>& value)  { m_value  * value; return *this; }
    inline VarEnum<E>&  operator/(const SafeEnum<E>& value)  { m_value / value; return *this; }
    inline VarEnum<E>&  operator=(const SafeEnum<E>& value)  { m_value = value; return *this; }
    inline VarEnum<E>& operator-=(const SafeEnum<E>& value)  { m_value -= value; return *this; }
    inline VarEnum<E>& operator+=(const SafeEnum<E>& value)  { m_value += value; return *this; }
    inline VarEnum<E>& operator*=(const SafeEnum<E>& value)  { m_value *= value; return *this; }
    inline VarEnum<E>& operator/=(const SafeEnum<E>& value)  { m_value /= value; return *this; }
    inline bool operator==(const SafeEnum<E>& value)     { return m_value == value; }
    inline bool operator!=(const SafeEnum<E>& value)     { return m_value != value; }
    inline VarEnum<E>&  operator-(const VarEnum<E>& v)  { m_value - v.m_value; return *this; }
    inline VarEnum<E>&  operator+(const VarEnum<E>& v)  { m_value + v.m_value; return *this; }
    inline VarEnum<E>&  operator*(const VarEnum<E>& v)  { m_value  * v.m_value; return *this; }
    inline VarEnum<E>&  operator/(const VarEnum<E>& v)  { m_value / v.m_value; return *this; }
    inline VarEnum<E>&  operator=(const VarEnum<E>& v)  { m_value = v.m_value; return *this; }
    inline VarEnum<E>& operator-=(const VarEnum<E>& v)  { m_value -= v.m_value; return *this; }
    inline VarEnum<E>& operator+=(const VarEnum<E>& v)  { m_value += v.m_value; return *this; }
    inline VarEnum<E>& operator*=(const VarEnum<E>& v)  { m_value *= v.m_value; return *this; }
    inline VarEnum<E>& operator/=(const VarEnum<E>& v)  { m_value /= v.m_value; return *this; }
    inline bool operator==(const VarEnum<E>& v)     { return m_value == v.m_value; }
    inline bool operator!=(const VarEnum<E>& v)     { return m_value != v.m_value; }

protected:
    void InnerInit()
    {
        m_value = SafeEnum<E>(0);
    }
    bool InnerIsValid(lua_State* l, int index)
    {
        Var<String> s;
        return s.IsValid(l, index);
    }
    void InnerGet(lua_State* l, int& index)
    {
        Var<String> c(l, index);
        *this = FindValue<SafeEnum<E> >(c);
    }
    int InnerPush(lua_State* l)
    {
        Var<String> s = this->GetValue().ToString();
        return s.Return(l);
    }
};
#endif //CUSTOM_TYPES

#endif //OLD_VAR_SYSTEM

//-----------------------------------------------------------------------------
// Stack: Main class that encapsulates everything -----------------------------
//-----------------------------------------------------------------------------
class Stack
{
public:

#if NEW_VAR_SYSTEM

    //-------------------------------------------------------------------------
    static Stack Begin(lua_State* state, int32_t start_index = 1)
    {
        return Stack(state, start_index);
    }

    //-------------------------------------------------------------------------
    int32_t End()
    {
        return m_result;
    }

#if !OLD_VAR_SYSTEM
protected:
#endif //!OLD_VAR_SYSTEM

#endif //NEW_VAR_SYSTEM
    
    Stack(lua_State* l, int32_t start_index = 1)
    {
        m_state = l;
        m_index = start_index;
    }
    virtual ~Stack() { }

    int32_t GetArgs()
    {
        return lua_gettop(m_state);
    }

#if NEW_VAR_SYSTEM

public:
    //-------------------------------------------------------------------------
    //The encapsulating struct for pointers
    template<typename T>
    struct Ptr
    {
    public:
        T* m_value = nullptr;
        bool m_throw_error = false;

        //---------------------------------------------------------------------
        //m_value: Your stored value
        //throw_error: If true, lua will throw an error if the value get fails
        Ptr(T* value, bool throw_error = false)
        {
            m_value = value;
            m_throw_error = throw_error;
        }
        Ptr(const T*& value) { m_value = value; }
        inline operator T*() { return m_value; }
        inline Ptr<T>& operator=(T const*& value) { m_value = value; return *this; }
    };

	//-------------------------------------------------------------------------
    template<typename T> T Get(bool isOptional = false) { return Get(InnerDefault<T>(), isOptional); }
    template<typename E> SafeEnum<E> GetEnum(bool isOptional = false) { return GetEnum(InnerDefaultSafeEnum<E>(), isOptional); }
    template<typename P> Ptr<P> GetPtr(bool isOptional = false) { return GetPtr(InnerDefaultPtr<P>(), isOptional); }

    //-------------------------------------------------------------------------
#define DECLARE_STACK_GET(T0, T1, GET_NAME, INNER_IS_VALID, INNER_GET) \
    template<typename T0> \
    T1 GET_NAME(T1 value, bool isOptional = false) \
    { \
        bool is_nil = lua_isnil(m_state, m_index); \
        if (!isOptional || (!is_nil && INNER_IS_VALID<T0>())) \
        { \
            ASSERT(!is_nil); /* touky: should assert, though ? */ \
            return INNER_GET<T0>(value); \
        } \
        return value; \
    }

    DECLARE_STACK_GET(T, T, Get, InnerIsValid, InnerGet);
    DECLARE_STACK_GET(E, SafeEnum<E>, GetEnum, InnerIsValidSafeEnum, InnerGetSafeEnum);
    DECLARE_STACK_GET(P, Ptr<P>, GetPtr, InnerIsValidPtr, InnerGetPtr);

#undef DECLARE_STACK_GET

    //-------------------------------------------------------------------------
    template<typename T> Stack& operator<<(T& value) { m_result += InnerPush<T>(value); return *this; }
    template<typename E> Stack& operator<<(SafeEnum<E>& value) { m_result += InnerPushSafeEnum<E>(value); return *this; }
    template<typename P> Stack& operator<<(Ptr<P>& value) { m_result += InnerPushPtr<P>(value); return *this; }

protected:
    #define INNER_ERROR "Your type is not implemented. For pointers, use LuaPtr<MyType>()"
    template<typename T> T InnerDefault() { return T(0); }
	template<typename T> bool InnerIsValid() { ASSERT(false, INNER_ERROR); return false; }
    template<typename T> T InnerGet(T value) { UNUSED(value); ASSERT(false, INNER_ERROR); return InnerDefault<T>(); }
    template<typename T> int InnerPush(T value) { UNUSED(value); ASSERT(false, INNER_ERROR); return 0; }

#ifndef INNER_T_T
    //template<typename T, template <typename> class C> C<T> InnerDefault() { return C<E>(); }
    //template<typename E> bool InnerIsValid() { return InnerIsValid<String>(); }
    //template<typename E> SafeEnum<E> InnerGet(SafeEnum<E> value) { return FindValue<SafeEnum<E> >(InnerGet<char const*>(value.ToString().C())); }
    //template<typename E> int InnerPush(SafeEnum<E> value) { return InnerPush<char const*>(value.ToString.C()); }
#endif //STACK_STRING

#ifndef INNER_SAFE_ENUM
    template<typename E> SafeEnum<E> InnerDefaultSafeEnum() { return SafeEnum<E>(); }
    template<typename E> bool InnerIsValidSafeEnum() { return InnerIsValid<String>(); }
    template<typename E> SafeEnum<E> InnerGetSafeEnum(SafeEnum<E> value) { return FindValue<SafeEnum<E> >(InnerGet<String>(value.ToString())); }
    template<typename E> int InnerPushSafeEnum(SafeEnum<E> value) { return InnerPush<String>(value.ToString()); }
#endif //STACK_STRING

#ifndef INNER_PTR
    template<typename P> inline Ptr<P> InnerDefaultPtr() { return Ptr<P>(nullptr); }
    template<typename P> inline bool InnerIsValidPtr() { return !!lua_isuserdata(m_state, m_index); }
    template<typename P> inline Ptr<P> InnerGetPtr(Ptr<P> value)
    {
        P** obj = static_cast<P**>(value.m_throw_error
                                        ? luaL_checkudata(m_state, m_index++, ObjectHelper::GetMethodName<P>())
                                        : luaL_testudata(m_state, m_index++, ObjectHelper::GetMethodName<P>()) );
        return Ptr<P>(obj ? *obj : value.m_value);
    }
    template<typename P> inline int InnerPushPtr(Ptr<P> value)
    {
        P** data = (P**)lua_newuserdata(m_state, sizeof(P*));
        *data = value.m_value;
    }
#endif //STACK_STRING

#endif //NEW_VAR_SYSTEM

	//-------------------------------------------------------------------------
#if OLD_VAR_SYSTEM
public:
    inline operator int32_t() { return m_result; }
    //-------------------------------------------------------------------------

    template<typename T>
    Stack& operator>>(T& var)
    {
        var = T(var.GetValue(), m_state, m_index, var.IsOptional());
        return *this;
    }

#if !NEW_VAR_SYSTEM

    //-------------------------------------------------------------------------
    template<typename T>
    Stack& operator<<(T& var)
    {
        m_result += var.Return(m_state);
        return *this;
    }

#endif //NEW_VAR_SYSTEM

#endif //OLD_VAR_SYSTEM

private:
    lua_State*  m_state = nullptr;
    int32_t m_index = 1;
    int32_t m_result = 0;
};

//-----------------------------------------------------------------------------
#if NEW_VAR_SYSTEM

#ifndef REGION_STACK_VAR

//#if OLD_VAR_SYSTEM && NEW_VAR_SYSTEM
//
////-------------------------------------------------------------------------
//template<typename T, typename V>
//Stack& Stack::operator<<(Var<T>& var)
//{
//    m_result += var.Return(m_state);
//    return *this;
//}
//
//#endif //OLD_VAR_SYSTEM && NEW_VAR_SYSTEM

#ifndef STACK_BOOL
template<> inline bool Stack::InnerIsValid<bool>()       { return lua_isboolean(m_state, m_index); }
template<> inline bool Stack::InnerGet<bool>(bool value) { UNUSED(value); return !!lua_toboolean(m_state, m_index++); }
template<> inline int Stack::InnerPush<bool>(bool value) { lua_pushboolean(m_state, value); return 1; }
#endif // STACK_BOOL

//-----------------------------------------------------------------------------
#ifndef STACK_CHAR_CONST
template<> inline bool Stack::InnerIsValid<char const*>()                     { return !!lua_isstring(m_state, m_index); }
template<> inline char const* Stack::InnerGet<char const*>(char const* value) { UNUSED(value); return lua_tostring(m_state, m_index++); }
template<> inline int Stack::InnerPush<char const*>(char const* value)        { lua_pushstring(m_state, value); return 1; }
#endif // STACK_CHAR_CONST

//-----------------------------------------------------------------------------
#ifndef STACK_STRING
template<> inline String Stack::InnerDefault<String>()         { return String(); }
template<> inline bool Stack::InnerIsValid<String>()           { return InnerIsValid<char const*>(); }
template<> inline String Stack::InnerGet<String>(String value) { return String(InnerGet<char const*>(value.C())); }
template<> inline int Stack::InnerPush<String>(String value)   { return InnerPush<char const*>(value.C()); }
#endif //STACK_STRING

//-----------------------------------------------------------------------------
#ifndef STACK_STRING
template<> inline bool Stack::InnerIsValid<double>()           { return !!lua_isnumber(m_state, m_index); }
template<> inline double Stack::InnerGet<double>(double value) { UNUSED(value); return lua_tonumber(m_state, m_index++); }
template<> inline int Stack::InnerPush<double>(double value)   { lua_pushnumber(m_state, value); return 1; }
#endif //STACK_STRING

//-----------------------------------------------------------------------------
#ifndef STACK_FLOAT
template<> inline bool Stack::InnerIsValid<float>()         { return InnerIsValid<double>(); }
template<> inline float Stack::InnerGet<float>(float value) { return (float)InnerGet<double>((double)value); }
template<> inline int Stack::InnerPush<float>(float value)  { return InnerPush<double>((double)value); }
#endif //STACK_FLOAT

//-----------------------------------------------------------------------------
//DO NOT REMOVE: IT IS THE BASIS FOR INT 32
#ifndef STACK_INT64
template<> inline bool Stack::InnerIsValid<int64_t>()             { return !!lua_isnumber(m_state, m_index); }
template<> inline int64_t Stack::InnerGet<int64_t>(int64_t value) { UNUSED(value); return lua_tointeger(m_state, m_index++); }
template<> inline int Stack::InnerPush<int64_t>(int64_t value)    { lua_pushinteger(m_state, value); return 1; }
#endif //STACK_INT64

//-----------------------------------------------------------------------------
#ifndef STACK_UINT64
template<> inline bool Stack::InnerIsValid<uint64_t>()               { return !!lua_isnumber(m_state, m_index); }
template<> inline uint64_t Stack::InnerGet<uint64_t>(uint64_t value) { UNUSED(value); return (uint64_t)lua_tointeger(m_state, m_index++); }
template<> inline int Stack::InnerPush<uint64_t>(uint64_t value)     { lua_pushinteger(m_state, (lua_Unsigned)value); return 1; }
#endif //STACK_UINT64

//-----------------------------------------------------------------------------
#ifndef STACK_INT32
template<> inline bool Stack::InnerIsValid<int32_t>()             { return !!lua_isnumber(m_state, m_index); }
template<> inline int32_t Stack::InnerGet<int32_t>(int32_t value) { UNUSED(value); return (int32_t)lua_tointeger(m_state, m_index++); }
template<> inline int Stack::InnerPush<int32_t>(int32_t value)    { lua_pushinteger(m_state, (lua_Integer)value); return 1; }
#endif STACK_INT32

//-----------------------------------------------------------------------------
#ifndef STACK_UINT32
template<> inline bool Stack::InnerIsValid<uint32_t>()               { return !!lua_isnumber(m_state, m_index); }
template<> inline uint32_t Stack::InnerGet<uint32_t>(uint32_t value) { UNUSED(value); return (uint32_t)(lua_Unsigned)lua_tointeger(m_state, m_index++); }
template<> inline int Stack::InnerPush<uint32_t>(uint32_t value)     { lua_pushinteger(m_state, (lua_Unsigned)value); return 1; }
#endif //STACK_UINT32

//-----------------------------------------------------------------------------
#ifndef STACK_VEC2
template<> inline bool Stack::InnerIsValid<vec2>()       { return InnerIsValid<float>(); }
template<> inline vec2 Stack::InnerGet<vec2>(vec2 value) { return vec2(InnerGet<float>(value.x), Get<float>(value.y, true)); }
template<> inline int Stack::InnerPush<vec2>(vec2 value) { return (InnerPush<float>(value.x) + InnerPush<float>(value.y)); }
#endif //STACK_VEC2

//-----------------------------------------------------------------------------
#ifndef STACK_VEC3
template<> inline bool Stack::InnerIsValid<vec3>()       { return InnerIsValid<float>(); }
template<> inline vec3 Stack::InnerGet<vec3>(vec3 value) { return vec3(InnerGet<float>(value.x), Get<float>(value.y, true), Get<float>(value.z, true)); }
template<> inline int Stack::InnerPush<vec3>(vec3 value) { return (InnerPush<float>(value.x) + InnerPush<float>(value.y) + InnerPush<float>(value.z)); }
#endif //STACK_VEC3

//-----------------------------------------------------------------------------
#ifndef STACK_VEC4
template<> inline bool Stack::InnerIsValid<vec4>()       { return InnerIsValid<float>(); }
template<> inline vec4 Stack::InnerGet<vec4>(vec4 value) { return vec4(InnerGet<float>(value.x), Get<float>(value.y, true), Get<float>(value.z, true), Get<float>(value.w, true)); }
template<> inline int Stack::InnerPush<vec4>(vec4 value) { return (InnerPush<float>(value.x) + InnerPush<float>(value.y) + InnerPush<float>(value.z) + InnerPush<float>(value.w)); }
#endif STACK_VEC4

//-----------------------------------------------------------------------------
#ifndef STACK_SAFE_ENUM
//template<typename E> inline SafeEnum<E> Stack::InnerDefault<SafeEnum, E>()              { return SafeEnum<E>(); }
//template<typename E> inline bool Stack::InnerIsValid<SafeEnum<E> >()                     { return InnerIsValid<String>(); }
//template<typename E> inline SafeEnum<E> Stack::InnerGet<SafeEnum<E> >(SafeEnum<E> value) { return FindValue<SafeEnum<E> >(InnerGet<char const*>(value.ToString().C())); }
//template<typename E> inline int Stack::InnerPush<SafeEnum<E> >(SafeEnum<E> value)        { return InnerPush<char const*>(value.ToString.C()); }
#endif //STACK_STRING

//-----------------------------------------------------------------------------
#ifndef STACK_PTR
//template<typename P> inline Stack::Ptr<P> Stack::InnerDefault<Stack::Ptr<P> >() { return Stack::Ptr<P>(nullptr); }
//template<typename P> inline bool Stack::InnerIsValid<Stack::Ptr<P> >() { return !!lua_isuserdata(m_state, m_index); }
//template<typename P> inline Stack::Ptr<P> Stack::InnerGet<Stack::Ptr<P> >(Stack::Ptr<P> value)
//{
//    P** obj = static_cast<P**>(value.m_throw_error
//                                    ? luaL_checkudata(m_state, m_index++, ObjectHelper::GetMethodName<P>())
//                                    : luaL_testudata(m_state, m_index++, ObjectHelper::GetMethodName<P>()) );
//    return Stack::Ptr<P>(obj ? *obj : value.m_value);
//}
//template<typename P> inline int Stack::InnerPush<Stack::Ptr<P> >(Stack::Ptr<P> value)
//{
//    P** data = (P**)lua_newuserdata(m_state, sizeof(P*));
//    *data = value.m_value;
//}
#endif //STACK_STRING

#endif //REGION_STACK_VAR

#endif //NEW_VAR_SYSTEM

//-----------------------------------------------------------------------------
class Loader
{
    friend class ObjectHelper;
public:
    Loader();
    virtual ~Loader();

    bool ExecLuaFile(String const &lua);
    bool ExecLuaCode(String const &lua);

#if NEW_VAR_SYSTEM
    //-------------------------------------------------------------------------
#define DECLARE_LOADER_GET(T0, T1, GET_NAME) \
    template<typename T0> \
    T1 GET_NAME(String const &name) \
    { \
        lua_getglobal(m_lua_state, name.C()); \
        auto stack = LuaStack::Begin(m_lua_state, -1); \
        auto result = stack.GET_NAME<T0>(); \
        lua_pop(m_lua_state, 1); \
        return result; \
    }

    DECLARE_LOADER_GET(T, T, Get);
    DECLARE_LOADER_GET(E, SafeEnum<E>, GetEnum);
    DECLARE_LOADER_GET(P, P*, GetPtr);

#undef DECLARE_LOADER_GET

#else //OLD_VAR_SYSTEM

    template<typename T>
    T GetVar(String const &name)
    {
        lua_getglobal(m_lua_state, name.C());
        Var<T> var; var.Get(m_lua_state, -1);
        lua_pop(m_lua_state, 1);
        return var;
    }
    template<typename T>
    T* GetPtr(String const &name)
    {
        lua_getglobal(m_lua_state, name.C());
        VarPtr<T> var; var.Get(m_lua_state, -1);
        lua_pop(m_lua_state, 1);
        return var();
    }

#endif //OLD_VAR_SYSTEM

protected:
    lua_State* GetLuaState();
    static void Store(lua_State* l, Loader* loader);
    static void Release(lua_State* l, Loader* loader);
    static void StoreObject(lua_State* l, Object* obj);
    //Virtual Store lua object ------------------------------------------------
    virtual void Store(Object* obj) { UNUSED(obj); }

private:
    lua_State* m_lua_state;
};

//-----------------------------------------------------------------------------
// ObjectHelper member implementations that require VarPtr

#if NEW_VAR_SYSTEM
template <typename TLuaClass>
int ObjectHelper::Store(lua_State * l)
{
    auto stack = LuaStack::Begin(l);
    auto obj = stack.Get<Stack::Ptr<TLuaClass> >();
    ASSERT(obj);
    Loader::StoreObject(l, obj);
    return 0;
}

template <typename TLuaClass>
int ObjectHelper::Del(lua_State * l)
{
    auto stack = LuaStack::Begin(l);
    auto obj = stack.Get<Stack::Ptr<TLuaClass> >();
    ASSERT(obj);
    delete obj;
    return 0;
}

#else //OLD_VAR_SYSTEM

template <typename TLuaClass>
int ObjectHelper::Store(lua_State * l)
{
    VarPtr<TLuaClass> obj;
    obj.Get(l, 1);
    ASSERT(obj());
    Loader::StoreObject(l, obj());
    return 0;
}

template <typename TLuaClass>
int ObjectHelper::Del(lua_State * l)
{
    VarPtr<TLuaClass> obj;
    obj.Get(l, 1);
    ASSERT(obj());
    delete obj();
    return 0;
}

#endif //OLD_VAR_SYSTEM


} /* namespace Lolua */

//TYPEDEFS
typedef Lolua::Function         LuaFunction;
typedef Lolua::ObjectHelper     LuaObjectHelper;
typedef Lolua::Object           LuaObject;
typedef Lolua::Object::Library  LuaObjectLibrary;
typedef Lolua::Loader           LuaLoader;
typedef Lolua::Stack            LuaStack;
template <typename P> using LuaPtr = Lolua::Stack::Ptr<P>;
//template <typename P>
//typedef Lolua::Stack::Ptr<P>    LuaPtr<P>;
#if OLD_VAR_SYSTEM
typedef Lolua::Var<bool>        LuaBool;
typedef Lolua::Var<char const*> LuaCharPtr;
typedef Lolua::Var<String>      LuaString;
typedef Lolua::Var<double>      LuaDouble;
typedef Lolua::Var<float>       LuaFloat;
typedef Lolua::Var<int64_t>     LuaInt64;
typedef Lolua::Var<int32_t>     LuaInt32;
typedef Lolua::Var<uint32_t>    LuaUInt32;
typedef Lolua::Var<uint64_t>    LuaUInt64;
typedef Lolua::Var<vec2>        LuaVec2;
typedef Lolua::Var<vec3>        LuaVec3;
typedef Lolua::Var<vec4>        LuaVec4;
typedef Lolua::VarColor         LuaColor;
#endif //OLD_VAR_SYSTEM

} /* namespace lol */
