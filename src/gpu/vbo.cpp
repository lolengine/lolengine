//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "lolgl.h"
#include "gpu/vbo.h"

using namespace std;

namespace lol
{

/*
 * GpuVbo implementation class
 */

class GpuVboData
{
    friend class GpuVbo;

    size_t elemsize, elemcount;
    uint8_t *alloc_buffer;

    static size_t const GPU_ALIGN = 128;
};

/*
 * Public GpuVbo class
 */

GpuVbo::GpuVbo()
  : data(new GpuVboData())
{
    data->elemsize = 0;
    data->elemcount = 0;
    data->alloc_buffer = 0;
}

void GpuVbo::SetSize(size_t elemsize, size_t elemcount)
{
    size_t oldsize = data->elemsize * data->elemcount;
    size_t newsize = elemsize * elemcount;

    if (newsize == oldsize)
        return;

    if (oldsize)
        delete[] data->alloc_buffer;
    data->alloc_buffer = NULL;
    if (newsize)
        data->alloc_buffer = new uint8_t[newsize + GpuVboData::GPU_ALIGN - 1];

    data->elemsize = elemsize;
    data->elemcount = elemcount;
}

size_t GpuVbo::GetSize()
{
    return data->elemsize * data->elemcount;
}

uint8_t *GpuVbo::GetData()
{
    return (uint8_t *)(((uintptr_t)data->alloc_buffer)
                        & ~((uintptr_t)GpuVboData::GPU_ALIGN - 1));
}

uint8_t const *GpuVbo::GetData() const
{
    return (uint8_t const *)(((uintptr_t)data->alloc_buffer)
                              & ~((uintptr_t)GpuVboData::GPU_ALIGN - 1));
}

void GpuVbo::Bind()
{
}

void GpuVbo::Unbind()
{
}

GpuVbo::~GpuVbo()
{
    delete[] data->alloc_buffer;
    delete data;
}

} /* namespace lol */

