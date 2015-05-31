//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
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

// EasyMesh-Primitive — The code belonging to primitive operations

namespace lol
{

//-----------------------------------------------------------------------------
void EasyMesh::AppendCylinder(int nsides, float h, float d1, float d2,
                              bool dualside, bool smooth, bool close)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendCylinder);
        BD()->CmdStack() << nsides << h << d1 << d2 << dualside << smooth << close;
        return;
    }

    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r1 = d1 * .5f;
    float r2 = d2 * .5f;

    //SAVE
    vec4 Saved_ColorA = BD()->ColorA();
    vec4 Saved_ColorB = BD()->ColorB();
    vec2 Save_texcoord_offset = BD()->TexCoordOffset();
    vec2 Save_texcoord_scale = BD()->TexCoordScale();

    int vbase = m_vert.count();

    mat3 rotmat = mat3::rotate(radians(360.0f) / (float)nsides, 0.f, 1.f, 0.f);
    vec3 p1(r1, -h * .5f, 0.f), p2(r2, h * .5f, 0.f), n;
    vec2 uv1(.0f, .0f), uv2(.0f, 1.0f), uvadd(1.0f / (float)nsides, .0f);
    if (close)
        SetTexCoordData(vec2(.0f), vec2(1.0f, .5f));

    /* Construct normal */
    if (r2 != .0f)
        n = vec3(r2, h * .5f, 0.f);
    else
        n = vec3(r1, h * .5f, 0.f);
    n.y = r1 * (r1 - r2) / h;
    if (!smooth)
        n = mat3::rotate(radians(180.0f) / nsides, 0.f, 1.f, 0.f) * n;
    n = normalize(n);

    //Two passes necessary to ensure "weighted quad" compatibility
    //First pass : Add vertices
    for (int i = 0; i < nsides; i++)
    {
        /* FIXME: normals should be flipped in two-sided mode, but that
         * means duplicating the vertices again... */
        AddVertex(p1); SetCurVertNormal(n); SetCurVertTexCoord(uv1); SetCurVertTexCoord2(uv1);
        AddVertex(p2); SetCurVertNormal(n); SetCurVertTexCoord(uv2); SetCurVertTexCoord2(uv2); SetCurVertColor(BD()->ColorB());

        p1 = rotmat * p1; uv1 += uvadd;
        p2 = rotmat * p2; uv2 += uvadd;

        if (!smooth)
        {
            AddVertex(p1); SetCurVertNormal(n); SetCurVertTexCoord(uv1); SetCurVertTexCoord2(uv1);
            AddVertex(p2); SetCurVertNormal(n); SetCurVertTexCoord(uv2); SetCurVertTexCoord2(uv2); SetCurVertColor(BD()->ColorB());
        }

        n = rotmat * n;
    }
    //Second pass : Build quad
    for (int i = 0; i < nsides; i++)
    {
        if (smooth)
        {
            int j = (i + 1) % nsides;
            AddQuad(j * 2, j * 2 + 1, i * 2 + 1, i * 2, vbase);
            if (dualside)
                AddQuad(i * 2, i * 2 + 1, j * 2 + 1, j * 2, vbase);
        }
        else
        {
            AddQuad(i * 4 + 2, i * 4 + 3, i * 4 + 1, i * 4, vbase);
            if (dualside)
                AddQuad(i * 4, i * 4 + 1, i * 4 + 3, i * 4 + 2, vbase);
        }

    }

    if (close)
    {
        //START
        OpenBrace();
        //LOWER DISC
        SetTexCoordData(vec2(.0f, .5f), vec2(.5f, .5f));
        SetCurColorA(BD()->ColorA());
        AppendDisc(nsides, d1);
        Translate(vec3(.0f, h, .0f));
        RotateX(180.0f);
        //UPPER DISC
        SetTexCoordData(vec2(.5f, .5f), vec2(.5f, .5f));
        SetCurColorA(BD()->ColorB());
        AppendDisc(nsides, d2);
        Translate(vec3(.0f, h * .5f, .0f));
        CloseBrace();
    }
    //RESTORE
    SetCurColorA(Saved_ColorA);
    SetCurColorB(Saved_ColorB);
    SetTexCoordData(Save_texcoord_offset, Save_texcoord_scale);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSphere(int ndivisions, float d)
{
    AppendCapsule(ndivisions, 0.f, d);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendCapsule(int ndivisions, float h, float d)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendCapsule);
        BD()->CmdStack() << ndivisions << h << d;
        return;
    }

    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r = d * .5f;

    int ibase = m_indices.count();

    array<vec3> vertices;
    float uv_h = 0;
    float uv_r = 0;

    /* FIXME: we don't know how to handle even-divided capsules, so we
     * force the count to be odd. */
    if (h)
    {
        ndivisions |= 1;
        //calculate uv h&r percents
        uv_h = (float)h / (float)(h + r * 2);
        uv_r = (float)r / (float)(h + r * 2);
    }

    /* Fill in the icosahedron vertices, rotating them so that there
     * is a vertex at [0 1 0] and [0 -1 0] after normalisation. */
    float phi = 0.5f + 0.5f * sqrt(5.f);
    mat3 m = mat3::rotate(asin(1.f / sqrt(2.f + phi)),
                          vec3(0.f, 0.f, 1.f));
    for (int i = 0; i < 4; i++)
    {
        float x = (i & 1) ? 0.5f : -0.5f;
        float y = (i & 2) ? phi * 0.5f : phi * -0.5f;
        vertices << m * vec3(x, y, 0.f);
        vertices << m * vec3(0.f, x, y);
        vertices << m * vec3(y, 0.f, x);
    }

    static int const tris[] =
    {
        0, 1, 2, 2, 4, 6, 3, 8, 1, 9, 4, 8,
        7, 0, 5, 7, 11, 3, 10, 5, 6, 10, 9, 11,

        0, 3, 1, 7, 3, 0, 1, 4, 2, 8, 4, 1,
        2, 5, 0, 6, 5, 2, 6, 9, 10, 4, 9, 6,
        7, 10, 11, 5, 10, 7, 8, 11, 9, 3, 11, 8
    };

    for (unsigned i = 0; i < sizeof(tris) / sizeof(*tris); i += 3)
    {
        vec3 const &a = vertices[tris[i]];
        vec3 const &b = vertices[tris[i + 1]];
        vec3 const &c = vertices[tris[i + 2]];

        vec3 const vb = 1.f / ndivisions * (b - a);
        vec3 const vc = 1.f / ndivisions * (c - a);

        int line = ndivisions + 1;

        for (int v = 0, x = 0, y = 0; x < ndivisions + 1; v++)
        {
            vec3 p[] = { a + (float)x * vb + (float)y * vc,
                         p[0] + vb,
                         p[0] + vc,
                         p[0] + vb + vc };
            vec2 uv[4];

            /* FIXME: when we normalise here, we get a volume that is slightly
             * smaller than the sphere of radius 1, since we are not using
             * the midradius. */
            for (int k = 0; k < 4; k++)
            {
                //keep normalized until the end of the UV calculations
                p[k] = normalize(p[k]);

                uv[k].x = (lol::atan2(p[k].z, p[k].x) + F_PI) / (F_PI * 2.f);
                if (abs(p[k].y) >= 1.0f)
                    uv[k].x = -1.f;
                uv[k].y = lol::atan2(p[k].y, dot(p[k], normalize(p[k] * vec3(1.f,0.f,1.f)))) / F_PI + 0.5f;
                if (h)
                {
                    if (uv[k].y > .5f)
                        uv[k].y = uv_r + uv_h + (uv[k].y - .5f) * uv_r * 2.f;
                    else
                        uv[k].y *= uv_r * 2.f;
                }
                p[k] *= r;
            }

            /* If this is a capsule, grow in the Y direction */
            if (h > 0.f)
            {
                for (int k = 0; k < 4; k++)
                    p[k].y += (p[k].y > 0.f) ? 0.5f * h : -0.5f * h;
            }

            /* Add zero, one or two triangles */
            int id[] = { 0, 1, 2,
                         1, 3 ,2 };
            int l = 6;
            while ((l -= 3) >= 0)
            {
                if ((l == 0 && y < line - 1) || (l == 3 && y < line - 2))
                {
                    int k = -1;
                    while (++k < 3)
                    {
                        int rid[] = { id[k + l], id[(k + 1) % 3 + l] };
                        if (uv[rid[0]].x >= .0f &&
                            uv[rid[1]].x >= .0f &&
                            abs(uv[rid[0]].x - uv[rid[1]].x) > .5f)
                        {
                            if (uv[rid[0]].x < uv[rid[1]].x)
                                uv[rid[0]].x += 1.0f;
                            else
                                uv[rid[1]].x += 1.0f;
                        }
                    }
                    k = -1;
                    while (++k < 3)
                    {
                        int rid[] = { id[k + l], id[(k + 1) % 3 + l], id[(k + 2) % 3 + l] };
                        AddVertex(p[rid[0]]);
                        vec2 new_uv;
                        if (uv[rid[0]].x < .0f)
                            new_uv = vec2((uv[rid[1]].x + uv[rid[2]].x) * .5f, uv[rid[0]].y);
                        else
                            new_uv = uv[rid[0]];
                        SetCurVertTexCoord(vec2(0.f, 1.f) - new_uv);
                        SetCurVertTexCoord2(vec2(0.f, 1.f) - new_uv);
                    }
                    AddTriangle(0, 2, 1, m_vert.count() - 3);
                }
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

    ComputeNormals(ibase, m_indices.count() - ibase);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendTorus(int ndivisions, float d1, float d2)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendTorus);
        BD()->CmdStack() << ndivisions << d1 << d2;
        return;
    }

    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r1 = d1 * .5f;
    float r2 = d2 * .5f;

    int ibase = m_indices.count();
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

            //Location on the donut
            float x = 0.5f * (r2 - r1) * (float)lol::cos(2.f * F_PI * i2 / nidiv) + 0.5f * (r1 + r2);
            float y = 0.5f * (r2 - r1) * (float)lol::sin(2.f * F_PI * i2 / nidiv);
            float z = 0.0f;

            //Center circle
            float ca = (float)lol::cos(2.f * F_PI * j2 / njdiv);
            float sa = (float)lol::sin(2.f * F_PI * j2 / njdiv);

            //Actual location
            float x2 = x * ca - z * sa;
            float z2 = z * ca + x * sa;

            AddVertex(vec3(x2, y, z2));
            SetCurVertTexCoord(vec2((float)(i + di) / (float)nidiv, (float)(j + dj) / (float)nidiv));
            SetCurVertTexCoord2(vec2((float)(i + di) / (float)nidiv, (float)(j + dj) / (float)nidiv));
        }

        AddTriangle(0, 2, 3, m_vert.count() - 4);
        AddTriangle(0, 3, 1, m_vert.count() - 4);
    }

    ComputeNormals(ibase, m_indices.count() - ibase);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendBox(vec3 const &size, float chamf)
{
    AppendBox(size, chamf, false);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSmoothChamfBox(vec3 const &size, float chamf)
{
    AppendBox(size, chamf, true);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendFlatChamfBox(vec3 const &size, float chamf)
{
    AppendBox(size, chamf, false);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendBox(vec3 const &size, float chamf, bool smooth)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendBox);
        BD()->CmdStack() << size << chamf << smooth;
        return;
    }

    if (chamf < 0.0f)
    {
        AppendBox(size + vec3(chamf * 2.0f), -chamf, smooth);
        return;
    }

    int vbase = m_vert.count();
    int ibase = m_indices.count();

    vec3 d = size * 0.5f;

    MeshType mt = MeshType::Box;
    TexCoordPos bl = TexCoordPos::BL;
    TexCoordPos br = TexCoordPos::BR;
    TexCoordPos tl = TexCoordPos::TL;
    TexCoordPos tr = TexCoordPos::TR;

    //--
    //Side vertices
    //--
    MeshFaceType mft = MeshFaceType::BoxFront;
    AddVertex(vec3(-d.x, -d.y, -d.z - chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, +d.y, -d.z - chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, +d.y, -d.z - chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, -d.y, -d.z - chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    mft = MeshFaceType::BoxLeft;
    AddVertex(vec3(-d.x - chamf, -d.y, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x - chamf, +d.y, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x - chamf, +d.y, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x - chamf, -d.y, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    mft = MeshFaceType::BoxBack;
    AddVertex(vec3(+d.x, -d.y, +d.z + chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, +d.y, +d.z + chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, +d.y, +d.z + chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, -d.y, +d.z + chamf));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    mft = MeshFaceType::BoxRight;
    AddVertex(vec3(+d.x + chamf, -d.y, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x + chamf, +d.y, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x + chamf, +d.y, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x + chamf, -d.y, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    //Bottom vertices
    //--
    mft = MeshFaceType::BoxBottom;
    AddVertex(vec3(-d.x, -d.y - chamf, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, -d.y - chamf, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, -d.y - chamf, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, -d.y - chamf, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    //--
    //Top vertices
    //--
    mft = MeshFaceType::BoxTop;
    AddVertex(vec3(-d.x, +d.y + chamf, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(-d.x, +d.y + chamf, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, +d.y + chamf, +d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    //--
    AddVertex(vec3(+d.x, +d.y + chamf, -d.z));
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));

    ComputeNormals(ibase, m_indices.count() - ibase);
    ibase = m_indices.count();

    //Build the box at the end : The 6 quads on each side of the box.
    for (int i = 0; i < 24; i += 4)
        AddQuad(i, i + 1, i + 2, i + 3, vbase);

    /* The 8 quads at each edge of the box */
    if (chamf)
    {
        static int const quads[48] =
        {
            0, 3, 18, 17, 4, 7, 17, 16, 8, 11, 16, 19, 12, 15, 19, 18,
            2, 1, 20, 23, 6, 5, 21, 20, 10, 9, 22, 21, 14, 13, 23, 22,
            1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 3, 2,
        };

        for (int i = 0; i < 48; i += 4)
            AddQuad(quads[i], quads[i + 1], quads[i + 2], quads[i + 3], vbase, !smooth);
    }

    /* The 8 triangles at each corner of the box */
    if (chamf)
    {
        static int const tris[24] =
        {
            3, 12, 18, 15, 8, 19, 11, 4, 16, 7, 0, 17,
            2, 23, 13, 14, 22, 9, 10, 21, 5, 6, 20, 1,
        };

        for (int i = 0; i < 24; i += 3)
            AddTriangle(tris[i], tris[i + 1], tris[i + 2], vbase, !smooth);
    }

    if (!smooth)
        ComputeNormals(ibase, m_indices.count() - ibase);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendStar(int nbranches, float d1, float d2,
                          bool fade, bool fade2)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendStar);
        BD()->CmdStack() << nbranches << d1 << d2 << fade << fade2;
        return;
    }

    //Should ignore quad weight, as it does not destroy star symmetry
    BD()->Enable(MeshBuildOperation::IgnoreQuadWeighting);

    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r1 = d1 * .5f;
    float r2 = d2 * .5f;

    //TODO: It would probably be good to think of another way of UV painting this, like "branch repeating"
    int vbase = m_vert.count();
    float maxr = max(r1, r2);

    AddVertex(vec3(0.f, 0.f, 0.f)); SetCurVertTexCoord(vec2(.5f, .5f)); SetCurVertTexCoord2(vec2(.5f, .5f));

    mat3 rotmat = mat3::rotate(radians(180.0f) / nbranches, 0.f, 1.f, 0.f);
    vec3 p1(r1, 0.f, 0.f), p2(r2, 0.f, 0.f);
    vec3 uv1(0.f, 0.f, -.5f * ((float)r1 / maxr)),
         uv2(0.f, 0.f, -.5f * ((float)r2 / maxr));

    p2 = rotmat * p2; uv2 = rotmat * uv2;
    rotmat = rotmat * rotmat;

    for (int i = 0; i < nbranches; i++)
    {
        AddVertex(p1); SetCurVertTexCoord(uv1.xz + vec2(.5f)); SetCurVertTexCoord2(uv1.xz + vec2(.5f));
        if (fade2)
            SetCurVertColor(BD()->ColorB());

        AddVertex(p2); SetCurVertTexCoord(uv2.xz + vec2(.5f)); SetCurVertTexCoord2(uv2.xz + vec2(.5f));
        if (fade)
            SetCurVertColor(BD()->ColorB());

        //Append quad at the end
        AddQuad(0, 2 * i + 1, 2 * i + 2, (2 * i + 3) % (2 * nbranches), vbase);

        p1 = rotmat * p1; uv1 = rotmat * uv1;
        p2 = rotmat * p2; uv2 = rotmat * uv2;
    }

    //Restore
    BD()->Disable(MeshBuildOperation::IgnoreQuadWeighting);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendExpandedStar(int nbranches, float d1, float d2, float extrad)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendExpandedStar);
        BD()->CmdStack() << nbranches << d1 << d2 << extrad;
        return;
    }

    //Should ignore quad weight, as it does not destroy star symmetry
    BD()->Enable(MeshBuildOperation::IgnoreQuadWeighting);

    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r1 = d1 * .5f;
    float r2 = d2 * .5f;
    float extrar = extrad * .5f;

    int vbase = m_vert.count();
    float maxr = lol::max(lol::max(r1, r2), lol::max(r1 + extrar, r2 + extrar));

    AddVertex(vec3(0.f, 0.f, 0.f)); SetCurVertTexCoord(vec2(.5f, .5f)); SetCurVertTexCoord2(vec2(.5f, .5f));

    mat3 rotmat = mat3::rotate(radians(180.0f) / nbranches, 0.f, 1.f, 0.f);
    vec3 p1(r1, 0.f, 0.f), p2(r2, 0.f, 0.f),
         p3(r1 + extrar, 0.f, 0.f), p4(r2 + extrar, 0.f, 0.f);;
    vec3 uv1(0.f, 0.f, -.5f * ((float)r1 / maxr)),
         uv2(0.f, 0.f, -.5f * ((float)r2 / maxr)),
         uv3(0.f, 0.f, -.5f * ((float)(r1 + extrar) / maxr)),
         uv4(0.f, 0.f, -.5f * ((float)(r2 + extrar) / maxr));

    p2 = rotmat * p2; uv2 = rotmat * uv2;
    p4 = rotmat * p4; uv4 = rotmat * uv4;
    rotmat = rotmat * rotmat;

    for (int i = 0; i < nbranches; i++)
    {
        AddVertex(p1); SetCurVertTexCoord(uv1.xz + vec2(.5f)); SetCurVertTexCoord2(uv1.xz + vec2(.5f));
        AddVertex(p2); SetCurVertTexCoord(uv2.xz + vec2(.5f)); SetCurVertTexCoord2(uv2.xz + vec2(.5f));
        AddVertex(p3); SetCurVertTexCoord(uv3.xz + vec2(.5f)); SetCurVertTexCoord2(uv3.xz + vec2(.5f)); SetCurVertColor(BD()->ColorB());
        AddVertex(p4); SetCurVertTexCoord(uv4.xz + vec2(.5f)); SetCurVertTexCoord2(uv4.xz + vec2(.5f)); SetCurVertColor(BD()->ColorB());

        int j = (i + 1) % nbranches;
        //
        AddQuad(0, 4 * i + 1, 4 * i + 2, 4 * j + 1, vbase);
        AddQuad(4 * i + 1, 4 * i + 3, 4 * i + 4, 4 * i + 2, vbase);
        AddQuad(4 * j + 1, 4 * i + 2, 4 * i + 4, 4 * j + 3, vbase);

        p1 = rotmat * p1; uv1 = rotmat * uv1;
        p2 = rotmat * p2; uv2 = rotmat * uv2;
        p3 = rotmat * p3; uv3 = rotmat * uv3;
        p4 = rotmat * p4; uv4 = rotmat * uv4;
    }

    //Restore
    BD()->Disable(MeshBuildOperation::IgnoreQuadWeighting);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendDisc(int nsides, float d, bool fade)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendDisc);
        BD()->CmdStack() << nsides << d << fade;
        return;
    }

    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r = d * .5f;

    int vbase = m_vert.count();

    AddVertex(vec3(0.f, 0.f, 0.f)); SetCurVertTexCoord(vec2(.5f, .5f)); SetCurVertTexCoord2(vec2(.5f, .5f));

    mat3 rotmat = mat3::rotate(radians(360.0f) / nsides, 0.f, 1.f, 0.f);
    vec3 p1(r, 0.f, 0.f);
    vec3 uv(.5f, .0f, .0f);

    for (int i = 0; i < nsides; i++)
    {
        AddVertex(p1); SetCurVertTexCoord(uv.xz + vec2(.5f, .5f)); SetCurVertTexCoord2(uv.xz + vec2(.5f, .5f));
        if (fade)
            SetCurVertColor(BD()->ColorB());
        AddTriangle(0, i + 1, ((i + 1) % nsides) + 1, vbase);
        p1 = rotmat * p1;
        uv = rotmat * uv;
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSimpleTriangle(float d, bool fade)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendSimpleTriangle);
        BD()->CmdStack() << d << fade;
        return;
    }

    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float size = d * .5f;

    mat3 m = mat3::rotate(radians(120.f), 0.f, 1.f, 0.f);
    vec3 p(0.f, 0.f, size);

    AddVertex(p); SetCurVertTexCoord(vec2(.5f, 0.133975f)); SetCurVertTexCoord2(vec2(.5f, 0.133975f));
    p = m * p;
    AddVertex(p); SetCurVertTexCoord(vec2(0.f, 1.f)); SetCurVertTexCoord2(vec2(0.f, 1.f));
    if (fade)
        SetCurVertColor(BD()->ColorB());
    p = m * p;
    AddVertex(p); SetCurVertTexCoord(vec2(1.f, 1.f)); SetCurVertTexCoord2(vec2(1.f, 1.f));
    if (fade)
        SetCurVertColor(BD()->ColorB());

    AddTriangle(0, 1, 2, m_vert.count() - 3);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSimpleQuad(float size, bool fade)
{
    AppendSimpleQuad(vec2(size * .5f), vec2(size * -.5f), 0.f, fade);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendSimpleQuad(vec2 p1, vec2 p2, float z, bool fade)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendSimpleQuad);
        BD()->CmdStack() << p1 << p2 << z << fade;
        return;
    }

    MeshType mt = MeshType::Quad;
    MeshFaceType mft = MeshFaceType::QuadDefault;

    //--
    AddVertex(vec3(p2.x, z, -p1.y));
    TexCoordPos br = TexCoordPos::BR;
    SetCurVertTexCoord(BD()->TexCoord(mt, br, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, br, mft));
    //--
    AddVertex(vec3(p2.x, z, -p2.y));
    TexCoordPos bl = TexCoordPos::BL;
    SetCurVertTexCoord(BD()->TexCoord(mt, bl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, bl, mft));
    //--
    AddVertex(vec3(p1.x, z, -p2.y));
    TexCoordPos tl = TexCoordPos::TL;
    SetCurVertTexCoord(BD()->TexCoord(mt, tl, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tl, mft));
    if (fade) SetCurVertColor(BD()->ColorB());
    //--
    AddVertex(vec3(p1.x, z, -p1.y));
    TexCoordPos tr = TexCoordPos::TR;
    SetCurVertTexCoord(BD()->TexCoord(mt, tr, mft));
    SetCurVertTexCoord2(BD()->TexCoord2(mt, tr, mft));
    if (fade) SetCurVertColor(BD()->ColorB());

    AddQuad(0, 1, 2, 3, m_vert.count() - 4);
    ComputeNormals(m_indices.count() - 6, 6);
}

//-----------------------------------------------------------------------------
void EasyMesh::AppendCog(int nbsides, float h, float d10, float d20,
                         float d11, float d21, float d12, float d22,
                         float sidemul, bool offset)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::AppendCog);
        BD()->CmdStack() << nbsides << h
                         << d10 << d20
                         << d11 << d21
                         << d12 << d22
                         << sidemul << offset;
        return;
    }

    //XXX : This operation is done to convert radius to diameter without changing all the code.
    float r10 = d10 * .5f;
    float r20 = d20 * .5f;
    float r11 = d11 * .5f;
    float r21 = d21 * .5f;
    float r12 = d12 * .5f;
    float r22 = d22 * .5f;

    int ibase = m_indices.count();
    int vbase = m_vert.count();

    /* FIXME: enforce this some other way */
    if (r12 < 0)
        h = -h;

    mat3 rotmat = mat3::rotate(radians(180.0f) / nbsides, 0.f, 1.f, 0.f);
    mat3 smat1 = mat3::rotate(sidemul * radians(180.0f) / nbsides, 0.f, 1.f, 0.f);
    mat3 smat2 = mat3::rotate(sidemul * radians(-360.0f) / nbsides, 0.f, 1.f, 0.f);

    vec3 p[12];

    //Upper points
    p[0] = vec3(r10, h * .5f, 0.f);
    p[1] = rotmat * p[0];
    p[2] = vec3(r11, h * .5f, 0.f);
    p[3] = rotmat * p[2];
    p[4] = smat1 * (rotmat * vec3(r11 + r12, h * .5f, 0.f));
    p[5] = smat2 * (rotmat * p[4]);

    //Lower points
    p[6] = vec3(r20, h * -.5f, 0.f);
    p[7] = rotmat * p[6];
    p[8] = vec3(r21, h * -.5f, 0.f);
    p[9] = rotmat * p[8];
    p[10] = smat1 * (rotmat * vec3(r21 + r22, h * -.5f, 0.f));
    p[11] = smat2 * (rotmat * p[10]);

    if (offset)
        for (int n = 0; n < 12; n++)
            p[n] = rotmat * p[n];

    rotmat = rotmat * rotmat;

    //UV base computation
    float maxr = max(max(r11 + r12, r21 + r22), max(r10, r20));
    float InLn = length(p[1] - p[0]);
    float CogLn[8] = { .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0, k = 2; j < 8 && k < 12; j += 4, k += 6)
        {
            CogLn[j + i] = length(p[k + i + 1] - p[k + i]);
            CogLn[j + 3] += CogLn[j + i];
            if (i == 1) //Add 3to4 twice since it's automatically completed by +1 loop.
                CogLn[j + 3] += CogLn[j + i];
        }
    }

    //Choose the biggest cog length
    int CogSrc = (CogLn[7] > CogLn[3])?(4):(0);
    CogLn[3] = CogLn[CogSrc + 3];
    for (int i = 0; i < 3; i++)
        CogLn[i] = CogLn[CogSrc + i] / CogLn[CogSrc + 3];

    //Calculate Cog Modifiers
    vec2 InUV[2] = { vec2(.0f), vec2(.5f) };
    vec2 CogUV[2] = { vec2(.0f), vec2(.5f) };
    vec2 upadd = vec2(.25f, .75f);
    vec2 lowadd = vec2(.75f, .75f);
    {
        if (h < InLn)
        {
            InUV[0].x  = 1.0f;
            InUV[0].y  = h / InLn;
            InUV[1].x  = .0f;
            InUV[1].y -= InUV[0].y * .5f;
        }
        else
        {
            InUV[0].x  = InLn / h;
            InUV[0].y  = 1.0f;
            InUV[1].x -= InUV[0].x * .5f;
            InUV[1].y = .0f;
        }
        if (h < CogLn[3])
        {
            CogUV[0].x  = 1.0f;
            CogUV[0].y  = h / CogLn[3];
            CogUV[1].x  = .0f;
            CogUV[1].y -= CogUV[0].y * .5f;
        }
        else
        {
            CogUV[0].x  = CogLn[3] / h;
            CogUV[0].y  = 1.0f;
            CogUV[1].x -= CogUV[0].x * .5f;
            CogUV[1].y  = .0f;
        }
        if (InUV[0].x + CogUV[0].x < .5f)
        {
            InUV[1].x = .0f;
            CogUV[1].x = .5f - CogUV[0].x;
            upadd  = vec2(.75f, .25f);
            lowadd = vec2(.75f, .75f);
        }
        else if (InUV[0].y + CogUV[0].y < .5f)
        {
            InUV[1].y = .0f;
            CogUV[1].y = .5f - CogUV[0].y;
        }
        else
        {
            InUV[0] *= .5f;
            InUV[1] *= .5f;
            CogUV[0] *= .5f;
            CogUV[1] *= .5f;
            InUV[1] += vec2(.5f, .0f);
        }
    }

    //Build UV tab
    vec2 uv[12]; float CogSz;
    //Upper points
    CogSz = 1.0f - CogLn[1];
    uv[0]  = vec2(0.f,   0.f) * InUV[0]  + InUV[1];
    uv[1]  = vec2(1.f,   0.f) * InUV[0]  + InUV[1];
    uv[5]  = vec2(CogSz, 0.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[2];
    uv[4]  = vec2(CogSz, 0.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[1];
    uv[3]  = vec2(CogSz, 0.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[0];
    uv[2]  = vec2(0.f,   0.f) * CogUV[0] + CogUV[1];

    //Lower points
    CogSz = 1.0f - CogLn[1];
    uv[6]  = vec2(0.f,   1.f) * InUV[0]  + InUV[1];
    uv[7]  = vec2(1.f,   1.f) * InUV[0]  + InUV[1];
    uv[11] = vec2(CogSz, 1.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[2];
    uv[10] = vec2(CogSz, 1.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[1];
    uv[ 9] = vec2(CogSz, 1.f) * CogUV[0] + CogUV[1]; CogSz -= CogLn[0];
    uv[ 8] = vec2(0.f,   1.f) * CogUV[0] + CogUV[1];

#define DEF_J_K_Q                         \
    int j = 3 * 12 * i,                   \
        k = 3 * 12 * ((i + 1) % nbsides); \
    int q[] = {                \
/* The top and bottom faces */ \
                j, j, j, j, \
                j, j, j, j, \
                j, j, k, k, \
                k, k, j, j, \
                j, j, j, k, \
                k, j, j, j, \
/* The inner side quads */  \
                j, j, j, j, \
                j, k, k, j, \
/* The outer side quads */  \
                j, j, j, j, \
                j, j, j, j, \
                j, j, j, j, \
                k, j, j, k  \
                };          \
    UNUSED(q);
    int m[] = { /* The top and bottom faces */
                0,  2,  3,  1,
                7,  9,  8,  6,
                1,  3,  2,  0,
                6,  8,  9,  7,
                3,  4,  5,  2,
                8, 11, 10,  9,
                /* The inner side quads */
                0,  1,  7,  6,
                1,  0,  6,  7,
                /* The outer side quads */
                3,  2,  8,  9,
                4,  3,  9, 10,
                5,  4, 10, 11,
                2,  5, 11, 8
                };
    int a[] = { /* The top and bottom faces */
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                /* The inner side quads */
                1, 1, 1, 1,
                2, 2, 2, 2,
                /* The outer side quads */
                1, 1, 1, 1,
                1, 2, 2, 1,
                1, 2, 2, 1,
                2, 2, 2, 2
                };
    //Gear generation loop
    //Two passes necessary to ensure "weighted quad" compatibility
    //First pass : Add vertices
    for (int i = 0; i < nbsides; i++)
    {
        DEF_J_K_Q;

        /* Each vertex will share three faces, so three different
         * normals, therefore we add each vertex three times. */
        for (int n = 0; n < 3 * 12; n++)
        {
            int d = n / 3;
            int e = d % 6;
            AddVertex(p[d]);
            if (n % 3 == 0) //Top-Bottom logic
            {
                vec2 tmp = (p[d].xz / maxr);
                vec2 add;
                if (d >= 6)
                {
                    tmp *= -1.0f;
                    add = lowadd;
                }
                else
                    add = upadd;
                SetCurVertTexCoord(tmp * vec2(.25f) + add);
                SetCurVertTexCoord2(tmp * vec2(.25f) + add);
            }
            else if (e == 0 || e == 1) //inner Logic
            {
                SetCurVertTexCoord(uv[d]);
                SetCurVertTexCoord2(uv[d]);
            }
            else //Cog logic
            {
                if (e == 2 && n % 3 == 2)
                {
                    SetCurVertTexCoord(vec2(1.f, (d == 2)?(0.f):(1.f)) * CogUV[0] + CogUV[1]);
                    SetCurVertTexCoord2(vec2(1.f, (d == 2)?(0.f):(1.f)) * CogUV[0] + CogUV[1]);
                }
                else
                {
                    SetCurVertTexCoord(uv[d]);
                    SetCurVertTexCoord2(uv[d]);
                }
            }
            if (d >= 6)
                SetCurVertColor(BD()->ColorB());
        }

        for (int n = 0; n < 12; n++)
            p[n] = rotmat * p[n];
    }
    //Second pass : Build quad
    for (int i = 0; i < nbsides; i++)
    {
        DEF_J_K_Q;
        int l = -4;
        while ((l += 4) < 48)
            AddQuad(q[l + 0] + m[l + 0] * 3 + a[l + 0],
                    q[l + 1] + m[l + 1] * 3 + a[l + 1],
                    q[l + 2] + m[l + 2] * 3 + a[l + 2],
                    q[l + 3] + m[l + 3] * 3 + a[l + 3],
                    vbase);
    }

    ComputeNormals(ibase, m_indices.count() - ibase);
}

} /* namespace lol */
