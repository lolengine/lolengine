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

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.AssetManager; /* getAssets() */
import android.content.res.Resources; /* getResources() */
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.view.Window;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

/* FIXME: this needs to have a different name for each project */
public class LolActivity extends Activity
{
    @Override protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                             WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                             WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        nativeInit();

        mView = new LolView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause()
    {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume()
    {
        super.onResume();
        mView.onResume();
    }

    private LolView mView;

    static
    {
        System.loadLibrary("stlport_shared");
        System.loadLibrary("@PROGRAM@");
    }

    public Bitmap openImage(String name)
    {
        try
        {
            return BitmapFactory.decodeStream(getAssets().open(name));
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

    private native void nativeInit();
}

class LolView extends GLSurfaceView
{
    public LolView(Context context)
    {
        super(context);
        setEGLContextFactory(new ContextFactory());
        //setEGLConfigChooser(new ConfigChooser(4, 4, 4, 0, 8, 0));
        setEGLConfigChooser(new ConfigChooser(4, 4, 4, 4, 8, 0));
        setRenderer(new LolRenderer());
    }

    private static class ContextFactory implements GLSurfaceView.EGLContextFactory
    {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public EGLContext createContext(EGL10 egl, EGLDisplay dpy, EGLConfig cfg)
        {
            Log.w("LOL", "creating OpenGL ES 2.0 context");
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            EGLContext ctx = egl.eglCreateContext(dpy, cfg, EGL10.EGL_NO_CONTEXT, attrib_list);
            return ctx;
        }

        public void destroyContext(EGL10 egl, EGLDisplay dpy, EGLContext ctx)
        {
            egl.eglDestroyContext(dpy, ctx);
        }
    }

    private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser
    {
        public ConfigChooser(int r, int g, int b, int a, int depth, int stencil)
        {
            mRed = r;
            mGreen = g;
            mBlue = b;
            mAlpha = a;
            mDepth = depth;
            mStencil = stencil;
        }

        private static int EGL_OPENGL_ES2_BIT = 4;
        private static int[] s_configAttribs2 =
        {
//            EGL10.EGL_BUFFER_SIZE, 16,
            EGL10.EGL_DEPTH_SIZE, 8,
            EGL10.EGL_RED_SIZE, 4,
            EGL10.EGL_GREEN_SIZE, 4,
            EGL10.EGL_BLUE_SIZE, 4,
//            EGL10.EGL_ALPHA_SIZE, 4,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL10.EGL_NONE
        };

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay dpy)
        {
            int[] num_config = new int[1];
            egl.eglChooseConfig(dpy, s_configAttribs2, null, 0, num_config);

            int n = num_config[0];
            if (n <= 0)
                throw new IllegalArgumentException("No GLES configs");

            EGLConfig[] configs = new EGLConfig[n];
            egl.eglChooseConfig(dpy, s_configAttribs2, configs, n, num_config);
            return choose(egl, dpy, configs);
        }

        public EGLConfig choose(EGL10 egl, EGLDisplay dpy, EGLConfig[] configs)
        {
            for(EGLConfig cfg : configs)
            {
                /* Do not complain if we get more bits than we asked. */
                if (get(egl, dpy, cfg, EGL10.EGL_STENCIL_SIZE, 0) >= mStencil
                 && get(egl, dpy, cfg, EGL10.EGL_DEPTH_SIZE, 0) >= mDepth
                 && get(egl, dpy, cfg, EGL10.EGL_RED_SIZE, 0) >= mRed
                 && get(egl, dpy, cfg, EGL10.EGL_GREEN_SIZE, 0) >= mGreen
                 && get(egl, dpy, cfg, EGL10.EGL_BLUE_SIZE, 0) >= mBlue
                 && get(egl, dpy, cfg, EGL10.EGL_ALPHA_SIZE, 0) >= mAlpha)
                    return cfg;
            }
            return null;
        }

        private int get(EGL10 egl, EGLDisplay dpy, EGLConfig cfg,
                        int attr, int defval)
        {
            int[] value = new int[1];

            if (egl.eglGetConfigAttrib(dpy, cfg, attr, value))
                return value[0];
            return defval;
        }

        protected int mRed, mGreen, mBlue, mAlpha, mDepth, mStencil;
    }

    public boolean onTouchEvent(final MotionEvent ev)
    {
        final int action = ev.getAction();
        switch (action)
        {
            case MotionEvent.ACTION_DOWN:
                nativeMove((int)ev.getX(), (int)ev.getY());
                nativeDown();
                break;
            case MotionEvent.ACTION_UP:
                nativeMove((int)ev.getX(), (int)ev.getY());
                nativeUp();
                break;
            case MotionEvent.ACTION_MOVE:
                nativeMove((int)ev.getX(), (int)ev.getY());
                break;
        }

        return true;
    }

    private static native void nativePause();
    private static native void nativeDown();
    private static native void nativeUp();
    private static native void nativeMove(int x, int y);
}

class LolRenderer implements GLSurfaceView.Renderer
{
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        nativeInit();
    }

    public void onSurfaceChanged(GL10 gl, int w, int h)
    {
        Log.w("LOL", String.format("resizing to %dx%d", w, h));
        nativeResize(w, h);
    }

    public void onDrawFrame(GL10 gl)
    {
        nativeRender();
    }

    private static native void nativeInit();
    private static native void nativeResize(int w, int h);
    private static native void nativeRender();
    private static native void nativeDone();
}

