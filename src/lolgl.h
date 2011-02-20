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

#if !defined __DH_LOLGL_H__
#define __DH_LOLGL_H__

#if defined HAVE_GL_1X
#   if defined __APPLE__ && defined __MACH__
#       include <OpenGL/gl.h>
#   else
#       define GL_GLEXT_PROTOTYPES
#       include <GL/gl.h>
#   endif
#elif defined HAVE_GLES_1X
#   include <GLES/gl.h>
#elif defined HAVE_GLES_2X
#   include <GLES2/gl.h>
#endif

#endif // __DH_LOLGL_H__

