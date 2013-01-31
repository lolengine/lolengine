
# LOL_CHECK_SDL()
# ---------------
AC_DEFUN([LOL_CHECK_SDL],
[
dnl  Find which version of SDL to use (always required on Linux or Win32)
dnl
dnl  Defined C/C++ macros:
dnl    USE_SDL         -- whether SDL or SDL2 were found at all
dnl    USE_SDL_IMAGE   -- whether SDL_image was found
dnl    USE_SDL_MIXER   -- whether SDL_mixer was found
dnl    HAVE_SDL2_SDL_H -- whether to include <SDL2/SDL.h>
dnl    HAVE_SDL_SDL_H  -- whether to include <SDL/SDL.h>
dnl    HAVE_SDL_H      -- whether to include <SDL.h>
dnl  Generated automake conditionals:
dnl    USE_SDL         -- whether SDL or SDL2 were found at all (conditional)
dnl    USE_SDL_IMAGE   -- whether SDL_image was found (conditional)
dnl    USE_SDL_MIXER   -- whether SDL_mixer was found (conditional)
dnl  Generated shell variables:
dnl    SDL_CFLAGS      -- flags for SDL compilation
dnl    SDL_LIBS        -- flags for SDL linking

ac_cv_my_have_sdl="no"
ac_cv_my_have_sdl_image="no"
ac_cv_my_have_sdl_mixer="no"

dnl  First, try the proper pkg-config check for SDL2
dnl if test "x${ac_cv_my_have_sdl}" = xno; then
dnl   PKG_CHECK_MODULES(SDL2, sdl2,
dnl    [ac_cv_my_have_sdl="yes"
dnl     PKG_CHECK_MODULES(SDL2MIXER, SDL2_mixer,
dnl      [ac_cv_my_have_sdl_mixer="yes"
dnl       AC_DEFINE(HAVE_SDL_MIXER_H, 1, Define to 1 to use SDL_mixer.h)],
dnl      [:])
dnl     PKG_CHECK_MODULES(SDL2IMAGE, SDL2_image,
dnl      [ac_cv_my_have_sdl_image="yes"
dnl       AC_DEFINE(HAVE_SDL_IMAGE_H, 1, Define to 1 to use SDL_image.h)],
dnl      [:])
dnl     SDL_CFLAGS="${SDL2_CFLAGS} ${SDL2MIXER_CFLAGS} ${SDL2IMAGE_CFLAGS}"
dnl     SDL_LIBS="${SDL2_LIBS} ${SDL2MIXER_LIBS} ${SDL2IMAGE_LIBS}"
dnl     AC_DEFINE(HAVE_SDL_H, 1, Define to 1 to use SDL.h)],
dnl    [:])
dnl fi

dnl  Then, try the proper pkg-config check for SDL 1.x
if test "x${ac_cv_my_have_sdl}" = xno; then
  PKG_CHECK_MODULES(SDL, sdl,
   [ac_cv_my_have_sdl="yes"
    PKG_CHECK_MODULES(SDLMIXER, SDL_mixer,
     [ac_cv_my_have_sdl_mixer="yes"
      AC_DEFINE(HAVE_SDL_MIXER_H, 1, Define to 1 to use SDL_mixer.h)],
     [:])
    PKG_CHECK_MODULES(SDLIMAGE, SDL_image,
     [ac_cv_my_have_sdl_image="yes"
      AC_DEFINE(HAVE_SDL_IMAGE_H, 1, Define to 1 to use SDL_image.h)],
     [:])
    SDL_CFLAGS="${SDL_CFLAGS} ${SDLMIXER_CFLAGS} ${SDLIMAGE_CFLAGS}"
    SDL_LIBS="${SDL_LIBS} ${SDLMIXER_LIBS} ${SDLIMAGE_LIBS}"
    AC_DEFINE(HAVE_SDL_H, 1, Define to 1 to use SDL.h)],
   [:])
fi

dnl  Then the old sdl-config method
if test "x${cross_compiling}" != xyes -a "x${ac_cv_my_have_sdl}" = xno; then
  AC_PATH_PROG(SDL_CONFIG, sdl-config, no)
  if test "${SDL_CONFIG}" != "no"; then
    ac_cv_my_have_sdl="yes"
    SDL_CFLAGS="${SDL_CFLAGS} `${SDL_CONFIG} --cflags`"
    SDL_LIBS="${SDL_LIBS} `${SDL_CONFIG} --libs`"
    AC_DEFINE(HAVE_SDL_H, 1, Define to 1 to use SDL.h),
  fi
fi

dnl  Maybe all this has failed, try direct inclusion instead
if test "x${ac_cv_my_have_sdl}" = xno; then
  save_CPPFLAGS="${CPPFLAGS}"
  save_LIBS="${LIBS}"
  CPPFLAGS="${CPPFLAGS} ${SDL_CFLAGS}"
  LIBS="${LIBS} ${SDL_LIBS}"
  AC_CHECK_HEADERS(SDL.h SDL/SDL.h, [ac_cv_my_have_sdl="yes"])
  if test "x${ac_cv_my_have_sdl}" != xno; then
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
  fi
  SDL_CFLAGS="${SDL_CFLAGS} ${SDLMIXER_CFLAGS} ${SDLIMAGE_CFLAGS}"
  SDL_LIBS="${SDL_LIBS} ${SDLMIXER_LIBS} ${SDLIMAGE_LIBS}"
  CPPFLAGS="${save_CPPFLAGS}"
  LIBS="${save_LIBS}"
fi

if test "x${ac_cv_my_have_sdl}" = xno; then
  AC_MSG_WARN([SDL not found])
else
  AC_DEFINE(USE_SDL, 1, Define to 1 to use SDL)
fi
AM_CONDITIONAL(USE_SDL, test "x${ac_cv_my_have_sdl}" = xyes)

if test "x${ac_cv_my_have_sdl_mixer}" = xno; then
  AC_MSG_WARN([SDL_mixer not found])
else
  AC_DEFINE(USE_SDL_MIXER, 1, Define to 1 to use SDL_mixer)
fi
AM_CONDITIONAL(USE_SDL_MIXER, test "x${ac_cv_my_have_sdl_mixer}" = xyes)

if test "x${ac_cv_my_have_sdl_image}" = xno; then
  AC_MSG_WARN([SDL_image not found])
else
  AC_DEFINE(USE_SDL_IMAGE, 1, Define to 1 to use SDL_image)
fi
AM_CONDITIONAL(USE_SDL_IMAGE, test "x${ac_cv_my_have_sdl_image}" = xyes)


])# LOL_CHECK_SDL


