//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The EasyMesh class
// ------------------
//

#define VU_BONES   2
#define VU_TEX_UV  1
#define VU_VANILLA 0

#define VERTEX_USEAGE VU_VANILLA

#if !defined __EASYMESH_EASYMESH_H__
#define __EASYMESH_EASYMESH_H__

namespace lol
{

/* A safe enum for MeshCSG operations. */
struct CSGUsage
{
    enum Value
    {
        Union,
        Substract,
        SubstractLoss, //will remove B from A, but not add inverted B
        And,
        Xor,
    }
    m_value;

    inline CSGUsage(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

/* A safe enum for VertexDictionnary operations. */
struct VDictType
{
    enum Value
    {
        DoesNotExist=-3,
        Alone=-2,
        Master=-1
    }
    m_value;

    inline VDictType(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

//a class whose goal is to keep a list of the adjacent vertices for mesh operations purposes
class VertexDictionnary
{
public:
    int FindVertexMaster(const int search_idx);
    bool FindMatchingVertices(const int search_idx, Array<int> &matching_ids);
    bool FindConnectedVertices(const int search_idx, const Array<int> &tri_list, Array<int> &connected_vert, Array<int> const *ignored_tri = NULL);
    bool FindConnectedTriangles(const int search_idx, const Array<int> &tri_list, Array<int> &connected_tri, Array<int> const *ignored_tri = NULL);
    bool FindConnectedTriangles(const ivec2 &search_idx, const Array<int> &tri_list, Array<int> &connected_tri, Array<int> const *ignored_tri = NULL);
    bool FindConnectedTriangles(const ivec3 &search_idx, const Array<int> &tri_list, Array<int> &connected_tri, Array<int> const *ignored_tri = NULL);
    void AddVertex(int vert_id, vec3 vert_coord);
    void Clear() { vertex_list.Empty(); }
private:
    //<VertexId, VertexLocation, VertexMasterId>
    Array<int, vec3, int>   vertex_list;
    //List of the master_ vertices
    Array<int>              master_list;
};

class EasyMesh
{
    friend class EasyMeshParser;

public:
    EasyMesh();

    bool Compile(char const *command);
    void MeshConvert(Shader* ProvidedShader = NULL);
    void Render(mat4 const &model, float damage = 0.f);

private:
    void UpdateVertexDict(Array< int, int > &vertex_dict);

    //-------------------------------------------------------------------------
    //Mesh CSG operations
    //-------------------------------------------------------------------------
private:
    void MeshCsg(CSGUsage csg_operation);
public:
    void CsgUnion()         { MeshCsg(CSGUsage::Union); }
    void CsgSubstract()     { MeshCsg(CSGUsage::Substract); }
    void CsgSubstractLoss() { MeshCsg(CSGUsage::SubstractLoss); }
    void CsgAnd()           { MeshCsg(CSGUsage::And); }
    void CsgXor()           { MeshCsg(CSGUsage::Xor); }

public:
    void OpenBrace();
    void CloseBrace();

    void ToggleScaleWinding();
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
public:
    void ComputeTexCoord(float uv_scale, int uv_offset);
    void SetVertColor(vec4 const &color);

    void SetCurVertNormal(vec3 const &normal);
    void SetCurVertColor(vec4 const &color);

public:
    //-------------------------------------------------------------------------
    //Mesh transform operations
    //-------------------------------------------------------------------------
    void Translate(vec3 const &v);
    void RotateX(float t);
    void RotateY(float t);
    void RotateZ(float t);
    void Rotate(float t, vec3 const &axis);
    void RadialJitter(float r);
    void TaperX(float y, float z, float xoff);
    void TaperY(float x, float z, float yoff);
    void TaperZ(float x, float y, float zoff);
    void Scale(vec3 const &s);
    void MirrorX();
    void MirrorY();
    void MirrorZ();
    void DupAndScale(vec3 const &s);
    void Chamfer(float f);

    //-------------------------------------------------------------------------
    //Mesh shape operations
    //-------------------------------------------------------------------------
    void AppendCylinder(int nsides, float h, float r1, float r2,
                        int dualside, int smooth);
    void AppendCapsule(int ndivisions, float h, float r);
    void AppendSphere(int ndivisions, vec3 const &size);
    void AppendTorus(int ndivisions, float r1, float r2);
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
    void AppendCog(int nbsides, float h, float r10, float r20, float r1,
                   float r2, float r12, float r22, float sidemul, int offset);

    //-------------------------------------------------------------------------
    //TODO : Mesh Bone operations
    //-------------------------------------------------------------------------
    //void AddBone(int parent_id) {}

    //Convenience functions
public:
    int GetVertexCount() { return m_vert.Count(); }
    vec3 const &GetVertexLocation(int i) { return m_vert[i].m1; }

private:
    vec4 m_color, m_color2;
    Array<uint16_t> m_indices;
#if VERTEX_USEAGE == VU_BONES
    //TODO : -- BONE SUPPORT --
    //TODO : <COORD, NORM, COLOR, BONE_ID, BONE_WEIGHT>
    Array<vec3, vec3, vec4, ivec2, vec2> m_vert;
    //TODO : More bone blend support than 2 ?
#elif VERTEX_USEAGE == VU_TEX_UV
    //TODO : -- UV SUPPORT --
    //TODO : <COORD, NORM, COLOR, UV>
    Array<vec3, vec3, vec4, vec2> m_vert;
#else
    //-- VANILLA --
    //<COORD, NORM, COLOR>
    Array<vec3, vec3, vec4> m_vert;
#endif

    //<vert count, indices count>
    Array<int, int> m_cursors;
    //When this flag is up, negative scaling will not invert faces.
    bool m_ignore_winding_on_scale;

    /* FIXME: put this in a separate class so that we can copy meshes. */
    struct
    {
        /* FIXME: very naughty way of handling debug render modes */
        Array<Shader *>shader;
#if VERTEX_USEAGE == VU_BONES
        //TODO : -- BONE SUPPORT --
        Array<ShaderAttrib> coord, norm, color, bone_id, bone_weight;
#elif VERTEX_USEAGE == VU_TEX_UV
        //-- UV SUPPORT --
        Array<ShaderAttrib> coord, norm, color, tex_coord;
#else
        //-- VANILLA --
        Array<ShaderAttrib> coord, norm, color;
#endif
        Array<ShaderUniform> modelview, invmodelview, view, invview, proj, normalmat, damage, lights;

        VertexDeclaration *vdecl;
        VertexBuffer *vbo;
        IndexBuffer *ibo;
        int vertexcount, indexcount;
    }
    m_gpu;
};

} /* namespace lol */

#endif /* __EASYMESH_EASYMESH_H__ */

