//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2014—2015 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

#include <iostream>
namespace lol
{

lolunit_declare_fixture(timer_test)
{
    void setup()
    {
    }

    void teardown()
    {
    }

    lolunit_declare_test(timers)
    {
        Timer timer0, timer1;

        timer0.Wait(1.5);

        float t0 = timer1.Poll();
        float t1 = timer1.Get();
        lolunit_assert_doubles_equal(t0, t1, 1e-5);
        lolunit_assert_doubles_equal(1.5, t1, 1e-3);

        timer1.Wait(1.5);
        lolunit_assert_doubles_equal(3.0, timer0.Get(), 1e-3);
    }
};

}