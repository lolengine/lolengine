//
// EasyMesh-Internal: The code belonging to internal operations
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
void EasyMesh::AddVertex(vec3 const &coord)
{
    m_vert.Push(VertexData(coord, vec3(0.f, 1.f, 0.f), BD()->ColorA()));
    m_state = MeshRender::NeedConvert;
}

//-----------------------------------------------------------------------------
void EasyMesh::AddDupVertex(int i)
{
    m_vert << m_vert[i];
    m_state = MeshRender::NeedConvert;
}

//-----------------------------------------------------------------------------
void EasyMesh::AddLerpVertex(int i, int j, float alpha) { AddLerpVertex(m_vert[i], m_vert[j], alpha); }
void EasyMesh::AddLerpVertex(VertexData const &vi, VertexData const &vj, float alpha)
{
    m_vert.Push(GetLerpVertex(vi, vj, alpha));
    m_state = MeshRender::NeedConvert;
}

//-----------------------------------------------------------------------------
VertexData EasyMesh::GetLerpVertex(int i, int j, float alpha) { return GetLerpVertex(m_vert[i], m_vert[j], alpha); }
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
void EasyMesh::AddQuad(int i1, int i2, int i3, int i4, int base, bool duplicate)
{
    if (duplicate)
    {
        int vbase = (int)m_vert.Count();
        AddDupVertex(base + i1);
        AddDupVertex(base + i2);
        AddDupVertex(base + i3);
        AddDupVertex(base + i4);

        AddQuad(0, 1, 2, 3, vbase);
    }
    else
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
            m_indices << i1 + base;
            m_indices << i2 + base;
            m_indices << i3 + base;

            m_indices << i4 + base;
            m_indices << i1 + base;
            m_indices << i3 + base;
        }
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::AddTriangle(int i1, int i2, int i3, int base, bool duplicate)
{
    if (duplicate)
    {
        m_indices << (uint16_t)m_vert.Count(); AddDupVertex(base + i1);
        m_indices << (uint16_t)m_vert.Count(); AddDupVertex(base + i2);
        m_indices << (uint16_t)m_vert.Count(); AddDupVertex(base + i3);
    }
    else
    {
        m_indices << base + i1;
        m_indices << base + i2;
        m_indices << base + i3;
    }
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
            AddDupVertex(i);
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
