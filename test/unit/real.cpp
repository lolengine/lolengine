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
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(RealTest)
{
public:
    LOLUNIT_TEST(test_real_from_float)
    {
        float x = real4k(0.0f);
        float y = real4k(1.0f);

        LOLUNIT_ASSERT_EQUAL(x, 0.0f);
        LOLUNIT_ASSERT_EQUAL(y, 1.0f);
    }
};

} /* namespace lol */

