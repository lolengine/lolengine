//
//  MY CLASS TYPE
//
//  Copyright © 2009-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include <lol/engine.h>
#include "loldebug.h"

using namespace lol;

//-----------------------------------------------------------------------------
EasyMeshLuaLoader::EasyMeshLuaLoader() : Lolua::Loader()
{
    Lolua::State* l = GetLuaState();

    //Registering demo object
    Lolua::Object::Register<EasyMeshLuaObject>(l);
}

//-----------------------------------------------------------------------------
EasyMeshLuaLoader::~EasyMeshLuaLoader()
{

}

//-----------------------------------------------------------------------------
EasyMeshLuaObject::EasyMeshLuaObject() : Lolua::ObjectDef()
{
}

//-----------------------------------------------------------------------------
EasyMeshLuaObject::~EasyMeshLuaObject()
{
}

//-----------------------------------------------------------------------------
EasyMeshLuaObject* EasyMeshLuaObject::New(Lolua::State* l, int arg_nb)
{
    UNUSED(l);
    UNUSED(arg_nb);
    return new EasyMeshLuaObject();
}

//-------------------------------------------------------------------------
const Lolua::ObjectLib* EasyMeshLuaObject::GetLib()
{
    static const Lolua::ObjectLib lib = Lolua::ObjectLib(
        "EasyMesh",
        { { nullptr, nullptr } },
        { { nullptr, nullptr } },
        { { nullptr, nullptr, nullptr } });
    return &lib;
}

//-----------------------------------------------------------------------------
bool EasyMesh::Compile(char const *command, bool Execute)
{
    bool res = false;
    // FIXME: make this work again
#if 0
    EasyMeshCompiler mc(*this);
    BD()->Enable(MeshBuildOperation::CommandRecording);
    if ((res = mc.ParseString(command)))
    {
        BD()->Disable(MeshBuildOperation::CommandRecording);
        if (Execute)
            ExecuteCmdStack();
    }
#endif
    return res;
}

//-----------------------------------------------------------------------------
#define EZSET(M0)                                               BD()->CmdStack().GetValue(M0);
#define EZDEF_1(T0)                                             T0 m0; EZSET(m0)
#define EZDEF_2(T0, T1)                                         EZDEF_1(T0) T1 m1; EZSET(m1)
#define EZDEF_3(T0, T1, T2)                                     EZDEF_2(T0, T1) T2 m2; EZSET(m2)
#define EZDEF_4(T0, T1, T2, T3)                                 EZDEF_3(T0, T1, T2) T3 m3; EZSET(m3)
#define EZDEF_5(T0, T1, T2, T3, T4)                             EZDEF_4(T0, T1, T2, T3) T4 m4; EZSET(m4)
#define EZDEF_6(T0, T1, T2, T3, T4, T5)                         EZDEF_5(T0, T1, T2, T3, T4) T5 m5; EZSET(m5)
#define EZDEF_7(T0, T1, T2, T3, T4, T5, T6)                     EZDEF_6(T0, T1, T2, T3, T4, T5) T6 m6; EZSET(m6)
#define EZDEF_8(T0, T1, T2, T3, T4, T5, T6, T7)                 EZDEF_7(T0, T1, T2, T3, T4, T5, T6) T7 m7; EZSET(m7)
#define EZDEF_9(T0, T1, T2, T3, T4, T5, T6, T7, T8)             EZDEF_8(T0, T1, T2, T3, T4, T5, T6, T7) T8 m8; EZSET(m8)
#define EZDEF_10(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)        EZDEF_9(T0, T1, T2, T3, T4, T5, T6, T7, T8) T9 m9; EZSET(m9)

//----
#define EZCALL_1(F)                                             F();
#define EZCALL_2(F, T0)                                         EZDEF_1(T0) F(m0);
#define EZCALL_3(F, T0, T1)                                     EZDEF_2(T0, T1) F(m0, m1);
#define EZCALL_4(F, T0, T1, T2)                                 EZDEF_3(T0, T1, T2) F(m0, m1, m2);
#define EZCALL_5(F, T0, T1, T2, T3)                             EZDEF_4(T0, T1, T2, T3) F(m0, m1, m2, m3);
#define EZCALL_6(F, T0, T1, T2, T3, T4)                         EZDEF_5(T0, T1, T2, T3, T4) F(m0, m1, m2, m3, m4);
#define EZCALL_7(F, T0, T1, T2, T3, T4, T5)                     EZDEF_6(T0, T1, T2, T3, T4, T5) F(m0, m1, m2, m3, m4, m5);
#define EZCALL_8(F, T0, T1, T2, T3, T4, T5, T6)                 EZDEF_7(T0, T1, T2, T3, T4, T5, T6) F(m0, m1, m2, m3, m4, m5, m6);
#define EZCALL_9(F, T0, T1, T2, T3, T4, T5, T6, T7)             EZDEF_8(T0, T1, T2, T3, T4, T5, T6, T7) F(m0, m1, m2, m3, m4, m5, m6, m7);
#define EZCALL_10(F, T0, T1, T2, T3, T4, T5, T6, T7, T8)        EZDEF_9(T0, T1, T2, T3, T4, T5, T6, T7, T8) F(m0, m1, m2, m3, m4, m5, m6, m7, m8);
#define EZCALL_11(F, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)    EZDEF_10(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) F(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9);

//----
#define EZM_CALL_FUNC(...) \
    LOL_CALL(LOL_CAT(EZCALL_, LOL_CALL(LOL_COUNT_TO_12, (__VA_ARGS__))), (__VA_ARGS__))

//-----------------------------------------------------------------------------
void EasyMesh::ExecuteCmdStack(bool ExecAllStack)
{
#define DO_EXEC_CMD(MESH_CMD, FUNC_PARAMS)  \
        case EasyMeshCmdType::MESH_CMD:     \
    { EZM_CALL_FUNC FUNC_PARAMS; break; }

    BD()->Enable(MeshBuildOperation::CommandExecution);
    if (ExecAllStack)
        BD()->Cmdi() = 0;

    for (; BD()->Cmdi() < BD()->CmdStack().GetCmdNb() && BD()->CmdExecNb() != 0; ++BD()->Cmdi())
    {
        if (BD()->CmdExecNb() > 0)
            --BD()->CmdExecNb();

        switch (BD()->CmdStack().GetCmd(BD()->Cmdi()))
        {
            DO_EXEC_CMD(MeshCsg, (MeshCsg, CSGUsage))
                DO_EXEC_CMD(LoopStart, (LoopStart, int))
                DO_EXEC_CMD(LoopEnd, (LoopEnd))
                DO_EXEC_CMD(OpenBrace, (OpenBrace))
                DO_EXEC_CMD(CloseBrace, (CloseBrace))
                DO_EXEC_CMD(ScaleWinding, (ToggleScaleWinding))
                DO_EXEC_CMD(QuadWeighting, (ToggleQuadWeighting))
                DO_EXEC_CMD(PostBuildNormal, (TogglePostBuildNormal))
                DO_EXEC_CMD(PreventVertCleanup, (ToggleVerticeNoCleanup))
                DO_EXEC_CMD(VerticesMerge, (VerticesMerge))
                DO_EXEC_CMD(VerticesSeparate, (VerticesSeparate))
                DO_EXEC_CMD(SetColorA, (SetCurColorA, vec4))
                DO_EXEC_CMD(SetColorB, (SetCurColorB, vec4))
                DO_EXEC_CMD(SetVertColor, (SetVertColor, vec4))
                DO_EXEC_CMD(Translate, (Translate, vec3))
                DO_EXEC_CMD(Rotate, (Rotate, float, vec3))
                DO_EXEC_CMD(RadialJitter, (RadialJitter, float))
                DO_EXEC_CMD(MeshTranform, (DoMeshTransform, MeshTransform, Axis, Axis, float, float, float, bool))
                DO_EXEC_CMD(Scale, (Scale, vec3))
                DO_EXEC_CMD(DupAndScale, (DupAndScale, vec3, bool))
                DO_EXEC_CMD(Chamfer, (Chamfer, float))
                DO_EXEC_CMD(SplitTriangles, (SplitTriangles, int))
                DO_EXEC_CMD(SmoothMesh, (SmoothMesh, int, int, int))
                DO_EXEC_CMD(AppendCylinder, (AppendCylinder, int, float, float, float, bool, bool, bool))
                DO_EXEC_CMD(AppendCapsule, (AppendCapsule, int, float, float))
                DO_EXEC_CMD(AppendTorus, (AppendTorus, int, float, float))
                DO_EXEC_CMD(AppendBox, (AppendBox, vec3, float, bool))
                DO_EXEC_CMD(AppendStar, (AppendStar, int, float, float, bool, bool))
                DO_EXEC_CMD(AppendExpandedStar, (AppendExpandedStar, int, float, float, float))
                DO_EXEC_CMD(AppendDisc, (AppendDisc, int, float, bool))
                DO_EXEC_CMD(AppendSimpleTriangle, (AppendSimpleTriangle, float, bool))
                DO_EXEC_CMD(AppendSimpleQuad, (AppendSimpleQuad, vec2, vec2, float, bool))
                DO_EXEC_CMD(AppendCog, (AppendCog, int, float, float, float, float, float, float, float, float, bool))
        default:
            ASSERT(0, "Unknown command pseudo bytecode");
        }
    }
    BD()->Disable(MeshBuildOperation::CommandExecution);

    if (!BD()->IsEnabled(MeshBuildOperation::PreventVertCleanup))
        VerticesCleanup();

    if (BD()->IsEnabled(MeshBuildOperation::PostBuildComputeNormals))
        ComputeNormals(0, (int)m_indices.Count());

    BD()->Disable(MeshBuildOperation::PostBuildComputeNormals);
    BD()->Disable(MeshBuildOperation::PreventVertCleanup);

    if (BD()->CmdExecNb() > 0)
        BD()->CmdExecNb() = -1;
}

