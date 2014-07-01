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

#include <lol/main.h>
#include "lol/unit.h"

namespace lol
{

/* Data taken from “The CIEDE2000 Color-Difference Formula: Implementation
 * Notes, Supplementary Test Data, and Mathematical Observations” (2004)
 * by Gaurav Sharma, Wencheng Wu, Edul N. Dalal. Some data is disabled
 * because it is at the discontinuity limit. */
static float const ciede2k[] =
{
    50.0000f,   2.6772f, -79.7751f, 50.0000f,   0.0000f, -82.7485f,  2.0425f,
    50.0000f,   3.1571f, -77.2803f, 50.0000f,   0.0000f, -82.7485f,  2.8615f,
    50.0000f,   2.8361f, -74.0200f, 50.0000f,   0.0000f, -82.7485f,  3.4412f,
    50.0000f,  -1.3802f, -84.2814f, 50.0000f,   0.0000f, -82.7485f,  1.0000f,
    50.0000f,  -1.1848f, -84.8006f, 50.0000f,   0.0000f, -82.7485f,  1.0000f,
    50.0000f,  -0.9009f, -85.5211f, 50.0000f,   0.0000f, -82.7485f,  1.0000f,
    50.0000f,   0.0000f,   0.0000f, 50.0000f,  -1.0000f,   2.0000f,  2.3669f,
    50.0000f,  -1.0000f,   2.0000f, 50.0000f,   0.0000f,   0.0000f,  2.3669f,
    50.0000f,   2.4900f,  -0.0010f, 50.0000f,  -2.4900f,   0.0009f,  7.1792f,
    //50.0000f,   2.4900f,  -0.0010f, 50.0000f,  -2.4900f,   0.0010f,  7.1792f,
    50.0000f,   2.4900f,  -0.0010f, 50.0000f,  -2.4900f,   0.0011f,  7.2195f,
    50.0000f,   2.4900f,  -0.0010f, 50.0000f,  -2.4900f,   0.0012f,  7.2195f,
    50.0000f,  -0.0010f,   2.4900f, 50.0000f,   0.0009f,  -2.4900f,  4.8045f,
    //50.0000f,  -0.0010f,   2.4900f, 50.0000f,   0.0010f,  -2.4900f,  4.8045f,
    50.0000f,  -0.0010f,   2.4900f, 50.0000f,   0.0011f,  -2.4900f,  4.7461f,
    50.0000f,   2.5000f,   0.0000f, 50.0000f,   0.0000f,  -2.5000f,  4.3065f,
    50.0000f,   2.5000f,   0.0000f, 73.0000f,  25.0000f, -18.0000f, 27.1492f,
    50.0000f,   2.5000f,   0.0000f, 61.0000f,  -5.0000f,  29.0000f, 22.8977f,
    50.0000f,   2.5000f,   0.0000f, 56.0000f, -27.0000f,  -3.0000f, 31.9030f,
    50.0000f,   2.5000f,   0.0000f, 58.0000f,  24.0000f,  15.0000f, 19.4535f,
    50.0000f,   2.5000f,   0.0000f, 50.0000f,   3.1736f,   0.5854f,  1.0000f,
    50.0000f,   2.5000f,   0.0000f, 50.0000f,   3.2972f,   0.0000f,  1.0000f,
    50.0000f,   2.5000f,   0.0000f, 50.0000f,   1.8634f,   0.5757f,  1.0000f,
    50.0000f,   2.5000f,   0.0000f, 50.0000f,   3.2592f,   0.3350f,  1.0000f,
    60.2574f, -34.0099f,  36.2677f, 60.4626f, -34.1751f,  39.4387f,  1.2644f,
    63.0109f, -31.0961f,  -5.8663f, 62.8187f, -29.7946f,  -4.0864f,  1.2630f,
    61.2901f,   3.7196f,  -5.3901f, 61.4292f,   2.2480f,  -4.9620f,  1.8731f,
    35.0831f, -44.1164f,   3.7933f, 35.0232f, -40.0716f,   1.5901f,  1.8645f,
    22.7233f,  20.0904f, -46.6940f, 23.0331f,  14.9730f, -42.5619f,  2.0373f,
    36.4612f,  47.8580f,  18.3852f, 36.2715f,  50.5065f,  21.2231f,  1.4146f,
    90.8027f,  -2.0831f,   1.4410f, 91.1528f,  -1.6435f,   0.0447f,  1.4441f,
    90.9257f,  -0.5406f,  -0.9208f, 88.6381f,  -0.8985f,  -0.7239f,  1.5381f,
     6.7747f,  -0.2908f,  -2.4247f,  5.8714f,  -0.0985f,  -2.2286f,  0.6377f,
     2.0776f,   0.0795f,  -1.1350f,  0.9033f,  -0.0636f,  -0.5514f,  0.9082f,
};

LOLUNIT_FIXTURE(ColorTest)
{
    void SetUp() {}

    void TearDown() {}

    LOLUNIT_TEST(CIEDE2000)
    {
        size_t count = sizeof(ciede2k) / sizeof(*ciede2k);

        for (size_t n = 0; n < count; n += 7)
        {
            vec3 a(ciede2k[n + 0], ciede2k[n + 1], ciede2k[n + 2]);
            vec3 b(ciede2k[n + 3], ciede2k[n + 4], ciede2k[n + 5]);
            float d1 = ciede2k[n + 6];

            float d2 = Color::DistanceCIEDE2000(a, b);
            float d3 = Color::DistanceCIEDE2000(b, a);

            /* Check that our function works, and check that
             * it is symmetrical. */
            LOLUNIT_SET_CONTEXT(n / 7);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(d1, d2, 0.0001);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(d2, d3, 0.0001);
            LOLUNIT_UNSET_CONTEXT(n / 7);
        }
    }

    LOLUNIT_TEST(RGBToHSV)
    {
        for (int r = 0; r < 20; r++)
        for (int g = 0; g < 20; g++)
        for (int b = 0; b < 20; b++)
        {
            vec3 v1 = vec3(r / 20.f, g / 20.f, b / 20.f);
            vec3 v2 = Color::RGBToHSV(v1);
            vec3 v3 = Color::HSVToRGB(v2);

            String rgb = String::Printf("[%f %f %f]", v1.r, v1.g, v1.b);
            LOLUNIT_SET_CONTEXT(&rgb[0]);

            if (r != g || g != b)
                LOLUNIT_ASSERT_DOUBLES_EQUAL(v1.r, v3.r, 0.0001);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(v1.g, v3.g, 0.0001);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(v1.b, v3.b, 0.0001);

            LOLUNIT_UNSET_CONTEXT(&rgb[0]);
        }
    }

    LOLUNIT_TEST(RGBToHSL)
    {
        for (int r = 0; r < 20; r++)
        for (int g = 0; g < 20; g++)
        for (int b = 0; b < 20; b++)
        {
            vec3 v1 = vec3(r / 20.f, g / 20.f, b / 20.f);
            vec3 v2 = Color::RGBToHSL(v1);
            vec3 v3 = Color::HSVToHSL(Color::RGBToHSV(v1));

            String rgb = String::Printf("[%f %f %f]", v1.r, v1.g, v1.b);
            LOLUNIT_SET_CONTEXT(&rgb[0]);

            /* Don’t check hue if saturation is zero. */
            if (r != g || g != b)
                LOLUNIT_ASSERT_DOUBLES_EQUAL(v2.x, v3.x, 0.0001);
            /* Don’t check saturation if lightness is zero. */
            if (r || g || b)
                LOLUNIT_ASSERT_DOUBLES_EQUAL(v2.y, v3.y, 0.0001);
            LOLUNIT_ASSERT_DOUBLES_EQUAL(v2.z, v3.z, 0.0001);

            LOLUNIT_UNSET_CONTEXT(&rgb[0]);
        }
    }
};

} /* namespace lol */

