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

LOLUNIT_FIXTURE(ThreadTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(QueueTryPush)
    {
        Queue<int, 1> q;

        bool b1 = q.TryPush(0);
        LOLUNIT_ASSERT_EQUAL(true, b1);

        bool b2 = q.TryPush(1);
        LOLUNIT_ASSERT_EQUAL(false, b2);
    }

    LOLUNIT_TEST(QueueTryPop)
    {
        Queue<int, 1> q;
        int tmp;

        q.Push(42);

        bool b1 = q.TryPop(tmp);
        LOLUNIT_ASSERT_EQUAL(true, b1);
        LOLUNIT_ASSERT_EQUAL(42, tmp);

        bool b2 = q.TryPop(tmp);
        LOLUNIT_ASSERT_EQUAL(false, b2);
        LOLUNIT_ASSERT_EQUAL(42, tmp);
    }
};

} /* namespace lol */

