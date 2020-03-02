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

#include <string>   // std::string
#include <cstdlib>  // _dupenv_s / std::getenv

namespace lol
{

namespace os
{

static inline std::string getenv(std::string const &var)
{
#if _MSC_VER
    char *buf = nullptr;
    size_t count = 0;
    if (_dupenv_s(&buf, &count, var.c_str()) == 0 && buf)
    {
        std::string ret(buf);
        free(buf);
        return ret;
    }
#else
    if (auto val = std::getenv(var.c_str()))
        return std::string(val);
#endif
    return std::string();
}

// A handy endianness test function
static inline bool is_big_endian()
{
    union { int i; char c; } u;
    u.i = 1;
    return u.c == 0;
}

} // namespace os

} // namespace lol
