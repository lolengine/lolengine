//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The build-time features
// -----------------------
//

#if !defined __LOL_BASE_FEATURES_H__
#define __LOL_BASE_FEATURES_H__

/*
 * System and CPU features.
 */
#define LOL_FEATURE_THREADS 1
#define LOL_FEATURE_CHEAP_BRANCHES 1
#define LOL_FEATURE_VERY_CHEAP_BRANCHES 0

#if defined EMSCRIPTEN
#   undef LOL_FEATURE_THREADS
#   define LOL_FEATURE_THREADS 0
#endif

#if defined __CELLOS_LV2__
#   undef LOL_FEATURE_CHEAP_BRANCHES
#   define LOL_FEATURE_CHEAP_BRANCHES 0
#endif


/*
 * Check for C++11 features.
 */

/* Set these to 1 when Visual Studio finally understands the features
 * (planned for Visual Studion 14) */
#define LOL_FEATURE_CXX11_RELAXED_UNIONS 0
#define LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS 0
#define LOL_FEATURE_CXX11_ARRAY_INITIALIZERS 0
#define LOL_FEATURE_CXX11_CONSTEXPR 0
#define LOL_FEATURE_CXX11_ISNAN 0

/* This one is OK, except on GCC <= 4.6 */
#define LOL_FEATURE_CXX11_TEMPLATE_ALIASES 1

#if defined __GNUC__ /* GCC */
#   if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
#       undef LOL_FEATURE_CXX11_CONSTEXPR
#       define LOL_FEATURE_CXX11_CONSTEXPR 1
#       undef LOL_FEATURE_CXX11_ISNAN
#       define LOL_FEATURE_CXX11_ISNAN 1
#       undef LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
#       define LOL_FEATURE_CXX11_ARRAY_INITIALIZERS 1
#   endif
#   if (__GNUC__ * 100 + __GNUC_MINOR__) < 470
#       undef LOL_FEATURE_CXX11_TEMPLATE_ALIASES
#       define LOL_FEATURE_CXX11_TEMPLATE_ALIASES 0
#   endif
#elif defined __has_feature /* Clang */
#   if __has_feature(cxx_constexpr)
#       undef LOL_FEATURE_CXX11_CONSTEXPR
#       define LOL_FEATURE_CXX11_CONSTEXPR 1
#   endif
#   undef LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
#   define LOL_FEATURE_CXX11_ARRAY_INITIALIZERS 1
#endif


/*
 * Ensure we have nullptr.
 */

#if defined nullptr
    /* do nothing */
#elif defined __GNUC__
#   define nullptr __null
#else
#   include <cstddef>
#   define nullptr NULL
#endif


/*
 * Ensure we have ssize_t.
 */

#if defined ssize_t
    /* Do nothing, someone knows better than us it seems */
#elif HAVE_SYS_TYPES_H
#   include <sys/types.h>
#elif _MSC_VER /* Visual Studio compiler */
#   include <BaseTsd.h>
#   define _MSC_STDINT_H_
typedef SSIZE_T ssize_t;
#endif


/*
 * Ensure isnan() is present even on systems that don't define it, or
 * when -ffast-math is being used.
 */

#include <cmath>
#if defined __FAST_MATH__
#   undef isnan
#endif
#if !defined isnan && !LOL_FEATURE_CXX11_ISNAN
#   define isnan isnan
#   include <stdint.h>
static inline int isnan(float f)
{
    union { float f; uint32_t x; } u = { f };
    return (u.x << 1) > 0xff000000u;
}
#endif


/* XXX: workaround for a compilation bug in NaCl headers */
#if defined __native_client__
#   define typeid(x) (*(type_info*)nullptr)
#endif


/* XXX: workaround for X11 headers that try to #define these */
#undef Always
#define Always Always
#undef None
#define None None


#endif // __LOL_BASE_FEATURES_H__

