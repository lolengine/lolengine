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
// Various system helper functions
// -------------------------------
//

#include <string> // std::string

namespace lol
{

/*
 * Module-specific macros. These can be overridden by the build process,
 * typically with compiler command-line flags.
 */

#if !defined LOL_CONFIG_PROJECTDIR
#   define LOL_CONFIG_PROJECTDIR ""
#endif

#if !defined LOL_CONFIG_SOLUTIONDIR
#   define LOL_CONFIG_SOLUTIONDIR ""
#endif

#if !defined LOL_CONFIG_SOURCESUBDIR
#   define LOL_CONFIG_SOURCESUBDIR ""
#endif

/*
 * System namespace. The platform-specific stuff in there makes the API
 * not as clean as the rest of the framework.
 */

namespace sys
{

extern void init(int argc, char *argv[],
                 std::string const &projectdir = LOL_CONFIG_PROJECTDIR,
                 std::string const &solutiondir = LOL_CONFIG_SOLUTIONDIR,
                 std::string const &sourcesubdir = LOL_CONFIG_SOURCESUBDIR);

extern void add_data_dir(std::string const &dir);
extern std::string get_data_path(std::string const &file);

} /* namespace sys */

} /* namespace lol */

