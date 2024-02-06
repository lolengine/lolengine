//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

// FIXME: fine-tune this define
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
#include "lolgl.h"
#endif

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
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    GLuint m_ibo;
#endif
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
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glGenBuffers(1, &m_data->m_ibo);
#endif
    m_data->m_memory = new uint8_t[size];
}

IndexBuffer::~IndexBuffer()
{
    if (m_data->m_size)
    {
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
        glDeleteBuffers(1, &m_data->m_ibo);
#endif
        delete[] m_data->m_memory;
    }
    delete m_data;
}

size_t IndexBuffer::size()
{
    return m_data->m_size;
}

void *IndexBuffer::lock(size_t offset, size_t size)
{
    if (!m_data->m_size)
        return nullptr;

    (void)size;
    return m_data->m_memory + offset;
}

void IndexBuffer::unlock()
{
    if (!m_data->m_size)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data->m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data->m_size, m_data->m_memory,
                 GL_STATIC_DRAW);
#endif
}

void IndexBuffer::Bind()
{
    if (!m_data->m_size)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data->m_ibo);
    /* XXX: not necessary because we kept track of the size */
    //int size;
    //glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
#endif
}

void IndexBuffer::Unbind()
{
    if (!m_data->m_size)
        return;

#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

} // namespace lol
