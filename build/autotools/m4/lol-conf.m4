dnl
dnl  Lol Engine
dnl
dnl  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
dnl
dnl  Lol Engine is free software. It comes without any warranty, to
dnl  the extent permitted by applicable law. You can redistribute it
dnl  and/or modify it under the terms of the Do What the Fuck You Want
dnl  to Public License, Version 2, as published by the WTFPL Task Force.
dnl  See http://www.wtfpl.net/ for more details.
dnl


# LOL_AC_INIT()
# ---------------------
AC_DEFUN([LOL_AC_INIT], [

dnl  C++11 and later mode. Checked early so that we don't run into surprises.
version_flag=''
LOL_TRY_CXXFLAGS(-std=c++0x, [version_flag='-std=c++0x'])
LOL_TRY_CXXFLAGS(-std=c++11, [version_flag='-std=c++11'])
LOL_TRY_CXXFLAGS(-std=c++14, [version_flag='-std=c++14'])
LOL_TRY_CXXFLAGS(-std=c++17, [version_flag='-std=c++17'])
AM_CXXFLAGS="${AM_CXXFLAGS} ${version_flag}"
CXXFLAGS="${CXXFLAGS} ${version_flag}"

AC_LANG_PUSH(C++)
AC_MSG_CHECKING(for C++11 enum class support)
AC_TRY_LINK([], [enum class Foo : int { Bar, Baz };],
 [AC_MSG_RESULT(yes)],
 [AC_MSG_RESULT(no)
  AC_MSG_ERROR([[C++ compiler does not support C++11 enum classes]])])
AC_LANG_POP(C++)

]) # LOL_AC_INIT


# LOL_AC_CHECK()
# ---------------------
AC_DEFUN([LOL_AC_CHECK], [

dnl  Check that the C++ compiler really works
AC_LANG_PUSH(C++)
AC_MSG_CHECKING(for a fully working C++ compiler)
AC_TRY_LINK([], [],
 [AC_MSG_RESULT(yes)],
 [AC_MSG_RESULT(no)
  AC_MSG_ERROR([[C++ compiler cannot link executables]])])
AC_LANG_POP(C++)


AC_C_CONST
AC_C_INLINE

dnl
dnl  Common libraries
dnl

AC_CHECK_LIB(m, sin, LIBS="${LIBS} -lm")
AC_CHECK_LIB(pthread, main, LIBS="${LIBS} -lpthread")


dnl
dnl  Activate some header-only libraries
dnl

LOL_CFLAGS="$LOL_CFLAGS -I\$(lol_srcdir)/src/3rdparty/pegtl/include"
LOL_CFLAGS="$LOL_CFLAGS -I\$(lol_srcdir)/src/3rdparty/imgui"

dnl  Use system-provided getopt_long?
ac_cv_have_getopt_long="no"
AC_CHECK_HEADERS(getopt.h unistd.h)
AC_CHECK_FUNCS(getopt_long,
 [ac_cv_have_getopt_long="yes"],
 [AC_CHECK_LIB(gnugetopt, getopt_long,
   [ac_cv_have_getopt_long="yes"
    LIBS="${LIBS} -lgnugetopt"])])
if test "$ac_cv_have_getopt_long" != "no"; then
  AC_DEFINE(HAVE_GETOPT_LONG, 1, Define to 1 if you have the ‘getopt_long’ function.)
fi


dnl  Use Bullet Physics?
ac_cv_my_have_bullet="no"
if test "${enable_bullet}" != "no"; then
  ac_cv_my_have_bullet="yes"
  AC_DEFINE(LOL_USE_BULLET, 1, Define to 1 to use Bullet Physics)
fi
AM_CONDITIONAL(LOL_USE_BULLET, test "${ac_cv_my_have_bullet}" = "yes")


dnl  Use Android? FIXME: super hacks!
ac_cv_my_have_android="no"
AC_CHECK_LIB(log, __android_log_vprint,
 [ac_cv_my_have_android="yes"
  LOL_LIBS="${LOL_LIBS} -llog -landroid -module -lEGL -lGLESv2"])
AM_CONDITIONAL(LOL_USE_ANDROID, test "${ac_cv_my_have_android}" != "no")


dnl  Use EGL? (autodetect unless GL is disabled)
ac_cv_my_have_egl="no"
if test "${enable_gl}" != "no"; then
  PKG_CHECK_MODULES(EGL, egl, [ac_cv_my_have_egl="yes"], [:])
  if test "${ac_cv_my_have_egl}" != "no"; then
    AC_DEFINE(LOL_USE_EGL, 1, Define to 1 to use libegl)
    EGL_LIBS="${EGL_LIBS} -lX11"
  fi
  AC_CHECK_HEADERS(EGL/egl.h,
   [AC_CHECK_LIB(EGL, eglInitialize,
     [ac_cv_my_have_egl="yes"
      AC_DEFINE(LOL_USE_EGL, 1, Define to 1 to use libegl)
      EGL_LIBS="-lEGL"])
    dnl  Raspberry Pi is different, check for it with extra libs; also we
    dnl  look for a different function to bypass autoconf caching
    AC_CHECK_LIB(EGL, eglGetDisplay,
     [ac_cv_my_have_egl="yes"
      AC_DEFINE(LOL_USE_EGL, 1, Define to 1 to use libegl)
      EGL_LIBS="-lEGL -lvcos -lvchiq_arm -lbcm_host -lGLESv2"],
     [:],
     [-lvcos -lvchiq_arm -lbcm_host -lGLESv2])])
fi
AM_CONDITIONAL(LOL_USE_EGL, test "${ac_cv_my_have_egl}" != "no")


dnl Use libpng? (replacement for SDL_image)
ac_cv_my_have_libpng="no"
if test "${enable_png}" != "no"; then
  PKG_CHECK_MODULES(LIBPNG, libpng, [ac_cv_my_have_libpng="yes"], [:])
  if test "${ac_cv_my_have_libpng}" != "no"; then
    AC_DEFINE(LOL_USE_LIBPNG, 1, Define to 1 to use libpng)
  fi
fi
AM_CONDITIONAL(LOL_USE_LIBPNG, test "${ac_cv_my_have_libpng}" != "no")


dnl  Use Imlib2?
ac_cv_my_have_imlib2="no"
if test "${enable_imlib2}" != "no"; then
  PKG_CHECK_MODULES(IMLIB2, imlib2, [ac_cv_my_have_imlib2="yes"], [:])
  if test "${ac_cv_my_have_imlib2}" != "no"; then
    AC_DEFINE(LOL_USE_IMLIB2, 1, Define to 1 to use Imlib2)
    LOL_CFLAGS="${LOL_CFLAGS} ${IMLIB2_CFLAGS}"
    LOL_LIBS="${LOL_LIBS} ${IMLIB2_LIBS}"
  fi
fi
AM_CONDITIONAL(LOL_USE_IMLIB2, test "${ac_cv_my_have_imlib2}" = "yes")


dnl Use Windows GDI+?
ac_cv_my_have_gdiplus="no"
AC_LANG_PUSH(C++)
AC_CHECK_HEADERS(gdiplus.h,
 [ac_cv_my_have_gdiplus="yes"
  LOL_LIBS="${LOL_LIBS} -lgdiplus"],
 [ac_cv_my_have_gdiplus="no"],
 [#include <algorithm>
  using std::min;
  using std::max;
  #include <windows.h>])
AC_LANG_POP(C++)
if test "${ac_cv_my_have_gdiplus}" != "no"; then
  AC_DEFINE(LOL_USE_GDIPLUS, 1, Define to 1 to use GDI+)
fi
AM_CONDITIONAL(LOL_USE_GDIPLUS, test "${ac_cv_my_have_gdiplus}" = "yes")


dnl  Are we building using MinGW?
LOL_TRY_CXXFLAGS(-mwindows -mwin32,
 [AM_CXXFLAGS="${AM_CXXFLAGS} -mwindows -mwin32"
  LOL_CFLAGS="${LOL_CFLAGS} -I\$(lol_srcdir)/src/3rdparty/mingw-std-threads"
  LOL_CFLAGS="${LOL_CFLAGS} -D__STDC_FORMAT_MACROS"
  LOL_LIBS="${LOL_LIBS} -uWinMain -u_WinMain@16"])


dnl  Are we building using Emscripten?
ac_cv_my_have_emscripten="no"
AC_CHECK_HEADERS(emscripten.h,
 [ac_cv_my_have_emscripten="yes"
  AC_CHECK_HEADERS(emscripten/html5.h)
  dnl  XXX: activate this if memory heap is too small
  #AM_CXXFLAGS="${AM_CXXFLAGS} -s ALLOW_MEMORY_GROWTH=1"
  dnl  HACK: until emcc properly adds these to EMSDK_OPTS
  CPPFLAGS="${CPPFLAGS} -U__i386__ -U__x86_64__ -U__i386 -U__x86_64 -Ui386 -Ux86_64"
  CPPFLAGS="${CPPFLAGS} -U__SSE__ -U__SSE2__ -U__MMX__ -UX87_DOUBLE_ROUNDING"
  CPPFLAGS="${CPPFLAGS} -UHAVE_GCC_ASM_FOR_X87 -DEMSCRIPTEN -U__STRICT_ANSI__"
  CPPFLAGS="${CPPFLAGS} -U__CYGWIN__"])
AM_CONDITIONAL(LOL_USE_EMSCRIPTEN, test "${ac_cv_my_have_emscripten}" != "no")


dnl  Are we on an OS X or iOS platform?
LOL_TRY_LDFLAGS(-framework Foundation,
 [LOL_LIBS="${LOL_LIBS} -framework Foundation"])
LOL_TRY_LDFLAGS(-framework CoreGraphics,
 [LOL_LIBS="${LOL_LIBS} -framework CoreGraphics"])
LOL_TRY_LDFLAGS(-framework CoreData,
 [LOL_LIBS="${LOL_LIBS} -framework CoreData"])
LOL_TRY_LDFLAGS(-framework UIKit,
 [LOL_LIBS="${LOL_LIBS} -framework UIKit"])


dnl  Other complex checks
LOL_AC_CHECK_OPENGL()
LOL_AC_CHECK_SDL()
LOL_AC_CHECK_FFMPEG()


dnl  Debug symbols
LOL_TRY_LDFLAGS(-rdynamic, [AM_LDFLAGS="${AM_LDFLAGS} -rdynamic"])

dnl  Code qui fait des warnings == code de porc == deux baffes dans ta gueule
LOL_TRY_CXXFLAGS(-Wall, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wall"])
LOL_TRY_CXXFLAGS(-Wextra, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wextra"])
LOL_TRY_CXXFLAGS(-Wpointer-arith, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wpointer-arith"])
LOL_TRY_CXXFLAGS(-Wcast-align, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wcast-align"])
LOL_TRY_CXXFLAGS(-Wcast-qual, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wcast-qual"])
LOL_TRY_CXXFLAGS(-Wshadow, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wshadow"])
LOL_TRY_CXXFLAGS(-Wsign-compare, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wsign-compare"])
LOL_TRY_CXXFLAGS(-Wstrict-overflow=1, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wstrict-overflow=1"])

dnl  Add these even though they're implicitly set, so that we can safely
dnl  remove them from within a Makefile.
LOL_TRY_CXXFLAGS(-Wuninitialized, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wuninitialized"])
LOL_TRY_CXXFLAGS(-Wnarrowing, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wnarrowing"])
LOL_TRY_CXXFLAGS(-Wunused, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wunused"])
LOL_TRY_CXXFLAGS(-Wcast-align, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wcast-align"])
LOL_TRY_CXXFLAGS(-Wunused-parameter, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wunused-parameter"])
LOL_TRY_CXXFLAGS(-Wstrict-aliasing, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wstrict-aliasing"])
LOL_TRY_CXXFLAGS(-Wparentheses, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wparentheses"])
LOL_TRY_CXXFLAGS(-Wreorder, [AM_CXXFLAGS="${AM_CXXFLAGS} -Wreorder"])

dnl  Disable these warnings because they're annoyingly verbose
LOL_TRY_CXXFLAGS(-Wno-psabi, [AM_CPPFLAGS="${AM_CPPFLAGS} -Wno-psabi"])

]) # LOL_AC_CHECK


# LOL_AC_FINI()
# ---------------------
AC_DEFUN([LOL_AC_FINI], [

dnl  How to use the Lol Engine outside this tree
LOL_CFLAGS="$LOL_CFLAGS $SDL_CFLAGS $GL_CFLAGS $EGL_CFLAGS $LIBPNG_CFLAGS"
LOL_LIBS="$LOL_LIBS $SDL_LIBS $GL_LIBS $EGL_LIBS $LIBPNG_LIBS $D3D_LIBS"
LOL_DEPS="${LOL_DEPS} \$(lol_builddir)/src/liblol-core.a"
LOL_BULLET_DEPS="${LOL_BULLET_DEPS} \$(lol_builddir)/src/3rdparty/liblol-bullet.a"
LOL_LUA_DEPS="${LOL_LUA_DEPS} \$(lol_builddir)/src/3rdparty/liblol-lua.a"

dnl  How to use the Lol Engine inside this tree
AM_CPPFLAGS="${AM_CPPFLAGS} -I\$(lol_srcdir)/src"
AM_CPPFLAGS="${AM_CPPFLAGS} -DLOL_CONFIG_SOURCESUBDIR=\\\"\$(subdir)\\\""
AM_CPPFLAGS="${AM_CPPFLAGS} ${LOL_CFLAGS}"
AM_LDFLAGS="${AM_LDFLAGS} ${LOL_DEPS}"
AM_LDFLAGS="${AM_LDFLAGS} ${LOL_LIBS}"

dnl  Extra flags
AC_SUBST(LOL_CFLAGS)
AC_SUBST(LOL_LIBS)
AC_SUBST(LOL_DEPS)
AC_SUBST(LOL_BULLET_DEPS)
AC_SUBST(LOL_LUA_DEPS)

AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(AM_LDFLAGS)

]) # LOL_AC_FINI

