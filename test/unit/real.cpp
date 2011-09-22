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
    LOLUNIT_TEST(test_real_from_float)
    {
        float a1 = real(0.0f);
        float a2 = real(-0.0f);
        float a3 = real(1.0f);
        float a4 = real(-1.0f);
        float a5 = real(1.5f);

        LOLUNIT_ASSERT_EQUAL(a1, 0.0f);
        LOLUNIT_ASSERT_EQUAL(a2, -0.0f);
        LOLUNIT_ASSERT_EQUAL(a3, 1.0f);
        LOLUNIT_ASSERT_EQUAL(a4, -1.0f);
        LOLUNIT_ASSERT_EQUAL(a5, 1.5f);
    }

    LOLUNIT_TEST(test_real_neg)
    {
        float a1 = - real(1.0f);
        float a2 = - real(-1.0f);
        float a3 = - real(0.0f);
        float a4 = - real(-0.0f);

        LOLUNIT_ASSERT_EQUAL(a1, -1.0f);
        LOLUNIT_ASSERT_EQUAL(a2, 1.0f);
        LOLUNIT_ASSERT_EQUAL(a3, -0.0f);
        LOLUNIT_ASSERT_EQUAL(a4, 0.0f);
    }

    LOLUNIT_TEST(test_real_comp)
    {
        LOLUNIT_ASSERT(real(1.0f) > real(0.5f));
        LOLUNIT_ASSERT(real(1.0f) >= real(0.5f));
        LOLUNIT_ASSERT(real(1.0f) >= real(1.0f));

        LOLUNIT_ASSERT(real(-1.0f) < real(-0.5f));
        LOLUNIT_ASSERT(real(-1.0f) <= real(-0.5f));
        LOLUNIT_ASSERT(real(-1.0f) <= real(-1.0f));

        LOLUNIT_ASSERT(real(-1.0f) < real(0.5f));
        LOLUNIT_ASSERT(real(-0.5f) < real(1.0f));
        LOLUNIT_ASSERT(real(-1.0f) <= real(0.5f));
        LOLUNIT_ASSERT(real(-0.5f) <= real(1.0f));

        LOLUNIT_ASSERT(real(1.0f) > real(-0.5f));
        LOLUNIT_ASSERT(real(0.5f) > real(-1.0f));
        LOLUNIT_ASSERT(real(1.0f) >= real(-0.5f));
        LOLUNIT_ASSERT(real(0.5f) >= real(-1.0f));
    }

    LOLUNIT_TEST(test_real_add)
    {
        float a1 = real(1.0f) + real(0.0f);
        float a2 = real(0.0f) + real(1.0f);
        float a3 = real(1.0f) + real(1.0f);
        float a4 = real(-1.0f) + real(-1.0f);
        float a5 = real(1.0f) + real(0.125f);

        LOLUNIT_ASSERT_EQUAL(a1, 1.0f);
        LOLUNIT_ASSERT_EQUAL(a2, 1.0f);
        LOLUNIT_ASSERT_EQUAL(a3, 2.0f);
        LOLUNIT_ASSERT_EQUAL(a4, -2.0f);
        LOLUNIT_ASSERT_EQUAL(a5, 1.125f);
    }

    LOLUNIT_TEST(test_real_sub)
    {
#if 0
printf("\n");
real k(1.25f);
k.print();
real l(1.0f);
l.print();
real m = k - l;
m.print();
#endif
        LOLUNIT_ASSERT(true);
    }

    LOLUNIT_TEST(test_real_mul)
    {
        real x(1.25f);
        real y(1.5f);
        real z(1.99999f);
        real w(-1.5f);

        float m1 = x * x;
        float m2 = y * y;
        float m3 = z * z;
        float m4 = w * w;

        LOLUNIT_ASSERT_EQUAL(m1, 1.25f * 1.25f);
        LOLUNIT_ASSERT_EQUAL(m2, 1.5f * 1.5f);
        LOLUNIT_ASSERT_EQUAL(m3, 1.99999f * 1.99999f);
        LOLUNIT_ASSERT_EQUAL(m4, -1.5f * -1.5f);
    }
};

} /* namespace lol */

