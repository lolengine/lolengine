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

LOLUNIT_FIXTURE(StringTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(StringBuild)
    {
        String s1;
        LOLUNIT_ASSERT_EQUAL(s1.Count(), 0);
        LOLUNIT_ASSERT_EQUAL(s1[0], '\0');

        String s2("");
        LOLUNIT_ASSERT_EQUAL(s2.Count(), 0);
        LOLUNIT_ASSERT_EQUAL(s2[0], '\0');

        String s3("a");
        LOLUNIT_ASSERT_EQUAL(s3.Count(), 1);
        LOLUNIT_ASSERT_EQUAL(s3[0], 'a');
        LOLUNIT_ASSERT_EQUAL(s3[1], '\0');

        String s4(s3);
        LOLUNIT_ASSERT_EQUAL(s4.Count(), 1);
        LOLUNIT_ASSERT_EQUAL(s4[0], 'a');
        LOLUNIT_ASSERT_EQUAL(s4[1], '\0');
    }

    LOLUNIT_TEST(StringAppendChar)
    {
        String s;
        s += 'a';
        s += 'b';
        s += 'c';

        LOLUNIT_ASSERT_EQUAL(s[0], 'a');
        LOLUNIT_ASSERT_EQUAL(s[1], 'b');
        LOLUNIT_ASSERT_EQUAL(s[2], 'c');
        LOLUNIT_ASSERT_EQUAL(s[3], '\0');
    }

    LOLUNIT_TEST(StringCopy)
    {
        String s1 = "abc";

        String s2 = s1;

        LOLUNIT_ASSERT_EQUAL(s1[0], s2[0]);
        LOLUNIT_ASSERT_EQUAL(s1[1], s2[1]);
        LOLUNIT_ASSERT_EQUAL(s1[2], s2[2]);
        LOLUNIT_ASSERT_EQUAL(s1[3], s2[3]);
    }

    LOLUNIT_TEST(StringConcat)
    {
        String s1("ab"), s2("cd");

        String s3 = s1 + s2;
        LOLUNIT_ASSERT_EQUAL(s3[0], 'a');
        LOLUNIT_ASSERT_EQUAL(s3[1], 'b');
        LOLUNIT_ASSERT_EQUAL(s3[2], 'c');
        LOLUNIT_ASSERT_EQUAL(s3[3], 'd');
        LOLUNIT_ASSERT_EQUAL(s3[4], '\0');
    }

    LOLUNIT_TEST(StringAppendString)
    {
        String s1("ab"), s2("cd");

        s1 += s2;
        LOLUNIT_ASSERT_EQUAL(s1.Count(), 4);
        LOLUNIT_ASSERT_EQUAL(s1[0], 'a');
        LOLUNIT_ASSERT_EQUAL(s1[1], 'b');
        LOLUNIT_ASSERT_EQUAL(s1[2], 'c');
        LOLUNIT_ASSERT_EQUAL(s1[3], 'd');
        LOLUNIT_ASSERT_EQUAL(s1[4], '\0');

        s2 += s2;
        LOLUNIT_ASSERT_EQUAL(s2.Count(), 4);
        LOLUNIT_ASSERT_EQUAL(s2[0], 'c');
        LOLUNIT_ASSERT_EQUAL(s2[1], 'd');
        LOLUNIT_ASSERT_EQUAL(s2[2], 'c');
        LOLUNIT_ASSERT_EQUAL(s2[3], 'd');
        LOLUNIT_ASSERT_EQUAL(s2[4], '\0');
    }

    LOLUNIT_TEST(StringEqual)
    {
        String s1("abc");
        String s2("abc");
        String s3("ab");

        LOLUNIT_ASSERT(s1 == s2);
        LOLUNIT_ASSERT(!(s1 == s3));
    }

    LOLUNIT_TEST(StringDifferent)
    {
        String s1("abc");
        String s2("ab");
        String s3("abc");

        LOLUNIT_ASSERT(s1 != s2);
        LOLUNIT_ASSERT(!(s1 != s3));
    }

    LOLUNIT_TEST(StringCharsEqual)
    {
        char const* sz = "abc";
        String s1("abc");
        String s2("ab");

        LOLUNIT_ASSERT(s1 == sz);
        LOLUNIT_ASSERT(sz == s1);
        LOLUNIT_ASSERT(!(s2 == sz));
        LOLUNIT_ASSERT(!(sz == s2));
    }

    LOLUNIT_TEST(StringCharsDifferent)
    {
        char const* sz = "abc";
        String s1("ab");
        String s2("abc");

        LOLUNIT_ASSERT(s1 != sz);
        LOLUNIT_ASSERT(sz != s1);
        LOLUNIT_ASSERT(!(s2 != sz));
        LOLUNIT_ASSERT(!(sz != s2));
    }


    LOLUNIT_TEST(StringPrintf)
    {
        String s1 = "3a";
        String s2 = String::Printf("%d%x", 3, 10);

        LOLUNIT_ASSERT(s1 == s2);

        String s3 = "abc 3";
        String s4 = String::Printf("abc %d", 3);

        LOLUNIT_ASSERT(s3 == s4);
    }

    LOLUNIT_TEST(SubString)
    {
        String s1 = "Hello World";

        String s2 = s1.Sub(0, 5);
        String s3 = "Hello";
        LOLUNIT_ASSERT(s2 == s3);

        String s4 = s1.Sub(6, 5);
        String s5 = "World";
        LOLUNIT_ASSERT(s4 == s5);
    }

    LOLUNIT_TEST(IndexOf)
    {
        String s1 = "Hello World";
        int i1 = s1.IndexOf('H');
        int i2 = s1.IndexOf('W');
        int i3 = s1.IndexOf('d');
        int i4 = s1.IndexOf("Hello");
        int i5 = s1.IndexOf("World");
        int i6 = s1.IndexOf("lo");
        int i7 = s1.IndexOf("Hello World");
        int i8 = s1.IndexOf("Sup' dude");

        LOLUNIT_ASSERT(i1 == 0);
        LOLUNIT_ASSERT(i2 == 6);
        LOLUNIT_ASSERT(i3 == 10);
        LOLUNIT_ASSERT(i4 == i1);
        LOLUNIT_ASSERT(i5 == i2);
        LOLUNIT_ASSERT(i6 == 3);
        LOLUNIT_ASSERT(i7 == 0);
        LOLUNIT_ASSERT(i8 == -1);
    }

    LOLUNIT_TEST(LastIndexOf)
    {
        String s1 = "Hello World";
        int i1 = s1.LastIndexOf('H');
        int i2 = s1.LastIndexOf('W');
        int i3 = s1.LastIndexOf('d');
        int i4 = s1.LastIndexOf("Hello");
        int i5 = s1.LastIndexOf("World");
        int i6 = s1.LastIndexOf("lo");
        int i7 = s1.LastIndexOf("Hello World");
        int i8 = s1.LastIndexOf("Sup' dude");
        int i9 = s1.LastIndexOf('l');

        LOLUNIT_ASSERT(i1 == 0);
        LOLUNIT_ASSERT(i2 == 6);
        LOLUNIT_ASSERT(i3 == 10);
        LOLUNIT_ASSERT(i4 == i1);
        LOLUNIT_ASSERT(i5 == i2);
        LOLUNIT_ASSERT(i6 == 3);
        LOLUNIT_ASSERT(i7 == 0);
        LOLUNIT_ASSERT(i8 == -1);
        LOLUNIT_ASSERT(i9 == 9);
    }};

} /* namespace lol */
