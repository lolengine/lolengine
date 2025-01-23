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

#include <algorithm> // std::remove_if
#if !defined _LIBCPP_HAS_NO_FILESYSTEM_LIBRARY && !defined _LIBCPP_AVAILABILITY_HAS_NO_FILESYSTEM_LIBRARY
#   include <filesystem> // std::filesystem::exists
#endif
#include <lol/engine/audio> // lol::audio
#include <lol/engine/sys> // lol::sys
#include <lol/msg> // lol::msg
#include <string> // std::string
#include <vector> // std::vector

#if _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef WIN32_LEAN_AND_MEAN
#endif

#include <kinc/graphics4/graphics.h>
#include <kinc/system.h>

namespace lol::sys
{

#if !defined _LIBCPP_HAS_NO_FILESYSTEM_LIBRARY && !defined _LIBCPP_AVAILABILITY_HAS_NO_FILESYSTEM_LIBRARY
static std::vector<std::filesystem::path> data_dir;
#endif

static std::vector<std::function<int()>> g_callbacks;

void init(int argc, char *argv[],
          std::string const &name, int width, int height,
          std::string const &projectdir,
          std::string const &solutiondir,
          std::string const &sourcesubdir)
{
    using namespace std;

#if !defined _LIBCPP_HAS_NO_FILESYSTEM_LIBRARY && !defined _LIBCPP_AVAILABILITY_HAS_NO_FILESYSTEM_LIBRARY
    msg::debug("project dir: “%s”\n", std::filesystem::path(projectdir).generic_string().c_str());
    msg::debug("solution dir: “%s”\n", std::filesystem::path(solutiondir).generic_string().c_str());
    msg::debug("source subdir: “%s”\n", std::filesystem::path(sourcesubdir).generic_string().c_str());

    //
    // Retrieve the binary directory, defaulting to current directory if no
    // reasonable guess can be made.
    //

    auto binarydir = argc > 0 ? std::filesystem::path(argv[0]).parent_path()
                              : std::filesystem::current_path();
    bool got_rootdir = false;

    //
    // If project dir and solution dir are set, add them to data; also
    // add current directory in case we were launched from another place.
    //

    if (!got_rootdir && projectdir.length() && solutiondir.length())
    {
        // This data dir is for standalone executables
        if (std::filesystem::is_directory(binarydir))
            data_dir.push_back(binarydir);

        // This data dir is for engine stuff
        auto rootdir = std::filesystem::path(solutiondir).parent_path() / "src";
        if (std::filesystem::is_directory(rootdir))
            data_dir.push_back(rootdir);

        // This data dir is for submodule support stuff
        rootdir = std::filesystem::path(solutiondir) / "lol" / "src";
        if (std::filesystem::is_directory(rootdir))
            data_dir.push_back(rootdir);

        // This data dir is for project-specific stuff
        rootdir = std::filesystem::path(projectdir);
        if (std::filesystem::is_directory(rootdir))
            data_dir.push_back(rootdir);

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
        auto rootdir = binarydir;
        auto subdir = std::filesystem::path(sourcesubdir);

        while (subdir.parent_path() != subdir)
        {
            rootdir = rootdir.parent_path();
            subdir = subdir.parent_path();
        }

        data_dir.push_back(rootdir / "src");

        // This data dir is for project-specific stuff
        if (std::filesystem::is_directory(binarydir))
            data_dir.push_back(std::filesystem::path(binarydir));

        got_rootdir = true;
    }

    msg::debug("binary dir: “%s”\n", binarydir.generic_string().c_str());
    for (int i = 0; i < int(data_dir.size()); ++i)
        msg::debug("data dir %d/%d: “%s”\n", i + 1, int(data_dir.size()),
                   data_dir[i].generic_string().c_str());
#endif

#if LOL_USE_KINC
    kinc_init(name.c_str(), width, height, nullptr, nullptr);

    // Call all the registered callbacks and remove the ones that return false
    kinc_set_update_callback([](void *){
        kinc_g4_begin(0);
        kinc_g4_clear(KINC_G4_CLEAR_COLOR, 0xFF000000, 0.0f, 0);

        // FIXME: this will be std::erase_if in C++20
        g_callbacks.erase(std::remove_if(g_callbacks.begin(), g_callbacks.end(),
                          [](auto fn) { return !fn(); }), g_callbacks.end());

        kinc_g4_end(0);
        kinc_g4_swap_buffers();
    } , nullptr);
#endif

    lol::audio::init();

#if _WIN32
   // If we have no console but a debugger is attached, register a logging function
   if (GetConsoleWindow() == 0 && IsDebuggerPresent())
   {
       lol::msg::set_output([](std::string const& s) -> bool
       {
           OutputDebugStringA(s.c_str());
           return false;
       });
   }
#endif
}

void add_callback(std::function<bool()> fn)
{
    g_callbacks.push_back(fn);
}

void run()
{
#if LOL_USE_KINC
    kinc_start();
#endif

    lol::audio::shutdown();
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
#if !defined _LIBCPP_HAS_NO_FILESYSTEM_LIBRARY && !defined _LIBCPP_AVAILABILITY_HAS_NO_FILESYSTEM_LIBRARY
    data_dir.push_back(dir);
#endif
}

std::string get_data_path(std::string const &file)
{
#if !defined _LIBCPP_HAS_NO_FILESYSTEM_LIBRARY && !defined _LIBCPP_AVAILABILITY_HAS_NO_FILESYSTEM_LIBRARY
    // If not an absolute path, look through known data directories
    if (file[0] != '/')
        for (auto const &dir : data_dir)
        {
            auto path = dir / file;
            std::error_code ec;
            if (std::filesystem::exists(path, ec))
                return path.generic_string();
        }
#endif

    return file;
}

} // namespace lol::sys