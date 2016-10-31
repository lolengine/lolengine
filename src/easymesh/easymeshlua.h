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

    //-------------------------------------------------------------------------
    static int AppendCylinder(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto nsides = s.Get<int32_t>();
        auto h = s.Get<float>();
        auto d1 = s.Get<float>();
        auto d2 = s.Get<float>();
        auto dualside = s.Get<bool>(false);
        auto smooth = s.Get<bool>(false);
        auto close = s.Get<bool>(false);
        m->m_instance.AppendCylinder(nsides, h, d1, d2, dualside, smooth, close);
        return s.End();
    }
    //LOLUA_DECLARE_VOID_METHOD(AppendSphere, GetPtr<EasyMeshLuaObject>(), m_instance.AppendSphere, Get<int32_t>(), Get<float>());
    static int AppendSphere(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto ndivisions = s.Get<int32_t>();
        auto d = s.Get<float>();
        m->m_instance.AppendSphere(ndivisions, d);
        return s.End();
    }

    static int AppendCapsule(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto ndivisions = s.Get<int32_t>();
        auto h = s.Get<float>();
        auto d = s.Get<float>();
        m->m_instance.AppendCapsule(ndivisions, h, d);
        return s.End();
    }
    static int AppendTorus(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto ndivisions = s.Get<int32_t>();
        auto d1 = s.Get<float>();
        auto d2 = s.Get<float>();
        m->m_instance.AppendTorus(ndivisions, d1, d2);
        return s.End();
    }
    static int AppendBox(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto size = s.Get<vec3>();
        auto chamf = s.Get<float>(0.f);
        auto smooth = s.Get<bool>(false);
        m->m_instance.AppendBox(size, chamf, smooth);
        return s.End();
    }
    static int AppendStar(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto nbranches = s.Get<int32_t>();
        auto d1 = s.Get<float>();
        auto d2 = s.Get<float>();
        auto fade = s.Get<bool>(false);
        auto fade2 = s.Get<bool>(false);
        m->m_instance.AppendStar(nbranches, d1, d2, fade, fade2);
        return s.End();
    }
    static int AppendExpandedStar(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto nbranches = s.Get<int32_t>();
        auto d1 = s.Get<float>();
        auto d2 = s.Get<float>();
        auto extrad = s.Get<float>(0.f);
        m->m_instance.AppendExpandedStar(nbranches, d1, d2, extrad);
        return s.End();
    }
    static int AppendDisc(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto nsides = s.Get<int32_t>();
        auto d = s.Get<float>();
        auto fade = s.Get<bool>(false);
        m->m_instance.AppendDisc(nsides, d, fade);
        return s.End();
    }
    static int AppendSimpleTriangle(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto d = s.Get<float>();
        auto fade = s.Get<bool>(false);
        m->m_instance.AppendSimpleTriangle(d, fade);
        return s.End();
    }
    static int AppendSimpleQuad(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto size = s.Get<float>();
        auto fade = s.Get<bool>(false);
        m->m_instance.AppendSimpleQuad(size, fade);
        return s.End();
    }
    static int AppendCog(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto nbsides = s.Get<int32_t>();
        auto h = s.Get<float>();
        auto sidemul = s.Get<float>(0.f);
        auto d0 = s.Get<vec2>();
        auto d1 = s.Get<vec2>();
        auto d2 = s.Get<vec2>();
        auto offset = s.Get<bool>(false);
        m->m_instance.AppendCog(nbsides, h, d0.x, d0.y, d1.x, d1.y, d2.x, d2.y, sidemul, offset);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int TranslateX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto f = s.Get<float>();
        m->m_instance.TranslateX(f);
        return s.End();
    }
    static int TranslateY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto f = s.Get<float>();
        m->m_instance.TranslateY(f);
        return s.End();
    }
    static int TranslateZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto f = s.Get<float>();
        m->m_instance.TranslateZ(f);
        return s.End();
    }
    static int Translate(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto v = s.Get<vec3>();
        m->m_instance.Translate(v);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int RotateX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto a = s.Get<float>();
        m->m_instance.RotateX(a);
        return s.End();
    }
    static int RotateY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto a = s.Get<float>();
        m->m_instance.RotateY(a);
        return s.End();
    }
    static int RotateZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto a = s.Get<float>();
        m->m_instance.RotateZ(a);
        return s.End();
    }
    static int Rotate(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto a = s.Get<float>();
        auto v = s.Get<vec3>();
        m->m_instance.Rotate(a, v);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int ScaleX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto x = s.Get<float>();
        m->m_instance.ScaleX(x);
        return s.End();
    }
    static int ScaleY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto y = s.Get<float>();
        m->m_instance.ScaleY(y);
        return s.End();
    }
    static int ScaleZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto z = s.Get<float>();
        m->m_instance.ScaleZ(z);
        return s.End();
    }
    static int Scale(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto v = s.Get<vec3>();
        m->m_instance.Scale(v);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int RadialJitter(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto f = s.Get<float>();
        m->m_instance.RadialJitter(f);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int TaperX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto y = s.Get<float>();
        auto z = s.Get<float>();
        auto xoff = s.Get<float>(0.f);
        auto abs = s.Get<bool>(true);
        m->m_instance.TaperX(y, z, xoff, abs);
        return s.End();
    }
    static int TaperY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto x = s.Get<float>();
        auto z = s.Get<float>();
        auto yoff = s.Get<float>(0.f);
        auto abs = s.Get<bool>(true);
        m->m_instance.TaperY(x, z, yoff, abs);
        return s.End();
    }
    static int TaperZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto x = s.Get<float>();
        auto y = s.Get<float>();
        auto zoff = s.Get<float>(0.f);
        auto abs = s.Get<bool>(true);
        m->m_instance.TaperZ(x, y, zoff, abs);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int TwistX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto t = s.Get<float>();
        auto toff = s.Get<float>(0.f);
        m->m_instance.TwistX(t, toff);
        return s.End();
    }
    static int TwistY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto t = s.Get<float>();
        auto toff = s.Get<float>(0.f);
        m->m_instance.TwistY(t, toff);
        return s.End();
    }
    static int TwistZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto t = s.Get<float>();
        auto toff = s.Get<float>(0.f);
        m->m_instance.TwistZ(t, toff);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int ShearX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto y = s.Get<float>();
        auto z = s.Get<float>();
        auto xoff = s.Get<float>(0.f);
        auto abs = s.Get<bool>(true);
        m->m_instance.ShearX(y, z, xoff, abs);
        return s.End();
    }
    static int ShearY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto x = s.Get<float>();
        auto z = s.Get<float>();
        auto yoff = s.Get<float>(0.f);
        auto abs = s.Get<bool>(true);
        m->m_instance.ShearY(x, z, yoff, abs);
        return s.End();
    }
    static int ShearZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto x = s.Get<float>();
        auto y = s.Get<float>();
        auto zoff = s.Get<float>(0.f);
        auto abs = s.Get<bool>(true);
        m->m_instance.ShearZ(x, y, zoff, abs);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int StretchX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto y = s.Get<float>();
        auto z = s.Get<float>();
        auto xoff = s.Get<float>(0.f);
        m->m_instance.StretchX(y, z, xoff);
        return s.End();
    }
    static int StretchY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto x = s.Get<float>();
        auto z = s.Get<float>();
        auto yoff = s.Get<float>(0.f);
        m->m_instance.StretchY(x, z, yoff);
        return s.End();
    }
    static int StretchZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto x = s.Get<float>();
        auto y = s.Get<float>();
        auto zoff = s.Get<float>(0.f);
        m->m_instance.StretchZ(x, y, zoff);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int BendXY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto t = s.Get<float>();
        auto toff = s.Get<float>(0.f);
        m->m_instance.BendXY(t, toff);
        return s.End();
    }
    static int BendXZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto t = s.Get<float>();
        auto toff = s.Get<float>(0.f);
        m->m_instance.BendXZ(t, toff);
        return s.End();
    }
    static int BendYX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto t = s.Get<float>();
        auto toff = s.Get<float>(0.f);
        m->m_instance.BendYX(t, toff);
        return s.End();
    }
    static int BendYZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto t = s.Get<float>();
        auto toff = s.Get<float>(0.f);
        m->m_instance.BendYZ(t, toff);
        return s.End();
    }
    static int BendZX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto t = s.Get<float>();
        auto toff = s.Get<float>(0.f);
        m->m_instance.BendZX(t, toff);
        return s.End();
    }
    static int BendZY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto t = s.Get<float>();
        auto toff = s.Get<float>(0.f);
        m->m_instance.BendZY(t, toff);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int MirrorX(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.MirrorX();
        return s.End();
    }
    static int MirrorY(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.MirrorY();
        return s.End();
    }
    static int MirrorZ(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.MirrorZ();
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int LoopStart(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto loopnb = s.Get<int32_t>();
        m->m_instance.LoopStart(loopnb);
        return s.End();
    }
    static int LoopEnd(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.LoopEnd();
        return s.End();
    }
    static int OpenBrace(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.OpenBrace();
        return s.End();
    }
    static int CloseBrace(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.CloseBrace();
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int ToggleScaleWinding(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.ToggleScaleWinding();
        return s.End();
    }
    static int ToggleQuadWeighting(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.ToggleQuadWeighting();
        return s.End();
    }
    static int TogglePostBuildNormal(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.TogglePostBuildNormal();
        return s.End();
    }
    static int ToggleVerticeNoCleanup(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.ToggleVerticeNoCleanup();
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int VerticesMerge(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.VerticesMerge();
        return s.End();
    }
    static int VerticesSeparate(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.VerticesSeparate();
        return s.End();
    }
    static int VerticesCleanup(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        m->m_instance.VerticesCleanup();
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int Duplicate(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto ds = s.Get<vec3>(vec3(1.f));
        m->m_instance.DupAndScale(ds, true);
        return s.End();
    }
    static int Smooth(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto pass = s.Get<int32_t>();
        auto split_per_pass = s.Get<int32_t>();
        auto smooth_per_pass = s.Get<int32_t>();
        m->m_instance.SmoothMesh(pass, split_per_pass, smooth_per_pass);
        return s.End();
    }
    static int SplitTriangles(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto pass = s.Get<int32_t>();
        m->m_instance.SplitTriangles(pass);
        return s.End();
    }
    static int Chamfer(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto f = s.Get<float>();
        m->m_instance.Chamfer(f);
        return s.End();
    }
    //-------------------------------------------------------------------------
    static int SetCurColor(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto c = s.Get<vec4>();
        m->m_instance.SetCurColor(c);
        return s.End();
    }
    static int SetCurColorA(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto c = s.Get<vec4>();
        m->m_instance.SetCurColorA(c);
        return s.End();
    }
    static int SetCurColorB(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto c = s.Get<vec4>();
        m->m_instance.SetCurColorB(c);
        return s.End();
    }
    static int SetVertColor(lua_State* l)
    {
        auto s = LuaStack::Begin(l);
        auto m = s.GetPtr<EasyMeshLuaObject>();
        auto c = s.Get<vec4>();
        m->m_instance.SetVertColor(c);
        return s.End();
    }
    /*
    (csgu|csgunion)          { return token::T_CSGUNION; }
    (csgs|CsgSub)            { return token::T_CSGSUBSTRACT; }
    (csgsl|CsgSubL)          { return token::T_CSGSUBSTRACTLOSS; }
    (csga|csgand)            { return token::T_CSGAND; }
    (csgx|csgxor)            { return token::T_CSGXOR; }
    */

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
