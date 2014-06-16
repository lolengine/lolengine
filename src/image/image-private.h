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
    friend class ImageBank;
    friend class ImageCodec;

public:
    ImageData()
      : m_size(0, 0),
        m_format(PixelFormat::Unknown),
        m_refcount(0),
        m_codecdata(nullptr)
    {}

    ivec2 m_size;

    Map<int, void *> m_pixels;
    PixelFormat m_format;

    int m_refcount;

/* protected: */ /* FIXME: fix the ImageCodec subclasses access rights */
    class ImageCodecData *m_codecdata;
};

class ImageCodec
{
    friend class ImageBank;
    friend class ImageCodecData;

public:
    bool (*m_tryload)(Image *image, char const *path);
    ImageCodec *m_next;
    int m_priority;

    static void RegisterCodec(ImageCodec *codec)
    {
        Helper(codec);
    }

private:
    static void Load(Image *image, char const *path)
    {
        ImageCodec *codec = Helper(nullptr);
        bool success = false;

        while (codec && !success)
        {
            success = codec->m_tryload(image, path);
            codec = codec->m_next;
        }
    }

    static ImageCodec *Helper(ImageCodec *set)
    {
        static ImageCodec *codec_list = nullptr;

        if (!set)
            return codec_list;

        ImageCodec **codec = &codec_list;
        while (*codec && (*codec)->m_priority > set->m_priority)
            codec = &(*codec)->m_next;
        set->m_next = *codec;
        *codec = set;

        return nullptr;
    }
};

/*
 * Codec-specific data that is stored in some images
 */
class ImageCodecData
{
    friend class Image;
    friend class ImageBank;

public:
    inline ImageCodecData() {}
    virtual ~ImageCodecData() {}

    virtual bool Load(Image *, char const *) = 0;
    virtual bool Save(Image *, char const *) = 0;

    virtual bool RetrieveTiles(Array<ivec2, ivec2>& tiles) { return false; }
};

#define REGISTER_IMAGE_CODEC(name) \
    extern void Register##name(); \
    Register##name();

#define DECLARE_IMAGE_CODEC(name, prio) \
    template<typename T> class name##ImageCodec : public ImageCodec \
    { \
    public: \
        name##ImageCodec() \
        { \
            static ImageCodec codec; \
            codec.m_tryload = Load; \
            codec.m_priority = prio; \
            RegisterCodec(&codec); \
        } \
        static bool Load(Image *image, char const *path) \
        { \
            T *codecdata = new T(); \
            bool ret = codecdata->Load(image, path); \
            delete codecdata; \
            return ret; \
        } \
    }; \
    class name; \
    name##ImageCodec<name> name##ImageCodecInstance; \
    void Register##name() \
    { \
        (void)&name##ImageCodecInstance; \
    } \
    class name : public ImageCodecData

} /* namespace lol */

#endif // __LOL_IMAGE_PRIVATE_H__

