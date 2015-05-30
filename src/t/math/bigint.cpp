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

#include <cmath>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(bigint_test)
{
    lolunit_declare_test(declaration)
    {
        bigint<> a;
        bigint<0> b;
        bigint<32> c;
    }

    lolunit_declare_test(int32_cast)
    {
        bigint<> a(0), b(1), c(-1);

        lolunit_assert_equal((int32_t)a, 0);
        lolunit_assert_equal((int32_t)b, 1);
        lolunit_assert_equal((int32_t)c, -1);
    }

    lolunit_declare_test(uint32_cast)
    {
        bigint<> a(0), b(1), c(~(uint32_t)0);

        lolunit_assert_equal((uint32_t)a, 0);
        lolunit_assert_equal((uint32_t)b, 1);
        lolunit_assert_equal((uint32_t)c, ~(uint32_t)0);
    }

    lolunit_declare_test(operator_equal)
    {
        bigint<> a(-1), b(0), c(1);

        lolunit_assert(a == a);
        lolunit_assert(!(a == b));
        lolunit_assert(!(a == c));

        lolunit_assert(!(b == a));
        lolunit_assert(b == b);
        lolunit_assert(!(b == c));

        lolunit_assert(!(c == a));
        lolunit_assert(!(c == b));
        lolunit_assert(c == c);
    }

    lolunit_declare_test(operator_notequal)
    {
        bigint<> a(-1), b(0), c(1);

        lolunit_assert(!(a != a));
        lolunit_assert(a != b);
        lolunit_assert(a != c);

        lolunit_assert(b != a);
        lolunit_assert(!(b != b));
        lolunit_assert(b != c);

        lolunit_assert(c != a);
        lolunit_assert(c != b);
        lolunit_assert(!(c != c));
    }

    lolunit_declare_test(operator_smaller)
    {
        bigint<> a(-10), b(-1), c(0), d(1), e(10);

        lolunit_assert(!(a < a));
        lolunit_assert(a < b);
        lolunit_assert(a < c);
        lolunit_assert(a < d);
        lolunit_assert(a < e);

        lolunit_assert(!(b < a));
        lolunit_assert(!(b < b));
        lolunit_assert(b < c);
        lolunit_assert(b < d);
        lolunit_assert(b < e);

        lolunit_assert(!(c < a));
        lolunit_assert(!(c < b));
        lolunit_assert(!(c < c));
        lolunit_assert(c < d);
        lolunit_assert(c < e);

        lolunit_assert(!(d < a));
        lolunit_assert(!(d < b));
        lolunit_assert(!(d < c));
        lolunit_assert(!(d < d));
        lolunit_assert(d < e);

        lolunit_assert(!(e < a));
        lolunit_assert(!(e < b));
        lolunit_assert(!(e < c));
        lolunit_assert(!(e < d));
        lolunit_assert(!(e < e));
    }

    lolunit_declare_test(operator_smaller_or_equal)
    {
        bigint<> a(-10), b(-1), c(0), d(1), e(10);

        lolunit_assert(a <= a);
        lolunit_assert(a <= b);
        lolunit_assert(a <= c);
        lolunit_assert(a <= d);
        lolunit_assert(a <= e);

        lolunit_assert(!(b <= a));
        lolunit_assert(b <= b);
        lolunit_assert(b <= c);
        lolunit_assert(b <= d);
        lolunit_assert(b <= e);

        lolunit_assert(!(c <= a));
        lolunit_assert(!(c <= b));
        lolunit_assert(c <= c);
        lolunit_assert(c <= d);
        lolunit_assert(c <= e);

        lolunit_assert(!(d <= a));
        lolunit_assert(!(d <= b));
        lolunit_assert(!(d <= c));
        lolunit_assert(d <= d);
        lolunit_assert(d <= e);

        lolunit_assert(!(e <= a));
        lolunit_assert(!(e <= b));
        lolunit_assert(!(e <= c));
        lolunit_assert(!(e <= d));
        lolunit_assert(e <= e);
    }

    lolunit_declare_test(operator_greater)
    {
        bigint<> a(-10), b(-1), c(0), d(1), e(10);

        lolunit_assert(!(a > a));
        lolunit_assert(!(a > b));
        lolunit_assert(!(a > c));
        lolunit_assert(!(a > d));
        lolunit_assert(!(a > e));

        lolunit_assert(b > a);
        lolunit_assert(!(b > b));
        lolunit_assert(!(b > c));
        lolunit_assert(!(b > d));
        lolunit_assert(!(b > e));

        lolunit_assert(c > a);
        lolunit_assert(c > b);
        lolunit_assert(!(c > c));
        lolunit_assert(!(c > d));
        lolunit_assert(!(c > e));

        lolunit_assert(d > a);
        lolunit_assert(d > b);
        lolunit_assert(d > c);
        lolunit_assert(!(d > d));
        lolunit_assert(!(d > e));

        lolunit_assert(e > a);
        lolunit_assert(e > b);
        lolunit_assert(e > c);
        lolunit_assert(e > d);
        lolunit_assert(!(e > e));
    }

    lolunit_declare_test(operator_greater_or_equal)
    {
        bigint<> a(-10), b(-1), c(0), d(1), e(10);

        lolunit_assert(a >= a);
        lolunit_assert(!(a >= b));
        lolunit_assert(!(a >= c));
        lolunit_assert(!(a >= d));
        lolunit_assert(!(a >= e));

        lolunit_assert(b >= a);
        lolunit_assert(b >= b);
        lolunit_assert(!(b >= c));
        lolunit_assert(!(b >= d));
        lolunit_assert(!(b >= e));

        lolunit_assert(c >= a);
        lolunit_assert(c >= b);
        lolunit_assert(c >= c);
        lolunit_assert(!(c >= d));
        lolunit_assert(!(c >= e));

        lolunit_assert(d >= a);
        lolunit_assert(d >= b);
        lolunit_assert(d >= c);
        lolunit_assert(d >= d);
        lolunit_assert(!(d >= e));

        lolunit_assert(e >= a);
        lolunit_assert(e >= b);
        lolunit_assert(e >= c);
        lolunit_assert(e >= d);
        lolunit_assert(e >= e);
    }

    lolunit_declare_test(multiply)
    {
        bigint<> a(-10), b(0), c(10);

        lolunit_assert_equal((int32_t)(a * a), 100);
        lolunit_assert_equal((int32_t)(a * b), 0);
        lolunit_assert_equal((int32_t)(a * c), -100);

        lolunit_assert_equal((int32_t)(b * a), 0);
        lolunit_assert_equal((int32_t)(b * b), 0);
        lolunit_assert_equal((int32_t)(b * c), 0);

        lolunit_assert_equal((int32_t)(c * a), -100);
        lolunit_assert_equal((int32_t)(c * b), 0);
        lolunit_assert_equal((int32_t)(c * c), 100);
    }
};

} /* namespace lol */

