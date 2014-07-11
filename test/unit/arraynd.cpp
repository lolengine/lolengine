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
        arraynd<int,10> a;
        arraynd<float,20> b;
        arraynd<uint8_t, 30> c;
        arraynd<double,40> d;
    }

    LOLUNIT_TEST(ArrayNDInit)
    {
        int const dimension = 8;
        vec_t<size_t, dimension> size;
        for (int i = 0; i < dimension; ++i)
            size[i] = 5;

        arraynd<uint8_t, dimension> a(size);
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

