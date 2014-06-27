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

LOLUNIT_FIXTURE(MapTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(MapDeclare)
    {
        map<uint8_t, uint8_t> m1;
        map<int, int> m2;
        map<float, float> m3;
        map<char const *, char const *> m4;
    }

    LOLUNIT_TEST(MapSet)
    {
        map<int, int> map;

        for (int i = 0; i < 1000; i++)
            map[i] = -1;

        for (int i = 0; i < 1000; i++)
            map[i] = i;

        for (int i = 0; i < 1000; i++)
            LOLUNIT_ASSERT_EQUAL(map[i], i);
    }

    LOLUNIT_TEST(MapHasKey)
    {
        map<int, int> map;

        map[0] = 1;
        map[2] = 2;

        LOLUNIT_ASSERT(map.HasKey(0));
        LOLUNIT_ASSERT(!map.HasKey(1));
        LOLUNIT_ASSERT(map.HasKey(2));
    }

    LOLUNIT_TEST(StringMap)
    {
        map<char const *, int> map;

        map["foo"] = 42;
        map["bar"] = 12;
        map["baz"] = 2;

        int foo = map["foo"];
        int bar = map["bar"];
        int baz = map["baz"];

        LOLUNIT_ASSERT_EQUAL(42, foo);
        LOLUNIT_ASSERT_EQUAL(12, bar);
        LOLUNIT_ASSERT_EQUAL(2, baz);
    }
};

} /* namespace lol */

