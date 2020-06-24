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

#pragma once

#include <map>

//
// The ImageCodecData class
// ------------------------
//

namespace lol
{

class PixelDataBase
{
public:
    virtual void *data() = 0;
    virtual void const *data() const = 0;
    virtual void *data2d() = 0;
    virtual void const *data2d() const = 0;

    inline virtual ~PixelDataBase() {}
};

template<PixelFormat T>
class PixelData : public PixelDataBase
{
public:
    inline PixelData(ivec2 size) { m_array2d.resize(size); }

    virtual void *data() { return m_array2d.data(); }
    virtual void const *data() const { return m_array2d.data(); }
    virtual void *data2d() { return &m_array2d; }
    virtual void const *data2d() const { return &m_array2d; }

    old_array2d<typename PixelType<T>::type> m_array2d;
};

class image_data
{
    friend class old_image;

public:
    image_data()
      : m_size(0, 0),
        m_wrap_x(WrapMode::Clamp),
        m_wrap_y(WrapMode::Clamp),
        m_format(PixelFormat::Unknown)
    {}

    ivec2 m_size;

    /* The wrap modes for pixel access */
    WrapMode m_wrap_x, m_wrap_y;

    /* A map of the various available bitplanes */
    std::map<int, PixelDataBase *> m_pixels;
    /* The last bitplane being accessed for writing */
    PixelFormat m_format;
};

} /* namespace lol */

