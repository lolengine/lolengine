//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_BASE_ASSERT_H__
#define __LOL_BASE_ASSERT_H__

#include <cstdlib>

namespace lol
{

static inline void Abort()
{
#if defined __CELLOS_LV2__
    *(uint32_t *)nullptr = 0xdead;
#else
    std::abort();
#endif
}

extern void DumpStack();

/* FIXME: see http://stackoverflow.com/q/3596781/111461 for discussions
 * on implementing __debugbreak() on POSIX systems. */
static inline void DebugAbort()
{
    lol::DumpStack();
#if defined _WIN32
    __debugbreak();
#endif
    lol::Abort();
}

#define LOL_CALL(macro, args) macro args
#define LOL_EVAL(a) a
#define LOL_1ST(a, ...) a

#define LOL_GET_63RD(a01, a02, a03, a04, a05, a06, a07, a08, a09, a10, \
                     a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, \
                     a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, \
                     a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, \
                     a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, \
                     a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, \
                     a61, a62, a63, ...) a63
#define LOL_COUNT_TO_3(...) \
    LOL_EVAL(LOL_GET_63RD(__VA_ARGS__, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                       3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                       3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                       3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                       3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                       3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                       2, 1, TOO_FEW_ARGUMENTS))
#define LOL_COUNT_TO_8(...) \
    LOL_EVAL(LOL_GET_63RD(__VA_ARGS__, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, \
                                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, \
                                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, \
                                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, \
                                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, \
                                       8, 8, 8, 8, 8, 7, 6, 5, 4, 3, \
                                       2, 1, TOO_FEW_ARGUMENTS))

#define LOL_COUNT_TO_12(...) \
    LOL_EVAL(LOL_GET_63RD(__VA_ARGS__, 12,12,12,12,12,12,12,12,12,12,\
                                       12,12,12,12,12,12,12,12,12,12,\
                                       12,12,12,12,12,12,12,12,12,12,\
                                       12,12,12,12,12,12,12,12,12,12,\
                                       12,12,12,12,12,12,12,12,12,12,\
                                       12,11,10,9, 8, 7, 6, 5, 4, 3, \
                                       2, 1, TOO_FEW_ARGUMENTS))

/* Three levels of dispatch are needed because of Visual Studio's bizarre
 * handling of __VA_ARGS__ inside macro calls */
#define LOL_CAT3(a, b) a##b
#define LOL_CAT2(a, b) LOL_CAT3(a,b)
#define LOL_CAT(a, b) LOL_CAT2(a,b)

/*
 * UNUSED(): declare one or several variables as unused
 */

#define LOL_UNUSED_1(a01) \
    sizeof(a01)
#define LOL_UNUSED_2(a01, a02) \
    sizeof(a01) + LOL_UNUSED_1(a02)
#define LOL_UNUSED_3(a01, a02, a03) \
    sizeof(a01) + LOL_UNUSED_2(a02, a03)
#define LOL_UNUSED_4(a01, a02, a03, a04) \
    sizeof(a01) + LOL_UNUSED_3(a02, a03, a04)
#define LOL_UNUSED_5(a01, a02, a03, a04, a05) \
    sizeof(a01) + LOL_UNUSED_4(a02, a03, a04, a05)
#define LOL_UNUSED_6(a01, a02, a03, a04, a05, a06) \
    sizeof(a01) + LOL_UNUSED_5(a02, a03, a04, a05, a06)
#define LOL_UNUSED_7(a01, a02, a03, a04, a05, a06, a07) \
    sizeof(a01) + LOL_UNUSED_6(a02, a03, a04, a05, a06, a07)
#define LOL_UNUSED_8(a01, a02, a03, a04, a05, a06, a07, a08) \
    sizeof(a01) + LOL_UNUSED_7(a02, a03, a04, a05, a06, a07, a08)

#define UNUSED(...) (void)sizeof((void)( \
    LOL_CALL(LOL_CAT(LOL_UNUSED_, LOL_CALL(LOL_COUNT_TO_8, (__VA_ARGS__))), \
             (__VA_ARGS__))), 0) \

/*
 * ASSERT(): test and enforce conditions at runtime
 */

#define LOL_ERROR_1(t) \
    lol::Log::Error("assertion at %s:%d: %s\n", __FILE__, __LINE__, #t)

#define LOL_ERROR_2(t, s) \
    lol::Log::Error("assertion at %s:%d: %s\n", __FILE__, __LINE__, s)

#define LOL_ERROR_3(t, s, ...) \
    lol::Log::Error("assertion at %s:%d: " s "\n", __FILE__, __LINE__, __VA_ARGS__)

#if LOL_BUILD_RELEASE
#   define ASSERT(...) UNUSED(LOL_CALL(LOL_1ST, (__VA_ARGS__)))
#else
#   define ASSERT(...) \
        if (LOL_CALL(LOL_1ST, (__VA_ARGS__))) \
        { \
            /* Don't use "!" so that ASSERT(x = 1) instead of ASSERT(x == 1)
             * triggers a compilation warning. */ \
        } \
        else \
        { \
            LOL_CALL(LOL_CAT(LOL_ERROR_, LOL_CALL(LOL_COUNT_TO_3, \
                                                  (__VA_ARGS__))), \
                     (__VA_ARGS__)); \
            lol::DebugAbort(); \
        }
#endif

} /* namespace lol */

#endif // __LOL_BASE_ASSERT_H__

