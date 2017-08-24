dnl
dnl  Lol Engine
dnl
dnl  Copyright © 2010—2017 Sam Hocevar <sam@hocevar.net>
dnl
dnl  Lol Engine is free software. It comes without any warranty, to
dnl  the extent permitted by applicable law. You can redistribute it
dnl  and/or modify it under the terms of the Do What the Fuck You Want
dnl  to Public License, Version 2, as published by the WTFPL Task Force.
dnl  See http://www.wtfpl.net/ for more details.
dnl


# LOL_AC_CHECK_OPENGL()
# ---------------------
AC_DEFUN([LOL_AC_CHECK_OPENGL],
[
ac_cv_my_have_gl="no"
ac_cv_my_have_glew="no"
if test "${enable_gl}" != "no"; then
  LOL_AC_CHECK_OPENGL_INNER()
fi
AM_CONDITIONAL(LOL_USE_GL, test "${ac_cv_my_have_gl}" != "no")
AM_CONDITIONAL(LOL_USE_GLEW, test "${ac_cv_my_have_glew}" != "no")
])

# LOL_AC_CHECK_OPENGL_INNER()
# ---------------------------
AC_DEFUN([LOL_AC_CHECK_OPENGL_INNER],
[
dnl  Find which version of OpenGL to use
ac_cv_my_have_gl="no"
ac_cv_my_stop_looking_for_gl="no"

if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  LIBS_save="$LIBS"
  LIBS="$LIBS -Wl,-framework -Wl,OpenGL"
  AC_MSG_CHECKING(for -framework OpenGL)
  AC_TRY_LINK([], [],
   [AC_MSG_RESULT(yes)
    ac_cv_my_have_gl="yes"
    GL_LIBS="${GL_LIBS} -framework OpenGL"
    LOL_TRY_CXXFLAGS(-ObjC++, [CXXFLAGS="${CXXFLAGS} -ObjC++"])
    AC_DEFINE(HAVE_GL_2X, 1, Define to 1 if GL 2.x is available)],
   [AC_MSG_RESULT(no)])
  LIBS="$LIBS_save"
fi

if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  LIBS_save="$LIBS"
  LIBS="$LIBS -Wl,-framework -Wl,OpenGLES"
  AC_MSG_CHECKING(for -framework OpenGLES)
  AC_TRY_LINK([], [],
   [AC_MSG_RESULT(yes)
    ac_cv_my_have_gl="yes"
    GL_LIBS="${GL_LIBS} -framework OpenGLES"
    LOL_TRY_CXXFLAGS(-ObjC++, [CXXFLAGS="${CXXFLAGS} -ObjC++"])
    AC_DEFINE(HAVE_GLES_2X, 1, Define to 1 if GLES 2.x is available)],
   [AC_MSG_RESULT(no)])
  LIBS="$LIBS_save"
fi

dnl  Use the Raspberry Pi libraries?
if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  AC_CHECK_HEADERS(bcm_host.h,
   [AC_CHECK_LIB(vcos, main,
     [ac_cv_my_have_gl="yes"
      ac_cv_my_stop_looking_for_gl="yes"
      AC_DEFINE(HAVE_GLES_2X, 1, Define to 1 if GLES 2.x is available)
      dnl  FIXME: -lEGL does not belong here but the configure test fails
      dnl  when cross-compiling, so we add it manually here.
      GL_LIBS="${GL_LIBS} -lGLESv2 -lEGL -lvcos -lvchiq_arm -lbcm_host"])])
fi

if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  PKG_CHECK_MODULES(GLES2, glesv2,
   [ac_cv_my_have_gl="yes"
    AC_DEFINE(HAVE_GLES_2X, 1, Define to 1 if GLES 2.x is available)
    GL_CFLAGS="${GL_CFLAGS} ${GLES2_CFLAGS}"
    GL_LIBS="${GL_LIBS} ${GLES2_LIBS}"],
   [:])
fi

if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  AC_CHECK_HEADER(GLES2/gl2.h,
   [ac_cv_my_have_gl="yes"
    AC_DEFINE(HAVE_GLES_2X, 1, Define to 1 if GLES 2.x is available)
    AC_CHECK_LIB(GLESv2, glEnable,
     [GL_LIBS="${GL_LIBS} -lGLESv2"])])
fi

if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  PKG_CHECK_MODULES(GL, gl,
   [ac_cv_my_have_gl="yes"
    AC_DEFINE(HAVE_GL_2X, 1, Define to 1 if GL 2.x is available)],
   [:])
fi

if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  AC_CHECK_LIB(GL, glEnable,
   [ac_cv_my_have_gl="yes"
    AC_DEFINE(HAVE_GL_2X, 1, Define to 1 if GL 2.x is available) # FIXME: hackish
    GL_LIBS="-lGL"])
fi

if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  AC_CHECK_HEADER(GL/gl.h,
   [LIBS_save="$LIBS"
    LIBS="$LIBS -lopengl32"
    AC_MSG_CHECKING(for glLoadIdentity in -lopengl32)
    AC_TRY_LINK([#include <GL/gl.h>],
     [glLoadIdentity();],
     [ac_cv_my_have_gl="yes"
      AC_DEFINE(HAVE_GL_2X, 1, Define to 1 if GL 2.x is available) # FIXME: hack
      AC_MSG_RESULT(yes)
      GL_LIBS="-lopengl32"],
     [AC_MSG_RESULT(no)])
    LIBS="$LIBS_save"])
fi

dnl  Use Glew?
PKG_CHECK_MODULES(GLEW, glew,
 [ac_cv_my_have_glew="yes"
  GL_CFLAGS="${GLEW_CFLAGS} ${GL_CFLAGS}"
  GL_LIBS="${GLEW_LIBS} ${GL_LIBS}"],
 [:])
AC_CHECK_HEADER(GL/glew.h,
 [LIBS_save="${LIBS}"
  LIBS="${LIBS} -lglew32s ${GL_LIBS}"
  AC_MSG_CHECKING(for glewInit in -lglew32s)
  AC_TRY_LINK(
   [#include <GL/glew.h>],
   [glewInit();],
   [ac_cv_my_have_glew="yes"
    GL_LIBS="-lglew32s ${GL_LIBS}"
    AC_MSG_RESULT(yes)],
   [AC_MSG_RESULT(no)])
  LIBS="${LIBS_save}"])
if test "${ac_cv_my_have_glew}" != "no"; then
  AC_DEFINE(HAVE_GLES_2X, 1, Define to 1 if GLES 2.x is available)
  AC_DEFINE(LOL_USE_GLEW, 1, Define to 1 to use libglew)
fi

dnl  Poor man's GL feature detection if all else failed.
save_LIBS="${LIBS}"
LIBS="${LIBS} ${GL_LIBS} ${GLES2_LIBS}"
AC_CHECK_FUNCS(glBegin)
LIBS="${save_LIBS}"

dnl  Warn if we couldn't find an OpenGL-like API
if test "${ac_cv_my_have_gl}" = "no"; then
  AC_MSG_WARN([[No OpenGL or OpenGL ES implementation found]])
fi

]) # LOL_AC_CHECK_OPENGL_INNER

