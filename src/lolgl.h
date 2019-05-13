//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The GL support
// --------------
//

#define GL_GLEXT_PROTOTYPES

/* Prefer GLES on browsers */
#if defined __EMSCRIPTEN__
#   undef HAVE_GL_2X
#   undef LOL_USE_GLEW
#endif

/* Only define one GL platform */
#if defined HAVE_GL_2X
#   undef HAVE_GLES_2X
#endif

/* Include GL development headers.
 * Do not include glew.h on OS X, because the version shipped with Fink
 * links with X11 whereas we want the system’s Cocoa-friendly GL libs. */
#if defined LOL_USE_GLEW && !defined __APPLE__
#   include <GL/glew.h>
#elif defined HAVE_NN_GLL_GLL_GL_H
#   include <nn/gll/gll_Gl.h>
#elif defined HAVE_GL_2X
#   if defined __APPLE__ && defined __MACH__ && defined __arm__
#       include <OpenGL/gl.h>
#   elif defined __APPLE__ && defined __MACH__
#       define MACOS_OPENGL
#       define GL_GLEXT_PROTOTYPES
#       include <OpenGL/OpenGL.h>
#       include <OpenGL/gl.h>
#       include <OpenGL/glext.h>
#   else
#       define GL_GLEXT_PROTOTYPES
#       include <GL/gl.h>
#       include <GL/glext.h>
#   endif
#elif defined HAVE_GLES_2X
#   if defined __APPLE__ && defined __MACH__
#       include <OpenGLES/ES2/gl.h>
#       include <OpenGLES/ES2/glext.h>
#   else
#       include <GLES2/gl2.h>
#       include <GLES2/gl2ext.h>
#   endif
#endif

#define LOL_STRINGIFY_INNER(n) #n
#define LOL_STRINGIFY(n) LOL_STRINGIFY_INNER(n)

#define LOL_CHECK_GLERROR() \
    { \
        GLenum error = glGetError(); \
        if (error != GL_NO_ERROR) \
            lol::gpu::error(__FILE__ ":" LOL_STRINGIFY(__LINE__), (int)error); \
    }

