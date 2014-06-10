//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
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

/*
 * Safe enum helpers
 */

Map<int64_t, String> BuildEnumMap(char const *str)
{
    Map<int64_t, String> ret;
    char const *parser = str;
    int64_t next_value = 0;

    for (;;)
    {
        /* Find name */
        while (*parser == ' ' || *parser == ',')
            ++parser;
        if (!*parser)
            break;

        /* Parse name */
        char const *name = parser;
        while (*parser && *parser != ' ' && *parser != ',' && *parser != '=')
            ++parser;
        char const *name_end = parser;

        /* Find the value (if any) */
        uint64_t current_value = next_value;
        while (*parser == ' ' || *parser == '=')
            ++parser;
        if (*parser && *parser != ',')
        {
#if defined _WIN32
            current_value = _strtoi64(parser, nullptr, 0);
#else
            current_value = strtoll(parser, nullptr, 0);
#endif

            while (*parser && *parser != ' ' && *parser != ',')
                ++parser;
        }

        /* Store in the map */
        ret[current_value] = String(name, (int)(name_end - name));
        next_value = current_value + 1;
    }

    return ret;
}

} /* namespace lol */

