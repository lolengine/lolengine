//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <direct.h>
#endif

#include "core.h"

namespace lol
{

namespace System
{

#if defined _WIN32
#   define SEPARATOR '\\'
#else
#   define SEPARATOR '/'
#endif

void Init(int argc, char *argv[],
          String const projectdir, String const solutiondir)
{
    /*
     * Retrieve binary directory, defaulting to current dir.
     */

#if defined _WIN32
    char const *cwd = _getcwd(NULL, 0);
    String binarydir = String(cwd ? cwd : ".") + SEPARATOR;
    free((void *)cwd);
#else
    String binarydir = String(getcwd()) + SEPARATOR;
#endif
    if (argc > 0)
    {
        char const *last_sep = strchr(argv[0], SEPARATOR);
        if (last_sep)
            binarydir = String(argv[0], last_sep - argv[0] + 1);
    }

    /*
     * Find the common prefix between project dir and solution dir.
     */

    bool got_rootdir = false;
    for (int i = 0; ; i++)
    {
        if (projectdir[i] != solutiondir[i] || projectdir[i] == '\0')
        {
            /* FIXME: at this point we should check whether the binary
             * was launched from this subdirectory; from now we just
             * assume it was. */
            if (i)
            {
                String rootdir = projectdir;
                if (rootdir.Last() != SEPARATOR)
                    rootdir += SEPARATOR;
                SetDataDir(&rootdir[0]);
                got_rootdir = true;
            }
            break;
        }
    }

    /* If no rootdir found, use the executable location */
    if (!got_rootdir)
    {
        SetDataDir(&binarydir[0]);
        got_rootdir = true;
    }
}

/*
 * Data directory handling
 */

String data_dir = "";

void SetDataDir(char const *dir)
{
    data_dir = dir;
}

char const *GetDataDir()
{
    return &data_dir[0];
}


} /* namespace System */

} /* namespace lol */

