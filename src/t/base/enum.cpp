//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
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

lolunit_declare_fixture(enum_test)
{
    lolunit_declare_test(enum_to_string)
    {
        struct my_enum_base : public StructSafeEnum
        {
            enum Type
            {
                first = -10,
                second,
                third = 5,
            };

        protected:
            virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
            {
                enum_map[first] = "first";
                enum_map[second] = "second";
                enum_map[third] = "third";
                return true;
            }
        };
        typedef SafeEnum<my_enum_base> my_enum;

        my_enum e = my_enum::first;
        lolunit_assert(e.ToString() == "first");

        e = my_enum::second;
        lolunit_assert(e.ToString() == "second");

        e = my_enum::third;
        lolunit_assert(e.ToString() == "third");

        e = my_enum(42);
        lolunit_assert(e.ToString() != "first");
        lolunit_assert(e.ToString() != "second");
        lolunit_assert(e.ToString() != "third");
    }
};

} /* namespace lol */

