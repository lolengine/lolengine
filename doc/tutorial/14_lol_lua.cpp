//
// Lol Engine - Graphing tutorial
//
// Copyright: (c) 2012-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>
#include "loldebug.h"
#include <cstdio>

using namespace lol;

//-----------------------------------------------------------------------------
class DemoObject : public Lolua::ObjectDef
{
public:
    DemoObject() : Lolua::ObjectDef() {}
    virtual ~DemoObject() {}
    static DemoObject* New(Lolua::State* l, int arg_nb)
    {
        UNUSED(l);
        UNUSED(arg_nb);
        return new DemoObject();
    }
    static const char* GetClassName() { static const char name[] = "LoluaDemo"; return name; }
    static const char* GetClassLibName() { static const char name[] = "LoluaDemoLib"; return name; }
    static const char* GetClassInstName() { static const char name[] = "LoluaDemoInst"; return name; }

    static const Lolua::ClassMethod* GetStaticMethods();
    static const Lolua::ClassMethod* GetInstanceMethods();

    static int AddFive(Lolua::State* l)
    {
        Lolua::Var<int> i(l, 1);
        i += 5;
        return i.Return(l);
    }
    static int AddTenInstance(Lolua::State* l)
    {
        Lolua::VarPtr<DemoObject> obj(l, 1);
        Lolua::Var<float> f(l, 2);
        f = obj.V()->AddTenMethod(f.V());
        return f.Return(l);
    }
    float AddTenMethod(float f)
    {
        return (f + 10);
    }
};

//-----------------------------------------------------------------------------
static const Lolua::ClassMethod loluademo_statics[] = { { "AddFive", &DemoObject::AddFive }, { NULL, NULL } };
static const Lolua::ClassMethod loluademo_methods[] = { { "AddTenInstance", &DemoObject::AddTenInstance }, { NULL, NULL } };
const Lolua::ClassMethod* DemoObject::GetStaticMethods() { return loluademo_statics; }
const Lolua::ClassMethod* DemoObject::GetInstanceMethods() { return loluademo_methods; }

//-----------------------------------------------------------------------------
static int GlobalAddString(Lolua::State* l)
{
    Lolua::Var<String> s(l, 1);
    s += "_added";
    return s.Return(l);
}

//-----------------------------------------------------------------------------
class LoluaDemoLoader : public Lolua::Loader
{
public:
    LoluaDemoLoader() : Lolua::Loader()
    {
        Lolua::State* l = GetLuaState();

        //Registering demo object
        Lolua::Object::Register<DemoObject>(l);

        //Registering function
        Lolua::Function add_string(l, "GlobalAddString", &GlobalAddString);
    }
    virtual ~LoluaDemoLoader()
    {

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
        demo_loader->ExecLua("14_lol_lua.lua");

        //Grab global test values
        float testvalue_num = demo_loader->GetVar<float>("testvalue_num");
        int32_t testvalue_int = demo_loader->GetVar<int32_t>("testvalue_int");
        uint32_t testvalue_uint = demo_loader->GetVar<uint32_t>("testvalue_uint");
        String testvalue_str = demo_loader->GetVar<String>("testvalue_str");

        //Grab string modified with function
        String function_return = demo_loader->GetVar<String>("function_return");

        //Grab global values modified with DemoObject
        int32_t loluademo_return = demo_loader->GetVar<int32_t>("loluademo_return");
        float loluademo_inst_return = demo_loader->GetVar<float>("loluademo_inst_return");
        DemoObject* loluademo_inst = demo_loader->GetPtr<DemoObject>("loluademo_inst");

        String loluademo_inst_name = loluademo_inst->GetClassName();

        Log::Info("Lua Vars: \
            testvalue_num: %.2f, testvalue_int: %i, testvalue_uint: %i, testvalue_str: %s.\n", 
            testvalue_num, testvalue_int, testvalue_uint, testvalue_str.C());
        Log::Info("Lua Vars: \
            function_return: %s, loluademo_return: %i, loluademo_inst_return: %.f, loluademo_inst_name: %s.\n",
            function_return.C(), loluademo_return, loluademo_inst_return, loluademo_inst_name.C());

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
    System::Init(argc, argv);

    Application app("Tutorial 14: Lolua Demo", ivec2(800, 600), 60.0f);

    new LoluaDemo();

    app.Run();
    return EXIT_SUCCESS;
}

