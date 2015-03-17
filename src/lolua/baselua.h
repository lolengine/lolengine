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

typedef lua_State LuaState;

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
        array<ClassMethod> const& statics,
        array<ClassMethod> const& methods,
        array<ClassVar> const& variables)
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

        for (ClassVar const& cv : variables)
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
    static ObjectDef* New(LuaState* l, int arg_nb)
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
    static void Register(LuaState *l)
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
    static int New(LuaState* l)
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
    static int Del(LuaState * l)
    {
        VarPtr<TLuaClass> obj; obj.Get(l, 1);
        ASSERT(obj());
        delete obj();
        return 0;
    }
};

//-----------------------------------------------------------------------------
class Function
{
public:
    Function(LuaState* l, const char* name, int(*function)(LuaState*))
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
    VarPtr(LuaState* l, int& index, bool optional = false)
        : VarPtr(optional)
    {
        GetInc(l, index);
    }
    VarPtr(T* value, LuaState* l, int& index, bool optional = false)
        : VarPtr(value, optional)
    {
        GetInc(l, index);
    }
    inline T* operator ()()        { return m_value; }
    inline T* operator ->()        { return m_value; }
    inline T* operator=(T* value)  { m_value = value; }
    inline bool IsValid(LuaState* l, int index)
    {
        return InnerValid(l, index);
    }
private:
    inline void GetInc(LuaState* l, int& index)
    {
        bool is_nil = lua_isnil(l, index);
        if (!m_optional || (!is_nil && InnerIsValid(l, index)))
        {
            ASSERT(!is_nil);
            InnerGet(l, index);
        }
    }
public:
    inline void Get(LuaState* l, int index)
    {
        int idx = index;
        GetInc(l, idx);
    }
    inline int Return(LuaState* l)
    {
        InnerPush(l);
        return 1;
    }

protected:
    virtual bool InnerIsValid(LuaState* l, int index)
    {
        return !!lua_isuserdata(l, index);
    }
    virtual void InnerGet(LuaState* l, int& index)
    {
        T** obj = static_cast<T**>(luaL_checkudata(l, index++, Object::GetMethodName<T>()));
        m_value = obj ? *obj : nullptr;
    }
    void InnerPush(LuaState* l)
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
    VarPtrLight(LuaState* l, int& index, bool optional = false) : VarPtr(l, index, optional) { }
    VarPtrLight(T* value, LuaState* l, int& index, bool optional = false) : VarPtr(value, l, index, optional) { }
protected:
    virtual void InnerGet(LuaState* l, int& index)
    {
        T** obj = static_cast<T**>(luaL_testudata(l, index++, Object::GetMethodName<T>()));
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
    Var(LuaState* l, int& index, bool optional = false)
    {
        m_optional = optional;
        GetInc(l, index);
    }
    Var(T value, LuaState* l, int& index, bool optional = false)
    {
        m_optional = optional;
        m_value = value;
        GetInc(l, index);
    }
    inline operator T()     { return m_value; }
    inline T& operator ()() { return m_value; }
    inline T& GetValue()    { return m_value; }
    inline bool IsValid(LuaState* l, int index)
    {
        return InnerIsValid(l, index);
    }
    inline bool IsOptional()
    {
        return m_optional;
    }
private:
    void GetInc(LuaState* l, int& index)
    {
        bool is_nil = lua_isnil(l, index);
        if (!m_optional || (!is_nil && InnerIsValid(l, index)))
        {
            ASSERT(!is_nil);
            InnerGet(l, index);
        }
    }
public:
    inline void Get(LuaState* l, int index)
    {
        int idx = index;
        GetInc(l, idx);
    }
    inline int Return(LuaState* l)
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
    bool InnerIsValid(LuaState* l, int index) { UNUSED(l); UNUSED(index); ASSERT(false); return false; }
    void InnerGet(LuaState* l, int& index)    { UNUSED(l); UNUSED(index); ASSERT(false); }
    int InnerPush(LuaState* l)                { UNUSED(l); ASSERT(false); return 0; }
};

//-----------------------------------------------------------------------------
template<> inline bool Var<bool>::InnerIsValid(LuaState* l, int index)
{
    return lua_isboolean(l, index);
}
template<> inline void Var<bool>::InnerGet(LuaState* l, int& index)
{
    m_value = !!lua_toboolean(l, index++);
}
template<> inline int Var<bool>::InnerPush(LuaState* l)
{
    lua_pushboolean(l, m_value);
    return 1;
}
//-----------------------------------------------------------------------------
template<> inline bool Var<char const*>::InnerIsValid(LuaState* l, int index)
{
    return !!lua_isstring(l, index);
}
template<> inline void Var<char const*>::InnerGet(LuaState* l, int& index)
{
    m_value = lua_tostring(l, index++);
}
template<> inline int Var<char const*>::InnerPush(LuaState* l)
{
    lua_pushstring(l, m_value);
    return 1;
}
//-----------------------------------------------------------------------------
template<> inline void Var<String>::InnerInit()
{
    m_value = String();
}
template<> inline bool Var<String>::InnerIsValid(LuaState* l, int index)
{
    Var<char const*> v;
    return v.IsValid(l, index);
}
template<> inline void Var<String>::InnerGet(LuaState* l, int& index)
{
    Var<char const*> v(l, index);
    m_value = v();
}
template<> inline int Var<String>::InnerPush(LuaState* l)
{
    Var<char const*> v;
    v = m_value.C();
    return v.Return(l);
}
//-----------------------------------------------------------------------------
template<> inline bool Var<double>::InnerIsValid(LuaState* l, int index)
{
    return !!lua_isnumber(l, index);
}
template<> inline void Var<double>::InnerGet(LuaState* l, int& index)
{
    m_value = lua_tonumber(l, index++);
}
template<> inline int Var<double>::InnerPush(LuaState* l)
{
    lua_pushnumber(l, m_value);
    return 1;
}
//-----------------------------------------------------------------------------
template<> inline bool Var<float>::InnerIsValid(LuaState* l, int index)
{
    Var<double> v;
    return v.IsValid(l, index);
}
template<> inline void Var<float>::InnerGet(LuaState* l, int& index)
{
    Var<double> v(l, index);
    m_value = (float)v();
}
template<> inline int Var<float>::InnerPush(LuaState* l)
{
    Var<double> v = (double)m_value;
    return v.Return(l);
}
//-----------------------------------------------------------------------------
template<> inline bool Var<int64_t>::InnerIsValid(LuaState* l, int index)
{
    return !!lua_isnumber(l, index);
}
template<> inline void Var<int64_t>::InnerGet(LuaState* l, int& index)
{
    m_value = lua_tointeger(l, index++);
}
template<> inline int Var<int64_t>::InnerPush(LuaState* l)
{
    lua_pushinteger(l, m_value);
    return 1;
}
//-----------------------------------------------------------------------------
template<> inline bool Var<int32_t>::InnerIsValid(LuaState* l, int index)
{
    Var<int64_t> v;
    return v.IsValid(l, index);
}
template<> inline void Var<int32_t>::InnerGet(LuaState* l, int& index)
{
    Var<int64_t> v(l, index);
    m_value = (int32_t)v();
}
template<> inline int Var<int32_t>::InnerPush(LuaState* l)
{
    Var<int64_t> v = (int64_t)m_value;
    return v.Return(l);
}
//-----------------------------------------------------------------------------
template<> inline bool Var<uint32_t>::InnerIsValid(LuaState* l, int index)
{
    return !!lua_isnumber(l, index);
}
template<> inline void Var<uint32_t>::InnerGet(LuaState* l, int& index)
{
    m_value = lua_tounsigned(l, index++);
}
template<> inline int Var<uint32_t>::InnerPush(LuaState* l)
{
    lua_pushunsigned(l, m_value);
    return 1;
}
//-----------------------------------------------------------------------------
template<> inline bool Var<uint64_t>::InnerIsValid(LuaState* l, int index)
{
    Var<uint32_t> v;
    return v.IsValid(l, index);
}
template<> inline void Var<uint64_t>::InnerGet(LuaState* l, int& index)
{
    Var<uint32_t> v(l, index);
    m_value = (uint64_t)v();
}
template<> inline int Var<uint64_t>::InnerPush(LuaState* l)
{
    Var<uint32_t> v = (uint32_t)m_value;
    return v.Return(l);
}
//-----------------------------------------------------------------------------
template<> inline bool Var<vec2>::InnerIsValid(LuaState* l, int index)
{
    Var<float> x;
    return x.IsValid(l, index);
}
template<> inline void Var<vec2>::InnerGet(LuaState* l, int& index)
{
    Var<float> x(l, index);
    Var<float> y(x(), l, index, true);
    m_value = vec2(x, y);
}
template<> inline int Var<vec2>::InnerPush(LuaState* l)
{
    Var<float> x = m_value.x;
    Var<float> y = m_value.y;
    return (x.Return(l) + y.Return(l));
}
//-----------------------------------------------------------------------------
template<> inline bool Var<vec3>::InnerIsValid(LuaState* l, int index)
{
    Var<float> x;
    return x.IsValid(l, index);
}
template<> inline void Var<vec3>::InnerGet(LuaState* l, int& index)
{
    Var<float> x(l, index);
    Var<float> y(x(), l, index, true);
    Var<float> z(x(), l, index, true);
    m_value = vec3(x, y, z);
}
template<> inline int Var<vec3>::InnerPush(LuaState* l)
{
    Var<float> x = m_value.x;
    Var<float> y = m_value.y;
    Var<float> z = m_value.z;
    return (x.Return(l) + y.Return(l) + z.Return(l));
}
//-----------------------------------------------------------------------------
template<> inline bool Var<vec4>::InnerIsValid(LuaState* l, int index)
{
    Var<float> x;
    return x.IsValid(l, index);
}
template<> inline void Var<vec4>::InnerGet(LuaState* l, int& index)
{
    Var<float> x(l, index);
    Var<float> y(x(), l, index, true);
    Var<float> z(x(), l, index, true);
    Var<float> w(x(), l, index, true);
    m_value = vec4(x, y, z, w);
}
template<> inline int Var<vec4>::InnerPush(LuaState* l)
{
    Var<float> x = m_value.x;
    Var<float> y = m_value.y;
    Var<float> z = m_value.z;
    Var<float> w = m_value.w;
    return (x.Return(l) + y.Return(l) + z.Return(l) + w.Return(l));
}

//-----------------------------------------------------------------------------
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
    VarColor(LuaState* l, int& index, bool optional = false)
        : VarColor(optional)
    {
        GetInc(l, index);
    }
    VarColor(vec4 value, LuaState* l, int& index, bool optional = false)
        : VarColor(value, optional)
    {
        GetInc(l, index);
    }

    inline operator vec4()     { return m_value; }
    inline vec4& operator ()() { return m_value(); }
    inline vec4& GetValue()    { return m_value.GetValue(); }
    inline bool IsValid(LuaState* l, int index)
    {
        return InnerIsValid(l, index);
    }
private:
    void GetInc(LuaState* l, int& index)
    {
        bool is_nil = lua_isnil(l, index);
        if (!m_value.IsOptional() || (!is_nil && InnerIsValid(l, index)))
        {
            ASSERT(!is_nil);
            InnerGet(l, index);
        }
    }
public:
    inline void Get(LuaState* l, int index)
    {
        int idx = index;
        GetInc(l, idx);
    }
    inline int Return(LuaState* l)
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
    bool InnerIsValid(LuaState* l, int index)
    {
        Var<String> s;
        return m_value.IsValid(l, index) || s.IsValid(l, index);
    }
    void InnerGet(LuaState* l, int& index)
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
    int InnerPush(LuaState* l)
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
    VarEnum(LuaState* l, int& index, bool optional = false)
        : VarEnum(optional)
    {
        GetInc(l, index);
    }
    VarEnum(vec4 value, LuaState* l, int& index, bool optional = false)
        : VarEnum(value, optional)
    {
        GetInc(l, index);
    }

    inline operator SafeEnum<E>()     { return m_value; }
    inline SafeEnum<E>& operator ()() { return m_value; }
    inline SafeEnum<E>& GetValue()    { return m_value; }
    inline bool IsValid(LuaState* l, int index)
    {
        return InnerIsValid(l, index);
    }
private:
    void GetInc(LuaState* l, int& index)
    {
        bool is_nil = lua_isnil(l, index);
        if (!m_optional || (!is_nil && InnerIsValid(l, index)))
        {
            ASSERT(!is_nil);
            InnerGet(l, index);
        }
    }
public:
    inline void Get(LuaState* l, int index)
    {
        int idx = index;
        GetInc(l, idx);
    }
    inline int Return(LuaState* l)
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
    bool InnerIsValid(LuaState* l, int index)
    {
        Var<String> s;
        return s.IsValid(l, index);
    }
    void InnerGet(LuaState* l, int& index)
    {
        Var<String> c(l, index);
        *this = FindValue<SafeEnum<E> >(c);
    }
    int InnerPush(LuaState* l)
    {
        Var<String> s = this->GetValue().ToString();
        return s.Return(l);
    }
};

//-----------------------------------------------------------------------------
class Stack
{
public:
    Stack(LuaState* l, int32_t start_index = 1)
    {
        m_state = l;
        m_index = start_index;
    }
    virtual ~Stack() { }
    inline operator int32_t() { return m_result; }
    int32_t GetArgs()
    {
        return lua_gettop(m_state);
    }

    //-------------------------------------------------------------------------
    template<typename T>
    Stack& operator>>(T& var)
    {
        var = T(m_state, m_index);
        return *this;
    }
    /*
    template<typename T>
    Stack& operator>>(Var<T>& var)
    {
    var = Var<T>(m_state, m_index);
    return *this;
    }
    template<typename T>
    Stack& operator>>(VarPtr<T>& var)
    {
        var = VarPtr<T>(m_state, m_index);
        return *this;
    }
    template<typename T>
    Stack& operator>>(VarPtrLight<T>& var)
    {
        var = VarPtrLight<T>(m_state, m_index);
        return *this;
    }
    */

    //-------------------------------------------------------------------------
    template<typename T>
    Stack& operator<<(T& var)
    {
        m_result += var.Return(m_state);
        return *this;
    }
    /*
    template<typename T>
    Stack& operator<<(Var<T>& var)
    {
        m_result += var.Return(m_state);
        return *this;
    }
    template<typename T>
    Stack& operator<<(VarPtr<T>& var)
    {
        m_result += var.Return(m_state);
        return *this;
    }
    template<typename T>
    Stack& operator<<(VarPtrLight<T>& var)
    {
        m_result += var.Return(m_state);
        return *this;
    }
    */

private:
    LuaState*  m_state = nullptr;
    int32_t m_index = 1;
    int32_t m_result = 0;
};

//-----------------------------------------------------------------------------
class Loader
{
public:
    Loader();
    virtual ~Loader();

    bool ExecLuaFile(String const &lua);
    bool ExecLuaCode(String const &lua);

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

protected:
    LuaState* GetLuaState();

private:
    LuaState* m_lua_state;
};

} /* namespace Lolua */

typedef Lolua::Function         LuaFunction;
typedef Lolua::Object           LuaObject;
typedef Lolua::ObjectDef        LuaObjectDef;
typedef Lolua::ObjectLib        LuaObjectLib;
typedef Lolua::Loader           LuaLoader;
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
typedef Lolua::Stack            LuaStack;

} /* namespace lol */
