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
    DEF_VALUE
        //When this flag is up, negative scaling will not invert faces.
        ADD_VALUE_SET(ScaleWinding                , (1 << 0))
        ADD_VALUE_SET(CommandRecording            , (1 << 1))
        ADD_VALUE_SET(CommandExecution            , (1 << 2))
        ADD_VALUE_SET(QuadWeighting               , (1 << 3))
        ADD_VALUE_SET(IgnoreQuadWeighting         , (1 << 4))
        ADD_VALUE_SET(PostBuildComputeNormals     , (1 << 5))
        ADD_VALUE_SET(PreventVertCleanup          , (1 << 6))

        ADD_VALUE_SET(All                         , 0xffffffff)
    END_E_VALUE

    LOL_DECLARE_ENUM_METHODS(MeshBuildOperation)
};

struct EasyMeshCmdType
{
    DEF_VALUE
        ADD_VALUE(MeshCsg)

        ADD_VALUE(LoopStart)
        ADD_VALUE(LoopEnd)
        ADD_VALUE(OpenBrace)
        ADD_VALUE(CloseBrace)

        ADD_VALUE(ScaleWinding)
        ADD_VALUE(QuadWeighting)
        ADD_VALUE(PostBuildNormal)
        ADD_VALUE(PreventVertCleanup)
        ADD_VALUE(SetColorA)
        ADD_VALUE(SetColorB)
        ADD_VALUE(SetVertColor)

        ADD_VALUE(VerticesMerge)
        ADD_VALUE(VerticesSeparate)

        ADD_VALUE(Translate)
        ADD_VALUE(Rotate)
        ADD_VALUE(RadialJitter)
        ADD_VALUE(MeshTranform)
        ADD_VALUE(Scale)
        ADD_VALUE(DupAndScale)
        ADD_VALUE(Chamfer)

        ADD_VALUE(SplitTriangles)
        ADD_VALUE(SmoothMesh)

        ADD_VALUE(AppendCylinder)
        ADD_VALUE(AppendCapsule)
        ADD_VALUE(AppendTorus)
        ADD_VALUE(AppendBox)
        ADD_VALUE(AppendStar)
        ADD_VALUE(AppendExpandedStar)
        ADD_VALUE(AppendDisc)
        ADD_VALUE(AppendSimpleTriangle)
        ADD_VALUE(AppendSimpleQuad)
        ADD_VALUE(AppendCog)
    END_E_VALUE

    LOL_DECLARE_ENUM_METHODS(EasyMeshCmdType)
};

LOL_SAFE_ENUM(MeshType,
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
);

struct TexCoordBuildType
{
    DEF_VALUE
        ADD_VALUE_SET(TriangleDefault     , 0)
        ADD_VALUE_SET(QuadDefault         , 0)
        ADD_VALUE_SET(BoxDefault          , 0)
        ADD_VALUE_SET(SphereDefault       , 0)
        ADD_VALUE_SET(CapsuleDefault      , 0)
        ADD_VALUE_SET(TorusDefault        , 0)
        ADD_VALUE_SET(CylinderDefault     , 0)
        ADD_VALUE_SET(DiscDefault         , 0)
        ADD_VALUE_SET(StarDefault         , 0)
        ADD_VALUE_SET(ExpandedStarDefault , 0)
        ADD_VALUE_SET(CogDefault          , 0)

        //NEVER FORGET TO INCREMENT THIS WHEN ADDING A VALUE
        ADD_VALUE_SET(Max                 , 1)
    END_E_VALUE

    LOL_DECLARE_ENUM_METHODS(TexCoordBuildType)
};

LOL_SAFE_ENUM(MeshFaceType,
    BoxFront    = 0,
    QuadDefault = 0,
    BoxLeft     = 1,
    BoxBack     = 2,
    BoxRight    = 3,
    BoxTop      = 4,
    BoxBottom   = 5,

    Max
);

LOL_SAFE_ENUM(TexCoordPos,
    BL, // Bottom Left
    BR, // Bottom Right
    TL, // Top Left
    TR  // Top Right
);

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
    inline Array<int, int> &LoopStack(){ return m_loop_stack; }
    inline vec4 &ColorA()           { return m_color_a; }
    inline vec4 &ColorB()           { return m_color_b; }
    inline vec2 &TexCoordOffset()   { return m_texcoord_offset; }
    inline vec2 &TexCoordScale()    { return m_texcoord_scale; }
    inline vec2 &TexCoordOffset2()  { return m_texcoord_offset2; }
    inline vec2 &TexCoordScale2()   { return m_texcoord_scale2; }

    //UV1
    void SetTexCoordBuildType(MeshType mt, TexCoordBuildType tcbt) { m_texcoord_build_type[mt.ToScalar()] = (1 << (tcbt + 1)) | (m_texcoord_build_type[mt.ToScalar()] & 1); }
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
    void SetTexCoordBuildType2(MeshType mt, TexCoordBuildType tcbt) { m_texcoord_build_type2[mt.ToScalar()] = (1 << (tcbt + 1)) | (m_texcoord_build_type2[mt.ToScalar()] & 1); }
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
    Array<vec2, vec2>   m_texcoord_custom_build[MeshType::MAX];
    Array<vec2, vec2>   m_texcoord_custom_build2[MeshType::MAX];
    uint32_t            m_texcoord_build_type[MeshType::MAX];
    uint32_t            m_texcoord_build_type2[MeshType::MAX];
    uint32_t            m_build_flags;
};

/* A safe enum for VertexDictionnary operations. */
struct VDictType
{
    DEF_VALUE
        ADD_VALUE_SET(DoesNotExist  , -3)
        ADD_VALUE_SET(Alone         , -2)
        ADD_VALUE_SET(Master        , -1)
    END_E_VALUE

    LOL_DECLARE_ENUM_METHODS(VDictType)
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

