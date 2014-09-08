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
public:
    IndexBuffer(size_t size);
    ~IndexBuffer();

    size_t GetSize();

    void *Lock(size_t offset, size_t size);
    void Unlock();

    void Bind();
    void Unbind();

private:
    class IndexBufferData *m_data;
};

} /* namespace lol */

