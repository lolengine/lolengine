//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cctype>

#if HAVE_UNISTD_H
#   include <unistd.h>
#endif

#if _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <direct.h>
#   undef WIN32_LEAN_AND_MEAN
#endif

namespace lol
{

namespace sys
{

#if _WIN32
#   define SEPARATOR '\\'
#else
#   define SEPARATOR '/'
#endif

static array<std::string> data_dir;

void init(int argc, char *argv[],
          std::string const &projectdir,
          std::string const &solutiondir,
          std::string const &sourcesubdir)
{
    using namespace std;

    msg::debug("project dir: “%s”\n", projectdir.c_str());
    msg::debug("solution dir: “%s”\n", solutiondir.c_str());
    msg::debug("source subdir: “%s”\n", sourcesubdir.c_str());

    /*
     * Retrieve binary directory, defaulting to no directory on Android
     * and emscripten, and the current directory on other platforms.
     */

#if __ANDROID__ || EMSCRIPTEN
    std::string binarydir = "";
#else
    std::string binarydir = ".";
    char *cwd = nullptr;

#   if HAVE_GETCWD
    cwd = getcwd(nullptr, 0);
#   elif HAVE__GETCWD || _WIN32
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
            binarydir = std::string(argv[0], last_sep - argv[0] + 1);
    }
#endif

    bool got_rootdir = false;

    /*
     * If project dir and solution dir are set, add them to data; also
     * add current directory in case we were launched from another place.
     */

    if (!got_rootdir && projectdir.length() && solutiondir.length())
    {
        /* This data dir is for standalone executables */
        std::string rootdir = binarydir;
        if (rootdir.length() && rootdir.back() != SEPARATOR)
            rootdir += SEPARATOR;
        add_data_dir(rootdir);

        /* This data dir is for engine stuff */
        rootdir = solutiondir;
        if (rootdir.length() && rootdir.back() != SEPARATOR)
            rootdir += SEPARATOR;
        rootdir += "../src/"; /* FIXME: use SEPARATOR? */
        add_data_dir(rootdir);

        /* This data dir is for submodule support stuff */
        rootdir = solutiondir;
        if (rootdir.length() && rootdir.back() != SEPARATOR)
            rootdir += SEPARATOR;
        rootdir += "./lol/src/"; /* FIXME: use SEPARATOR? */
        add_data_dir(rootdir);

        /* This data dir is for project-specific stuff */
        rootdir = projectdir;
        if (rootdir.length() && rootdir.back() != SEPARATOR)
            rootdir += SEPARATOR;
        add_data_dir(rootdir);

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
        std::string rootdir = binarydir;
        if (rootdir.length() && rootdir.back() != SEPARATOR)
            rootdir += SEPARATOR;
        for (int i = 1; i < (int)sourcesubdir.length(); ++i)
        {
            if ((sourcesubdir[i] == SEPARATOR
                  && sourcesubdir[i - 1] != SEPARATOR)
                 || i == (int)sourcesubdir.length() - 1)
                rootdir += "../";
        }
        rootdir += "src/";
        add_data_dir(rootdir);

        /* This data dir is for project-specific stuff */
        rootdir = binarydir;
        add_data_dir(rootdir);

        got_rootdir = true;
    }

    msg::debug("binary dir: “%s”\n", binarydir.c_str());
    for (int i = 0; i < data_dir.count(); ++i)
        msg::debug("data dir %d/%d: “%s”\n", i + 1, data_dir.count(),
                   data_dir[i].c_str());
}

/*
 * Data directory handling
 */

void add_data_dir(std::string const &dir)
{
    data_dir << dir;
}

array<std::string> get_path_list(std::string const &file)
{
    array<std::string> ret;

    /* If not an absolute path, look through known data directories */
    if (file[0] != '/')
    {
        for (int i = 0; i < data_dir.count(); ++i)
            ret << data_dir[i] + file;
    }

    ret << file;

    return ret;
}

} /* namespace sys */

} /* namespace lol */

