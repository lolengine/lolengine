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

#include <lol/main.h>
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(InterpTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(TimeInterpTest)
    {
        TimeInterp<float> ti;

        ti.Set(1.f, 10.f);
        ti.Set(1.f, 20.f);
        ti.Set(1.f, 30.f);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(0.f, ti.Get(-3.0f), 1.e-5f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(10.f, ti.Get(-2.0f), 1.e-5f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(20.f, ti.Get(-1.0f), 1.e-5f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(30.f, ti.Get(0.0f), 1.e-5f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(40.f, ti.Get(1.0f), 1.e-5f);
    }
};

} /* namespace lol */

