# $Id$

AC_INIT(deushax, 0.0)
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

dnl AC_PROG_EGREP only exists in autoconf 2.54+, so we use AC_EGREP_CPP right
dnl now otherwise it might be set in an obscure if statement. Same thing for
dnl PKG_PROG_PKG_CONFIG which needs to be called first.
AC_EGREP_CPP(yes, foo)
PKG_PROG_PKG_CONFIG()
m4_pattern_allow([^PKG_CONFIG_LIBDIR$])
if test "${build}" != "${host}" -a "${PKG_CONFIG_LIBDIR}" = ""; then
  export PKG_CONFIG_LIBDIR=/dev/null
fi

dnl conditional builds
AC_ARG_ENABLE(debug,
  [  --enable-debug          build debug versions of the game (default no)])
AC_ARG_ENABLE(release,
  [  --enable-release        build final release of the game (default no)])

AC_CHECK_HEADERS(stdio.h stdarg.h inttypes.h endian.h stdint.h getopt.h)

# Optimizations
CXXFLAGS="${CXXFLAGS} -g -O2 -fno-strength-reduce -fomit-frame-pointer"
# Code qui fait des warnings == code de porc == deux baffes dans ta gueule
CXXFLAGS="${CXXFLAGS} -Wall -Wextra -Wpointer-arith -Wcast-align -Wcast-qual -Wshadow -Wsign-compare"

AC_CHECK_LIB(m, sin, MATH_LIBS="${MATH_LIBS} -lm")

# Use SDL?
ac_cv_my_have_sdl="no"
save_CPPFLAGS="${CPPFLAGS}"
AC_PATH_PROG(SDL_CONFIG, sdl-config, no)
if test "${SDL_CONFIG}" != "no"; then
  CPPFLAGS="${CPPFLAGS} `sdl-config --cflags`"
fi
AC_CHECK_HEADERS(SDL_image.h,
 [ac_cv_my_have_sdl="yes"],
 [ac_cv_my_have_sdl="no"])
CPPFLAGS="${save_CPPFLAGS}"
if test "${ac_cv_my_have_sdl}" != "no"; then
  AC_DEFINE(USE_SDL, 1, Define to 1 to use SDL_image)
fi
AM_CONDITIONAL(USE_SDL, test "${ac_cv_my_have_sdl}" = "yes")

if test "${ac_cv_my_have_sdl}" = "no" -a "${ac_cv_my_have_imlib2}" = "no"; then
  AC_MSG_ERROR([[cannot find SDL_Image or GTK+, please install one of them]])
fi

if test "${enable_debug}" = "yes"; then
  AC_DEFINE(DEBUG, 1, Define to 1 to activate debug)
fi

if test "${enable_release}" = "yes"; then
  AC_DEFINE(FINAL_RELEASE, 1, Define to 1 to activate final release)
fi

AC_SUBST(MATH_LIBS)

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  tools/Makefile
  art/Makefile
  art/test/Makefile
  gfx/Makefile
  gfx/font/Makefile
  maps/Makefile
])

AC_OUTPUT

