//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
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

LOLUNIT_FIXTURE(ArrayNDTest)
{
    void SetUp() {}

    void TearDown() {}

#if 0
    LOLUNIT_TEST(ArrayNDCreate)
    {
        arraynd<10, int> a;
        arraynd<20, float> b;
        arraynd<30, uint8_t> c;
        arraynd<40, double> d;
    }

    LOLUNIT_TEST(ArrayNDInit)
    {
        int const dimension = 8;
        vec_t<size_t, dimension> size;
        for (int i = 0; i < dimension; ++i)
            size[i] = 5;

        arraynd<dimension, uint8_t> a(size);
        memset(a.Data(), 0, a.Bytes());
        LOLUNIT_ASSERT_EQUAL(a[2][3][2][0][1][4][0][1], 0x00);

        vec_t<int, dimension> v{ 2, 3, 2, 0, 1, 4, 0, 1 };
        LOLUNIT_ASSERT_EQUAL(a[v], 0x00);

        a[2][3][2][0][1][4][0][1] = 0xcd;
        LOLUNIT_ASSERT_EQUAL(a[2][3][2][0][1][4][0][1], 0xcd);
        LOLUNIT_ASSERT_EQUAL(a[v], 0xcd);
    }
#endif
};

} /* namespace lol */

