//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
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
// ----------------
// Contains some utilities to work with std::string objects.
//

#include <string>

namespace lol
{

/* Split a string along a single separator */
array<std::string> split(std::string const &s, char sep = '\n');

/* Split a string along multiple separators */
array<std::string> split(std::string const &s, std::string const &seps);

/* Check whether a string starts or ends with a given substring */
bool starts_with(std::string const &s, std::string const &prefix);
bool ends_with(std::string const &s, std::string const &suffix);

/* Convert a string to lowercase or uppercase */
std::string tolower(std::string const &s);
std::string toupper(std::string const &s);

/* Format a string, printf-style */
std::string format(char const *format, ...) LOL_ATTR_FORMAT(1, 2);
std::string vformat(char const *format, va_list ap);

} /* namespace lol */

