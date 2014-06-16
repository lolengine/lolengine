//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
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

class ImageData
{
    friend class Image;

public:
    ImageData()
      : m_size(0, 0),
        m_format(PixelFormat::Unknown)
    {}

    ivec2 m_size;

    Map<int, void *> m_pixels;
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

