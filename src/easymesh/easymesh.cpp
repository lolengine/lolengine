//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2012 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The EasyMesh class
// ------------------
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined _XBOX
#   define _USE_MATH_DEFINES /* for M_PI */
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#elif defined _WIN32
#   define _USE_MATH_DEFINES /* for M_PI */
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

#include "core.h"
#include "easymesh/easymesh-compiler.h"

extern char const *lolfx_shiny;

namespace lol
{

EasyMesh::EasyMesh()
  : m_color(0), m_color2(0), m_ignore_winding_on_scale(0)
{
    m_cursors.Push(0, 0);
}

bool EasyMesh::Compile(char const *command)
{
    EasyMeshCompiler mc(*this);
    return mc.ParseString(command);
}

void EasyMesh::OpenBrace()
{
    m_cursors.Push(m_vert.Count(), m_indices.Count());
}

void EasyMesh::CloseBrace()
{
    m_cursors.Pop();
}

void EasyMesh::MeshConvert()
{
    m_gpu.shader = Shader::Create(lolfx_shiny);

    m_gpu.modelview = m_gpu.shader->GetUniformLocation("in_ModelView");
    m_gpu.view = m_gpu.shader->GetUniformLocation("in_View");
	m_gpu.invview = m_gpu.shader->GetUniformLocation("in_Inv_View");
    m_gpu.proj = m_gpu.shader->GetUniformLocation("in_Proj");
    m_gpu.normalmat = m_gpu.shader->GetUniformLocation("in_NormalMat");
    m_gpu.damage = m_gpu.shader->GetUniformLocation("in_Damage");
    m_gpu.coord = m_gpu.shader->GetAttribLocation("in_Vertex",
                                          VertexUsage::Position, 0);
    m_gpu.norm = m_gpu.shader->GetAttribLocation("in_Normal",
                                         VertexUsage::Normal, 0);
    m_gpu.color = m_gpu.shader->GetAttribLocation("in_Color",
                                          VertexUsage::Color, 0);

    m_gpu.vdecl = new VertexDeclaration(
        VertexStream<vec3,vec3,u8vec4>(VertexUsage::Position,
                                       VertexUsage::Normal,
                                       VertexUsage::Color));

    Array<vec3,vec3,u8vec4> vertexlist;
    for (int i = 0; i < m_vert.Count(); i++)
        vertexlist.Push(m_vert[i].m1,
                        m_vert[i].m2,
                        (u8vec4)(m_vert[i].m3 * 255.f));

    Array<uint16_t> indexlist;
    for (int i = 0; i < m_indices.Count(); i += 3)
    {
        indexlist << m_indices[i + 0];
        indexlist << m_indices[i + 1];
        indexlist << m_indices[i + 2];
    }

    m_gpu.vbo = new VertexBuffer(vertexlist.Bytes());
    void *mesh = m_gpu.vbo->Lock(0, 0);
    memcpy(mesh, &vertexlist[0], vertexlist.Bytes());
    m_gpu.vbo->Unlock();

    m_gpu.ibo = new IndexBuffer(indexlist.Bytes());
    void *indices = m_gpu.ibo->Lock(0, 0);
    memcpy(indices, &indexlist[0], indexlist.Bytes());
    m_gpu.ibo->Unlock();

    m_gpu.vertexcount = vertexlist.Count();
    m_gpu.indexcount = indexlist.Count();
}

void EasyMesh::Render(mat4 const &model, float damage)
{
    mat4 modelview = Scene::GetDefault()->GetViewMatrix() * model;
    mat3 normalmat = transpose(inverse(mat3(modelview)));

    m_gpu.shader->Bind();
    m_gpu.shader->SetUniform(m_gpu.modelview, modelview);
    m_gpu.shader->SetUniform(m_gpu.view, Scene::GetDefault()->GetViewMatrix());
    m_gpu.shader->SetUniform(m_gpu.invview, inverse(Scene::GetDefault()->GetViewMatrix()));
    m_gpu.shader->SetUniform(m_gpu.proj, Scene::GetDefault()->GetProjMatrix());
    m_gpu.shader->SetUniform(m_gpu.normalmat, normalmat);
    m_gpu.shader->SetUniform(m_gpu.damage, damage);
    m_gpu.vdecl->Bind();
    m_gpu.vdecl->SetStream(m_gpu.vbo, m_gpu.coord, m_gpu.norm, m_gpu.color);
    m_gpu.ibo->Bind();
    m_gpu.vdecl->DrawIndexedElements(MeshPrimitive::Triangles,
                                     0, 0, m_gpu.vertexcount,
                                     0, m_gpu.indexcount);
    m_gpu.ibo->Unbind();
    m_gpu.vdecl->Unbind();
}

void EasyMesh::ToggleScaleWinding()
{
    m_ignore_winding_on_scale = !m_ignore_winding_on_scale;
}

void EasyMesh::SetCurColor(vec4 const &color)
{
    m_color = color;
}

void EasyMesh::SetCurColor2(vec4 const &color)
{
    m_color2 = color;
}

void EasyMesh::AddVertex(vec3 const &coord)
{
    m_vert.Push(coord, vec3(0.f, 1.f, 0.f), m_color);
}

void EasyMesh::AddDuplicateVertex(int i)
{
    m_vert.Push(m_vert[i].m1, vec3(0.f, 1.f, 0.f), m_vert[i].m3);
}

void EasyMesh::AppendQuad(int i1, int i2, int i3, int i4, int base)
{
    m_indices << base + i1;
    m_indices << base + i2;
    m_indices << base + i3;

    m_indices << base + i4;
    m_indices << base + i1;
    m_indices << base + i3;
}

void EasyMesh::AppendQuadDuplicateVerts(int i1, int i2, int i3, int i4, int base)
{
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i1);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i2);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i3);

    m_indices << m_vert.Count(); AddDuplicateVertex(base + i4);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i1);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i3);
}

void EasyMesh::AppendTriangle(int i1, int i2, int i3, int base)
{
    m_indices << base + i1;
    m_indices << base + i2;
    m_indices << base + i3;
}

void EasyMesh::AppendTriangleDuplicateVerts(int i1, int i2, int i3, int base)
{
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i1);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i2);
    m_indices << m_vert.Count(); AddDuplicateVertex(base + i3);
}

void EasyMesh::ComputeNormals(int start, int vcount)
{
    for (int i = 0; i < vcount; i += 3)
    {
        vec3 v0 = m_vert[m_indices[start + i + 2]].m1
                - m_vert[m_indices[start + i + 0]].m1;
        vec3 v1 = m_vert[m_indices[start + i + 1]].m1
                - m_vert[m_indices[start + i + 0]].m1;
        vec3 n = normalize(cross(v1, v0));

        for (int j = 0; j < 3; j++)
            m_vert[m_indices[start + i + j]].m2 = n;
    }
}

void EasyMesh::SetVertColor(vec4 const &color)
{
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        m_vert[i].m3 = color;
}

void EasyMesh::SetCurVertNormal(vec3 const &normal)
{
    m_vert[m_vert.Count() - 1].m2 = normal;
}

void EasyMesh::SetCurVertColor(vec4 const &color)
{
    m_vert[m_vert.Count() - 1].m3 = color;
}

void EasyMesh::Translate(vec3 const &v)
{
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        m_vert[i].m1 += v;
}

void EasyMesh::RotateX(float t) { Rotate(t, vec3(1, 0, 0)); }
void EasyMesh::RotateY(float t) { Rotate(t, vec3(0, 1, 0)); }
void EasyMesh::RotateZ(float t) { Rotate(t, vec3(0, 0, 1)); }

void EasyMesh::Rotate(float t, vec3 const &axis)
{
    mat3 m = mat3::rotate(t, axis);
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        m_vert[i].m1 = m * m_vert[i].m1;
        m_vert[i].m2 = m * m_vert[i].m2;
    }
}

void EasyMesh::RadialJitter(float r)
{
    Array<int> Welded;
    Welded.Push(-1);
    for (int i = m_cursors.Last().m1 + 1; i < m_vert.Count(); i++)
    {
        int j, k;
        for (j = m_cursors.Last().m1, k = 0; j < i; j++, k++)
        {
            if(Welded[k] < 0)
            {
                vec3 diff = m_vert[i].m1 - m_vert[j].m1;

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
            m_vert[i].m1 *= 1.0f + RandF(r);
        else
            m_vert[i].m1 = m_vert[Welded[j]].m1;
    }

    ComputeNormals(m_cursors.Last().m2, m_indices.Count() - m_cursors.Last().m2);
}

void EasyMesh::TaperX(float y, float z, float xoff)
{
    /* FIXME: this code breaks normals! */
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        m_vert[i].m1.y *= 1.f + (y * m_vert[i].m1.x + xoff);
        m_vert[i].m1.z *= 1.f + (z * m_vert[i].m1.x + xoff);
    }
}

void EasyMesh::TaperY(float x, float z, float yoff)
{
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        m_vert[i].m1.x *= 1.f + (x * m_vert[i].m1.y + yoff);
        m_vert[i].m1.z *= 1.f + (z * m_vert[i].m1.y + yoff);
    }
}

void EasyMesh::TaperZ(float x, float y, float zoff)
{
    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        m_vert[i].m1.x *= 1.f + (x * m_vert[i].m1.z + zoff);
        m_vert[i].m1.y *= 1.f + (y * m_vert[i].m1.z + zoff);
    }
}

void EasyMesh::Scale(vec3 const &s)
{
    vec3 const invs = vec3(1) / s;

    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
    {
        m_vert[i].m1 *= s;
        m_vert[i].m2 = normalize(m_vert[i].m2 * invs);
    }

    /* Flip winding if the scaling involves mirroring */
    if (!m_ignore_winding_on_scale && s.x * s.y * s.z < 0)
    {
        for (int i = m_cursors.Last().m2; i < m_indices.Count(); i += 3)
        {
            uint16_t tmp = m_indices[i + 0];
            m_indices[i + 0] = m_indices[i + 1];
            m_indices[i + 1] = tmp;
        }
    }
}

void EasyMesh::MirrorX() { DupAndScale(vec3(-1, 1, 1)); }
void EasyMesh::MirrorY() { DupAndScale(vec3(1, -1, 1)); }
void EasyMesh::MirrorZ() { DupAndScale(vec3(1, 1, -1)); }

void EasyMesh::DupAndScale(vec3 const &s)
{
    int vlen = m_vert.Count() - m_cursors.Last().m1;
    int tlen = m_indices.Count() - m_cursors.Last().m2;

    for (int i = 0; i < vlen; i++)
        m_vert << m_vert[m_cursors.Last().m1++];

    for (int i = 0; i < tlen; i++)
        m_indices << m_indices[m_cursors.Last().m2++] + vlen;

    Scale(s);

    m_cursors.Last().m1 -= vlen;
    m_cursors.Last().m2 -= tlen;
}

void EasyMesh::AppendCylinder(int nsides, float h, float r1, float r2,
                    int dualside, int smooth)
{
    int vbase = m_vert.Count();

    mat3 rotmat = mat3::rotate(360.0f / nsides, 0.f, 1.f, 0.f);
    vec3 p1(r1, -h * .5f, 0.f), p2(r2, h * .5f, 0.f), n;

    /* Construct normal */
    if (r2 != .0f)
        n = vec3(r2, h * .5f, 0.f);
    else
        n = vec3(r1, h * .5f, 0.f);
    n.y = r1 * (r1 - r2) / h;
    if (!smooth)
        n = mat3::rotate(180.0f / nsides, 0.f, 1.f, 0.f) * n;
    n = normalize(n);

    /* FIXME: normals should be flipped in two-sided mode, but that
     * means duplicating the vertices again... */
    for (int i = 0; i < nsides; i++)
    {
        AddVertex(p1); SetCurVertNormal(n);
        AddVertex(p2); SetCurVertNormal(n);
        SetCurVertColor(m_color2);

        if (smooth)
        {
            int j = (i + 1) % nsides;
            AppendQuad(j * 2, j * 2 + 1, i * 2 + 1, i * 2, vbase);
            if (dualside)
                AppendQuad(i * 2, i * 2 + 1, j * 2 + 1, j * 2, vbase);
        }

        p1 = rotmat * p1;
        p2 = rotmat * p2;

        if (!smooth)
        {
            AddVertex(p1); SetCurVertNormal(n);
            AddVertex(p2); SetCurVertNormal(n);
            SetCurVertColor(m_color2);

            AppendQuad(i * 4 + 2, i * 4 + 3, i * 4 + 1, i * 4, vbase);
            if (dualside)
                AppendQuad(i * 4, i * 4 + 1, i * 4 + 3, i * 4 + 2, vbase);
        }

        n = rotmat * n;
    }
}

void EasyMesh::AppendCapsule(int ndivisions, float h, float r)
{
    int ibase = m_indices.Count();

    Array<vec3> vertices;

    /* FIXME: we don't know how to handle even-divided capsules, so we
     * force the count to be odd. */
    if (h)
        ndivisions |= 1;

    /* Fill in the icosahedron vertices, rotating them so that there
     * is a vertex at [0 1 0] and [0 -1 0] after normalisation. */
    float phi = 0.5f + 0.5f * sqrt(5.f);
    mat3 mat = mat3::rotate(asin(1.f / sqrt(2.f + phi)) * (180.f / M_PI),
                            vec3(0.f, 0.f, 1.f));
    for (int i = 0; i < 4; i++)
    {
        float x = (i & 1) ? 0.5f : -0.5f;
        float y = (i & 2) ? phi * 0.5f : phi * -0.5f;
        vertices << mat * vec3(x, y, 0.f);
        vertices << mat * vec3(0.f, x, y);
        vertices << mat * vec3(y, 0.f, x);
    }

    static int const trilist[] =
    {
        0, 1, 2, 2, 4, 6, 3, 8, 1, 9, 4, 8,
        7, 0, 5, 7, 11, 3, 10, 5, 6, 10, 9, 11,

        0, 3, 1, 7, 3, 0, 1, 4, 2, 8, 4, 1,
        2, 5, 0, 6, 5, 2, 6, 9, 10, 4, 9, 6,
        7, 10, 11, 5, 10, 7, 8, 11, 9, 3, 11, 8
    };

    for (unsigned i = 0; i < sizeof(trilist) / sizeof(*trilist); i += 3)
    {
        vec3 const &a = vertices[trilist[i]];
        vec3 const &b = vertices[trilist[i + 1]];
        vec3 const &c = vertices[trilist[i + 2]];

        vec3 const vb = 1.f / ndivisions * (b - a);
        vec3 const vc = 1.f / ndivisions * (c - a);

        int line = ndivisions + 1;

        for (int v = 0, x = 0, y = 0; x < ndivisions + 1; v++)
        {
            vec3 p[] = { a + x * vb + y * vc,
                         p[0] + vb,
                         p[0] + vc,
                         p[0] + vb + vc };

            /* FIXME: when we normalise here, we get a volume that is slightly
             * smaller than the sphere of radius 1, since we are not using
             * the midradius. */
            for (int k = 0; k < 4; k++)
                p[k] = normalize(p[k]) * r;

            /* If this is a capsule, grow in the Z direction */
            if (h > 0.f)
            {
                for (int k = 0; k < 4; k++)
                    p[k].y += (p[k].y > 0.f) ? 0.5f * h : -0.5f * h;
            }

            /* Add zero, one or two triangles */
            if (y < line - 1)
            {
                AddVertex(p[0]);
                AddVertex(p[1]);
                AddVertex(p[2]);
                AppendTriangle(0, 2, 1, m_vert.Count() - 3);
            }

            if (y < line - 2)
            {
                AddVertex(p[1]);
                AddVertex(p[3]);
                AddVertex(p[2]);
                AppendTriangle(0, 2, 1, m_vert.Count() - 3);
            }

            y++;
            if (y == line)
            {
                x++;
                y = 0;
                line--;
            }
        }
    }

    ComputeNormals(ibase, m_indices.Count() - ibase);
}

void EasyMesh::AppendSphere(int ndivisions, vec3 const &size)
{
    OpenBrace();
    AppendCapsule(ndivisions, 0.f, 1.f);
    Scale(size);
    CloseBrace();
}

void EasyMesh::AppendTorus(int ndivisions, float r1, float r2)
{
    int ibase = m_indices.Count();
    int nidiv = ndivisions; /* Cross-section */
    int njdiv = ndivisions; /* Full circumference */

    for (int j = 0; j < njdiv; j++)
    for (int i = 0; i < 2 * nidiv; i++)
    {
        for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
        {
            int i2 = (i + di) % nidiv;
            int j2 = (j + dj) % njdiv;
            float x = 0.5f * (r1 + r2) + 0.5 * (r2 - r1) * lol::cos(2.0 * M_PI * i2 / nidiv);
            float y = 0.5f * (r2 - r1) * lol::sin(2.0 * M_PI * i2 / nidiv);
            float z = 0.0f;

            float ca = lol::cos(2.0 * M_PI * j2 / njdiv);
            float sa = lol::sin(2.0 * M_PI * j2 / njdiv);
            float x2 = x * ca - z * sa;
            float z2 = z * ca + x * sa;

            AddVertex(vec3(x2, y, z2));
        }

        AppendTriangle(0, 2, 3, m_vert.Count() - 4);
        AppendTriangle(0, 3, 1, m_vert.Count() - 4);
    }

    ComputeNormals(ibase, m_indices.Count() - ibase);
}

void EasyMesh::AppendBox(vec3 const &size, float chamf)
{
    AppendBox(size, chamf, false);
}

void EasyMesh::AppendSmoothChamfBox(vec3 const &size, float chamf)
{
    AppendBox(size, chamf, true);
}

void EasyMesh::AppendFlatChamfBox(vec3 const &size, float chamf)
{
    AppendBox(size, chamf, false);
}

void EasyMesh::AppendBox(vec3 const &size, float chamf, bool smooth)
{
    if (chamf < 0.0f)
    {
        AppendBox(size + vec3(chamf * 2.0f), -chamf, smooth);
        return;
    }

    int vbase = m_vert.Count();
    int ibase = m_indices.Count();

    vec3 d = size * 0.5f;

    AddVertex(vec3(-d.x, -d.y, -d.z - chamf));
    AddVertex(vec3(-d.x, +d.y, -d.z - chamf));
    AddVertex(vec3(+d.x, +d.y, -d.z - chamf));
    AddVertex(vec3(+d.x, -d.y, -d.z - chamf));

    AddVertex(vec3(-d.x - chamf, -d.y, +d.z));
    AddVertex(vec3(-d.x - chamf, +d.y, +d.z));
    AddVertex(vec3(-d.x - chamf, +d.y, -d.z));
    AddVertex(vec3(-d.x - chamf, -d.y, -d.z));

    AddVertex(vec3(+d.x, -d.y, +d.z + chamf));
    AddVertex(vec3(+d.x, +d.y, +d.z + chamf));
    AddVertex(vec3(-d.x, +d.y, +d.z + chamf));
    AddVertex(vec3(-d.x, -d.y, +d.z + chamf));

    AddVertex(vec3(+d.x + chamf, -d.y, -d.z));
    AddVertex(vec3(+d.x + chamf, +d.y, -d.z));
    AddVertex(vec3(+d.x + chamf, +d.y, +d.z));
    AddVertex(vec3(+d.x + chamf, -d.y, +d.z));

    AddVertex(vec3(-d.x, -d.y - chamf, +d.z));
    AddVertex(vec3(-d.x, -d.y - chamf, -d.z));
    AddVertex(vec3(+d.x, -d.y - chamf, -d.z));
    AddVertex(vec3(+d.x, -d.y - chamf, +d.z));

    AddVertex(vec3(-d.x, +d.y + chamf, -d.z));
    AddVertex(vec3(-d.x, +d.y + chamf, +d.z));
    AddVertex(vec3(+d.x, +d.y + chamf, +d.z));
    AddVertex(vec3(+d.x, +d.y + chamf, -d.z));

    /* The 6 quads on each side of the box */
    for (int i = 0; i < 24; i += 4)
        AppendQuad(i, i + 1, i + 2, i + 3, vbase);

    ComputeNormals(ibase, m_indices.Count() - ibase);
    ibase = m_indices.Count();

    /* The 8 quads at each edge of the box */
    if (chamf)
    {
        static int const quadlist[48] =
        {
            0, 3, 18, 17, 4, 7, 17, 16, 8, 11, 16, 19, 12, 15, 19, 18,
            2, 1, 20, 23, 6, 5, 21, 20, 10, 9, 22, 21, 14, 13, 23, 22,
            1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 3, 2,
        };

        for (int i = 0; i < 48; i += 4)
        {
            if (smooth)
                AppendQuad(quadlist[i], quadlist[i + 1],
                           quadlist[i + 2], quadlist[i + 3], vbase);
            else
                AppendQuadDuplicateVerts(quadlist[i], quadlist[i + 1],
                                 quadlist[i + 2], quadlist[i + 3], vbase);
        }
    }

    /* The 8 triangles at each corner of the box */
    if (chamf)
    {
        static int const trilist[24] =
        {
            3, 12, 18, 15, 8, 19, 11, 4, 16, 7, 0, 17,
            2, 23, 13, 14, 22, 9, 10, 21, 5, 6, 20, 1,
        };

        for (int i = 0; i < 24; i += 3)
        {
            if (smooth)
                AppendTriangle(trilist[i], trilist[i + 1],
                               trilist[i + 2], vbase);
            else
                AppendTriangleDuplicateVerts(trilist[i], trilist[i + 1],
                                             trilist[i + 2], vbase);
        }
    }

    if (!smooth)
        ComputeNormals(ibase, m_indices.Count() - ibase);
}

void EasyMesh::AppendStar(int nbranches, float r1, float r2,
                          int fade, int fade2)
{
    int vbase = m_vert.Count();

    AddVertex(vec3(0.f, 0.f, 0.f));

    mat3 rotmat = mat3::rotate(180.0f / nbranches, 0.f, 1.f, 0.f);
    vec3 p1(r1, 0.f, 0.f), p2(r2, 0.f, 0.f);

    p2 = rotmat * p2;
    rotmat = rotmat * rotmat;

    for (int i = 0; i < nbranches; i++)
    {
        AddVertex(p1);
        if (fade2)
            SetCurVertColor(m_color2);

        AddVertex(p2);
        if (fade)
            SetCurVertColor(m_color2);

        AppendQuad(0, 2 * i + 1, 2 * i + 2, (2 * i + 3) % (2 * nbranches),
                   vbase);

        p1 = rotmat * p1;
        p2 = rotmat * p2;
    }
}

void EasyMesh::AppendExpandedStar(int nbranches, float r1,
                                  float r2, float extrar)
{
    int vbase = m_vert.Count();

    AddVertex(vec3(0.f, 0.f, 0.f));

    mat3 rotmat = mat3::rotate(180.0f / nbranches, 0.f, 1.f, 0.f);
    vec3 p1(r1, 0.f, 0.f), p2(r2, 0.f, 0.f),
         p3(r1 + extrar, 0.f, 0.f), p4(r2 + extrar, 0.f, 0.f);;

    p2 = rotmat * p2;
    p4 = rotmat * p4;
    rotmat = rotmat * rotmat;

    for (int i = 0; i < nbranches; i++)
    {
        AddVertex(p1);
        AddVertex(p2);
        AddVertex(p3); SetCurVertColor(m_color2);
        AddVertex(p4); SetCurVertColor(m_color2);

        int j = (i + 1) % nbranches;
        AppendQuad(0, 4 * i + 1, 4 * i + 2, 4 * j + 1, vbase);
        AppendQuad(4 * i + 1, 4 * i + 3, 4 * i + 4, 4 * i + 2, vbase);
        AppendQuad(4 * j + 1, 4 * i + 2, 4 * i + 4, 4 * j + 3, vbase);

        p1 = rotmat * p1;
        p2 = rotmat * p2;
        p3 = rotmat * p3;
        p4 = rotmat * p4;
    }
}

void EasyMesh::AppendDisc(int nsides, float r, int fade)
{
    int vbase = m_vert.Count();

    AddVertex(vec3(0.f, 0.f, 0.f));

    mat3 rotmat = mat3::rotate(360.0f / nsides, 0.f, 1.f, 0.f);
    vec3 p1(r, 0.f, 0.f);

    for (int i = 0; i < nsides; i++)
    {
        AddVertex(p1);
        if (fade)
            SetCurVertColor(m_color2);
        AppendTriangle(0, i + 1, ((i + 1) % nsides) + 1, vbase);
        p1 = rotmat * p1;
    }
}

void EasyMesh::AppendSimpleTriangle(float size, int fade)
{
    mat3 m = mat3::rotate(120.f, 0.f, 1.f, 0.f);
    vec3 p(0.f, 0.f, size);

    AddVertex(p);
    p = m * p;
    AddVertex(p);
    if (fade)
        SetCurVertColor(m_color2);
    p = m * p;
    AddVertex(p);
    if (fade)
        SetCurVertColor(m_color2);

    AppendTriangle(0, 1, 2, m_vert.Count() - 3);
}

void EasyMesh::AppendSimpleQuad(float size, int fade)
{
    AppendSimpleQuad(vec2(size * .5f), vec2(size * -.5f), 0.f, fade);
}

void EasyMesh::AppendSimpleQuad(vec2 p1, vec2 p2, float z, int fade)
{
    AddVertex(vec3(p2.x, z, -p1.y));
    AddVertex(vec3(p2.x, z, -p2.y));
    AddVertex(vec3(p1.x, z, -p2.y));
    if (fade)
        SetCurVertColor(m_color2);
    AddVertex(vec3(p1.x, z, -p1.y));
    if (fade)
        SetCurVertColor(m_color2);

    AppendQuad(3, 2, 1, 0, m_vert.Count() - 4);
    ComputeNormals(m_indices.Count() - 6, 6);
}

void EasyMesh::AppendCog(int nbsides, float h, float r1, float r2, float r12,
                         float r22, float sidemul, int offset)
{
    int ibase = m_indices.Count();
    int vbase = m_vert.Count();

    AddVertex(vec3(0.f, h * .5f, 0.f));
    AddVertex(vec3(0.f, h * -.5f, 0.f));
    SetCurVertColor(m_color2);

    mat3 rotmat = mat3::rotate(180.0f / nbsides, 0.f, 1.f, 0.f);
    mat3 smat1 = mat3::rotate(sidemul * 180.0f / nbsides, 0.f, 1.f, 0.f);
    mat3 smat2 = mat3::rotate(sidemul * -360.0f / nbsides, 0.f, 1.f, 0.f);

    vec3 p[8];

    p[0] = vec3(r1, h * .5f, 0.f);
    p[1] = rotmat * p[0];
    p[2] = smat1 * (rotmat * vec3(r1 + r12, h * .5f, 0.f));
    p[3] = smat2 * (rotmat * p[2]);

    p[4] = vec3(r2, h * -.5f, 0.f);
    p[5] = rotmat * p[4];
    p[6] = smat1 * (rotmat * vec3(r2 + r22, h * -.5f, 0.f));
    p[7] = smat2 * (rotmat * p[6]);

    if (offset & 1)
        for (int n = 0; n < 8; n++)
            p[n] = rotmat * p[n];

    rotmat = rotmat * rotmat;

    for (int i = 0; i < nbsides; i++)
    {
        /* Each vertex will share three faces, so three different
         * normals, therefore we add each vertex three times. */
        for (int n = 0; n < 24; n++)
        {
            AddVertex(p[n / 3]);
            if (n / 3 >= 4)
                SetCurVertColor(m_color2);
        }

        int j = 24 * i, k = 24 * ((i + 1) % nbsides);

        /* The top and bottom faces */
        AppendQuad(0, j + 2, j + 5, k + 2, vbase);
        AppendQuad(1, k + 14, j + 17, j + 14, vbase);
        AppendQuad(j + 5, j + 8, j + 11, k + 2, vbase);
        AppendQuad(k + 14, j + 23, j + 20, j + 17, vbase);

        /* The side quads */
        AppendQuad(j + 6, j + 3, j + 15, j + 18, vbase);
        AppendQuad(j + 9, j + 7, j + 19, j + 21, vbase);
        AppendQuad(j + 12, j + 10, j + 22, j + 24, vbase);
        AppendQuad(k + 4, j + 13, j + 25, k + 16, vbase);

        for (int n = 0; n < 8; n++)
            p[n] = rotmat * p[n];
    }

    ComputeNormals(ibase, m_indices.Count() - ibase);
}

void EasyMesh::Chamfer(float f)
{
    int vlen = m_vert.Count() - m_cursors.Last().m1;
    int ilen = m_indices.Count() - m_cursors.Last().m2;

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

        vec3 bary = 1.f / 3.f * (m_vert[m_indices[i * 3]].m1 +
                                 m_vert[m_indices[i * 3 + 1]].m1 +
                                 m_vert[m_indices[i * 3 + 2]].m1);
        for (int k = 0; k < 3; k++)
        {
            vec3 &p = m_vert[m_indices[i * 3 + k]].m1;
            p -= normalize(p - bary) * f;
        }
    }
}

} /* namespace lol */

