//
//  Lol Engine — Lua tutorial
//
//  Copyright © 2014—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>
#include "loldebug.h"
#include <cstdio>

using namespace lol;

//-----------------------------------------------------------------------------
class DemoObject : public LuaObject
{
    typedef Lolua::VarPtr<DemoObject> LuaDemoObjectPtr;
public:
    DemoObject() : LuaObject() {}
    virtual ~DemoObject() {}
    static DemoObject* New(lua_State* l, int arg_nb)
    {
        UNUSED(l);
        UNUSED(arg_nb);
        return new DemoObject();
    }

    //-------------------------------------------------------------------------
    static int AddFive(lua_State* l)
    {
		auto stack = LuaStack::Begin(l);
		int32_t i = stack.Get<int32_t>();

        i += 5;

        return (stack << i).End();
    }

    static int AddTenInstance(lua_State* l)
    {
        auto stack = LuaStack::Begin(l);
        DemoObject* obj = stack.GetPtr<DemoObject>();
		float f = stack.Get<float>();

        f = obj->AddTenMethod(f);

        return (stack << f).End();
    }

    float AddTenMethod(float f)
    {
        return (f + 10);
    }

    static int GetX(lua_State* l)
    {
        auto stack = LuaStack::Begin(l);
        DemoObject* obj = stack.GetPtr<DemoObject>();
        auto i = stack.Get<int32_t>();

        i = obj->m_x;

        return (stack << i).End();
    }

    static int SetX(lua_State* l)
    {
        auto stack = LuaStack::Begin(l);
        DemoObject* obj = stack.GetPtr<DemoObject>();
        auto i = stack.Get<int32_t>();

        obj->m_x = i;

        return stack.End();
    }

    //-------------------------------------------------------------------------
    static const LuaObjectLibrary* GetLib()
    {
        static const LuaObjectLibrary lib = LuaObjectLibrary(
            "LoluaDemo",
            { { "AddFive", &DemoObject::AddFive } },
            { { "AddTenInstance", &DemoObject::AddTenInstance } },
            { { "X", &DemoObject::GetX, &DemoObject::SetX } });
        return &lib;
    }

    int m_x = 0;
};

//-----------------------------------------------------------------------------
static int GlobalAddString(lua_State* l)
{
    auto stack = LuaStack::Begin(l);
    auto s = stack.Get<String>();

    s += "_added";

    return (stack << s).End();
}

//-----------------------------------------------------------------------------
class LoluaDemoLoader : public LuaLoader
{
public:
    LoluaDemoLoader() : LuaLoader()
    {
        lua_State* l = GetLuaState();

        //Registering demo object
        LuaObjectHelper::Register<DemoObject>(l);

        //Registering function
        LuaFunction add_string(l, "GlobalAddString", &GlobalAddString);
    }
    virtual ~LoluaDemoLoader()
    {

    }
    void TestStuff()
    {
        lua_State* l = GetLuaState();

        /*
        //create property
        lua_pushnumber(l, 5.0);
        lua_setfield(l, -2, "x");

        lua_getglobal(l, "vector");
        int table = lua_istable(l, -1);
        lua_getfield(l, -1, "x");
        Lolua::Var<float> var(l, -1);
        lua_pop(l, 2);
        vec3 t;
        */

        //table = lua_isuserdata(l, -1);
        //Var<T> var(m_lua_state, -1);
        //lua_pop(m_lua_state, 1);
        //lua_getfield (lua_State *L, int index, const char *k);
        //return var.V();
    }
};

//-----------------------------------------------------------------------------
class LoluaDemo : public WorldEntity
{
public:
    LoluaDemo()
    {
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        LoluaDemoLoader* demo_loader = new LoluaDemoLoader();

        //Execute script
        demo_loader->ExecLuaFile("14_lol_lua.lua");
        demo_loader->TestStuff();

        //Grab global test values
        float testvalue_num = demo_loader->Get<float>("testvalue_num");
        int32_t testvalue_int = demo_loader->Get<int32_t>("testvalue_int");
        uint32_t testvalue_uint = demo_loader->Get<uint32_t>("testvalue_uint");
        String testvalue_str = demo_loader->Get<String>("testvalue_str");

        //Grab string modified with function
        String function_return = demo_loader->Get<String>("function_return");

        //Grab global values modified with DemoObject
        int32_t loluademo_return = demo_loader->Get<int32_t>("loluademo_return");
        int32_t loluademo_getx = demo_loader->Get<int32_t>("loluademo_getx");
        float loluademo_inst_return = demo_loader->Get<float>("loluademo_inst_return");
        DemoObject* loluademo_inst = demo_loader->GetPtr<DemoObject>("loluademo_inst");

        msg::info("Lua Vars: \
            testvalue_num: %.2f, testvalue_int: %i, testvalue_uint: %i, testvalue_str: %s.\n",
            testvalue_num, testvalue_int, testvalue_uint, testvalue_str.C());
        msg::info("Lua Vars: \
            function_return: %s, loluademo_return: %i, loluademo_inst_return: %.f, loluademo_getx: %i, loluademo_inst->m_x: %i.\n",
            function_return.C(), loluademo_return, loluademo_inst_return, loluademo_getx, loluademo_inst->m_x);

        delete demo_loader;

        Ticker::Shutdown();
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);
    }
};

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    sys::init(argc, argv);

    Application app("Tutorial 14: Lolua Demo", ivec2(800, 600), 60.0f);

    new LoluaDemo();

    app.Run();
    return EXIT_SUCCESS;
}

