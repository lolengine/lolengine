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
            uint32_t r = rand<int>();
            for (int k = 0; k < 31; k++)
            {
                bits[k] += r & 1;
                r >>= 1;
            }
        }

        for (int k = 0; k < 31; k++)
        {
            LOLUNIT_SET_CONTEXT(k);
            LOLUNIT_ASSERT_GREATER(bits[k], rolls / 3);
            LOLUNIT_ASSERT_LESS(bits[k], rolls * 2 / 3);
            LOLUNIT_UNSET_CONTEXT(k);
        }
    }
};

} /* namespace lol */

