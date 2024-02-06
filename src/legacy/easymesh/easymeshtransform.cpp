//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2009—2015 Cédric Lecacheur <jordx@free.fr>
//            © 2009—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

// EasyMesh-Transform — The code belonging to transform operations

namespace lol
{

//-----------------------------------------------------------------------------
void EasyMesh::TranslateX(float t) { Translate(vec3(t, 0.f, 0.f)); }
void EasyMesh::TranslateY(float t) { Translate(vec3(0.f, t, 0.f)); }
void EasyMesh::TranslateZ(float t) { Translate(vec3(0.f, 0.f, t)); }

//-----------------------------------------------------------------------------
void EasyMesh::Translate(vec3 const &v)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::Translate);
        BD()->CmdStack() << v;
        return;
    }

    for (int i = std::get<0>(m_cursors.last()); i < m_vert.count(); i++)
        m_vert[i].m_coord += v;
}

//-----------------------------------------------------------------------------
void EasyMesh::RotateX(float degrees) { Rotate(degrees, vec3(1, 0, 0)); }
void EasyMesh::RotateY(float degrees) { Rotate(degrees, vec3(0, 1, 0)); }
void EasyMesh::RotateZ(float degrees) { Rotate(degrees, vec3(0, 0, 1)); }

//-----------------------------------------------------------------------------
void EasyMesh::Rotate(float degrees, vec3 const &axis)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::Rotate);
        BD()->CmdStack() << degrees << axis;
        return;
    }

    mat3 m = mat3::rotate(radians(degrees), axis);
    for (int i = std::get<0>(m_cursors.last()); i < m_vert.count(); i++)
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

    easy_array<int> welded;
    welded.push(-1);
    for (int i = std::get<0>(m_cursors.last()) + 1; i < m_vert.count(); i++)
    {
        int j, k;
        for (j = std::get<0>(m_cursors.last()), k = 0; j < i; j++, k++)
        {
            if(welded[k] < 0)
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

        welded.push(j != i ? j : -1);
    }

    int i, j;
    for (i = std::get<0>(m_cursors.last()), j = 0; i < m_vert.count(); i++, j++)
    {
        if(welded[j] == -1)
            m_vert[i].m_coord *= 1.0f + rand(r);
        else
            m_vert[i].m_coord = m_vert[welded[j]].m_coord;
    }

    ComputeNormals(std::get<1>(m_cursors.last()), m_indices.count() - std::get<1>(m_cursors.last()));
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

    for (int i = std::get<0>(m_cursors.last()); i < m_vert.count(); i++)
    {
        switch (ct.ToScalar())
        {
            case MeshTransform::Taper:
            {
                float value = m_vert[i].m_coord[axis0.ToScalar()];
                if (absolute) value = fabs(value);
                m_vert[i].m_coord[(axis0.ToScalar() + 1) % 3] *= max(0.f, 1.f + (n0 * value + noff));
                m_vert[i].m_coord[(axis0.ToScalar() + 2) % 3] *= max(0.f, 1.f + (n1 * value + noff));
                break;
            }
            case MeshTransform::Twist:
            {
                vec3 rotaxis = vec3(1.f); rotaxis[(axis0.ToScalar() + 1) % 3] = .0f; rotaxis[(axis0.ToScalar() + 2) % 3] = .0f;
                m_vert[i].m_coord = mat3::rotate(radians(m_vert[i].m_coord[axis0.ToScalar()] * n0 + noff), rotaxis) * m_vert[i].m_coord;
                break;
            }
            case MeshTransform::Shear:
            {
                float value = m_vert[i].m_coord[axis0.ToScalar()];
                if (absolute) value = fabs(value);
                m_vert[i].m_coord[(axis0.ToScalar() + 1) % 3] += (n0 * value + noff);
                m_vert[i].m_coord[(axis0.ToScalar() + 2) % 3] += (n1 * value + noff);
                break;
            }
            case MeshTransform::Stretch:
            {
                //float value = fabs(std::get<0>(m_vert[i])[axis0.ToScalar()]);
                //std::get<0>(m_vert[i])[(axis0.ToScalar() + 1) % 3] += (lol::pow(value, n0) + noff);
                //std::get<0>(m_vert[i])[(axis0.ToScalar() + 2) % 3] += (lol::pow(value, n1) + noff);
                break;
            }
            case MeshTransform::Bend:
            {
                vec3 rotaxis = vec3(1.f); rotaxis[(axis1.ToScalar() + 1) % 3] = .0f; rotaxis[(axis1.ToScalar() + 2) % 3] = .0f;
                m_vert[i].m_coord = mat3::rotate(radians(m_vert[i].m_coord[axis0.ToScalar()] * n0 + noff), rotaxis) * m_vert[i].m_coord;
                break;
            }
        }
    }
    ComputeNormals(std::get<1>(m_cursors.last()), m_indices.count() - std::get<1>(m_cursors.last()));
}

//-----------------------------------------------------------------------------
void EasyMesh::Scale(float s) { Scale(vec3(s)); }
void EasyMesh::ScaleX(float s) { Scale(vec3(s, 0.f, 0.f)); }
void EasyMesh::ScaleY(float s) { Scale(vec3(0.f, s, 0.f)); }
void EasyMesh::ScaleZ(float s) { Scale(vec3(0.f, 0.f, s)); }
void EasyMesh::Scale(vec3 const &s)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::Scale);
        BD()->CmdStack() << s;
        return;
    }

    vec3 const invs = vec3(1) / s;

    for (int i = std::get<0>(m_cursors.last()); i < m_vert.count(); i++)
    {
        m_vert[i].m_coord *= s;
        m_vert[i].m_normal = normalize(m_vert[i].m_normal * invs);
    }

    /* Flip winding if the scaling involves mirroring */
    if (!BD()->IsEnabled(MeshBuildOperation::ScaleWinding) && s.x * s.y * s.z < 0)
    {
        for (int i = std::get<1>(m_cursors.last()); i < m_indices.count(); i += 3)
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

    int vlen = m_vert.count() - std::get<0>(m_cursors.last());
    int tlen = m_indices.count() - std::get<1>(m_cursors.last());

    for (int i = 0; i < vlen; i++)
        AddDupVertex(std::get<0>(m_cursors.last())++);

    for (int i = 0; i < tlen; i++)
        m_indices << m_indices[std::get<1>(m_cursors.last())++] + vlen;

    Scale(s);

    std::get<0>(m_cursors.last()) -= vlen;
    std::get<1>(m_cursors.last()) -= tlen;

    if (open_brace)
    {
        OpenBrace();

        std::get<0>(m_cursors.last()) -= vlen;
        std::get<1>(m_cursors.last()) -= tlen;
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

    int vlen = m_vert.count() - std::get<0>(m_cursors.last());
    int ilen = m_indices.count() - std::get<1>(m_cursors.last());

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
        int trimax = m_indices.count();
        for (int i = std::get<1>(m_cursors.last()); i < trimax; i += 3)
        {
            int vbase = m_vert.count();
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
    ComputeNormals(std::get<1>(m_cursors.last()), m_indices.count() - std::get<1>(m_cursors.last()));
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
    easy_array<vec3> smooth_buf[2];
    easy_array<int> master_list;
    easy_array<int> matching_ids;
    easy_array<int> connected_vert;
    int smbuf = 0;

    for (int i = std::get<0>(m_cursors.last()); i < m_vert.count(); i++)
        vert_dict.RegisterVertex(i, m_vert[i].m_coord);

    while (main_pass--)
    {
        int split_pass = split_per_main_pass;
        int smooth_pass = smooth_per_main_pass;

        SplitTriangles(split_pass, &vert_dict);

        matching_ids.reserve(m_vert.count() - std::get<0>(m_cursors.last()));
        connected_vert.reserve(m_vert.count() - std::get<0>(m_cursors.last()));
        smooth_buf[0].resize(m_vert.count() - std::get<0>(m_cursors.last()));
        smooth_buf[1].resize(m_vert.count() - std::get<0>(m_cursors.last()));

        for (int i = std::get<0>(m_cursors.last()); i < m_vert.count(); i++)
            smooth_buf[smbuf][i - std::get<0>(m_cursors.last())] = m_vert[i].m_coord;

        while (smooth_pass--)
        {
            master_list.clear();
            if (vert_dict.GetMasterList(master_list))
            {
                for (int i = 0; i < master_list.count(); i++)
                {
                    connected_vert.clear();
                    if (vert_dict.FindConnectedVertices(master_list[i], m_indices, std::get<1>(m_cursors.last()), connected_vert))
                    {
                        //Calculate vertices sum
                        vec3 vert_sum = vec3(.0f);
                        for (int j = 0; j < connected_vert.count(); j++)
                            vert_sum += smooth_buf[smbuf][connected_vert[j] - std::get<0>(m_cursors.last())];

                        //Calculate new master vertex
                        float n = (float)connected_vert.count();
                        //b(n) = 5/4 - pow(3 + 2 * cos(2.f * F_PI / n), 2) / 32
                        float beta = 3.f + 2.f * cos(2.f * F_PI / n);
                        beta = 5.f / 4.f - beta * beta / 32.f;
                        //a(n) = n * (1 - b(n)) / b(n)
                        float alpha = (n * (1 - beta)) / beta;
                        //V = (a(n) * v + v1 + ... + vn) / (a(n) + n)
                        vec3 new_vert = (alpha * smooth_buf[smbuf][master_list[i] - std::get<0>(m_cursors.last())] + vert_sum) / (alpha + n);

                        //Set all matching vertices to new value
                        matching_ids.clear();
                        matching_ids << master_list[i];
                        vert_dict.FindMatchingVertices(master_list[i], matching_ids);
                        for (int j = 0; j < matching_ids.count(); j++)
                            smooth_buf[1 - smbuf][matching_ids[j] - std::get<0>(m_cursors.last())] = new_vert;
                    }
                }
            }
            smbuf = 1 - smbuf;
        }

        for (int i = 0; i < smooth_buf[smbuf].count(); i++)
            m_vert[i + std::get<0>(m_cursors.last())].m_coord = smooth_buf[smbuf][i];
    }
}

} /* namespace lol */
