//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The build-time features
// -----------------------
//

/*
 * System, CPU and compiler features.
 */

#define LOL_FEATURE_THREADS 1
#define LOL_FEATURE_CHEAP_BRANCHES 1
#define LOL_FEATURE_VERY_CHEAP_BRANCHES 0
#define LOL_FEATURE_VISUAL_STUDIO_THAT_FUCKING_PIECE_OF_SHIT_COMPILER 0

#if defined EMSCRIPTEN
#   undef LOL_FEATURE_THREADS
#endif

/* Use this to disable code that causes compiler crashes. */
#if defined _MSC_VER
#   undef LOL_FEATURE_VISUAL_STUDIO_THAT_FUCKING_PIECE_OF_SHIT_COMPILER
#   define LOL_FEATURE_VISUAL_STUDIO_THAT_FUCKING_PIECE_OF_SHIT_COMPILER 1
#endif


/*
 * Check for C++11 features.
 */

/* These features aren't necessarily supported by all compilers */
#undef LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
#undef LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
#undef LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
#undef LOL_FEATURE_CXX11_CONSTEXPR
#undef LOL_FEATURE_CXX11_ISNAN /* FIXME: is this the right place? */
#undef LOL_FEATURE_CXX11_NULLPTR
#undef LOL_FEATURE_CXX11_TEMPLATE_ALIASES
#undef LOL_FEATURE_CXX11_SFINAE_FOR_CTORS

/* Features supported by GCC */
#if defined __GNUC__
#   define LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS 1
#   define LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS 1
#   if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
#       define LOL_FEATURE_CXX11_CONSTEXPR 1
#       define LOL_FEATURE_CXX11_ISNAN 1
#       define LOL_FEATURE_CXX11_ARRAY_INITIALIZERS 1
#       define LOL_FEATURE_CXX11_NULLPTR 1
#       define LOL_FEATURE_CXX11_SFINAE_FOR_CTORS 1
#   endif
#   if (__GNUC__ * 100 + __GNUC_MINOR__) >= 470
#       define LOL_FEATURE_CXX11_TEMPLATE_ALIASES 1
#   endif
#endif

/* Features supported by Clang */
#if !defined __GNUC__ && defined __has_feature
#   define LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS 1
#   define LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS 1
#   define LOL_FEATURE_CXX11_ARRAY_INITIALIZERS 1
#   if __has_feature(cxx_constexpr)
#       define LOL_FEATURE_CXX11_CONSTEXPR 1
#   endif
#   define LOL_FEATURE_CXX11_ISNAN 1
#   if __has_feature(cxx_nullptr)
#       define LOL_FEATURE_CXX11_NULLPTR 1
#   endif
#   define LOL_FEATURE_CXX11_TEMPLATE_ALIASES 1
#   define LOL_FEATURE_CXX11_SFINAE_FOR_CTORS 1
#endif

/* Features supported by Visual Studio */
#if defined _MSC_VER
#   define LOL_FEATURE_CXX11_TEMPLATE_ALIASES 1
#   define LOL_FEATURE_CXX11_ISNAN 1
#   if _MSC_VER >= 1900 /* 2015 CTP (not too bad) */
#       define LOL_FEATURE_CXX11_NULLPTR 1
#       define LOL_FEATURE_CXX11_SFINAE_FOR_CTORS 1
#       define LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS 1
#   endif
#   if _MSC_VER < 1800 /* 2012 or older (ugly piece of shit) */
#       error "sorry, Visual Studio 2013 or later is needed"
#   endif
    /* Supported in VS 2015 but causes massive warning output */
#   undef LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
    /* Still unsupported as of VS 2015 */
#   undef LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
#   undef LOL_FEATURE_CXX11_CONSTEXPR
#endif


/*
 * Ensure we have ptrdiff_t.
 */

#include <cstddef>


/*
 * Ensure we have nullptr.
 */

#if !LOL_FEATURE_CXX11_NULLPTR
#   if defined nullptr
        /* do nothing */
#   elif defined __GNUC__
#       define nullptr __null
#   else
#       include <cstddef>
#       define nullptr NULL
#   endif
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


/*
 * A handy endianness test function
 */

namespace lol
{
    static inline bool is_big_endian()
    {
        union { int i; char c; } u;
        u.i = 1;
        return u.c == 0;
    }
}


/* XXX: workaround for a compilation bug in NaCl headers */
#if defined __native_client__
#   define typeid(x) (*(type_info*)nullptr)
#endif


/* XXX: workaround for X11 headers that try to #define these */
#undef Always
#define Always Always
#undef None
#define None None

