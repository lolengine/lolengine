//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The World class
// ---------------
//

namespace lol
{

class test1
{
public:
    test1() {}
    static int getTest(int i1, String s2)
    {
        return -1;
    }
};

class test1Lua : public LuaObject
{
public:
    test1Lua()
    {
    }
    static test1* New(lua_State* L)
    {
        return new test1();
    }

private:
    static const luaL_Reg m_statics[];
    static const luaL_Reg m_methods[];
    static const char m_class[];

public:
    typedef LuaLibrary<test1, m_class, m_statics, m_methods, test1Lua::New> Library;
};
static int getTest(lua_State* L)
{
    LuaVar<int> i1(L, 1);
    LuaVar<String> s2(L, 2);
    LuaVar<int64_t> res = (int64_t)test1::getTest(i1.V(), s2.V());
    return res.Return(L);
}

class World : public LuaLoader
{
public:
    World();
    virtual ~World();

protected:
    test1Lua::Library m_test1;
};

extern World g_world;

} /* namespace lol */

