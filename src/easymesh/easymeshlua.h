//
//  Lol Engine — EasyMesh Lua loader
//
//  Copyright © 2009—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

namespace lol
{

//-----------------------------------------------------------------------------
class EasyMeshLuaObject : public LuaObject
{
    EasyMesh m_instance;
public:
    //-------------------------------------------------------------------------
    EasyMeshLuaObject(String const& name);
    virtual ~EasyMeshLuaObject();
    EasyMesh& GetMesh() { return m_instance; }

    //-------------------------------------------------------------------------
    static EasyMeshLuaObject* New(lua_State* l, int arg_nb);
    static const LuaObjectLibrary* GetLib();

#define EMLO GetPtr<EasyMeshLuaObject>()

    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendCylinder, EMLO, m_instance.AppendCylinder, Get<int32_t>(), Get<float>(), Get<float>(), Get<float>(), Get<bool>(false), Get<bool>(false), Get<bool>(false));
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendSphere, EMLO, m_instance.AppendSphere, Get<int32_t>(), Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendCapsule, EMLO, m_instance.AppendCapsule, Get<int32_t>(), Get<float>(), Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendTorus, EMLO, m_instance.AppendTorus, Get<int32_t>(), Get<float>(), Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendBox, EMLO, m_instance.AppendBox, Get<vec3>(), Get<float>(0.f), Get<bool>(false));
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendStar, EMLO, m_instance.AppendStar, Get<int32_t>(), Get<float>(), Get<float>(), Get<bool>(false), Get<bool>(false));
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendExpandedStar, EMLO, m_instance.AppendExpandedStar, Get<int32_t>(), Get<float>(), Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendDisc, EMLO, m_instance.AppendDisc, Get<int32_t>(), Get<float>(), Get<bool>(false));
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendSimpleTriangle, EMLO, m_instance.AppendSimpleTriangle, Get<float>(), Get<bool>(false));
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendSimpleQuad, EMLO, m_instance.AppendSimpleQuad, Get<float>(), Get<bool>(false));
    LOLUA_DECLARE_VOID_METHOD_ARGS(AppendCog, EMLO, m_instance.AppendCog, Get<int32_t>(), Get<float>(), Get<vec2>(), Get<vec2>(), Get<vec2>(), Get<float>(0.f), Get<bool>(false));
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(TranslateX, EMLO, m_instance.TranslateX, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(TranslateY, EMLO, m_instance.TranslateY, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(TranslateZ, EMLO, m_instance.TranslateZ, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(Translate, EMLO, m_instance.Translate, Get<vec3>());
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(RotateX, EMLO, m_instance.RotateX, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(RotateY, EMLO, m_instance.RotateY, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(RotateZ, EMLO, m_instance.RotateZ, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(Rotate, EMLO, m_instance.Rotate, Get<float>(), Get<vec3>());
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(ScaleX, EMLO, m_instance.ScaleX, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(ScaleY, EMLO, m_instance.ScaleY, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(ScaleZ, EMLO, m_instance.ScaleZ, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(Scale, EMLO,  m_instance.Scale, Get<vec3>());
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(RadialJitter, EMLO, m_instance.RadialJitter, Get<float>());
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(TaperX, EMLO, m_instance.TaperX, Get<float>(), Get<float>(), Get<float>(0.f), Get<bool>(true));
    LOLUA_DECLARE_VOID_METHOD_ARGS(TaperY, EMLO, m_instance.TaperY, Get<float>(), Get<float>(), Get<float>(0.f), Get<bool>(true));
    LOLUA_DECLARE_VOID_METHOD_ARGS(TaperZ, EMLO, m_instance.TaperZ, Get<float>(), Get<float>(), Get<float>(0.f), Get<bool>(true));
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(TwistX, EMLO, m_instance.TwistX, Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(TwistY, EMLO, m_instance.TwistY, Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(TwistZ, EMLO, m_instance.TwistZ, Get<float>(), Get<float>(0.f));
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(ShearX, EMLO, m_instance.ShearX, Get<float>(), Get<float>(), Get<float>(0.f), Get<bool>(true));
    LOLUA_DECLARE_VOID_METHOD_ARGS(ShearY, EMLO, m_instance.ShearY, Get<float>(), Get<float>(), Get<float>(0.f), Get<bool>(true));
    LOLUA_DECLARE_VOID_METHOD_ARGS(ShearZ, EMLO, m_instance.ShearZ, Get<float>(), Get<float>(), Get<float>(0.f), Get<bool>(true));
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(StretchX, EMLO, m_instance.StretchX, Get<float>(), Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(StretchY, EMLO, m_instance.StretchY, Get<float>(), Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(StretchZ, EMLO, m_instance.StretchZ, Get<float>(), Get<float>(), Get<float>(0.f));
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(BendXY, EMLO, m_instance.BendXY, Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(BendXZ, EMLO, m_instance.BendXZ, Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(BendYX, EMLO, m_instance.BendYX, Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(BendYZ, EMLO, m_instance.BendYZ, Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(BendZX, EMLO, m_instance.BendZX, Get<float>(), Get<float>(0.f));
    LOLUA_DECLARE_VOID_METHOD_ARGS(BendZY, EMLO, m_instance.BendZY, Get<float>(), Get<float>(0.f));
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_VOID(MirrorX, EMLO, m_instance.MirrorX);
    LOLUA_DECLARE_VOID_METHOD_VOID(MirrorY, EMLO, m_instance.MirrorY);
    LOLUA_DECLARE_VOID_METHOD_VOID(MirrorZ, EMLO, m_instance.MirrorZ);
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(LoopStart, EMLO, m_instance.LoopStart, Get<int32_t>());
    LOLUA_DECLARE_VOID_METHOD_VOID(LoopEnd, EMLO, m_instance.LoopEnd);
    LOLUA_DECLARE_VOID_METHOD_VOID(OpenBrace, EMLO, m_instance.OpenBrace, Get<float>());
    LOLUA_DECLARE_VOID_METHOD_VOID(CloseBrace, EMLO, m_instance.CloseBrace, Get<float>());
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_VOID(ToggleScaleWinding, EMLO, m_instance.ToggleScaleWinding);
    LOLUA_DECLARE_VOID_METHOD_VOID(ToggleQuadWeighting, EMLO, m_instance.ToggleQuadWeighting);
    LOLUA_DECLARE_VOID_METHOD_VOID(TogglePostBuildNormal, EMLO, m_instance.TogglePostBuildNormal);
    LOLUA_DECLARE_VOID_METHOD_VOID(ToggleVerticeNoCleanup, EMLO, m_instance.ToggleVerticeNoCleanup);
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_VOID(VerticesMerge, EMLO, m_instance.VerticesMerge);
    LOLUA_DECLARE_VOID_METHOD_VOID(VerticesSeparate, EMLO, m_instance.VerticesSeparate);
    LOLUA_DECLARE_VOID_METHOD_VOID(VerticesCleanup, EMLO, m_instance.VerticesCleanup);
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(Duplicate, EMLO, m_instance.DupAndScale, Get<vec3>(vec3(1.f)), Get<bool>(true));
    LOLUA_DECLARE_VOID_METHOD_ARGS(Smooth, EMLO, m_instance.SmoothMesh, Get<int32_t>(), Get<int32_t>(), Get<int32_t>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(SplitTriangles, EMLO, m_instance.SplitTriangles, Get<int32_t>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(Chamfer, EMLO, m_instance.Chamfer, Get<float>());
    //-------------------------------------------------------------------------
    LOLUA_DECLARE_VOID_METHOD_ARGS(SetCurColor, EMLO, m_instance.SetCurColor, Get<vec4>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(SetCurColorA, EMLO, m_instance.SetCurColorA, Get<vec4>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(SetCurColorB, EMLO, m_instance.SetCurColorB, Get<vec4>());
    LOLUA_DECLARE_VOID_METHOD_ARGS(SetVertColor, EMLO, m_instance.SetVertColor, Get<vec4>());

    /*
    (csgu|csgunion)          { return token::T_CSGUNION; }
    (csgs|CsgSub)            { return token::T_CSGSUBSTRACT; }
    (csgsl|CsgSubL)          { return token::T_CSGSUBSTRACTLOSS; }
    (csga|csgand)            { return token::T_CSGAND; }
    (csgx|csgxor)            { return token::T_CSGXOR; }
    */

    #undef EMLO
};

//-----------------------------------------------------------------------------
class EasyMeshLuaLoader : public LuaLoader
{
    friend class EasyMeshLuaObject;

public:
    EasyMeshLuaLoader();
    virtual ~EasyMeshLuaLoader();
    //Virtual Store lua object ------------------------------------------------
    virtual void Store(LuaObject* obj);
    array<EasyMeshLuaObject*>& GetInstances();

    //-------------------------------------------------------------------------
protected:
    static void RegisterMesh(EasyMeshLuaObject* mesh, String const& name);
public:
    static bool GetRegisteredMeshes(map<String, EasyMeshLuaObject*>& meshes);

private:
    static map<String, EasyMeshLuaObject*> m_meshes;
};

} /* namespace lol */
