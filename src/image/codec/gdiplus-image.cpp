//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
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

DECLARE_IMAGE_LOADER(GdiPlusImageData, 100)
{
public:
    virtual bool Open(char const *);
    virtual bool Close();

    virtual void *GetData() const;

private:
    Gdiplus::Bitmap *bitmap;
    Gdiplus::BitmapData bdata;
};

/*
 * Public Image class
 */

bool GdiPlusImageData::Open(char const *path)
{
    Gdiplus::Status status;
    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;
    status = Gdiplus::GdiplusStartup(&token, &input, NULL);
    if (status != Gdiplus::Ok)
    {
#if !LOL_RELEASE
        Log::Error("error %d while initialising GDI+\n", status);
#endif
        return false;
    }

    size_t len;
    len = mbstowcs(NULL, path, 0);
    wchar_t *wpath = new wchar_t[len + 1];
    if (mbstowcs(wpath, path, len + 1) == (size_t)-1)
    {
#if !LOL_RELEASE
        Log::Error("invalid image name %s\n", path);
#endif
        delete[] wpath;
        return false;
    }

    bitmap = NULL;
    status = Gdiplus::Ok;
    for (wchar_t const *wname = wpath; *wname; wname++)
    {
        bitmap = Gdiplus::Bitmap::FromFile(wname, 0);
        if (bitmap)
        {
            status = bitmap->GetLastStatus();
            if (status == Gdiplus::Ok)
                break;
#if !LOL_RELEASE
            if (status != Gdiplus::InvalidParameter)
                Log::Error("error %d loading %s\n", status, path);
#endif
            delete bitmap;
        }
    }

    delete[] wpath;
    if (!bitmap)
    {
#if !LOL_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        return false;
    }

    size = ivec2(bitmap->GetWidth(), bitmap->GetHeight());
    format = Image::FORMAT_RGBA;

    Gdiplus::Rect rect(0, 0, size.x, size.y);
    if(bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead,
                        PixelFormat32bppARGB, &bdata) != Gdiplus::Ok)
    {
#if !LOL_RELEASE
        Log::Error("could not lock bits in %s\n", path);
#endif
        delete bitmap;
        return false;
    }

    /* FIXME: GDI+ doesn't know about RGBA, only ARGB. And OpenGL doesn't
     * know about ARGB, only RGBA. So we swap bytes. We could also fix
     * this in the shader. */
    uint8_t *p = static_cast<uint8_t *>(bdata.Scan0);
    for (int y = 0; y < size.y; y++)
        for (int x = 0; x < size.x; x++)
        {
            uint8_t tmp = p[2];
            p[2] = p[0];
            p[0] = tmp;
            p += 4;
        }

    return true;
}

bool GdiPlusImageData::Close()
{
    bitmap->UnlockBits(&bdata);
    delete bitmap;

    return true;
}

void * GdiPlusImageData::GetData() const
{
    return bdata.Scan0;
}

} /* namespace lol */

#endif /* defined USE_GDIPLUS */

