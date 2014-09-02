//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cmath>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(ImageTest)
{
    lolunit_declare_test(OpenImage)
    {
        Image image("data/gradient.png");

        ivec2 size = image.GetSize();
        lolunit_assert_equal(size.x, 256);
        lolunit_assert_equal(size.y, 16);

        u8vec4 *data = image.Lock<PixelFormat::RGBA_8>();
        lolunit_assert(data);

        lolunit_assert_equal((int)data[0].r, 0x00);
        lolunit_assert_equal((int)data[0].g, 0x00);
        lolunit_assert_equal((int)data[0].b, 0x00);

        lolunit_assert_equal((int)data[255].r, 0xff);
        lolunit_assert_equal((int)data[255].g, 0xff);
        lolunit_assert_equal((int)data[255].b, 0xff);

        image.Unlock(data);
    }
};

} /* namespace lol */

