//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>
#include <lol/msg>

#if LOL_USE_SDL
#   include "ui/sdl-input.h"
#endif

#include "lolgl.h"
#include "application/egl-app.h"

namespace lol
{

//
// Public app class
//

egl::app::app(char const *title, ivec2 res)
{
#if defined LOL_USE_EGL && !defined __ANDROID__
#   if defined HAVE_BCM_HOST_H
    bcm_host_init();

    m_egl_dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#   else
    m_dpy = XOpenDisplay(nullptr);
    if (m_dpy == nullptr)
    {
        msg::error("cannot connect to X server\n");
        exit(EXIT_FAILURE);
    }

    Window root = DefaultRootWindow(m_dpy);

    XSetWindowAttributes swa;
    swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;

    m_win = XCreateWindow(m_dpy, root, 0, 0, res.x, res.y, 0,
                          CopyFromParent, InputOutput,
                          CopyFromParent, CWEventMask, &swa);

    XSetWindowAttributes xattr;

    xattr.override_redirect = False;
    XChangeWindowAttributes(m_dpy, m_win, CWOverrideRedirect, &xattr);

    XWMHints hints;
    hints.flags = InputHint;
    hints.input = True;
    XSetWMHints(m_dpy, m_win, &hints);

    XMapWindow(m_dpy, m_win);
    XStoreName(m_dpy, m_win, title);

    m_egl_dpy = eglGetDisplay((EGLNativeDisplayType)m_dpy);
#   endif
    if (m_egl_dpy == EGL_NO_DISPLAY)
    {
        msg::error("cannot get EGL display\n");
        exit(EXIT_FAILURE);
    }

    if (!eglInitialize(m_egl_dpy, nullptr, nullptr))
    {
        msg::error("cannot initialize EGL\n");
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
    if (!eglChooseConfig(m_egl_dpy, attr, &ecfg, 1, &num_config))
    {
        msg::error("cannot choose EGL config (%i)\n", eglGetError());
        exit(EXIT_FAILURE);
    }

    if (num_config != 1)
    {
        msg::error("cannot choose between %i EGL configs\n", num_config);
        exit(EXIT_FAILURE);
    }

    if (!eglBindAPI(EGL_OPENGL_ES_API))
    {
        msg::error("cannot bind OpenGL ES API (%i)\n", eglGetError());
        exit(EXIT_FAILURE);
    }

#   if defined HAVE_BCM_HOST_H
    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    graphics_get_display_size(0 /* LCD */, &m_screen_size.x, &m_screen_size.y);

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = m_screen_size.x;
    dst_rect.height = m_screen_size.y;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = m_screen_size.x << 16;
    src_rect.height = m_screen_size.y << 16;

    dispman_display = vc_dispmanx_display_open(0 /* LCD */);
    dispman_update = vc_dispmanx_update_start(0);

    dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display,
        0/*layer*/, &dst_rect, 0/*src*/, &src_rect, DISPMANX_PROTECTION_NONE,
        0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

    m_nativewindow.element = dispman_element;
    m_nativewindow.width = m_screen_size.x;
    m_nativewindow.height = m_screen_size.y;
    vc_dispmanx_update_submit_sync(dispman_update);

    m_egl_surf = eglCreateWindowSurface(m_egl_dpy, ecfg,
                                        &m_nativewindow, nullptr);
#   else
    m_egl_surf = eglCreateWindowSurface(m_egl_dpy, ecfg,
                                        (EGLNativeWindowType)m_win,
                                        nullptr);
#   endif
    if (m_egl_surf == EGL_NO_SURFACE)
    {
        switch (eglGetError())
        {
        case EGL_BAD_DISPLAY:
            msg::error("missing EGL display connection\n");
            break;
        case EGL_NOT_INITIALIZED:
            msg::error("EGL display not initialized\n");
            break;
        case EGL_BAD_CONFIG:
            msg::error("invalid EGL configuration\n");
            break;
        case EGL_BAD_NATIVE_WINDOW:
            msg::error("invalid EGL native window\n");
            break;
        case EGL_BAD_ATTRIBUTE:
            msg::error("invalid EGL window attribute\n");
            break;
        case EGL_BAD_ALLOC:
            msg::error("cannot allocate EGL surface\n");
            break;
        case EGL_BAD_MATCH:
            msg::error("unsupported EGL window\n");
            break;
        default:
            msg::error("cannot create EGL surface (%i)\n", eglGetError());
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
    m_egl_ctx = eglCreateContext(m_egl_dpy, ecfg,
                                 EGL_NO_CONTEXT, ctxattr);
    if (m_egl_ctx == EGL_NO_CONTEXT)
    {
        msg::error("cannot create EGL context (%i)\n", eglGetError());
        exit(EXIT_FAILURE);
    }

    eglMakeCurrent(m_egl_dpy, m_egl_surf,
                   m_egl_surf, m_egl_ctx);

#   if !defined HAVE_BCM_HOST_H
    XWindowAttributes gwa;
    XGetWindowAttributes(m_dpy, m_win, &gwa);
    m_screen_size = uvec2(gwa.width, gwa.height);
#   endif

#   if LOL_USE_SDL
    new SdlInput(m_screen_size.x, m_screen_size.y);
#   endif

    video::init((ivec2)m_screen_size);
    audio::init();
#else
    (void)title;
    (void)res;
#endif
}

egl::app::~app()
{
#if defined LOL_USE_EGL && !defined __ANDROID__
    eglDestroyContext(m_egl_dpy, m_egl_ctx);
    eglDestroySurface(m_egl_dpy, m_egl_surf);
    eglTerminate(m_egl_dpy);
#   if defined HAVE_BCM_HOST_H
    /* FIXME */
#   else
    XDestroyWindow(m_dpy, m_win);
    XCloseDisplay(m_dpy);
#   endif
#endif
}

std::shared_ptr<app::display> egl::app::get_display()
{
    return shared_from_this();
}

void egl::app::show_pointer(bool show)
{
    /* FIXME: unimplemented (do we have a mouse pointer anyway? */
    (void)show;
}

void egl::app::tick()
{
    /* Tick the renderer, show the frame and clamp to desired framerate. */
    ticker::tick_draw();
#if defined LOL_USE_EGL && !defined __ANDROID__
    eglSwapBuffers(m_egl_dpy, m_egl_surf);
#endif
}

} // namespace lol

