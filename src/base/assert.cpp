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

#if defined HAVE_CXXABI_H
#   include <cxxabi.h>
#endif
#if defined HAVE_EXECINFO_H
#   include <execinfo.h>
#endif

#include "core.h"

namespace lol
{

void DumpStack()
{
#if defined HAVE_CXA_DEMANGLE
    /* Get current stack frames */
    void *stack_ptrs[50];
    size_t size = backtrace(stack_ptrs, 50);
    char **callstack = backtrace_symbols(stack_ptrs, size);

    if (size > 1)
        Log::Debug("%d functions in stack trace:\n", (int)size - 1);

    /* Parse stack frames, skipping the first element (because
     * that’s ourselves) and print information. */
    for (size_t i = 1; i < size; ++i)
    {
        char *name = 0, *offset = 0, *address = 0;

        for (char *p = callstack[i]; *p; ++p)
        {
            if (*p == '(')
                name = p;
            else if (*p == '+')
                offset = p;
            else if (*p == ')')
            {
                address = p;
                break;
            }
        }

        if (name && offset && address && name < offset)
        {
            *name++ = *offset++ = *address++ = '\0';

            int ret;
            char *realname = abi::__cxa_demangle(name, 0, 0, &ret);

            if (ret == 0)
                name = realname;

            Log::Debug("#%d %s: %s+%s %s\n", (int)i,
                       callstack[i], name, offset, address);
            free(realname);
        }
        else
        {
            Log::Debug("#%d %s\n", (int)i, callstack[i]);
        }
    }

    free(callstack);
#endif
}

} /* namespace lol */

