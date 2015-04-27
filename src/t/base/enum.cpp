//
//  Lol Engine — Unit tests
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

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(EnumTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(EnumToString)
    {
        struct MyEnumBase : public StructSafeEnum
        {
            enum Type
            {
                First = -10,
                Second,
                Third = 5,
            };
        protected:
            virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
            {
                enum_map[First] = "First";
                enum_map[Second] = "Second";
                enum_map[Third] = "Third";
                return true;
            }
        };
        typedef SafeEnum<MyEnumBase> MyEnum;

        MyEnum e = MyEnum::First;
        lolunit_assert(e.ToString() == "First");

        e = MyEnum::Second;
        lolunit_assert(e.ToString() == "Second");

        e = MyEnum::Third;
        lolunit_assert(e.ToString() == "Third");

        e = MyEnum(42);
        lolunit_assert(e.ToString() != "First");
        lolunit_assert(e.ToString() != "Second");
        lolunit_assert(e.ToString() != "Third");
    }
};

} /* namespace lol */

