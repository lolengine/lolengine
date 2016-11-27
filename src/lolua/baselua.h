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
            { "__tostring", ToString<TLuaClass> },
            { "__add", OpAdd<TLuaClass> },
            { "__sub", OpSubstract<TLuaClass> },
            { "__mul", OpMultiply<TLuaClass> },
            { "__div", OpDivide<TLuaClass> },
            { "__mod", OpModulo<TLuaClass> },
            { "__unm", OpUnaryNeg<TLuaClass> },
            { "__concat", OpConcat<TLuaClass> },
            { "__eq", CmpEqual<TLuaClass> },
            { "__lt", CmpLessThan<TLuaClass> },
            { "__le", CmpLessEqual<TLuaClass> },
            { NULL, NULL }
        };

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
    //-------------------------------------------------------------------------
    template <typename TLuaClass> static int ToString(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int OpAdd(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int OpSubstract(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int OpMultiply(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int OpDivide(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int OpModulo(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int OpUnaryNeg(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int OpConcat(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int CmpEqual(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int CmpLessThan(lua_State* l) { ASSERT(false); return 0; }
    template <typename TLuaClass> static int CmpLessEqual(lua_State* l) { ASSERT(false); return 0; }
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
// Stack: Main class that encapsulates everything -----------------------------
//-----------------------------------------------------------------------------
class Stack
{
    //-------------------------------------------------------------------------
public:
    static Stack Begin(lua_State* state, int32_t start_index = 1)
    {
        return Stack(state, start_index);
    }

    //-------------------------------------------------------------------------
    void SetIndex(int32_t index)
    {
        m_index = index;
    }

    //-------------------------------------------------------------------------
    int32_t End()
    {
        return m_result;
    }

    //-------------------------------------------------------------------------
protected:
    Stack(lua_State* l, int32_t start_index = 1)
    {
        m_state = l;
        m_index = start_index;
    }

public:
    virtual ~Stack() { }

protected:
    int32_t GetArgs()
    {
        return lua_gettop(m_state);
    }

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
        inline T* operator ->() { return m_value; }
        inline Ptr<T>& operator=(T const*& value) { m_value = value; return *this; }
    };

private:
    bool AllowGet(bool is_optional, bool value_validity)
    {
        bool is_nil = lua_isnil(m_state, m_index);
        if (!is_optional || (!is_nil && value_validity))
        {
            ASSERT(!is_nil); /* touky: should it assert, though ? */
            return true;
        }
        return false;
    }

public:
	//-------------------------------------------------------------------------
    template<typename T> T Get() { return Get(InnerDefault<T>(), false); }
    template<typename T> T Get(T default_value) { return Get(default_value, true); }
    template<typename E> SafeEnum<E> GetEnum() { return GetEnum(InnerDefaultSafeEnum<E>(), false); }
    template<typename E> SafeEnum<E> GetEnum(SafeEnum<E> default_value) { return GetEnum(default_value, true); }
    template<typename P> Ptr<P> GetPtr() { return GetPtr(InnerDefaultPtr<P>(), false); }
    template<typename P> Ptr<P> GetPtr(Ptr<P> default_value) { return GetPtr(default_value, true); }

private:
    //-------------------------------------------------------------------------
    template<typename T> T Get(T default_value, bool is_optional)
    {
        if (AllowGet(is_optional, InnerIsValid<T>()))
            return InnerGet(default_value);
        return default_value;
    }
    template<typename E> SafeEnum<E> GetEnum(SafeEnum<E> default_value, bool is_optional)
    {
        if (AllowGet(is_optional, InnerIsValidSafeEnum<E>()))
            return InnerGetSafeEnum(default_value);
        return default_value;
    }
    template<typename P> Ptr<P> GetPtr(Ptr<P> default_value, bool is_optional)
    {
        if (AllowGet(is_optional, InnerIsValidPtr<P>()))
            return InnerGetPtr(default_value);
        return default_value;
    }

public:
    //-------------------------------------------------------------------------
    template<typename T> Stack& operator<<(T value) { m_result += InnerPush<T>(value); return *this; }
    template<typename E> Stack& operator<<(SafeEnum<E> value) { m_result += InnerPushSafeEnum<E>(value); return *this; }
    template<typename P> Stack& operator<<(Ptr<P> value) { m_result += InnerPushPtr<P>(value); return *this; }

protected:
    //-------------------------------------------------------------------------
    #define INNER_ERROR "Your type is not implemented. For pointers, use LuaPtr<MyType>()"
    template<typename T> T InnerDefault() { return T(0); }
	template<typename T> bool InnerIsValid() { ASSERT(false, INNER_ERROR); return false; }
    template<typename T> T InnerGet(T value) { UNUSED(value); ASSERT(false, INNER_ERROR); return InnerDefault<T>(); }
    template<typename T> int InnerPush(T value) { UNUSED(value); ASSERT(false, INNER_ERROR); return 0; }

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
        return 1;
    }
#endif //STACK_STRING

    //-------------------------------------------------------------------------
private:
    lua_State*  m_state = nullptr;
    int32_t m_index = 1;
    int32_t m_result = 0;
};

//-----------------------------------------------------------------------------
#define /***/ LOLUA_VAR_1(a00) auto v00 = s.a00;
#define /***/ LOLUA_VAR_2(a00, a01) LOLUA_VAR_1(a00) auto v01 = s.a01;
#define /***/ LOLUA_VAR_3(a00, a01, a02) LOLUA_VAR_2(a00, a01) auto v02 = s.a02;
#define /***/ LOLUA_VAR_4(a00, a01, a02, a03) LOLUA_VAR_3(a00, a01, a02) auto v03 = s.a03;
#define /***/ LOLUA_VAR_5(a00, a01, a02, a03, a04) LOLUA_VAR_4(a00, a01, a02, a03) auto v04 = s.a04;
#define /***/ LOLUA_VAR_6(a00, a01, a02, a03, a04, a05) LOLUA_VAR_5(a00, a01, a02, a03, a04) auto v05 = s.a05;
#define /***/ LOLUA_VAR_7(a00, a01, a02, a03, a04, a05, a06) LOLUA_VAR_6(a00, a01, a02, a03, a04, a05) auto v06 = s.a06;
#define /***/ LOLUA_VAR_8(a00, a01, a02, a03, a04, a05, a06, a07) LOLUA_VAR_7(a00, a01, a02, a03, a04, a05, a06) auto v07 = s.a07;
#define /***/ LOLUA_VAR_9(a00, a01, a02, a03, a04, a05, a06, a07, a08) LOLUA_VAR_8(a00, a01, a02, a03, a04, a05, a06, a07) auto v08 = s.a08;
#define /**/ LOLUA_VAR_10(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09) LOLUA_VAR_9(a00, a01, a02, a03, a04, a05, a06, a07, a08) auto v09 = s.a09;
#define /**/ LOLUA_VAR_11(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a10) LOLUA_VAR_10(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09) auto v10 = s.a10;
#define /**/ LOLUA_VAR_12(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a10, a11) LOLUA_VAR_11(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09) auto v11 = s.a11;
//-----------------------------------------------------------------------------
#define /***/ LOLUA_ARG_1(a00) v00
#define /***/ LOLUA_ARG_2(a00, a01) LOLUA_ARG_1(a00), v01
#define /***/ LOLUA_ARG_3(a00, a01, a02) LOLUA_ARG_2(a00, a01), v02
#define /***/ LOLUA_ARG_4(a00, a01, a02, a03) LOLUA_ARG_3(a00, a01, a02), v03
#define /***/ LOLUA_ARG_5(a00, a01, a02, a03, a04) LOLUA_ARG_4(a00, a01, a02, a03), v04
#define /***/ LOLUA_ARG_6(a00, a01, a02, a03, a04, a05) LOLUA_ARG_5(a00, a01, a02, a03, a04), v05
#define /***/ LOLUA_ARG_7(a00, a01, a02, a03, a04, a05, a06) LOLUA_ARG_6(a00, a01, a02, a03, a04, a05), v06
#define /***/ LOLUA_ARG_8(a00, a01, a02, a03, a04, a05, a06, a07) LOLUA_ARG_7(a00, a01, a02, a03, a04, a05, a06), v07
#define /***/ LOLUA_ARG_9(a00, a01, a02, a03, a04, a05, a06, a07, a08) LOLUA_ARG_8(a00, a01, a02, a03, a04, a05, a06, a07), v08
#define /**/ LOLUA_ARG_10(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09) LOLUA_ARG_9(a00, a01, a02, a03, a04, a05, a06, a07, a08), v09
#define /**/ LOLUA_ARG_11(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a10) LOLUA_ARG_10(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09), v10
#define /**/ LOLUA_ARG_12(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a10, a11) LOLUA_ARG_11(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09), v11
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define LOLUA_DECLARE_BEGIN(LUA_FUNC_NAME, INSTANCE_GET) \
    static int LUA_FUNC_NAME(lua_State* l) \
    { \
        auto s = LuaStack::Begin(l); \
        auto o = s.INSTANCE_GET;

#define LOLUA_DECLARE_VARS(...) \
    LOL_CALL(LOL_CAT(LOLUA_VAR_, LOL_CALL(LOL_COUNT_TO_12, (__VA_ARGS__))), (__VA_ARGS__));

#define LOLUA_DECLARE_CALL(INSTANCE_CALL, ...) \
    o->INSTANCE_CALL(LOL_CALL(LOL_CAT(LOLUA_ARG_, LOL_CALL(LOL_COUNT_TO_12, (__VA_ARGS__))), (__VA_ARGS__)));

#define LOLUA_DECLARE_CALL_VOID(INSTANCE_CALL) \
    o->INSTANCE_CALL();

#define LOLUA_DECLARE_END \
        return s.End(); \
    }

//-----------------------------------------------------------------------------
#define LOLUA_DECLARE_VOID_METHOD_VOID(LUA_FUNC_NAME, INSTANCE_GET, INSTANCE_CALL, ...) \
    LOLUA_DECLARE_BEGIN(LUA_FUNC_NAME, INSTANCE_GET); \
    LOLUA_DECLARE_CALL_VOID(INSTANCE_CALL) \
    LOLUA_DECLARE_END

#define LOLUA_DECLARE_VOID_METHOD_ARGS(LUA_FUNC_NAME, INSTANCE_GET, INSTANCE_CALL, ...) \
    LOLUA_DECLARE_BEGIN(LUA_FUNC_NAME, INSTANCE_GET); \
    LOLUA_DECLARE_VARS(__VA_ARGS__) \
    LOLUA_DECLARE_CALL(INSTANCE_CALL, __VA_ARGS__) \
    LOLUA_DECLARE_END

//-----------------------------------------------------------------------------
#define LOLUA_DECLARE_RETURN_METHOD_ARGS(LUA_FUNC_NAME, INSTANCE_GET, INSTANCE_CALL, ...) \
    static int LUA_FUNC_NAME(lua_State* l) \
    { \
        auto s = LuaStack::Begin(l); \
        auto o = s.INSTANCE_GET; \
        LOL_CALL(LOL_CAT(LOLUA_VAR_, LOL_CALL(LOL_COUNT_TO_12, (__VA_ARGS__))), (__VA_ARGS__)) \
        s << o->INSTANCE_CALL(LOL_CALL(LOL_CAT(LOLUA_ARG_, LOL_CALL(LOL_COUNT_TO_12, (__VA_ARGS__))), (__VA_ARGS__))); \
        return s.End(); \
    }

//-----------------------------------------------------------------------------
#ifndef REGION_STACK_VAR

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

#endif //REGION_STACK_VAR

//-----------------------------------------------------------------------------
class Loader
{
    friend class ObjectHelper;
public:
    Loader();
    virtual ~Loader();

    bool ExecLuaFile(String const &lua);
    bool ExecLuaCode(String const &lua);

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

template <typename TLuaClass>
int ObjectHelper::Store(lua_State * l)
{
    auto stack = LuaStack::Begin(l);
    TLuaClass* obj = stack.GetPtr<TLuaClass>();
    ASSERT(obj);
    Loader::StoreObject(l, obj);
    return 0;
}

template <typename TLuaClass>
int ObjectHelper::Del(lua_State * l)
{
    auto stack = LuaStack::Begin(l);
    TLuaClass* obj = stack.GetPtr<TLuaClass>();
    ASSERT(obj);
    delete obj;
    return 0;
}

} /* namespace Lolua */

//TYPEDEFS
typedef Lolua::Function         LuaFunction;
typedef Lolua::ObjectHelper     LuaObjectHelper;
typedef Lolua::Object           LuaObject;
typedef Lolua::Object::Library  LuaObjectLibrary;
typedef Lolua::Loader           LuaLoader;
typedef Lolua::Stack            LuaStack;
template <typename P> using LuaPtr = Lolua::Stack::Ptr<P>;

} /* namespace lol */
