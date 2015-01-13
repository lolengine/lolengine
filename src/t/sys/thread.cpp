//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(thread_test)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(queue_try_push)
    {
        queue<int, 1> q;

        bool b1 = q.try_push(0);
        lolunit_assert_equal(true, b1);

        bool b2 = q.try_push(1);
        lolunit_assert_equal(false, b2);
    }

    lolunit_declare_test(queue_try_pop)
    {
        queue<int, 1> q;
        int tmp;

        q.push(42);

        bool b1 = q.try_pop(tmp);
        lolunit_assert_equal(true, b1);
        lolunit_assert_equal(42, tmp);

        bool b2 = q.try_pop(tmp);
        lolunit_assert_equal(false, b2);
        lolunit_assert_equal(42, tmp);
    }
};

} /* namespace lol */

