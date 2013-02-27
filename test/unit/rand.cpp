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

LOLUNIT_FIXTURE(RandTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(Int32Bits)
    {
        int const rolls = 2000;

        int bits[32];
        memset(bits, 0, sizeof(bits));

        for (int i = 0; i < rolls; ++i)
        {
            int32_t r = rand<int32_t>();

            LOLUNIT_ASSERT_GEQUAL(r, 0);

            for (int k = 0; k < 31; k++)
            {
                bits[k] += r & 1;
                r >>= 1;
            }
        }

        for (int k = 0; k < 31; k++)
        {
            LOLUNIT_SET_CONTEXT(k);
            LOLUNIT_ASSERT_GEQUAL(bits[k], rolls / 3);
            LOLUNIT_ASSERT_LEQUAL(bits[k], rolls * 2 / 3);
            LOLUNIT_UNSET_CONTEXT(k);
        }
    }

    LOLUNIT_TEST(Int16Bits)
    {
        int const rolls = 2000;

        int bits[16];
        memset(bits, 0, sizeof(bits));

        for (int i = 0; i < rolls; ++i)
        {
            int16_t r = rand<int16_t>();

            LOLUNIT_ASSERT_GEQUAL(r, 0);

            for (int k = 0; k < 15; k++)
            {
                bits[k] += r & 1;
                r >>= 1;
            }
        }

        for (int k = 0; k < 15; k++)
        {
            LOLUNIT_SET_CONTEXT(k);
            LOLUNIT_ASSERT_GEQUAL(bits[k], rolls / 3);
            LOLUNIT_ASSERT_LEQUAL(bits[k], rolls * 2 / 3);
            LOLUNIT_UNSET_CONTEXT(k);
        }
    }

    LOLUNIT_TEST(Int8Bits)
    {
        int const rolls = 2000;

        int bits[8];
        memset(bits, 0, sizeof(bits));

        for (int i = 0; i < rolls; ++i)
        {
            int8_t r = rand<int8_t>();

            LOLUNIT_ASSERT_GEQUAL(r, 0);

            for (int k = 0; k < 7; k++)
            {
                bits[k] += r & 1;
                r >>= 1;
            }
        }

        for (int k = 0; k < 7; k++)
        {
            LOLUNIT_SET_CONTEXT(k);
            LOLUNIT_ASSERT_GEQUAL(bits[k], rolls / 3);
            LOLUNIT_ASSERT_LEQUAL(bits[k], rolls * 2 / 3);
            LOLUNIT_UNSET_CONTEXT(k);
        }
    }
};

} /* namespace lol */

