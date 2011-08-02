dnl configure script for Lol Engine

AC_INIT(lolengine, 0.0)
AC_PREREQ(2.50)
AC_CONFIG_AUX_DIR(.auto)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([no-define tar-ustar])
dnl AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_RANLIB

AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL
AC_LIBTOOL_CXX

AC_C_CONST
AC_C_INLINE

dnl Ensure an error is thrown if pkg-config M4 files are not found.
m4_pattern_allow([^PKG_CONFIG_LIBDIR$])
m4_pattern_forbid([^PKG_CHECK_MODULES$])
m4_pattern_forbid([^PKG_PROG_PKG_CONFIG$])

dnl Do not use PKG_CONFIG_LIBDIR when cross-compiling.
if test "${build}" != "${host}" -a "${PKG_CONFIG_LIBDIR}" = ""; then
  export PKG_CONFIG_LIBDIR=/dev/null
fi

dnl AC_PROG_EGREP only exists in autoconf 2.54+, so we use AC_EGREP_CPP right
dnl now otherwise it might be set in an obscure if statement. Same thing for
dnl PKG_PROG_PKG_CONFIG which needs to be called first.
AC_EGREP_CPP(yes, foo)
PKG_PROG_PKG_CONFIG()

dnl conditional builds
AC_ARG_ENABLE(debug,
  [  --enable-debug          build debug versions of the game (default no)])
AC_ARG_ENABLE(release,
  [  --enable-release        build final release of the game (default no)])
AC_ARG_ENABLE(experimental,
  [  --enable-experimental   experimental build (default no)])

AC_ARG_ENABLE(cppunit,
  [  --enable-cppunit        use cppunit for unit tests (autodetected)])

AC_CHECK_HEADERS(stdio.h stdarg.h inttypes.h endian.h stdint.h getopt.h)

if test "${enable_debug}" = "yes"; then
  AC_DEFINE(LOL_DEBUG, 1, Define to 1 to activate debug)
  OPT="-O"
else
  OPT="-O2 -fno-strength-reduce -fomit-frame-pointer"
fi

if test "${enable_release}" = "yes"; then
  AC_DEFINE(LOL_RELEASE, 1, Define to 1 to activate final release)
  REL=""
else
  REL="-g"
fi

if test "${enable_experimental}" = "yes"; then
  AC_DEFINE(LOL_EXPERIMENTAL, 1, Define to 1 to activate experimental build)
fi

dnl Optimizations
CXXFLAGS="${CXXFLAGS} ${REL} ${OPT}"
dnl Code qui fait des warnings == code de porc == deux baffes dans ta gueule
CXXFLAGS="${CXXFLAGS} -Wall -Wextra -Wpointer-arith -Wcast-align -Wcast-qual -Wshadow -Wsign-compare"

AC_CHECK_LIB(m, sin, MATH_LIBS="${MATH_LIBS} -lm")

dnl Which version of OpenGL to use?
ac_cv_my_have_gl="no"
PKG_CHECK_MODULES(GLES2, glesv2,
 [ac_cv_my_have_gl="yes"
  AC_DEFINE(HAVE_GLES_2X, 1, Define to 1 if GLES 2.x is available)
  GL_CFLAGS="${GLES2_CFLAGS}"
  GL_LIBS="${GLES2_LIBS}"],
 [:])
AC_CHECK_LIB(GL, glLoadIdentity,
 [ac_cv_my_have_gl="yes"
  AC_DEFINE(HAVE_GL_2X, 1, Define to 1 if GL 2.x is available) # FIXME: hackish
  GL_LIBS="-lGL"])
AC_CHECK_HEADER(GL/gl.h,
 [LIBS_save="$LIBS"
  LIBS="$LIBS -lopengl32"
  AC_MSG_CHECKING(for glLoadIdentity in -lopengl32)
  AC_TRY_LINK([#include <GL/gl.h>],
   [glLoadIdentity();],
   [ac_cv_my_have_gl="yes"
    AC_DEFINE(HAVE_GL_2X, 1, Define to 1 if GL 2.x is available) # FIXME: hacky
    AC_MSG_RESULT("yes")
    GL_LIBS="-lopengl32"],
   [AC_MSG_RESULT("no")])
  LIBS="$LIBS_save"])
if test "${ac_cv_my_have_gl}" = "no"; then
  AC_MSG_ERROR([[No OpenGL or OpenGL ES implementation found]])
fi

dnl Poor man's GL feature detection.
save_LIBS="${LIBS}"
LIBS="${LIBS} ${GL_LIBS} ${GLES2_LIBS}"
AC_CHECK_FUNCS(glBegin)
LIBS="${save_LIBS}"

dnl Use SDL? (always required)
ac_cv_my_have_sdl="yes"
AC_PATH_PROG(SDL_CONFIG, sdl-config, no)
if test "${SDL_CONFIG}" != "no"; then
  SDL_CFLAGS="${SDL_CFLAGS} `sdl-config --cflags`"
  SDL_LIBS="${SDL_LIBS} `sdl-config --libs`"
fi
SDL_CFLAGS="${SDL_CFLAGS} ${GL_CFLAGS}"
SDL_LIBS="${SDL_LIBS} ${GL_LIBS}"
PKG_CHECK_MODULES(SDL, sdl, [:],
 [SDL_LIBS="${SDL_LIBS} -lSDL"])
PKG_CHECK_MODULES(SDLMIXER, SDL_mixer, [:],
 [SDLMIXER_LIBS="${SDLMIXER_LIBS} -lSDL_mixer"])
PKG_CHECK_MODULES(SDLIMAGE, SDL_image, [:],
 [SDLIMAGE_LIBS="${SDLIMAGE_LIBS} -lSDL_image"])
SDL_CFLAGS="${SDL_CFLAGS} ${SDLMIXER_CFLAGS} ${SDLIMAGE_CFLAGS}"
SDL_LIBS="${SDL_LIBS} ${SDLMIXER_LIBS} ${SDLIMAGE_LIBS}"

save_CPPFLAGS="${CPPFLAGS}"
CPPFLAGS="${CPPFLAGS} ${SDL_CFLAGS}"
AC_CHECK_HEADERS(SDL_mixer.h, [:], [ac_cv_my_have_sdl="no"])
AC_CHECK_HEADERS(SDL_image.h, [:], [ac_cv_my_have_sdl="no"])
CPPFLAGS="${save_CPPFLAGS}"
if test "${ac_cv_my_have_sdl}" = "no"; then
  AC_MSG_ERROR([[One of SDL, SDL_Image or SDL_Mixer not found]])
else
  AC_DEFINE(USE_SDL, 1, Define to 1 to use SDL)
fi
AM_CONDITIONAL(USE_SDL, test "${ac_cv_my_have_sdl}" = "yes")


dnl Use EGL?
ac_cv_my_have_egl="no"
PKG_CHECK_MODULES(EGL, egl, [ac_cv_my_have_egl="yes"], [:])
if test "${ac_cv_my_have_egl}" != "no"; then
  AC_DEFINE(USE_EGL, 1, Define to 1 to use libegl)
  EGL_LIBS="${EGL_LIBS} -lX11"
fi
AM_CONDITIONAL(USE_EGL, test "${ac_cv_my_have_egl}" != "no")


dnl Use libpng? (replacement for SDL_image)
ac_cv_my_have_libpng="no"
PKG_CHECK_MODULES(LIBPNG, libpng, [ac_cv_my_have_libpng="yes"], [:])
if test "${ac_cv_my_have_libpng}" != "no"; then
  AC_DEFINE(USE_LIBPNG, 1, Define to 1 to use libpng)
fi
AM_CONDITIONAL(USE_LIBPNG, test "${ac_cv_my_have_libpng}" != "no")


dnl Use libcaca? (required for font generation)
ac_cv_my_have_caca="no"
PKG_CHECK_MODULES(CACA, caca >= 0.99.beta17, [ac_cv_my_have_caca="yes"], [:])
if test "${ac_cv_my_have_caca}" != "no"; then
  AC_DEFINE(USE_CACA, 1, Define to 1 to use libcaca)
fi
AM_CONDITIONAL(USE_CACA, test "${ac_cv_my_have_caca}" != "no")


dnl Use libpipi? (required for video recording)
ac_cv_my_have_pipi="no"
PKG_CHECK_MODULES(PIPI, pipi, [ac_cv_my_have_pipi="yes"], [:])
if test "${ac_cv_my_have_pipi}" != "no"; then
  AC_DEFINE(USE_PIPI, 1, Define to 1 to use libpipi)
fi
AM_CONDITIONAL(USE_PIPI, test "${ac_cv_my_have_pipi}" != "no")


dnl Use GTK+? (required for the deushax editor)
ac_cv_my_have_gtkgl="no"
PKG_CHECK_MODULES(GTK, gtk+-2.0, [ac_cv_my_have_gtkgl="yes"], [:])
PKG_CHECK_MODULES(GTKGL, gtkgl-2.0, [:], [ac_cv_my_have_gtkgl="no"])
if test "${ac_cv_my_have_gtkgl}" != "no"; then
  AC_DEFINE(USE_GTKGL, 1, Define to 1 to use GtkGl)
fi
AM_CONDITIONAL(USE_GTKGL, test "${ac_cv_my_have_gtkgl}" != "no")


dnl Use cppunit for unit tests?
PKG_CHECK_MODULES(CPPUNIT, cppunit, [CPPUNIT="yes"], [CPPUNIT="no"])
AM_CONDITIONAL(USE_CPPUNIT, test "$CPPUNIT" = "yes")


dnl How to use the Lol Engine inside this tree
LOL_CFLAGS="$LOL_CFLAGS -I \$(top_srcdir)/src $SDL_CFLAGS $EGL_CFLAGS $LIBPNG_CFLAGS"
LOL_LIBS="$LOL_LIBS $SDL_LIBS $EGL_LIBS $LIBPNG_LIBS"

AC_SUBST(MATH_LIBS)
AC_SUBST(LOL_CFLAGS)
AC_SUBST(LOL_LIBS)

AC_CONFIG_FILES(
 [Makefile
  src/Makefile
  test/Makefile
  gfx/Makefile
  gfx/font/Makefile
  tools/Makefile
])
AC_CONFIG_FILES(
 [monsterz/Makefile
])
AC_CONFIG_FILES(
 [deushax/Makefile
  deushax/art/Makefile
  deushax/art/test/Makefile
  deushax/gfx/Makefile
  deushax/maps/Makefile
])

AC_OUTPUT

