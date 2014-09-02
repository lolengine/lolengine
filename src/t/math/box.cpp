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

#include <lol/main.h>
#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(BoxTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(Box2DIsect)
    {
        box2 b1(vec2(0.f, 0.f), vec2(10.f, 10.f));
        box2 b2(vec2(5.f, 8.f), vec2(8.f, 12.f));
        box2 b3(vec2(5.f, 11.f), vec2(8.f, 13.f));

        lolunit_assert_equal(true, TestAABBVsAABB(b1, b2));
        lolunit_assert_equal(false, TestAABBVsAABB(b1, b3));

        box2 b4(vec2(96.f, 33.f), vec2(144.f, 129.f));
        box2 b5(vec2(264.f, 91.f), vec2(244.f, 71.f));

        lolunit_assert_equal(false, TestAABBVsAABB(b4, b5));
    }

    lolunit_declare_test(Box2DMove)
    {
        box2 b1(vec2(0.f, 0.f), vec2(1.f, 1.f));
        box2 b2(vec2(2.f, 2.f), vec2(3.f, 3.f));

        b1 += vec2(0.6f, 0.6f);
        lolunit_assert_equal(false, TestAABBVsAABB(b1, b2));

        b1 += vec2(0.6f, 0.6f);
        lolunit_assert_equal(true, TestAABBVsAABB(b1, b2));

        b1 -= vec2(0.0f, 0.6f);
        lolunit_assert_equal(false, TestAABBVsAABB(b1, b2));
    }
};

} /* namespace lol */

