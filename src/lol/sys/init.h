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
// Various system helper functions
// -------------------------------
//

#include <stdint.h>

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

namespace System
{

extern void Init(int argc, char *argv[],
                 String const &projectdir = LOL_CONFIG_PROJECTDIR,
                 String const &solutiondir = LOL_CONFIG_SOLUTIONDIR,
                 String const &sourcesubdir = LOL_CONFIG_SOURCESUBDIR);

extern void AddDataDir(String const &dir);
extern array<String> GetPathList(String const &file);

} /* namespace System */

} /* namespace lol */

