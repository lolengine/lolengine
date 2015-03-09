//
// EasyMesh-Csg: The code belonging to CSG operations
//
// Copyright: (c) 2010-2015 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2015 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{

//-----------------------------------------------------------------------------
void EasyMesh::CsgUnion() { MeshCsg(CSGUsage::Union); }
void EasyMesh::CsgSub()   { MeshCsg(CSGUsage::Substract); }
void EasyMesh::CsgSubL()  { MeshCsg(CSGUsage::SubstractLoss); }
void EasyMesh::CsgAnd()   { MeshCsg(CSGUsage::And); }
void EasyMesh::CsgXor()   { MeshCsg(CSGUsage::Xor); }

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

                    AddVertex(vert_list[k].m1);

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
                    AddTriangle(P0, P1, P2, 0);
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
                                AddDupVertex(m_indices[tri_idx + l]);
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

} /* namespace lol */
