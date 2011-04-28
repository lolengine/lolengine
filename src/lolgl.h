//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The GL support
// --------------
//

#if !defined __LOL_LOLGL_H__
#define __LOL_LOLGL_H__

#define GL_GLEXT_PROTOTYPES

/* Only define one GL platform */
#if defined HAVE_GL_2X
#   undef HAVE_GLES_2X
#endif

/* Include GL */
#if defined HAVE_GL_2X
#   if defined __APPLE__ && defined __MACH__
#       include <OpenGL/gl.h>
#   else
#       define GL_GLEXT_PROTOTYPES
#       include <GL/gl.h>
#   endif
#elif defined HAVE_GLES_2X
#   if defined __APPLE__ && defined __MACH__
#       include <OpenGLES/ES2/gl.h>
#       include <OpenGLES/ES2/glext.h>
#   elif defined __CELLOS_LV2__
#       include <PSGL/psgl.h>
#       include <PSGL/psglu.h>
#   else
#       include <GLES2/gl2.h>
#       include <GLES2/gl2ext.h>
#   endif
#endif

/* Redefine some function names */
#if defined HAVE_GL_2X
#   define glClearDepthf glClearDepth
#elif defined HAVE_GLES_2X
#   define glGenVertexArrays glGenVertexArraysOES
#   define glDeleteVertexArrays glDeleteVertexArraysOES
#   define glBindVertexArray glBindVertexArrayOES
#endif

#endif // __LOL_LOLGL_H__

