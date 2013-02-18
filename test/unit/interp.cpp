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

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(InterpTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(FloatInterp)
    {
        Interp<float> interp;

        interp.Set(1.f, 100.f);
        interp.Set(2.f, 150.f);

        LOLUNIT_ASSERT_DOUBLES_EQUAL(75.f, interp.Get(0.5f), 1.e-5f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(125.f, interp.Get(1.5f), 1.e-5f);
        LOLUNIT_ASSERT_DOUBLES_EQUAL(175.f, interp.Get(2.5f), 1.e-5f);
    }
};

} /* namespace lol */

