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

#if defined USE_GDIPLUS

#include <algorithm>
using std::min;
using std::max;

#include <windows.h>
#include <gdiplus.h>

#include <lol/main.h>
#include "../../image/image-private.h"

namespace lol
{

/*
 * Image implementation class
 */

class GdiPlusImageCodec : public ImageCodec
{
public:
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);
};

DECLARE_IMAGE_CODEC(GdiPlusImageCodec, 100)

/*
 * Public Image class
 */

bool GdiPlusImageCodec::Load(Image *image, char const *path)
{
    Gdiplus::Status status;
    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;
    status = Gdiplus::GdiplusStartup(&token, &input, nullptr);
    if (status != Gdiplus::Ok)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("error %d while initialising GDI+\n", status);
#endif
        return false;
    }

    array<String> pathlist = System::GetPathList(path);
    Gdiplus::Bitmap *bitmap = nullptr;
    for (auto fullpath : pathlist)
    {
        size_t len;
        len = mbstowcs(nullptr, fullpath.C(), 0);
        wchar_t *wpath = new wchar_t[len + 1];
        if (mbstowcs(wpath, fullpath.C(), len + 1) == (size_t)-1)
        {
#if !LOL_BUILD_RELEASE
            Log::Error("invalid image name %s\n", fullpath.C());
#endif
            delete[] wpath;
            continue;
        }

        status = Gdiplus::Ok;
        bitmap = Gdiplus::Bitmap::FromFile(wpath, 0);

        if (bitmap)
        {
            status = bitmap->GetLastStatus();
            if (status != Gdiplus::Ok)
            {
#if !LOL_BUILD_RELEASE
                if (status != Gdiplus::InvalidParameter)
                    Log::Error("error %d loading %s\n",
                               status, fullpath.C());
#endif
                delete bitmap;
                bitmap = nullptr;
            }
        }

        delete[] wpath;
        if (bitmap)
            break;
    }

    if (!bitmap)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        return false;
    }

    ivec2 size(bitmap->GetWidth(), bitmap->GetHeight());
    Gdiplus::Rect rect(0, 0, size.x, size.y);
    Gdiplus::BitmapData bdata;
    if (bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead,
                           PixelFormat32bppARGB, &bdata) != Gdiplus::Ok)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("could not lock bits in %s\n", path);
#endif
        delete bitmap;
        return false;
    }

    /* FIXME: GDI+ doesn't know about RGBA, only ARGB. And OpenGL doesn't
     * know about ARGB, only RGBA. So we swap bytes. We could also fix
     * this in the shader. */
    image->SetSize(size);
    u8vec4 *pdst = image->Lock<PixelFormat::RGBA_8>();
    u8vec4 *psrc = static_cast<u8vec4 *>(bdata.Scan0);
    for (int n = 0; n < size.x * size.y; n++)
        pdst[n] = psrc[n].bgra;
    image->Unlock(pdst);

    bitmap->UnlockBits(&bdata);
    delete bitmap;

    return true;
}

bool GdiPlusImageCodec::Save(Image *image, char const *path)
{
    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&token, &input, NULL);

    wchar_t const *fmt;
    if (strstr(path, ".gif"))
        fmt = L"image/gif";
    else if (strstr(path, ".jpg") || strstr(path, ".jpeg"))
        fmt = L"image/jpeg";
    else if (strstr(path, ".png"))
        fmt = L"image/png";
    else if (strstr(path, ".tiff"))
        fmt = L"image/tiff";
    else /* if (strstr(path, ".bmp")) */
        fmt = L"image/bmp";

    unsigned int num = 0, encoder_size = 0;
    Gdiplus::GetImageEncodersSize(&num, &encoder_size);
    if (num == 0)
    {
        Log::Error("no GDI+ image encoders found\n");
        return false;
    }
    Gdiplus::ImageCodecInfo *codecs
        = (Gdiplus::ImageCodecInfo *)new uint8_t[encoder_size];
    Gdiplus::GetImageEncoders(num, encoder_size, codecs);
    CLSID clsid;
    for (unsigned int i = 0; i < num; i++)
    {
        if (wcscmp(codecs[i].MimeType, fmt) == 0)
        {
            clsid = codecs[i].Clsid;
            break;
        }
    }

    size_t len;
    len = mbstowcs(NULL, path, 0);
    wchar_t *wpath = new wchar_t[len + 1];
    if (mbstowcs(wpath, path, len + 1) == (size_t)-1)
    {
        Log::Error("could not convert GDI+ filename '%s' to widechar\n", path);
        delete[] wpath;
        delete[] codecs;
        return false;
    }

    ivec2 size = image->GetSize();

    /* FIXME: we create a new image because there is no guarantee that
     * the image comes from GDI. But the engine architecture doesn't
     * allow us to save other images anyway. */
    Gdiplus::Bitmap *b = new Gdiplus::Bitmap(size.x, size.y,
                                             PixelFormat32bppARGB);

    Gdiplus::BitmapData bdata;
    Gdiplus::Rect rect(0, 0, size.x, size.y);

    if (b->LockBits(&rect, (unsigned int)Gdiplus::ImageLockModeWrite,
                    PixelFormat32bppARGB, &bdata) != Gdiplus::Ok)
    {
        Log::Error("could not lock GDI+ image bits\n");
        delete b;
        delete[] wpath;
        delete[] codecs;
        return false;
    }

    u8vec4 *psrc = image->Lock<PixelFormat::RGBA_8>();
    u8vec4 *pdst = static_cast<u8vec4 *>(bdata.Scan0);
    for (int y = 0; y < size.y; y++)
        for (int x = 0; x < size.x; x++)
            *pdst++ = (*psrc++).bgra;
    image->Unlock(pdst);
    b->UnlockBits(&bdata);

    if (b->Save(wpath, &clsid, NULL) != Gdiplus::Ok)
    {
        Log::Error("could not save GDI+ image '%s'\n", path);
        delete[] wpath;
        delete[] codecs;
        delete b;
        return false;
    }
    delete[] wpath;
    delete[] codecs;
    delete b;

    return true;
}

} /* namespace lol */

#endif /* defined USE_GDIPLUS */

