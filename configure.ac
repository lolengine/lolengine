dnl configure script for Lol Engine

AC_INIT(lolengine, 0.0)
AC_PREREQ(2.50)
AC_CONFIG_AUX_DIR(.auto)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([subdir-objects no-define tar-ustar silent-rules])
dnl AM_MAINTAINER_MODE
AM_DEFAULT_VERBOSITY=0

dnl Versioning of the separate software we ship
LOLUNIT_VERSION=0.1
AC_SUBST(LOLUNIT_VERSION)
LOLREMEZ_VERSION=0.2
AC_SUBST(LOLREMEZ_VERSION)

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

AM_CONDITIONAL(USE_GLEW, test "${ac_cv_my_have_glew}" != "no")
dnl conditional builds
AC_ARG_ENABLE(debug,
  [  --enable-debug          build debug versions of the game (default no)])
AC_ARG_ENABLE(release,
  [  --enable-release        build final release of the game (default no)])
AC_ARG_ENABLE(experimental,
  [  --enable-experimental   experimental build (default no)])

AC_CHECK_HEADERS(stdio.h stdarg.h inttypes.h endian.h stdint.h getopt.h)
AC_CHECK_HEADERS(fastmath.h pthread.h libutil.h util.h pty.h glob.h)
AC_CHECK_HEADERS(sys/ioctl.h sys/ptrace.h sys/stat.h sys/syscall.h sys/user.h)
AC_CHECK_HEADERS(sys/wait.h)
AC_CHECK_HEADERS(linux/kdev_t.h linux/major.h)
AC_CHECK_HEADERS(security/pam_appl.h security/pam_misc.h)
AC_CHECK_HEADERS(pam/pam_appl.h pam/pam_misc.h)

if test "${enable_debug}" = "yes"; then
  AC_DEFINE(LOL_DEBUG, 1, Define to 1 to activate debug)
  OPT="-O"
else
  OPT="-O3 -ffast-math -fno-strength-reduce -fomit-frame-pointer"
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

dnl No exceptions
CXXFLAGS="${CXXFLAGS} -fno-exceptions -fno-rtti"
dnl Optimizations
CXXFLAGS="${CXXFLAGS} ${REL} ${OPT}"
dnl Code qui fait des warnings == code de porc == deux baffes dans ta gueule
CXXFLAGS="${CXXFLAGS} -Wall -Wextra -Wpointer-arith -Wcast-align -Wcast-qual -Wshadow -Wsign-compare"

AC_CHECK_LIB(m, sin, MATH_LIBS="${MATH_LIBS} -lm")
AC_CHECK_LIB(pthread, main, LIBS="$LIBS -lpthread")
AC_CHECK_LIB(util, forkpty, UTIL_LIBS="${UTIL_LIBS} -lutil")

dnl Are we on the PS3?
ac_cv_my_have_ps3="no"
AC_CHECK_LIB(sysmodule_stub, cellSysmoduleLoadModule,
 [ac_cv_my_have_ps3="yes"
  LOL_LIBS="${LOL_LIBS} -lsysmodule_stub -lsysutil_stub -lresc_stub"
  dnl  For PSGL
  AC_DEFINE(HAVE_GLES_2X, 1, Define to 1 if GLES 2.x is available) # FIXME: hack
  GL_LIBS="${GL_LIBS} -lPSGL -lgcm_cmd -lgcm_sys_stub"
  dnl  This is the old way; we now use CELL_SDK instead of CELLSDK
  if test "x${CELLSDK}" != x -a "x${CELL_SDK}" = x; then
    CELL_SDK="${CELLSDK}"
  fi
  if test "x${enable_debug}" = xyes; then
    GL_LIBS="${GL_LIBS} -L${CELL_SDK}/target/ppu/lib/PSGL/RSX/debug"
  elif test "x${enable_release}" = xyes; then
    GL_LIBS="${GL_LIBS} -L${CELL_SDK}/target/ppu/lib/PSGL/RSX/ultra-opt"
  else
    GL_LIBS="${GL_LIBS} -L${CELL_SDK}/target/ppu/lib/PSGL/RSX/opt"
  fi
  dnl  For the runtime Cg compiler
  GL_LIBS="${GL_LIBS} -lcgc -lPSGLcgc"
  dnl  For the PNG decoder
  LOL_LIBS="${LOL_LIBS} -lpngdec_stub"
  dnl  For the pad library
  LOL_LIBS="${LOL_LIBS} -lio_stub -lusbd_stub -lpadfilter"
  dnl  Disable this warning, it's too verbose with vector.h
  CXXFLAGS="${CXXFLAGS} -Wno-sign-compare"
  AC_PATH_PROG(MAKE_FSELF, make_fself, no)],
 [MAKE_FSELF=no])
AM_CONDITIONAL(USE_MAKE_FSELF, test "${MAKE_FSELF}" != "no")
AM_CONDITIONAL(USE_PS3, test "${ac_cv_my_have_ps3}" != "no")


dnl  Are we building using MinGW?
LIBS_save="$LIBS"
LIBS="$LIBS -mwindows -mwin32"
AC_MSG_CHECKING(for -mwindows -mwin32)
AC_TRY_LINK([], [],
 [AC_MSG_RESULT(yes)
  CXXFLAGS="${CXXFLAGS} -mwindows -mwin32"
  LOL_LIBS="${LOL_LIBS} -uWinMain -u_WinMain@16"],
 [AC_MSG_RESULT(no)])
LIBS="$LIBS_save"


dnl  Are we on the Xbox 360?
dnl  Answer: NO! we don't know how to build for it anyway
AM_CONDITIONAL(USE_X360, false)


dnl Are we on a Direct3D 9 platform?
#ac_cv_my_have_d3d9="no"
#AC_CHECK_HEADERS(d3d9.h, [ac_cv_my_have_d3d9="yes"])
#if test "${ac_cv_my_have_d3d9}" != "no"; then
#  D3D_LIBS="${D3D_LIBS} -ld3d9 -ld3dx9 -lxinput"
#  AC_DEFINE(USE_D3D9, 1, Define to 1 to use DirectX 9)
#fi
#AM_CONDITIONAL(USE_D3D9, test "${ac_cv_my_have_d3d9}" != "no")


dnl  Find which version of OpenGL to use
ac_cv_my_have_gl="no"
ac_cv_my_stop_looking_for_gl="no"

if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  LIBS_save="$LIBS"
  LIBS="$LIBS -Wl,-framework -Wl,OpenGL"
  AC_MSG_CHECKING(for -framework OpenGL)
  AC_TRY_LINK([], [],
   [AC_MSG_RESULT(yes)
    GL_LIBS="${GL_LIBS} -framework OpenGL"
    CXXFLAGS="${CXXFLAGS} -ObjC++"],
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

dnl  Use the PS3 PSGL?
if test "x${ac_cv_my_stop_looking_for_gl}" = "xno"; then
  AC_CHECK_HEADERS(PSGL/psgl.h,
   [ac_cv_my_have_gl="yes"
    ac_cv_my_stop_looking_for_gl="yes"])
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

if test "${ac_cv_my_have_gl}" = "no"; then
  AC_MSG_ERROR([[No OpenGL or OpenGL ES implementation found]])
fi


dnl Use Glew?
ac_cv_my_have_glew="no"
PKG_CHECK_MODULES(GLEW, glew,
 [ac_cv_my_have_glew="yes"
  GL_CFLAGS="${GLEW_CFLAGS} ${GL_CFLAGS}"
  GL_LIBS="${GLEW_LIBS} ${GL_LIBS}"],
 [:])
AC_CHECK_HEADER(glew.h,
 [LIBS_save="${LIBS}"
  LIBS="${LIBS} -lglew32 ${GL_LIBS}"
  AC_MSG_CHECKING(for glewInit in -lglew32)
  AC_TRY_LINK(
   [#include <glew.h>],
   [glewInit();],
   [ac_cv_my_have_glew="yes"
    GL_LIBS="-lglew32 ${GL_LIBS}"
    AC_MSG_RESULT(yes)],
   [AC_MSG_RESULT(no)])
  LIBS="${LIBS_save}"])
if test "${ac_cv_my_have_glew}" != "no"; then
  AC_DEFINE(HAVE_GLES_2X, 1, Define to 1 if GLES 2.x is available)
  AC_DEFINE(USE_GLEW, 1, Define to 1 to use libglew)
fi
AM_CONDITIONAL(USE_GLEW, test "${ac_cv_my_have_glew}" != "no")

dnl Poor man's GL feature detection if all else failed.
save_LIBS="${LIBS}"
LIBS="${LIBS} ${GL_LIBS} ${GLES2_LIBS}"
AC_CHECK_FUNCS(glBegin)
LIBS="${save_LIBS}"


dnl Use SDL? (always required on Linux or Win32)
ac_cv_my_have_sdl="no"
ac_cv_my_have_sdl_image="no"
ac_cv_my_have_sdl_mixer="no"
dnl
dnl  First, try the proper pkg-config check
PKG_CHECK_MODULES(SDL, sdl, [ac_cv_my_have_sdl="yes"], [:])
PKG_CHECK_MODULES(SDLMIXER, SDL_mixer, [ac_cv_my_have_sdl_mixer="yes"], [:])
PKG_CHECK_MODULES(SDLIMAGE, SDL_image, [ac_cv_my_have_sdl_image="yes"], [:])
dnl
dnl  Then the old sdl-config method
if test "x${cross_compiling}" != xyes -a "${ac_cv_my_have_sdl}" != yes; then
  AC_PATH_PROG(SDL_CONFIG, sdl-config, no)
  if test "${SDL_CONFIG}" != "no"; then
    SDL_CFLAGS="${SDL_CFLAGS} `${SDL_CONFIG} --cflags`"
    SDL_LIBS="${SDL_LIBS} `${SDL_CONFIG} --libs`"
    ac_cv_my_have_sdl="yes"
  fi
fi
dnl
dnl  Maybe all this has failed, try direct inclusion
save_CPPFLAGS="${CPPFLAGS}"
save_LIBS="${LIBS}"
CPPFLAGS="${CPPFLAGS} ${SDL_CFLAGS} ${SDLMIXER_CFLAGS} ${SDLIMAGE_CFLAGS}"
LIBS="${LIBS} ${SDL_LIBS} ${SDLMIXER_LIBS} ${SDLIMAGE_LIBS}"
AC_CHECK_HEADERS(SDL.h SDL/SDL.h, [ac_cv_my_have_sdl="yes"])
AC_CHECK_HEADERS(SDL_mixer.h SDL/SDL_mixer.h, [ac_cv_my_have_sdl_mixer="yes"])
AC_CHECK_HEADERS(SDL_image.h SDL/SDL_image.h, [ac_cv_my_have_sdl_image="yes"])
AC_CHECK_LIB(SDL, main,
 [SDL_LIBS="${SDL_LIBS} -lSDL"],
 [ac_cv_my_have_sdl="no"])
AC_CHECK_LIB(SDLmain, main,
 [SDL_LIBS="${SDL_LIBS} -lSDLmain -lSDL"])
AC_CHECK_LIB(SDL_mixer, main,
 [SDLMIXER_LIBS="${SDLMIXER_LIBS} -lSDL_mixer"],
 [ac_cv_my_have_sdl_mixer="no"])
AC_CHECK_LIB(SDL_image, main,
 [SDLIMAGE_LIBS="${SDLIMAGE_LIBS} -lSDL_image"],
 [ac_cv_my_have_sdl_image="no"])
SDL_CFLAGS="${SDL_CFLAGS} ${SDLMIXER_CFLAGS} ${SDLIMAGE_CFLAGS}"
SDL_LIBS="${SDL_LIBS} ${SDLMIXER_LIBS} ${SDLIMAGE_LIBS}"
CPPFLAGS="${save_CPPFLAGS}"
LIBS="${save_LIBS}"
if test "${ac_cv_my_have_sdl}" = "no"; then
  AC_MSG_WARN([SDL not found])
else
  AC_DEFINE(USE_SDL, 1, Define to 1 to use SDL)
fi
if test "${ac_cv_my_have_sdl_mixer}" = "no"; then
  AC_MSG_WARN([SDL_mixer not found])
else
  AC_DEFINE(USE_SDL_MIXER, 1, Define to 1 to use SDL_mixer)
fi
if test "${ac_cv_my_have_sdl_image}" = "no"; then
  AC_MSG_WARN([SDL_image not found])
else
  AC_DEFINE(USE_SDL_IMAGE, 1, Define to 1 to use SDL_image)
fi
AM_CONDITIONAL(USE_SDL, test "${ac_cv_my_have_sdl}" = "yes")
AM_CONDITIONAL(USE_SDL_MIXER, test "${ac_cv_my_have_sdl_mixer}" = "yes")
AM_CONDITIONAL(USE_SDL_IMAGE, test "${ac_cv_my_have_sdl_image}" = "yes")


dnl  Use Flex's FlexLexer.h or ours?
ac_cv_my_have_flexlexer_h="no"
AC_LANG_PUSH(C++)
AC_CHECK_HEADERS(FlexLexer.h, [ac_cv_my_have_flexlexer_h="yes"])
AC_LANG_POP(C++)
if test "x${ac_cv_my_have_flexlexer_h}" = "xno"; then
  LOL_CFLAGS="$LOL_CFLAGS -I\$(top_srcdir)/contrib/flex-2.5.35/include"
fi


dnl  Use NativeClient?
ac_cv_my_have_nacl="no"
AC_LANG_PUSH(C++)
AC_CHECK_HEADERS(ppapi/cpp/instance.h,
 [ac_cv_my_have_nacl="yes"
  dnl  Disable this warning, it's too verbose with vector.h
  CXXFLAGS="${CXXFLAGS} -Wno-sign-compare"])
AC_LANG_POP(C++)
AM_CONDITIONAL(USE_NACL, test "${ac_cv_my_have_nacl}" != "no")


dnl  Use Android? FIXME: super hacks!
ac_cv_my_have_android="no"
AC_CHECK_LIB(log, __android_log_vprint,
 [ac_cv_my_have_android="yes"
  LOL_LIBS="${LOL_LIBS} -llog -module"])
AM_CONDITIONAL(USE_ANDROID, test "${ac_cv_my_have_android}" != "no")


dnl  Use EGL?
ac_cv_my_have_egl="no"
PKG_CHECK_MODULES(EGL, egl, [ac_cv_my_have_egl="yes"], [:])
if test "${ac_cv_my_have_egl}" != "no"; then
  AC_DEFINE(USE_EGL, 1, Define to 1 to use libegl)
  EGL_LIBS="${EGL_LIBS} -lX11"
fi
AC_CHECK_LIB(EGL, main,
 [ac_cv_my_have_egl="yes"
  AC_DEFINE(USE_EGL, 1, Define to 1 to use libegl)
  EGL_LIBS="-lEGL"])
dnl  Raspberry Pi is different, check for it with extra libs; also we
dnl  look for a different function to bypass autoconf caching
AC_CHECK_LIB(EGL, eglGetDisplay,
 [ac_cv_my_have_egl="yes"
  AC_DEFINE(USE_EGL, 1, Define to 1 to use libegl)
  EGL_LIBS="-lEGL -lvcos -lvchiq_arm -lbcm_host -lGLESv2"],
 [:],
 [-lvcos -lvchiq_arm -lbcm_host -lGLESv2])
AM_CONDITIONAL(USE_EGL, test "${ac_cv_my_have_egl}" != "no")


dnl Use libpng? (replacement for SDL_image)
ac_cv_my_have_libpng="no"
PKG_CHECK_MODULES(LIBPNG, libpng, [ac_cv_my_have_libpng="yes"], [:])
if test "${ac_cv_my_have_libpng}" != "no"; then
  AC_DEFINE(USE_LIBPNG, 1, Define to 1 to use libpng)
fi
AM_CONDITIONAL(USE_LIBPNG, test "${ac_cv_my_have_libpng}" != "no")


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
  AC_DEFINE(USE_GDIPLUS, 1, Define to 1 to use GDI+)
fi
AM_CONDITIONAL(USE_GDIPLUS, test "${ac_cv_my_have_gdiplus}" = "yes")


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


dnl  Can we build neercs?
AM_CONDITIONAL(BUILD_NEERCS, test "${ac_cv_my_have_caca}" != "no" -a "${ac_cv_header_glob_h}" = "yes")


dnl Extra libraries we may need
AC_SUBST(MATH_LIBS)
AC_SUBST(PAM_LIBS)
AC_SUBST(UTIL_LIBS)

dnl How to use the Lol Engine inside this tree
LOL_CFLAGS="$LOL_CFLAGS -I \$(top_srcdir)/src"
LOL_CFLAGS="$LOL_CFLAGS $SDL_CFLAGS $GL_CFLAGS $EGL_CFLAGS $LIBPNG_CFLAGS"
LOL_LIBS="$LOL_LIBS $SDL_LIBS $GL_LIBS $EGL_LIBS $LIBPNG_LIBS $D3D_LIBS"

AC_SUBST(LOL_CFLAGS)
AC_SUBST(LOL_LIBS)


AC_CONFIG_FILES(
 [Makefile
  src/Makefile
  src/data/Makefile
  src/data/font/Makefile
  tutorial/Makefile
  test/Makefile
  test/math/Makefile
  test/sandbox/Makefile
  test/xolotl/Makefile
  build/Makefile
  binaries/Makefile
  people/Makefile
  games/Makefile
  tools/Makefile
  tools/vslol/Makefile
])
AC_CONFIG_FILES(
 [games/monsterz/Makefile
])
AC_CONFIG_FILES(
 [games/deushax/Makefile
  games/deushax/art/Makefile
  games/deushax/art/test/Makefile
  games/deushax/gfx/Makefile
  games/deushax/maps/Makefile
])
AC_CONFIG_FILES(
 [games/mrpigeon/Makefile
])
AC_CONFIG_FILES(
 [games/orbital/Makefile
])
AC_CONFIG_FILES(
 [tools/neercs/Makefile
])
AC_CONFIG_FILES(
 [people/jnat/Makefile
  people/peeweek/Makefile
  people/touky/Makefile
])

AC_OUTPUT

