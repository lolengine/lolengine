//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cctype>

#if HAVE_UNISTD_H
#   include <unistd.h>
#endif

#if _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <direct.h>
#endif

#include "core.h"

namespace lol
{

namespace System
{

#if _WIN32
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

    Log::Debug("project dir: “%s”\n", projectdir.C());
    Log::Debug("solution dir: “%s”\n", solutiondir.C());
    Log::Debug("source subdir: “%s”\n", sourcesubdir.C());

    /*
     * Retrieve binary directory, defaulting to no directory on Android
     * and emscripten, and the current directory on other platforms.
     */

#if __ANDROID__ || EMSCRIPTEN
    String binarydir = "";
#else
    String binarydir = ".";
    char *cwd = nullptr;

#   if HAVE_GETCWD
    cwd = getcwd(nullptr, 0);
#   elif HAVE__GETCWD || (_WIN32 && !_XBOX)
    cwd = _getcwd(nullptr, 0);
#   endif

    if (cwd)
    {
        binarydir = cwd;
        free(cwd);
    }

    binarydir += SEPARATOR;

    if (argc > 0)
    {
        char const *last_sep = strrchr(argv[0], SEPARATOR);
        if (last_sep)
            binarydir = String(argv[0], last_sep - argv[0] + 1);
    }
#endif

    bool got_rootdir = false;

    /*
     * If project dir and solution dir are set, add them to data; also
     * add current directory in case we were launched from another place.
     */

    if (!got_rootdir && projectdir.Count() && solutiondir.Count())
    {
        /* This data dir is for standalone executables */
        String rootdir = binarydir;
        if (rootdir.Count() && rootdir.Last() != SEPARATOR)
            rootdir += SEPARATOR;
        AddDataDir(rootdir);

        /* This data dir is for engine stuff */
        rootdir = solutiondir;
        if (rootdir.Count() && rootdir.Last() != SEPARATOR)
            rootdir += SEPARATOR;
        rootdir += "../../src/"; /* FIXME: use SEPARATOR? */
        AddDataDir(rootdir);

        /* This data dir is for project-specific stuff */
        rootdir = projectdir;
        if (rootdir.Count() && rootdir.Last() != SEPARATOR)
            rootdir += SEPARATOR;
        AddDataDir(rootdir);

        got_rootdir = true;
    }

    /*
     * If no project dir, use the executable location as the starting point
     * to guess the data dir.
     */
    if (!got_rootdir)
    {
        /* First climb back the hierarchy to get to the engine root and
         * add a data dir for engine stuff. */
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

        /* This data dir is for project-specific stuff */
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

