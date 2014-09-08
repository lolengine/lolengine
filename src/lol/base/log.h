//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Log interface
// -----------------
// The central logging system.
//

#include <stdint.h>
#include <cstdarg>

namespace lol
{

class Log
{
public:
#ifdef __GNUC__
#   define LOL_FMT_ATTR(n, p) __attribute__((format(printf, n, p)))
#else
#   define LOL_FMT_ATTR(n, p)
#endif
    static void Debug(char const *format, ...) LOL_FMT_ATTR(1, 2);
    static void Info(char const *format, ...) LOL_FMT_ATTR(1, 2);
    static void Warn(char const *format, ...) LOL_FMT_ATTR(1, 2);
    static void Error(char const *format, ...) LOL_FMT_ATTR(1, 2);
#undef LOL_FMT_ATTR

private:
    enum MessageType
    {
        DebugMessage,
        InfoMessage,
        WarnMessage,
        ErrorMessage
    };

    static void Helper(MessageType type, char const *fmt, va_list ap);
};

} /* namespace lol */

