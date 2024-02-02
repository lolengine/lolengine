//
//  Lol Engine
//
//  Copyright © 2010–2024 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine/sys>
#include <lol/msg>

#include <vector> // std::vector
#include <string> // std::string
#include <cctype>
#if !__ANDROID__ && !__EMSCRIPTEN__ && !__NX__
#   include <filesystem> // std::filesystem::exists
#endif

#include <kinc/system.h>

namespace lol::sys
{

#if _WIN32
#   define SEPARATOR '\\'
#else
#   define SEPARATOR '/'
#endif

static std::vector<std::string> data_dir;

void init(int argc, char *argv[],
          std::string const &name, int width, int height,
          std::string const &projectdir,
          std::string const &solutiondir,
          std::string const &sourcesubdir)
{
    using namespace std;

#if LOL_USE_KINC
    kinc_init(name.c_str(), width, height, nullptr, nullptr);
#endif

    msg::debug("project dir: “%s”\n", projectdir.c_str());
    msg::debug("solution dir: “%s”\n", solutiondir.c_str());
    msg::debug("source subdir: “%s”\n", sourcesubdir.c_str());

    //
    // Retrieve binary directory, defaulting to no directory on Android
    // and emscripten, and the current directory on other platforms.
    //

#if __ANDROID__ || __EMSCRIPTEN__ || __NX__
    std::string binarydir = "";
#else
    std::string binarydir = std::filesystem::current_path().string() + SEPARATOR;

    if (argc > 0)
    {
        char const *last_sep = strrchr(argv[0], SEPARATOR);
        if (last_sep)
            binarydir = std::string(argv[0], last_sep - argv[0] + 1);
    }
#endif

    bool got_rootdir = false;

    //
    // If project dir and solution dir are set, add them to data; also
    // add current directory in case we were launched from another place.
    //

    if (!got_rootdir && projectdir.length() && solutiondir.length())
    {
        // This data dir is for standalone executables
        std::string rootdir = binarydir;
#if !__NX__
        if (rootdir.length() && rootdir.back() != SEPARATOR)
            rootdir += SEPARATOR;
        add_data_dir(rootdir);
#endif

        // This data dir is for engine stuff
        rootdir = solutiondir;
        if (rootdir.length() && rootdir.back() != SEPARATOR)
            rootdir += SEPARATOR;
        rootdir += "../src/"; // FIXME: use SEPARATOR?
        add_data_dir(rootdir);

        // This data dir is for submodule support stuff
        rootdir = solutiondir;
        if (rootdir.length() && rootdir.back() != SEPARATOR)
            rootdir += SEPARATOR;
        rootdir += "./lol/src/"; // FIXME: use SEPARATOR?
        add_data_dir(rootdir);

        // This data dir is for project-specific stuff
        rootdir = projectdir;
        if (rootdir.length() && rootdir.back() != SEPARATOR)
            rootdir += SEPARATOR;
        add_data_dir(rootdir);

        got_rootdir = true;
    }

    //
    // If no project dir, use the executable location as the starting point
    // to guess the data dir.
    //
    if (!got_rootdir)
    {
        // First climb back the hierarchy to get to the engine root and
        // add a data dir for engine stuff.
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

        // This data dir is for project-specific stuff
        rootdir = binarydir;
        add_data_dir(rootdir);

        got_rootdir = true;
    }

    msg::debug("binary dir: “%s”\n", binarydir.c_str());
    for (int i = 0; i < int(data_dir.size()); ++i)
        msg::debug("data dir %d/%d: “%s”\n", i + 1, int(data_dir.size()),
                   data_dir[i].c_str());
}

void run()
{
#if LOL_USE_KINC
    kinc_start();
#endif
}

void stop()
{
#if LOL_USE_KINC
    kinc_stop();
#endif
}

//
// Data directory handling
//

void add_data_dir(std::string const &dir)
{
    data_dir.push_back(dir);
}

std::string get_data_path(std::string const &file)
{
    // If not an absolute path, look through known data directories
    if (file[0] != '/')
        for (auto const &dir : data_dir)
        {
            auto path = dir + file;
#if !__ANDROID__ && !__EMSCRIPTEN__ && !__NX__
            std::error_code ec;
            if (std::filesystem::exists(path, ec))
                return path;
#endif
        }

    return file;
}

} // namespace lol::sys
