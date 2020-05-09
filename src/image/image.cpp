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
#include <../legacy/lol/base/assert.h>

#include "image-private.h"

#include <algorithm> /* for std::swap */

namespace lol
{

/*
 * Public image class
 */

image::image()
  : m_data(new image_data())
{
}

image::image(std::string const &path)
  : m_data(new image_data())
{
    load(path);
}

image::image(ivec2 size)
  : m_data(new image_data())
{
    resize(size);
}

image::image (image const &other)
  : m_data(new image_data())
{
    Copy(other);
}

image & image::operator =(image other)
{
    /* Since the argument is passed by value, we’re assured it’s a new
     * object and we can safely swap our m_data pointers. */
    std::swap(m_data, other.m_data);
    return *this;
}

image::~image()
{
    for (auto &kv : m_data->m_pixels)
        delete kv.second;

    delete m_data;
}

void image::Copy(uint8_t* src_pixels, ivec2 const& size, PixelFormat fmt)
{
    ASSERT(fmt != PixelFormat::Unknown);
    resize(size);
    set_format(fmt);
    memcpy(m_data->m_pixels[(int)fmt]->data(), src_pixels,
            size.x * size.y * BytesPerPixel(fmt));
}

void image::Copy(image const &src)
{
    ivec2 size = src.size();
    PixelFormat fmt = src.format();

    resize(size);
    if (fmt != PixelFormat::Unknown)
    {
        set_format(fmt);
        memcpy(m_data->m_pixels[(int)fmt]->data(),
            src.m_data->m_pixels[(int)fmt]->data(),
            size.x * size.y * BytesPerPixel(fmt));
    }
}

void image::DummyFill()
{
    load("DUMMY");
}

bool image::load(std::string const &path)
{
    auto resource = ResourceLoader::Load(path);
    if (resource == nullptr)
        return false;

    auto image_resource = dynamic_cast<ResourceImageData*>(resource);
    if (image_resource == nullptr)
    {
        delete image_resource;
        return false;
    }

    Copy(*image_resource->m_image);
    delete image_resource;
    return true;
}

bool image::save(std::string const &path)
{
    auto data = new ResourceImageData(new image(*this));
    auto result = ResourceLoader::Save(path, data);
    delete data;
    return result;
}

ivec2 image::size() const
{
    return m_data->m_size;
}

void image::resize(ivec2 size)
{
    ASSERT(size.x > 0);
    ASSERT(size.y > 0);

    if (m_data->m_size != size)
    {
        for (auto &kv : m_data->m_pixels)
            delete kv.second;
        m_data->m_pixels.clear();
        m_data->m_format = PixelFormat::Unknown;
    }

    m_data->m_size = size;
}

/* Wrap-around mode for some operations */
WrapMode image::GetWrapX() const
{
    return m_data->m_wrap_x;
}

WrapMode image::GetWrapY() const
{
    return m_data->m_wrap_y;
}

void image::SetWrap(WrapMode wrap_x, WrapMode wrap_y)
{
    m_data->m_wrap_x = wrap_x;
    m_data->m_wrap_y = wrap_y;
}

/* The lock() method */
template<PixelFormat T> typename PixelType<T>::type *image::lock()
{
    set_format(T);

    return (typename PixelType<T>::type *)m_data->m_pixels[(int)T]->data();
}

/* The lock2d() method */
void *image::lock2d_helper(PixelFormat T)
{
    set_format(T);

    return m_data->m_pixels[(int)T]->data2d();
}

template<typename T>
void image::unlock2d(array2d<T> const &array)
{
    ASSERT(has_key(m_data->m_pixels, (int)m_data->m_format));
    ASSERT(array.data() == m_data->m_pixels[(int)m_data->m_format]->data());
}

/* Explicit specialisations for the above templates */
#define _T(T) \
    template PixelType<T>::type *image::lock<T>(); \
    template array2d<PixelType<T>::type> &image::lock2d<T>(); \
    template void image::unlock2d(array2d<PixelType<T>::type> const &array);
_T(PixelFormat::Y_8)
_T(PixelFormat::RGB_8)
_T(PixelFormat::RGBA_8)
_T(PixelFormat::Y_F32)
_T(PixelFormat::RGB_F32)
_T(PixelFormat::RGBA_F32)
#undef _T

/* Special case for the "any" format: return the last active buffer */
void *image::lock()
{
    ASSERT(m_data->m_format != PixelFormat::Unknown);

    return m_data->m_pixels[(int)m_data->m_format]->data();
}

void image::unlock(void const *pixels)
{
    ASSERT(has_key(m_data->m_pixels, (int)m_data->m_format));
    ASSERT(pixels == m_data->m_pixels[(int)m_data->m_format]->data());
}

} /* namespace lol */

