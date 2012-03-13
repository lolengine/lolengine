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
// The ImageData class
// -------------------
//

#if !defined __LOL_IMAGE_PRIVATE_H__
#define __LOL_IMAGE_PRIVATE_H__

#include "image.h"

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
        ImageLoader *parser = Helper(NULL);
        ImageData *ret = NULL;

        while (parser && !ret)
        {
            ret = parser->fun(path);
            parser = parser->next;
        }

        return ret;
    }

    static ImageLoader *Helper(ImageLoader *set)
    {
        static ImageLoader *loaders = NULL;

        if (!set)
            return loaders;

        ImageLoader **parser = &loaders;
        while (*parser && (*parser)->priority > set->priority)
            parser = &(*parser)->next;
        set->next = *parser;
        *parser = set;

        return NULL;
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
    ivec2 size;
    Image::format_t format;
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
                return NULL; \
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

