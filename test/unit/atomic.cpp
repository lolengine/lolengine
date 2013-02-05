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

#include <cmath>

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(AtomicTest)
{
    LOLUNIT_TEST(SingleThread)
    {
        Atomic<int> i = 0;

        LOLUNIT_ASSERT_EQUAL(0, (int)i);

        int a1 = i++;
        LOLUNIT_ASSERT_EQUAL(0, a1);
        LOLUNIT_ASSERT_EQUAL(1, (int)i);

        int a2 = i--;
        LOLUNIT_ASSERT_EQUAL(1, a2);
        LOLUNIT_ASSERT_EQUAL(0, (int)i);

        int a3 = ++i;
        LOLUNIT_ASSERT_EQUAL(1, a3);
        LOLUNIT_ASSERT_EQUAL(1, (int)i);

        int a4 = --i;
        LOLUNIT_ASSERT_EQUAL(0, a4);
        LOLUNIT_ASSERT_EQUAL(0, (int)i);
    }
};

} /* namespace lol */

