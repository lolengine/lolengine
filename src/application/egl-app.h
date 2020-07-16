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

#pragma once

//
// The EglApp class
// ----------------
//

#include <lol/vector>

#if defined LOL_USE_EGL && !defined __ANDROID__
#   if defined HAVE_BCM_HOST_H
#       include <bcm_host.h>
#   else
#       include <X11/Xlib.h>
#       include <X11/Xatom.h>
#       include <X11/Xutil.h>
#   endif
#   if defined HAVE_EGL_EGL_H
#       include <EGL/egl.h>
#       include <EGL/eglext.h>
#   endif
#endif

#include "application/application.h"

namespace lol::egl
{

// Application is its own display
class app : public lol::app, public lol::app::display, std::enable_shared_from_this<app>
{
public:
    app(char const *title, ivec2 res);
    virtual ~app();

    virtual void tick();
    virtual void show_pointer(bool show);
    virtual std::shared_ptr<display> get_display();

#if defined LOL_USE_EGL && !defined __ANDROID__
private:
    EGLDisplay m_egl_dpy;
    EGLContext m_egl_ctx;
    EGLSurface m_egl_surf;
    uvec2 m_screen_size;
#   if defined HAVE_BCM_HOST_H
    EGL_DISPMANX_WINDOW_T m_nativewindow;
#   else
    Display *m_dpy;
    Window m_win;
#   endif
#endif
};

} // namespace lol
