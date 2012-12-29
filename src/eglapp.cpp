//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined USE_EGL && !defined __ANDROID__
#   if defined HAVE_BCM_HOST_H
#       include <bcm_host.h>
#   else
#       include <X11/Xlib.h>
#       include <X11/Xatom.h>
#       include <X11/Xutil.h>
#   endif
#   include <EGL/egl.h>
#   include <EGL/eglext.h>
#endif

#if defined USE_SDL
#   if defined HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#endif

#include "core.h"
#include "lolgl.h"
#include "eglapp.h"
#if defined USE_SDL
#   include "platform/sdl/sdlinput.h"
#endif

namespace lol
{

/*
 * EGL App implementation class
 */

class EglAppData
{
    friend class EglApp;

private:
#if defined USE_EGL && !defined __ANDROID__
    EGLDisplay egl_dpy;
    EGLContext egl_ctx;
    EGLSurface egl_surf;
    uvec2 screen_size;
#   if defined HAVE_BCM_HOST_H
    EGL_DISPMANX_WINDOW_T nativewindow;
#   else
    Display *dpy;
    Window win;
#   endif
#endif
};

/*
 * Public EglApp class
 */

EglApp::EglApp(char const *title, ivec2 res, float fps) :
    data(new EglAppData())
{
#if defined USE_EGL && !defined __ANDROID__
#   if defined HAVE_BCM_HOST_H
    bcm_host_init();

    data->egl_dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#   else
    data->dpy = XOpenDisplay(NULL);
    if (data->dpy == NULL)
    {
        Log::Error("cannot connect to X server\n");
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
#   endif
    if (data->egl_dpy == EGL_NO_DISPLAY)
    {
        Log::Error("cannot get EGL display\n");
        exit(EXIT_FAILURE);
    }

    if (!eglInitialize(data->egl_dpy, NULL, NULL))
    {
        Log::Error("cannot initialize EGL\n");
        exit(EXIT_FAILURE);
    }

    EGLint attr[] =
    {
        EGL_BUFFER_SIZE, 16,
        EGL_DEPTH_SIZE, 16,
        EGL_RED_SIZE, 4,
        EGL_GREEN_SIZE, 4,
        EGL_BLUE_SIZE, 4,
        EGL_ALPHA_SIZE, 4,
#if defined HAVE_GLES_2X
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
#endif
        EGL_NONE
    };

    EGLConfig ecfg;
    EGLint num_config;
    if (!eglChooseConfig(data->egl_dpy, attr, &ecfg, 1, &num_config))
    {
        Log::Error("cannot choose EGL config (%i)\n", eglGetError());
        exit(EXIT_FAILURE);
    }

    if (num_config != 1)
    {
        Log::Error("cannot choose between %i EGL configs\n", num_config);
        exit(EXIT_FAILURE);
    }

    if (!eglBindAPI(EGL_OPENGL_ES_API))
    {
        Log::Error("cannot bind OpenGL ES API (%i)\n", eglGetError());
        exit(EXIT_FAILURE);
    }

#   if defined HAVE_BCM_HOST_H
    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    graphics_get_display_size(0 /* LCD */, &data->screen_size.x, &data->screen_size.y);

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = data->screen_size.x;
    dst_rect.height = data->screen_size.y;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = data->screen_size.x << 16;
    src_rect.height = data->screen_size.y << 16;

    dispman_display = vc_dispmanx_display_open(0 /* LCD */);
    dispman_update = vc_dispmanx_update_start(0);

    dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display,
        0/*layer*/, &dst_rect, 0/*src*/, &src_rect, DISPMANX_PROTECTION_NONE,
        0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

    data->nativewindow.element = dispman_element;
    data->nativewindow.width = data->screen_size.x;
    data->nativewindow.height = data->screen_size.y;
    vc_dispmanx_update_submit_sync(dispman_update);

    data->egl_surf = eglCreateWindowSurface(data->egl_dpy, ecfg,
                                            &data->nativewindow, NULL);
#   else
    data->egl_surf = eglCreateWindowSurface(data->egl_dpy, ecfg,
                                            (EGLNativeWindowType)data->win,
                                            NULL);
#   endif
    if (data->egl_surf == EGL_NO_SURFACE)
    {
        switch (eglGetError())
        {
        case EGL_BAD_DISPLAY:
            Log::Error("missing EGL display connection\n");
            break;
        case EGL_NOT_INITIALIZED:
            Log::Error("EGL display not initialized\n");
            break;
        case EGL_BAD_CONFIG:
            Log::Error("invalid EGL configuration\n");
            break;
        case EGL_BAD_NATIVE_WINDOW:
            Log::Error("invalid EGL native window\n");
            break;
        case EGL_BAD_ATTRIBUTE:
            Log::Error("invalid EGL window attribute\n");
            break;
        case EGL_BAD_ALLOC:
            Log::Error("cannot allocate EGL surface\n");
            break;
        case EGL_BAD_MATCH:
            Log::Error("unsupported EGL window\n");
            break;
        default:
            Log::Error("cannot create EGL surface (%i)\n", eglGetError());
            break;
        }

        exit(EXIT_FAILURE);
    }

    EGLint ctxattr[] =
    {
#if defined HAVE_GLES_2X
        EGL_CONTEXT_CLIENT_VERSION, 2,
#endif
        EGL_NONE
    };
    data->egl_ctx = eglCreateContext(data->egl_dpy, ecfg,
                                     EGL_NO_CONTEXT, ctxattr);
    if (data->egl_ctx == EGL_NO_CONTEXT)
    {
        Log::Error("cannot create EGL context (%i)\n", eglGetError());
        exit(EXIT_FAILURE);
    }

    eglMakeCurrent(data->egl_dpy, data->egl_surf,
                   data->egl_surf, data->egl_ctx);

#   if !defined HAVE_BCM_HOST_H
    XWindowAttributes gwa;
    XGetWindowAttributes(data->dpy, data->win, &gwa);
    data->screen_size = ivec2(gwa.width, gwa.height);
#   endif

#   if defined USE_SDL
    new SdlInput();
#   endif

    /* Initialise everything */
    Ticker::Setup(fps);
    Video::Setup((ivec2)data->screen_size);
    Audio::Setup(2);
#endif
}

void EglApp::ShowPointer(bool show)
{
    /* FIXME: unimplemented (do we have a mouse pointer anyway? */
    (void)show;
}

void EglApp::Run()
{
    while (!Ticker::Finished())
    {
        /* Tick the renderer, show the frame and clamp to desired framerate. */
        Ticker::TickDraw();
#if defined USE_EGL && !defined __ANDROID__
        eglSwapBuffers(data->egl_dpy, data->egl_surf);
#endif
    }
}

EglApp::~EglApp()
{
#if defined USE_EGL && !defined __ANDROID__
    eglDestroyContext(data->egl_dpy, data->egl_ctx);
    eglDestroySurface(data->egl_dpy, data->egl_surf);
    eglTerminate(data->egl_dpy);
#   if defined HAVE_BCM_HOST_H
    /* FIXME */
#   else
    XDestroyWindow(data->dpy, data->win);
    XCloseDisplay(data->dpy);
#   endif
#endif

    delete data;
}

} /* namespace lol */

