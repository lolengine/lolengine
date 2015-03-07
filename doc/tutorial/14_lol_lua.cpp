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
class FooTestLua : public Lolua::ObjectDef
{
public:
    FooTestLua() : Lolua::ObjectDef() {}
    virtual ~FooTestLua() {}
    static FooTestLua* New(Lolua::State* l, int arg_nb)
    {
        return new FooTestLua();
    }
    static const char* GetClassName();
    static const char* GetClassLibName();
    static const char* GetClassInstName();
    static const Lolua::ClassMethod* GetStaticMethods();
    static const Lolua::ClassMethod* GetInstanceMethods();
    static int Test1(Lolua::State* L)
    {
        Lolua::Var<int> i1(L, 1);
        i1 += 5;
        return i1.Return(L);
    }
    static int Test2(Lolua::State* L)
    {
        Lolua::VarPtr<FooTestLua> foo(L, 1);
        Lolua::Var<float> i1(L, 2);
        i1 = foo.V()->Test2Inst(i1.V());
        return i1.Return(L);
    }
    float Test2Inst(float f)
    {
        return (f + 10);
    }
};

static const Lolua::ClassMethod FooTest_statics[] = { { "Test1", &FooTestLua::Test1 }, { NULL, NULL } };
const Lolua::ClassMethod FooTest_methods[] = { { "Test2", &FooTestLua::Test2 }, { NULL, NULL } };

const char* FooTestLua::GetClassName() { static const char name[] = "FooTest"; return name; }
const char* FooTestLua::GetClassLibName() { static const char name[] = "FooTestLib"; return name; }
const char* FooTestLua::GetClassInstName() { static const char name[] = "FooTestInst"; return name; }
const Lolua::ClassMethod* FooTestLua::GetStaticMethods() { return FooTest_statics; }
const Lolua::ClassMethod* FooTestLua::GetInstanceMethods() { return FooTest_methods; }

//-----------------------------------------------------------------------------
class LoluaDemoLoader : public Lolua::Loader
{
public:
    LoluaDemoLoader() : Lolua::Loader()
    {
        Lolua::State* l = GetLuaState();

        Lolua::Object::Register<FooTestLua>(l);

        ExecLua("14_lol_lua.lua");
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

        float TestValueNum = demo_loader->GetVar<float>("TestValueNum");
        int32_t TestValueInt = demo_loader->GetVar<int32_t>("TestValueInt");
        uint32_t TestValueUint = demo_loader->GetVar<uint32_t>("TestValueUint");
        String TestValueStr = demo_loader->GetVar<String>("TestValueStr");
        int32_t testtruc_return = demo_loader->GetVar<int32_t>("footest_return");
        float testtruc_return2 = demo_loader->GetVar<float>("footest_return2");

        String::Printf("Lua Vars: TestValueNum: %.2f, TestValueInt: %i, TestValueUint: %i, TestValueStr: %s.", 
                                  TestValueNum, TestValueInt, TestValueUint, TestValueStr);

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

