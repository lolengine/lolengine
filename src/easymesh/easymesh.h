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

#if !defined __EASYMESH_EASYMESH_H__
#define __EASYMESH_EASYMESH_H__

namespace lol
{

class EasyMesh
{
    friend class EasyMeshParser;

public:
    EasyMesh();

    bool Compile(char const *command);
    void MeshConvert();
    void Render(mat4 const &model, float damage = 0.f);

    void OpenBrace();
    void CloseBrace();

    void SetCurColor(vec4 const &color);
    void SetCurColor2(vec4 const &color);

private:
    void AddVertex(vec3 const &coord);
    void AddDuplicateVertex(int i);
    void AppendQuad(int i1, int i2, int i3, int i4, int base);
    void AppendQuadDuplicateVerts(int i1, int i2, int i3, int i4, int base);
    void AppendTriangle(int i1, int i2, int i3, int base);
    void AppendTriangleDuplicateVerts(int i1, int i2, int i3, int base);
    void ComputeNormals(int start, int vcount);
    void SetVertColor(vec4 const &color);

public:
    void SetCurVertNormal(vec3 const &normal);
    void SetCurVertColor(vec4 const &color);

    void Translate(vec3 const &v);
    void RotateX(float t);
    void RotateY(float t);
    void RotateZ(float t);
    void Rotate(float t, vec3 const &axis);
    void TaperX(float y, float z, float xoff);
    void TaperY(float x, float z, float yoff);
    void TaperZ(float x, float y, float zoff);
    void Scale(vec3 const &s);
    void MirrorX();
    void MirrorY();
    void MirrorZ();
    void DupAndScale(vec3 const &s);
    void Chamfer(float f);

    void AppendCylinder(int nsides, float h, float r1, float r2,
                        int dualside, int smooth);
    void AppendCapsule(int ndivisions, float h, float r);
    void AppendSphere(int ndivisions, vec3 const &size);
    void AppendBox(vec3 const &size, float chamf = 0.f);
    void AppendSmoothChamfBox(vec3 const &size, float chamf);
    void AppendFlatChamfBox(vec3 const &size, float chamf);
    void AppendBox(vec3 const &size, float chamf, bool smooth);
    void AppendStar(int nbranches, float r1, float r2,
                    int fade = 0, int fade2 = 0);
    void AppendExpandedStar(int nbranches, float r1, float r2, float extrar);
    void AppendDisc(int nsides, float r, int fade = 0);
    void AppendSimpleTriangle(float size, int fade = 0);
    void AppendSimpleQuad(float size, int fade = 0);
    void AppendSimpleQuad(vec2 p1, vec2 p2, float z = 0.f, int fade = 0);
    void AppendCog(int nbsides, float h, float r1, float r2,
                   float r12, float r22, float sidemul, int offset);

private:
    vec4 m_color, m_color2;
    Array<uint16_t> m_indices;
    Array<vec3, vec3, vec4> m_vert;
    Array<int, int> m_cursors;

    /* FIXME: put this in a separate class so that we can copy meshes. */
    struct
    {
        Shader *shader;
        ShaderAttrib coord, norm, color;
        ShaderUniform modelview, proj, normalmat, damage;
        VertexDeclaration *vdecl;
        VertexBuffer *vbo;
        IndexBuffer *ibo;
        int vertexcount, indexcount;
    }
    m_gpu;
};

} /* namespace lol */

#endif /* __EASYMESH_EASYMESH_H__ */

