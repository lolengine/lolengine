//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/unit_test>
#include <lol/thread>

namespace lol
{

lolunit_declare_fixture(timer_test)
{
    lolunit_declare_test(timers)
    {
        timer t0, t1;

        t0.wait(1.5);

        float s0 = t1.poll();
        float s1 = t1.get();
        lolunit_assert_doubles_equal(s0, s1, 1e-5);
        lolunit_assert_doubles_equal(1.5, s1, 1e-3);

        t1.wait(1.5);
        lolunit_assert_doubles_equal(3.0, t0.get(), 1e-3);
    }
};

}