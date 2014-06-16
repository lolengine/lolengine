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
static bool RegisterAllCodecs()
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
    void Init();
    Image *Create(char const *path);

    void Unref(Image *img);

private:
    Map<String, Image *> m_images;
}
g_image_bank;

void ImageBank::Init()
{
    /* Initialise codecs (see above) */
    static bool init = RegisterAllCodecs();
    UNUSED(init);
}

Image *ImageBank::Create(char const *path)
{
    Init();

    /* Is our image already in the bank? If so, no need to create it. */
    if (!m_images.HasKey(path))
    {
        m_images[path] = new Image();
        ImageCodec::Load(m_images[path], path);
    }

    Image *img = m_images[path];
    ++img->m_data->m_refcount;
    return img;
}

void ImageBank::Unref(Image *img)
{
    ASSERT(img->m_data->m_refcount > 0);
    if (--img->m_data->m_refcount == 0)
    {
        /* FIXME: unload images etc. here */
        /* XXX: we’re gonna hit a problem here because we didn’t keep
         * the image path within the object, so unless we store it
         * ourselves we’re good for a O(n) lookup… which we can’t do
         * on Map objects anyway. */
        /* TODO: 1. remove image from Map
                 2. delete img; */
    }
}


/*
 * Static image methods
 */

Image *Image::Create(char const *path)
{
    return g_image_bank.Create(path);
}

/*
 * Public Image class
 */

Image::Image()
  : m_data(new ImageData())
{
}

Image::~Image()
{
    delete m_data->m_codecdata;
    delete m_data;
}

void Image::Destroy()
{
    g_image_bank.Unref(this);
}

bool Image::Save(char const *path)
{
    /* FIXME: add autoloading of save codecs */
    if (!m_data->m_codecdata)
        return false;
    return m_data->m_codecdata->Save(this, path);
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

void *Image::LockGeneric()
{
    ASSERT(m_data->m_format != PixelFormat::Unknown);

    return m_data->m_pixels[(int)m_data->m_format];
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
          new typename PixelType<T>::type(m_data->m_size.x * m_data->m_size.y);
    }

    return (typename PixelType<T>::type *)m_data->m_pixels[(int)T];
}

template typename PixelType<PixelFormat::Y_8>::type *
Image::Lock<PixelFormat::Y_8>();
template typename PixelType<PixelFormat::RGB_8>::type *
Image::Lock<PixelFormat::RGB_8>();
template typename PixelType<PixelFormat::RGBA_8>::type *
Image::Lock<PixelFormat::RGBA_8>();

void Image::Unlock()
{
    /* TODO: ensure we are actually unlocking something we locked */
    ASSERT(m_data->m_pixels.HasKey((int)m_data->m_format));
}

bool Image::RetrieveTiles(Array<ivec2, ivec2>& tiles) const
{
    if (!m_data->m_codecdata)
        return false;
    return m_data->m_codecdata->RetrieveTiles(tiles);
}

} /* namespace lol */

