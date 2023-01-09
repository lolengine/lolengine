dnl
dnl  Lol Engine
dnl
dnl  Copyright © 2010–2023 Sam Hocevar <sam@hocevar.net>
dnl
dnl  Lol Engine is free software. It comes without any warranty, to
dnl  the extent permitted by applicable law. You can redistribute it
dnl  and/or modify it under the terms of the Do What the Fuck You Want
dnl  to Public License, Version 2, as published by the WTFPL Task Force.
dnl  See http://www.wtfpl.net/ for more details.
dnl


dnl LOL_TRY_CFLAGS (CFLAGS, [ACTION-IF-WORKS], [ACTION-IF-FAILS])
dnl check if $CC supports a given set of cflags
AC_DEFUN([LOL_TRY_CFLAGS],
    [AC_MSG_CHECKING([if $CC supports $1 flags])
    save_CFLAGS="$CFLAGS"
    CFLAGS="$1 -Werror"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],[ac_cv_try_cflags_ok=yes],[ac_cv_try_cflags_ok=no])
    CFLAGS="$save_CFLAGS"
    AC_MSG_RESULT([$ac_cv_try_cflags_ok])
    if test x"$ac_cv_try_cflags_ok" = x"yes"; then
        ifelse([$2],[],[:],[$2])
    else
        ifelse([$3],[],[:],[$3])
    fi])

dnl LOL_TRY_CXXFLAGS (CXXFLAGS, [ACTION-IF-WORKS], [ACTION-IF-FAILS])
dnl check if $CXX supports a given set of cxxflags
AC_DEFUN([LOL_TRY_CXXFLAGS],
    [AC_MSG_CHECKING([if $CXX supports $1 flags])
    AC_LANG_PUSH(C++)
    save_CXXFLAGS="$CXXFLAGS"
    CXXFLAGS="$1 -Werror"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],[ac_cv_try_cxxflags_ok=yes],[ac_cv_try_cxxflags_ok=no])
    CXXFLAGS="$save_CXXFLAGS"
    AC_MSG_RESULT([$ac_cv_try_cxxflags_ok])
    if test x"$ac_cv_try_cxxflags_ok" = x"yes"; then
        ifelse([$2],[],[:],[$2])
    else
        ifelse([$3],[],[:],[$3])
    fi
    AC_LANG_POP(C++)])

dnl LOL_TRY_LDFLAGS (LDFLAGS, [ACTION-IF-WORKS], [ACTION-IF-FAILS])
dnl check if $CC supports a given set of ldflags
AC_DEFUN([LOL_TRY_LDFLAGS],
    [AC_MSG_CHECKING([if $CC supports $1 flags])
    save_LDFLAGS="$LDFLAGS"
    LDFLAGS="$1 -Werror"
    AC_LINK_IFELSE([AC_LANG_PROGRAM([],[])],[ac_cv_try_ldflags_ok=yes],[ac_cv_try_ldflags_ok=no])
    LDFLAGS="$save_LDFLAGS"
    AC_MSG_RESULT([$ac_cv_try_ldflags_ok])
    if test x"$ac_cv_try_ldflags_ok" = x"yes"; then
        ifelse([$2],[],[:],[$2])
    else
        ifelse([$3],[],[:],[$3])
    fi])
