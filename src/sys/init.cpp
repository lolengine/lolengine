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

void Init(Array<char const *> &args)
{
    /* Try to guess the data directory from the executable location. */
    if (args.Count() > 0)
    {
#if defined _WIN32
#   define SEPARATOR '\\'
#else
#   define SEPARATOR '/'
#endif
        char const *last_slash = strrchr(args[0], SEPARATOR);

        if (last_slash)
        {
            String dir;
            dir.Resize(last_slash - args[0] + 1);
            memcpy(&dir[0], args[0], last_slash - args[0] + 1);

            SetDataDir(&dir[0]);
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

