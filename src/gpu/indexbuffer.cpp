//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "lolgl.h"

namespace lol
{

//
// The IndexBufferData class
// -------------------------
//

class IndexBufferData
{
    friend class IndexBuffer;

    size_t m_size;
    GLuint m_ibo;
    uint8_t *m_memory;
};

//
// The IndexBuffer class
// ----------------------
//

IndexBuffer::IndexBuffer(size_t size)
  : m_data(new IndexBufferData)
{
    m_data->m_size = size;
    if (!size)
        return;
    glGenBuffers(1, &m_data->m_ibo);
    m_data->m_memory = new uint8_t[size];
}

IndexBuffer::~IndexBuffer()
{
    if (m_data->m_size)
    {
        glDeleteBuffers(1, &m_data->m_ibo);
        delete[] m_data->m_memory;
    }
    delete m_data;
}

size_t IndexBuffer::GetSize()
{
    return m_data->m_size;
}

void *IndexBuffer::Lock(size_t offset, size_t size)
{
    if (!m_data->m_size)
        return nullptr;

    UNUSED(size);
    return m_data->m_memory + offset;
}

void IndexBuffer::Unlock()
{
    if (!m_data->m_size)
        return;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data->m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data->m_size, m_data->m_memory,
                 GL_STATIC_DRAW);
}

void IndexBuffer::Bind()
{
    if (!m_data->m_size)
        return;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data->m_ibo);
    /* XXX: not necessary because we kept track of the size */
    //int size;
    //glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
}

void IndexBuffer::Unbind()
{
    if (!m_data->m_size)
        return;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} /* namespace lol */

