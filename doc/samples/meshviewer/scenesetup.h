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

#pragma once

//
// The Scene setup class
// ----------------
//

#include <string>
#include <map>

namespace lol
{

//-----------------------------------------------------------------------------
class SceneSetup
{
    friend class SceneSetupLuaLoader;
    friend class SceneSetupLuaObject;

public:
    //CTor/DTor
    SceneSetup() : SceneSetup("default") { }
    SceneSetup(std::string const& name);
    ~SceneSetup();

    static std::string GetName() { return "<scenesetup>"; }

    //--
    bool Startup();
    bool Shutdown(bool destroy=false);

    //-- Setup command
    void AddLight(LightType type);
    void SetupScene();

    //-- main funcs
    void SetPosition(vec3 const& v);
    void SetLookAt(vec3 const& v);
    void SetColor(vec4 const& c);

    //-------------------------------------------------------------------------
    struct DisplayBase : public StructSafeEnum
    {
        enum Type
        {
            Gizmo,
            Light,

            Max
        };
    protected:
        virtual bool BuildEnumMap(std::map<int64_t, std::string>& enum_map)
        {
            enum_map[Gizmo] = "Gizmo";
            enum_map[Light] = "Light";
            return true;
        }
    };
    typedef SafeEnum<DisplayBase>   Display;

    void Show(Display const& d);
    void Hide(Display const& d);
    void Toggle(Display const& d);
protected:
    void Set(Display const& d, DisplayFlag const& f);

protected:
    //-------------------------------------------------------------------------
    struct CommandBase : public StructSafeEnum
    {
        enum Type
        {
            AddLight,
            SetupScene,

            Max
        };
    protected:
        virtual bool BuildEnumMap(std::map<int64_t, std::string>& enum_map)
        {
            enum_map[AddLight] = "AddLight";
            enum_map[SetupScene] = "SetupScene";
            return true;
        }
    };
    typedef SafeEnum<CommandBase> Command;

public:
    //--
    std::string m_name;
    Command m_last_cmd;
    vec4 m_clear_color;
    array<Light *> m_lights;
    array<std::string, std::string> m_custom_cmd;
    bool m_show_gizmo;
    bool m_show_lights;
};

//-----------------------------------------------------------------------------
class SceneSetupLuaObject : public LuaObject
{
public:
    //-------------------------------------------------------------------------
    SceneSetupLuaObject(std::string const& name);
    virtual ~SceneSetupLuaObject();

    //-------------------------------------------------------------------------
    static SceneSetupLuaObject* New(lua_State* l, int arg_nb);
    static const LuaObjectLibrary* GetLib();

    //-------------------------------------------------------------------------
public:
    //-- Setup command
    static int AddLight(lua_State* l);
    static int SetupScene(lua_State* l);
    //-- main funcs
    static int SetPosition(lua_State* l);
    static int SetLookAt(lua_State* l);
    static int SetColor(lua_State* l);
    static int Show(lua_State* l);
    static int Hide(lua_State* l);
    static int Toggle(lua_State* l);

    //-- Setup command
    void AddLight(LightType type);
    void SetupScene();

    //-- main funcs
    void SetPosition(vec3& v);
    void SetLookAt(vec3& v);
    void SetColor(vec4& c);
    void Show(SceneSetup::Display const& d);
    void Hide(SceneSetup::Display const& d);
    void Toggle(SceneSetup::Display const& d);

private:
    SceneSetup* m_setup = nullptr;
};

//-----------------------------------------------------------------------------
class SceneSetupLuaLoader : public LuaLoader
{
    friend class SceneSetupLuaObject;
public:
    SceneSetupLuaLoader();
    virtual ~SceneSetupLuaLoader();
    //Virtual Store lua object ------------------------------------------------
    virtual void Store(LuaObject* obj);
    array<SceneSetupLuaObject*>& GetInstances();

    //-------------------------------------------------------------------------
protected:
    static void RegisterSetup(SceneSetup* setup);
    static bool GetRegisteredSetups(std::map<std::string, SceneSetup*>& setups);
public:
    bool GetLoadedSetups(std::map<std::string, SceneSetup*>& setups);

private:
    static std::map<std::string, SceneSetup*> m_setups;
};

/*
addlight            { return token::T_ADDLIGHT;     }
position            { return token::T_OBJPOSITION;  }
lookat              { return token::T_OBJLOOKAT;    }
color               { return token::T_OBJCOLOR;     }
clearcolor          { return token::T_CLEARCOLOR;   }

showgizmo           { return token::T_SHOWGIZMO;    }
showlight           { return token::T_SHOWLIGHT;    }
custom              { return token::T_CUSTOMCMD;    }

custom              { return token::T_CUSTOMCMD;    }

light_command:
T_ADDLIGHT                      { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT"; }
| T_ADDLIGHT      fv              { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT";
uc.m_sstp.m_lights.last()->SetType(LightType($2)); }
| T_ADDLIGHT      svv             { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT";
uc.m_sstp.m_lights.last()->SetType(FindValue<LightType>($2)); }
;

setup_command:
T_OBJPOSITION   v3              { if (uc.m_last_cmd == "ADDLIGHT")
uc.m_sstp.m_lights.last()->SetPosition(vec3($2[0], $2[1], $2[2])); }
| T_OBJLOOKAT     v3              { if (uc.m_last_cmd == "ADDLIGHT")
{
} }
| T_OBJCOLOR      v4{ if (uc.m_last_cmd == "ADDLIGHT")
uc.m_sstp.m_lights.last()->SetColor(vec4($2[0], $2[1], $2[2], $2[3])); }
| T_OBJCOLOR      COLOR{ uint32_t x = $2;
ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
vec4 vv = vec4(v) * (1.f / 255.f);
if (uc.m_last_cmd == "ADDLIGHT")
uc.m_sstp.m_lights.last()->SetColor(vv); }
;

scene_command:
T_CLEARCOLOR    v4{ uc.m_sstp.m_clear_color = vec4($2[0], $2[1], $2[2], $2[3]); }
T_CLEARCOLOR    v3{ uc.m_sstp.m_clear_color = vec4($2[0], $2[1], $2[2], 1.f); }
| T_CLEARCOLOR    COLOR{ uint32_t x = $2; ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
uc.m_sstp.m_clear_color = vec4(v) * (1.f / 255.f); }
| T_SHOWGIZMO     bv{ uc.m_sstp.m_show_gizmo = $2; }
| T_SHOWLIGHT     bv{ uc.m_sstp.m_show_lights = $2; }
;

custom_command:
T_CUSTOMCMD     svv sv{ uc.m_sstp.m_custom_cmd.push($2, $3); }
;

*/

} /* namespace lol */

