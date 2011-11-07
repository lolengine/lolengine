//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The GpuVbo class
// ----------------
//

#if !defined __LOL_VBO_H__
#define __LOL_VBO_H__

namespace lol
{

class GpuVboData;

class GpuVbo
{
public:
    GpuVbo();
    ~GpuVbo();

    void SetSize(size_t elemsize, size_t elemcount);
    size_t GetSize();
    uint8_t *GetData();
    uint8_t const *GetData() const;

    void Bind();
    void Unbind();

private:
    GpuVboData *data;
};

} /* namespace lol */

#endif // __LOL_VBO_H__

