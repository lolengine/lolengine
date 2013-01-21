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
          char const *projectdir, char const *solutiondir)
{
    bool got_rootdir = false;

    /* Find the common prefix between project dir and solution dir. */
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

    /* Try to guess the data directory from the executable location. */
    if (!got_rootdir && argc > 0)
    {
        char const *last_slash = strrchr(argv[0], SEPARATOR);

        if (last_slash)
        {
            String dir(argv[0], last_slash - argv[0] + 1);
            SetDataDir(&dir[0]);
            got_rootdir = true;
        }
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

