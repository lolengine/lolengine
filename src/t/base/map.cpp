//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <map>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(map_test)
{
    lolunit_declare_test(map_has_key)
    {
        std::map<int, int> m;

        m[0] = 1;
        m[2] = 2;

        lolunit_assert(has_key(m, 0));
        lolunit_assert(!has_key(m, 1));
        lolunit_assert(has_key(m, 2));
    }
};

} /* namespace lol */

