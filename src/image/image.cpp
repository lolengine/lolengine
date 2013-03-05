//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "image-private.h"

using namespace std;

namespace lol
{

static bool RegisterAllLoaders()
{
    /* We cannot make this an ImageLoader member function, because the
     * REGISTER_IMAGE_LOADER macro forward-declares free functions from
     * the "lol" namespace. An apparent bug in Visual Studio's compiler
     * makes it think these functions are actually in the top-level
     * namespace when the forward declaration is in a class member function.
     * To avoid the problem, we make the forward declaration in a free
     * function. */
#if defined __ANDROID__
    REGISTER_IMAGE_LOADER(AndroidImageData)
#endif
    REGISTER_IMAGE_LOADER(DummyImageData)
#if defined USE_GDIPLUS
    REGISTER_IMAGE_LOADER(GdiPlusImageData)
#endif
#if defined __APPLE__ && defined __MACH__ && defined __arm__
    REGISTER_IMAGE_LOADER(IosImageData)
#endif
#if defined __CELLOS_LV2__
    REGISTER_IMAGE_LOADER(Ps3ImageData)
#endif
#if defined USE_SDL_IMAGE
    REGISTER_IMAGE_LOADER(SdlImageData)
#endif

    return true;
}

/*
 * Public Image class
 */

Image::Image(char const *path)
{
    static bool init = RegisterAllLoaders();
    UNUSED(init);

    m_data = ImageLoader::Load(path);
}

ivec2 Image::GetSize() const
{
    return m_data->m_size;
}

PixelFormat Image::GetFormat() const
{
    return m_data->m_format;
}

void * Image::GetData() const
{
    return m_data->GetData();
}

Image::~Image()
{
    m_data->Close();
    delete m_data;
}

} /* namespace lol */

