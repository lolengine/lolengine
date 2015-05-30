//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if defined USE_IMLIB2

#include <Imlib2.h>

/* Check that the Imlib2 types are safe */
static_assert(sizeof(DATA64) == sizeof(uint64_t), "Imlib2 type DATA64 is broken");
static_assert(sizeof(DATA32) == sizeof(uint32_t), "Imlib2 type DATA32 is broken");
static_assert(sizeof(DATA16) == sizeof(uint16_t), "Imlib2 type DATA16 is broken");
static_assert(sizeof(DATA8)  == sizeof(uint8_t),  "Imlib2 type DATA8 is broken");

#include "../../image/image-private.h"

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
    Imlib_Image im = nullptr;

    for (auto candidate : System::GetPathList(path))
    {
        im = imlib_load_image(candidate.C());
        if (im)
            break;
    }

    if (!im)
    {
#if !LOL_BUILD_RELEASE
        msg::error("could not load image %s\n", path);
#endif
        return false;
    }

    imlib_context_set_image(im);
    u8vec4 const *srcdata = (u8vec4 *)imlib_image_get_data_for_reading_only();

    if (!srcdata)
    {
        imlib_free_image();
#if !LOL_BUILD_RELEASE
        msg::error("could not get image data for %s\n", path);
#endif
        return false;
    }

    ivec2 size(imlib_image_get_width(), imlib_image_get_height());
    image->SetSize(size);

    u8vec4 *dstdata = image->Lock<PixelFormat::RGBA_8>();

    for (int i = 0; i < size.x * size.y; i++)
    {
        if (is_big_endian())
            dstdata[i] = srcdata[i].argb;
        else
            dstdata[i] = srcdata[i].bgra;
    }
    image->Unlock(dstdata);

    imlib_free_image();

    return true;
}

bool Imlib2ImageCodec::Save(Image *image, char const *path)
{
    ivec2 size = image->GetSize();
    Imlib_Image priv = imlib_create_image(size.x, size.y);

    imlib_context_set_image(priv);
    imlib_image_set_has_alpha(1);

    u8vec4 const *srcdata = image->Lock<PixelFormat::RGBA_8>();
    u8vec4 *dstdata = (u8vec4 *)imlib_image_get_data();

    for (int i = 0; i < size.x * size.y; i++)
    {
        if (is_big_endian())
            dstdata[i] = srcdata[i].argb;
        else
            dstdata[i] = srcdata[i].bgra;
    }

    imlib_image_put_back_data((DATA32 *)dstdata);
    image->Unlock(srcdata);

    imlib_save_image(path);
    imlib_free_image();

    return true;
}

} /* namespace lol */

#endif /* defined USE_IMLIB2 */

