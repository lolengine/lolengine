dnl
dnl  Lol Engine
dnl
dnl  Copyright © 2010—2016 Sam Hocevar <sam@hocevar.net>
dnl
dnl  Lol Engine is free software. It comes without any warranty, to
dnl  the extent permitted by applicable law. You can redistribute it
dnl  and/or modify it under the terms of the Do What the Fuck You Want
dnl  to Public License, Version 2, as published by the WTFPL Task Force.
dnl  See http://www.wtfpl.net/ for more details.
dnl

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

AC_SUBST(lol_srcdir, '${top_srcdir}')
AC_SUBST(lol_builddir, '${top_builddir}')

AC_CONFIG_HEADER(config.h)

AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_RANLIB

LOL_AC_INIT()

AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL
AC_LIBTOOL_CXX

dnl Ensure an error is thrown if pkg-config M4 files are not found.
m4_pattern_allow([^PKG_CONFIG_LIBDIR$])
m4_pattern_forbid([^PKG_CHECK_MODULES$])
m4_pattern_forbid([^PKG_PROG_PKG_CONFIG$])

dnl  Same for Lol Engine M4 files.
m4_pattern_forbid([^LOL_AC_])


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


dnl conditional builds
AC_ARG_ENABLE(debug,
  [  --enable-debug          build debug versions of the game (default no)])
AC_ARG_ENABLE(release,
  [  --enable-release        build final release of the game (default no)])
AC_ARG_ENABLE(experimental,
  [  --enable-experimental   experimental build (default no)])

AC_ARG_ENABLE(test,
  [  --enable-test           build test suite (default yes)])
AC_ARG_ENABLE(tools,
  [  --enable-tools          build miscellaneous tools (default yes)])
AC_ARG_ENABLE(tutorial,
  [  --enable-tutorial       build tutorial applications (default yes)])
AC_ARG_ENABLE(samples,
  [  --enable-samples        build sample applications (default yes)])
AC_ARG_ENABLE(doc,
  [  --enable-doc            build documentation (needs doxygen and LaTeX)])


dnl  Common C headers
AC_CHECK_HEADERS(stdio.h stdarg.h inttypes.h stdint.h math.h)
AC_CHECK_HEADERS(fastmath.h unistd.h io.h)
AC_CHECK_HEADERS(execinfo.h)
AC_CHECK_HEADERS(sys/ioctl.h sys/ptrace.h sys/stat.h sys/syscall.h sys/user.h)
AC_CHECK_HEADERS(sys/wait.h sys/time.h sys/types.h)


dnl  Common C++ headers
AC_LANG_PUSH(C++)
AC_CHECK_HEADERS(cxxabi.h)
AC_LANG_POP(C++)

dnl  Common C functions
AC_CHECK_FUNCS(getenv system tmpfile tmpnam getcwd _getcwd getlogin)
AC_CHECK_FUNCS(backtrace_symbols)
AC_CHECK_FUNCS(gettimeofday usleep)


dnl  Build mode
ac_cv_my_build_mode="devel"
if test "x${enable_release}" = "xyes"; then
  ac_cv_my_build_mode="release"
fi
if test "x${enable_devel}" = "xyes"; then
  ac_cv_my_build_mode="devel"
fi
if test "x${enable_debug}" = "xyes"; then
  ac_cv_my_build_mode="debug"
fi

if test "x${ac_cv_my_build_mode}" = "xdebug"; then
  AC_DEFINE(LOL_BUILD_DEBUG, 1, Define to 1 to activate debug build)
  LOL_TRY_CXXFLAGS(-O, [AM_CXXFLAGS="${AM_CXXFLAGS} -O"])
  LOL_TRY_CXXFLAGS(-g, [AM_CXXFLAGS="${AM_CXXFLAGS} -g"])
elif test "x${ac_cv_my_build_mode}" = "xdevel"; then
  AC_DEFINE(LOL_BUILD_DEVEL, 1, Define to 1 to activate development build)
  LOL_TRY_CXXFLAGS(-Os, [AM_CXXFLAGS="${AM_CXXFLAGS} -Os"])
  LOL_TRY_CXXFLAGS(-g, [AM_CXXFLAGS="${AM_CXXFLAGS} -g"])
  LOL_TRY_CXXFLAGS(-ffast-math, [AM_CXXFLAGS="${AM_CXXFLAGS} -ffast-math"])
else
  AC_DEFINE(LOL_BUILD_RELEASE, 1, Define to 1 to activate final release)
  LOL_TRY_CXXFLAGS(-Os, [AM_CXXFLAGS="${AM_CXXFLAGS} -Os"])
  LOL_TRY_CXXFLAGS(-ffast-math, [AM_CXXFLAGS="${AM_CXXFLAGS} -ffast-math"])
  LOL_TRY_CXXFLAGS(-fomit-frame-pointer, [AM_CXXFLAGS="${AM_CXXFLAGS} -fomit-frame-pointer"])
  LOL_TRY_CXXFLAGS(-fno-strength-reduce, [AM_CXXFLAGS="${AM_CXXFLAGS} -fno-strength-reduce"])
fi


dnl  Build documentation?
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
      LOL_USE_DOT="YES"
    else
      LOL_USE_DOT="NO"
    fi
  fi
fi
dnl  XXX: disable LaTeX because we use too much memory
LATEX=no
AM_CONDITIONAL(BUILD_DOCUMENTATION, test "${DOXYGEN}" != "no")
AM_CONDITIONAL(LOL_USE_LATEX, test "${LATEX}" != "no")
AM_CONDITIONAL(LOL_USE_DOT, test "${DOT}" != "no")
AC_SUBST(LOL_USE_DOT)


dnl  GCC-specific symbol demangling
AC_LANG_PUSH(C++)
AC_TRY_LINK(
 [#include <cxxabi.h>],
 [abi::__cxa_demangle(NULL, 0, 0, NULL);],
 [AC_DEFINE(HAVE_CXA_DEMANGLE, 1, Define to 1 if abi::__cxa_demangle is available)])
AC_LANG_POP(C++)


LOL_AC_CHECK()


dnl  Use libcaca? (required for font generation)
ac_cv_my_have_caca="no"
PKG_CHECK_MODULES(CACA, caca >= 0.99.beta17, [ac_cv_my_have_caca="yes"], [:])
if test "${ac_cv_my_have_caca}" != "no"; then
  AC_DEFINE(LOL_USE_CACA, 1, Define to 1 to use libcaca)
fi
AM_CONDITIONAL(LOL_USE_CACA, test "${ac_cv_my_have_caca}" != "no")


dnl Use GTK+? (required for the deushax editor)
ac_cv_my_have_gtkgl="no"
PKG_CHECK_MODULES(GTK, gtk+-2.0, [ac_cv_my_have_gtkgl="yes"], [:])
PKG_CHECK_MODULES(GTKGL, gtkgl-2.0, [:], [ac_cv_my_have_gtkgl="no"])
if test "${ac_cv_my_have_gtkgl}" != "no"; then
  AC_DEFINE(LOL_USE_GTKGL, 1, Define to 1 to use GtkGl)
fi
AM_CONDITIONAL(LOL_USE_GTKGL, test "${ac_cv_my_have_gtkgl}" != "no")


dnl  Optional features
AM_CONDITIONAL(BUILD_TEST, test "${enable_test}" != "no")
AM_CONDITIONAL(BUILD_TOOLS, test "${enable_tools}" != "no")
AM_CONDITIONAL(BUILD_TUTORIAL, test "${enable_tutorial}" != "no")
AM_CONDITIONAL(BUILD_SAMPLES, test "${enable_samples}" != "no")


dnl Should we ship non-free data?
AM_CONDITIONAL(HAVE_NONFREE, true)


LOL_AC_SUBST()

AC_CONFIG_FILES(
 [Makefile
  src/Makefile
  src/3rdparty/Makefile
  src/data/Makefile
  src/data/font/Makefile
  src/t/Makefile
  build/Makefile
  binaries/Makefile
  doc/Makefile
  doc/doxygen.cfg
  doc/samples/Makefile
  doc/samples/math/Makefile
  doc/samples/meshviewer/Makefile
  doc/samples/sandbox/Makefile
  doc/tutorial/Makefile
  tools/Makefile
  tools/lolremez/Makefile
  tools/lolunit/Makefile
  tools/vimlol/Makefile
  tools/vslol/Makefile
])

AC_OUTPUT

