//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(ThreadTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(QueueTryPush)
    {
        Queue<int, 1> q;

        bool b1 = q.TryPush(0);
        lolunit_assert_equal(true, b1);

        bool b2 = q.TryPush(1);
        lolunit_assert_equal(false, b2);
    }

    lolunit_declare_test(QueueTryPop)
    {
        Queue<int, 1> q;
        int tmp;

        q.Push(42);

        bool b1 = q.TryPop(tmp);
        lolunit_assert_equal(true, b1);
        lolunit_assert_equal(42, tmp);

        bool b2 = q.TryPop(tmp);
        lolunit_assert_equal(false, b2);
        lolunit_assert_equal(42, tmp);
    }
};

} /* namespace lol */

