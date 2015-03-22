//
//  Lol Engine
//
//  Copyright © 2010-2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "image-private.h"

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
static bool RegisterAllCodecs(array<ImageCodec *> &codeclist)
{
#if defined __ANDROID__
    REGISTER_IMAGE_CODEC(AndroidImageCodec)
#endif
#if defined USE_GDIPLUS
    REGISTER_IMAGE_CODEC(GdiPlusImageCodec)
#endif
#if defined __APPLE__ && defined __MACH__ && defined __arm__
    REGISTER_IMAGE_CODEC(IosImageCodec)
#endif
#if defined USE_SDL_IMAGE
    REGISTER_IMAGE_CODEC(SdlImageCodec)
#endif
#if defined USE_IMLIB2
    REGISTER_IMAGE_CODEC(Imlib2ImageCodec)
#endif
    REGISTER_IMAGE_CODEC(DummyImageCodec)
    REGISTER_IMAGE_CODEC(ZedImageCodec)
    REGISTER_IMAGE_CODEC(ZedPaletteImageCodec)
    REGISTER_IMAGE_CODEC(OricImageCodec)

    return true;
}

/*
 * Our static image loader
 */

static class ImageLoader
{
    friend class Image;

public:
    inline ImageLoader()
    {
        RegisterAllCodecs(m_codecs);
    }

private:
    array<ImageCodec *> m_codecs;
    map<String, Image *> m_images;
}
g_image_loader;

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

Image::Image (Image const &other)
  : m_data(new ImageData())
{
    Copy(other);
}

Image & Image::operator =(Image other)
{
    /* Since the argument is passed by value, we’re assured it’s a new
     * object and we can safely swap our m_data pointers. */
    std::swap(m_data, other.m_data);
    return *this;
}

Image::~Image()
{
    for (int k : m_data->m_pixels.keys())
        delete m_data->m_pixels[k];

    delete m_data;
}

void Image::DummyFill()
{
    //TODO: This is not very beautiful
    for (auto codec : g_image_loader.m_codecs)
    {
        if (String(codec->GetName()).Contains("Dummy"))
        {
            codec->Load(this, nullptr);
            return;
        }
    }
}

void Image::Copy(uint8_t* pixels, ivec2 const& size, PixelFormat fmt)
{
    ASSERT(fmt != PixelFormat::Unknown);
    SetSize(size);
    SetFormat(fmt);
    memcpy(m_data->m_pixels[(int)fmt]->data(), pixels,
            size.x * size.y * BytesPerPixel(fmt));
}

void Image::Copy(Image const &other)
{
    ivec2 size = other.GetSize();
    PixelFormat fmt = other.GetFormat();

    SetSize(size);
    if (fmt != PixelFormat::Unknown)
    {
        SetFormat(fmt);
        memcpy(m_data->m_pixels[(int)fmt]->data(),
            other.m_data->m_pixels[(int)fmt]->data(),
            size.x * size.y * BytesPerPixel(fmt));
    }
}

bool Image::Load(char const *path)
{
    ImageCodec* last_codec = nullptr;
    for (auto codec : g_image_loader.m_codecs)
    {
        last_codec = codec;
        if (codec->Load(this, path))
        {
            Log::Info("Image::Load: Codec %s succesfully loaded %s.\n", codec->GetName(), path);
            return true;
        }
    }

    //Log error, because we shouldn't be here
    Log::Error("Image::Load: Last codec %s, Error loading image %s.\n", last_codec->GetName(), path);
    return false;
}

bool Image::Save(char const *path)
{
    ImageCodec* last_codec = nullptr;
    for (auto codec : g_image_loader.m_codecs)
    {
        last_codec = codec;
        if (codec->Save(this, path))
        {
            Log::Info("Image::Save: Codec %s succesfully saved %s.\n", codec->GetName(), path);
            return true;
        }
    }

    //Log error, because we shouldn't be here
    Log::Error("Image::Save: Last codec %s, Error saving image %s.\n", last_codec->GetName(), path);
    return false;
}

ivec2 Image::GetSize() const
{
    return m_data->m_size;
}

void Image::SetSize(ivec2 size)
{
    ASSERT(size.x > 0);
    ASSERT(size.y > 0);

    if (m_data->m_size != size)
    {
        for (int k : m_data->m_pixels.keys())
        {
            delete m_data->m_pixels[k];
            m_data->m_pixels[k] = nullptr;
        }

        m_data->m_format = PixelFormat::Unknown;
    }

    m_data->m_size = size;
}

/* Wrap-around mode for some operations */
WrapMode Image::GetWrapX() const
{
    return m_data->m_wrap_x;
}

WrapMode Image::GetWrapY() const
{
    return m_data->m_wrap_y;
}

void Image::SetWrap(WrapMode wrap_x, WrapMode wrap_y)
{
    m_data->m_wrap_x = wrap_x;
    m_data->m_wrap_y = wrap_y;
}

/* The Lock() method */
template<PixelFormat T> typename PixelType<T>::type *Image::Lock()
{
    SetFormat(T);

    return (typename PixelType<T>::type *)m_data->m_pixels[(int)T]->data();
}

/* The Lock2D() method */
void *Image::Lock2DHelper(PixelFormat T)
{
    SetFormat(T);

    return m_data->m_pixels[(int)T]->data2d();
}

template<typename T>
void Image::Unlock2D(array2d<T> const &array)
{
    ASSERT(m_data->m_pixels.has_key((int)m_data->m_format));
    ASSERT(array.data() == m_data->m_pixels[(int)m_data->m_format]->data());
}

/* Explicit specialisations for the above templates */
#define _T(T) \
    template PixelType<T>::type *Image::Lock<T>(); \
    template array2d<PixelType<T>::type> &Image::Lock2D<T>(); \
    template void Image::Unlock2D(array2d<PixelType<T>::type> const &array);
_T(PixelFormat::Y_8)
_T(PixelFormat::RGB_8)
_T(PixelFormat::RGBA_8)
_T(PixelFormat::Y_F32)
_T(PixelFormat::RGB_F32)
_T(PixelFormat::RGBA_F32)
#undef _T

/* Special case for the "any" format: return the last active buffer */
void *Image::Lock()
{
    ASSERT(m_data->m_format != PixelFormat::Unknown);

    return m_data->m_pixels[(int)m_data->m_format]->data();
}

void Image::Unlock(void const *pixels)
{
    ASSERT(m_data->m_pixels.has_key((int)m_data->m_format));
    ASSERT(pixels == m_data->m_pixels[(int)m_data->m_format]->data());
}

bool Image::RetrieveTiles(array<ivec2, ivec2>& tiles) const
{
    /* TODO: re-implement this */
    //return m_data->m_codecdata->RetrieveTiles(tiles);
    return false;
}

} /* namespace lol */

