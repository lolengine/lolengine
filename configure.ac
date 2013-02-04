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

AC_CONFIG_HEADER(config.h)

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

dnl  Same for Lol Engine M4 files.
m4_pattern_forbid([^LOL_])


dnl Do not use PKG_CONFIG_LIBDIR when cross-compiling.
if test "${build}" != "${host}" -a "${PKG_CONFIG_LIBDIR}" = ""; then
  export PKG_CONFIG_LIBDIR=/dev/null
fi

dnl AC_PROG_EGREP only exists in autoconf 2.54+, so we use AC_EGREP_CPP right
dnl now otherwise it might be set in an obscure if statement. Same thing for
dnl PKG_PROG_PKG_CONFIG which needs to be called first.
AC_EGREP_CPP(yes, foo)
PKG_PROG_PKG_CONFIG()


dnl  Check for a working implementation of sed
AC_PROG_SED
AC_MSG_CHECKING(for a sed that understands \t)
if test "$(echo 'x\x' | "${SED}" 's/.*@<:@^x\t@:>@//')" != x; then
  AC_MSG_RESULT(no)
  AC_MSG_ERROR([[consider installing GNU sed]])
else
  AC_MSG_RESULT(yes)
fi


AM_CONDITIONAL(USE_GLEW, test "${ac_cv_my_have_glew}" != "no")
dnl conditional builds
AC_ARG_ENABLE(debug,
  [  --enable-debug          build debug versions of the game (default no)])
AC_ARG_ENABLE(release,
  [  --enable-release        build final release of the game (default no)])
AC_ARG_ENABLE(experimental,
  [  --enable-experimental   experimental build (default no)])
AC_ARG_ENABLE(doc,
  [  --enable-doc            build documentation (needs doxygen and LaTeX)])

AC_CHECK_HEADERS(stdio.h stdarg.h inttypes.h endian.h stdint.h getopt.h)
AC_CHECK_HEADERS(fastmath.h pthread.h libutil.h util.h pty.h glob.h unistd.h)
AC_CHECK_HEADERS(sys/ioctl.h sys/ptrace.h sys/stat.h sys/syscall.h sys/user.h)
AC_CHECK_HEADERS(sys/wait.h)
AC_CHECK_HEADERS(linux/kdev_t.h linux/major.h)
AC_CHECK_HEADERS(security/pam_appl.h security/pam_misc.h)
AC_CHECK_HEADERS(pam/pam_appl.h pam/pam_misc.h)

AC_CHECK_FUNCS(getcwd _getcwd)

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

# Build documentation?
DOXYGEN="no"
LATEX="no"
if test "${enable_doc}" != "no"; then
  AC_PATH_PROG(DOXYGEN, doxygen, no)
  if test "${DOXYGEN}" != "no"; then
    # Build LaTeX documentation?
    AC_PATH_PROG(LATEX, pdflatex, no)
    AC_PATH_PROG(KPSEWHICH, kpsewhich, no)
    AC_PATH_PROG(DVIPS, dvips, no)
    if test "${DVIPS}" = "no" -o "${KPSEWHICH}" = "no"; then
      LATEX="no"
    fi
    if test "${LATEX}" != "no"; then
      AC_MSG_CHECKING(for a4.sty and a4wide.sty)
      if "${KPSEWHICH}" a4.sty >/dev/null 2>&1; then
        if "${KPSEWHICH}" a4wide.sty >/dev/null 2>&1; then
          AC_MSG_RESULT(yes)
        else
          LATEX="no"
          AC_MSG_RESULT(no)
        fi
      else
        LATEX="no"
        AC_MSG_RESULT(no)
      fi
    fi
    AC_PATH_PROG(DOT, dot, no)
    if test "${DOT}" != "no"; then
      USE_DOT="YES"
    else
      USE_DOT="NO"
    fi
  fi
fi
AM_CONDITIONAL(BUILD_DOCUMENTATION, test "${DOXYGEN}" != "no")
AM_CONDITIONAL(USE_LATEX, test "${LATEX}" != "no")
AM_CONDITIONAL(USE_DOT, test "${DOT}" != "no")

dnl No exceptions
CXXFLAGS="${CXXFLAGS} -fno-exceptions -fno-rtti"
dnl Optimizations
CXXFLAGS="${CXXFLAGS} ${REL} ${OPT}"
dnl Code qui fait des warnings == code de porc == deux baffes dans ta gueule
CXXFLAGS="${CXXFLAGS} -Wall -Wextra -Wpointer-arith -Wcast-align -Wcast-qual -Wshadow -Wsign-compare"

AC_CHECK_LIB(m, sin, MATH_LIBS="${MATH_LIBS} -lm")
AC_CHECK_LIB(pthread, main, LIBS="$LIBS -lpthread")

AC_CHECK_LIB(util, forkpty,
 [UTIL_LIBS="${UTIL_LIBS} -lutil"
  dnl  Override future forkpty detection
  ac_cv_func_forkpty="yes"])
AC_CHECK_FUNCS(forkpty)

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
  AC_PATH_PROG(MAKE_FSELF, make_fself, no)
  if test "${MAKE_FSELF}" = "no"; then
    AC_MSG_ERROR([[Building for PS3 but make_fself not found]])
  fi],
 [MAKE_FSELF=no])
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

LOL_CHECK_OPENGL

LOL_CHECK_SDL

dnl  Use Flex's FlexLexer.h or ours?
ac_cv_my_have_flexlexer_h="no"
AC_LANG_PUSH(C++)
AC_CHECK_HEADERS(FlexLexer.h,
 dnl  Ensure that FlexLexer::yleng is of type int, and not size_t like
 dnl  on recent Apple systems. It would break all our existing code.
 [AC_MSG_CHECKING(for FlexLexer.h validity)
  AC_COMPILE_IFELSE(
   [AC_LANG_PROGRAM([#include <FlexLexer.h>
      class Foo : public FlexLexer
      {
          Foo() { int &test = yyleng; }
      }],
     [])],
   [AC_MSG_RESULT(yes)
    ac_cv_my_have_flexlexer_h="yes"],
   [AC_MSG_RESULT(no)])])
AC_LANG_POP(C++)
if test "x${ac_cv_my_have_flexlexer_h}" = "xno"; then
  LOL_CFLAGS="$LOL_CFLAGS -I\$(top_srcdir)/external/flex-2.5.35/include"
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
  LOL_CFLAGS="${LOL_CFLAGS} ${PIPI_CFLAGS}"
  LOL_LIBS="${LOL_LIBS} ${PIPI_LIBS}"
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
AM_CONDITIONAL(BUILD_NEERCS, test "${ac_cv_my_have_caca}" != "no")


dnl  Extra libraries we may need
AC_SUBST(MATH_LIBS)
AC_SUBST(PAM_LIBS)
AC_SUBST(UTIL_LIBS)

dnl  How to use the Lol Engine inside this tree
LOL_CFLAGS="$LOL_CFLAGS -I\$(top_srcdir)/src"
LOL_CFLAGS="$LOL_CFLAGS -DLOL_SOURCE_SUBDIR=\\\"\$(subdir)\\\""
LOL_CFLAGS="$LOL_CFLAGS $SDL_CFLAGS $GL_CFLAGS $EGL_CFLAGS $LIBPNG_CFLAGS"
LOL_LIBS="$LOL_LIBS $SDL_LIBS $GL_LIBS $EGL_LIBS $LIBPNG_LIBS $D3D_LIBS"

dnl  Extra flags
AC_SUBST(LOL_CFLAGS)
AC_SUBST(LOL_LIBS)

AC_SUBST(USE_DOT)

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
  tools/vimlol/Makefile
  tools/vslol/Makefile
  doc/Makefile
  doc/doxygen.cfg
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
  people/jnat/TestProject/Makefile
  people/peeweek/Makefile
  people/peeweek/private/Makefile
  people/touky/Makefile
  people/touky/private/Makefile
  people/jylam/Makefile
  people/jylam/private/Makefile
  people/jylam/private/chat/Makefile
])

AC_OUTPUT

