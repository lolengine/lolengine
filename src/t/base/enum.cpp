//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/base/lolunit.h>
#include <lol/base/enum.h>

#include <string>
#include <map>

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
            virtual bool BuildEnumMap(std::map<int64_t, std::string>& enum_map)
            {
                enum_map[first] = "first";
                enum_map[second] = "second";
                enum_map[third] = "third";
                return true;
            }
        };
        typedef SafeEnum<my_enum_base> my_enum;

        my_enum e = my_enum::first;
        lolunit_assert(e.tostring() == "first");

        e = my_enum::second;
        lolunit_assert(e.tostring() == "second");

        e = my_enum::third;
        lolunit_assert(e.tostring() == "third");

        e = my_enum(42);
        lolunit_assert(e.tostring() != "first");
        lolunit_assert(e.tostring() != "second");
        lolunit_assert(e.tostring() != "third");
    }
};

} /* namespace lol */

