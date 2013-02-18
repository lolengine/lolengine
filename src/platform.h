//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Platform interface
// ----------------------
// Helper functions to set up the platform.
//

#if !defined __LOL_PLATFORM_H__
#define __LOL_PLATFORM_H__

#include <stdint.h>

namespace lol
{

class Platform
{
public:
    static int GetMouseCount();

private:
    Platform() {}
};

} /* namespace lol */

#endif // __LOL_PLATFORM_H__

