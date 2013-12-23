//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

package net.lolengine;

import android.app.NativeActivity; /* NativeActivity */
import android.os.Bundle; /* Bundle */

import android.content.res.AssetManager; /* getAssets() */
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

public class LolActivity extends NativeActivity
{
    static
    {
        /* Need to preload libstlport_shared.so somewhere; also need to
         * preload lib@PROGRAM@.so otherwise nativeInit() can’t be found. */
        System.loadLibrary("stlport_shared");
        System.loadLibrary("@PROGRAM@");
    }

    @Override
    protected void onCreate(Bundle saved_instance)
    {
        super.onCreate(saved_instance);
        m_assets = getAssets();
    }

    private AssetManager m_assets;

    /*
     * Bitmap loading helpers
     */

    public Bitmap openImage(String name)
    {
        try
        {
            return BitmapFactory.decodeStream(m_assets.open(name));
        }
        catch (Exception e) { }
        return null;
    }

    public int getWidth(Bitmap bmp) { return bmp.getWidth(); }
    public int getHeight(Bitmap bmp) { return bmp.getHeight(); }

    public void getPixels(Bitmap bmp, int[] pixels)
    {
        int w = bmp.getWidth();
        int h = bmp.getHeight();
        bmp.getPixels(pixels, 0, w, 0, 0, w, h);
    }

    public void closeImage(Bitmap bmp)
    {
        bmp.recycle();
    }
}

