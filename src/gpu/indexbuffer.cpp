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

#if defined _WIN32 && defined USE_D3D9
#   define FAR
#   define NEAR
#   include <d3d9.h>
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

#if defined USE_D3D9
    IDirect3DDevice9 *m_dev;
    IDirect3DIndexBuffer9 *m_ibo;
#else
    GLuint m_ibo;
    uint8_t *m_memory;
#endif
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
#if defined USE_D3D9
    m_data->m_dev = (IDirect3DDevice9 *)Renderer::Get()->GetDevice();
    if (FAILED(m_data->m_dev->CreateIndexBuffer(size, D3DUSAGE_WRITEONLY,
                                              D3DFMT_INDEX16, D3DPOOL_MANAGED,
                                              &m_data->m_ibo, nullptr)))
        Abort();
#else
    glGenBuffers(1, &m_data->m_ibo);
    m_data->m_memory = new uint8_t[size];
#endif
}

IndexBuffer::~IndexBuffer()
{
    if (m_data->m_size)
    {
#if defined USE_D3D9
        if (FAILED(m_data->m_ibo->Release()))
            Abort();
#else
        glDeleteBuffers(1, &m_data->m_ibo);
        delete[] m_data->m_memory;
#endif
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

#if defined USE_D3D9
    void *ret;
    if (FAILED(m_data->m_ibo->Lock(offset, size, (void **)&ret, 0)))
        Abort();
    return ret;
#else
    UNUSED(size);
    return m_data->m_memory + offset;
#endif
}

void IndexBuffer::Unlock()
{
    if (!m_data->m_size)
        return;

#if defined USE_D3D9
    if (FAILED(m_data->m_ibo->Unlock()))
        Abort();
#else
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data->m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data->m_size, m_data->m_memory,
                 GL_STATIC_DRAW);
#endif
}

void IndexBuffer::Bind()
{
    if (!m_data->m_size)
        return;

#if defined USE_D3D9
    if (FAILED(m_data->m_dev->SetIndices(m_data->m_ibo)))
        Abort();
#else
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

#if defined USE_D3D9
    if (FAILED(m_data->m_dev->SetIndices(nullptr)))
        Abort();
#else
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

} /* namespace lol */

