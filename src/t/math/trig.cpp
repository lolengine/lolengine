//
//  Lol Engine — Unit tests for trigonometric functions
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
#include <lol/math>

namespace lol
{

lolunit_declare_fixture(trig_test)
{
    lolunit_declare_test(angle_conversions)
    {
        lolunit_assert_doubles_equal(D_PI, radians(180.0), 1e-5);
        lolunit_assert_doubles_equal(D_PI_2, radians(90.0), 1e-5);

        lolunit_assert_doubles_equal(F_PI, radians(180.0f), 1e-5f);
        lolunit_assert_doubles_equal(F_PI_2, radians(90.0f), 1e-5f);

        lolunit_assert_doubles_equal(180.0, degrees(D_PI), 1e-5);
        lolunit_assert_doubles_equal(90.0, degrees(D_PI_2), 1e-5);

        lolunit_assert_doubles_equal(180.0f, degrees(F_PI), 1e-5f);
        lolunit_assert_doubles_equal(90.0f, degrees(F_PI_2), 1e-5f);
    }

    lolunit_declare_test(integer_angle_conversions)
    {
        lolunit_assert_doubles_equal(radians(90), radians(90.0), 1e-5);

        lolunit_assert_doubles_equal(radians((int8_t)90),   radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((int16_t)90),  radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((int32_t)90),  radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((int64_t)90),  radians(90.0), 1e-5);
    }
};

} /* namespace lol */

