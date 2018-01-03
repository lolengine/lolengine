//
//  Lol Engine — Mesh viewer
//
//  Copyright © 2003—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2013—2018 Sam Hocevar <sam@hocevar.net>
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
#include <lol/lua.h>

#include <string>

#include "scenesetup.h"

namespace lol
{

//-----------------------------------------------------------------------------
//CTor/DTor
SceneSetup::SceneSetup(std::string const& name)
{
    m_name = name;
    m_clear_color = vec4(vec3::zero, 1.f);
    m_show_gizmo = true;
    m_show_lights = true;
}

//----
SceneSetup::~SceneSetup()
{
    Shutdown(true);
}

//-----------------------------------------------------------------------------
bool SceneSetup::Startup()
{
    for (int i = 0; i < m_lights.count(); i++)
        Ticker::Ref(m_lights[i]);
    return true;
}

//-----------------------------------------------------------------------------
bool SceneSetup::Shutdown(bool destroy)
{
    for (int i = 0; i < m_lights.count(); i++)
        if (m_lights[i]->IsTicked())
            Ticker::Unref(m_lights[i]);

    if (destroy)
        m_lights.empty();
    return true;
}

//-----------------------------------------------------------------------------
void SceneSetup::AddLight(LightType type)
{
    m_last_cmd = SceneSetup::Command::AddLight;
    m_lights << new Light();
    m_lights.last()->SetType(type);
}

//-----------------------------------------------------------------------------
void SceneSetup::SetupScene()
{
    m_last_cmd = SceneSetup::Command::SetupScene;
}

//-----------------------------------------------------------------------------
void SceneSetup::SetPosition(vec3 const& v)
{
    switch (m_last_cmd.ToScalar())
    {
    case SceneSetup::Command::AddLight:
        m_lights.last()->SetPosition(v);
        break;
    }
}

//-----------------------------------------------------------------------------
void SceneSetup::SetLookAt(vec3 const& v)
{
    switch (m_last_cmd.ToScalar())
    {
    case SceneSetup::Command::AddLight:
        //TODO: Do better
        m_lights.last()->SetPosition(v - m_lights.last()->GetPosition());
        break;
    }
}

//-----------------------------------------------------------------------------
void SceneSetup::SetColor(vec4 const& c)
{
    switch (m_last_cmd.ToScalar())
    {
    case SceneSetup::Command::AddLight:
        m_lights.last()->SetColor(c);
        break;
    case SceneSetup::Command::SetupScene:
        m_clear_color = c;
        break;
    }
}

//-----------------------------------------------------------------------------
void SceneSetup::Show(SceneSetup::Display const& d)
{
    Set(d, DisplayFlag::On);
}

//-----------------------------------------------------------------------------
void SceneSetup::Hide(SceneSetup::Display const& d)
{
    Set(d, DisplayFlag::Off);
}

//-----------------------------------------------------------------------------
void SceneSetup::Toggle(SceneSetup::Display const& d)
{
    Set(d, DisplayFlag::Toggle);
}

//-----------------------------------------------------------------------------
void SceneSetup::Set(SceneSetup::Display const& d, DisplayFlag const& f)
{
    bool value = f == DisplayFlag::On ? true : false;
    switch (m_last_cmd.ToScalar())
    {
    case SceneSetup::Command::SetupScene:
        switch (d.ToScalar())
        {
        case SceneSetup::Display::Gizmo:
            m_show_gizmo = f == DisplayFlag::Toggle ? !m_show_gizmo : value;
            break;
        case SceneSetup::Display::Light:
            m_show_lights = f == DisplayFlag::Toggle ? !m_show_lights : value;
            break;
        }
        break;
    }
}

//-----------------------------------------------------------------------------
SceneSetupLuaObject::SceneSetupLuaObject(std::string const& name) : LuaObject()
{
    m_setup = new SceneSetup(name);
    SceneSetupLuaLoader::RegisterSetup(m_setup);
}

//-----------------------------------------------------------------------------
SceneSetupLuaObject::~SceneSetupLuaObject()
{
}

//-----------------------------------------------------------------------------
SceneSetupLuaObject* SceneSetupLuaObject::New(lua_State* l, int arg_nb)
{
    UNUSED(arg_nb);
    auto s = LuaStack::Begin(l);
    auto n = s.Get<std::string>();
    return new SceneSetupLuaObject(n);
}

//-- Setup command ------------------------------------------------------------
int SceneSetupLuaObject::AddLight(lua_State* l)
{
    auto s = LuaStack::Begin(l);
    auto o = s.GetPtr<SceneSetupLuaObject>();
    auto t = s.Get<std::string>();
    o->m_setup->AddLight(FindValue<LightType>(t));
    return s.End();
}
int SceneSetupLuaObject::SetupScene(lua_State* l)
{
    auto s = LuaStack::Begin(l);
    auto o = s.GetPtr<SceneSetupLuaObject>();
    o->m_setup->SetupScene();
    return s.End();
}
//-- main funcs ---------------------------------------------------------------
int SceneSetupLuaObject::SetPosition(lua_State* l)
{
    auto s = LuaStack::Begin(l);
    auto o = s.GetPtr<SceneSetupLuaObject>();
    auto c = s.Get<vec3>();
    o->m_setup->SetPosition(c);
    return s.End();
}
int SceneSetupLuaObject::SetLookAt(lua_State* l)
{
    auto s = LuaStack::Begin(l);
    auto o = s.GetPtr<SceneSetupLuaObject>();
    auto c = s.Get<vec3>();
    o->m_setup->SetLookAt(c);
    return s.End();
}
int SceneSetupLuaObject::SetColor(lua_State* l)
{
    auto s = LuaStack::Begin(l);
    auto o = s.GetPtr<SceneSetupLuaObject>();
    auto c = s.Get<vec4>();
    o->m_setup->SetColor(c);
    return s.End();
}
int SceneSetupLuaObject::Show(lua_State* l)
{
    auto s = LuaStack::Begin(l);
    auto o = s.GetPtr<SceneSetupLuaObject>();
    auto e = s.GetEnum<SceneSetup::DisplayBase>();
    o->m_setup->Show(e);
    return s.End();
}
int SceneSetupLuaObject::Hide(lua_State* l)
{
    auto s = LuaStack::Begin(l);
    auto o = s.GetPtr<SceneSetupLuaObject>();
    auto e = s.GetEnum<SceneSetup::DisplayBase>();
    o->m_setup->Hide(e);
    return s.End();
}
int SceneSetupLuaObject::Toggle(lua_State* l)
{
    auto s = LuaStack::Begin(l);
    auto o = s.GetPtr<SceneSetupLuaObject>();
    auto e = s.GetEnum<SceneSetup::DisplayBase>();
    o->m_setup->Toggle(e);
    return s.End();
}

//-----------------------------------------------------------------------------
const LuaObjectLibrary* SceneSetupLuaObject::GetLib()
{
    typedef SceneSetupLuaObject SSLO;
    static const LuaObjectLibrary lib = LuaObjectLibrary(
        "SceneSetup",
        //Statics
        { { nullptr, nullptr } },
        //Methods
        {
            //-----------------------------------------------------------------
            { "AddLight", AddLight },
            { "Setup", SetupScene },
            //-- main funcs
            { "Position", SetPosition },
            { "LookAt", SetLookAt },
            { "Color", SetColor },
            { "Show", Show },
            { "Hide", Hide },
            { "Toggle", Toggle },
        },
        //Variables
        { { nullptr, nullptr, nullptr } });
    return &lib;
}

//-----------------------------------------------------------------------------
map<std::string, SceneSetup*> SceneSetupLuaLoader::m_setups;
SceneSetupLuaLoader::SceneSetupLuaLoader() : LuaLoader()
{
    lua_State* l = GetLuaState();

    LuaObjectHelper::Register<SceneSetupLuaObject>(l);
}

//-----------------------------------------------------------------------------
SceneSetupLuaLoader::~SceneSetupLuaLoader() { }

//-----------------------------------------------------------------------------
static array<SceneSetupLuaObject*> g_instances;
void SceneSetupLuaLoader::Store(LuaObject* obj)
{
    SceneSetupLuaObject* ezm = static_cast<SceneSetupLuaObject*>(obj);
    g_instances << ezm;
}

//-----------------------------------------------------------------------------
array<SceneSetupLuaObject*>& SceneSetupLuaLoader::GetInstances()
{
    return g_instances;
}

//-----------------------------------------------------------------------------
void SceneSetupLuaLoader::RegisterSetup(SceneSetup* setup)
{
    m_setups[setup->m_name] = setup;
}

//-----------------------------------------------------------------------------
bool SceneSetupLuaLoader::GetRegisteredSetups(map<std::string, SceneSetup*>& setups)
{
    setups = m_setups;
    return !!m_setups.count();
}

//-----------------------------------------------------------------------------
bool SceneSetupLuaLoader::GetLoadedSetups(map<std::string, SceneSetup*>& setups)
{
    return GetRegisteredSetups(setups);
}


} /* namespace lol */
