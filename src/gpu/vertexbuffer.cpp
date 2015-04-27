//
//  Lol Engine
//
//  Copyright: © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "lolgl.h"

#if defined _WIN32 && defined USE_D3D9
#   define FAR
#   define NEAR
#   include <d3d9.h>
#elif defined _XBOX
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
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
    IDirect3DDevice9 *m_dev;
    IDirect3DVertexBuffer9 *m_vbo;
#elif defined _XBOX
    D3DDevice *m_dev;
    D3DVertexBuffer *m_vbo;
#else
    GLuint m_vbo;
    uint8_t *m_memory;
#endif
};

//
// The VertexDeclarationData class
// -------------------------------
//

class VertexDeclarationData
{
    friend class VertexBuffer;
    friend class VertexDeclaration;

#if defined USE_D3D9
    IDirect3DDevice9 *m_dev;
    IDirect3DVertexDeclaration9 *m_vdecl;
#elif defined _XBOX
    D3DDevice *m_dev;
    D3DVertexDeclaration *m_vdecl;
#else
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
                                     VertexStreamBase const &s12)
  : m_count(0),
    m_data(new VertexDeclarationData())
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
    if (FAILED(m_data->m_vdecl->Release()))
        Abort();
#else

#endif

    delete m_data;
}

void VertexDeclaration::Bind()
{
#if defined _XBOX || defined USE_D3D9
    if (FAILED(m_data->m_dev->SetVertexDeclaration(m_data->m_vdecl)))
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
    switch (type.ToScalar())
    {
    case MeshPrimitive::Triangles:
        if (FAILED(m_data->m_dev->DrawPrimitive(D3DPT_TRIANGLELIST,
                                                skip, count)))
            Abort();
        break;
    case MeshPrimitive::TriangleStrips:
        if (FAILED(m_data->m_dev->DrawPrimitive(D3DPT_TRIANGLESTRIP,
                                                skip, count)))
            Abort();
        break;
    case MeshPrimitive::TriangleFans:
        if (FAILED(m_data->m_dev->DrawPrimitive(D3DPT_TRIANGLEFAN,
                                                skip, count)))
            Abort();
        break;
    case MeshPrimitive::Points:
        if (FAILED(m_data->m_dev->DrawPrimitive(D3DPT_POINTLIST,
                                                skip, count)))
            Abort();
        break;
    case MeshPrimitive::Lines:
        if (FAILED(m_data->m_dev->DrawPrimitive(D3DPT_LINELIST,
                                                skip, count)))
            Abort();
        break;
    }
#else
    /* FIXME: this has nothing to do here! */
    switch (type.ToScalar())
    {
    case MeshPrimitive::Triangles:
        glDrawArrays(GL_TRIANGLES, skip, count);
        break;
    case MeshPrimitive::TriangleStrips:
        glDrawArrays(GL_TRIANGLE_STRIP, skip, count);
        break;
    case MeshPrimitive::TriangleFans:
        glDrawArrays(GL_TRIANGLE_FAN, skip, count);
        break;
    case MeshPrimitive::Points:
        glDrawArrays(GL_POINTS, skip, count);
        break;
    case MeshPrimitive::Lines:
        glDrawArrays(GL_LINES, skip, count);
        break;
    }
#endif
}

void VertexDeclaration::DrawIndexedElements(MeshPrimitive type, int vbase,
                                            int vskip, int vcount,
                                            int skip, int count)
{
    if (count <= 0)
        return;

#if defined _XBOX || defined USE_D3D9
    switch (type.ToScalar())
    {
    case MeshPrimitive::Triangles:
        count = count / 3;
        if (FAILED(m_data->m_dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
                                           vbase, vskip, vcount, skip, count)))
            Abort();
        break;
    case MeshPrimitive::TriangleStrips:
        count = count - 2;
        if (FAILED(m_data->m_dev->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
                                           vbase, vskip, vcount, skip, count)))
            Abort();
        break;
    case MeshPrimitive::TriangleFans:
        count = count - 2;
        if (FAILED(m_data->m_dev->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN,
                                           vbase, vskip, vcount, skip, count)))
            Abort();
        break;
    case MeshPrimitive::Points:
        if (FAILED(m_data->m_dev->DrawIndexedPrimitive(D3DPT_POINTLIST,
                                           vbase, vskip, vcount, skip, count)))
            Abort();
        break;
    case MeshPrimitive::Lines:
        if (FAILED(m_data->m_dev->DrawIndexedPrimitive(D3DPT_LINELIST,
                                           vbase, vskip, vcount, skip, count)))
            Abort();
        break;
    }
#else
    /* FIXME: this has nothing to do here! */
    switch (type.ToScalar())
    {
    case MeshPrimitive::Triangles:
        /* FIXME: ignores most of the arguments! */
        UNUSED(vbase, vskip, vcount, skip);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
        break;
    case MeshPrimitive::TriangleStrips:
        /* FIXME: ignores most of the arguments! */
        UNUSED(vbase, vskip, vcount, skip);
        glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_SHORT, 0);
        break;
    case MeshPrimitive::TriangleFans:
        /* FIXME: ignores most of the arguments! */
        UNUSED(vbase, vskip, vcount, skip);
        glDrawElements(GL_TRIANGLE_FAN, count, GL_UNSIGNED_SHORT, 0);
        break;
    case MeshPrimitive::Points:
        /* FIXME: ignores most of the arguments! */
        UNUSED(vbase, vskip, vcount, skip);
        glDrawElements(GL_POINTS, count, GL_UNSIGNED_SHORT, 0);
        break;
    case MeshPrimitive::Lines:
        /* FIXME: ignores most of the arguments! */
        UNUSED(vbase, vskip, vcount, skip);
        glDrawElements(GL_LINES, count, GL_UNSIGNED_SHORT, 0);
        break;
    }
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
            if (FAILED(m_data->m_dev->SetStreamSource(stream, 0, 0, 0)))
                Abort();
        }
    /* "NULL is an invalid input to SetVertexDeclaration" (DX9 guide), so
     * we just don't touch the current vertex declaration. */
#else
    for (int i = 0; i < m_count; i++)
    {
        if (m_streams[i].reg >= 0)
        {
            for (int j = i + 1; j < m_count; j++)
                if (m_streams[j].reg == m_streams[i].reg)
                    m_streams[j].reg = -1;

            glDisableVertexAttribArray(m_streams[i].reg);
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
#if defined _XBOX || defined USE_D3D9
    // Don't bother in DirectX world, shader attributes are not used
    SetStream(vb, nullptr);
#else
    ShaderAttrib attribs[12] = { attr1, attr2, attr3, attr4, attr5, attr6,
                           attr7, attr8, attr9, attr10, attr11, attr12 };

    SetStream(vb, attribs);
#endif
}

void VertexDeclaration::SetStream(VertexBuffer *vb, ShaderAttrib attribs[])
{
    if (!vb->m_data->m_size)
        return;

#if defined _XBOX || defined USE_D3D9
    /* Only the first item is required to know which stream this
     * is about; the rest of the information is stored in the
     * vertex declaration already. */
    VertexUsage usage = VertexUsage((attr1.m_flags >> 16) & 0xffff);
    uint32_t index = attr1.m_flags & 0xffff;

    /* Find the stream number */
    uint32_t usage_index = 0;
    int stream = -1;
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
        if (FAILED(m_data->m_dev->SetStreamSource(stream, vb->m_data->m_vbo,
                                                  0, stride)))
            Abort();
    }
#else
    glBindBuffer(GL_ARRAY_BUFFER, vb->m_data->m_vbo);
    for (int n = 0; n < 12 && attribs[n].m_flags != (uint64_t)0 - 1; n++)
    {
        VertexUsage usage = VertexUsage((attribs[n].m_flags >> 16) & 0xffff);
        uint32_t index = attribs[n].m_flags & 0xffff;
        uint32_t reg = attribs[n].m_flags >> 32;

        if (reg != 0xffffffffu)
            glEnableVertexAttribArray((GLint)reg);

        /* We need to parse the whole vertex declaration to retrieve
         * the information. It sucks. */

        int attr_index = 0;
        /* First, find the stream index */
        for (uint32_t usage_index = 0; attr_index < m_count; attr_index++)
            if (m_streams[attr_index].usage == usage)
                if (usage_index++ == index)
                    break;

        if (attr_index == m_count)
        {
            msg::error("stream #%d with usage %x not found in declaration\n",
                       index, usage.ToScalar());
            attr_index = 0;
        }

        /* Now compute the stride and offset up to this stream index */
        int stride = 0, offset = 0;
        for (int i = 0; i < m_count; i++)
            if (m_streams[i].index == m_streams[attr_index].index)
            {
                /* Remember the register used for this stream */
                m_streams[i].reg = reg;

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
#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
            { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, /* half */
#endif
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
        if (type_index < 0 || type_index >= (int)(sizeof(tlut) / sizeof(*tlut)))
            type_index = 0;


        if (reg != 0xffffffff)
        {
            if (tlut[type_index].type == GL_FLOAT
                 || tlut[type_index].type == GL_DOUBLE
                 || tlut[type_index].type == GL_BYTE
                 || tlut[type_index].type == GL_UNSIGNED_BYTE
#       if defined USE_GLEW && !defined __APPLE__
                 /* If this is not available, don't use it */
                 || !glVertexAttribIPointer
#       endif
                 || false)
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
        }
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
#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
        X, D3DDECLTYPE_FLOAT16_2, X, D3DDECLTYPE_FLOAT16_4, /* half */
#endif
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
    m_data->m_dev = (IDirect3DDevice9 *)Renderer::Get()->GetDevice();
#   elif defined _XBOX
    m_data->m_dev = (D3DDevice *)Renderer::Get()->GetDevice();
#   endif

    if (FAILED(m_data->m_dev->CreateVertexDeclaration(elements,
                                                      &m_data->m_vdecl)))
        Abort();
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
        m_streams[m_count].reg = -1;
        m_count++;
    }
}

int VertexDeclaration::GetStreamCount() const
{
    return m_count ? m_streams[m_count - 1].index + 1 : 0;
}

VertexStreamBase VertexDeclaration::GetStream(int index) const
{
    VertexStreamBase stream;
    int n = 0;
    int count = 0;

    for (int i = 0; i < m_count; ++i)
    {
        if (m_streams[i].index != index)
            continue;

        switch (m_streams[i].stream_type)
        {
#define __T(T) \
            case VertexStreamBase::Type##T: stream.AddStream<T>(n++, m_streams[i].usage); break;

            __T(void)
#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
            __T(half)     __T(f16vec2) __T(f16vec3) __T(f16vec4)
#endif
            __T(float)    __T(vec2)    __T(vec3)    __T(vec4)
            __T(double)   __T(dvec2)   __T(dvec3)   __T(dvec4)
            __T(int8_t)   __T(i8vec2)  __T(i8vec3)  __T(i8vec4)
            __T(uint8_t)  __T(u8vec2)  __T(u8vec3)  __T(u8vec4)
            __T(int16_t)  __T(i16vec2) __T(i16vec3) __T(i16vec4)
            __T(uint16_t) __T(u16vec2) __T(u16vec3) __T(u16vec4)
            __T(int32_t)  __T(ivec2)   __T(ivec3)   __T(ivec4)
            __T(uint32_t) __T(uvec2)   __T(uvec3)   __T(uvec4)
#undef __T
        }
        ++count;
    }

    while (count < 12)
        stream.AddStream<void>(count++, VertexUsage::Position);

    return stream;
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
#   if defined USE_D3D9
    m_data->m_dev = (IDirect3DDevice9 *)Renderer::Get()->GetDevice();
#   elif defined _XBOX
    m_data->m_dev = (D3DDevice *)Renderer::Get()->GetDevice();
#   endif

    if (FAILED(m_data->m_dev->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, nullptr,
                                               D3DPOOL_MANAGED, &m_data->m_vbo, nullptr)))
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

size_t VertexBuffer::GetSize()
{
    return m_data->m_size;
}

void *VertexBuffer::Lock(size_t offset, size_t size)
{
    if (!m_data->m_size)
        return nullptr;
#if defined USE_D3D9 || defined _XBOX
    void *ret;
    if (FAILED(m_data->m_vbo->Lock(offset, size, (void **)&ret, 0)))
        Abort();
    return ret;
#else
    /* FIXME: is there a way to use "size"? */
    UNUSED(size);
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

