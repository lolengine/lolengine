//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// Various system helper functions
// -------------------------------
//

#if !defined __LOL_SYS_INIT_H__
#define __LOL_SYS_INIT_H__

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

/*
 * System namespace. The platform-specific stuff in there makes the API
 * not as clean as the rest of the framework.
 */

namespace System
{

extern void Init(int argc, char *argv[],
                 String const &projectdir = LOL_CONFIG_PROJECTDIR,
                 String const &solutiondir = LOL_CONFIG_SOLUTIONDIR);

extern void AddDataDir(String const &dir);
extern Array<String> GetPathList(String const &file);

} /* namespace System */

} /* namespace lol */

#endif // __LOL_SYS_INIT_H__

