//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cmath>

#include "core.h"
#include "lol/unit.h"

namespace lol
{

LOLUNIT_FIXTURE(ImageTest)
{
    LOLUNIT_TEST(OpenImage)
    {
        Image *image = Image::Create("data/gradient.png");

        ivec2 size = image->GetSize();
        LOLUNIT_ASSERT_EQUAL(size.x, 256);
        LOLUNIT_ASSERT_EQUAL(size.y, 16);

        uint8_t *data = image->GetData();
        LOLUNIT_ASSERT(data);

        LOLUNIT_ASSERT_EQUAL((int)data[0], 0x00);
        LOLUNIT_ASSERT_EQUAL((int)data[1], 0x00);
        LOLUNIT_ASSERT_EQUAL((int)data[2], 0x00);

        LOLUNIT_ASSERT_EQUAL((int)data[255 * 4 + 0], 0xff);
        LOLUNIT_ASSERT_EQUAL((int)data[255 * 4 + 1], 0xff);
        LOLUNIT_ASSERT_EQUAL((int)data[255 * 4 + 2], 0xff);

        Image::Destroy(image);
    }
};

} /* namespace lol */
