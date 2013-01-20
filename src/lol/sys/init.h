//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// Various system helper functions
// -------------------------------
//

#if !defined __LOL_SYS_INIT_H__
#define __LOL_SYS_INIT_H__

#include <stdint.h>

namespace lol
{

namespace System
{

extern void Init(Array<char const *> &args);
extern void SetDataDir(char const *dir);
extern char const *GetDataDir();

static inline void Init(int argc, char *argv[])
{
    Array<char const *> args;

    for (int i = 0; i < argc; i++)
        args << argv[i];

    Init(args);
}

} /* namespace System */

} /* namespace lol */

#endif // __LOL_SYS_INIT_H__

