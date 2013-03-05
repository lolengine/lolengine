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

DECLARE_IMAGE_LOADER(GdiPlusImageData, 100)
{
public:
    virtual bool Open(char const *);
    virtual bool Close();

    virtual uint8_t *GetData() const;

private:
    Gdiplus::Bitmap *m_bitmap;
    Gdiplus::BitmapData m_bdata;
};

/*
 * Public Image class
 */

bool GdiPlusImageData::Open(char const *path)
{
    Gdiplus::Status status;
    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;
    status = Gdiplus::GdiplusStartup(&token, &input, nullptr);
    if (status != Gdiplus::Ok)
    {
#if !LOL_RELEASE
        Log::Error("error %d while initialising GDI+\n", status);
#endif
        return false;
    }

    Array<String> pathlist = System::GetPathList(path);
    m_bitmap = nullptr;
    for (int i = 0; i < pathlist.Count(); ++i)
    {
        size_t len;
        len = mbstowcs(nullptr, pathlist[i].C(), 0);
        wchar_t *wpath = new wchar_t[len + 1];
        if (mbstowcs(wpath, pathlist[i].C(), len + 1) == (size_t)-1)
        {
#if !LOL_RELEASE
            Log::Error("invalid image name %s\n", pathlist[i].C());
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
#if !LOL_RELEASE
                if (status != Gdiplus::InvalidParameter)
                    Log::Error("error %d loading %s\n",
                               status, pathlist[i].C());
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
#if !LOL_RELEASE
        Log::Error("could not load %s\n", path);
#endif
        return false;
    }

    m_size = ivec2(m_bitmap->GetWidth(), m_bitmap->GetHeight());
    m_format = PixelFormat::RGBA_8;

    Gdiplus::Rect rect(0, 0, m_size.x, m_size.y);
    if(m_bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead,
                          PixelFormat32bppARGB, &m_bdata) != Gdiplus::Ok)
    {
#if !LOL_RELEASE
        Log::Error("could not lock bits in %s\n", path);
#endif
        delete m_bitmap;
        return false;
    }

    /* FIXME: GDI+ doesn't know about RGBA, only ARGB. And OpenGL doesn't
     * know about ARGB, only RGBA. So we swap bytes. We could also fix
     * this in the shader. */
    uint8_t *p = static_cast<uint8_t *>(m_bdata.Scan0);
    for (int y = 0; y < m_size.y; y++)
        for (int x = 0; x < m_size.x; x++)
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
    m_bitmap->UnlockBits(&m_bdata);
    delete m_bitmap;

    return true;
}

uint8_t * GdiPlusImageData::GetData() const
{
    return static_cast<uint8_t *>(m_bdata.Scan0);
}

} /* namespace lol */

#endif /* defined USE_GDIPLUS */

