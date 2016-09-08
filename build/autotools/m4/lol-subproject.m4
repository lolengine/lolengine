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


# LOL_AC_SUBPROJECT()
# ---------------------
AC_DEFUN([LOL_AC_SUBPROJECT], [

dnl
dnl  Build and configure Lol Engine before our repository
dnl  Ensure $lol_srcdir and $lol_builddir are properly set up
dnl

ac_configure_args="${ac_configure_args} --disable-test --disable-doc --disable-tutorial --disable-samples --disable-tools"
AC_CONFIG_SUBDIRS([lol])
AC_SUBST(lol_srcdir, '${top_srcdir}/lol')
AC_SUBST(lol_builddir, '${top_builddir}/lol')

dnl
dnl  Subprojects use the same macros as us
dnl

LOL_AC_INIT()
LOL_AC_CHECK()
LOL_AC_SUBST()

]) # LOL_AC_SUBPROJECT

