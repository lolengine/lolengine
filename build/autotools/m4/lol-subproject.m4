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

ac_lol_subproject_dir="$1"
AC_MSG_NOTICE([Configuring Lol Engine in ${ac_lol_subproject_dir}])

dnl
dnl  Parse build.config if present
dnl

ac_lol_configure_args=""
if test -f ${srcdir}/build.config; then
  for x in `sed -ne 's,.*<enable_\(@<:@^>@:>@*\)>\(@<:@^<@:>@*\)</enable_@<:@^>@:>@*>.*,\1=\2,p' ${srcdir}/build.config`; do
    eval "enable_${x}"
    case ${x} in
      *=no)  arg="--disable-${x%=no}" ;;
      *=yes) arg="--enable-${x%=yes}" ;;
      *) arg="" ;;
    esac
    ac_lol_configure_args="${ac_lol_configure_args} ${arg}"
  done
fi

dnl
dnl  Build and configure Lol Engine before our repository
dnl  Ensure $lol_srcdir and $lol_builddir are properly set
dnl

ac_configure_args="${ac_configure_args} --enable-subproject ${ac_lol_configure_args}"
AC_CONFIG_SUBDIRS($1)
AC_SUBST(lol_srcdir, '${top_srcdir}/$1')
AC_SUBST(lol_builddir, '${top_builddir}/$1')

dnl
dnl  Subprojects use the same macros as us
dnl

LOL_AC_INIT()
LOL_AC_CHECK()
LOL_AC_FINI()

]) # LOL_AC_SUBPROJECT

