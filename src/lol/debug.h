//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// Debug utilities
// ---------------
//

#if !defined __LOL_DEBUG_H__
#define __LOL_DEBUG_H__

#include <cstdlib>

namespace lol
{

static inline void Abort()
{
    std::abort();
}

} /* namespace lol */

#endif // __LOL_DEBUG_H__

