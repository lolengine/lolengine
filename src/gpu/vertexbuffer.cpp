//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "lolgl.h"

#if defined _WIN32 && defined USE_D3D9
#   define FAR
#   define NEAR
#   include <d3d9.h>
#endif

using namespace std;

#if defined USE_D3D9
extern IDirect3DDevice9 *g_d3ddevice;
#elif defined _XBOX
extern D3DDevice *g_d3ddevice;
#endif

namespace lol
{

VertexStreamBase const VertexStreamBase::Empty;

void VertexDeclaration::Initialize()
{
#if defined _XBOX || defined USE_D3D9
    static D3DVERTEXELEMENT9 const end_element[] = { D3DDECL_END() };
    static D3DECLTYPE const X = D3DDECLTYPE_UNUSED;
    static D3DECLTYPE const tlut[] =
    {
        D3DDECLTYPE_UNUSED,
        X, D3DDECLTYPE_FLOAT16_2, X, D3DDECLTYPE_FLOAT16_4, /* half */
        D3DDECLTYPE_FLOAT1, D3DDECLTYPE_FLOAT2, D3DDECLTYPE_FLOAT3,
            D3DDECLTYPE_FLOAT4, /* float */
        X, X, X, X, /* double */
        X, X, X, X, /* int8_t */
        X, X, X, D3DDECLTYPE_UBYTE4, /* uint8_t */
        X, D3DDECLTYPE_SHORT2N, X, D3DDECLTYPE_SHORT4N, /* int16_t */
        X, D3DDECLTYPE_USHORT2N, X, D3DDECLTYPE_USHORT4N, /* uint16_t */
        X, X, X, X, /* int32_t */
        X, X, X, X, /* int64_t */
    };
    static D3DDECLUSAGE const ulut[] =
    {
        D3DDECLUSAGE_POSITION,
        D3DDECLUSAGE_BLENDWEIGHT,
        D3DDECLUSAGE_BLENDINDICES,
        D3DDECLUSAGE_NORMAL,
        D3DDECLUSAGE_PSIZE,
        D3DDECLUSAGE_TEXCOORD,
        D3DDECLUSAGE_TANGENT,
        D3DDECLUSAGE_BINORMAL,
        D3DDECLUSAGE_TESSFACTOR,
        D3DDECLUSAGE_POSITIONT,
        D3DDECLUSAGE_COLOR,
        D3DDECLUSAGE_FOG,
        D3DDECLUSAGE_DEPTH,
        D3DDECLUSAGE_SAMPLE,
    };

    D3DVERTEXELEMENT9 elements[12 + 1];
    int nstreams = 0;
    while (m_streams[nstreams].size)
    {
        elements[nstreams].Stream = m_streams[nstreams].index;
        elements[nstreams].Offset = 0;
        for (int i = 0; i < nstreams; i++)
            elements[nstreams].Offset += m_streams[nstreams].size;

        if (m_streams[nstreams].type >= 0
             && m_streams[nstreams].type < sizeof(tlut) / sizeof(*tlut))
            elements[nstreams].Type = tlut[m_streams[nstreams].type];
        else
            elements[nstreams].Type = D3DDECLTYPE_UNUSED;

        elements[nstreams].Method = D3DDECLMETHOD_DEFAULT;

        if (m_streams[nstreams].usage >= 0
             && m_streams[nstreams].usage < sizeof(ulut) / sizeof(*ulut))
            elements[nstreams].Type = ulut[m_streams[nstreams].usage];
        else
            elements[nstreams].Type = D3DDECLUSAGE_POSITION;

        elements[nstreams].UsageIndex = 0;
        for (int i = 0; i < nstreams; i++)
            if (elements[i].Stream == elements[nstreams].Stream
                 && elements[i].Usage == elements[nstreams].Usage)
                elements[nstreams].UsageIndex++;

        nstreams++;
    }
    elements[nstreams] = end_element[0];

#   if defined USE_D3D9
    IDirect3DVertexDeclaration9 *vdecl;
#   elif defined _XBOX
    D3DVertexDeclaration *vdecl;
#   endif

    g_d3ddevice->CreateVertexDeclaration(elements, &vdecl);

    m_data = vdecl;
#else

#endif
}

VertexDeclaration::~VertexDeclaration()
{
#if defined _XBOX || defined USE_D3D9
#   if defined USE_D3D9
    IDirect3DVertexDeclaration9 *vdecl = (IDirect3DVertexDeclaration9 *)m_data;
#   elif defined _XBOX
    D3DVertexDeclaration *vdecl = (D3DVertexDeclaration *)m_data;
#   endif

    vdecl->Release();
#else

#endif
}

void VertexDeclaration::Bind()
{
#if defined _XBOX || defined USE_D3D9
#   if defined USE_D3D9
    IDirect3DVertexDeclaration9 *vdecl = (IDirect3DVertexDeclaration9 *)m_data;
#   elif defined _XBOX
    D3DVertexDeclaration *vdecl = (D3DVertexDeclaration *)m_data;
#   endif

    g_d3ddevice->SetVertexDeclaration(vdecl);
#else

#endif
}

VertexDeclaration::VertexDeclaration(VertexStreamBase const &s1,
                                     VertexStreamBase const &s2,
                                     VertexStreamBase const &s3,
                                     VertexStreamBase const &s4,
                                     VertexStreamBase const &s5,
                                     VertexStreamBase const &s6,
                                     VertexStreamBase const &s7,
                                     VertexStreamBase const &s8,
                                     VertexStreamBase const &s9,
                                     VertexStreamBase const &s10,
                                     VertexStreamBase const &s11,
                                     VertexStreamBase const &s12) : m_count(0)
{
    if (&s1 != &VertexStreamBase::Empty) AddStream(s1);
    if (&s2 != &VertexStreamBase::Empty) AddStream(s2);
    if (&s3 != &VertexStreamBase::Empty) AddStream(s3);
    if (&s4 != &VertexStreamBase::Empty) AddStream(s4);
    if (&s5 != &VertexStreamBase::Empty) AddStream(s5);
    if (&s6 != &VertexStreamBase::Empty) AddStream(s6);
    if (&s7 != &VertexStreamBase::Empty) AddStream(s7);
    if (&s8 != &VertexStreamBase::Empty) AddStream(s8);
    if (&s9 != &VertexStreamBase::Empty) AddStream(s9);
    if (&s10 != &VertexStreamBase::Empty) AddStream(s10);
    if (&s11 != &VertexStreamBase::Empty) AddStream(s11);
    if (&s12 != &VertexStreamBase::Empty) AddStream(s12);
    Initialize();
}

void VertexDeclaration::AddStream(VertexStreamBase const &s)
{
    int index = m_count ? m_streams[m_count - 1].index + 1 : 0;

    for (int i = 0; s.m_streams[i].size; i++)
    {
        m_streams[m_count].stream_type = s.m_streams[i].stream_type;
        m_streams[m_count].usage = s.m_streams[i].usage;
        m_streams[m_count].size = s.m_streams[i].size;
        m_streams[m_count].index = index;
        m_count++;
    }
}

} /* namespace lol */

