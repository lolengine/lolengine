/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2008 Sam Hocevar <sam@zoy.org>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/*
 * gdi.c: Windows GDI I/O functions (BMP only)
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#include "pipi.h"
#include "pipi-internals.h"

pipi_image_t *pipi_load_gdi(const char *name)
{
    BITMAPINFO binfo;
    pipi_image_t *img;
    pipi_pixels_t *p;
    uint8_t *data;
    HBITMAP hbmap;
    HDC hdc;

    hbmap = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if(!hbmap)
        return NULL;

    hdc = CreateCompatibleDC(NULL);
    SelectObject(hdc, hbmap);

    memset(&binfo, 0, sizeof(binfo));
    binfo.bmiHeader.biSize = sizeof(binfo.bmiHeader);
    if(!GetDIBits(hdc, hbmap, 0, 0, 0, &binfo, DIB_RGB_COLORS))
    {
        ReleaseDC(0, hdc);
        DeleteObject(hbmap);
        return NULL;
    }

    img = pipi_new(binfo.bmiHeader.biWidth, binfo.bmiHeader.biHeight);
    p = pipi_get_pixels(img, PIPI_PIXELS_RGBA_U8);
    data = p->pixels;

    binfo.bmiHeader.biBitCount = 32;
    binfo.bmiHeader.biCompression = BI_RGB;
    binfo.bmiHeader.biHeight = - abs(binfo.bmiHeader.biHeight);
    if(!GetDIBits(hdc, hbmap, 0, abs(binfo.bmiHeader.biHeight), data,
                  &binfo, DIB_RGB_COLORS))
    {
        ReleaseDC(0, hdc);
        DeleteObject(hbmap);
        return NULL;
    }

    /* FIXME: do we need to swap bytes? Apparently Vista doesn't need it,
     * but we'd need a more thorough test. */

    pipi_release_pixels(img, p);

    img->codec_priv = NULL;

    img->wrap = 0;
    img->u8 = 1;

    ReleaseDC(0, hdc);
    DeleteObject(hbmap);

    return img;
}

int pipi_save_gdi(pipi_image_t *img, const char *name)
{
    BITMAPINFOHEADER binfo;
    BITMAPFILEHEADER bfheader;
    uint8_t dummy[4] = { 0 };
    HANDLE hfile;
    pipi_pixels_t *p;
    DWORD ret = 0;
    uint8_t *data;
    int x, y, padding;

    p = pipi_get_pixels(img, PIPI_PIXELS_RGBA_U8);
    data = p->pixels;

    padding = ((img->w * 3) + 3) / 4 * 4 - img->w * 3;

    memset(&binfo, 0, sizeof(binfo));
    binfo.biSize = sizeof(binfo);
    binfo.biWidth = img->w;
    binfo.biHeight = img->h;
    binfo.biPlanes = 1;
    binfo.biBitCount = 24;
    binfo.biCompression = BI_RGB;
    binfo.biSizeImage = (img->w * 3 + padding) * img->h;

    memset(&bfheader, 0, sizeof(bfheader));
    bfheader.bfType = 0x4D42;
    bfheader.bfOffBits = sizeof(bfheader) + sizeof(binfo);
    bfheader.bfSize = bfheader.bfOffBits + binfo.biSizeImage;

    /* We don’t even create the bitmap object, since we know exactly
     * what kind of file we are saving. But later, when we support
     * different depths and BMP options, we'll need to care about it. */
    hfile = CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                       FILE_ATTRIBUTE_ARCHIVE, NULL);
    if(!hfile)
        return -1;
    WriteFile(hfile, &bfheader, sizeof(bfheader), &ret, NULL);
    WriteFile(hfile, &binfo, sizeof(binfo), &ret, NULL);
    for(y = 0; y < img->h; y++)
    {
        for(x = 0; x < img->w; x++)
        {
            uint8_t tmp[3];
            tmp[0] = data[4 * (img->w * (img->h - 1 - y) + x) + 0];
            tmp[1] = data[4 * (img->w * (img->h - 1 - y) + x) + 1];
            tmp[2] = data[4 * (img->w * (img->h - 1 - y) + x) + 2];
            WriteFile(hfile, tmp, 3, &ret, NULL);
        }
        if(padding)
            WriteFile(hfile, dummy, padding, &ret, NULL);
    }
    CloseHandle(hfile);

    pipi_release_pixels(img, p);

    return 0;
}

/*
 * XXX: The following functions are local.
 */

