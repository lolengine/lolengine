//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Log interface
// -----------------
// The central logging system.
//

#if !defined __DH_LOG_H__
#define __DH_LOG_H__

#include <stdint.h>

namespace lol
{

class Log
{
public:
#ifdef __GNUC__
#   define LOL_FMT_ATTR __attribute__((format(printf, 1, 2)))
#endif
    static void Debug(char const *format, ...) LOL_FMT_ATTR;
    static void Info(char const *format, ...) LOL_FMT_ATTR;
    static void Warn(char const *format, ...) LOL_FMT_ATTR;
    static void Error(char const *format, ...) LOL_FMT_ATTR;
#undef LOL_FMT_ATTR
};

} /* namespace lol */

#endif // __DH_LOG_H__

