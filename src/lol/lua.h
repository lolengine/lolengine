//
//  Lol Engine
//
//  Copyright © 2010—2017 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Lua subsystem
// -------------
//

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

// Three levels of dispatch are needed because of Visual Studio's bizarre
// handling of __VA_ARGS__ inside macro calls
#define LOL_CAT3(a, b) a##b
#define LOL_CAT2(a, b) LOL_CAT3(a,b)
#define LOL_CAT(a, b) LOL_CAT2(a,b)

// Lua
#include <lol/../lolua/baselua.h>
#include <lol/../easymesh/easymeshlua.h>

