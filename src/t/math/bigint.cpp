//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cmath>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(bigint_test)
{
    lolunit_declare_test(declaration)
    {
        bigint<> a;
        bigint<0> b;
        bigint<32> c;
    }

    lolunit_declare_test(int32_cast)
    {
        bigint<> a(0), b(1), c(-1);

        lolunit_assert_equal((int32_t)a, 0);
        lolunit_assert_equal((int32_t)b, 1);
        lolunit_assert_equal((int32_t)c, -1);
    }

    lolunit_declare_test(uint32_cast)
    {
        bigint<> a(0), b(1), c(~(uint32_t)0);

        lolunit_assert_equal((uint32_t)a, 0);
        lolunit_assert_equal((uint32_t)b, 1);
        lolunit_assert_equal((uint32_t)c, ~(uint32_t)0);
    }

    lolunit_declare_test(empty_bigint_is_zero)
    {
        bigint<0> a, b(1), c(-1);

        lolunit_assert_equal((int)a, 0);
        lolunit_assert_equal((int)b, 0);
        lolunit_assert_equal((int)c, 0);
    }
};

} /* namespace lol */

