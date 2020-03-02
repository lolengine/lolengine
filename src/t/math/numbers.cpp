//
//  Lol Engine — Unit tests for number theory
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>
#include <lol/base/lolunit.h>

namespace lol
{

lolunit_declare_fixture(gcd_test)
{
    lolunit_declare_test(gcd_int)
    {
        lolunit_assert_equal(1, lol::gcd(37, 600));
        lolunit_assert_equal(2, lol::gcd(4, 6));
        lolunit_assert_equal(2, lol::gcd(6, 4));
        lolunit_assert_equal(13, lol::gcd(13, 13));
        lolunit_assert_equal(20, lol::gcd(20, 100));
        lolunit_assert_equal(18913, lol::gcd(624129, 2061517));
    }

    lolunit_declare_test(gcd_double)
    {
        lolunit_assert_equal(2.0, lol::gcd(4.0, 6.0));
        lolunit_assert_equal(2.5, lol::gcd(5.0, 7.5));
        lolunit_assert_equal(0.125, lol::gcd(4.625, 75.0));
    }

    lolunit_declare_test(gcd_negative)
    {
        lolunit_assert_equal(2, lol::gcd(4, -6));
        lolunit_assert_equal(2, lol::gcd(-4, 6));
        lolunit_assert_equal(2, lol::gcd(-4, -6));
        lolunit_assert_equal(2, lol::gcd(6, -4));
        lolunit_assert_equal(2, lol::gcd(-6, 4));
        lolunit_assert_equal(2, lol::gcd(-6, -4));
    }
};

} /* namespace lol */

