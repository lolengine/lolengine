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

#include <lol/engine-internal.h>
#include <lol/base/lolunit.h>

#include <cmath>

namespace lol
{

lolunit_declare_fixture(image_test)
{
    lolunit_declare_test(open_image)
    {
        image img("data/gradient.png");

        ivec2 isize = img.size();
        lolunit_assert_equal(isize.x, 256);
        lolunit_assert_equal(isize.y, 16);

        u8vec4 *data = img.lock<PixelFormat::RGBA_8>();
        lolunit_assert(data);

        lolunit_assert_equal((int)data[0].r, 0x00);
        lolunit_assert_equal((int)data[0].g, 0x00);
        lolunit_assert_equal((int)data[0].b, 0x00);

        lolunit_assert_equal((int)data[255].r, 0xff);
        lolunit_assert_equal((int)data[255].g, 0xff);
        lolunit_assert_equal((int)data[255].b, 0xff);

        img.unlock(data);
    }
};

} /* namespace lol */

