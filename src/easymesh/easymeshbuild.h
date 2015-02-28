//
// Lol Engine
//
// Copyright: (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Cédric Lecacheur <jordx@free.fr>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The EasyMesh class
// ------------------
//

namespace lol
{

//MeshBuildOperation ----------------------------------------------------------
struct MeshBuildOperationBase : public StructSafeEnum
{
    enum Type
    {
        //When this flag is up, negative scaling will not invert faces.
        ScaleWinding = (1 << 0),
        CommandRecording = (1 << 1),
        CommandExecution = (1 << 2),
        QuadWeighting = (1 << 3),
        IgnoreQuadWeighting = (1 << 4),
        PostBuildComputeNormals = (1 << 5),
        PreventVertCleanup = (1 << 6),

        All = 0xffff,
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[ScaleWinding] = "ScaleWinding";
        enum_map[CommandRecording] = "CommandRecording";
        enum_map[CommandExecution] = "CommandExecution";
        enum_map[QuadWeighting] = "QuadWeighting";
        enum_map[IgnoreQuadWeighting] = "IgnoreQuadWeighting";
        enum_map[PostBuildComputeNormals] = "PostBuildComputeNormals";
        enum_map[PreventVertCleanup] = "PreventVertCleanup";
        enum_map[All] = "All";
        return true;
    }
};
typedef SafeEnum<MeshBuildOperationBase> MeshBuildOperation;

//EasyMeshCmdType -------------------------------------------------------------
struct EasyMeshCmdTypeBase : public StructSafeEnum
{
    enum Type
    {
        MeshCsg,

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
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[MeshCsg] = "MeshCsg";
        enum_map[LoopStart] = "LoopStart";
        enum_map[LoopEnd] = "LoopEnd";
        enum_map[OpenBrace] = "OpenBrace";
        enum_map[CloseBrace] = "CloseBrace";
        enum_map[ScaleWinding] = "ScaleWinding";
        enum_map[QuadWeighting] = "QuadWeighting";
        enum_map[PostBuildNormal] = "PostBuildNormal";
        enum_map[PreventVertCleanup] = "PreventVertCleanup";
        enum_map[SetColorA] = "SetColorA";
        enum_map[SetColorB] = "SetColorB";
        enum_map[SetVertColor] = "SetVertColor";
        enum_map[VerticesMerge] = "VerticesMerge";
        enum_map[VerticesSeparate] = "VerticesSeparate";
        enum_map[Translate] = "Translate";
        enum_map[Rotate] = "Rotate";
        enum_map[RadialJitter] = "RadialJitter";
        enum_map[MeshTranform] = "MeshTranform";
        enum_map[Scale] = "Scale";
        enum_map[DupAndScale] = "DupAndScale";
        enum_map[Chamfer] = "Chamfer";
        enum_map[SplitTriangles] = "SplitTriangles";
        enum_map[SmoothMesh] = "SmoothMesh";
        enum_map[AppendCylinder] = "AppendCylinder";
        enum_map[AppendCapsule] = "AppendCapsule";
        enum_map[AppendTorus] = "AppendTorus";
        enum_map[AppendBox] = "AppendBox";
        enum_map[AppendStar] = "AppendStar";
        enum_map[AppendExpandedStar] = "AppendExpandedStar";
        enum_map[AppendDisc] = "AppendDisc";
        enum_map[AppendSimpleTriangle] = "AppendSimpleTriangle";
        enum_map[AppendSimpleQuad] = "AppendSimpleQuad";
        enum_map[AppendCog] = "AppendCog";
        return true;
    }
};
typedef SafeEnum<EasyMeshCmdTypeBase> EasyMeshCmdType;

//MeshTypeBase ----------------------------------------------------------------
struct MeshTypeBase : public StructSafeEnum
{
    /* A safe enum for Primitive edge face. */
    enum Type
    {
        Triangle,
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

        MAX
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[Triangle] = "Triangle";
        enum_map[Quad] = "Quad";
        enum_map[Box] = "Box";
        enum_map[Sphere] = "Sphere";
        enum_map[Capsule] = "Capsule";
        enum_map[Torus] = "Torus";
        enum_map[Cylinder] = "Cylinder";
        enum_map[Disc] = "Disc";
        enum_map[Star] = "Star";
        enum_map[ExpandedStar] = "ExpandedStar";
        enum_map[Cog] = "Cog";
        enum_map[MAX] = "MAX";
        return true;
    }
};
typedef SafeEnum<MeshTypeBase> MeshType;

//TexCoordBuildType -----------------------------------------------------------
struct TexCoordBuildTypeBase : public StructSafeEnum
{
    enum Type
    {
        TriangleDefault,
        QuadDefault,
        BoxDefault,
        SphereDefault,
        CapsuleDefault,
        TorusDefault,
        CylinderDefault,
        DiscDefault,
        StarDefault,
        ExpandedStarDefault,
        CogDefault,

        //NEVER FORGET TO INCREMENT THIS WHEN ADDING A VALUE
        Max
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[TriangleDefault] = "TriangleDefault";
        enum_map[QuadDefault] = "QuadDefault";
        enum_map[BoxDefault] = "BoxDefault";
        enum_map[SphereDefault] = "SphereDefault";
        enum_map[CapsuleDefault] = "CapsuleDefault";
        enum_map[TorusDefault] = "TorusDefault";
        enum_map[CylinderDefault] = "CylinderDefault";
        enum_map[DiscDefault] = "DiscDefault";
        enum_map[StarDefault] = "StarDefault";
        enum_map[ExpandedStarDefault] = "ExpandedStarDefault";
        enum_map[CogDefault] = "CogDefault";
        enum_map[Max] = "Max";
        return true;
    }
};
typedef SafeEnum<TexCoordBuildTypeBase> TexCoordBuildType;

//MeshFaceType ----------------------------------------------------------------
struct MeshFaceTypeBase : public StructSafeEnum
{
    enum Type
    {
        BoxFront = 0,
        QuadDefault = 0,
        BoxLeft = 1,
        BoxBack = 2,
        BoxRight = 3,
        BoxTop = 4,
        BoxBottom = 5,

        MAX
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[BoxFront] = "BoxFront";
        enum_map[QuadDefault] = "QuadDefault";
        enum_map[BoxLeft] = "BoxLeft";
        enum_map[BoxBack] = "BoxBack";
        enum_map[BoxRight] = "BoxRight";
        enum_map[BoxTop] = "BoxTop";
        enum_map[BoxBottom] = "BoxBottom";
        enum_map[MAX] = "MAX";
        return true;
    }
};
typedef SafeEnum<MeshFaceTypeBase> MeshFaceType;

//TexCoordPos -----------------------------------------------------------------
struct TexCoordPosBase : public StructSafeEnum
{
    enum Type
    {
        BL, // Bottom Left
        BR, // Bottom Right
        TL, // Top Left
        TR  // Top Right
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[BL] = "BL";
        enum_map[BR] = "BR";
        enum_map[TL] = "TL";
        enum_map[TR] = "TR";
        return true;
    }
};
typedef SafeEnum<TexCoordPosBase> TexCoordPos;

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
        for (int i = 0; i < MeshType::MAX; ++i)
        {
            m_texcoord_build_type[i] = TexCoordBuildType::TriangleDefault;
            m_texcoord_build_type2[i] = TexCoordBuildType::TriangleDefault;
        }
    }

    inline CommandStack &CmdStack() { return m_stack; }
    inline int &Cmdi()              { return m_i_cmd; }
    inline int &CmdExecNb()         { return m_exec_nb; }
    inline array<int, int> &LoopStack(){ return m_loop_stack; }
    inline vec4 &ColorA()           { return m_color_a; }
    inline vec4 &ColorB()           { return m_color_b; }
    inline vec2 &TexCoordOffset()   { return m_texcoord_offset; }
    inline vec2 &TexCoordScale()    { return m_texcoord_scale; }
    inline vec2 &TexCoordOffset2()  { return m_texcoord_offset2; }
    inline vec2 &TexCoordScale2()   { return m_texcoord_scale2; }

    //UV1
    void SetTexCoordBuildType(MeshType mt, TexCoordBuildType tcbt) { m_texcoord_build_type[mt.ToScalar()] = (1 << (tcbt.ToScalar() + 1)) | (m_texcoord_build_type[mt.ToScalar()] & 1); }
    TexCoordBuildType GetTexCoordBuildType(MeshType mt)
    {
        uint32_t flag = (uint32_t)((m_texcoord_build_type[mt.ToScalar()] & ~(1)) >> 1);
        int i = 0;
        while (flag >>= 1)
            i++;
        return TexCoordBuildType(i);
    }
    void SetTexCoordCustomBuild(MeshType mt, MeshFaceType face, vec2 BL, vec2 TR)
    {
        if (face.ToScalar() >= m_texcoord_custom_build[mt.ToScalar()].Count())
            m_texcoord_custom_build[mt.ToScalar()].Resize(face.ToScalar() + 1);
        m_texcoord_custom_build[mt.ToScalar()][face.ToScalar()].m1 = BL;
        m_texcoord_custom_build[mt.ToScalar()][face.ToScalar()].m2 = TR;
        m_texcoord_build_type[mt.ToScalar()] |= 1;
    }
    void ClearTexCoordCustomBuild(MeshType mt) { m_texcoord_build_type[mt.ToScalar()] &= ~1; }
    /* FIXME : Do something better ? */
    vec2 TexCoord(MeshType mt, TexCoordPos tcp, MeshFaceType face)
    {
        vec2 BL = vec2(0.f);
        vec2 TR = vec2(0.f);
        if (m_texcoord_build_type[mt.ToScalar()] & 1
             && face.ToScalar() < m_texcoord_custom_build[mt.ToScalar()].Count())
        {
            BL = m_texcoord_custom_build[mt.ToScalar()][face.ToScalar()].m1;
            TR = m_texcoord_custom_build[mt.ToScalar()][face.ToScalar()].m2;
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
                // There's nothin' else than QuadDefault
                BL = vec2(0.f);
                TR = vec2(1.f);
            }
            else if (mt == MeshType::Box)
            {
                vec2 data[][2] =
                { // TexCoordBuildType::BoxDefault
                    { vec2(0.f), vec2(.5f) },
                    { vec2(.5f, 0.f), vec2(1.f, .5f) },
                    { vec2(0.f), vec2(.5f) },
                    { vec2(.5f, 0.f), vec2(1.f, .5f) },
                    { vec2(0.f, .5f), vec2(.5f, 1.f) },
                    { vec2(.5f, .5f), vec2(1.f, 1.f) }
                };
                BL = data[face.ToScalar()][0]; // [tcbt]
                TR = data[face.ToScalar()][1]; // [tcbt]
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

    // UV2
    void SetTexCoordBuildType2(MeshType mt, TexCoordBuildType tcbt) { m_texcoord_build_type2[mt.ToScalar()] = (1 << (tcbt.ToScalar() + 1)) | (m_texcoord_build_type2[mt.ToScalar()] & 1); }
    TexCoordBuildType GetTexCoordBuildType2(MeshType mt)
    {
        uint32_t flag = ((m_texcoord_build_type2[mt.ToScalar()] & ~(1)) >> 1);
        int i = 0;
        while (flag >>= 1)
            i++;
        return TexCoordBuildType(i);
    }
    void SetTexCoordCustomBuild2(MeshType mt, MeshFaceType face, vec2 BL, vec2 TR)
    {
        if (face.ToScalar() >= m_texcoord_custom_build2[mt.ToScalar()].Count())
            m_texcoord_custom_build2[mt.ToScalar()].Resize(face.ToScalar() + 1);
        m_texcoord_custom_build2[mt.ToScalar()][face.ToScalar()].m1 = BL;
        m_texcoord_custom_build2[mt.ToScalar()][face.ToScalar()].m2 = TR;
        m_texcoord_build_type2[mt.ToScalar()] |= 1;
    }
    void ClearTexCoordCustomBuild2(MeshType mt) { m_texcoord_build_type2[mt.ToScalar()] &= ~1; }
    vec2 TexCoord2(MeshType mt, TexCoordPos tcp, MeshFaceType face)
    {
        vec2 BL = vec2(0.f);
        vec2 TR = vec2(0.f);
        if (m_texcoord_build_type2[mt.ToScalar()] & 1
             && face.ToScalar() < m_texcoord_custom_build2[mt.ToScalar()].Count())
        {
            BL = m_texcoord_custom_build2[mt.ToScalar()][face.ToScalar()].m1;
            TR = m_texcoord_custom_build2[mt.ToScalar()][face.ToScalar()].m2;
        }
        else
        {
            TexCoordBuildType tcbt = GetTexCoordBuildType2(mt);
            UNUSED(tcbt);
            if (mt == MeshType::Triangle)
                mt = mt;
            else if (mt == MeshType::Quad)
            {
                // There's nothin' else than QuadDefault
                BL = vec2(0.f);
                TR = vec2(1.f);
            }
            else if (mt == MeshType::Box)
            {
                vec2 data[][2] =
                { // TexCoordBuildType::BoxDefault
                    { vec2(0.f), vec2(.5f) },
                    { vec2(.5f, 0.f), vec2(1.f, .5f) },
                    { vec2(0.f), vec2(.5f) },
                    { vec2(.5f, 0.f), vec2(1.f, .5f) },
                    { vec2(0.f, .5f), vec2(.5f, 1.f) },
                    { vec2(.5f, .5f), vec2(1.f, 1.f) }
                };
                BL = data[face.ToScalar()][0]; // [tcbt]
                TR = data[face.ToScalar()][1]; // [tcbt]
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

    inline bool IsEnabled(MeshBuildOperation mbo) { return (m_build_flags & mbo.ToScalar()) != 0; }
    inline void Enable(MeshBuildOperation mbo) { m_build_flags |= mbo.ToScalar(); }
    inline void Disable(MeshBuildOperation mbo) { m_build_flags &= ~mbo.ToScalar(); }
    inline void Toggle(MeshBuildOperation mbo) { m_build_flags ^= mbo.ToScalar(); }
    inline void Set(MeshBuildOperation mbo, bool value) { if (value) Enable(mbo); else Disable(mbo); }

public:
    CommandStack        m_stack;
    int                 m_i_cmd;
    int                 m_exec_nb;
    array<int, int>     m_loop_stack;
    vec4                m_color_a;
    vec4                m_color_b;
    vec2                m_texcoord_offset;
    vec2                m_texcoord_offset2;
    vec2                m_texcoord_scale;
    vec2                m_texcoord_scale2;
    array<vec2, vec2>   m_texcoord_custom_build[MeshType::MAX];
    array<vec2, vec2>   m_texcoord_custom_build2[MeshType::MAX];
    uint32_t            m_texcoord_build_type[MeshType::MAX];
    uint32_t            m_texcoord_build_type2[MeshType::MAX];
    uint32_t            m_build_flags;
};

//VDictType -- A safe enum for VertexDictionnary operations. ------------------
struct VDictTypeBase : public StructSafeEnum
{
    enum Type
    {
        DoesNotExist = -3,
        Alone = -2,
        Master = -1,
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[DoesNotExist] = "DoesNotExist";
        enum_map[Alone] = "Alone";
        enum_map[Master] = "Master";
        return true;
    }
};
typedef SafeEnum<VDictTypeBase> VDictType;

/* TODO : replace VDict by a proper Half-edge system */
//a class whose goal is to keep a list of the adjacent vertices for mesh operations purposes
class VertexDictionnary
{
public:
    int FindVertexMaster(const int search_idx);
    bool FindMatchingVertices(const int search_idx, array<int> &matching_ids);
    bool FindConnectedVertices(const int search_idx, const array<uint16_t> &tri_list, const int tri0, array<int> &connected_vert, array<int> const *ignored_tri = nullptr);
    bool FindConnectedTriangles(const int search_idx, const array<uint16_t> &tri_list, const int tri0, array<int> &connected_tri, array<int> const *ignored_tri = nullptr);
    bool FindConnectedTriangles(const ivec2 &search_idx, const array<uint16_t> &tri_list, const int tri0, array<int> &connected_tri, array<int> const *ignored_tri = nullptr);
    bool FindConnectedTriangles(const ivec3 &search_idx, const array<uint16_t> &tri_list, const int tri0, array<int> &connected_tri, array<int> const *ignored_tri = nullptr);
    void RegisterVertex(int vert_id, vec3 vert_coord);
    void RemoveVertex(int vert_id);
    bool GetMasterList(array<int> &ret_master_list) { ret_master_list = master_list; return ret_master_list.Count() > 0; }
    void Clear() { vertex_list.Empty(); }
private:
    //<VertexId, VertexLocation, VertexMasterId>
    array<int, vec3, int>   vertex_list;
    //List of the master_ vertices
    array<int>              master_list;
};

} /* namespace lol */

