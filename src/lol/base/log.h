//
//  Lol Engine
//
//  Copyright: © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The msg interface
// -----------------
// The central logging system.
//

#include <stdint.h>
#include <cstdarg>

namespace lol
{

class msg
{
public:
#ifdef __GNUC__
#   define LOL_FMT_ATTR(n, p) __attribute__((format(printf, n, p)))
#else
#   define LOL_FMT_ATTR(n, p)
#endif
    static void debug(char const *format, ...) LOL_FMT_ATTR(1, 2);
    static void info(char const *format, ...) LOL_FMT_ATTR(1, 2);
    static void warn(char const *format, ...) LOL_FMT_ATTR(1, 2);
    static void error(char const *format, ...) LOL_FMT_ATTR(1, 2);
#undef LOL_FMT_ATTR

private:
    enum class MessageType
    {
        Debug,
        Info,
        Warning,
        Error
    };

    static void helper(MessageType type, char const *fmt, va_list ap);
};

} /* namespace lol */

