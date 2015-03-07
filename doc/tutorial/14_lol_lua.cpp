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
class FooTest
{
public:
    FooTest()
    {
    }
    static int New(Lolua::State* l)
    {
        int n_args = lua_gettop(l);  // Number of arguments

        // We could actually allocate Foo itself as a user data but 
        // since user data can be GC'ed and we gain unity by using CRT's heap 
        // all along.
        FooTest** udata = (FooTest**)lua_newuserdata(l, sizeof(FooTest*));
        *udata = new FooTest();

        // Usually, we'll just use "Foo" as the second parameter, but I 
        // say luaL_Foo here to distinguish the difference:
        //
        // This 2nd parameter here is an _internal label_ for luaL, it is 
        // _not_ exposed to Lua by default.
        //
        // Effectively, this metatable is not accessible by Lua by default.
        //luaL_getmetatable(l, m_table_name);

        // The Lua stack at this point looks like this:
        //     
        //     3| metatable "luaL_foo"   |-1
        //     2| userdata               |-2
        //     1| string parameter       |-3
        //
        // So the following line sets the metatable for the user data to the luaL_Foo 
        // metatable
        //
        // We must set the metatable here because Lua prohibits setting 
        // the metatable of a userdata in Lua. The only way to set a metatable 
        // of a userdata is to do it in C.
        //lua_setmetatable(l, -2);

        luaL_getmetatable(l, m_table2_name);
        lua_setmetatable(l, -2);

        // The Lua stack at this point looks like this:
        //     
        //     2| userdata               |-1
        //     1| string parameter       |-2
        // 
        // We return 1 so Lua callsite will get the user data and 
        // Lua will clean the stack after that.

        return 1;
    }
    static FooTest* Check(Lolua::State* l, int n)
    {
        // This checks that the argument is a userdata 
        // with the metatable "luaL_Foo"
        return *(FooTest**)luaL_testudata(l, n, m_table2_name);
    }
    static int Del(Lolua::State * l)
    {
        FooTest* foo = Check(l, 1);
        delete foo;

        return 0;
    }
    static void Register(Lolua::State * l)
    {
        // Create a luaL metatable. This metatable is not 
        // exposed to Lua. The "luaL_Foo" label is used by luaL
        // internally to identity things.
        luaL_newmetatable(l, m_table2_name);
        luaL_setfuncs(l, FooTest::m_statics2, 0);
        lua_pushvalue(l, -1);
        lua_setfield(l, -1, "__index");
        //lua_setglobal(l, m_class2_name);
        lua_pop(l, 1);

        // Create a luaL metatable. This metatable is not 
        // exposed to Lua. The "luaL_Foo" label is used by luaL
        // internally to identity things.
        luaL_newmetatable(l, m_table_name);

        // Register the C functions _into_ the metatable we just created.
        luaL_setfuncs(l, FooTest::m_statics, 0);

        // The Lua stack at this point looks like this:
        //     
        //     1| metatable "luaL_Foo"   |-1
        lua_pushvalue(l, -1);

        // The Lua stack at this point looks like this:
        //     
        //     2| metatable "luaL_Foo"   |-1
        //     1| metatable "luaL_Foo"   |-2

        // Set the "__index" field of the metatable to point to itself
        // This pops the stack
        lua_setfield(l, -1, "__index");

        // The Lua stack at this point looks like this:
        //     
        //     1| metatable "luaL_Foo"   |-1

        // The luaL_Foo metatable now has the following fields
        //     - __gc
        //     - __index
        //     - new

        // Now we use setglobal to officially expose the luaL_Foo metatable 
        // to Lua. And we use the name "Foo".
        //
        // This allows Lua scripts to _override_ the metatable of Foo.
        // For high security code this may not be called for but 
        // we'll do this to get greater flexibility.
        lua_setglobal(l, m_class_name);
    }
    static int Test1(Lolua::State* L)
    {
        Lolua::Var<int> i1(L, 1);
        i1 += 5;
        return i1.Return(L);
    }
    static int Test2(Lolua::State* L)
    {
        FooTest* foo = Check(L, 1);
        Lolua::Var<float> i1(L, 2);
        i1 = foo->Test2Inst(i1.V());
        return i1.Return(L);
    }
    float Test2Inst(float f)
    {
        return (f + 10);
    }

    static const char m_class_name[];
    static const char m_class2_name[];
    static const char m_table_name[];
    static const char m_table2_name[];
    static const luaL_Reg m_statics[];
    static const luaL_Reg m_statics2[];
    static const luaL_Reg m_methods[];
};

const luaL_Reg FooTest::m_statics[] = {
    { "New", &FooTest::New },
    { "__gc", &FooTest::Del },
    { "Test1", &FooTest::Test1 },
    { NULL, NULL } };
const luaL_Reg FooTest::m_statics2[] = {
    { "Test2", &FooTest::Test2 },
    { NULL, NULL } };
const luaL_Reg FooTest::m_methods[] = { { NULL, NULL } };
const char     FooTest::m_class_name[] = "FooTest";
const char     FooTest::m_class2_name[] = "FooTest2";
const char     FooTest::m_table_name[] = "FooTest_mt";
const char     FooTest::m_table2_name[] = "FooTest_mt2";

typedef Lolua::ObjectDef<FooTest> FooTestLuaBase;
class FooTestLua : public FooTestLuaBase
{
public:
    FooTestLua() : FooTestLuaBase() {}
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
        //String test_return2 = demo_loader->GetVar<String>("test_return2");
        //String test_return3 = demo_loader->GetVar<String>("test_return3");
        int32_t testtruc_return = demo_loader->GetVar<int32_t>("footest_return");
        float testtruc_return2 = demo_loader->GetVar<float>("footest_return2");
        //int32_t testtruc2_return = demo_loader->GetVar<int32_t>("testtruc_return2");

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

