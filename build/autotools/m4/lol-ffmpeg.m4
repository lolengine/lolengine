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


# LOL_AC_CHECK_FFMPEG()
# ---------------------
AC_DEFUN([LOL_AC_CHECK_FFMPEG],
[
if test "${enable_ffmpeg}" != "no"; then
  LOL_AC_CHECK_FFMPEG_INNER()
fi
AM_CONDITIONAL(LOL_USE_FFMPEG, test "${ac_cv_my_have_ffmpeg}" != "no")
])

# LOL_AC_CHECK_FFMPEG_INNER()
# ---------------------------
AC_DEFUN([LOL_AC_CHECK_FFMPEG_INNER],
[
ac_cv_my_have_ffmpeg="yes"
PKG_CHECK_MODULES([LIBAVCODEC], [libavcodec], [:], [ac_cv_my_have_ffmpeg=no])
PKG_CHECK_MODULES([LIBAVUTIL], [libavutil], [:], [ac_cv_my_have_ffmpeg=no])
PKG_CHECK_MODULES([LIBAVFORMAT], [libavformat], [:], [ac_cv_my_have_ffmpeg=no])
PKG_CHECK_MODULES([LIBSWSCALE], [libswscale], [:], [ac_cv_my_have_ffmpeg=no])
if test "${ac_cv_my_have_ffmpeg}" != "yes"; then
  ac_cv_my_have_ffmpeg="yes"
  AC_CHECK_HEADERS(libavcodec/avcodec.h, [:], [ac_cv_my_have_ffmpeg=no])
  AC_CHECK_HEADERS(libavformat/avformat.h, [:], [ac_cv_my_have_ffmpeg=no])
  AC_CHECK_HEADERS(libswscale/swscale.h, [:], [ac_cv_my_have_ffmpeg=no])
  if test "${ac_cv_my_have_ffmpeg}" = "yes"; then
    LIBAVCODEC_LIBS="-lavcodec"
    LIBAVUTIL_LIBS="-lavutil"
    LIBAVFORMAT_LIBS="-lavformat"
    LIBSWSCALE_LIBS="-lswscale"
    AC_CHECK_LIB(ws2_32, main, LOL_LIBS="${LOL_LIBS} -lws2_32")
  fi
fi
if test "${ac_cv_my_have_ffmpeg}" = "yes"; then
  AC_DEFINE(LOL_USE_FFMPEG, 1, Define to 1 to use FFmpeg)
  LOL_CFLAGS="${LOL_CFLAGS} ${LIBAVFORMAT_CFLAGS} ${LIBAVUTIL_CFLAGS} ${LIBAVCODEC_CFLAGS} ${LIBSWSCALE_CFLAGS}"
  LOL_LIBS="${LOL_LIBS} ${LIBAVFORMAT_LIBS} ${LIBAVUTIL_LIBS} ${LIBAVCODEC_LIBS} ${LIBSWSCALE_LIBS}"
fi

]) # LOL_AC_CHECK_FFMPEG_INNER

