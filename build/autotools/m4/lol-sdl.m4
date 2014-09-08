
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
dnl    HAVE_SDL_SDL_H  -- whether to include <SDL/SDL.h>
dnl    HAVE_SDL_H      -- whether to include <SDL.h>
dnl  Generated automake conditionals:
dnl    USE_SDL         -- whether SDL or SDL2 were found at all
dnl    USE_SDL_IMAGE   -- whether SDL_image was found
dnl    USE_SDL_MIXER   -- whether SDL_mixer was found
dnl  Generated shell variables:
dnl    SDL_CFLAGS      -- flags for SDL compilation
dnl    SDL_LIBS        -- flags for SDL linking

ac_cv_my_have_sdl="no"
ac_cv_my_have_sdl2="no"
ac_cv_my_have_sdl_image="no"
ac_cv_my_have_sdl_mixer="no"

dnl  First, try the proper pkg-config check for SDL2
if test "x${ac_cv_my_have_sdl}" = xno; then
  PKG_CHECK_MODULES(SDL2, sdl2,
   [ac_cv_my_have_sdl="yes"
    ac_cv_my_have_sdl2="yes"
    PKG_CHECK_MODULES(SDL2MIXER, SDL2_mixer,
     [ac_cv_my_have_sdl_mixer="yes"
      AC_DEFINE(HAVE_SDL_MIXER_H, 1, Define to 1 to use SDL_mixer.h)],
     [:])
    PKG_CHECK_MODULES(SDL2IMAGE, SDL2_image,
     [ac_cv_my_have_sdl_image="yes"
      AC_DEFINE(HAVE_SDL_IMAGE_H, 1, Define to 1 to use SDL_image.h)],
     [:])
    SDL_CFLAGS="${SDL2_CFLAGS} ${SDL2MIXER_CFLAGS} ${SDL2IMAGE_CFLAGS}"
    SDL_LIBS="${SDL2_LIBS} ${SDL2MIXER_LIBS} ${SDL2IMAGE_LIBS}"
    AC_DEFINE(HAVE_SDL_H, 1, Define to 1 to use SDL.h)],
   [:])
fi

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
  if test "${SDL_CONFIG}" != "no" && "${SDL_CONFIG}" --version >/dev/null; then
    ac_cv_my_have_sdl="yes"
    SDL_CFLAGS="${SDL_CFLAGS} `${SDL_CONFIG} --cflags`"
    SDL_LIBS="${SDL_LIBS} `${SDL_CONFIG} --libs`"
    AC_DEFINE(HAVE_SDL_H, 1, Define to 1 to use SDL.h)
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
     [AC_CHECK_LIB(SDL2, main,
       [SDL_LIBS="${SDL_LIBS} -lSDL2"
        ac_cv_my_have_sdl2="yes"],
       [ac_cv_my_have_sdl="no"])])

    AC_CHECK_LIB(SDLmain, main,
     [SDL_LIBS="${SDL_LIBS} -lSDLmain -lSDL"])
    AC_CHECK_LIB(SDL2main, main,
     [SDL_LIBS="${SDL_LIBS} -lSDL2main -lSDL2"])

    AC_CHECK_LIB(SDL_mixer, main,
     [SDLMIXER_LIBS="${SDLMIXER_LIBS} -lSDL_mixer"],
     [AC_CHECK_LIB(SDL2_mixer, main,
       [SDLMIXER_LIBS="${SDLMIXER_LIBS} -lSDL2_mixer"],
       [ac_cv_my_have_sdl_mixer="no"])])

    AC_CHECK_LIB(SDL_image, main,
     [SDLIMAGE_LIBS="${SDLIMAGE_LIBS} -lSDL_image"],
     [AC_CHECK_LIB(SDL2_image, main,
       [SDLIMAGE_LIBS="${SDLIMAGE_LIBS} -lSDL2_image"],
       [ac_cv_my_have_sdl_image="no"])])
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

if test "x${ac_cv_my_have_sdl2}" != xno; then
  AC_DEFINE(USE_SDL2, 1, Define to 1 to use SDL)
fi
AM_CONDITIONAL(USE_SDL2, test "x${ac_cv_my_have_sdl2}" = xyes)

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


