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

/* HACK: We cannot make this an ImageCodec member function, because the
 * REGISTER_IMAGE_CODEC macro forward-declares free functions from
 * the "lol" namespace. An apparent bug in Visual Studio's compiler
 * makes it think these functions are actually in the top-level
 * namespace when the forward declaration is in a class member function.
 * To avoid the problem, we make the forward declaration in a free
 * function.
 * The bug was reported to Microsoft and fixed by them, but the fix
 * is not yet available.
 * https://connect.microsoft.com/VisualStudio/feedback/details/730878/ */
static bool RegisterAllCodecs(Array<ImageCodec *> &codeclist)
{
#if defined __ANDROID__
    REGISTER_IMAGE_CODEC(AndroidImageCodec)
#endif
    REGISTER_IMAGE_CODEC(DummyImageCodec)
#if defined USE_GDIPLUS
    REGISTER_IMAGE_CODEC(GdiPlusImageCodec)
#endif
#if defined __APPLE__ && defined __MACH__ && defined __arm__
    REGISTER_IMAGE_CODEC(IosImageCodec)
#endif
#if defined __CELLOS_LV2__
    REGISTER_IMAGE_CODEC(Ps3ImageCodec)
#endif
#if defined USE_SDL_IMAGE
    REGISTER_IMAGE_CODEC(SdlImageCodec)
#endif
    REGISTER_IMAGE_CODEC(ZedImageCodec)
    REGISTER_IMAGE_CODEC(ZedPaletteImageCodec)

    return true;
}

/*
 * Our static image loader
 */

static class ImageBank
{
public:
    ImageBank();

    bool Load(Image *image, char const *path);
    bool Save(Image *image, char const *path);

private:
    Array<ImageCodec *> m_codecs;
    Map<String, Image *> m_images;
}
g_image_bank;

ImageBank::ImageBank()
{
    RegisterAllCodecs(m_codecs);
}

bool ImageBank::Load(Image *image, char const *path)
{
    /* FIXME: priority is ignored */
    for (auto codec : m_codecs)
        if (codec->Load(image, path))
            return true;
    return false;
}

bool ImageBank::Save(Image *image, char const *path)
{
    /* FIXME: priority is ignored */
    for (auto codec : m_codecs)
        if (codec->Save(image, path))
            return true;
    return false;
}

/*
 * Public Image class
 */

Image::Image()
  : m_data(new ImageData())
{
}

Image::Image(char const *path)
  : m_data(new ImageData())
{
    Load(path);
}

Image::Image(ivec2 size)
  : m_data(new ImageData())
{
    SetSize(size);
}

Image::~Image()
{
    delete m_data;
}

bool Image::Load(char const *path)
{
    return g_image_bank.Load(this, path);
}

bool Image::Save(char const *path)
{
    return g_image_bank.Save(this, path);
}

ivec2 Image::GetSize() const
{
    return m_data->m_size;
}

void Image::SetSize(ivec2 size)
{
    if (m_data->m_size != size)
    {
        /* FIXME: delete data or resize it */
        if (m_data->m_format != PixelFormat::Unknown)
        {
            m_data->m_pixels.Remove((int)m_data->m_format);
        }
    }
    m_data->m_size = size;

    /* FIXME: don’t do this! */
    if (m_data->m_format != PixelFormat::Unknown)
    {
        Lock<PixelFormat::RGBA_8>();
        Unlock();
    }
}

PixelFormat Image::GetFormat() const
{
    return m_data->m_format;
}

/* The Lock() method and its explicit specialisations */
template<PixelFormat T>
typename PixelType<T>::type *Image::Lock()
{
    /* TODO: convert data if this doesn’t match */
    ASSERT(m_data->m_format == T || m_data->m_format == PixelFormat::Unknown);
    m_data->m_format = (PixelFormat)T;

    if (!m_data->m_pixels.HasKey((int)T))
    {
        m_data->m_pixels[(int)T] =
          new typename PixelType<T>::type[m_data->m_size.x * m_data->m_size.y];
    }

    return (typename PixelType<T>::type *)m_data->m_pixels[(int)T];
}

template PixelType<PixelFormat::Y_8>::type *
Image::Lock<PixelFormat::Y_8>();
template PixelType<PixelFormat::RGB_8>::type *
Image::Lock<PixelFormat::RGB_8>();
template PixelType<PixelFormat::RGBA_8>::type *
Image::Lock<PixelFormat::RGBA_8>();

/* Special case for the "any" format */
template<>
void *Image::Lock<PixelFormat::Unknown>()
{
    ASSERT(m_data->m_format != PixelFormat::Unknown);

    return m_data->m_pixels[(int)m_data->m_format];
}

void Image::Unlock()
{
    /* TODO: ensure we are actually unlocking something we locked */
    ASSERT(m_data->m_pixels.HasKey((int)m_data->m_format));
}

bool Image::RetrieveTiles(Array<ivec2, ivec2>& tiles) const
{
    /* TODO: re-implement this */
    //return m_data->m_codecdata->RetrieveTiles(tiles);
    return false;
}

} /* namespace lol */

