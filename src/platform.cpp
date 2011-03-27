//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cmath>

#include "core.h"

namespace lol
{

/*
 * Public Platform class
 */

int Platform::GetMouseCount()
{
#if defined ANDROID_NDK
    /* Assume Android devices are touch devices that don't have a mouse */
    return 0;
#endif
    return 1;
}

} /* namespace lol */

