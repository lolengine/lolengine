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

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(EnumTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(EnumToString)
    {
        LOL_SAFE_ENUM(MyEnum,
            First = -10,
            Second,
            Third = 5,
        );

        MyEnum e = MyEnum::First;
        LOLUNIT_ASSERT(e.ToString() == "First");

        e = MyEnum::Second;
        LOLUNIT_ASSERT(e.ToString() == "Second");

        e = MyEnum::Third;
        LOLUNIT_ASSERT(e.ToString() == "Third");

        e = MyEnum(42);
        LOLUNIT_ASSERT(e.ToString() != "First");
        LOLUNIT_ASSERT(e.ToString() != "Second");
        LOLUNIT_ASSERT(e.ToString() != "Third");
    }
};

} /* namespace lol */

