//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The string tools
// ————————————————
// Contains some utilities to work with std::string objects.
//

#include <lol/base/features.h>
#include <lol/base/core.h>

#include <vector>
#include <string>
#include <algorithm> // std::transform
#include <cstdarg>   // va_list
#include <cctype>    // size_t

namespace lol
{

// Split a string along a single separator
template<typename T>
std::vector<std::basic_string<T>> split(std::basic_string<T> const &s,
                                        T sep = T('\n'))
{
    std::vector<std::basic_string<T>> ret;
    size_t start = 0, end = 0;
    while ((end = s.find(sep, start)) != std::basic_string<T>::npos)
    {
        ret.push_back(s.substr(start, end - start));
        start = end + 1;
    }
    ret.push_back(s.substr(start));
    return ret;
}

// Split a string along multiple separator
template<typename T>
std::vector<std::basic_string<T>> split(std::basic_string<T> const &s,
                                        std::basic_string<T> const &seps)
{
    std::vector<std::string> ret;
    size_t start = s.find_first_not_of(seps), end = 0;

    while ((end = s.find_first_of(seps, start)) != std::basic_string<T>::npos)
    {
        ret.push_back(s.substr(start, end - start));
        start = s.find_first_not_of(seps, end);
    }
    if (start != std::string::npos)
        ret.push_back(s.substr(start));

    return ret;

}

// Check whether a string starts or ends with a given substring
template<typename T>
bool starts_with(std::basic_string<T> const &s,
                 std::basic_string<T> const &prefix)
{
    return s.size() >= prefix.size() &&
           s.compare(0, prefix.size(), prefix) == 0;
}

template<typename T>
bool ends_with(std::basic_string<T> const &s,
               std::basic_string<T> const &suffix)
{
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// Convert a string to lowercase or uppercase
template<typename T>
std::basic_string<T> tolower(std::basic_string<T> const &s)
{
    std::string ret;
    std::transform(s.begin(), s.end(), std::back_inserter(ret),
                   [](T c){ return std::tolower(c); });
    return ret;
}

template<typename T>
std::basic_string<T> toupper(std::basic_string<T> const &s)
{
    std::string ret;
    std::transform(s.begin(), s.end(), std::back_inserter(ret),
                   [](T c){ return std::toupper(c); });
    return ret;
}

// Format a string, printf-style
template<typename T = char> LOL_ATTR_FORMAT(1, 2)
std::basic_string<T> format(T const *format, ...)
{
    va_list ap;
    va_start(ap, format);
    std::string ret = vformat(format, ap);
    va_end(ap);
    return ret;
}

template<typename T = char>
std::basic_string<T> vformat(char const *format, va_list ap)
{
    va_list ap2;
#if defined va_copy || !defined _MSC_VER
    // Visual Studio 2010 does not support va_copy.
    va_copy(ap2, ap);
#else
    ap2 = ap;
#endif

    // vsnprintf() tells us how many characters we need, not counting
    // the terminating null character.
    size_t needed = vsnprintf(nullptr, 0, format, ap2);

#if defined va_copy || !defined _MSC_VER
    // do not call va_end() if va_copy() wasn't called.
    va_end(ap2);
#endif

    std::string ret;
    ret.resize(needed);
    vsnprintf(&ret[0], needed + 1, format, ap);

    return ret;
}

} /* namespace lol */

