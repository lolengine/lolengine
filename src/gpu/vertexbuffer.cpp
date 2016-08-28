//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "lolgl.h"

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

    GLuint m_vbo;
    uint8_t *m_memory;
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
  : m_count(0)
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
}

VertexDeclaration::~VertexDeclaration()
{
}

void VertexDeclaration::Bind()
{
    /* FIXME: Nothing to do? */
}

void VertexDeclaration::DrawElements(MeshPrimitive type, int skip, int count)
{
    if (count <= 0)
        return;

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
}

void VertexDeclaration::DrawIndexedElements(MeshPrimitive type, int vbase,
                                            int vskip, int vcount,
                                            int skip, int count)
{
    if (count <= 0)
        return;

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
}

void VertexDeclaration::Unbind()
{
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
    ShaderAttrib attribs[12] = { attr1, attr2, attr3, attr4, attr5, attr6,
                           attr7, attr8, attr9, attr10, attr11, attr12 };

    SetStream(vb, attribs);
}

void VertexDeclaration::SetStream(VertexBuffer *vb, ShaderAttrib attribs[])
{
    if (!vb->m_data->m_size)
        return;

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
#if !defined GL_DOUBLE
#   define GL_DOUBLE 0
#endif
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
#if defined USE_GLEW && !defined __APPLE__
                 /* If this is not available, don't use it */
                 || !glVertexAttribIPointer
#endif
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
#if defined GL_VERSION_3_0
            else
            {
                glVertexAttribIPointer((GLint)reg, tlut[type_index].size,
                                       tlut[type_index].type,
                                       stride, (GLvoid const *)(uintptr_t)offset);
            }
#endif
        }
    }
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

    glGenBuffers(1, &m_data->m_vbo);
    m_data->m_memory = new uint8_t[size];
}

VertexBuffer::~VertexBuffer()
{
    if (m_data->m_size)
    {
        glDeleteBuffers(1, &m_data->m_vbo);
        delete[] m_data->m_memory;
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

    /* FIXME: is there a way to use "size"? */
    UNUSED(size);
    return m_data->m_memory + offset;
}

void VertexBuffer::Unlock()
{
    if (!m_data->m_size)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, m_data->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_data->m_size, m_data->m_memory,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} /* namespace lol */

