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

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(BoxTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(Box2DIsect)
    {
        Box2D b1(vec2(0.f, 0.f), vec2(10.f, 10.f));
        Box2D b2(vec2(5.f, 8.f), vec2(8.f, 12.f));
        Box2D b3(vec2(5.f, 11.f), vec2(8.f, 13.f));

        LOLUNIT_ASSERT_EQUAL(true, BoxIsectBox(b1, b2));
        LOLUNIT_ASSERT_EQUAL(false, BoxIsectBox(b1, b3));

        Box2D b4(vec2(96.f, 33.f), vec2(144.f, 129.f));
        Box2D b5(vec2(264.f, 91.f), vec2(244.f, 71.f));

        LOLUNIT_ASSERT_EQUAL(false, BoxIsectBox(b4, b5));
    }
};

} /* namespace lol */

