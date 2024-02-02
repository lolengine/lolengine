//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The IndexBuffer class
// ---------------------
//

#include <cstring>

namespace lol
{

class IndexBuffer
{
    friend class Mesh;

public:
    IndexBuffer(size_t size);
    ~IndexBuffer();

    size_t size();

    void set_data(void const *data, size_t size)
    {
        auto ptr = lock(0, size);
        ::memcpy(ptr, data, size);
        unlock();
    }

    void *lock(size_t offset, size_t size);
    void unlock();

    void Bind();
    void Unbind();

protected:
    uint16_t *GetData();

private:
    class IndexBufferData *m_data;
};

} /* namespace lol */

