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

#include <cstdio>

#if defined USE_EGL
#   include <X11/Xlib.h>
#   include <X11/Xatom.h>
#   include <X11/Xutil.h>
#   include <EGL/egl.h>
#endif

#include "core.h"
#include "lolgl.h"
#include "eglapp.h"

namespace lol
{

/*
 * EGL App implementation class
 */

class EglAppData
{
    friend class EglApp;

private:
#if defined USE_EGL
    Display *dpy;
    Window win;
    EGLDisplay egl_dpy;
    EGLContext egl_ctx;
    EGLSurface egl_surf;
#endif
};

/*
 * Public EglApp class
 */

EglApp::EglApp(char const *title, vec2i res, float fps) :
    data(new EglAppData())
{
#if defined USE_EGL
    data->dpy = XOpenDisplay(NULL);
    if (data->dpy == NULL)
    {
        fprintf(stderr, "Cannot connect to X server\n");
        exit(EXIT_FAILURE);
    }

    Window root = DefaultRootWindow(data->dpy);

    XSetWindowAttributes swa;
    swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;

    data->win = XCreateWindow(data->dpy, root, 0, 0, res.x, res.y, 0,
                              CopyFromParent, InputOutput,
                              CopyFromParent, CWEventMask, &swa);

    XSetWindowAttributes xattr;

    xattr.override_redirect = False;
    XChangeWindowAttributes(data->dpy, data->win, CWOverrideRedirect, &xattr);

    XWMHints hints;
    hints.flags = InputHint;
    hints.input = True;
    XSetWMHints(data->dpy, data->win, &hints);

    XMapWindow(data->dpy, data->win);
    XStoreName(data->dpy, data->win, title);

    data->egl_dpy = eglGetDisplay((EGLNativeDisplayType)data->dpy);
    if (data->egl_dpy == EGL_NO_DISPLAY)
    {
        fprintf(stderr, "Cannot get EGL display\n");
        exit(EXIT_FAILURE);
    }

    if (!eglInitialize(data->egl_dpy, NULL, NULL))
    {
        fprintf(stderr, "Cannot initialize EGL\n");
        exit(EXIT_FAILURE);
    }

    EGLint attr[] =
    {
        EGL_BUFFER_SIZE, 16,
        EGL_RENDERABLE_TYPE,
#if defined HAVE_GLES_1X
        EGL_OPENGL_ES_BIT,
#elif defined HAVE_GLES_2X
        EGL_OPENGL_ES2_BIT,
#endif
        EGL_NONE
    };

    EGLConfig ecfg;
    EGLint num_config;
    if (!eglChooseConfig(data->egl_dpy, attr, &ecfg, 1, &num_config))
    {
        fprintf(stderr, "Cannot choose EGL config (%i)\n", eglGetError());
        exit(EXIT_FAILURE);
    }

    if (num_config != 1)
    {
        fprintf(stderr, "Cannot choose between %i EGL configs\n", num_config);
        exit(EXIT_FAILURE);
    }

    data->egl_surf = eglCreateWindowSurface(data->egl_dpy, ecfg,
                                            data->win, NULL);
    if (data->egl_surf == EGL_NO_SURFACE)
    {
        fprintf(stderr, "Cannot create EGL surface (%i)\n", eglGetError());
        exit(EXIT_FAILURE);
    }

    EGLint ctxattr[] =
    {
#if defined HAVE_GLES_1X
        EGL_CONTEXT_CLIENT_VERSION, 1,
#elif defined HAVE_GLES_2X
        EGL_CONTEXT_CLIENT_VERSION, 2,
#endif
        EGL_NONE
    };
    data->egl_ctx = eglCreateContext(data->egl_dpy, ecfg,
                                     EGL_NO_CONTEXT, ctxattr);
    if (data->egl_ctx == EGL_NO_CONTEXT)
    {
        fprintf(stderr, "Cannot create EGL context (%i)\n", eglGetError());
        exit(EXIT_FAILURE);
    }

    eglMakeCurrent(data->egl_dpy, data->egl_surf,
                   data->egl_surf, data->egl_ctx);

    XWindowAttributes gwa;
    XGetWindowAttributes(data->dpy, data->win, &gwa);

    /* Initialise everything */
    Ticker::Setup(fps);
    Video::Setup(gwa.width, gwa.height);
    Audio::Setup(2);
#endif
}

void EglApp::Run()
{
    while (!Ticker::Finished())
    {
        /* Tick the game */
        Ticker::TickGame();

        /* Tick the renderer, show the frame and clamp to desired framerate. */
        Ticker::TickDraw();
#if defined USE_EGL
        eglSwapBuffers(data->egl_dpy, data->egl_surf);
#endif

        Ticker::ClampFps();
    }
}

EglApp::~EglApp()
{
#if defined USE_EGL
    eglDestroyContext(data->egl_dpy, data->egl_ctx);
    eglDestroySurface(data->egl_dpy, data->egl_surf);
    eglTerminate(data->egl_dpy);
    XDestroyWindow(data->dpy, data->win);
    XCloseDisplay(data->dpy);
#endif

    delete data;
}

} /* namespace lol */

