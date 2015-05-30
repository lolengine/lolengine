//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2014 Sam Hocevar <sam@hocevar.net>
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

lolunit_declare_fixture(RandTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(Int32Bits)
    {
        int const rolls = 2000;

        int bits[32];
        memset(bits, 0, sizeof(bits));

        for (int i = 0; i < rolls; ++i)
        {
            int32_t r = rand<int32_t>();

            lolunit_assert_gequal(r, 0);

            for (int k = 0; k < 31; k++)
            {
                bits[k] += r & 1;
                r >>= 1;
            }
        }

        for (int k = 0; k < 31; k++)
        {
            lolunit_set_context(k);
            lolunit_assert_gequal(bits[k], rolls / 3);
            lolunit_assert_lequal(bits[k], rolls * 2 / 3);
            lolunit_unset_context(k);
        }
    }

    lolunit_declare_test(Int16Bits)
    {
        int const rolls = 2000;

        int bits[16];
        memset(bits, 0, sizeof(bits));

        for (int i = 0; i < rolls; ++i)
        {
            int16_t r = rand<int16_t>();

            lolunit_assert_gequal(r, 0);

            for (int k = 0; k < 15; k++)
            {
                bits[k] += r & 1;
                r >>= 1;
            }
        }

        for (int k = 0; k < 15; k++)
        {
            lolunit_set_context(k);
            lolunit_assert_gequal(bits[k], rolls / 3);
            lolunit_assert_lequal(bits[k], rolls * 2 / 3);
            lolunit_unset_context(k);
        }
    }

    lolunit_declare_test(Int8Bits)
    {
        int const rolls = 2000;

        int bits[8];
        memset(bits, 0, sizeof(bits));

        for (int i = 0; i < rolls; ++i)
        {
            int8_t r = rand<int8_t>();

            lolunit_assert_gequal(r, 0);

            for (int k = 0; k < 7; k++)
            {
                bits[k] += r & 1;
                r >>= 1;
            }
        }

        for (int k = 0; k < 7; k++)
        {
            lolunit_set_context(k);
            lolunit_assert_gequal(bits[k], rolls / 3);
            lolunit_assert_lequal(bits[k], rolls * 2 / 3);
            lolunit_unset_context(k);
        }
    }
};

} /* namespace lol */

