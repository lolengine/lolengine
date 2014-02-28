//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2010-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The EasyMesh class
// ------------------
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

namespace lol
{

//-- Face stuff
void PrimitiveFace::AddPolygon(vec3 v0, vec3 v1, vec3 v2)
{
    vec3 vp[3] = { v0, v1, v2 };
    int  vi[3];

    //Add vertices, if needed
    for (int i = 0; i < 3; ++i)
    {
        vi[i] = FindVert(vp[i]);
        if (vi[i] == -1)
            vi[i] = AddVert(vp[i]);
    }

    //Add edges, if needed
    for (int j = 0; j < 3; ++j)
    {
        bool found = false;
        for (int i = 0; !found && i < m_edges.Count(); ++i)
        {
            if ((vi[j] == m_edges[i].m1 && vi[(j + 1) % 3] == m_edges[i].m2) ||
                (vi[j] == m_edges[i].m2 && vi[(j + 1) % 3] == m_edges[i].m1))
            {
                found = true;
                m_edges[i].m3 = PrimitiveEdge::OriginalFull;
            }
        }
        if (!found)
            m_edges.Push(vi[j], vi[(j + 1) % 3], PrimitiveEdge::OriginalHalf);
    }
}

//--
void PrimitiveFace::CleanEdges()
{
    Array<int> vert_num;

    vert_num.Resize(m_vertices.Count());
    //Check the vertices useage first
    for (int i = 0; i < m_edges.Count(); ++i)
    {
        vert_num[EI0(i)]++;
        vert_num[EI1(i)]++;
    }
    //Remove the full edges
    for (int i = 0; i < m_edges.Count(); ++i)
    {
        if (m_edges[i].m3 == PrimitiveEdge::OriginalFull)
        {
            vert_num[EI0(i)] -= 1;
            vert_num[EI1(i)] -= 1;
            m_edges.Remove(i--);
        }
    }

    //Reorder edges
    for (int i = 0; i < m_edges.Count() - 1; ++i)
    {
        int starter_i;
        //Find the starters
        for (int j = i + 1; j < m_edges.Count(); ++j)
        {
            if (EI1(i) == EI1(j))
                Swap(m_edges[j].m1, m_edges[j].m2);
            if (EI1(i) == EI0(j))
            {
                if (i + 1 != j)
                    m_edges.Swap(i + 1, j);
                starter_i = i;
                break;
            }
        }

        //Change order to be ok with normal
        if (dot(cross(EV0(i) - EV1(i), EV1(i + 1) - EV0(i + 1)), m_normal) < 0.f)
        {
            m_edges.Swap(i, i + 1);
            Swap(m_edges[i].m1, m_edges[i].m2);
            Swap(m_edges[i + 1].m1, m_edges[i + 1].m2);
        }

        //Go on with the sorting
        if (++i < m_edges.Count() - 1)
        {
            for (int j = i + 1; j < m_edges.Count(); ++j)
            {
                if (EI1(i) == EI1(j))
                    Swap(m_edges[j].m1, m_edges[j].m2);
                if (EI1(i) == EI0(j))
                {
                    if (i + 1 != j)
                        m_edges.Swap(i + 1, j);
                    //We encountered a loop, push i toward the next potential edge
                    if (EI1(i + 1) == EI0(starter_i))
                    {
                        i += 2;
                        break;
                    }
                    //Or, we're at the end
                    else if (++i >= m_edges.Count() - 1)
                        break;
                }
            }
        }
    }

    //Remove same direction edges
    int starter_i = 0;
    for (int i = 0; i < m_edges.Count() - 1; ++i)
    {
        int ia = i;
        int ib = i + 1;
        if (EI1(ia) == EI0(starter_i))
            ib = starter_i;

        if (EI1(ia) == EI0(ib) &&
            dot(normalize(EV1(ia) - EV0(ia)), normalize(EV1(ib) - EV0(ib))) == 1.f)
        {
            vert_num[EI1(ia)] -= 2;
            m_edges[ib].m1 = m_edges[ia].m1;
            m_edges.Remove(ia);
            i--;
            if (EI1(ia) == EI0(starter_i))
            {
                i += 2;
                starter_i = i;
            }
        }
    }

    //Build the new indices
    int rem = 0;
    for (int i = 0; i < vert_num.Count(); ++i)
    {
        if (vert_num[i] != 2)
        {
            rem++;
            vert_num[i] = -1;
        }
        else
            vert_num[i] = i - rem;
    }

    //Apply the new indices
    for (int i = 0; rem > 0 && i < m_edges.Count(); ++i)
    {
        m_edges[i].m1 = vert_num[EI0(i)];
        m_edges[i].m2 = vert_num[EI1(i)];
    }

    //Remove the surnumeral vertices
    for (int i = m_vertices.Count() - 1; i >= 0; --i)
        if (vert_num[i] == -1)
            m_vertices.Remove(i);
}

//Edge functions
int PrimitiveFace::FindVert(vec3 vertex)
{
    for (int i = 0; i < m_vertices.Count(); ++i)
        if (length(m_vertices[i] - vertex) < .00001f)
            return i;
    return -1;
}

//--
int PrimitiveFace::AddVert(vec3 vertex)
{
    m_vertices.Push(vertex);
    return m_vertices.Count() - 1;
}

//--
void PrimitiveMesh::AddPolygon(vec3 v0, vec3 v1, vec3 v2, vec3 normal)
{
    for (int i = 0; i < m_faces.Count(); ++i)
    {
        vec3 proj_point = ProjectPointOnPlane(v0, m_faces[i].GetCenter(), m_faces[i].GetNormal());
        //Found the same face
        if (dot(normal, m_faces[i].GetNormal()) == 1.f && length(proj_point - v0) == .0f)
        {
            m_faces[i].AddPolygon(v0, v1, v2);
            return;
        }
    }
    //Didn't find a thing, so add .....
    m_faces.Resize(m_faces.Count() + 1);
    m_faces.Last().SetCenter(v0);
    m_faces.Last().SetNormal(normal);
    m_faces.Last().AddPolygon(v0, v1, v2);
}

//--
void PrimitiveMesh::CleanFaces()
{
    for (int i = 0; i < m_faces.Count(); ++i)
        m_faces[i].CleanEdges();
}

//--
int CsgBsp::AddLeaf(int leaf_type, vec3 origin, vec3 normal, int above_idx)
{
    if (leaf_type > 2 && leaf_type < -1)
        return -1;

    if ((m_tree.Count() == 0 && above_idx == -1) ||
        (above_idx >= 0 &&
            above_idx < m_tree.Count() &&
            leaf_type > LEAF_CURRENT &&
            leaf_type < LEAF_ABOVE &&
            m_tree[above_idx].m_leaves[leaf_type] == -1))
    {
        if (m_tree.Count() != 0)
            m_tree[above_idx].m_leaves[leaf_type] = m_tree.Count();
        m_tree.Push(CsgBspLeaf(origin, normal, above_idx));
        return m_tree.Count() - 1;
    }

    return -1;
}

int CsgBsp::TestPoint(int leaf_idx, vec3 point)
{
    if (leaf_idx >= 0 && leaf_idx < m_tree.Count())
    {
        vec3 p2o = point - m_tree[leaf_idx].m_origin;

        if (length(p2o) < CSG_EPSILON)
            return LEAF_CURRENT;

        float p2o_dot = dot(normalize(p2o), m_tree[leaf_idx].m_normal);

        if (p2o_dot > CSG_EPSILON)
            return LEAF_FRONT;
        else if (p2o_dot < -CSG_EPSILON)
            return LEAF_BACK;
    }
    return LEAF_CURRENT;
}

void CsgBsp::AddTriangleToTree(int const &tri_idx, vec3 const &tri_p0, vec3 const &tri_p1, vec3 const &tri_p2)
{
    //<Leaf_Id, v0, v1, v2>
    Array< int, vec3, vec3, vec3 > tri_to_process;
    //<FW/BW, Leaf_Id, v0, v1, v2, twin_leaf>
    Array< int, int, vec3, vec3, vec3, int > Leaf_to_add;

    //Tree is empty, so this leaf is the first
    if (m_tree.Count() == 0)
    {
        AddLeaf(LEAF_CURRENT, tri_p0, cross(normalize(tri_p1 - tri_p0), normalize(tri_p2 - tri_p1)), LEAF_CURRENT);
        m_tree.Last().m_tri_list.Push(tri_idx, tri_p0, tri_p1, tri_p2);
        return;
    }

    tri_to_process.Reserve(20);
    tri_to_process.Push(0, tri_p0, tri_p1, tri_p2);

    while (tri_to_process.Count())
    {
        int leaf_idx = tri_to_process.Last().m1;
        vec3 v[3] = { tri_to_process.Last().m2, tri_to_process.Last().m3, tri_to_process.Last().m4 };

        int res_nb[3] = { 0, 0, 0 };
        int res_side[3] = { -1, -1, -1 };

        //Check where each point is located
        for (int i = 0; i < 3; i++)
        {
            int result = TestPoint(leaf_idx, v[i]);
            if (result != LEAF_CURRENT)
            {
                res_nb[result]++;
                res_side[i] = result;
            }
        }

        //Points are located on each sides, let's do the mumbo-jumbo
        if (res_nb[LEAF_BACK] && res_nb[LEAF_FRONT])
        {
            //there are two intersections, no more.
            vec3 isec_v[2] = { vec3(.0f), vec3(.0f) };
            int isec_i[2] = { 0, 0 };
            int isec_base = 0;
            int isec_idx = 0;

            for (int i = 0; i < 3; i++)
            {
                vec3 ray = v[(i + 1) % 3] - v[i];

                if (RayIsectPlane(v[i], v[(i + 1) % 3],
                                    m_tree[leaf_idx].m_origin, m_tree[leaf_idx].m_normal,
                                    isec_v[isec_idx]))
                    isec_i[isec_idx++] = i;
                else
                    isec_base = i;
            }

            int v_idx0 = (isec_base == 1)?(1):(0);
            int v_idx1 = (isec_base == 1)?(0):(1);
            int leaf_type = res_side[(isec_base + 2) % 3];

            tri_to_process.Pop();

#if 1
            if (m_tree[leaf_idx].m_leaves[leaf_type] == LEAF_CURRENT)
                Leaf_to_add.Push(leaf_type, leaf_idx, v[((isec_base + 2) % 3)], isec_v[v_idx1], isec_v[v_idx0], -1);
            else
                tri_to_process.Push(leaf_idx, v[((isec_base + 2) % 3)], isec_v[v_idx1], isec_v[v_idx0]);

            if (m_tree[leaf_idx].m_leaves[1 - leaf_type] == LEAF_CURRENT)
            {
                Leaf_to_add.Push(1 - leaf_type, leaf_idx, v[isec_base], v[((isec_base + 1) % 3)], isec_v[v_idx0], -1);
                Leaf_to_add.Push(1 - leaf_type, leaf_idx, v[isec_base], isec_v[v_idx0], isec_v[v_idx1], Leaf_to_add.Count() - 1);
            }
            else
            {
                tri_to_process.Push(m_tree[leaf_idx].m_leaves[1 - leaf_type], v[isec_base], v[((isec_base + 1) % 3)], isec_v[v_idx0]);
                tri_to_process.Push(m_tree[leaf_idx].m_leaves[1 - leaf_type], v[isec_base], isec_v[v_idx0], isec_v[v_idx1]);
            }
#else
            vec3 new_v[9] = { v[((isec_base + 2) % 3)], isec_v[v_idx1],             isec_v[v_idx0],
                                v[isec_base],             v[((isec_base + 1) % 3)],   isec_v[v_idx0],
                                v[isec_base],             isec_v[v_idx0],             isec_v[v_idx1] };

            //Error check : Skip the triangle where two points are on the same location.
            //it fixes the problem of having an intersection with one of the isec-point being on one of the triangle vertices.
            //(the problem being a very funny infinite loop)
            for(int k = 0; k < 9; k += 3)
            {
                bool skip_tri = false;
                for(int l = 0; l < 3; l++)
                {
                    if (length(new_v[k + l] - new_v[k + (l + 1) % 3]) < CSG_EPSILON)
                    {
                        skip_tri = true;
                        break;
                    }
                }

                if (skip_tri)
                    continue;

                tri_to_process.Push(0, new_v[k], new_v[k + 1], new_v[k + 2]);
            }
#endif
        }
        //All points are on one side, transfer to the next leaf
        else if (res_nb[LEAF_BACK] || res_nb[LEAF_FRONT])
        {
            int new_leaf_type = ((res_nb[LEAF_FRONT])?(LEAF_FRONT):(LEAF_BACK));
            int new_leaf = m_tree[leaf_idx].m_leaves[new_leaf_type];

            //No leaf exist, so add a new one
            if (new_leaf == LEAF_CURRENT)
            {
                tri_to_process.Pop();
                Leaf_to_add.Push(new_leaf_type, leaf_idx, v[0], v[1], v[2], -1);
            }
            else
                tri_to_process.Last().m1 = new_leaf;
        }
        //All points are on the current leaf, add the tri_idx to the list of this leaf.
        else
        {
            tri_to_process.Pop();

            bool already_exist = false;
            for (int i = 0; !already_exist && i < m_tree[leaf_idx].m_tri_list.Count(); i++)
                already_exist = (m_tree[leaf_idx].m_tri_list[i].m1 == tri_idx);
            if (!already_exist)
                m_tree[leaf_idx].m_tri_list.Push(tri_idx, tri_p0, tri_p1, tri_p2);
        }
    }

    //Add all leaves to the tree.
    for (int i = 0; i < Leaf_to_add.Count(); i++)
    {
        //If we had it to an already existing leaf.
        if (Leaf_to_add[i].m2 < m_tree.Count() && m_tree[Leaf_to_add[i].m2].m_leaves[Leaf_to_add[i].m1] == LEAF_CURRENT)
        {
            AddLeaf(Leaf_to_add[i].m1, tri_p0, cross(normalize(tri_p1 - tri_p0), normalize(tri_p2 - tri_p1)), Leaf_to_add[i].m2);
            m_tree.Last().m_tri_list.Push(tri_idx, tri_p0, tri_p1, tri_p2);
        }

        /*
        if (Leaf_to_add[i].m6 == -1)
        {
            AddLeaf(Leaf_to_add[i].m1, tri_p0, cross(normalize(tri_p1 - tri_p0), normalize(tri_p2 - tri_p1)), Leaf_to_add[i].m2);
            m_tree.Last().m_tri_list.Push(tri_idx, tri_p0, tri_p1, tri_p2);
        }
        else
            m_tree[Leaf_to_add[i].m6].m_tri_list.Push(tri_idx, tri_p0, tri_p1, tri_p2);
        */
    }
}

//return 0 when no split has been done.
//return 1 when split has been done.
//return -1 when error.
int CsgBsp::TestTriangleToTree(vec3 const &tri_p0, vec3 const &tri_p1, vec3 const &tri_p2,
                               //In order to easily build the actual vertices list afterward, this list stores each Vertices location and its source vertices & Alpha.
                               //<Point_Loc, Src_V0, Src_V1, Alpha> as { Point_Loc = Src_V0 + (Src_V1 - Src_V0) * Alpha; }
                               Array< vec3, int, int, float > &vert_list,
                               //This is the final triangle list : If Side_Status is LEAF_CURRENT, a new test will be done point by point.
                               //<{IN|OUT}side_status, v0, v1, v2>
                               Array< int, int, int, int > &tri_list)
{
    //This list stores the current triangles to process.
    //<Leaf_Id_List, v0, v1, v2, Should_Point_Test>
    Array< Array< int >, int, int, int, int > tri_to_process;

    //Tree is empty, ABORT !
    if (m_tree.Count() == 0)
        return -1;

    //Let's push the source vertices in here.
    vert_list.Push(tri_p0, -1, -1, .0f);
    vert_list.Push(tri_p1, -1, -1, .0f);
    vert_list.Push(tri_p2, -1, -1, .0f);

    //Let's push the triangle in here.
    tri_to_process.Reserve(20);
    tri_to_process.Push( Array< int >(), 0, 1, 2, 0);
    tri_to_process.Last().m1.Push(0);

    while (tri_to_process.Count())
    {
        while (tri_to_process.Count())
        {
            int leaf_idx = tri_to_process.Last().m1.Last();
            int t[3] = { tri_to_process.Last().m2,
                            tri_to_process.Last().m3,
                            tri_to_process.Last().m4 };
            vec3 v[3] = { vert_list[t[0]].m1,
                            vert_list[t[1]].m1,
                            vert_list[t[2]].m1 };

            int res_nb[3] = { 0, 0, 0 };
            int res_side[3] = { -1, -1, -1 };
            //Check where each point is located
            for (int i = 0; i < 3; i++)
            {
                int result = TestPoint(leaf_idx, v[i]);
                if (result != LEAF_CURRENT)
                {
                    res_nb[result]++;
                    res_side[i] = result;
                }
            }

            //Points are located on each sides, let's do the mumbo-jumbo
            if (res_nb[LEAF_BACK] && res_nb[LEAF_FRONT])
            {
                //there are two intersections, no more.
                vec3 isec_v[2] = { vec3(.0f), vec3(.0f) };
                int isec_i[2] = { 0, 0 };
                int new_v_idx[2] = { 0, 0 };
                int isec_base = 0;
                int isec_idx = 0;

                int i = 0;
                for (; i < m_tree[leaf_idx].m_tri_list.Count(); i++)
                {
                    if (TestTriangleVsTriangle(v[0], v[1], v[2],
                                                m_tree[leaf_idx].m_tri_list[i].m2, m_tree[leaf_idx].m_tri_list[i].m3, m_tree[leaf_idx].m_tri_list[i].m4,
                                                isec_v[0], isec_v[1]))
                        break;
                }

                //There was no triangle intersection, the complex case.
                if (i == m_tree[leaf_idx].m_tri_list.Count())
                {
                    if (m_tree[leaf_idx].m_leaves[LEAF_FRONT] == LEAF_CURRENT &&
                        m_tree[leaf_idx].m_leaves[LEAF_BACK] == LEAF_CURRENT &&
                        tri_to_process.Last().m1.Count() == 1)
                    {
                        tri_list.Push(LEAF_CURRENT, tri_to_process.Last().m2, tri_to_process.Last().m3, tri_to_process.Last().m4);
                        tri_to_process.Pop();
                    }
                    else
                    {
                        tri_to_process.Last().m1.Pop();

                        //Register the triangle as needing to intersect with Front & back leaves.
                        if (m_tree[leaf_idx].m_leaves[LEAF_FRONT] != LEAF_CURRENT)
                            tri_to_process.Last().m1.Push(m_tree[leaf_idx].m_leaves[LEAF_FRONT]);
                        if (m_tree[leaf_idx].m_leaves[LEAF_BACK] != LEAF_CURRENT)
                            tri_to_process.Last().m1.Push(m_tree[leaf_idx].m_leaves[LEAF_BACK]);
                        //Mark the triangle as needing point by point test

                        tri_to_process.Last().m5 = 1;
                    }
                }
                //there was an intersection, so let's split the triangle.
                else
                {
                    //Get intersection on actual triangle sides.
                    if (RayIsectTriangleSide(v[0], v[1], v[2],
                                                isec_v[0], isec_v[1],
                                                isec_v[0], isec_i[0], isec_v[1], isec_i[1]))
                    {
                        {
                            for(int k = 0; k < 2; k++)
                            {
                                if (isec_base == isec_i[k])
                                    isec_base++;

#if 1 //Skip point creation if it's on the same location a one of the triangle.
                                bool skip_point = false;
                                int l = 0;
                                for(; l < 3; l++)
                                {
                                    if (length(v[l] - isec_v[k]) < CSG_EPSILON)
                                    {
                                        skip_point = true;
                                        new_v_idx[k] = t[l];
                                        break;
                                    }
                                }

                                if (skip_point)
                                    continue;
#endif
                                new_v_idx[k] = vert_list.Count();
                                vec3 PmV0 = (isec_v[k] - vert_list[t[isec_i[k]]].m1);
                                vec3 V1mV0 = (vert_list[t[(isec_i[k] + 1) % 3]].m1 - vert_list[t[isec_i[k]]].m1);
                                float alpha = length(PmV0) / length(V1mV0);
                                vert_list.Push(isec_v[k],
                                                t[isec_i[k]], t[(isec_i[k] + 1) % 3],
                                                //Alpha = length((Point_Loc - Src_V0) / (Src_V1 - Src_V0));
                                                alpha);
                            }

                            int v_idx0 = (isec_base == 1)?(1):(0);
                            int v_idx1 = (isec_base == 1)?(0):(1);
                            //Leaf_type is the type for the triangle that is alone on its side.
                            int leaf_type = res_side[(isec_base + 2) % 3];
                            int tri_to_remove = tri_to_process.Count() - 1;

#if 0
                            if (m_tree[leaf_idx].m_leaves[leaf_type] == LEAF_CURRENT && tri_to_process.Last().m1.Last() == 1)
                                tri_list.Push(leaf_type,
                                                t[(isec_base + 2) % 3], new_v_idx[v_idx1], new_v_idx[v_idx0]);
                            else
                            {
                                tri_to_process.Push(Array< int >(), t[(isec_base + 2) % 3], new_v_idx[v_idx1], new_v_idx[v_idx0], 0);
                                tri_to_process.Last().m1.Push(0);
                            }

                            if (m_tree[leaf_idx].m_leaves[1 - leaf_type] == LEAF_CURRENT && tri_to_process.Last().m1.Last() == 1)
                            {
                                tri_list.Push((tri_to_process.Last().m5)?(LEAF_CURRENT):(1 - leaf_type),
                                                t[isec_base], new_v_idx[((isec_base + 1) % 3)], new_v_idx[v_idx0]);
                                tri_list.Push((tri_to_process.Last().m5)?(LEAF_CURRENT):(1 - leaf_type),
                                                t[isec_base], new_v_idx[v_idx0], new_v_idx[v_idx1]);
                            }
                            else
                            {
                                tri_to_process.Push(Array< int >(), t[isec_base], t[((isec_base + 1) % 3)], new_v_idx[v_idx0], 0);
                                tri_to_process.Last().m1.Push(0);
                                tri_to_process.Push(Array< int >(), t[isec_base], new_v_idx[v_idx0], new_v_idx[v_idx1], 0);
                                tri_to_process.Last().m1.Push(0);
                            }
#else
                            int new_t[9] = { t[(isec_base + 2) % 3], new_v_idx[v_idx1],         new_v_idx[v_idx0],
                                                t[isec_base],           t[((isec_base + 1) % 3)],  new_v_idx[v_idx0],
                                                t[isec_base],           new_v_idx[v_idx0],         new_v_idx[v_idx1] };
                            int new_side[3] = { res_side[(isec_base + 2) % 3],
                                                (res_side[isec_base] == LEAF_CURRENT)?(res_side[((isec_base + 1) % 3)]):(res_side[isec_base]),
                                                res_side[isec_base] };

                            //Error check : Skip the triangle where two points are on the same location.
                            //it fixes the problem of having an intersection with one of the isec-point being on one of the triangle vertices.
                            //(the problem being a very funny infinite loop)
                            for(int k = 0; k < 9; k += 3)
                            {
#if 1 //Error check
                                bool skip_tri = false;
                                for(int l = 0; l < 3; l++)
                                {
                                    if (length(vert_list[new_t[k + l]].m1 - vert_list[new_t[k + (l + 1) % 3]].m1) < CSG_EPSILON)
                                    {
                                        skip_tri = true;
                                        break;
                                    }
                                }

                                if (skip_tri)
                                    continue;
#endif
#if 0 //Send the newly created triangle back to the beginning
                                tri_to_process.Push(Array< int >(), new_t[k], new_t[k + 1], new_t[k + 2], 0);
                                tri_to_process.Last().m1.Push(0);
#else //Inherit parent tree
                                if (m_tree[leaf_idx].m_leaves[new_side[k / 3]] == LEAF_CURRENT && tri_to_process[tri_to_remove].m1.Count() == 1)
                                    tri_list.Push(new_side[k / 3], new_t[k], new_t[k + 1], new_t[k + 2]);
                                else
                                {
                                    tri_to_process.Push(Array< int >(), new_t[k], new_t[k + 1], new_t[k + 2], 0);
                                    tri_to_process.Last().m1 = tri_to_process[tri_to_remove].m1;
                                    if (m_tree[leaf_idx].m_leaves[new_side[k / 3]] == LEAF_CURRENT)
                                        tri_to_process.Last().m1.Pop();
                                    else
                                        tri_to_process.Last().m1.Last() = m_tree[leaf_idx].m_leaves[new_side[k / 3]];
                                }
#endif
                            }
#endif

                            tri_to_process.Remove(tri_to_remove);
                        }
                    }
                }
            }
            //All points are on one side, transfer to the next leaf
            else if (res_nb[LEAF_BACK] || res_nb[LEAF_FRONT])
            {
                int new_leaf_type = ((res_nb[LEAF_FRONT])?(LEAF_FRONT):(LEAF_BACK));
                int new_leaf = m_tree[leaf_idx].m_leaves[new_leaf_type];

                //No leaf exist, we're at the end
                if (new_leaf == LEAF_CURRENT)
                {
                    //We still need to test other leaves.
                    if (tri_to_process.Last().m1.Count() > 1)
                        tri_to_process.Last().m1.Pop();
                    else
                    {
                        tri_list.Push((tri_to_process.Last().m5)?(LEAF_CURRENT):(new_leaf_type),
                                        tri_to_process.Last().m2, tri_to_process.Last().m3, tri_to_process.Last().m4);
                        tri_to_process.Pop();
                    }
                }
                else
                    tri_to_process.Last().m1.Last() = new_leaf;
            }
            //All points are on the current leaf, add the tri_idx to the list of this leaf.
            else
            {
                //TODO : Special case, handle coplanar cut.
                tri_list.Push(LEAF_CURRENT, tri_to_process.Last().m2, tri_to_process.Last().m3, tri_to_process.Last().m4);
                tri_to_process.Pop();
            }
        }

        //Now that we have all the split points, let's double-check the results
        for (int i = 0; i < tri_list.Count(); i++)
        {
#define TEST_MAX 4
            int t[3] = { tri_list[i].m2,
                            tri_list[i].m3,
                            tri_list[i].m4 };
            vec3 v[4] = { vert_list[t[0]].m1,
                            vert_list[t[1]].m1,
                            vert_list[t[2]].m1,
                            (vert_list[t[0]].m1 +
                            vert_list[t[1]].m1 +
                            vert_list[t[2]].m1) / 3.0f };

            int res_total = 0;
            int res_nb[3] = { 0, 0, 0 };

            int res_Leaf[4] = { 0, 0, 0, 0 };
            int res_side[4] = { -1, -1, -1, -1 };
            while (res_total < TEST_MAX)
            {
                for (int k = 0; k < TEST_MAX; k++)
                {
                    if (res_Leaf[k] != LEAF_CURRENT)
                    {
                        int result = TestPoint(res_Leaf[k], v[k]);
                        if (result != LEAF_CURRENT)
                        {
                            res_Leaf[k] = m_tree[res_Leaf[k]].m_leaves[result];
                            res_side[k] = result;
                            if (res_Leaf[k] == LEAF_CURRENT)
                            {
                                res_total++;
                                res_nb[result]++;
                            }
                        }
                        else
                        {
                            res_Leaf[k] = LEAF_CURRENT;
                            res_side[k] = LEAF_CURRENT;
                            res_total++;
                        }
                    }
                }
            }
            int k = 0;
            if (res_nb[LEAF_BACK] && res_nb[LEAF_FRONT])
            {
                res_total = res_total;
                tri_list[i].m1 = LEAF_BACK;
#if 0
                tri_to_process.Push( Array< int >(), tri_list[i].m2, tri_list[i].m3, tri_list[i].m4, 0);
                tri_to_process.Last().m1.Push(0);
                tri_list.Remove(i--);
                break;
#endif
            }
            else
            {
                for (; k < TEST_MAX; k++)
                {
                    if (res_side[k] != LEAF_CURRENT)
                    {
                        tri_list[i].m1 = res_side[k];
                        break;
                    }
                }
                if (k == TEST_MAX)
                    tri_list[i].m1 = LEAF_FRONT;
            }
        }
    }

    if (tri_list.Count() == 1)
        return 0;
    return 1;
}

} /* namespace lol */

