
# LOL_AC_SUBST()
# ---------------------
AC_DEFUN([LOL_AC_SUBST], [

dnl  How to use the Lol Engine outside this tree
LOL_CFLAGS="$LOL_CFLAGS $SDL_CFLAGS $GL_CFLAGS $EGL_CFLAGS $LIBPNG_CFLAGS"
LOL_LIBS="$LOL_LIBS $SDL_LIBS $GL_LIBS $EGL_LIBS $LIBPNG_LIBS $D3D_LIBS"
LOL_DEPS="${LOL_DEPS} \$(lol_builddir)/src/liblol-core.a"
LOL_DEPS="${LOL_DEPS} \$(lol_builddir)/src/bullet/liblol-bullet.a"
LOL_DEPS="${LOL_DEPS} \$(lol_builddir)/src/lua/liblol-lua.a"

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

AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(AM_LDFLAGS)

]) # LOL_AC_SUBST


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


dnl  Use mingw threads if necessary
if true; then
  LOL_CFLAGS="$LOL_CFLAGS -I\$(lol_srcdir)/external/mingw-std-threads-0.0.ac5c9e3b/include/"
fi


dnl  Use pegtl? Yes, always
if true; then
  LOL_CFLAGS="$LOL_CFLAGS -I\$(lol_srcdir)/external/pegtl-1.3.1-e7752e81/include"
fi


dnl  Use Imgui? Yes, always
if true; then
  LOL_CFLAGS="$LOL_CFLAGS -I\$(lol_srcdir)/external/imgui"
fi


dnl  Use NativeClient?
ac_cv_my_have_nacl="no"
AC_LANG_PUSH(C++)
AC_CHECK_HEADERS(ppapi/cpp/instance.h,
 [ac_cv_my_have_nacl="yes"
  LOL_LIBS="${LOL_LIBS} -lnosys"
  if test "${ac_cv_my_build_mode}" = "xrelease"; then
    LOL_TRY_CXXFLAGS(-s, [AM_CXXFLAGS="${AM_CXXFLAGS} -s"])
  fi])
AC_LANG_POP(C++)
AM_CONDITIONAL(LOL_USE_NACL, test "${ac_cv_my_have_nacl}" != "no")


dnl  Use Android? FIXME: super hacks!
ac_cv_my_have_android="no"
AC_CHECK_LIB(log, __android_log_vprint,
 [ac_cv_my_have_android="yes"
  LOL_LIBS="${LOL_LIBS} -llog -landroid -module -lEGL -lGLESv2"])
AM_CONDITIONAL(LOL_USE_ANDROID, test "${ac_cv_my_have_android}" != "no")


dnl  Use EGL?
ac_cv_my_have_egl="no"
PKG_CHECK_MODULES(EGL, egl, [ac_cv_my_have_egl="yes"], [:])
if test "${ac_cv_my_have_egl}" != "no"; then
  AC_DEFINE(LOL_USE_EGL, 1, Define to 1 to use libegl)
  EGL_LIBS="${EGL_LIBS} -lX11"
fi
AC_CHECK_LIB(EGL, main,
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
 [-lvcos -lvchiq_arm -lbcm_host -lGLESv2])
AM_CONDITIONAL(LOL_USE_EGL, test "${ac_cv_my_have_egl}" != "no")


dnl Use libpng? (replacement for SDL_image)
ac_cv_my_have_libpng="no"
PKG_CHECK_MODULES(LIBPNG, libpng, [ac_cv_my_have_libpng="yes"], [:])
if test "${ac_cv_my_have_libpng}" != "no"; then
  AC_DEFINE(LOL_USE_LIBPNG, 1, Define to 1 to use libpng)
fi
AM_CONDITIONAL(LOL_USE_LIBPNG, test "${ac_cv_my_have_libpng}" != "no")


dnl  Use Imlib2?
ac_cv_my_have_imlib2="no"
PKG_CHECK_MODULES(IMLIB2, imlib2, [ac_cv_my_have_imlib2="yes"], [:])
if test "${ac_cv_my_have_imlib2}" != "no"; then
  AC_DEFINE(LOL_USE_IMLIB2, 1, Define to 1 to use Imlib2)
  LOL_CFLAGS="${LOL_CFLAGS} ${IMLIB2_CFLAGS}"
  LOL_LIBS="${LOL_LIBS} ${IMLIB2_LIBS}"
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


dnl  Use libavcodec? (required for movie encoding)
ac_cv_my_have_ffmpeg="yes"
PKG_CHECK_MODULES([LIBAVCODEC], [libavcodec], [:], [ac_cv_my_have_ffmpeg=no])
PKG_CHECK_MODULES([LIBAVFORMAT], [libavformat], [:], [ac_cv_my_have_ffmpeg=no])
PKG_CHECK_MODULES([LIBSWSCALE], [libswscale], [:], [ac_cv_my_have_ffmpeg=no])
if test "${ac_cv_my_have_ffmpeg}" = "no"; then
  ac_cv_my_have_ffmpeg="yes"
  AC_CHECK_HEADERS(libavcodec/avcodec.h, [:], [ac_cv_my_have_ffmpeg=no])
  AC_CHECK_HEADERS(libavformat/avformat.h, [:], [ac_cv_my_have_ffmpeg=no])
  AC_CHECK_HEADERS(libswscale/swscale.h, [:], [ac_cv_my_have_ffmpeg=no])
  if test "${ac_cv_my_have_ffmpeg}" = "yes"; then
    LIBAVFORMAT_LIBS="-lavformat"
    LIBAVCODEC_LIBS="-lavcodec -lavutil"
    LIBSWSCALE_LIBS="-lswscale -lswresample"
    AC_CHECK_LIB(ws2_32, main, LOL_LIBS="${LOL_LIBS} -lws2_32")
  fi
fi
if test "${ac_cv_my_have_ffmpeg}" != "no"; then
  AC_DEFINE(LOL_USE_FFMPEG, 1, Define to 1 to use FFmpeg)
  LOL_CFLAGS="${LOL_CFLAGS} ${LIBAVFORMAT_CFLAGS} ${LIBAVCODEC_CFLAGS} ${SWSCALE_CFLAGS}"
  LOL_LIBS="${LOL_LIBS} ${LIBAVFORMAT_LIBS} ${LIBAVCODEC_LIBS} ${LIBSWSCALE_LIBS}"
fi
AM_CONDITIONAL(LOL_USE_FFMPEG, test "${ac_cv_my_have_ffmpeg}" != "no")


dnl  Are we building using MinGW?
LOL_TRY_CXXFLAGS(-mwindows -mwin32,
 [AM_CXXFLAGS="${AM_CXXFLAGS} -mwindows -mwin32"
  LOL_LIBS="${LOL_LIBS} -uWinMain -u_WinMain@16"])


dnl  Are we building using Emscripten?
ac_cv_my_have_emscripten="no"
AC_CHECK_HEADERS(emscripten.h,
 [ac_cv_my_have_emscripten="yes"
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

]) # LOL_AC_CHECK

