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
    }
};

} /* namespace lol */

