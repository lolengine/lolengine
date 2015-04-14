//
// Lol Engine
//
// Copyright: (c) 2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Scene setup class
// ----------------
//

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
    SceneSetup(String const& name);
    ~SceneSetup();

    static char const *GetName() { return "<scenesetup>"; }

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
        virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
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
        virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
        {
            enum_map[AddLight] = "AddLight";
            enum_map[SetupScene] = "SetupScene";
            return true;
        }
    };
    typedef SafeEnum<CommandBase> Command;

public:
    //--
    String                      m_name;
    Command                     m_last_cmd;
    vec4                        m_clear_color;
    array<Light *>              m_lights;
    array<String, String>       m_custom_cmd;
    bool                        m_show_gizmo;
    bool                        m_show_lights;
};
typedef Lolua::VarEnum<SceneSetup::DisplayBase> LuaDisplay;

//-----------------------------------------------------------------------------
class SceneSetupLuaObject : public LuaObject
{
    typedef Lolua::VarPtr<SceneSetupLuaObject> LuaSSetupPtr;
public:
    //-------------------------------------------------------------------------
    SceneSetupLuaObject(String& name);
    virtual ~SceneSetupLuaObject();

    //-------------------------------------------------------------------------
    static SceneSetupLuaObject* New(LuaState* l, int arg_nb);
    static const LuaObjectLib* GetLib();

    //-------------------------------------------------------------------------
public:
    //-- Setup command
    static int AddLight(LuaState* l);
    static int SetupScene(LuaState* l);
    //-- main funcs
    static int SetPosition(LuaState* l);
    static int SetLookAt(LuaState* l);
    static int SetColor(LuaState* l);
    static int Show(LuaState* l);
    static int Hide(LuaState* l);
    static int Toggle(LuaState* l);

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
    static bool GetRegisteredSetups(map<String, SceneSetup*>& setups);
public:
    bool GetLoadedSetups(map<String, SceneSetup*>& setups);

private:
    static map<String, SceneSetup*> m_setups;
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
uc.m_sstp.m_lights.Last()->SetType(LightType($2)); }
| T_ADDLIGHT      svv             { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT";
uc.m_sstp.m_lights.Last()->SetType(FindValue<LightType>($2)); }
;

setup_command:
T_OBJPOSITION   v3              { if (uc.m_last_cmd == "ADDLIGHT")
uc.m_sstp.m_lights.Last()->SetPosition(vec3($2[0], $2[1], $2[2])); }
| T_OBJLOOKAT     v3              { if (uc.m_last_cmd == "ADDLIGHT")
{
} }
| T_OBJCOLOR      v4{ if (uc.m_last_cmd == "ADDLIGHT")
uc.m_sstp.m_lights.Last()->SetColor(vec4($2[0], $2[1], $2[2], $2[3])); }
| T_OBJCOLOR      COLOR{ uint32_t x = $2;
ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
vec4 vv = vec4(v) * (1.f / 255.f);
if (uc.m_last_cmd == "ADDLIGHT")
uc.m_sstp.m_lights.Last()->SetColor(vv); }
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
T_CUSTOMCMD     svv sv{ uc.m_sstp.m_custom_cmd.Push($2, $3); }
;

*/

} /* namespace lol */

