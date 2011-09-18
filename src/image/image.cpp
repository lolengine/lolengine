//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cmath>

#include "core.h"
#include "image-private.h"

using namespace std;

namespace lol
{

bool ImageLoader::RegisterAllLoaders()
{
#if defined __ANDROID__
    REGISTER_IMAGE_LOADER(AndroidImageData)
#endif
    REGISTER_IMAGE_LOADER(DummyImageData)
#if defined __APPLE__ && defined __MACH__
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
    static bool unused = ImageLoader::RegisterAllLoaders();

    data = ImageLoader::Load(path);
}

ivec2 Image::GetSize() const
{
    return data->size;
}

Image::format_t Image::GetFormat() const
{
    return data->format;
}

void * Image::GetData() const
{
    return data->GetData();
}

Image::~Image()
{
    data->Close();
    delete data;
}

} /* namespace lol */

