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
    *(uint32_t *)NULL = 0xdead;
#else
    std::abort();
#endif
}

#define LOL_CALL(macro, args) macro args
#define LOL_EVAL(a) a
#define LOL_1ST(a, ...) a

#define LOL_NUM2(a01, a02, a03, a04, a05, a06, a07, a08, a09, a10, \
                 a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, \
                 a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, \
                 a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, \
                 a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, \
                 a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, \
                 a61, a62, a63, ...) a63
#define LOL_NUM(...) \
    LOL_EVAL(LOL_NUM2(__VA_ARGS__, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
                                   2, 1, TOO_MANY_ARGUMENTS))

/* Three levels of dispatch are needed because of Visual Studio's bizarre
 * handling of __VA_ARGS__ inside macro calls */
#define LOL_CAT3(a, b) a##b
#define LOL_CAT2(a, b) LOL_CAT3(a,b)
#define LOL_CAT(a, b) LOL_CAT2(a,b)

#define LOL_ERROR_1(t) \
    Log::Error("assertion failure: " #t "\n")

#define LOL_ERROR_2(t, s) \
    Log::Error("assertion failure: %s\n", s)

#define LOL_ERROR_3(t, s, ...) \
    Log::Error("assertion failure: " s "\n", __VA_ARGS__)

#if FINAL_RELEASE
#   define ASSERT(...) (void)sizeof(LOL_CALL(LOL_1ST, (__VA_ARGS__)))
#else
#   define ASSERT(...) \
        if (!(LOL_CALL(LOL_1ST, (__VA_ARGS__)))) \
        { \
            LOL_CALL(LOL_CAT(LOL_ERROR_, LOL_CALL(LOL_NUM, (__VA_ARGS__))), \
                     (__VA_ARGS__)); \
            Abort(); \
        }
#endif

} /* namespace lol */

#endif // __LOL_BASE_ASSERT_H__

