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

lolunit_declare_fixture(StringTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(StringBuild)
    {
        String s1;
        lolunit_assert_equal(s1.Count(), 0);
        lolunit_assert_equal(s1[0], '\0');

        String s2("");
        lolunit_assert_equal(s2.Count(), 0);
        lolunit_assert_equal(s2[0], '\0');

        String s3("a");
        lolunit_assert_equal(s3.Count(), 1);
        lolunit_assert_equal(s3[0], 'a');
        lolunit_assert_equal(s3[1], '\0');

        String s4(s3);
        lolunit_assert_equal(s4.Count(), 1);
        lolunit_assert_equal(s4[0], 'a');
        lolunit_assert_equal(s4[1], '\0');
    }

    lolunit_declare_test(StringAppendChar)
    {
        String s;
        s += 'a';
        s += 'b';
        s += 'c';

        lolunit_assert_equal(s[0], 'a');
        lolunit_assert_equal(s[1], 'b');
        lolunit_assert_equal(s[2], 'c');
        lolunit_assert_equal(s[3], '\0');
    }

    lolunit_declare_test(StringCopy)
    {
        String s1 = "abc";

        String s2 = s1;

        lolunit_assert_equal(s1[0], s2[0]);
        lolunit_assert_equal(s1[1], s2[1]);
        lolunit_assert_equal(s1[2], s2[2]);
        lolunit_assert_equal(s1[3], s2[3]);
    }

    lolunit_declare_test(StringConcat)
    {
        String s1("ab"), s2("cd");

        String s3 = s1 + s2;
        lolunit_assert_equal(s3[0], 'a');
        lolunit_assert_equal(s3[1], 'b');
        lolunit_assert_equal(s3[2], 'c');
        lolunit_assert_equal(s3[3], 'd');
        lolunit_assert_equal(s3[4], '\0');
    }

    lolunit_declare_test(StringAppendString)
    {
        String s1("ab"), s2("cd");

        s1 += s2;
        lolunit_assert_equal(s1.Count(), 4);
        lolunit_assert_equal(s1[0], 'a');
        lolunit_assert_equal(s1[1], 'b');
        lolunit_assert_equal(s1[2], 'c');
        lolunit_assert_equal(s1[3], 'd');
        lolunit_assert_equal(s1[4], '\0');

        s2 += s2;
        lolunit_assert_equal(s2.Count(), 4);
        lolunit_assert_equal(s2[0], 'c');
        lolunit_assert_equal(s2[1], 'd');
        lolunit_assert_equal(s2[2], 'c');
        lolunit_assert_equal(s2[3], 'd');
        lolunit_assert_equal(s2[4], '\0');
    }

    lolunit_declare_test(StringEqual)
    {
        String s1("abc");
        String s2("abc");
        String s3("ab");

        lolunit_assert(s1 == s2);
        lolunit_assert(!(s1 == s3));
    }

    lolunit_declare_test(StringDifferent)
    {
        String s1("abc");
        String s2("ab");
        String s3("abc");

        lolunit_assert(s1 != s2);
        lolunit_assert(!(s1 != s3));
    }

    lolunit_declare_test(StringCharsEqual)
    {
        char const* sz = "abc";
        String s1("abc");
        String s2("ab");

        lolunit_assert(s1 == sz);
        lolunit_assert(sz == s1);
        lolunit_assert(!(s2 == sz));
        lolunit_assert(!(sz == s2));
    }

    lolunit_declare_test(StringCharsDifferent)
    {
        char const* sz = "abc";
        String s1("ab");
        String s2("abc");

        lolunit_assert(s1 != sz);
        lolunit_assert(sz != s1);
        lolunit_assert(!(s2 != sz));
        lolunit_assert(!(sz != s2));
    }


    lolunit_declare_test(StringPrintf)
    {
        String s1 = "3a";
        String s2 = String::Printf("%d%x", 3, 10);

        lolunit_assert(s1 == s2);

        String s3 = "abc 3";
        String s4 = String::Printf("abc %d", 3);

        lolunit_assert(s3 == s4);
    }

    lolunit_declare_test(SubString)
    {
        String s1 = "Hello World";

        String s2 = s1.Sub(0, 5);
        String s3 = "Hello";
        lolunit_assert(s2 == s3);

        String s4 = s1.Sub(6, 5);
        String s5 = "World";
        lolunit_assert(s4 == s5);
    }

    lolunit_declare_test(IndexOf)
    {
        String s1 = "Hello World";
        ptrdiff_t i1 = s1.IndexOf('H');
        ptrdiff_t i2 = s1.IndexOf('W');
        ptrdiff_t i3 = s1.IndexOf('d');
        ptrdiff_t i4 = s1.IndexOf("Hello");
        ptrdiff_t i5 = s1.IndexOf("World");
        ptrdiff_t i6 = s1.IndexOf("lo");
        ptrdiff_t i7 = s1.IndexOf("Hello World");
        ptrdiff_t i8 = s1.IndexOf("Sup' dude");

        lolunit_assert(i1 == 0);
        lolunit_assert(i2 == 6);
        lolunit_assert(i3 == 10);
        lolunit_assert(i4 == i1);
        lolunit_assert(i5 == i2);
        lolunit_assert(i6 == 3);
        lolunit_assert(i7 == 0);
        lolunit_assert(i8 == -1);
    }

    lolunit_declare_test(LastIndexOf)
    {
        String s1 = "Hello World";
        ptrdiff_t i1 = s1.LastIndexOf('H');
        ptrdiff_t i2 = s1.LastIndexOf('W');
        ptrdiff_t i3 = s1.LastIndexOf('d');
        ptrdiff_t i4 = s1.LastIndexOf("Hello");
        ptrdiff_t i5 = s1.LastIndexOf("World");
        ptrdiff_t i6 = s1.LastIndexOf("lo");
        ptrdiff_t i7 = s1.LastIndexOf("Hello World");
        ptrdiff_t i8 = s1.LastIndexOf("Sup' dude");
        ptrdiff_t i9 = s1.LastIndexOf('l');

        lolunit_assert(i1 == 0);
        lolunit_assert(i2 == 6);
        lolunit_assert(i3 == 10);
        lolunit_assert(i4 == i1);
        lolunit_assert(i5 == i2);
        lolunit_assert(i6 == 3);
        lolunit_assert(i7 == 0);
        lolunit_assert(i8 == -1);
        lolunit_assert(i9 == 9);
    }

    lolunit_declare_test(StartsEndsWith)
    {
        String s = "lolilol";
        lolunit_assert(s.StartsWith("loli"));
        lolunit_assert(!s.StartsWith("lolo"));
        lolunit_assert(!s.StartsWith("lolilolilol"));
        lolunit_assert(s.EndsWith("ilol"));
        lolunit_assert(!s.EndsWith("olol"));
        lolunit_assert(!s.EndsWith("lolilolilol"));
    }
};

} /* namespace lol */

