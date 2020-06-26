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

#include <lol/unit_test>
#include <lol/vector>
#include <lol/image>

namespace lol
{

lolunit_declare_fixture(image_test)
{
    lolunit_declare_test(image_pixels)
    {
        image im(128, 128);

        // Access pixels of an image
        auto p1 = im.pixels();
        p1[12345].r = 127;
        p1[12345].g = 0;
        p1[12345].b = 255;

        p1[42] = p1[12345];

        // Access pixels of a const image
        image const &im2 = im;
        auto p2 = im2.pixels();

        lolunit_assert_equal(p2[42].r, 127);
        lolunit_assert_equal(p2[42].g, 0);
        lolunit_assert_equal(p2[42].b, 255);
    }

    lolunit_declare_test(open_image)
    {
        image img;
        bool ret = img.load("data/gradient.png");

        lolunit_assert(ret);

        ivec2 isize = img.size();
        lolunit_assert_equal(isize.x, 256);
        lolunit_assert_equal(isize.y, 16);

        lol::span2d<u8vec4> data = img.pixels();

        lolunit_assert_equal((int)data[0].r, 0x00);
        lolunit_assert_equal((int)data[0].g, 0x00);
        lolunit_assert_equal((int)data[0].b, 0x00);

        lolunit_assert_equal((int)data[255].r, 0xff);
        lolunit_assert_equal((int)data[255].g, 0xff);
        lolunit_assert_equal((int)data[255].b, 0xff);
    }

    lolunit_declare_test(image_convert)
    {
        image src;
        image dst = src;
    }
};

} // namespace lol

