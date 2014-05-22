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
 * REGISTER_IMAGE_LOADER macro forward-declares free functions from
 * the "lol" namespace. An apparent bug in Visual Studio's compiler
 * makes it think these functions are actually in the top-level
 * namespace when the forward declaration is in a class member function.
 * To avoid the problem, we make the forward declaration in a free
 * function.
 * The bug was reported to Microsoft and fixed by them, but the fix
 * is not yet available.
 * https://connect.microsoft.com/VisualStudio/feedback/details/730878/ */
static bool RegisterAllLoaders()
{
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
    REGISTER_IMAGE_LOADER(ZedImageData)
    REGISTER_IMAGE_LOADER(ZedPaletteImageData)

    return true;
}

/*
 * Our static image loader
 */

static class ImageLoader
{
public:
    void Init();
    Image *Create(char const *path);
    Image *Get(char const *path);
    Image *Load(char const *path);
    Image *Store(char const *path, Image *image);
    void Destroy(Image *img);

private:
    Map<String, Image *> m_images;
}
image_loader;

void ImageLoader::Init()
{
    /* Initialise loaders (see above) */
    static bool init = RegisterAllLoaders();
    UNUSED(init);
}

Image *ImageLoader::Create(char const *path)
{
    Init();

    /* Is our image already in the bank? If so, no need to create it. */
    Image *img;

    if (m_images.HasKey(path))
    {
        img = m_images[path];
    }
    else
    {
        m_images[path] = Load(path);
        img = m_images[path];
    }

    ++img->m_data->m_refcount;
    return img;
}

Image *ImageLoader::Get(char const *path)
{
    /* Is our image already in the bank? If so, no need to create it. */
    Image *img;
    if (m_images.HasKey(path))
    {
        img = m_images[path];
        ++img->m_data->m_refcount;
        return img;
    }

    return nullptr;
}

Image *ImageLoader::Load(char const *path)
{
    Image* img = new Image();
    img->m_data = ImageCodec::Load(path);
    return img;
}

Image *ImageLoader::Store(char const *path, Image *image)
{
    m_images[path] = image;
    ++image->m_data->m_refcount;
    return image;
}

void ImageLoader::Destroy(Image *img)
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
    return image_loader.Create(path);
}

Image *Image::Store(char const *path, Image *img)
{
    return image_loader.Store(path, img);
}

Image *Image::Load(char const *path)
{
    return image_loader.Load(path);
}

void Image::Destroy(Image *img)
{
    return image_loader.Destroy(img);
}

/*
 * Public Image class
 */

Image::Image()
  : m_data(nullptr)
{
}

ivec2 Image::GetSize() const
{
    return m_data->m_size;
}

PixelFormat Image::GetFormat() const
{
    return m_data->m_format;
}

uint8_t *Image::GetData() const
{
    return m_data->GetData();
}

bool Image::RetrieveTiles(Array<ivec2, ivec2>& tiles) const
{
    return m_data->RetrieveTiles(tiles);
}

Image::~Image()
{
    m_data->Close();
    delete m_data;
}

} /* namespace lol */

