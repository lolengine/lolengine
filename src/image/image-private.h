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
// The ImageData class
// -------------------
//

#if !defined __LOL_IMAGE_PRIVATE_H__
#define __LOL_IMAGE_PRIVATE_H__

namespace lol
{

class ImageCodec
{
    friend class ImageLoader;
    friend class ImageData;

public:
    ImageData *(*fun)(char const *path);
    ImageCodec *next;
    int priority;

    static void RegisterLoader(ImageCodec *loader)
    {
        Helper(loader);
    }

private:
    static ImageData *Load(char const *path)
    {
        ImageCodec *parser = Helper(nullptr);
        ImageData *ret = nullptr;

        while (parser && !ret)
        {
            ret = parser->fun(path);
            parser = parser->next;
        }

        return ret;
    }

    static ImageCodec *Helper(ImageCodec *set)
    {
        static ImageCodec *loaders = nullptr;

        if (!set)
            return loaders;

        ImageCodec **parser = &loaders;
        while (*parser && (*parser)->priority > set->priority)
            parser = &(*parser)->next;
        set->next = *parser;
        *parser = set;

        return nullptr;
    }
};

class ImageData
{
    friend class Image;
    friend class ImageLoader;

public:
    inline ImageData()
      : m_size(0, 0),
        m_format(PixelFormat::Unknown),
        m_refcount(0)
    { }

    virtual ~ImageData() {}

    virtual bool Open(char const *) = 0;
    virtual bool Save(char const *) = 0;
    virtual bool Close() = 0;

    virtual uint8_t *GetData() const = 0;
    virtual bool RetrieveTiles(Array<ivec2, ivec2>& tiles) { return false; }

protected:
    ivec2 m_size;
    PixelFormat m_format;
    int m_refcount;
};

#define REGISTER_IMAGE_LOADER(name) \
    extern void Register##name(); \
    Register##name();

#define DECLARE_IMAGE_LOADER(name, prio) \
    template<typename T> class name##ImageCodec : public ImageCodec \
    { \
    public: \
        name##ImageCodec() \
        { \
            static ImageCodec loader; \
            loader.fun = Load; \
            loader.priority = prio; \
            RegisterLoader(&loader); \
        } \
        static ImageData *Load(char const *path) \
        { \
            T *ret = new T(); \
            if (!ret->Open(path)) \
            { \
                delete ret; \
                return nullptr; \
            } \
            return ret; \
        } \
    }; \
    class name; \
    name##ImageCodec<name> name##ImageCodecInstance; \
    void Register##name() \
    { \
        (void)&name##ImageCodecInstance; \
    } \
    class name : public ImageData

} /* namespace lol */

#endif // __LOL_IMAGE_PRIVATE_H__

