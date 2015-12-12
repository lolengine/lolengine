//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The GL support
// --------------
//

#define GL_GLEXT_PROTOTYPES

/* Prefer GLES on browsers */
#if defined EMSCRIPTEN
#   undef HAVE_GL_2X
#endif

/* Only define one GL platform */
#if defined HAVE_GL_2X
#   undef HAVE_GLES_2X
#endif

/* Include GL development headers.
 * Do not include glew.h on OS X, because the version shipped with Fink
 * links with X11 whereas we want the system’s Cocoa-friendly GL libs. */
#if defined USE_GLEW && !defined __APPLE__
#   include <GL/glew.h>
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

#define LOL_CHECK_GLERROR() \
    { \
        GLenum error = glGetError(); \
        ASSERT(error == GL_NO_ERROR, "OpenGL error: 0x%04x\n", error); \
    }

