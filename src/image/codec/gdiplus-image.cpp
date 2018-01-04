//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if LOL_USE_GDIPLUS
#   include <string>
#   include <algorithm>
using std::min;
using std::max;
#   include <windows.h>
#   include <objidl.h> // for DEFINE_GUID
#   include <gdiplus.h>

#include "../../image/resource-private.h"

namespace lol
{

/*
 * Image implementation class
 */

class GdiPlusImageCodec : public ResourceCodec
{
public:
    virtual std::string GetName() { return "<GdiPlusImageCodec>"; }
    virtual ResourceCodecData* Load(std::string const &path);
    virtual bool Save(std::string const &path, ResourceCodecData* data);
};

DECLARE_IMAGE_CODEC(GdiPlusImageCodec, 100)

/*
 * Public Image class
 */

ResourceCodecData* GdiPlusImageCodec::Load(std::string const &path)
{
    Gdiplus::Status status;
    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;
    status = Gdiplus::GdiplusStartup(&token, &input, nullptr);
    if (status != Gdiplus::Ok)
    {
        msg::error("error %d while initialising GDI+\n", status);
        return nullptr;
    }

    array<std::string> pathlist = sys::get_path_list(path);
    Gdiplus::Bitmap *bitmap = nullptr;
    for (auto const &fullpath : pathlist)
    {
        size_t len;
        len = mbstowcs(nullptr, fullpath.c_str(), 0);
        wchar_t *wpath = new wchar_t[len + 1];
        if (mbstowcs(wpath, fullpath.c_str(), len + 1) == (size_t)-1)
        {
            msg::error("invalid image name %s\n", fullpath.c_str());
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
                if (status != Gdiplus::InvalidParameter)
                    msg::error("error %d loading %s\n",
                               status, fullpath.c_str());
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
        msg::error("could not load %s\n", path);
        return nullptr;
    }

    ivec2 size(bitmap->GetWidth(), bitmap->GetHeight());
    Gdiplus::Rect rect(0, 0, size.x, size.y);
    Gdiplus::BitmapData bdata;
    if (bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead,
                           PixelFormat32bppARGB, &bdata) != Gdiplus::Ok)
    {
        msg::error("could not lock bits in %s\n", path);
        delete bitmap;
        return nullptr;
    }

    /* FIXME: GDI+ doesn't know about RGBA, only ARGB. And OpenGL doesn't
     * know about ARGB, only RGBA. So we swap bytes. We could also fix
     * this in the shader. */
    auto data = new ResourceImageData(new image(ivec2(size)));
    auto image = data->m_image;
    u8vec4 *pdst = image->lock<PixelFormat::RGBA_8>();
    u8vec4 *psrc = static_cast<u8vec4 *>(bdata.Scan0);
    for (int n = 0; n < size.x * size.y; n++)
        pdst[n] = psrc[n].bgra;
    image->unlock(pdst);

    bitmap->UnlockBits(&bdata);
    delete bitmap;

    return data;
}

bool GdiPlusImageCodec::Save(std::string const &path, ResourceCodecData* data)
{
    auto data_image = dynamic_cast<ResourceImageData*>(data);
    if (data_image == nullptr)
        return false;

    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&token, &input, nullptr);

    wchar_t const *fmt;
    if (ends_with(path, ".gif"))
        fmt = L"image/gif";
    else if (ends_with(path, ".jpg") || ends_with(path, ".jpeg"))
        fmt = L"image/jpeg";
    else if (ends_with(path, ".png"))
        fmt = L"image/png";
    else if (ends_with(path, ".tiff"))
        fmt = L"image/tiff";
    else /* if (ends_with(path, ".bmp")) */
        fmt = L"image/bmp";

    unsigned int num = 0, encoder_size = 0;
    Gdiplus::GetImageEncodersSize(&num, &encoder_size);
    if (num == 0)
    {
        msg::error("no GDI+ image encoders found\n");
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
    len = mbstowcs(nullptr, path.c_str(), 0);
    wchar_t *wpath = new wchar_t[len + 1];
    if (mbstowcs(wpath, path.c_str(), len + 1) == (size_t)-1)
    {
        msg::error("could not convert GDI+ filename '%s' to widechar\n", path.c_str());
        delete[] wpath;
        delete[] codecs;
        return false;
    }

    auto image = data_image->m_image;
    ivec2 size = image->size();

    Gdiplus::Bitmap *b = new Gdiplus::Bitmap(size.x, size.y,
                                             PixelFormat32bppARGB);
    Gdiplus::BitmapData bdata;
    Gdiplus::Rect rect(0, 0, size.x, size.y);

    if (b->LockBits(&rect, (unsigned int)Gdiplus::ImageLockModeWrite,
                    PixelFormat32bppARGB, &bdata) != Gdiplus::Ok)
    {
        msg::error("could not lock GDI+ image bits\n");
        delete b;
        delete[] wpath;
        delete[] codecs;
        return false;
    }

    u8vec4 *psrc = image->lock<PixelFormat::RGBA_8>();
    u8vec4 *psrc0 = psrc;
    u8vec4 *pdst = static_cast<u8vec4 *>(bdata.Scan0);
    for (int y = 0; y < size.y; y++)
        for (int x = 0; x < size.x; x++)
            *pdst++ = (*psrc++).bgra;
    image->unlock(psrc0);
    b->UnlockBits(&bdata);

    if (b->Save(wpath, &clsid, nullptr) != Gdiplus::Ok)
    {
        msg::error("could not save GDI+ image '%s'\n", path.c_str());
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

#endif /* defined LOL_USE_GDIPLUS */

