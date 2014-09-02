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

lolunit_declare_fixture(InterpTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(TimeInterpTest)
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

