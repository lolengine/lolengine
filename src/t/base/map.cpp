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
#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(MapTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(MapDeclare)
    {
        map<uint8_t, uint8_t> m1;
        map<int, int> m2;
        map<float, float> m3;
        map<char const *, char const *> m4;
    }

    lolunit_declare_test(MapSet)
    {
        map<int, int> map;

        for (int i = 0; i < 1000; i++)
            map[i] = -1;

        for (int i = 0; i < 1000; i++)
            map[i] = i;

        for (int i = 0; i < 1000; i++)
            lolunit_assert_equal(map[i], i);
    }

    lolunit_declare_test(MapHasKey)
    {
        map<int, int> map;

        map[0] = 1;
        map[2] = 2;

        lolunit_assert(map.HasKey(0));
        lolunit_assert(!map.HasKey(1));
        lolunit_assert(map.HasKey(2));
    }

    lolunit_declare_test(StringMap)
    {
        map<char const *, int> map;

        map["foo"] = 42;
        map["bar"] = 12;
        map["baz"] = 2;

        int foo = map["foo"];
        int bar = map["bar"];
        int baz = map["baz"];

        lolunit_assert_equal(42, foo);
        lolunit_assert_equal(12, bar);
        lolunit_assert_equal(2, baz);
    }
};

} /* namespace lol */

