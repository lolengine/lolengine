//
//  Lol Engine
//
//  Copyright © 2010—2017 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if defined LOL_USE_IMLIB2

#include <Imlib2.h>

/* Check that the Imlib2 types are safe */
static_assert(sizeof(DATA64) == sizeof(uint64_t), "Imlib2 type DATA64 is broken");
static_assert(sizeof(DATA32) == sizeof(uint32_t), "Imlib2 type DATA32 is broken");
static_assert(sizeof(DATA16) == sizeof(uint16_t), "Imlib2 type DATA16 is broken");
static_assert(sizeof(DATA8)  == sizeof(uint8_t),  "Imlib2 type DATA8 is broken");

#include "../../image/resource-private.h"

namespace lol
{

/*
 * Imlib2 image codec
 */

class Imlib2ImageCodec : public ResourceCodec
{
public:
    virtual char const *GetName() { return "<Imlib2ImageCodec>"; }
    virtual ResourceCodecData* Load(char const *path);
    virtual bool Save(char const *path, ResourceCodecData* data);
};

/* Set priority higher than SDL because we can save in many formats. */
DECLARE_IMAGE_CODEC(Imlib2ImageCodec, 70)

ResourceCodecData *Imlib2ImageCodec::Load(char const *path)
{
    Imlib_Image im = nullptr;

    for (auto candidate : sys::get_path_list(path))
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
        return nullptr;
    }

    imlib_context_set_image(im);
    u8vec4 const *srcdata = (u8vec4 *)imlib_image_get_data_for_reading_only();

    if (!srcdata)
    {
        imlib_free_image();
#if !LOL_BUILD_RELEASE
        msg::error("could not get image data for %s\n", path);
#endif
        return nullptr;
    }

    ivec2 size(imlib_image_get_width(), imlib_image_get_height());
    auto data = new ResourceImageData(new image(size));
    auto image = data->m_image;

    u8vec4 *dstdata = image->lock<PixelFormat::RGBA_8>();

    for (int i = 0; i < size.x * size.y; i++)
    {
        if (is_big_endian())
            dstdata[i] = srcdata[i].argb;
        else
            dstdata[i] = srcdata[i].bgra;
    }
    image->unlock(dstdata);

    imlib_free_image();

    return data;
}

bool Imlib2ImageCodec::Save(char const *path, ResourceCodecData *data)
{
    auto data_image = dynamic_cast<ResourceImageData*>(data);
    if (data_image == nullptr)
        return false;

    auto image = data_image->m_image;
    ivec2 size = image->size();
    Imlib_Image priv = imlib_create_image(size.x, size.y);

    imlib_context_set_image(priv);
    imlib_image_set_has_alpha(1);

    u8vec4 const *srcdata = image->lock<PixelFormat::RGBA_8>();
    u8vec4 *dstdata = (u8vec4 *)imlib_image_get_data();

    for (int i = 0; i < size.x * size.y; i++)
    {
        if (is_big_endian())
            dstdata[i] = srcdata[i].argb;
        else
            dstdata[i] = srcdata[i].bgra;
    }

    imlib_image_put_back_data((DATA32 *)dstdata);
    image->unlock(srcdata);

    imlib_save_image(path);
    imlib_free_image();

    return true;
}

} /* namespace lol */

#endif /* defined LOL_USE_IMLIB2 */

