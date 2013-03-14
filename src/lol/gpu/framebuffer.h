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
// The FrameBuffer class
// ---------------------
//

#if !defined __LOL_GPU_FRAMEBUFFER_H__
#define __LOL_GPU_FRAMEBUFFER_H__

#include <cstring>

namespace lol
{

class FrameBuffer
{
public:
    FrameBuffer(ivec2 size);
    ~FrameBuffer();

    ShaderTexture GetTexture() const;
    ivec2 GetSize() const;

    void Bind();
    void Unbind();

private:
    class FrameBufferData *m_data;
};

} /* namespace lol */

#endif // __LOL_GPU_FRAMEBUFFER_H__

