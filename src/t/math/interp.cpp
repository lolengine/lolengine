//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(interp_test)
{
    lolunit_declare_test(time_interp_test)
    {
        TimeInterp<float> ti;

        ti.Set(1.f, 10.f);
        ti.Set(1.f, 20.f);
        ti.Set(1.f, 30.f);

        lolunit_assert_doubles_equal(0.f, ti.Get(-3.0f), 1.e-5f);
        lolunit_assert_doubles_equal(10.f, ti.Get(-2.0f), 1.e-5f);
        lolunit_assert_doubles_equal(20.f, ti.Get(-1.0f), 1.e-5f);
        lolunit_assert_doubles_equal(30.f, ti.Get(0.0f), 1.e-5f);
        lolunit_assert_doubles_equal(40.f, ti.Get(1.0f), 1.e-5f);
    }
};

} /* namespace lol */

