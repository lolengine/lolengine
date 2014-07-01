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

#if defined USE_IMLIB2

#include <Imlib2.h>

#include <lol/main.h>
#include "../../image/image-private.h"

using namespace std;

namespace lol
{

/*
 * Imlib2 image codec
 */

class Imlib2ImageCodec : public ImageCodec
{
public:
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);
};

/* Set priority higher than SDL because we can save in many formats. */
DECLARE_IMAGE_CODEC(Imlib2ImageCodec, 70)

bool Imlib2ImageCodec::Load(Image *image, char const *path)
{
    Imlib_Image priv = nullptr;

    array<String> pathlist = System::GetPathList(path);
    for (int i = 0; i < pathlist.Count(); i++)
    {
        priv = imlib_load_image(pathlist[i].C());
        if (priv)
            break;
    }

    if (!priv)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("could not load image %s\n", path);
#endif
        return false;
    }

    imlib_context_set_image(priv);

    if (!imlib_image_get_data())
    {
        imlib_free_image();
#if !LOL_BUILD_RELEASE
        Log::Error("could not get image data for %s\n", path);
#endif
        return false;
    }

    ivec2 size(imlib_image_get_width(), imlib_image_get_height());

    image->SetSize(size);
    u8vec4 *data = image->Lock<PixelFormat::RGBA_8>();
    memcpy(data, imlib_image_get_data(), 4 * size.x * size.y);
    image->Unlock(data);

    imlib_free_image();

    return true;
}

bool Imlib2ImageCodec::Save(Image *image, char const *path)
{
    ivec2 size = image->GetSize();
    Imlib_Image priv = imlib_create_image(size.x, size.y);

    imlib_context_set_image(priv);
    imlib_image_set_has_alpha(1);

    u8vec4 *data = image->Lock<PixelFormat::RGBA_8>();
    memcpy(imlib_image_get_data(), data, 4 * size.x * size.y);
    image->Unlock(data);

    imlib_save_image(path);
    imlib_free_image();

    return true;
}

} /* namespace lol */

#endif /* defined USE_IMLIB2 */

