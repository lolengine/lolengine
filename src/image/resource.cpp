//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2016—2017 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>
#include <lol/msg>

#include "resource-private.h"

#include <algorithm> /* for std::swap */

namespace lol
{

/* HACK: We cannot make this an ImageLoader member function, because the
 * REGISTER_IMAGE_CODEC macro forward-declares free functions from
 * the "lol" namespace. An apparent bug in Visual Studio's compiler
 * makes it think these functions are actually in the top-level
 * namespace when the forward declaration is in a class member function.
 * To avoid the problem, we make the forward declaration in a free
 * function.
 * The bug was reported to Microsoft and fixed by them, but the fix
 * is not yet available.
 * https://connect.microsoft.com/VisualStudio/feedback/details/730878/ */
static bool RegisterAllCodecs(std::vector<ResourceCodec *> &codeclist)
{
#if defined __ANDROID__
    REGISTER_IMAGE_CODEC(AndroidImageCodec)
#endif
#if defined LOL_USE_GDIPLUS
    REGISTER_IMAGE_CODEC(GdiPlusImageCodec)
#endif
#if defined __APPLE__ && defined __MACH__ && defined __arm__
    REGISTER_IMAGE_CODEC(IosImageCodec)
#endif
#if defined LOL_USE_SDL_IMAGE
    REGISTER_IMAGE_CODEC(SdlImageCodec)
#endif
#if defined LOL_USE_IMLIB2
    REGISTER_IMAGE_CODEC(Imlib2ImageCodec)
#endif
    REGISTER_IMAGE_CODEC(OricImageCodec)

    REGISTER_IMAGE_CODEC(DummyImageCodec)

    return true;
}

/*
* Our static old_image loader
*/

static class StaticResourceLoader
{
    friend class ResourceLoader;

public:
    inline StaticResourceLoader()
    {
        RegisterAllCodecs(m_codecs);
    }

private:
    std::vector<ResourceCodec *> m_codecs;
}
g_resource_loader;

/*
* The public resource loader
*/

ResourceCodecData* ResourceLoader::Load(std::string const &path)
{
    ResourceCodec* last_codec = nullptr;
    for (auto codec : g_resource_loader.m_codecs)
    {
        last_codec = codec;
        auto data = codec->Load(path);
        if (data != nullptr)
        {
            msg::debug("old_image::load: codec %s succesfully loaded %s.\n",
                       codec->GetName().c_str(), path.c_str());
            return data;
        }
    }

    //Log error, because we shouldn't be here
    msg::error("old_image::load: last codec %s, error loading resource %s.\n",
               last_codec->GetName().c_str(), path.c_str());
    return nullptr;
}

bool ResourceLoader::Save(std::string const &path, ResourceCodecData* data)
{
    ResourceCodec* last_codec = nullptr;
    for (auto codec : g_resource_loader.m_codecs)
    {
        last_codec = codec;
        if (codec->Save(path, data))
        {
            msg::debug("old_image::save: codec %s succesfully saved %s.\n",
                      codec->GetName().c_str(), path.c_str());
            return true;
        }
    }

    //Log error, because we shouldn't be here
    msg::error("old_image::save: last codec %s, error saving resource %s.\n",
               last_codec->GetName().c_str(), path.c_str());
    return false;
}


} /* namespace lol */

