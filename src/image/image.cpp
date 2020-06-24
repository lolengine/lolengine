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

#include "image-private.h"

#include <cassert>   // assert
#include <algorithm> // std::swap

namespace lol
{

/*
 * Public old_image class
 */

old_image::old_image()
  : m_data(new image_data())
{
}

old_image::old_image(std::string const &path)
  : m_data(new image_data())
{
    load(path);
}

old_image::old_image(ivec2 size)
  : m_data(new image_data())
{
    resize(size);
}

old_image::old_image (old_image const &other)
  : m_data(new image_data())
{
    Copy(other);
}

old_image & old_image::operator =(old_image other)
{
    /* Since the argument is passed by value, we’re assured it’s a new
     * object and we can safely swap our m_data pointers. */
    std::swap(m_data, other.m_data);
    return *this;
}

old_image::~old_image()
{
    for (auto &kv : m_data->m_pixels)
        delete kv.second;

    delete m_data;
}

void old_image::Copy(uint8_t* src_pixels, ivec2 const& size, PixelFormat fmt)
{
    assert(fmt != PixelFormat::Unknown);
    resize(size);
    set_format(fmt);
    memcpy(m_data->m_pixels[(int)fmt]->data(), src_pixels,
            size.x * size.y * BytesPerPixel(fmt));
}

void old_image::Copy(old_image const &src)
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

void old_image::DummyFill()
{
    load("DUMMY");
}

bool old_image::load(std::string const &path)
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

bool old_image::save(std::string const &path)
{
    auto data = new ResourceImageData(new old_image(*this));
    auto result = ResourceLoader::Save(path, data);
    delete data;
    return result;
}

ivec2 old_image::size() const
{
    return m_data->m_size;
}

void old_image::resize(ivec2 size)
{
    assert(size.x > 0);
    assert(size.y > 0);

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
WrapMode old_image::GetWrapX() const
{
    return m_data->m_wrap_x;
}

WrapMode old_image::GetWrapY() const
{
    return m_data->m_wrap_y;
}

void old_image::SetWrap(WrapMode wrap_x, WrapMode wrap_y)
{
    m_data->m_wrap_x = wrap_x;
    m_data->m_wrap_y = wrap_y;
}

/* The lock() method */
template<PixelFormat T> typename PixelType<T>::type *old_image::lock()
{
    set_format(T);

    return (typename PixelType<T>::type *)m_data->m_pixels[(int)T]->data();
}

/* The lock2d() method */
void *old_image::lock2d_helper(PixelFormat T)
{
    set_format(T);

    return m_data->m_pixels[(int)T]->data2d();
}

template<typename T>
void old_image::unlock2d(old_array2d<T> const &array)
{
    assert(has_key(m_data->m_pixels, (int)m_data->m_format));
    assert(array.data() == m_data->m_pixels[(int)m_data->m_format]->data());
}

/* Explicit specialisations for the above templates */
#define _T(T) \
    template PixelType<T>::type *old_image::lock<T>(); \
    template old_array2d<PixelType<T>::type> &old_image::lock2d<T>(); \
    template void old_image::unlock2d(old_array2d<PixelType<T>::type> const &array);
_T(PixelFormat::Y_8)
_T(PixelFormat::RGB_8)
_T(PixelFormat::RGBA_8)
_T(PixelFormat::Y_F32)
_T(PixelFormat::RGB_F32)
_T(PixelFormat::RGBA_F32)
#undef _T

/* Special case for the "any" format: return the last active buffer */
void *old_image::lock()
{
    assert(m_data->m_format != PixelFormat::Unknown);

    return m_data->m_pixels[(int)m_data->m_format]->data();
}

void old_image::unlock(void const *pixels)
{
    assert(has_key(m_data->m_pixels, (int)m_data->m_format));
    assert(pixels == m_data->m_pixels[(int)m_data->m_format]->data());
}

} /* namespace lol */

