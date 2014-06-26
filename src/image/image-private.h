//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The ImageCodecData class
// ------------------------
//

#if !defined __LOL_IMAGE_PRIVATE_H__
#define __LOL_IMAGE_PRIVATE_H__

namespace lol
{

class PixelDataBase
{
public:
    virtual void *Data() = 0;
    virtual void const *Data() const = 0;
    virtual void *Data2D() = 0;
    virtual void const *Data2D() const = 0;

    inline virtual ~PixelDataBase() {}
};

template<PixelFormat T>
class PixelData : public PixelDataBase
{
public:
    inline PixelData(ivec2 size) { m_array2d.SetSize(size); }

    virtual void *Data() { return m_array2d.Data(); }
    virtual void const *Data() const { return m_array2d.Data(); }
    virtual void *Data2D() { return &m_array2d; }
    virtual void const *Data2D() const { return &m_array2d; }

    Array2D<typename PixelType<T>::type> m_array2d;
};

class ImageData
{
    friend class Image;

public:
    ImageData()
      : m_size(0, 0),
        m_wrap_x(WrapMode::Clamp),
        m_wrap_y(WrapMode::Clamp),
        m_format(PixelFormat::Unknown)
    {}

    ivec2 m_size;

    /* The wrap modes for pixel access */
    WrapMode m_wrap_x, m_wrap_y;

    /* A map of the various available bitplanes */
    Map<int, PixelDataBase *> m_pixels;
    /* The last bitplane being accessed for writing */
    PixelFormat m_format;
};

class ImageCodec
{
public:
    virtual bool Load(Image *image, char const *path) = 0;
    virtual bool Save(Image *image, char const *path) = 0;

    /* TODO: this should become more fine-grained */
    int m_priority;
};

#define REGISTER_IMAGE_CODEC(name) \
    extern ImageCodec *Register##name(); \
    { \
        /* Insert image codecs in a sorted list */ \
        ImageCodec *codec = Register##name(); \
        int i = 0, prio = codec->m_priority; \
        for ( ; i < codeclist.Count(); ++i) \
        { \
            if (codeclist[i]->m_priority <= prio) \
                break; \
        } \
        codeclist.Insert(codec, i); \
    }

#define DECLARE_IMAGE_CODEC(name, priority) \
    ImageCodec *Register##name() \
    { \
        ImageCodec *ret = new name(); \
        ret->m_priority = priority; \
        return ret; \
    }

} /* namespace lol */

#endif // __LOL_IMAGE_PRIVATE_H__

