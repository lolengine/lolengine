//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdio>
#include <string>

#if defined(_WIN32)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#endif

#include <cstdarg>
#include <cctype>

namespace lol
{

array<std::string> split(std::string const &s, char sep)
{
    array<std::string> ret;
    size_t start = 0, end = 0;
    while ((end = s.find(sep, start)) != std::string::npos)
    {
        ret << s.substr(start, end - start);
        start = end + 1;
    }
    ret << s.substr(start);
    return ret;
}

array<std::string> split(std::string const &s, std::string const &seps)
{
    array<std::string> ret;
    size_t start = s.find_first_not_of(seps), end = 0;

    while ((end = s.find_first_of(seps, start)) != std::string::npos)
    {
        ret << s.substr(start, end - start);
        start = s.find_first_not_of(seps, end);
    }
    if (start != std::string::npos)
        ret << s.substr(start);

    return ret;
}

bool starts_with(std::string const &s, std::string const &prefix)
{
    return s.size() >= prefix.size() &&
           s.compare(0, prefix.size(), prefix) == 0;
}

bool ends_with(std::string const &s, std::string const &suffix)
{
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string tolower(std::string const &s)
{
    std::string ret;
    std::transform(s.begin(), s.end(), std::back_inserter(ret),
                   [](unsigned char c){ return std::tolower(c); });
    return ret;
}

std::string toupper(std::string const &s)
{
    std::string ret;
    std::transform(s.begin(), s.end(), std::back_inserter(ret),
                   [](unsigned char c){ return std::toupper(c); });
    return ret;
}

std::string format(char const *format, ...)
{
    va_list ap;
    va_start(ap, format);
    std::string ret = vformat(format, ap);
    va_end(ap);
    return ret;
}

std::string vformat(char const *format, va_list ap)
{
    va_list ap2;
#if defined va_copy || !defined _MSC_VER
    /* Visual Studio 2010 does not support va_copy. */
    va_copy(ap2, ap);
#else
    ap2 = ap;
#endif

    /* vsnprintf() tells us how many character we need, and we need to
     * add one for the terminating null byte. */
    size_t needed = vsnprintf(nullptr, 0, format, ap2) + 1;

#if defined va_copy || !defined _MSC_VER
    /* do not call va_end() if va_copy() wasn't called. */
    va_end(ap2);
#endif

    std::string ret;
    ret.resize(needed);
    vsnprintf(&ret[0], needed, format, ap);

    return ret;
}

} /* namespace lol */

