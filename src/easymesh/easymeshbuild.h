//
// Lol Engine
//
// Copyright: (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Cédric Lecacheur <jordx@free.fr>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The EasyMesh class
// ------------------
//

#if !defined __EASYMESHBUILD_EASYMESHBUILD_H__
#define __EASYMESHBUILD_EASYMESHBUILD_H__

namespace lol
{

struct MeshBuildOperation
{
    enum Value
    {
        //When this flag is up, negative scaling will not invert faces.
        ScaleWinding                = (1 << 0),
        CommandRecording            = (1 << 1),
        CommandExecution            = (1 << 2),
        QuadWeighting               = (1 << 3),
        IgnoreQuadWeighting         = (1 << 4),
        PostBuildComputeNormals     = (1 << 5),
        PreventVertCleanup          = (1 << 6),

        All     = 0xffffffff
    }
    m_value;

    inline MeshBuildOperation(Value v) : m_value(v) {}
    inline MeshBuildOperation(uint64_t i) : m_value((Value)i) {}
    inline operator Value() { return m_value; }
};

struct EasyMeshCmdType
{
    enum Value
    {
        MeshCsg = 0,

        LoopStart,
        LoopEnd,
        OpenBrace,
        CloseBrace,

        ScaleWinding,
        QuadWeighting,
        PostBuildNormal,
        PreventVertCleanup,
        SetColorA,
        SetColorB,
        SetVertColor,

        VerticesMerge,
        VerticesSeparate,

        Translate,
        Rotate,
        RadialJitter,
        MeshTranform,
        Scale,
        DupAndScale,
        Chamfer,

        SplitTriangles,
        SmoothMesh,

        AppendCylinder,
        AppendCapsule,
        AppendTorus,
        AppendBox,
        AppendStar,
        AppendExpandedStar,
        AppendDisc,
        AppendSimpleTriangle,
        AppendSimpleQuad,
        AppendCog,

        Max
    }
    m_value;

    inline EasyMeshCmdType(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
    inline int Value() { return m_value; }
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
        m_color_a = vec4(0.f, 0.f, 0.f, 1.f);
        m_color_b = vec4(0.f, 0.f, 0.f, 1.f);
        m_texcoord_offset = vec2(0.f);
        m_texcoord_offset2 = vec2(0.f);
        m_texcoord_scale = vec2(1.f);
        m_texcoord_scale2 = vec2(1.f);
        m_build_flags = 0;
        m_i_cmd = 0;
        m_exec_nb = -1;
        for (int i = 0; i < MeshType::Max; ++i)
        {
            m_texcoord_build_type[i] = TexCoordBuildType::TriangleDefault;
            m_texcoord_build_type2[i] = TexCoordBuildType::TriangleDefault;
        }
    }

    inline CommandStack &CmdStack() { return m_stack; }
    inline int &Cmdi()              { return m_i_cmd; }
    inline int &CmdExecNb()         { return m_exec_nb; }
    inline Array<int, int> &LoopStack(){ return m_loop_stack; }
    inline vec4 &ColorA()           { return m_color_a; }
    inline vec4 &ColorB()           { return m_color_b; }
    inline vec2 &TexCoordOffset()   { return m_texcoord_offset; }
    inline vec2 &TexCoordScale()    { return m_texcoord_scale; }
    inline vec2 &TexCoordOffset2()  { return m_texcoord_offset2; }
    inline vec2 &TexCoordScale2()   { return m_texcoord_scale2; }

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
    CommandStack        m_stack;
    int                 m_i_cmd;
    int                 m_exec_nb;
    Array<int, int>     m_loop_stack;
    vec4                m_color_a;
    vec4                m_color_b;
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
    void RemoveVertex(int vert_id);
    bool GetMasterList(Array<int> &ret_master_list) { ret_master_list = master_list; return ret_master_list.Count() > 0; }
    void Clear() { vertex_list.Empty(); }
private:
    //<VertexId, VertexLocation, VertexMasterId>
    Array<int, vec3, int>   vertex_list;
    //List of the master_ vertices
    Array<int>              master_list;
};

} /* namespace lol */

#endif /* __EASYMESHBUILD_EASYMESHBUILD_H__ */

