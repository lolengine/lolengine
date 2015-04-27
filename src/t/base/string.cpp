//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
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

lolunit_declare_fixture(string_test)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(string_build)
    {
        String s1;
        lolunit_assert_equal(s1.count(), 0);
        lolunit_assert_equal(s1[0], '\0');

        String s2("");
        lolunit_assert_equal(s2.count(), 0);
        lolunit_assert_equal(s2[0], '\0');

        String s3("a");
        lolunit_assert_equal(s3.count(), 1);
        lolunit_assert_equal(s3[0], 'a');
        lolunit_assert_equal(s3[1], '\0');

        String s4(s3);
        lolunit_assert_equal(s4.count(), 1);
        lolunit_assert_equal(s4[0], 'a');
        lolunit_assert_equal(s4[1], '\0');
    }

    lolunit_declare_test(string_append_char)
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

    lolunit_declare_test(string_copy)
    {
        String s1 = "abc";

        String s2 = s1;

        lolunit_assert_equal(s1[0], s2[0]);
        lolunit_assert_equal(s1[1], s2[1]);
        lolunit_assert_equal(s1[2], s2[2]);
        lolunit_assert_equal(s1[3], s2[3]);
    }

    lolunit_declare_test(string_concat)
    {
        String s1("ab"), s2("cd");

        String s3 = s1 + s2;
        lolunit_assert_equal(s3[0], 'a');
        lolunit_assert_equal(s3[1], 'b');
        lolunit_assert_equal(s3[2], 'c');
        lolunit_assert_equal(s3[3], 'd');
        lolunit_assert_equal(s3[4], '\0');
    }

    lolunit_declare_test(string_append_string)
    {
        String s1("ab"), s2("cd");

        s1 += s2;
        lolunit_assert_equal(s1.count(), 4);
        lolunit_assert_equal(s1[0], 'a');
        lolunit_assert_equal(s1[1], 'b');
        lolunit_assert_equal(s1[2], 'c');
        lolunit_assert_equal(s1[3], 'd');
        lolunit_assert_equal(s1[4], '\0');

        s2 += s2;
        lolunit_assert_equal(s2.count(), 4);
        lolunit_assert_equal(s2[0], 'c');
        lolunit_assert_equal(s2[1], 'd');
        lolunit_assert_equal(s2[2], 'c');
        lolunit_assert_equal(s2[3], 'd');
        lolunit_assert_equal(s2[4], '\0');
    }

    lolunit_declare_test(string_equal)
    {
        String s1("abc");
        String s2("abc");
        String s3("ab");

        lolunit_assert(s1 == s2);
        lolunit_assert(!(s1 == s3));
    }

    lolunit_declare_test(string_different)
    {
        String s1("abc");
        String s2("ab");
        String s3("abc");

        lolunit_assert(s1 != s2);
        lolunit_assert(!(s1 != s3));
    }

    lolunit_declare_test(string_chars_equal)
    {
        char const* sz = "abc";
        String s1("abc");
        String s2("ab");

        lolunit_assert(s1 == sz);
        lolunit_assert(sz == s1);
        lolunit_assert(!(s2 == sz));
        lolunit_assert(!(sz == s2));
    }

    lolunit_declare_test(string_chars_different)
    {
        char const* sz = "abc";
        String s1("ab");
        String s2("abc");

        lolunit_assert(s1 != sz);
        lolunit_assert(sz != s1);
        lolunit_assert(!(s2 != sz));
        lolunit_assert(!(sz != s2));
    }


    lolunit_declare_test(string_printf)
    {
        String s1 = "3a";
        String s2 = String::format("%d%x", 3, 10);

        lolunit_assert(s1 == s2);

        String s3 = "abc 3";
        String s4 = String::format("abc %d", 3);

        lolunit_assert(s3 == s4);
    }

    lolunit_declare_test(substring)
    {
        String s1 = "Hello World";

        String s2 = s1.sub(0, 5);
        String s3 = "Hello";
        lolunit_assert(s2 == s3);

        String s4 = s1.sub(6, 5);
        String s5 = "World";
        lolunit_assert(s4 == s5);
    }

    lolunit_declare_test(index_of)
    {
        String s1 = "Hello World";
        int i1 = s1.index_of('H');
        int i2 = s1.index_of('W');
        int i3 = s1.index_of('d');
        int i4 = s1.index_of("Hello");
        int i5 = s1.index_of("World");
        int i6 = s1.index_of("lo");
        int i7 = s1.index_of("Hello World");
        int i8 = s1.index_of("Sup' dude");

        lolunit_assert(i1 == 0);
        lolunit_assert(i2 == 6);
        lolunit_assert(i3 == 10);
        lolunit_assert(i4 == i1);
        lolunit_assert(i5 == i2);
        lolunit_assert(i6 == 3);
        lolunit_assert(i7 == 0);
        lolunit_assert(i8 == -1);
    }

    lolunit_declare_test(last_index_of)
    {
        String s1 = "Hello World";
        int i1 = s1.last_index_of('H');
        int i2 = s1.last_index_of('W');
        int i3 = s1.last_index_of('d');
        int i4 = s1.last_index_of("Hello");
        int i5 = s1.last_index_of("World");
        int i6 = s1.last_index_of("lo");
        int i7 = s1.last_index_of("Hello World");
        int i8 = s1.last_index_of("Sup' dude");
        int i9 = s1.last_index_of('l');

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

    lolunit_declare_test(starts_ends_with)
    {
        String s = "lolilol";
        lolunit_assert(s.starts_with("loli"));
        lolunit_assert(!s.starts_with("lolo"));
        lolunit_assert(!s.starts_with("lolilolilol"));
        lolunit_assert(s.ends_with("ilol"));
        lolunit_assert(!s.ends_with("olol"));
        lolunit_assert(!s.ends_with("lolilolilol"));
    }

    lolunit_declare_test(string_compare)
    {
        String s1 = "lolilol";
        String s2 = s1;
        String s3 = "trololol";
        String s4 = "lolilololol";

        lolunit_assert(!(s1 < s2));
        lolunit_assert(!(s2 < s1));
        lolunit_assert(s1 < s3);
        lolunit_assert(s1 < s4);
        lolunit_assert(!(s4 < s1));
    }
};

} /* namespace lol */

