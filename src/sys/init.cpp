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

static Array<String> data_dir;

void Init(int argc, char *argv[],
          String const &projectdir,
          String const &solutiondir,
          String const &sourcesubdir)
{
    using namespace std;

    /*
     * Retrieve binary directory, defaulting to current dir.
     */

#if __ANDROID__
    /* Android assets are accessed using no prefix at all. */
    String binarydir = "";
#elif defined HAVE_GETCWD
    char *cwd = getcwd(nullptr, 0);
    String binarydir = String(cwd ? cwd : ".") + SEPARATOR;
    free(cwd);
#elif defined HAVE__GETCWD || (defined _WIN32 && !defined _XBOX)
    char *cwd = _getcwd(nullptr, 0);
    String binarydir = String(cwd ? cwd : ".") + SEPARATOR;
    free(cwd);
#else
    String binarydir = "./";
#endif

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
                String rootdir = solutiondir;
                if (rootdir.Last() != SEPARATOR)
                    rootdir += SEPARATOR;
                rootdir += "../../src/"; /* FIXME: use SEPARATOR? */
                AddDataDir(rootdir);

                rootdir = projectdir;
                if (rootdir.Last() != SEPARATOR)
                    rootdir += SEPARATOR;
                AddDataDir(rootdir);

                got_rootdir = true;
            }
            break;
        }
    }

    /* If no rootdir found, use the executable location */
    if (!got_rootdir)
    {
        String rootdir = binarydir;
        if (rootdir.Count() && rootdir.Last() != SEPARATOR)
            rootdir += SEPARATOR;
        for (int i = 1; i < sourcesubdir.Count(); ++i)
        {
            if ((sourcesubdir[i] == SEPARATOR
                  && sourcesubdir[i - 1] != SEPARATOR)
                 || i == sourcesubdir.Count() - 1)
                rootdir += "../";
        }
        rootdir += "src/";
        AddDataDir(rootdir);

        rootdir = binarydir;
        AddDataDir(rootdir);

        got_rootdir = true;
    }

    Log::Debug("binary dir: “%s”\n", binarydir.C());
    for (int i = 0; i < data_dir.Count(); ++i)
        Log::Debug("data dir %d/%d: “%s”\n", i + 1, data_dir.Count(),
                   data_dir[i].C());
}

/*
 * Data directory handling
 */

void AddDataDir(String const &dir)
{
    data_dir << dir;
}

Array<String> GetPathList(String const &file)
{
    Array<String> ret;

    for (int i = 0; i < data_dir.Count(); ++i)
        ret << data_dir[i] + file;

    if (ret.Count() == 0)
        ret << file;

    return ret;
}

} /* namespace System */

} /* namespace lol */

