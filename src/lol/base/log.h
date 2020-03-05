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

//
// The msg interface
// -----------------
// The central logging system.
//

#include <lol/base/private/features.h>

#include <cstdarg> // va_list

namespace lol
{

class msg
{
public:
    static void debug(char const *format, ...) lol_attr_printf_format(1, 2);
    static void info(char const *format, ...)  lol_attr_printf_format(1, 2);
    static void warn(char const *format, ...)  lol_attr_printf_format(1, 2);
    static void error(char const *format, ...) lol_attr_printf_format(1, 2);

    enum class message_type
    {
        debug,
        info,
        warning,
        error,
    };

private:
    static void helper(message_type type, char const *fmt, va_list ap);
};

} /* namespace lol */

