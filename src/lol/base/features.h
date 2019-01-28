//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
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

#if defined EMSCRIPTEN
#   undef LOL_FEATURE_THREADS
#endif

/*
 * Check for C++11 and later features.
 */

/* These features aren't necessarily supported by all compilers */
#undef LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS
#undef LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
#undef LOL_FEATURE_CXX11_ISNAN /* FIXME: is this the right place? */
#undef LOL_FEATURE_CXX11_NULLPTR
#undef LOL_FEATURE_CXX11_TEMPLATE_ALIASES
#undef LOL_FEATURE_CXX11_SFINAE_FOR_CTORS

#undef LOL_FEATURE_CXX17_ATTRIBUTE_NODISCARD
#undef LOL_FEATURE_CXX17_ATTRIBUTE_FALLTHROUGH

/* Features detected through __has_cpp_attribute */
#ifdef __has_cpp_attribute
#   if __has_cpp_attribute(nodiscard)
#       define LOL_FEATURE_CXX17_ATTRIBUTE_NODISCARD 1
#   endif
#   if __has_cpp_attribute(fallthrough)
#       define LOL_FEATURE_CXX17_ATTRIBUTE_FALLTHROUGH 1
#   endif
#endif

/* Features supported by GCC */
#if defined __GNUC__
#   if !defined(__GXX_EXPERIMENTAL_CXX0X) && __cplusplus < 201103L
#       error "sorry, this version of GCC does not support constexpr"
#   endif
#   define LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS 1
#   define LOL_FEATURE_CXX11_ISNAN 1
#   define LOL_FEATURE_CXX11_ARRAY_INITIALIZERS 1
#   define LOL_FEATURE_CXX11_NULLPTR 1
#   define LOL_FEATURE_CXX11_SFINAE_FOR_CTORS 1
#   if (__GNUC__ * 100 + __GNUC_MINOR__) >= 470
#       define LOL_FEATURE_CXX11_TEMPLATE_ALIASES 1
#   endif
#endif

/* Features supported by Clang */
#if !defined __GNUC__ && defined __has_feature
#   define LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS 1
#   define LOL_FEATURE_CXX11_ARRAY_INITIALIZERS 1
#   if !__has_feature(cxx_constexpr)
#       error "sorry, this version of clang does not support constexpr"
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
#   if _MSC_VER < 1910
#       error "sorry, Visual Studio 2017 or later is needed"
#   endif
#   define LOL_FEATURE_CXX11_TEMPLATE_ALIASES 1
#   define LOL_FEATURE_CXX11_ISNAN 1
#   define LOL_FEATURE_CXX11_NULLPTR 1
#   define LOL_FEATURE_CXX11_SFINAE_FOR_CTORS 1
#   define LOL_FEATURE_CXX11_INHERIT_CONSTRUCTORS 1
#   define LOL_FEATURE_CXX11_ARRAY_INITIALIZERS 1
#endif


/*
 * Define some attribute macros.
 */

#ifdef __GNUC__
#   define LOL_ATTR_FORMAT(n, p) __attribute__((format(printf, n, p)))
#else
#   define LOL_ATTR_FORMAT(n, p)
#endif

#ifdef LOL_FEATURE_CXX17_ATTRIBUTE_NODISCARD
#   define LOL_ATTR_NODISCARD [[nodiscard]]
#else
#   define LOL_ATTR_NODISCARD /* */
#endif

#ifdef LOL_FEATURE_CXX17_ATTRIBUTE_FALLTHROUGH
#   define LOL_ATTR_FALLTHROUGH [[fallthrough]]
#else
#   define LOL_ATTR_FALLTHROUGH /* */
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


/* XXX: workaround for X11 headers that try to #define these */
#undef Always
#define Always Always
#undef None
#define None None

