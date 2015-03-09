//
// EasyMesh-Transform: The code belonging to transform operations
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
void EasyMesh::Translate(vec3 const &v)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::Translate);
        BD()->CmdStack() << v;
        return;
    }

    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        m_vert[i].m_coord += v;
}

//-----------------------------------------------------------------------------
void EasyMesh::RotateX(float angle) { Rotate(angle, vec3(1, 0, 0)); }
void EasyMesh::RotateY(float angle) { Rotate(angle, vec3(0, 1, 0)); }
void EasyMesh::RotateZ(float angle) { Rotate(angle, vec3(0, 0, 1)); }

//-----------------------------------------------------------------------------
void EasyMesh::Rotate(float angle, vec3 const &axis)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::Rotate);
        BD()->CmdStack() << angle << axis;
        return;
    }

    mat3 m = mat3::rotate(angle, axis);
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        m_vert[i].m_coord  = m * m_vert[i].m_coord;
        m_vert[i].m_normal = m * m_vert[i].m_normal;
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::RadialJitter(float r)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::RadialJitter);
        BD()->CmdStack() << r;
        return;
    }

    array<int> Welded;
    Welded.Push(-1);
    for (int i = m_cursors.Last().m1 + 1; i < m_vert.Count(); i++)
    {
        int j, k;
        for (j = m_cursors.Last().m1, k = 0; j < i; j++, k++)
        {
            if(Welded[k] < 0)
            {
                vec3 diff = m_vert[i].m_coord - m_vert[j].m_coord;

                if(diff.x > 0.1f || diff.x < -0.1f)
                    continue;

                if(diff.y > 0.1f || diff.y < -0.1f)
                    continue;

                if(diff.z > 0.1f || diff.z < -0.1f)
                    continue;

                break;
            }
        }

        if(j == i)
            Welded.Push(-1);
        else
            Welded.Push(j);
    }

    int i, j;
    for (i = m_cursors.Last().m1, j = 0; i < m_vert.Count(); i++, j++)
    {
        if(Welded[j] == -1)
            m_vert[i].m_coord *= 1.0f + rand(r);
        else
            m_vert[i].m_coord = m_vert[Welded[j]].m_coord;
    }

    ComputeNormals(m_cursors.Last().m2, (int)m_indices.Count() - m_cursors.Last().m2);
}

//-----------------------------------------------------------------------------
void EasyMesh::TaperX(float ny, float nz, float xoff, bool absolute) { DoMeshTransform(MeshTransform::Taper, Axis::X, Axis::X, ny, nz, xoff, absolute); }
void EasyMesh::TaperY(float nx, float nz, float yoff, bool absolute) { DoMeshTransform(MeshTransform::Taper, Axis::Y, Axis::Y, nz, nx, yoff, absolute); }
void EasyMesh::TaperZ(float nx, float ny, float zoff, bool absolute) { DoMeshTransform(MeshTransform::Taper, Axis::Z, Axis::Z, nx, ny, zoff, absolute); }

//-----------------------------------------------------------------------------
void EasyMesh::TwistX(float t, float toff) { DoMeshTransform(MeshTransform::Twist, Axis::X, Axis::X, t, t, toff); }
void EasyMesh::TwistY(float t, float toff) { DoMeshTransform(MeshTransform::Twist, Axis::Y, Axis::Y, t, t, toff); }
void EasyMesh::TwistZ(float t, float toff) { DoMeshTransform(MeshTransform::Twist, Axis::Z, Axis::Z, t, t, toff); }

//-----------------------------------------------------------------------------
void EasyMesh::ShearX(float ny, float nz, float xoff, bool absolute) { DoMeshTransform(MeshTransform::Shear, Axis::X, Axis::X, ny, nz, xoff, absolute); }
void EasyMesh::ShearY(float nx, float nz, float yoff, bool absolute) { DoMeshTransform(MeshTransform::Shear, Axis::Y, Axis::Y, nz, nx, yoff, absolute); }
void EasyMesh::ShearZ(float nx, float ny, float zoff, bool absolute) { DoMeshTransform(MeshTransform::Shear, Axis::Z, Axis::Z, nx, ny, zoff, absolute); }

//-----------------------------------------------------------------------------
void EasyMesh::StretchX(float ny, float nz, float xoff) { DoMeshTransform(MeshTransform::Stretch, Axis::X, Axis::X, ny, nz, xoff); }
void EasyMesh::StretchY(float nx, float nz, float yoff) { DoMeshTransform(MeshTransform::Stretch, Axis::Y, Axis::Y, nz, nx, yoff); }
void EasyMesh::StretchZ(float nx, float ny, float zoff) { DoMeshTransform(MeshTransform::Stretch, Axis::Z, Axis::Z, nx, ny, zoff); }

//-----------------------------------------------------------------------------
void EasyMesh::BendXY(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::X, Axis::Y, t, t, toff); }
void EasyMesh::BendXZ(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::X, Axis::Z, t, t, toff); }
void EasyMesh::BendYX(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::Y, Axis::X, t, t, toff); }
void EasyMesh::BendYZ(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::Y, Axis::Z, t, t, toff); }
void EasyMesh::BendZX(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::Z, Axis::X, t, t, toff); }
void EasyMesh::BendZY(float t, float toff) { DoMeshTransform(MeshTransform::Bend, Axis::Z, Axis::Y, t, t, toff); }

//-----------------------------------------------------------------------------
void EasyMesh::DoMeshTransform(MeshTransform ct, Axis axis0, Axis axis1, float n0, float n1, float noff, bool absolute)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::MeshTranform);
        BD()->CmdStack() << ct << axis0 << axis1 << n0 << n1 << noff << absolute;
        return;
    }

    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        switch (ct.ToScalar())
        {
            case MeshTransform::Taper:
            {
                float value = m_vert[i].m_coord[axis0.ToScalar()];
                if (absolute) value = abs(value);
                m_vert[i].m_coord[(axis0.ToScalar() + 1) % 3] *= max(0.f, 1.f + (n0 * value + noff));
                m_vert[i].m_coord[(axis0.ToScalar() + 2) % 3] *= max(0.f, 1.f + (n1 * value + noff));
                break;
            }
            case MeshTransform::Twist:
            {
                vec3 rotaxis = vec3(1.f); rotaxis[(axis0.ToScalar() + 1) % 3] = .0f; rotaxis[(axis0.ToScalar() + 2) % 3] = .0f;
                m_vert[i].m_coord = mat3::rotate(m_vert[i].m_coord[axis0.ToScalar()] * n0 + noff, rotaxis) * m_vert[i].m_coord;
                break;
            }
            case MeshTransform::Shear:
            {
                float value = m_vert[i].m_coord[axis0.ToScalar()];
                if (absolute) value = abs(value);
                m_vert[i].m_coord[(axis0.ToScalar() + 1) % 3] += (n0 * value + noff);
                m_vert[i].m_coord[(axis0.ToScalar() + 2) % 3] += (n1 * value + noff);
                break;
            }
            case MeshTransform::Stretch:
            {
                //float value = abs(m_vert[i].m1[axis0.ToScalar()]);
                //m_vert[i].m1[(axis0.ToScalar() + 1) % 3] += (lol::pow(value, n0) + noff);
                //m_vert[i].m1[(axis0.ToScalar() + 2) % 3] += (lol::pow(value, n1) + noff);
                break;
            }
            case MeshTransform::Bend:
            {
                vec3 rotaxis = vec3(1.f); rotaxis[(axis1.ToScalar() + 1) % 3] = .0f; rotaxis[(axis1.ToScalar() + 2) % 3] = .0f;
                m_vert[i].m_coord = mat3::rotate(m_vert[i].m_coord[axis0.ToScalar()] * n0 + noff, rotaxis) * m_vert[i].m_coord;
                break;
            }
        }
    }
    ComputeNormals(m_cursors.Last().m2, (int)m_indices.Count() - m_cursors.Last().m2);
}

//-----------------------------------------------------------------------------
void EasyMesh::Scale(float s) { Scale(vec3(s)); }
void EasyMesh::Scale(vec3 const &s)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::Scale);
        BD()->CmdStack() << s;
        return;
    }

    vec3 const invs = vec3(1) / s;

    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        m_vert[i].m_coord *= s;
        m_vert[i].m_normal = normalize(m_vert[i].m_normal * invs);
    }

    /* Flip winding if the scaling involves mirroring */
    if (!BD()->IsEnabled(MeshBuildOperation::ScaleWinding) && s.x * s.y * s.z < 0)
    {
        for (int i = m_cursors.Last().m2; i < m_indices.Count(); i += 3)
        {
            uint16_t tmp = m_indices[i + 0];
            m_indices[i + 0] = m_indices[i + 1];
            m_indices[i + 1] = tmp;
        }
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::MirrorX() { DupAndScale(vec3(-1, 1, 1)); }
void EasyMesh::MirrorY() { DupAndScale(vec3(1, -1, 1)); }
void EasyMesh::MirrorZ() { DupAndScale(vec3(1, 1, -1)); }

//-----------------------------------------------------------------------------
void EasyMesh::DupAndScale(vec3 const &s, bool open_brace)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::DupAndScale);
        BD()->CmdStack() << s << open_brace;
        return;
    }

    int vlen = (int)m_vert.Count() - m_cursors.Last().m1;
    int tlen = (int)m_indices.Count() - m_cursors.Last().m2;

    for (int i = 0; i < vlen; i++)
        AddDupVertex(m_cursors.Last().m1++);

    for (int i = 0; i < tlen; i++)
        m_indices << m_indices[m_cursors.Last().m2++] + vlen;

    Scale(s);

    m_cursors.Last().m1 -= vlen;
    m_cursors.Last().m2 -= tlen;

    if (open_brace)
    {
        OpenBrace();

        m_cursors.Last().m1 -= vlen;
        m_cursors.Last().m2 -= tlen;
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::Chamfer(float f)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::Chamfer);
        BD()->CmdStack() << f;
        return;
    }

    int vlen = (int)m_vert.Count() - m_cursors.Last().m1;
    int ilen = (int)m_indices.Count() - m_cursors.Last().m2;

    /* Step 1: enumerate all faces. This is done by merging triangles
     * that are coplanar and share an edge. */
    int *triangle_classes = new int[ilen / 3];
    for (int i = 0; i < ilen / 3; i++)
        triangle_classes[i] = -1;

    for (int i = 0; i < ilen / 3; i++)
    {

    }

    /* Fun shit: reduce all triangles */
    int *vertices = new int[vlen];
    memset(vertices, 0, vlen * sizeof(int));
    for (int i = 0; i < ilen; i++)
        vertices[m_indices[i]]++;

    for (int i = 0; i < ilen / 3; i++)

    {
    #if 0
        if (vertices[m_indices[i * 3]] > 1)
            continue;
        if (vertices[m_indices[i * 3 + 1]] > 1)
            continue;
        if (vertices[m_indices[i * 3 + 2]] > 1)
            continue;
    #endif

        vec3 bary = 1.f / 3.f * (m_vert[m_indices[i * 3]].m_coord +
                                 m_vert[m_indices[i * 3 + 1]].m_coord +
                                 m_vert[m_indices[i * 3 + 2]].m_coord);
        for (int k = 0; k < 3; k++)
        {
            vec3 &p = m_vert[m_indices[i * 3 + k]].m_coord;
            p -= normalize(p - bary) * f;
        }
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::SplitTriangles(int pass)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SplitTriangles);
        BD()->CmdStack() << pass;
        return;
    }

    SplitTriangles(pass, nullptr);
}

//-----------------------------------------------------------------------------
void EasyMesh::SplitTriangles(int pass, VertexDictionnary *vert_dict)
{
    while (pass--)
    {
        int trimax = (int)m_indices.Count();
        for (int i = m_cursors.Last().m2; i < trimax; i += 3)
        {
            int vbase = (int)m_vert.Count();
            int j = -1;
            while (++j < 3)
            {
                AddLerpVertex(m_indices[i + j], m_indices[i + (j + 1) % 3], .5f);
                if (vert_dict)
                    vert_dict->RegisterVertex(vbase + j, m_vert[vbase + j].m_coord);
            }
            //Add new triangles
            AddTriangle(vbase, m_indices[i + 1], vbase + 1, 0);
            AddTriangle(vbase + 2, vbase + 1, m_indices[i + 2], 0);
            AddTriangle(vbase, vbase + 1, vbase + 2, 0);
            //Change current triangle
            m_indices[i + 1] = vbase;
            m_indices[i + 2] = vbase + 2;
        }
    }
    ComputeNormals(m_cursors.Last().m2, (int)m_indices.Count() - m_cursors.Last().m2);
}

//-----------------------------------------------------------------------------
//TODO : Add an half-edges implementation to refine smooth.
//TODO : Smooth should only use connected vertices that are on edges of the mesh (See box).
void EasyMesh::SmoothMesh(int main_pass, int split_per_main_pass, int smooth_per_main_pass)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SmoothMesh);
        BD()->CmdStack() << main_pass << split_per_main_pass << smooth_per_main_pass;
        return;
    }

    VertexDictionnary vert_dict;
    array<vec3> smooth_buf[2];
    array<int> master_list;
    array<int> matching_ids;
    array<int> connected_vert;
    int smbuf = 0;

    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        vert_dict.RegisterVertex(i, m_vert[i].m_coord);

    while (main_pass--)
    {
        int split_pass = split_per_main_pass;
        int smooth_pass = smooth_per_main_pass;

        SplitTriangles(split_pass, &vert_dict);

        matching_ids.Reserve(m_vert.Count() - m_cursors.Last().m1);
        connected_vert.Reserve(m_vert.Count() - m_cursors.Last().m1);
        smooth_buf[0].Resize(m_vert.Count() - m_cursors.Last().m1);
        smooth_buf[1].Resize(m_vert.Count() - m_cursors.Last().m1);

        for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
            smooth_buf[smbuf][i - m_cursors.Last().m1] = m_vert[i].m_coord;

        while (smooth_pass--)
        {
            master_list.Empty();
            if (vert_dict.GetMasterList(master_list))
            {
                for (int i = 0; i < master_list.Count(); i++)
                {
                    connected_vert.Empty();
                    if (vert_dict.FindConnectedVertices(master_list[i], m_indices, m_cursors.Last().m2, connected_vert))
                    {
                        //Calculate vertices sum
                        vec3 vert_sum = vec3(.0f);
                        for (int j = 0; j < connected_vert.Count(); j++)
                            vert_sum += smooth_buf[smbuf][connected_vert[j] - m_cursors.Last().m1];

                        //Calculate new master vertex
                        float n = (float)connected_vert.Count();
                        //b(n) = 5/4 - pow(3 + 2 * cos(2.f * F_PI / n), 2) / 32
                        float beta = 3.f + 2.f * cos(2.f * F_PI / n);
                        beta = 5.f / 4.f - beta * beta / 32.f;
                        //a(n) = n * (1 - b(n)) / b(n)
                        float alpha = (n * (1 - beta)) / beta;
                        //V = (a(n) * v + v1 + ... + vn) / (a(n) + n)
                        vec3 new_vert = (alpha * smooth_buf[smbuf][master_list[i] - m_cursors.Last().m1] + vert_sum) / (alpha + n);

                        //Set all matching vertices to new value
                        matching_ids.Empty();
                        matching_ids << master_list[i];
                        vert_dict.FindMatchingVertices(master_list[i], matching_ids);
                        for (int j = 0; j < matching_ids.Count(); j++)
                            smooth_buf[1 - smbuf][matching_ids[j] - m_cursors.Last().m1] = new_vert;
                    }
                }
            }
            smbuf = 1 - smbuf;
        }

        for (int i = 0; i < smooth_buf[smbuf].Count(); i++)
            m_vert[i + m_cursors.Last().m1].m_coord = smooth_buf[smbuf][i];
    }
}

} /* namespace lol */
