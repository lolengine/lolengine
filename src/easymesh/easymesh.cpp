//
// Lol Engine
//
// Copyright: (c) 2010-2015 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2015 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The EasyMesh class
// ------------------
//

#include <lol/engine-internal.h>

LOLFX_RESOURCE_DECLARE(shiny);

namespace lol
{

//-----------------------------------------------------------------------------
EasyMesh::EasyMesh()
  : m_build_data(nullptr)
{
    m_cursors.Push(0, 0);
    m_state = MeshRender::NeedData;
}

//-----------------------------------------------------------------------------
EasyMesh::EasyMesh(const EasyMesh& em)
{
    m_indices = em.m_indices;
    m_vert = em.m_vert;
    m_cursors = em.m_cursors;
    m_build_data = nullptr;
    if (em.m_build_data)
        m_build_data = new EasyMeshBuildData(*em.m_build_data);
    if (m_indices.Count() && m_vert.Count() && m_cursors.Count())
        m_state = MeshRender::NeedConvert;
    else
        m_state = MeshRender::NeedData;
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

void EasyMesh::MeshConvert()
{
    /* Default material */
    Shader *shader = Shader::Create(LOLFX_RESOURCE_NAME(shiny));

    /* Push index buffer to GPU */
    IndexBuffer *ibo = new IndexBuffer(m_indices.Count() * sizeof(uint16_t));
    uint16_t *indices = (uint16_t *)ibo->Lock(0, 0);
    for (int i = 0; i < m_indices.Count(); ++i)
        indices[i] = m_indices[i];
    ibo->Unlock();

    /* Push vertex buffer to GPU */
    struct Vertex
    {
        vec3 pos, normal;
        u8vec4 color;
        vec4 texcoord;
    };

    VertexDeclaration *vdecl = new VertexDeclaration(
        VertexStream<vec3, vec3, u8vec4, vec4>(VertexUsage::Position,
                                               VertexUsage::Normal,
                                               VertexUsage::Color,
                                               VertexUsage::TexCoord));

    VertexBuffer *vbo = new VertexBuffer(m_vert.Count() * sizeof(Vertex));
    Vertex *vert = (Vertex *)vbo->Lock(0, 0);
    for (int i = 0; i < m_vert.Count(); ++i)
    {
        vert[i].pos = m_vert[i].m_coord,
        vert[i].normal = m_vert[i].m_normal,
        vert[i].color = (u8vec4)(m_vert[i].m_color * 255.f);
        vert[i].texcoord = m_vert[i].m_texcoord;
    }
    vbo->Unlock();

    /* Reference our new data in our submesh */
    m_submeshes.Push(new SubMesh(shader, vdecl));
    m_submeshes.Last()->SetIndexBuffer(ibo);
    m_submeshes.Last()->SetVertexBuffer(0, vbo);

    m_state = MeshRender::CanRender;
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
            DO_EXEC_CMD(MeshCsg,                (MeshCsg, CSGUsage))
            DO_EXEC_CMD(LoopStart,              (LoopStart, int))
            DO_EXEC_CMD(LoopEnd,                (LoopEnd))
            DO_EXEC_CMD(OpenBrace,              (OpenBrace))
            DO_EXEC_CMD(CloseBrace,             (CloseBrace))
            DO_EXEC_CMD(ScaleWinding,           (ToggleScaleWinding))
            DO_EXEC_CMD(QuadWeighting,          (ToggleQuadWeighting))
            DO_EXEC_CMD(PostBuildNormal,        (TogglePostBuildNormal))
            DO_EXEC_CMD(PreventVertCleanup,     (ToggleVerticeNoCleanup))
            DO_EXEC_CMD(VerticesMerge,          (VerticesMerge))
            DO_EXEC_CMD(VerticesSeparate,       (VerticesSeparate))
            DO_EXEC_CMD(SetColorA,              (SetCurColorA, vec4))
            DO_EXEC_CMD(SetColorB,              (SetCurColorB, vec4))
            DO_EXEC_CMD(SetVertColor,           (SetVertColor, vec4))
            DO_EXEC_CMD(Translate,              (Translate, vec3))
            DO_EXEC_CMD(Rotate,                 (Rotate, float, vec3))
            DO_EXEC_CMD(RadialJitter,           (RadialJitter, float))
            DO_EXEC_CMD(MeshTranform,           (DoMeshTransform, MeshTransform, Axis, Axis, float, float, float, bool))
            DO_EXEC_CMD(Scale,                  (Scale, vec3))
            DO_EXEC_CMD(DupAndScale,            (DupAndScale, vec3, bool))
            DO_EXEC_CMD(Chamfer,                (Chamfer, float))
            DO_EXEC_CMD(SplitTriangles,         (SplitTriangles, int))
            DO_EXEC_CMD(SmoothMesh,             (SmoothMesh, int, int, int))
            DO_EXEC_CMD(AppendCylinder,         (AppendCylinder, int, float, float, float, bool, bool, bool))
            DO_EXEC_CMD(AppendCapsule,          (AppendCapsule, int, float, float))
            DO_EXEC_CMD(AppendTorus,            (AppendTorus, int, float, float))
            DO_EXEC_CMD(AppendBox,              (AppendBox, vec3, float, bool))
            DO_EXEC_CMD(AppendStar,             (AppendStar, int, float, float, bool, bool))
            DO_EXEC_CMD(AppendExpandedStar,     (AppendExpandedStar, int, float, float, float))
            DO_EXEC_CMD(AppendDisc,             (AppendDisc, int, float, bool))
            DO_EXEC_CMD(AppendSimpleTriangle,   (AppendSimpleTriangle, float, bool))
            DO_EXEC_CMD(AppendSimpleQuad,       (AppendSimpleQuad, vec2, vec2, float, bool))
            DO_EXEC_CMD(AppendCog,              (AppendCog, int, float, float, float, float, float, float, float, float, bool))
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

//-----------------------------------------------------------------------------
void EasyMesh::LoopStart(int loopnb)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::LoopStart);
        BD()->CmdStack() << loopnb;
        return;
    }
    //Loop is only available when executing a command recording
    else if (BD()->IsEnabled(MeshBuildOperation::CommandExecution))
    {
        //Only register if we're not the current loop command
        if (!BD()->LoopStack().Count() || BD()->LoopStack().Last().m1 != BD()->Cmdi())
            BD()->LoopStack().Push(BD()->Cmdi(), loopnb);
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::LoopEnd()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::LoopEnd);
        return;
    }
    //Loop is only available when executing a command recording
    else if (BD()->IsEnabled(MeshBuildOperation::CommandExecution))
    {
        //Only register if we're not the current loop command
        if (BD()->LoopStack().Count())
        {
            BD()->LoopStack().Last().m2--;
            if (BD()->LoopStack().Last().m2 > 0)
                BD()->Cmdi() = BD()->LoopStack().Last().m1 - 1;
            else
                BD()->LoopStack().Pop();
        }
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::OpenBrace()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::OpenBrace);
        return;
    }

    m_cursors.Push((int)m_vert.Count(), (int)m_indices.Count());
}

//-----------------------------------------------------------------------------
void EasyMesh::CloseBrace()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::CloseBrace);
        return;
    }

    m_cursors.Pop();
}

//-----------------------------------------------------------------------------
bool EasyMesh::SetRender(bool should_render)
{
    if (m_state == MeshRender::CanRender)
    {
        if (!should_render)
            m_state = MeshRender::IgnoreRender;
        return true;
    }
    else if (m_state == MeshRender::IgnoreRender)
    {
        if (should_render)
            m_state = MeshRender::CanRender;
        return true;
    }
    return false;
}

//-------------------
//Mesh Cursor operations
//-------------------

//-----------------------------------------------------------------------------
void EasyMesh::MeshCsg(CSGUsage csg_operation)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::MeshCsg);
        BD()->CmdStack() << csg_operation;
        return;
    }

    //A vertex dictionnary for vertices on the same spot.
    array< int, int > vertex_dict;
    //This list keeps track of the triangle that will need deletion at the end.
    array< int > triangle_to_kill;
    //Listing for each triangle of the vectors intersecting it. <tri_Id, <Point0, Point1, tri_isec_Normal>>
    array< int, array< vec3, vec3, vec3 > > triangle_isec;
    //keep a track of the intersection point on the triangle. <pos, side_id>
    array< vec3, int > triangle_vertex;
    for (int k = 0; k < 10; k++)
        triangle_vertex.Push(vec3(.0f), 0);

    //bsp infos
    CsgBsp mesh_bsp_0;
    CsgBsp mesh_bsp_1;

    if (m_cursors.Count() == 0)
        return;

    //BSP BUILD : We use the brace logic, csg should be used as : "[ exp .... [exp .... csg]]"
    int cursor_start = (m_cursors.Count() < 2)?(0):(m_cursors[(m_cursors.Count() - 2)].m2);
    for (int mesh_id = 0; mesh_id < 2; mesh_id++)
    {
        ptrdiff_t start_point = (mesh_id == 0) ? (cursor_start) : (m_cursors.Last().m2);
        ptrdiff_t end_point   = (mesh_id == 0) ? (m_cursors.Last().m2) : (m_indices.Count());
        CsgBsp &mesh_bsp      = (mesh_id == 0) ? (mesh_bsp_0) : (mesh_bsp_1);
        for (ptrdiff_t i = start_point; i < end_point; i += 3)
            mesh_bsp.AddTriangleToTree((int)i, m_vert[m_indices[i]].m_coord,
                                          m_vert[m_indices[i + 1]].m_coord,
                                          m_vert[m_indices[i + 2]].m_coord);
    }

    //BSP Usage : let's crunch all triangles on the correct BSP
    ptrdiff_t indices_count = m_indices.Count();
    for (ptrdiff_t mesh_id = 0; mesh_id < 2; mesh_id++)
    {
        ptrdiff_t start_point = (mesh_id == 0) ? (cursor_start) : (m_cursors.Last().m2);
        ptrdiff_t end_point   = (mesh_id == 0) ? (m_cursors.Last().m2) : (indices_count);
        CsgBsp &mesh_bsp      = (mesh_id == 0) ? (mesh_bsp_1) : (mesh_bsp_0);
        array< vec3, int, int, float > vert_list;
        array< int, int, int, int > tri_list;
        vec3 n0(.0f); vec3 n1(.0f);
        vec4 c0(.0f); vec4 c1(.0f);

        //Reserve some memory
        vert_list.Reserve(3);
        tri_list.Reserve(3);

        for (ptrdiff_t i = start_point; i < end_point; i += 3)
        {
            int Result = mesh_bsp.TestTriangleToTree(m_vert[m_indices[i]].m_coord,
                                                     m_vert[m_indices[i + 1]].m_coord,
                                                     m_vert[m_indices[i + 2]].m_coord, vert_list, tri_list);
            ptrdiff_t tri_base_idx = m_indices.Count();

            //one split has been done, we need to had the new vertices & the new triangles.
            if (Result == 1)
            {
                triangle_to_kill.Push((int)i);
#if 1
                ptrdiff_t base_idx = m_vert.Count();
                for (ptrdiff_t k = 3; k < vert_list.Count(); k++)
                {
                    ptrdiff_t P0 = (vert_list[k].m2 < 3) ? (m_indices[i + vert_list[k].m2]) : (base_idx + vert_list[k].m2 - 3);
                    ptrdiff_t P1 = (vert_list[k].m3 < 3) ? (m_indices[i + vert_list[k].m3]) : (base_idx + vert_list[k].m3 - 3);

                    InternalAddVertex(vert_list[k].m1);

                    //Normal : bad calculations there.
                    n0 = m_vert[P0].m_normal;
                    n1 = m_vert[P1].m_normal;
                    SetCurVertNormal(normalize(n0 + (n1 - n0) * vert_list[k].m4));

#if 1
                    //Color
                    c0 = m_vert[P0].m_color;
                    c1 = m_vert[P1].m_color;
                    vec4 res = c0 + ((c1 - c0) * vert_list[k].m4);
                    SetCurVertColor(res);
#else
                    if (mesh_id == 0)
                        SetCurVertColor(vec4(1.0f, .0f, .0f, 1.0f));
                    else
                        SetCurVertColor(vec4(.0f, 1.0f, 1.0f, 1.0f));
#endif
                }
                for (ptrdiff_t k = 0; k < tri_list.Count(); k++)
                {
                    int P0 = (int)(tri_list[k].m2 < 3) ? (m_indices[i + tri_list[k].m2]) : ((int)base_idx + (tri_list[k].m2 - 3));
                    int P1 = (int)(tri_list[k].m3 < 3) ? (m_indices[i + tri_list[k].m3]) : ((int)base_idx + (tri_list[k].m3 - 3));
                    int P2 = (int)(tri_list[k].m4 < 3) ? (m_indices[i + tri_list[k].m4]) : ((int)base_idx + (tri_list[k].m4 - 3));
                    InternalAddTriangle(P0, P1, P2, 0);
                }
#endif
            }
#if 1
            //Main case
            if (Result >= 0)
            {
                for (int k = 0; k < tri_list.Count(); k++)
                {
                    ptrdiff_t tri_idx = (ptrdiff_t)((tri_list.Count() == 1) ? (i) : ((int)tri_base_idx + k * 3));

                    //Triangle Kill Test
                    if (//csgu : CSGUnion() -> m0_Outside + m1_Outside
                        (csg_operation == CSGUsage::Union && tri_list[k].m1 == LEAF_BACK) ||
                        //csgs : CsgSub() -> m0_Outside + m1_Inside-inverted
                        (csg_operation == CSGUsage::Substract &&
                            ((mesh_id == 0 && tri_list[k].m1 == LEAF_BACK) ||
                            (mesh_id == 1 && tri_list[k].m1 == LEAF_FRONT))) ||
                        //csgs : CsgSubL() -> m0_Outside
                        (csg_operation == CSGUsage::SubstractLoss &&
                            ((mesh_id == 0 && tri_list[k].m1 == LEAF_BACK) || mesh_id == 1)) ||
                        //csga : CSGAnd() -> m0_Inside + m1_Inside
                        (csg_operation == CSGUsage::And && tri_list[k].m1 == LEAF_FRONT))
                    {
                        triangle_to_kill.Push((int)tri_idx);
                    }

                    //Triangle Invert Test
                    if (//csgs : CsgSub() -> m0_Outside + m1_Inside-inverted
                        (csg_operation == CSGUsage::Substract && mesh_id == 1 && tri_list[k].m1 == LEAF_BACK) ||
                        //csgx : CSGXor() -> m0_Outside/m0_Inside-inverted + m1_Outside/m1_Inside-inverted
                        (csg_operation == CSGUsage::Xor && tri_list[k].m1 == LEAF_BACK))
                    {
                        //a Xor means we will share vertices with the outside, so duplicate the vertices.
                        //TODO : This operation disconnect all triangle, in some cases, not a good thing.
                        if (csg_operation == CSGUsage::Xor)
                        {
                            for (ptrdiff_t l = 0; l < 3; l++)
                            {
                                AddDuplicateVertex(m_indices[tri_idx + l]);
                                m_indices[tri_idx + l] = (uint16_t)m_vert.Count() - 1;
                            }
                        }
                        m_indices[tri_idx + 1] += m_indices[tri_idx + 2];
                        m_indices[tri_idx + 2]  = m_indices[tri_idx + 1] - m_indices[tri_idx + 2];
                        m_indices[tri_idx + 1]  = m_indices[tri_idx + 1] - m_indices[tri_idx + 2];
                        ComputeNormals((int)tri_idx, 3);
                    }
                }
            }
#endif
            vert_list.Empty();
            tri_list.Empty();
        }
    }

    for (int i = 0; i < m_vert.Count(); i++)
        if (length(m_vert[i].m_normal) < 1.0f)
            i = i;

    int dir = 1;
    for (int i = 0; i >= 0 && i < triangle_to_kill.Count() - 1; i += dir)
    {
        if (triangle_to_kill[i] < triangle_to_kill[i + 1] && dir < 0)
            dir = 1;
        if (triangle_to_kill[i] == triangle_to_kill[i + 1])
        {
            triangle_to_kill.Remove(i);
            dir = -1;
        }
        if (triangle_to_kill[i] > triangle_to_kill[i + 1])
        {
            triangle_to_kill[i]     += triangle_to_kill[i + 1];
            triangle_to_kill[i + 1]  = triangle_to_kill[i] - triangle_to_kill[i + 1];
            triangle_to_kill[i]      = triangle_to_kill[i] - triangle_to_kill[i + 1];
            dir = -1;
        }
        if (i == 0 && dir == -1)
            dir = 1;
    }
    for (ptrdiff_t i = triangle_to_kill.Count() - 1; i >= 0; i--)
        m_indices.Remove(triangle_to_kill[i], 3);

    m_cursors.Last().m1 = (int)m_vert.Count();
    m_cursors.Last().m2 = (int)m_indices.Count();

    VerticesCleanup();
    //DONE for the splitting !
}

//-----------------------------------------------------------------------------
void EasyMesh::ToggleScaleWinding()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::ScaleWinding);
        return;
    }

    BD()->Toggle(MeshBuildOperation::ScaleWinding);
}

//-----------------------------------------------------------------------------
void EasyMesh::ToggleQuadWeighting()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::QuadWeighting);
        return;
    }

    BD()->Toggle(MeshBuildOperation::QuadWeighting);
}

//-----------------------------------------------------------------------------
void EasyMesh::TogglePostBuildNormal()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::PostBuildNormal);
        return;
    }

    BD()->Toggle(MeshBuildOperation::PostBuildComputeNormals);
}

//-----------------------------------------------------------------------------
void EasyMesh::ToggleVerticeNoCleanup()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::PreventVertCleanup);
        return;
    }

    BD()->Toggle(MeshBuildOperation::PreventVertCleanup);
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColor(vec4 const &color)
{
    SetCurColorA(color);
    SetCurColorB(color);
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColorA(vec4 const &color)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SetColorA);
        BD()->CmdStack() << color;
        return;
    }

    BD()->ColorA() = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColorB(vec4 const &color)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SetColorB);
        BD()->CmdStack() << color;
        return;
    }

    BD()->ColorB() = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetVertColor(vec4 const &color)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SetVertColor);
        BD()->CmdStack() << color;
        return;
    }

    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        m_vert[i].m_color = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::InternalAddVertex(vec3 const &coord)
{
    m_vert.Push(VertexData(coord, vec3(0.f, 1.f, 0.f), BD()->ColorA()));
    m_state = MeshRender::NeedConvert;
}

//-----------------------------------------------------------------------------
void EasyMesh::AddDuplicateVertex(int i)
{
    m_vert << m_vert[i];
    m_state = MeshRender::NeedConvert;
}

//-----------------------------------------------------------------------------
void EasyMesh::AddLerpVertex(int i, int j, float alpha)
{
    AddLerpVertex(m_vert[i], m_vert[j], alpha);

}
//-----------------------------------------------------------------------------
void EasyMesh::AddLerpVertex(VertexData const &vi, VertexData const &vj, float alpha)
{
    m_vert.Push(GetLerpVertex(vi, vj, alpha));
    m_state = MeshRender::NeedConvert;
}

//-----------------------------------------------------------------------------
VertexData EasyMesh::GetLerpVertex(int i, int j, float alpha)
{
    return GetLerpVertex(m_vert[i], m_vert[j], alpha);
}

//-----------------------------------------------------------------------------
VertexData EasyMesh::GetLerpVertex(VertexData const &vi, VertexData const &vj, float alpha)
{
    return VertexData(
        lol::lerp(vi.m_coord,    vj.m_coord,      alpha),
        lol::lerp(vi.m_normal,   vj.m_normal,     alpha),
        lol::lerp(vi.m_color,    vj.m_color,      alpha),
        lol::lerp(vi.m_texcoord, vj.m_texcoord,   alpha),
        ((alpha < .5f) ? (vi.m_bone_id) : (vj.m_bone_id)), /* FIXME ? */
        lol::lerp(vi.m_bone_weight, vj.m_bone_weight, alpha));
}

//-----------------------------------------------------------------------------
void EasyMesh::InternalAddQuad(int i1, int i2, int i3, int i4, int base)
{
    if (BD()->IsEnabled(MeshBuildOperation::QuadWeighting) &&
        !BD()->IsEnabled(MeshBuildOperation::IgnoreQuadWeighting))
    {
        int i5 = (int)m_vert.Count();
        AddLerpVertex(GetLerpVertex(base + i1, base + i3, .5f),
                      GetLerpVertex(base + i2, base + i4, .5f), .5f);
        m_indices << i1 + base;
        m_indices << i2 + base;
        m_indices << i5;

        m_indices << i2 + base;
        m_indices << i3 + base;
        m_indices << i5;

        m_indices << i4 + base;
        m_indices << i1 + base;
        m_indices << i5;

        m_indices << i5;
        m_indices << i3 + base;
        m_indices << i4 + base;
    }
    else
    {
        m_indices << base + i1;
        m_indices << base + i2;
        m_indices << base + i3;

        m_indices << base + i4;
        m_indices << base + i1;
        m_indices << base + i3;
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::InternalAddQuadDupVerts(int i1, int i2, int i3, int i4, int base)
{
    int vbase = (int)m_vert.Count();
    AddDuplicateVertex(base + i1);
    AddDuplicateVertex(base + i2);
    AddDuplicateVertex(base + i3);
    AddDuplicateVertex(base + i4);

    InternalAddQuad(0, 1, 2, 3, vbase);
}

//-----------------------------------------------------------------------------
void EasyMesh::InternalAddTriangle(int i1, int i2, int i3, int base)
{
    m_indices << base + i1;
    m_indices << base + i2;
    m_indices << base + i3;
}

//-----------------------------------------------------------------------------
void EasyMesh::InternalAddTriangleDupVerts(int i1, int i2, int i3, int base)
{
    m_indices << (uint16_t)m_vert.Count(); AddDuplicateVertex(base + i1);
    m_indices << (uint16_t)m_vert.Count(); AddDuplicateVertex(base + i2);
    m_indices << (uint16_t)m_vert.Count(); AddDuplicateVertex(base + i3);
}

//-----------------------------------------------------------------------------
void EasyMesh::ComputeNormals(int start, int vcount)
{

    if (BD()->IsEnabled(MeshBuildOperation::CommandExecution) &&
        BD()->IsEnabled(MeshBuildOperation::PostBuildComputeNormals))
        return;

    array< array<vec3> > normals;
    normals.Resize(m_vert.Count());
    for (int i = 0; i < vcount; i += 3)
    {
        vec3 v0 = m_vert[m_indices[start + i + 2]].m_coord
                - m_vert[m_indices[start + i + 0]].m_coord;
        vec3 v1 = m_vert[m_indices[start + i + 1]].m_coord
                - m_vert[m_indices[start + i + 0]].m_coord;
        vec3 n = normalize(cross(v1, v0));

        for (int j = 0; j < 3; j++)
            normals[m_indices[start + i + j]] << n;
    }

    for (int i = 0; i < normals.Count(); i++)
    {
        if (normals[i].Count() > 0)
        {
            //remove doubles
            for (int j = 0; j < normals[i].Count(); ++j)
                for (int k = j + 1; k < normals[i].Count(); ++k)
                    if (1.f - dot(normals[i][k], normals[i][j]) < .00001f)
                        normals[i].Remove(k--);

            vec3 newv = vec3::zero;
            for (int j = 0; j < normals[i].Count(); ++j)
                newv += normals[i][j];
            m_vert[i].m_normal = normalize(newv / (float)normals[i].Count());
        }
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::VerticesCleanup()
{
    array<int> vert_ids;
    vert_ids.Resize(m_vert.Count(), 0);

    //1: Remove triangles with two vertices on each other
    for (int i = 0; i < m_indices.Count(); i += 3)
    {
        bool remove = false;
        for (int j = 0; !remove && j < 3; ++j)
            if (length(m_vert[m_indices[i + j]].m_coord - m_vert[m_indices[i + (j + 1) % 3]].m_coord) < .00001f)
                remove = true;
        if (remove)
        {
            m_indices.RemoveSwap(i, 3);
            i -= 3;
        }
        else
        {
            //1.5: Mark all used vertices
            for (int j = 0; j < 3; ++j)
                vert_ids[m_indices[i + j]] = 1;
        }
    }

    //2: Remove all unused vertices
    array<VertexData> old_vert = m_vert;
    int shift = 0;
    m_vert.Empty();
    for (int i = 0; i < vert_ids.Count(); ++i)
    {
        //Unused vertex, update the shift quantity instead of keeping it.
        if (vert_ids[i] == 0)
            shift++;
        else
            m_vert << old_vert[i];
        //Always mark it with the shift quantity
        vert_ids[i] = shift;
    }

    //3: Update the indices
    for (int i = 0; i < m_indices.Count(); ++i)
        m_indices[i] -= vert_ids[m_indices[i]];
}

//-----------------------------------------------------------------------------
void EasyMesh::VerticesMerge()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::VerticesMerge);
        return;
    }

    //1: Crunch all vertices in the dictionnary
    VertexDictionnary vert_dict;
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        vert_dict.RegisterVertex(i, m_vert[i].m_coord);

    //2: Update the indices
    for (int i = 0; i < m_indices.Count(); ++i)
    {
        int master = vert_dict.FindVertexMaster(m_indices[i]);
        if (master >= 0)
            m_indices[i] = master;
    }

    //2: Cleanup
    VerticesCleanup();
}

//-----------------------------------------------------------------------------
void EasyMesh::VerticesSeparate()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::VerticesSeparate);
        return;
    }

    array< array<int> > new_ids;
    array<int> vert_ids;
    vert_ids.Resize(m_vert.Count(), 0);

    //1: Mark all used vertices
    for (int i = 0; i < m_indices.Count(); ++i)
        vert_ids[m_indices[i]]++;

    //2: Update the vertices
    int vbase = m_cursors.Last().m1;
    int vcount = (int)m_vert.Count();
    new_ids.Resize(vcount);
    for (int i = vbase; i < vcount; i++)
    {
        while (vert_ids[i] > 1)
        {
            //Add duplicate
            new_ids[i] << (int)m_vert.Count();
            AddDuplicateVertex(i);
            vert_ids[i]--;
        }
    }

    //3: Update the indices
    for (int i = 0; i < m_indices.Count(); ++i)
    {
        if (new_ids[m_indices[i]].Count())
        {
            int j = new_ids[m_indices[i]].Pop();
            m_indices[i] = j;
        }
    }

    //4: Cleanup
    VerticesCleanup();
}

//-----------------------------------------------------------------------------
void EasyMesh::ComputeTexCoord(float uv_scale, int uv_offset)
{
    UNUSED(uv_scale, uv_offset);
#if 0
    VertexDictionnary vert_dict;
    array<int> tri_list;

    tri_list.Reserve(m_indices.Count() - m_cursors.Last().m2);
    for (int i = m_cursors.Last().m2; i < m_indices.Count(); i++)
    {
        vert_dict.RegisterVertex(m_indices[i], m_vert[m_indices[i]].m_coord);
        tri_list << m_indices[i];
    }

    //full triangle count
    array<int> tri_done;
    array<int> tri_check;
    int tri_count = (m_indices.Count() - m_cursors.Last().m2) / 3;

    tri_check << tri_list[0];

    while (tri_check.Count())
    {
        int cur_tri = tri_check[0];
        int v[3]   = { tri_list[cur_tri + uv_offset % 3], tri_list[cur_tri + (1 + uv_offset) % 3], tri_list[cur_tri + (2 + uv_offset) % 3] };
        vec2 uv[3] = { m_vert[tri_list[cur_tri]].m_texcoord.xy, m_vert[tri_list[cur_tri + 1]].m_texcoord.xy, m_vert[tri_list[cur_tri + 2]].m_texcoord.xy };
        for (int j = 0; j < 3; j++)
        {
            if (uv[j] != vec2(-1.0f) && uv[j] == uv[(j + 1) % 3])
            {
                uv[0] = vec2(-1.0f);
                uv[1] = vec2(-1.0f);
                uv[2] = vec2(-1.0f);
                break;
            }
        }
        int uv_set = 0;
        for (int j = 0; j < 3; j++)
            uv_set += (uv[j].x < 0.f)?(0):(1);

        //this case shouldn't happen.
        if (uv_set == 1)
        {
            /*
            for (int j = 0; j < 3; j++)
            {
                if (uv[j] != vec2(-1.0f))
                {
                    uv[(j + 1) % 2] = uv[j] + vec2(.0f, uv_scale * length(m_vert[v[j]].m1 - m_vert[v[(j + 1) % 3]].m1));
                    uv_set = 2;
                    break;
                }
            }
            */
        }
        //No UV is set, let's do the arbitrary set and use the basic method.
        if (uv_set == 0)
        {
            float new_dot = FLT_MAX;
            int base_i = 0;
            for (int j = 0; j < 3; j++)
            {
                float tmp_dot = abs(dot(normalize(m_vert[v[(j + 1) % 3]].m_coord - m_vert[v[j]].m_coord),
                                        normalize(m_vert[v[(j + 2) % 3]].m_coord - m_vert[v[j]].m_coord)));
                if (tmp_dot < new_dot)
                {
                    base_i = j;
                    new_dot = tmp_dot;
                }
            }
            uv[base_i] = vec2(.0f);
            uv[(base_i + 1) % 3] = vec2(.0f, uv_scale * length(m_vert[v[base_i]].m_coord - m_vert[v[(base_i + 1) % 3]].m_coord));
            uv_set = 2;
        }
        //2 points have been set, let's figure the third
        if (uv_set == 2)
        {
            {
                //invert values so the two set uv are in [0, 1] slots.
                int new_v[3];
                vec2 new_uv[3];
                bool ignore_set = false;
                if (uv[0].x >= 0.f && uv[1].x < 0.f)
                {
                    new_v[0] = v[2]; new_v[1] = v[0]; new_v[2] = v[1];
                    new_uv[0] = uv[2]; new_uv[1] = uv[0]; new_uv[2] = uv[1];
                }
                else if (uv[0].x < 0.f && uv[1].x >= 0.f)
                {
                    new_v[0] = v[1]; new_v[1] = v[2]; new_v[2] = v[0];
                    new_uv[0] = uv[1]; new_uv[1] = uv[2]; new_uv[2] = uv[0];
                }
                else
                    ignore_set = true;
                if (!ignore_set)
                {
                    v[0]  = new_v[0];  v[1]  = new_v[1];  v[2]  = new_v[2];
                    uv[0] = new_uv[0]; uv[1] = new_uv[1]; uv[2] = new_uv[2];
                }
            }

            //Do this to be sure the normal is OK.
            ComputeNormals(cur_tri, 3);
            vec3 v01 = normalize(m_vert[v[1]].m_coord - m_vert[v[0]].m_coord);
            vec3 v02 = m_vert[v[2]].m_coord - m_vert[v[0]].m_coord;
            vec3 v_dir = normalize(cross(m_vert[m_indices[cur_tri]].m_normal, v01));
            vec2 texu_dir = uv[1] - uv[0];
            vec2 texv_dir = vec2(texu_dir.y, texu_dir.x);
            //Final calculations
            uv[2] = texu_dir * dot(v01, v02) + texv_dir * dot(v_dir, v02);

            //Set UV on ALL matching vertices!
            array<int> matching_vert;
            for (int i = 0; i < 3; i++)
            {
#if 1
                //This marks all same position UV to the same values
                //Deactivation is a test.
                matching_vert << v[i];
                vert_dict.FindMatchingVertices(v[i], matching_vert);
                for (int j = 0; j < matching_vert.Count(); j++)
                    if (m_vert[matching_vert[j]].m_texcoord.xy == vec2(-1.0f))
                        m_vert[matching_vert[j]].m_texcoord = vec4(abs(uv[i]), m_vert[matching_vert[j]].m_texcoord.zw);
#else
                m_vert[v[i]].m_texcoord = abs(uv[i]);
#endif
            }

            tri_done << cur_tri;
            tri_check.Remove(0);

            //Get connected triangles and go from there.
            for (int j = 0; j < 3; j++)
            {
#if 1
                //This finds triangle that are connected to this triangle
                vert_dict.FindConnectedTriangles(ivec2(v[j], v[(j + 1) % 3]), tri_list, tri_check, &tri_done);
#else
                //This finds triangle that are connected to the vertices of this triangle
                vert_dict.FindConnectedTriangles(v[j], tri_list, tri_check, &tri_done);
#endif
            }
        }
        else if (uv_set == 3)
        {
            for (int j = 0; j < 3; j++)
            {
                m_vert[tri_list[cur_tri]].m_texcoord     = vec4(vec2(-1.0f), m_vert[tri_list[cur_tri]].m_texcoord.zw);
                m_vert[tri_list[cur_tri + 1]].m_texcoord = vec4(vec2(-1.0f), m_vert[tri_list[cur_tri + 1]].m_texcoord.zw);
                m_vert[tri_list[cur_tri + 2]].m_texcoord = vec4(vec2(-1.0f), m_vert[tri_list[cur_tri + 2]].m_texcoord.zw);
            }

            //uv[0] = vec2(-1.0f);
            //uv[1] = vec2(-1.0f);
            //uv[2] = vec2(-1.0f);
            /*
            bool tri_present = false;
            for (int j = 0; j < tri_done.Count(); j++)
                if (cur_tri == tri_done[j])
                    tri_present = true;
            if (!tri_present)
                tri_done << cur_tri;
            tri_check.Remove(0);
            */
        }

        if (tri_check.Count() == 0 && tri_done.Count() != tri_count)
        {
            //look for unset triangle
            for (int i = 0; !tri_check.Count() && i < tri_list.Count(); i += 3)
            {
                bool tri_present = false;
                for (int j = 0; j < tri_done.Count(); j++)
                    if (i == tri_done[j])
                        tri_present = true;
                if (!tri_present)
                    tri_check << i;
            }
        }
    }
#endif
}

//-----------------------------------------------------------------------------
void EasyMesh::SetTexCoordData(vec2 const &new_offset, vec2 const &new_scale)
{
    BD()->TexCoordOffset() = new_offset;
    BD()->TexCoordScale() = new_scale;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetTexCoordData2(vec2 const &new_offset, vec2 const &new_scale)
{
    BD()->TexCoordOffset2() = new_offset;
    BD()->TexCoordScale2() = new_scale;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurVertNormal(vec3 const &normal)
{
    m_vert[m_vert.Count() - 1].m_normal = normal;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurVertColor(vec4 const &color)
{
    m_vert[m_vert.Count() - 1].m_color = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurVertTexCoord(vec2 const &texcoord)
{
    m_vert[m_vert.Count() - 1].m_texcoord = vec4(texcoord, m_vert[m_vert.Count() - 1].m_texcoord.zw);
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurVertTexCoord2(vec2 const &texcoord)
{
    m_vert[m_vert.Count() - 1].m_texcoord = vec4(m_vert[m_vert.Count() - 1].m_texcoord.xy, texcoord);
}

} /* namespace lol */
