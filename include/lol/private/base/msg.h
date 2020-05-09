//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#if !_DEBUG
#include "../base/env.h" // sys::getenv
#endif

#include <cstdio> // std::vfprintf

namespace lol
{

struct msg
{
    template<typename... T>
    static inline void debug(char const *fmt, T... args)
    {
        helper(message_type::debug, fmt, args...);
    }

    template<typename... T>
    static inline void info(char const *fmt, T... args)
    {
        helper(message_type::info, fmt, args...);
    }

    template<typename... T>
    static inline void warn(char const *fmt, T... args)
    {
        helper(message_type::warn, fmt, args...);
    }

    template<typename... T>
    static inline void error(char const *fmt, T... args)
    {
        helper(message_type::error, fmt, args...);
    }

private:
    msg() = delete;

    enum class message_type
    {
        debug,
        info,
        warn,
        error,
    };

    static void helper(message_type type, char const *fmt, ...)
    {
        // Unless this is a debug build, ignore debug messages unless
        // the LOL_DEBUG environment variable is set.
#if !_DEBUG
        if (type == message_type::debug)
        {
            static bool const disable_debug = sys::getenv("LOL_DEBUG").size() > 0;
            if (disable_debug)
                return;
        }
#endif

        static char const * const prefix[] =
        {
            "DEBUG",
            "INFO",
            "WARN",
            "ERROR",
        };

        fprintf(stdout, "%s: ", prefix[int(type)]);
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stdout, fmt, ap);
        va_end(ap);
        fflush(stdout);
    }
};

} // namespace lol

