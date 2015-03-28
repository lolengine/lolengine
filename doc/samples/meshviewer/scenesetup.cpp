//
// Lol Engine
//
// Copyright: (c) 2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//                2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>
#include "scenesetup.h"

namespace lol
{

//-----------------------------------------------------------------------------
//CTor/DTor
SceneSetup::SceneSetup(String const& name)
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
    for (int i = 0; i < m_lights.Count(); i++)
        Ticker::Ref(m_lights[i]);
    return true;
}

//-----------------------------------------------------------------------------
bool SceneSetup::Shutdown(bool destroy)
{
    for (int i = 0; i < m_lights.Count(); i++)
        if (m_lights[i]->IsTicked())
            Ticker::Unref(m_lights[i]);

    if (destroy)
        m_lights.Empty();
    return true;
}

//-----------------------------------------------------------------------------
void SceneSetup::AddLight(LightType type)
{
    m_last_cmd = SceneSetup::Command::AddLight;
    m_lights << new Light();
    m_lights.Last()->SetType(type);
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
        m_lights.Last()->SetPosition(v);
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
        m_lights.Last()->SetPosition(v - m_lights.Last()->GetPosition());
        break;
    }
}

//-----------------------------------------------------------------------------
void SceneSetup::SetColor(vec4 const& c)
{
    switch (m_last_cmd.ToScalar())
    {
    case SceneSetup::Command::AddLight:
        m_lights.Last()->SetColor(c);
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
SceneSetupLuaObject::SceneSetupLuaObject(String& name) : LuaObject()
{
    m_setup = new SceneSetup(name);
    SceneSetupLuaLoader::RegisterSetup(m_setup);
}

//-----------------------------------------------------------------------------
SceneSetupLuaObject::~SceneSetupLuaObject()
{
}

//-----------------------------------------------------------------------------
SceneSetupLuaObject* SceneSetupLuaObject::New(LuaState* l, int arg_nb)
{
    UNUSED(l);
    UNUSED(arg_nb);
    LuaStack s(l);
    LuaString n;
    s >> n;
    return new SceneSetupLuaObject(n());
}

//-- Setup command ------------------------------------------------------------
int SceneSetupLuaObject::AddLight(LuaState* l)
{
    LuaStack s(l);
    LuaSSetupPtr o;
    LuaString t;
    s >> o >> t;
    o->m_setup->AddLight(FindValue<LightType>(t().C()));
    return 0;
}
int SceneSetupLuaObject::SetupScene(LuaState* l)
{
    LuaStack s(l);
    LuaSSetupPtr o;
    s >> o;
    o->m_setup->SetupScene();
    return 0;
}
//-- main funcs ---------------------------------------------------------------
int SceneSetupLuaObject::SetPosition(LuaState* l)
{
    LuaStack s(l);
    LuaSSetupPtr o;
    LuaVec3 c;
    s >> o >> c;
    o->m_setup->SetPosition(c);
    return 0;
}
int SceneSetupLuaObject::SetLookAt(LuaState* l)
{
    LuaStack s(l);
    LuaSSetupPtr o;
    LuaVec3 c;
    s >> o >> c;
    o->m_setup->SetLookAt(c);
    return 0;
}
int SceneSetupLuaObject::SetColor(LuaState* l)
{
    LuaStack s(l);
    LuaSSetupPtr o;
    LuaColor c;
    s >> o >> c;
    o->m_setup->SetColor(c);
    return 0;
}
int SceneSetupLuaObject::Show(LuaState* l)
{
    LuaStack s(l);
    LuaSSetupPtr o;
    LuaDisplay e;
    s >> o >> e;
    o->m_setup->Show(e);
    return 0;
}
int SceneSetupLuaObject::Hide(LuaState* l)
{
    LuaStack s(l);
    LuaSSetupPtr o;
    LuaDisplay e;
    s >> o >> e;
    o->m_setup->Hide(e);
    return 0;
}
int SceneSetupLuaObject::Toggle(LuaState* l)
{
    LuaStack s(l);
    LuaSSetupPtr o;
    LuaDisplay e;
    s >> o >> e;
    o->m_setup->Toggle(e);
    return 0;
}

//-----------------------------------------------------------------------------
const LuaObjectLib* SceneSetupLuaObject::GetLib()
{
    typedef SceneSetupLuaObject SSLO;
    static const LuaObjectLib lib = LuaObjectLib(
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
map<String, SceneSetup*> SceneSetupLuaLoader::m_setups;
SceneSetupLuaLoader::SceneSetupLuaLoader() : LuaLoader()
{
    LuaState* l = GetLuaState();

    LuaObjectDef::Register<SceneSetupLuaObject>(l);
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

} /* namespace lol */
