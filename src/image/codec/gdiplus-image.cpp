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

using namespace std;

#include <windows.h>
#include <gdiplus.h>

#include "core.h"
#include "../../image/image-private.h"

namespace lol
{

/*
 * Image implementation class
 */

DECLARE_IMAGE_CODEC(GdiPlusImageCodec, 100)
{
public:
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);
    virtual bool Close();

    virtual uint8_t *GetData() const;

private:
    Gdiplus::Bitmap *m_bitmap;
    Gdiplus::BitmapData m_bdata;
};

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

    Array<String> pathlist = System::GetPathList(path);
    m_bitmap = nullptr;
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
        m_bitmap = Gdiplus::Bitmap::FromFile(wpath, 0);

        if (m_bitmap)
        {
            status = m_bitmap->GetLastStatus();
            if (status != Gdiplus::Ok)
            {
#if !LOL_BUILD_RELEASE
                if (status != Gdiplus::InvalidParameter)
                    Log::Error("error %d loading %s\n",
                               status, fullpath.C());
#endif
                delete m_bitmap;
                m_bitmap = nullptr;
            }
        }

        delete[] wpath;
        if (m_bitmap)
            break;
    }

    if (!m_bitmap)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        return false;
    }

    image->m_data->m_size = ivec2(m_bitmap->GetWidth(),
                                  m_bitmap->GetHeight());
    image->m_data->m_format = PixelFormat::RGBA_8;

    Gdiplus::Rect rect(0, 0, image->m_data->m_size.x, image->m_data->m_size.y);
    if (m_bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead,
                           PixelFormat32bppARGB, &m_bdata) != Gdiplus::Ok)
    {
#if !LOL_BUILD_RELEASE
        Log::Error("could not lock bits in %s\n", path);
#endif
        delete m_bitmap;
        return false;
    }

    /* FIXME: GDI+ doesn't know about RGBA, only ARGB. And OpenGL doesn't
     * know about ARGB, only RGBA. So we swap bytes. We could also fix
     * this in the shader. */
    uint8_t *p = static_cast<uint8_t *>(m_bdata.Scan0);
    for (int y = 0; y < image->m_data->m_size.y; y++)
        for (int x = 0; x < image->m_data->m_size.x; x++)
        {
            uint8_t tmp = p[2];
            p[2] = p[0];
            p[0] = tmp;
            p += 4;
        }

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

    unsigned int num = 0, size = 0;
    Gdiplus::GetImageEncodersSize(&num, &size);
    if (num == 0)
    {
        Log::Error("no GDI+ image encoders found\n");
        return false;
    }
    Gdiplus::ImageCodecInfo *codecs
        = (Gdiplus::ImageCodecInfo *)new uint8_t[size];
    Gdiplus::GetImageEncoders(num, size, codecs);
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

    /* FIXME: we create a new image because there is no guarantee that
     * the image comes from GDI. But the engine architecture doesn't
     * allow us to save other images anyway. */
    Gdiplus::Bitmap *b = new Gdiplus::Bitmap(image->m_data->m_size.x,
                                             image->m_data->m_size.y,
                                             PixelFormat32bppARGB);

    Gdiplus::BitmapData bdata;
    Gdiplus::Rect rect(0, 0, image->m_data->m_size.x, image->m_data->m_size.y);

    if (b->LockBits(&rect, (unsigned int)Gdiplus::ImageLockModeWrite,
                    PixelFormat32bppARGB, &bdata) != Gdiplus::Ok)
    {
        Log::Error("could not lock GDI+ image bits\n");
        delete b;
        delete[] wpath;
        delete[] codecs;
        return false;
    }

    u8vec4 *psrc = static_cast<u8vec4 *>(m_bdata.Scan0);
    u8vec4 *pdst = static_cast<u8vec4 *>(bdata.Scan0);
    for (int y = 0; y < image->m_data->m_size.y; y++)
        for (int x = 0; x < image->m_data->m_size.x; x++)
        {
            *pdst++ = (*psrc++).bgra;
        }

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

bool GdiPlusImageCodec::Close()
{
    m_bitmap->UnlockBits(&m_bdata);
    delete m_bitmap;

    return true;
}

uint8_t * GdiPlusImageCodec::GetData() const
{
    return static_cast<uint8_t *>(m_bdata.Scan0);
}

} /* namespace lol */

#endif /* defined USE_GDIPLUS */

