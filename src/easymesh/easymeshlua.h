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
    typedef Lolua::VarPtr<EasyMeshLuaObject> EzMeshPtr;
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
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 nsides;
        LuaFloat h, d1, d2;
        LuaBool dualside(false, true), smooth(false, true), close(false, true);
        s >> m >> nsides >> h >> d1 >> d2 >> dualside >> smooth >> close;
        m->m_instance.AppendCylinder(nsides, h, d1, d2, dualside, smooth, close);
        return 0;
    }
    static int AppendSphere(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 ndivisions;
        LuaFloat d;
        s >> m >> ndivisions >> d;
        m->m_instance.AppendSphere(ndivisions, d);
        return 0;
    }
    static int AppendCapsule(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 ndivisions;
        LuaFloat h, d;
        s >> m >> ndivisions >> h >> d;
        m->m_instance.AppendCapsule(ndivisions, h, d);
        return 0;
    }
    static int AppendTorus(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 ndivisions;
        LuaFloat d1, d2;
        s >> m >> ndivisions >> d1 >> d2;
        m->m_instance.AppendTorus(ndivisions, d1, d2);
        return 0;
    }
    static int AppendBox(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaVec3 size;
        LuaFloat chamf(0.f, true);
        LuaBool smooth(false, true);
        s >> m >> size >> chamf >> smooth;
        m->m_instance.AppendBox(size, chamf, smooth);
        return 0;
    }
    static int AppendStar(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 nbranches;
        LuaFloat d1, d2;
        LuaBool fade(false, true), fade2(false, true);
        s >> m >> nbranches >> d1 >> d2 >> fade >> fade2;
        m->m_instance.AppendStar(nbranches, d1, d2, fade, fade2);
        return 0;
    }
    static int AppendExpandedStar(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 nbranches;
        LuaFloat d1, d2, extrad(0.f, true);
        s >> m >> nbranches >> d1 >> d2 >> extrad;
        m->m_instance.AppendExpandedStar(nbranches, d1, d2, extrad);
        return 0;
    }
    static int AppendDisc(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 nsides;
        LuaFloat d;
        LuaBool fade(false, true);
        s >> m >> nsides >> d >> fade;
        m->m_instance.AppendDisc(nsides, d, fade);
        return 0;
    }
    static int AppendSimpleTriangle(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat d;
        LuaBool fade(false, true);
        s >> m >> d >> fade;
        m->m_instance.AppendSimpleTriangle(d, fade);
        return 0;
    }
    static int AppendSimpleQuad(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat size;
        LuaBool fade(false, true);
        s >> m >> size >> fade;
        m->m_instance.AppendSimpleQuad(size, fade);
        return 0;
    }
    static int AppendCog(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 nbsides;
        LuaFloat h, sidemul(0.f, true);
        LuaVec2 d0, d1, d2;
        LuaBool offset(false, true);
        s >> m >> nbsides >> h >> d0 >> d1 >> d2 >> sidemul >> offset;
        m->m_instance.AppendCog(nbsides, h, d0().x, d0().y, d1().x, d1().y, d2().x, d2().y, sidemul, offset);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int TranslateX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat f;
        s >> m >> f;
        m->m_instance.TranslateX(f);
        return 0;
    }
    static int TranslateY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat f;
        s >> m >> f;
        m->m_instance.TranslateY(f);
        return 0;
    }
    static int TranslateZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat f;
        s >> m >> f;
        m->m_instance.TranslateZ(f);
        return 0;
    }
    static int Translate(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaVec3 v;
        s >> m >> v;
        m->m_instance.Translate(v);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int RotateX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat a;
        s >> m >> a;
        m->m_instance.RotateX(a);
        return 0;
    }
    static int RotateY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat a;
        s >> m >> a;
        m->m_instance.RotateY(a);
        return 0;
    }
    static int RotateZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat a;
        s >> m >> a;
        m->m_instance.RotateZ(a);
        return 0;
    }
    static int Rotate(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat a;
        LuaVec3 v;
        s >> m >> a >> v;
        m->m_instance.Rotate(a, v);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int ScaleX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat x;
        s >> m >> x;
        m->m_instance.ScaleX(x);
        return 0;
    }
    static int ScaleY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat y;
        s >> m >> y;
        m->m_instance.ScaleY(y);
        return 0;
    }
    static int ScaleZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat z;
        s >> m >> z;
        m->m_instance.ScaleZ(z);
        return 0;
    }
    static int Scale(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaVec3 v;
        s >> m >> v;
        m->m_instance.Scale(v);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int RadialJitter(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat f;
        s >> m >> f;
        m->m_instance.RadialJitter(f);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int TaperX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat y, z, xoff(0.f, true);
        LuaBool abs(true, true);
        s >> m >> y >> z >> xoff >> abs;
        m->m_instance.TaperX(y, z, xoff, abs);
        return 0;
    }
    static int TaperY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat x, z, yoff(0.f, true);
        LuaBool abs(true, true);
        s >> m >> x >> z >> yoff >> abs;
        m->m_instance.TaperY(x, z, yoff, abs);
        return 0;
    }
    static int TaperZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat x, y, zoff(0.f, true);
        LuaBool abs(true, true);
        s >> m >> x >> y >> zoff >> abs;
        m->m_instance.TaperZ(x, y, zoff, abs);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int TwistX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat t, toff(0.f, true);
        s >> m >> t >> toff;
        m->m_instance.TwistX(t, toff);
        return 0;
    }
    static int TwistY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat t, toff(0.f, true);
        s >> m >> t >> toff;
        m->m_instance.TwistY(t, toff);
        return 0;
    }
    static int TwistZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat t, toff(0.f, true);
        s >> m >> t >> toff;
        m->m_instance.TwistZ(t, toff);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int ShearX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat y, z, xoff(0.f, true);
        LuaBool abs(true, true);
        s >> m >> y >> z >> xoff >> abs;
        m->m_instance.ShearX(y, z, xoff, abs);
        return 0;
    }
    static int ShearY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat x, z, yoff(0.f, true);
        LuaBool abs(true, true);
        s >> m >> x >> z >> yoff >> abs;
        m->m_instance.ShearY(x, z, yoff, abs);
        return 0;
    }
    static int ShearZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat x, y, zoff(0.f, true);
        LuaBool abs(true, true);
        s >> m >> x >> y >> zoff >> abs;
        m->m_instance.ShearZ(x, y, zoff, abs);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int StretchX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat y, z, xoff(0.f, true);
        s >> m >> y >> z >> xoff;
        m->m_instance.StretchX(y, z, xoff);
        return 0;
    }
    static int StretchY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat x, z, yoff(0.f, true);
        s >> m >> x >> z >> yoff;
        m->m_instance.StretchY(x, z, yoff);
        return 0;
    }
    static int StretchZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat x, y, zoff(0.f, true);
        s >> m >> x >> y >> zoff;
        m->m_instance.StretchZ(x, y, zoff);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int BendXY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat t, toff(0.f, true);
        s >> m >> t >> toff;
        m->m_instance.BendXY(t, toff);
        return 0;
    }
    static int BendXZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat t, toff(0.f, true);
        s >> m >> t >> toff;
        m->m_instance.BendXZ(t, toff);
        return 0;
    }
    static int BendYX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat t, toff(0.f, true);
        s >> m >> t >> toff;
        m->m_instance.BendYX(t, toff);
        return 0;
    }
    static int BendYZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat t, toff(0.f, true);
        s >> m >> t >> toff;
        m->m_instance.BendYZ(t, toff);
        return 0;
    }
    static int BendZX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat t, toff(0.f, true);
        s >> m >> t >> toff;
        m->m_instance.BendZX(t, toff);
        return 0;
    }
    static int BendZY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat t, toff(0.f, true);
        s >> m >> t >> toff;
        m->m_instance.BendZY(t, toff);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int MirrorX(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.MirrorX();
        return 0;
    }
    static int MirrorY(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.MirrorY();
        return 0;
    }
    static int MirrorZ(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.MirrorZ();
        return 0;
    }
    //-------------------------------------------------------------------------
    static int LoopStart(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 loopnb;
        s >> m >> loopnb;
        m->m_instance.LoopStart(loopnb);
        return 0;
    }
    static int LoopEnd(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.LoopEnd();
        return 0;
    }
    static int OpenBrace(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.OpenBrace();
        return 0;
    }
    static int CloseBrace(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.CloseBrace();
        return 0;
    }
    //-------------------------------------------------------------------------
    static int ToggleScaleWinding(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.ToggleScaleWinding();
        return 0;
    }
    static int ToggleQuadWeighting(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.ToggleQuadWeighting();
        return 0;
    }
    static int TogglePostBuildNormal(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.TogglePostBuildNormal();
        return 0;
    }
    static int ToggleVerticeNoCleanup(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.ToggleVerticeNoCleanup();
        return 0;
    }
    //-------------------------------------------------------------------------
    static int VerticesMerge(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.VerticesMerge();
        return 0;
    }
    static int VerticesSeparate(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.VerticesSeparate();
        return 0;
    }
    static int VerticesCleanup(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        s >> m;
        m->m_instance.VerticesCleanup();
        return 0;
    }
    //-------------------------------------------------------------------------
    static int Duplicate(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaVec3 ds(vec3(1.f));
        s >> m >> ds;
        m->m_instance.DupAndScale(ds, true);
        return 0;
    }
    static int Smooth(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 pass, split_per_pass, smooth_per_pass;
        s >> m >> pass >> split_per_pass >> smooth_per_pass;
        m->m_instance.SmoothMesh(pass, split_per_pass, smooth_per_pass);
        return 0;
    }
    static int SplitTriangles(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaInt32 pass;
        s >> m >> pass;
        m->m_instance.SplitTriangles(pass);
        return 0;
    }
    static int Chamfer(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaFloat f;
        s >> m >> f;
        m->m_instance.Chamfer(f);
        return 0;
    }
    //-------------------------------------------------------------------------
    static int SetCurColor(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaColor c;
        s >> m >> c;
        m->m_instance.SetCurColor(c);
        return 0;
    }
    static int SetCurColorA(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaColor c;
        s >> m >> c;
        m->m_instance.SetCurColorA(c);
        return 0;
    }
    static int SetCurColorB(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaColor c;
        m->m_instance.SetCurColorB(c);
        return 0;
    }
    static int SetVertColor(lua_State* l)
    {
        LuaStack s(l);
        EzMeshPtr m;
        LuaColor c;
        s >> m >> c;
        m->m_instance.SetVertColor(c);
        return 0;
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
