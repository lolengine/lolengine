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

#if !defined __EASYMESH_EASYMESH_H__
#define __EASYMESH_EASYMESH_H__

namespace lol
{

//Utility enum for renderers
struct MeshRender
{
    enum Value
    {
        NeedInit,
        CanRender,
        IgnoreRender
    }
    m_value;

    inline MeshRender(Value v) : m_value(v) {}
    inline MeshRender() : m_value(NeedInit) {}
    inline operator Value() { return m_value; }
};

//Vertex datas for easymesh vertex list.
//TODO : <COORD, NORM, COLOR, UV>
struct VertexData
{
    vec3    m_coord;
    vec3    m_normal;
    vec4    m_color;
    vec4    m_texcoord;
    ivec4   m_bone_id;
    vec4    m_bone_weight;

    VertexData(vec3  new_coord      = vec3(0.f),
               vec3  new_normal     = vec3(0.f, 1.f, 0.f),
               vec4  new_color      = vec4(0.f),
               vec4  new_texcoord   = vec4(0.f),
               ivec4 new_bone_id    = ivec4(0),
               vec4  new_bone_weight= vec4(0.f))
    {
        m_coord         = new_coord;
        m_normal        = new_normal;
        m_color         = new_color;
        m_texcoord      = new_texcoord;
        m_bone_id       = new_bone_id;
        m_bone_weight   = new_bone_weight;
    }
};

//Base class to declare shader datas
class GpuShaderData
{
    friend class GpuEasyMeshData;

protected:
    GpuShaderData();
public:
    //--
    GpuShaderData(uint16_t vert_decl_flags, Shader* shader, DebugRenderMode render_mode);
    virtual ~GpuShaderData();
    //--
    void AddUniform(const lol::String &new_uniform);
    void AddAttribute(const lol::String &new_attribute, VertexUsage usage, int index);
    ShaderUniform const *GetUniform(const lol::String &uniform);
    ShaderAttrib const *GetAttribute(const lol::String &attribute);
    //--
    virtual void SetupShaderDatas(mat4 const &model) { UNUSED(model); }
    //--
    virtual lol::String GetInVertexName()   { return lol::String("in_Vertex");   }
    virtual lol::String GetInNormalName()   { return lol::String("in_Normal");   }
    virtual lol::String GetInColorName()    { return lol::String("in_Color");    }
    virtual lol::String GetInTexCoordName() { return lol::String("in_TexCoord"); }

protected:
    uint16_t                            m_vert_decl_flags;
    Shader*                             m_shader;
    DebugRenderMode                     m_render_mode;
    Array<lol::String, ShaderUniform>   m_shader_uniform;
    Array<lol::String, ShaderAttrib>    m_shader_attrib;
};

class DefaultShaderData : public GpuShaderData
{
public:
    //---
    DefaultShaderData(DebugRenderMode render_mode);
    DefaultShaderData(uint16_t vert_decl_flags, Shader* shader, bool with_UV);
    virtual ~DefaultShaderData() {}
    //---
    void SetupDefaultData(bool with_UV);
    virtual void SetupShaderDatas(mat4 const &model);
};

class GpuEasyMeshData
{
public:
    //---
    GpuEasyMeshData();
    ~GpuEasyMeshData();
    //---
    void AddGpuData(GpuShaderData* gpudata, class EasyMesh* src_mesh);
    void RenderMeshData(mat4 const &model);

private:
    void SetupVertexData(uint16_t vdecl_flags, EasyMesh* src_mesh);

    Array<GpuShaderData*>               m_gpudatas;
    //uint16_t are the vdecl/vbo flags to avoid copy same vdecl several times.
    Array<uint16_t, VertexDeclaration*,
                    VertexBuffer*>      m_vdatas;
    int                                 m_vertexcount;
    //We only need only one ibo for the whole mesh
    IndexBuffer *                       m_ibo;
    int                                 m_indexcount;
};


struct MeshBuildOperation
{
    enum Value
    {
        //When this flag is up, negative scaling will not invert faces.
        Scale_Winding   = 1 << 0,

        All     = 0xffffffff
    }
    m_value;

    inline MeshBuildOperation(Value v) : m_value(v) {}
    inline MeshBuildOperation(uint64_t i) : m_value((Value)i) {}
    inline operator Value() { return m_value; }
};

struct MeshType
{
    enum Value
    {
        Triangle = 0,
        Quad,
        Box,
        Sphere,
        Capsule,
        Torus,
        Cylinder,
        Disc,
        Star,
        ExpandedStar,
        Cog,

        Max
    }
    m_value;

    inline MeshType(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

//TODO : Add other Build type
struct TexCoordBuildType
{
    enum Value
    {
        TriangleDefault     = 0,
        QuadDefault         = 0,
        BoxDefault          = 0,
        SphereDefault       = 0,
        CapsuleDefault      = 0,
        TorusDefault        = 0,
        CylinderDefault     = 0,
        DiscDefault         = 0,
        StarDefault         = 0,
        ExpandedStarDefault = 0,
        CogDefault          = 0,

        //NEVER FORGET TO INCREMENT THIS WHEN ADDING A VALUE
        Max = 1
    }
    m_value;

    inline TexCoordBuildType() : m_value(TriangleDefault) {}
    inline TexCoordBuildType(Value v) : m_value(v) {}
    inline TexCoordBuildType(int v) : m_value((Value)v) {}
    inline operator Value() { return m_value; }
};

struct MeshFaceType
{
    enum Value
    {
        BoxFront = 0,
        BoxLeft  = 1,
        BoxBack  = 2,
        BoxRight = 3,
        BoxTop    = 4,
        BoxBottom  = 5,
        QuadDefault = 0,

        //NEVER FORGET TO INCREMENT THIS WHEN ADDING A VALUE
        Max = 6
    }
    m_value;

    inline MeshFaceType(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

struct TexCoordPos
{
    enum Value
    {
        BL, //BottomLeft
        BR, //BottomRight
        TL, //TopLeft
        TR  //TopRight
    }
    m_value;

    inline TexCoordPos(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

class EasyMeshBuildData
{
public:
    EasyMeshBuildData()
    {
        m_color = vec4(0.f, 0.f, 0.f, 1.f);
        m_color2 = vec4(0.f, 0.f, 0.f, 1.f);
        m_texcoord_offset = vec2(0.f);
        m_texcoord_offset2 = vec2(0.f);
        m_texcoord_scale = vec2(1.f);
        m_texcoord_scale2 = vec2(1.f);
        m_build_flags = 0;
        for (int i = 0; i < MeshType::Max; ++i)
        {
            m_texcoord_build_type[i] = TexCoordBuildType::TriangleDefault;
            m_texcoord_build_type2[i] = TexCoordBuildType::TriangleDefault;
        }
    }

    inline vec4 &Color()           { return m_color; }
    inline vec4 &Color2()          { return m_color2; }
    inline vec2 &TexCoordOffset()  { return m_texcoord_offset; }
    inline vec2 &TexCoordScale()   { return m_texcoord_scale; }
    inline vec2 &TexCoordOffset2() { return m_texcoord_offset2; }
    inline vec2 &TexCoordScale2()  { return m_texcoord_scale2; }

    //UV1
    void SetTexCoordBuildType(MeshType mt, TexCoordBuildType tcbt) { m_texcoord_build_type[mt] = (1 << (tcbt + 1)) | (m_texcoord_build_type[mt] & 1); }
    TexCoordBuildType GetTexCoordBuildType(MeshType mt)
    {
        uint32_t flag = (uint32_t)((m_texcoord_build_type[mt] & ~(1)) >> 1);
        int i = 0;
        while (flag >>= 1)
            i++;
        return TexCoordBuildType(i);
    }
    void SetTexCoordCustomBuild(MeshType mt, MeshFaceType face, vec2 BL, vec2 TR)
    {
        if (face >= m_texcoord_custom_build[mt].Count())
            m_texcoord_custom_build[mt].Resize(face + 1);
        m_texcoord_custom_build[mt][face].m1 = BL;
        m_texcoord_custom_build[mt][face].m2 = TR;
        m_texcoord_build_type[mt] |= 1;
    }
    void ClearTexCoordCustomBuild(MeshType mt) { m_texcoord_build_type[mt] &= ~1; }
    /* FIXME : Do something better ? */
    vec2 TexCoord(MeshType mt, TexCoordPos tcp, MeshFaceType face)
    {
        vec2 BL = vec2(0.f);
        vec2 TR = vec2(0.f);
        if (m_texcoord_build_type[mt] & 1 && face < m_texcoord_custom_build[mt].Count())
        {
            BL = m_texcoord_custom_build[mt][face].m1;
            TR = m_texcoord_custom_build[mt][face].m2;
        }
        else
        {
            /* unused for now, but will be if new BuildType are added. */
            TexCoordBuildType tcbt = GetTexCoordBuildType(mt);
            UNUSED(tcbt);
            if (mt == MeshType::Triangle)
                mt = mt;
            else if (mt == MeshType::Quad)
            {
                //There's nothin' else than QuadDefault
                BL = vec2(0.f);
                TR = vec2(1.f);
            }
            else if (mt == MeshType::Box)
            {
                vec2 data[][2] =
                { //TexCoordBuildType::BoxDefault
                    { vec2(0.f), vec2(.5f) },
                    { vec2(.5f, 0.f), vec2(1.f, .5f) },
                    { vec2(0.f), vec2(.5f) },
                    { vec2(.5f, 0.f), vec2(1.f, .5f) },
                    { vec2(0.f, .5f), vec2(.5f, 1.f) },
                    { vec2(.5f, .5f), vec2(1.f, 1.f) }
                };
                BL = data[face][0]; //[tcbt]
                TR = data[face][1]; //[tcbt]
            }
            else if (mt == MeshType::Sphere)
                mt = mt;
            else if (mt == MeshType::Capsule)
                mt = mt;
            else if (mt == MeshType::Torus)
                mt = mt;
            else if (mt == MeshType::Cylinder)
                mt = mt;
            else if (mt == MeshType::Disc)
                mt = mt;
            else if (mt == MeshType::Star)
                mt = mt;
            else if (mt == MeshType::ExpandedStar)
                mt = mt;
            else if (mt == MeshType::Cog)
                mt = mt;
        }

        vec2 res = vec2(.0f);
        if (tcp == TexCoordPos::BL)
            res = BL;
        else if (tcp == TexCoordPos::BR)
            res = vec2(TR.x, BL.y);
        else if (tcp == TexCoordPos::TL)
            res = vec2(BL.x, TR.y);
        else if (tcp == TexCoordPos::TR)
            res = TR;

        return res * m_texcoord_scale + m_texcoord_offset2;
    }

    //UV2
    void SetTexCoordBuildType2(MeshType mt, TexCoordBuildType tcbt) { m_texcoord_build_type2[mt] = (1 << (tcbt + 1)) | (m_texcoord_build_type2[mt] & 1); }
    TexCoordBuildType GetTexCoordBuildType2(MeshType mt)
    {
        uint32_t flag = ((m_texcoord_build_type2[mt] & ~(1)) >> 1);
        int i = 0;
        while (flag >>= 1)
            i++;
        return TexCoordBuildType(i);
    }
    void SetTexCoordCustomBuild2(MeshType mt, MeshFaceType face, vec2 BL, vec2 TR)
    {
        if (face >= m_texcoord_custom_build2[mt].Count())
            m_texcoord_custom_build2[mt].Resize(face + 1);
        m_texcoord_custom_build2[mt][face].m1 = BL;
        m_texcoord_custom_build2[mt][face].m2 = TR;
        m_texcoord_build_type2[mt] |= 1;
    }
    void ClearTexCoordCustomBuild2(MeshType mt) { m_texcoord_build_type2[mt] &= ~1; }
    vec2 TexCoord2(MeshType mt, TexCoordPos tcp, MeshFaceType face)
    {
        vec2 BL = vec2(0.f);
        vec2 TR = vec2(0.f);
        if (m_texcoord_build_type2[mt] & 1 && face < m_texcoord_custom_build2[mt].Count())
        {
            BL = m_texcoord_custom_build2[mt][face].m1;
            TR = m_texcoord_custom_build2[mt][face].m2;
        }
        else
        {
            TexCoordBuildType tcbt = GetTexCoordBuildType2(mt);
            UNUSED(tcbt);
            if (mt == MeshType::Triangle)
                mt = mt;
            else if (mt == MeshType::Quad)
            {
                //There's nothin' else than QuadDefault
                BL = vec2(0.f);
                TR = vec2(1.f);
            }
            else if (mt == MeshType::Box)
            {
                vec2 data[][2] =
                { //TexCoordBuildType::BoxDefault
                    { vec2(0.f), vec2(.5f) },
                    { vec2(.5f, 0.f), vec2(1.f, .5f) },
                    { vec2(0.f), vec2(.5f) },
                    { vec2(.5f, 0.f), vec2(1.f, .5f) },
                    { vec2(0.f, .5f), vec2(.5f, 1.f) },
                    { vec2(.5f, .5f), vec2(1.f, 1.f) }
                };
                BL = data[face][0]; //[tcbt]
                TR = data[face][1]; //[tcbt]
            }
            else if (mt == MeshType::Sphere)
                mt = mt;
            else if (mt == MeshType::Capsule)
                mt = mt;
            else if (mt == MeshType::Torus)
                mt = mt;
            else if (mt == MeshType::Cylinder)
                mt = mt;
            else if (mt == MeshType::Disc)
                mt = mt;
            else if (mt == MeshType::Star)
                mt = mt;
            else if (mt == MeshType::ExpandedStar)
                mt = mt;
            else if (mt == MeshType::Cog)
                mt = mt;
        }

        vec2 res = vec2(.0f);
        if (tcp == TexCoordPos::BL)
            res = BL;
        else if (tcp == TexCoordPos::BR)
            res = vec2(TR.x, BL.y);
        else if (tcp == TexCoordPos::TL)
            res = vec2(BL.x, TR.y);
        else if (tcp == TexCoordPos::TR)
            res = TR;

        return res * m_texcoord_scale + m_texcoord_offset2;
    }

    inline bool IsEnabled(MeshBuildOperation mbo) { return (m_build_flags & mbo) != 0; }
    inline void Enable(MeshBuildOperation mbo) { m_build_flags |= mbo; }
    inline void Disable(MeshBuildOperation mbo) { m_build_flags &= ~mbo; }
    inline void Toggle(MeshBuildOperation mbo) { m_build_flags ^= mbo; }
    inline void Set(MeshBuildOperation mbo, bool value) { if (value) Enable(mbo); else Disable(mbo); }

public:
    vec4                m_color;
    vec4                m_color2;
    vec2                m_texcoord_offset;
    vec2                m_texcoord_offset2;
    vec2                m_texcoord_scale;
    vec2                m_texcoord_scale2;
    Array<vec2, vec2>   m_texcoord_custom_build[MeshType::Max];
    Array<vec2, vec2>   m_texcoord_custom_build2[MeshType::Max];
    uint32_t            m_texcoord_build_type[MeshType::Max];
    uint32_t            m_texcoord_build_type2[MeshType::Max];
    uint32_t            m_build_flags;
};

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

/* TODO : replace VDict by a proper Half-edge system */
//a class whose goal is to keep a list of the adjacent vertices for mesh operations purposes
class VertexDictionnary
{
public:
    int FindVertexMaster(const int search_idx);
    bool FindMatchingVertices(const int search_idx, Array<int> &matching_ids);
    bool FindConnectedVertices(const int search_idx, const Array<uint16_t> &tri_list, const int tri0, Array<int> &connected_vert, Array<int> const *ignored_tri = nullptr);
    bool FindConnectedTriangles(const int search_idx, const Array<uint16_t> &tri_list, const int tri0, Array<int> &connected_tri, Array<int> const *ignored_tri = nullptr);
    bool FindConnectedTriangles(const ivec2 &search_idx, const Array<uint16_t> &tri_list, const int tri0, Array<int> &connected_tri, Array<int> const *ignored_tri = nullptr);
    bool FindConnectedTriangles(const ivec3 &search_idx, const Array<uint16_t> &tri_list, const int tri0, Array<int> &connected_tri, Array<int> const *ignored_tri = nullptr);
    void AddVertex(int vert_id, vec3 vert_coord);
    bool GetMasterList(Array<int> &ret_master_list) { ret_master_list = master_list; return ret_master_list.Count() > 0; }
    void Clear() { vertex_list.Empty(); }
private:
    //<VertexId, VertexLocation, VertexMasterId>
    Array<int, vec3, int>   vertex_list;
    //List of the master_ vertices
    Array<int>              master_list;
};

struct Axis
{
    enum Value
    {
        X,
        Y,
        Z
    }
    m_value;

    inline Axis(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

class EasyMesh
{
    friend class EasyMeshParser;

public:
    EasyMesh();

    bool Compile(char const *command);
    void MeshConvert(GpuShaderData* new_gpu_sdata);
    void MeshConvert(Shader* ProvidedShader = nullptr);
    void Render(mat4 const &model);

private:
    void UpdateVertexDict(Array< int, int > &vertex_dict);

    //-------------------------------------------------------------------------
    //Mesh CSG operations
    //-------------------------------------------------------------------------
private:
    void MeshCsg(CSGUsage csg_operation);
public:
    /* [cmd:csgu] Performs a Union operation as (mesh0_Outside + mesh1_Outside) */
    void CsgUnion()         { MeshCsg(CSGUsage::Union); }
    /* [cmd:csgs] Performs a Substract operation as (mesh0_Outside + mesh1_Inside-inverted) */
    void CsgSubstract()     { MeshCsg(CSGUsage::Substract); }
    /* [cmd:csgsl] Performs a Substract operation without keeping the mesh1 part */
    void CsgSubstractLoss() { MeshCsg(CSGUsage::SubstractLoss); }
    /* [cmd:csga] Performs an And operation as (mesh0_Inside + mesh1_Inside) */
    void CsgAnd()           { MeshCsg(CSGUsage::And); }
    /* [cmd:csgx] Performs a Xor operation as (m0_Outside/m0_Inside-inverted + m1_Outside/m1_Inside-inverted) */
    void CsgXor()           { MeshCsg(CSGUsage::Xor); }

public:
    /* [cmd:[] from this point onward, any operation will not be performed on previous vertices */
    void OpenBrace();
    /* [cmd:]] Merge current vertices with previous context */
    void CloseBrace();
    /* [cmd:tsw] When activation, on negative-scaling, normal fixing will not occur */
    void ToggleScaleWinding();
    /* [cmd:sc] Set vertices color */
    void SetCurColor(vec4 const &color);
    /* [cmd:scb] Set vertices color 2 */
    void SetCurColor2(vec4 const &color);

private:
    //-------------------------------------------------------------------------
    //Internal : Basic triangle/vertex operations
    //-------------------------------------------------------------------------
    void AddVertex(vec3 const &coord);
    void AddDuplicateVertex(int i);
    void AddLerpVertex(int i, int j, float alpha);
    void AppendQuad(int i1, int i2, int i3, int i4, int base);
    void AppendQuadDuplicateVerts(int i1, int i2, int i3, int i4, int base);
    void AppendTriangle(int i1, int i2, int i3, int base);
    void AppendTriangleDuplicateVerts(int i1, int i2, int i3, int base);
    void ComputeNormals(int start, int vcount);
public: //DEBUG
    void ComputeTexCoord(float uv_scale, int uv_offset);

    //-------------------------------------------------------------------------
    //Vertices operations
    //-------------------------------------------------------------------------
    void SetVertColor(vec4 const &color);
    void SetTexCoordData(vec2 const &new_offset, vec2 const &new_scale);
    void SetTexCoordData2(vec2 const &new_offset, vec2 const &new_scale);

    void SetCurVertNormal(vec3 const &normal);
    void SetCurVertColor(vec4 const &color);
    void SetCurVertTexCoord(vec2 const &texcoord);
    void SetCurVertTexCoord2(vec2 const &texcoord);

public:
    //-------------------------------------------------------------------------
    //Mesh transform operations
    //-------------------------------------------------------------------------

    /* [cmd:t/tx/ty/tz] Translate vertices
        - v : Translation quantity.
     */
    void Translate(vec3 const &v);
    /* See Rotate */
    void RotateX(float angle);
    /* See Rotate */
    void RotateY(float angle);
    /* See Rotate */
    void RotateZ(float angle);
    /* [cmd:r/rx/ry/rz] Rotate vertices
        - angle : rotation quantity.
        - axis : rotation axis.
     */
    void Rotate(float angle, vec3 const &axis);
    /* [cmd:rj] Randomly move vertices along Origin-to-vertex as o2v *= (1.0 + rand(r))
        - r : jitter maximum value.
     */
    void RadialJitter(float r);
    /* [cmd:tax] multiply axis y&z by x as y *= (1.0 + (ny * x + xoff))
        - ny : value of n for y.
        - nz : value of n for z.
        - xoff : value of xoff.
        - absolute (def:1) : if (1) Multiply will use an absolute x.
     */
    void TaperX(float ny, float nz, float xoff, int absolute=1);
    /* [cmd:tay] Same as TaperX, with Y */
    void TaperY(float nx, float nz, float yoff, int absolute=1);
    /* [cmd:taz] Same as TaperX, with Z */
    void TaperZ(float nx, float ny, float zoff, int absolute=1);
    /* [cmd:twx] Twist vertices around x axis with x as rotation value as p = (RotateX(x * t + toff) * p)
        - t : Angle multiplier.
        - toff : Applied offset.
     */
    void TwistX(float t, float toff);
    /* [cmd:twy] Same as TwistX, with Y */
    void TwistY(float t, float toff);
    /* [cmd:twz] Same as TwistX, with Z */
    void TwistZ(float t, float toff);
    /* [cmd:shx] Shear vertices using x value as shear quantity as y += (ny * x + xoff)
        - ny : Value of n for y.
        - nz : Value of n for z.
        - xoff : Value of xoff.
        - absolute (def:1) : if (1) Multiply will use an absolute x.
     */
    void ShearX(float ny, float nz, float xoff, int absolute=1);
    /* [cmd:shy] Same as ShearX, with Y */
    void ShearY(float nx, float nz, float yoff, int absolute=1);
    /* [cmd:shz] Same as ShearX, with Z */
    void ShearZ(float nx, float ny, float zoff, int absolute=1);
    /* [cmd:stx] Stretch vertices using x value as stretch quantity as y += (pow(x, ny) + xoff)
        - ny : Value of n for y.
        - nz : Value of n for z.
        - xoff : Value of xoff.
     */
    void StretchX(float ny, float nz, float xoff);
    /* [cmd:sty] Same as StretchX, with Y */
    void StretchY(float nx, float nz, float yoff);
    /* [cmd:stz] Same as StretchX, with Z */
    void StretchZ(float nx, float ny, float zoff);
    /* [cmd:bdxy] Bend vertices using x as bend quantity along y axis using p = (RotateY(x * t + toff) * p)
        - t : Angle multiplier.
        - xoff : Applied offset.
     */
    void BendXY(float t, float toff);
    /* [cmd:bdxz] Same as BendXY, with X & Z */
    void BendXZ(float t, float toff);
    /* [cmd:bdyx] Same as BendXY, with Y & X */
    void BendYX(float t, float toff);
    /* [cmd:bdyz] Same as BendXY, with Y & Z */
    void BendYZ(float t, float toff);
    /* [cmd:bdzx] Same as BendXY, with Z & X */
    void BendZX(float t, float toff);
    /* [cmd:bdzy] Same as BendXY, with Z & Y */
    void BendZY(float t, float toff);
private:
    struct MeshTransform
    {
        enum Value
        {
            Taper,
            Twist,
            Bend,
            Stretch,
            Shear
        }
        m_value;

        inline MeshTransform(Value v) : m_value(v) {}
        inline operator Value() { return m_value; }
    };
    void DoMeshTransform(MeshTransform ct, Axis axis0, Axis axis1, float n0, float n1, float noff, int absolute);
public:
    /* [cmd:s/sx/sy/sz] Scale vertices
        - s : scale quantity.
     */
    void Scale(vec3 const &s);
    /* [cmd:mx] Mirror vertices through X-plane
        Acts as an OpenBrace
     */
    void MirrorX();
    /* [cmd:my] Mirror vertices through Y-plane
        Acts as an OpenBrace
     */
    void MirrorY();
    /* [cmd:mz] Mirror vertices through Z-plane
        Acts as an OpenBrace
     */
    void MirrorZ();
    /* [no-cmd] Duplicates vertices and scale duplicate
        Acts as an OpenBrace
     */
    void DupAndScale(vec3 const &s);
    /* [cmd:ch] Performs a chamfer operation //TODO : Make it work.
        - f : Chamfer quantity.
     */
    void Chamfer(float f);
    /* [cmd:splt] split triangles in 4 smaller ones.
        - pass : Number of pass applied.
     */
    void SplitTriangles(int pass);
private:
    void SplitTriangles(int pass, VertexDictionnary *vert_dict);
public:
    /* [cmd:smth] Smooth the mesh by subdivising it.
        - main_pass : a main pass is made of (n0 split then n1 smooth) repeat.
        - split_per_main_pass : n0 value in above explanation.
        - smooth_per_main_pass : n1 value in above explanation.
     */
    void SmoothMesh(int main_pass, int split_per_main_pass, int smooth_per_main_pass);

    //-------------------------------------------------------------------------
    //Mesh shape operations
    //-------------------------------------------------------------------------

    /* [cmd:ac] Cylinder centered on (0,0,0) with BBox [-.5*max(d1, d2), -.5*h, -.5*max(d1, d2)]
        - nbsides : Number of sides.                   [+.5*max(d1, d2), +.5*h, +.5*max(d1, d2)]
        - h : Height of the cylinder.
        - d1 : Lower diameter.
        - d2 : Upper diameter.
        - dualside : if (1) will also create inner sides : TOOD:TOREMOVE?? : needed ?
        - smooth : if (1) will smooth normals : TOOD:TOREMOVE : smooth should be handled elsewhere
        - close : if (1) will add discs to close the cylinder
     */
    void AppendCylinder(int nsides, float h, float d1, float d2,
                        int dualside, int smooth, int close);
    /* [cmd:asph] Sphere centered on (0,0,0) with BBox [-.5*d][.5*d]
        - ndivisions : number of subdivisions each Sphere triangle will sustain.
        - d : Diameter.
     */
    void AppendSphere(int ndivisions, float d);
    /* [cmd:acap] Capsule centered on (0,0,0) with BBox [-.5*d, -(.5*d+h), -.5*d][.5*d, (.5*d+h), .5*d]
        - ndivisions : number of subdivisions each Sphere triangle will sustain.
        - h : Inner height.
        - d : Diameter.
     */
    void AppendCapsule(int ndivisions, float h, float d);
    /* [cmd:ato] Torus centered on (0,0,0) with BBox [-.5*d2][.5*d2]
        - ndivisions : number of subdivisions of the torus.
        - d1 : Inner diameter.
        - d2 : Outer diameter.
     */
    void AppendTorus(int ndivisions, float d1, float d2);
    /* [cmd:ab] Box centered on (0,0,0) with BBox [-.5 * size][.5 * size]
        - size : size of the box.
        - chamf : size of the chamfer.
        - smooth : if (1) will smooth normals : TOOD:TOREMOVE : smooth should be handled elsewhere
     */
    void AppendBox(vec3 const &size, float chamf = 0.f);
    //Same as AppendBox
    void AppendSmoothChamfBox(vec3 const &size, float chamf);
    //Same as AppendBox
    void AppendFlatChamfBox(vec3 const &size, float chamf);
    //Same as AppendBox
    void AppendBox(vec3 const &size, float chamf, bool smooth);
    /* [cmd:as]
       Append a Star centered on (0,0,0) contained within a disc of "max(d1, d2)" diameter.
        - nbranches : Number of branches.
        - d1 : double Length of the branches.
        - d2 : double Length of the "branch" located between d1-branches.
        - fade : if (1) in-between branches use Color2.
        - fade2 : if (1) Star branches use Color2.
     */
    void AppendStar(int nbranches, float d1, float d2,
                    int fade = 0, int fade2 = 0);
    /* [cmd:aes] Star centered on (0,0,0) contained within a disc of "max(max(d1, d2), max(d1 + extrad, d2 + extrad))" diameter.
       Expanded star branches use Color2.
        - nbranches : Number of branches.
        - d1 : Double Length of the branches.
        - d2 : Double Length of the "branch" located between r1-branches.
        - extrad : Extra length added to expand all branches.
     */
    void AppendExpandedStar(int nbranches, float d1, float d2, float extrad);
    /* [cmd:ad] Disc centered on (0,0,0) with d diameter.
        - nbsides : Number of sides.
        - d : Diameter.
        - fade : if (1) Outer vertices will use Color2
     */
    void AppendDisc(int nsides, float d, int fade = 0);
    /* [cmd:at] Triangle centered on (0,0,0) contained within a disc of "d" diameter.
        - d : diameter of the containing disc..
        - fade : if (1) 2nd & 3rd Vertices will use Color2
     */
    void AppendSimpleTriangle(float d, int fade = 0);
    /* [cmd:aq] Quad centered on (0,0,0) contained within BBox [-size*.5f, 0, -size*.5f][size*.5f, 0, size*.5f]
        - size : Size of quad.
        - fade : if (1) 3rd & 4th Vertices will use Color2
     */
    void AppendSimpleQuad(float size, int fade = 0);
private:
    //complex version of above one
    void AppendSimpleQuad(vec2 p1, vec2 p2, float z = 0.f, int fade = 0);
public:
    /* [cmd:acg] Gear centered on (0,0,0) contained within BBox [-.5*max(d1,d2), -.5*h, -.5*max(d1, d2)]
        - h : Height of the Gear.                               [+.5*max(d1,d2), +.5*h, +.5*max(d1, d2)]
        - d10 : Upper Inner diameter.
        - d20 : Lower Inner diameter.
        - d1  : Upper Outer diameter.
        - d2  : Lower Outer diameter.
        - d12 : Upper Cog diameter.
        - d22 : Lower Cog diameter.
        - sidemul : multiplier for the size of the cogs.
        - offset : useless
     */
    void AppendCog(int nbsides, float h, float d10, float d20, float d1,
                   float d2, float d12, float d22, float sidemul, int offset);

    //-------------------------------------------------------------------------
    //TODO : Mesh Bone operations
    //-------------------------------------------------------------------------
    //void AddBone(int parent_id) {}

    //Convenience functions
public:
    int GetVertexCount() { return m_vert.Count(); }
    vec3 const &GetVertexLocation(int i) { return m_vert[i].m_coord; }

private:
    Array<uint16_t> m_indices;
    Array<VertexData> m_vert;

    //<vert count, indices count>
    Array<int, int> m_cursors;

    friend class GpuEasyMeshData;
    GpuEasyMeshData m_gpu_data;

public:
    inline EasyMeshBuildData* BD()
    {
        if (!m_build_data)
            m_build_data = new EasyMeshBuildData();
        return m_build_data;
    };
private:
    class EasyMeshBuildData* m_build_data;
};
} /* namespace lol */

#endif /* __EASYMESH_EASYMESH_H__ */

