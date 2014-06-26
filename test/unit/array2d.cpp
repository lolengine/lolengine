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

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(Array2DTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(Array2DCreate)
    {
        Array2D<int> a(ivec2(10, 10));

        a[0][0] = 2;
        a[9][0] = 4;
        a[0][9] = 6;
        a[9][9] = 8;

        LOLUNIT_ASSERT_EQUAL(a[0][0], 2);
        LOLUNIT_ASSERT_EQUAL(a[9][0], 4);
        LOLUNIT_ASSERT_EQUAL(a[0][9], 6);
        LOLUNIT_ASSERT_EQUAL(a[9][9], 8);

        Array2D<int> const &b = a;

        LOLUNIT_ASSERT_EQUAL(b[0][0], 2);
        LOLUNIT_ASSERT_EQUAL(b[9][0], 4);
        LOLUNIT_ASSERT_EQUAL(b[0][9], 6);
        LOLUNIT_ASSERT_EQUAL(b[9][9], 8);
    }
};

} /* namespace lol */

