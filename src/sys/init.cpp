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

#if defined HAVE_UNISTD_H
#   include <unistd.h>
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
          String const &projectdir, String const &solutiondir)
{
    using namespace std;

    /*
     * Retrieve binary directory, defaulting to current dir.
     */

#if defined HAVE_GETCWD
    char *cwd = getcwd(NULL, 0);
#elif defined HAVE__GETCWD || (defined _WIN32 && !defined _XBOX)
    char *cwd = _getcwd(NULL, 0);
#else
    char *cwd = NULL;
#endif
    String binarydir = String(cwd ? cwd : ".") + SEPARATOR;
    free(cwd);
    if (argc > 0)
    {
        char const *last_sep = strrchr(argv[0], SEPARATOR);
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

    Log::Debug("binary dir: %s\n", &binarydir[0]);
    Log::Debug("root dir: %s\n", &GetDataDir()[0]);
}

/*
 * Data directory handling
 */

String data_dir = "";

void SetDataDir(String const &dir)
{
    data_dir = dir;
}

String const &GetDataDir()
{
    return data_dir;
}

} /* namespace System */

} /* namespace lol */

