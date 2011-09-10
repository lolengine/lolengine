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

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(BuildTest)
{
    LOLUNIT_TEST(FastMath)
    {
        double x, y;

        y = x = 1.0 + RandF(0.1f, 0.2f);
        y += 4503599627370496.0;
        /* The compiler should optimise this away */
        y -= 4503599627370496.0;

        LOLUNIT_ASSERT_EQUAL(x, y);
    }

    LOLUNIT_TEST(FastMathOverride)
    {
        double x, y;

        y = x = 1.0 + RandF(0.1f, 0.2f);
        y += 4503599627370496.0;
        FP_USE(y);
        /* The compiler should not optimise this away */
        y -= 4503599627370496.0;

        LOLUNIT_ASSERT_EQUAL(1.0, y);
    }

    LOLUNIT_TEST(IsNaN)
    {
        union { float f; uint32_t x; } u;

        u.x = 0x7fc00000u;
        LOLUNIT_ASSERT(isnan(u.f));
    }
};

} /* namespace lol */

