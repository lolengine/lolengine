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

//
// The VertexBufferData class
// --------------------------
//

class VertexBufferData
{
    friend class VertexBuffer;
    friend class VertexDeclaration;

    size_t m_size;

#if defined USE_D3D9
    IDirect3DVertexBuffer9 *m_vbo;
#elif defined _XBOX
    D3DVertexBuffer *m_vbo;
#else
    GLuint m_vbo;
    uint8_t *m_memory;
#endif
};

//
// The VertexDeclaration class
// ---------------------------
//

VertexStreamBase const VertexStreamBase::Empty;

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

VertexDeclaration::~VertexDeclaration()
{
#if defined _XBOX || defined USE_D3D9
#   if defined USE_D3D9
    IDirect3DVertexDeclaration9 *vdecl = (IDirect3DVertexDeclaration9 *)m_data;
#   elif defined _XBOX
    D3DVertexDeclaration *vdecl = (D3DVertexDeclaration *)m_data;
#   endif

    if (FAILED(vdecl->Release()))
        Abort();
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

    if (FAILED(g_d3ddevice->SetVertexDeclaration(vdecl)))
        Abort();
#else
    /* FIXME: Nothing to do? */
#endif
}

void VertexDeclaration::DrawElements(MeshPrimitive type, int skip, int count)
{
    if (count <= 0)
        return;

#if defined _XBOX || defined USE_D3D9
    g_d3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
    g_d3ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_d3ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    if (FAILED(g_d3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW)))
        Abort();
    switch (type)
    {
    case MeshPrimitive::Triangles:
        if (FAILED(g_d3ddevice->DrawPrimitive(D3DPT_TRIANGLELIST, skip, count)))
            Abort();
        break;
    case MeshPrimitive::Points:
        if (FAILED(g_d3ddevice->DrawPrimitive(D3DPT_POINTLIST, skip, count)))
            Abort();
        break;
    }
#else
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
#   if defined HAVE_GL_2X && !defined __APPLE__
    /* FIXME: this has nothing to do here! */
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.01f);
#   endif
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    switch (type)
    {
    case MeshPrimitive::Triangles:
        glDrawArrays(GL_TRIANGLES, skip * 3, count * 3);
        break;
    case MeshPrimitive::Points:
        glDrawArrays(GL_POINTS, skip, count);
        break;
    }
#   if defined HAVE_GL_2X && !defined __APPLE__
    /* FIXME: this has nothing to do here! */
    glDisable(GL_ALPHA_TEST);
#   endif
#endif
}

void VertexDeclaration::DrawIndexedElements(MeshPrimitive type, int vbase,
                                            int vskip, int vcount,
                                            int skip, int count)
{
    if (count <= 0)
        return;

#if defined _XBOX || defined USE_D3D9
    g_d3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
    g_d3ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_d3ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_d3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    switch (type)
    {
    case MeshPrimitive::Triangles:
        if (FAILED(g_d3ddevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, vbase, vskip, vcount, skip, count)))
            Abort();
        break;
    case MeshPrimitive::Points:
        if (FAILED(g_d3ddevice->DrawIndexedPrimitive(D3DPT_POINTLIST, vbase, vskip, vcount, skip, count)))
            Abort();
        break;
    }
#else
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
#   if defined HAVE_GL_2X && !defined __APPLE__
    /* FIXME: this has nothing to do here! */
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.01f);
#   endif
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    switch (type)
    {
    case MeshPrimitive::Triangles:
        /* FIXME: ignores most of the arguments! */
        glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_SHORT, 0);
        break;
    case MeshPrimitive::Points:
        /* FIXME: ignores most of the arguments! */
        glDrawElements(GL_POINTS, count, GL_UNSIGNED_SHORT, 0);
        break;
    }
#   if defined HAVE_GL_2X && !defined __APPLE__
    /* FIXME: this has nothing to do here! */
    glDisable(GL_ALPHA_TEST);
#   endif
#endif
}

void VertexDeclaration::Unbind()
{
#if defined _XBOX || defined USE_D3D9
    int stream = -1;
    for (int i = 0; i < m_count; i++)
        if (m_streams[i].index != stream)
        {
            stream = m_streams[i].index;
            if (FAILED(g_d3ddevice->SetStreamSource(stream, 0, 0, 0)))
                Abort();
        }
    if (FAILED(g_d3ddevice->SetVertexDeclaration(NULL)))
        Abort();
#elif !defined __CELLOS_LV2__
    /* FIXME: we need to unbind what we bound */
    //glDisableVertexAttribArray(m_attrib);
    /* FIXME: temporary kludge */
    for (int i = 0; i < 12; i++)
        glDisableVertexAttribArray(i);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* FIXME: only useful for VAOs */
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Or: */
    //glDisableVertexAttribArray(m_attrib);
#else
    /* Or even: */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
#endif
}

void VertexDeclaration::SetStream(VertexBuffer *vb, ShaderAttrib attr1,
                                                    ShaderAttrib attr2,
                                                    ShaderAttrib attr3,
                                                    ShaderAttrib attr4,
                                                    ShaderAttrib attr5,
                                                    ShaderAttrib attr6,
                                                    ShaderAttrib attr7,
                                                    ShaderAttrib attr8,
                                                    ShaderAttrib attr9,
                                                    ShaderAttrib attr10,
                                                    ShaderAttrib attr11,
                                                    ShaderAttrib attr12)
{
    if (!vb->m_data->m_size)
        return;

#if defined _XBOX || defined USE_D3D9
    /* Only the first item is required to know which stream this
     * is about; the rest of the information is stored in the
     * vertex declaration already. */
    uint32_t usage = (attr1.m_flags >> 16) & 0xffff;
    uint32_t index = attr1.m_flags & 0xffff;

    /* Find the stream number */
    int usage_index = 0, stream = -1;
    for (int i = 0; i < m_count; i++)
        if (m_streams[i].usage == usage)
            if (usage_index++ == index)
            {
                stream = m_streams[i].index;
                break;
            }

    /* Compute this stream's stride */
    int stride = 0;
    for (int i = 0; i < m_count; i++)
        if (stream == m_streams[i].index)
            stride += m_streams[i].size;

    /* Now we know the stream index and the element stride */
    /* FIXME: precompute most of the crap above! */
    if (stream >= 0)
    {
        if (FAILED(g_d3ddevice->SetStreamSource(stream, vb->m_data->m_vbo, 0, stride)))
            Abort();
    }
#else
    glBindBuffer(GL_ARRAY_BUFFER, vb->m_data->m_vbo);
    ShaderAttrib l[12] = { attr1, attr2, attr3, attr4, attr5, attr6,
                           attr7, attr8, attr9, attr10, attr11, attr12 };
    for (int n = 0; n < 12 && l[n].m_flags != (uint64_t)0 - 1; n++)
    {
        uint32_t reg = l[n].m_flags >> 32;
        uint32_t usage = (l[n].m_flags >> 16) & 0xffff;
        uint32_t index = l[n].m_flags & 0xffff;

#   if !defined __CELLOS_LV2__
        glEnableVertexAttribArray((GLint)reg);
#   else
        switch (usage)
        {
        case VertexUsage::Position:
            glEnableClientState(GL_VERTEX_ARRAY);
            break;
        case VertexUsage::TexCoord:
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            break;
        case VertexUsage::Normal:
            glEnableClientState(GL_NORMAL_ARRAY);
            break;
        case VertexUsage::Color:
            glEnableClientState(GL_COLOR_ARRAY);
            break;
        }
#   endif

        /* We need to parse the whole vertex declaration to retrieve
         * the information. It sucks. */

        int attr_index = 0, usage_index = 0;
        /* First, find the stream index */
        for (; attr_index < m_count; attr_index++)
            if (m_streams[attr_index].usage == usage)
                if (usage_index++ == index)
                    break;

        /* Now compute the stride and offset up to this stream index */
        int stride = 0, offset = 0;
        for (int i = 0; i < m_count; i++)
            if (m_streams[i].index == m_streams[attr_index].index)
            {
                stride += m_streams[i].size;
                if (i < attr_index)
                    offset += m_streams[i].size;
            }

        /* Finally, we need to retrieve the type of the data */
#   if !defined GL_DOUBLE
#       define GL_DOUBLE 0
#   endif
        static struct { GLint size; GLenum type; } const tlut[] =
        {
            { 0, 0 },
            { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, /* half */
            { 1, GL_FLOAT }, { 2, GL_FLOAT }, { 3, GL_FLOAT },
                { 4, GL_FLOAT }, /* float */
            { 1, GL_DOUBLE }, { 2, GL_DOUBLE }, { 3, GL_DOUBLE },
                { 4, GL_DOUBLE }, /* double */
            { 1, GL_BYTE }, { 2, GL_BYTE }, { 3, GL_BYTE },
                { 4, GL_BYTE }, /* int8_t */
            { 1, GL_UNSIGNED_BYTE }, { 2, GL_UNSIGNED_BYTE },
                { 3, GL_UNSIGNED_BYTE }, { 4, GL_UNSIGNED_BYTE }, /* uint8_t */
            { 1, GL_SHORT }, { 2, GL_SHORT }, { 3, GL_SHORT },
                { 4, GL_SHORT }, /* int16_t */
            { 1, GL_UNSIGNED_SHORT }, { 2, GL_UNSIGNED_SHORT }, { 3,
                GL_UNSIGNED_SHORT }, { 4, GL_UNSIGNED_SHORT }, /* uint16_t */
            { 1, GL_INT }, { 2, GL_INT }, { 3, GL_INT },
                { 4, GL_INT }, /* int32_t */
            { 1, GL_UNSIGNED_INT }, { 2, GL_UNSIGNED_INT },
                { 3, GL_UNSIGNED_INT }, { 4, GL_UNSIGNED_INT }, /* uint32_t */
        };

        int type_index = m_streams[attr_index].stream_type;
        if (type_index < 0 || type_index >= sizeof(tlut) / sizeof(*tlut))
            type_index = 0;


#   if !defined __CELLOS_LV2__
        if (tlut[type_index].type == GL_FLOAT
             || tlut[type_index].type == GL_DOUBLE
             || tlut[type_index].type == GL_BYTE
             || tlut[type_index].type == GL_UNSIGNED_BYTE)
        {
            /* Normalize unsigned bytes by default, because it's usually
             * some color information. */
            GLboolean normalize = (tlut[type_index].type == GL_UNSIGNED_BYTE)
                               || (tlut[type_index].type == GL_BYTE);
            glVertexAttribPointer((GLint)reg, tlut[type_index].size,
                                  tlut[type_index].type, normalize,
                                  stride, (GLvoid const *)(uintptr_t)offset);
        }
#       if defined GL_VERSION_3_0
        else
        {
            glVertexAttribIPointer((GLint)reg, tlut[type_index].size,
                                   tlut[type_index].type,
                                   stride, (GLvoid const *)(uintptr_t)offset);
        }
#       endif
#   else
        switch (usage)
        {
        case VertexUsage::Position:
            glVertexPointer(tlut[type_index].size, tlut[type_index].type,
                            stride, (GLvoid const *)(uintptr_t)offset);
            break;
        case VertexUsage::Normal:
            glNormalPointer(tlut[type_index].type,
                            stride, (GLvoid const *)(uintptr_t)offset);
            break;
        case VertexUsage::Color:
            glColorPointer(tlut[type_index].size, tlut[type_index].type,
                           stride, (GLvoid const *)(uintptr_t)offset);
            break;
        }
#   endif
    }
#endif
}

void VertexDeclaration::Initialize()
{
#if defined _XBOX || defined USE_D3D9
    static D3DVERTEXELEMENT9 const end_element[] = { D3DDECL_END() };
    static D3DDECLTYPE const X = D3DDECLTYPE_UNUSED;
    static D3DDECLTYPE const tlut[] =
    {
        D3DDECLTYPE_UNUSED,
        X, D3DDECLTYPE_FLOAT16_2, X, D3DDECLTYPE_FLOAT16_4, /* half */
        D3DDECLTYPE_FLOAT1, D3DDECLTYPE_FLOAT2, D3DDECLTYPE_FLOAT3,
            D3DDECLTYPE_FLOAT4, /* float */
        X, X, X, X, /* double */
        X, X, X, X, /* int8_t */
        X, X, X, D3DDECLTYPE_UBYTE4N, /* uint8_t */
        X, D3DDECLTYPE_SHORT2N, X, D3DDECLTYPE_SHORT4N, /* int16_t */
        X, D3DDECLTYPE_USHORT2N, X, D3DDECLTYPE_USHORT4N, /* uint16_t */
        X, X, X, X, /* int32_t */
        X, X, X, X, /* uint32_t */
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
#if defined _XBOX
        D3DDECLUSAGE_TEXCOORD, /* FIXME: nonexistent */
#else
        D3DDECLUSAGE_POSITIONT,
#endif
        D3DDECLUSAGE_COLOR,
        D3DDECLUSAGE_FOG,
        D3DDECLUSAGE_DEPTH,
        D3DDECLUSAGE_SAMPLE,
    };

    D3DVERTEXELEMENT9 elements[12 + 1];
    for (int n = 0; n < m_count; n++)
    {
        elements[n].Stream = m_streams[n].index;
        elements[n].Offset = 0;
        for (int i = 0; i < n; i++)
            if (m_streams[i].index == m_streams[n].index)
                elements[n].Offset += m_streams[i].size;

        if (m_streams[n].stream_type >= 0
             && m_streams[n].stream_type < sizeof(tlut) / sizeof(*tlut))
            elements[n].Type = tlut[m_streams[n].stream_type];
        else
            elements[n].Type = D3DDECLTYPE_UNUSED;

        elements[n].Method = D3DDECLMETHOD_DEFAULT;

        if (m_streams[n].usage >= 0
             && m_streams[n].usage < sizeof(ulut) / sizeof(*ulut))
            elements[n].Usage = ulut[m_streams[n].usage];
        else
            elements[n].Usage = D3DDECLUSAGE_POSITION;

        elements[n].UsageIndex = 0;
        for (int i = 0; i < n; i++)
            if (elements[i].Stream == elements[n].Stream
                 && elements[i].Usage == elements[n].Usage)
                elements[n].UsageIndex++;
    }
    elements[m_count] = end_element[0];

#   if defined USE_D3D9
    IDirect3DVertexDeclaration9 *vdecl;
#   elif defined _XBOX
    D3DVertexDeclaration *vdecl;
#   endif

    if (FAILED(g_d3ddevice->CreateVertexDeclaration(elements, &vdecl)))
        Abort();

    m_data = vdecl;
#else

#endif
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

//
// The VertexBuffer class
// ----------------------
//

VertexBuffer::VertexBuffer(size_t size)
  : m_data(new VertexBufferData)
{
    m_data->m_size = size;
    if (!size)
        return;
#if defined USE_D3D9 || defined _XBOX
    if (FAILED(g_d3ddevice->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, NULL,
                                               D3DPOOL_MANAGED, &m_data->m_vbo, NULL)))
        Abort();
#else
    glGenBuffers(1, &m_data->m_vbo);
    m_data->m_memory = new uint8_t[size];
#endif
}

VertexBuffer::~VertexBuffer()
{
    if (m_data->m_size)
    {
#if defined USE_D3D9 || defined _XBOX
        if (FAILED(m_data->m_vbo->Release()))
            Abort();
#else
        glDeleteBuffers(1, &m_data->m_vbo);
        delete[] m_data->m_memory;
#endif
    }
    delete m_data;
}

void *VertexBuffer::Lock(size_t offset, size_t size)
{
    if (!m_data->m_size)
        return NULL;
#if defined USE_D3D9 || defined _XBOX
    void *ret;
    if (FAILED(m_data->m_vbo->Lock(offset, size, (void **)&ret, 0)))
        Abort();
    return ret;
#else
    return m_data->m_memory + offset;
#endif
}

void VertexBuffer::Unlock()
{
    if (!m_data->m_size)
        return;
#if defined USE_D3D9 || defined _XBOX
    if (FAILED(m_data->m_vbo->Unlock()))
        Abort();
#else
    glBindBuffer(GL_ARRAY_BUFFER, m_data->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_data->m_size, m_data->m_memory,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

} /* namespace lol */

