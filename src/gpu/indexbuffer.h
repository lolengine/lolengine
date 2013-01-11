//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The IndexBuffer class
// ---------------------
//

#if !defined __LOL_INDEXBUFFER_H__
#define __LOL_INDEXBUFFER_H__

#include <cstring>

namespace lol
{

class IndexBuffer
{
public:
    IndexBuffer(size_t size);
    ~IndexBuffer();

    void *Lock(size_t offset, size_t size);
    void Unlock();

    void Bind();
    void Unbind();

private:
    class IndexBufferData *m_data;
};

} /* namespace lol */

#endif // __LOL_INDEXBUFFER_H__

