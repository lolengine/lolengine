//
//  Lol Engine — Unit tests
//
//  Copyright © 2010–2024 Sam Hocevar <sam@hocevar.net>
//            © 2014–2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
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

#include <lol/unit_test>
#include <lol/format>
#include <lol/utils>

#include <string>

namespace lol
{

lolunit_declare_fixture(string_test)
{
    lolunit_declare_test(string_format)
    {
        std::string s1 = "3a";
        std::string s2 = std::format("{:d}{:x}", 3, 10);

        lolunit_assert(s1 == s2);

        std::string s3 = "abc 3";
        std::string s4 = std::format("abc {}", 3);

        lolunit_assert(s3 == s4);
    }

    lolunit_declare_test(starts_ends_with)
    {
        std::string s = "lolilol";

        lolunit_assert(starts_with(s, "loli"));
        lolunit_assert(!starts_with(s, "lolo"));
        lolunit_assert(!starts_with(s, "lolilolilol"));

        lolunit_assert(ends_with(s, "ilol"));
        lolunit_assert(!ends_with(s, "olol"));
        lolunit_assert(!ends_with(s, "lolilolilol"));
    }

    lolunit_declare_test(string_split)
    {
        auto l1 = split(std::string("abc"));
        lolunit_assert(l1.size() == 1);
        lolunit_assert(l1[0] == "abc");

        auto l2 = split(std::string("\nabc"));
        lolunit_assert(l2.size() == 2);
        lolunit_assert(l2[0] == "");
        lolunit_assert(l2[1] == "abc");

        auto l3 = split(std::string("abc\n"));
        lolunit_assert(l3.size() == 2);
        lolunit_assert(l3[0] == "abc");
        lolunit_assert(l3[1] == "");

        auto l4 = split(std::string("\n\n"));
        lolunit_assert(l4.size() == 3);
        lolunit_assert(l4[0] == "");
        lolunit_assert(l4[1] == "");
        lolunit_assert(l4[2] == "");

        auto l5 = split(std::string("abc\nde\n\nf\n"));
        lolunit_assert(l5.size() == 5);
        lolunit_assert(l5[0] == "abc");
        lolunit_assert(l5[1] == "de");
        lolunit_assert(l5[2] == "");
        lolunit_assert(l5[3] == "f");
        lolunit_assert(l5[4] == "");
    }
};

} /* namespace lol */

