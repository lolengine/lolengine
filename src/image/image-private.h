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

class ImageLoader
{
    friend class Image;
    friend class ImageData;

public:
    ImageData *(*fun)(char const *path);
    ImageLoader *next;
    int priority;

    static void RegisterLoader(ImageLoader *loader)
    {
        Helper(loader);
    }

private:
    static ImageData *Load(char const *path)
    {
        ImageLoader *parser = Helper(nullptr);
        ImageData *ret = nullptr;

        while (parser && !ret)
        {
            ret = parser->fun(path);
            parser = parser->next;
        }

        return ret;
    }

    static ImageLoader *Helper(ImageLoader *set)
    {
        static ImageLoader *loaders = nullptr;

        if (!set)
            return loaders;

        ImageLoader **parser = &loaders;
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

public:
    virtual ~ImageData() {}

    virtual bool Open(char const *) = 0;
    virtual bool Close() = 0;

    virtual void *GetData() const = 0;

protected:
    ivec2 m_size;
    PixelFormat m_format;
};

#define REGISTER_IMAGE_LOADER(name) \
    extern void Register##name(); \
    Register##name();

#define DECLARE_IMAGE_LOADER(name, prio) \
    template<typename T> class name##ImageLoader : public ImageLoader \
    { \
    public: \
        name##ImageLoader() \
        { \
            static ImageLoader loader; \
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
    name##ImageLoader<name> name##ImageLoaderInstance; \
    void Register##name() \
    { \
        (void)&name##ImageLoaderInstance; \
    } \
    class name : public ImageData

} /* namespace lol */

#endif // __LOL_IMAGE_PRIVATE_H__

