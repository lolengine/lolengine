//
//  Lol Engine
//
//  Copyright © 2004—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

/*
 * Image merge operations: merge, min/max, overlay, screen, multiply,
 * divide, add, sub, difference
 */

namespace lol
{

enum class MergeMode
{
    Mix,
    Min,
    Max,
    Overlay,
    Screen,
    Divide,
    Multiply,
    Add,
    Sub,
    Difference,
};

template<PixelFormat FORMAT, MergeMode MODE>
static image generic_merge(image &src1, image &src2, float alpha)
{
    using std::min, std::max, std::fabs;

    typedef typename PixelType<FORMAT>::type pixel_t;

    ASSERT(src1.size() == src2.size());
    int const count = src1.size().x * src2.size().y;

    image dst(src1.size());

    pixel_t const *src1p = src1.lock<FORMAT>();
    pixel_t const *src2p = src2.lock<FORMAT>();
    pixel_t *dstp = dst.lock<FORMAT>();

    for (int n = 0; n < count; ++n)
    {
        if (MODE == MergeMode::Mix)
            dstp[n] = lol::mix(src1p[n], src2p[n], alpha);
        else if (MODE == MergeMode::Min)
            dstp[n] = lol::min(src1p[n], src2p[n]);
        else if (MODE == MergeMode::Max)
            dstp[n] = lol::max(src1p[n], src2p[n]);
        else if (MODE == MergeMode::Overlay)
            dstp[n] = src1p[n] * (src1p[n] + 2.f * src2p[n]
                                                 * (pixel_t(1.f) - src1p[n]));
        else if (MODE == MergeMode::Screen)
            dstp[n] = src1p[n] + src2p[n] - src1p[n] * src2p[n];
        else if (MODE == MergeMode::Divide)
            dstp[n] = src1p[n] / (lol::max(src1p[n], src2p[n]) + pixel_t(1e-8f));
        else if (MODE == MergeMode::Multiply)
            dstp[n] = src1p[n] * src2p[n];
        else if (MODE == MergeMode::Add)
            dstp[n] = lol::min(src1p[n] + src2p[n], pixel_t(1.f));
        else if (MODE == MergeMode::Sub)
            dstp[n] = lol::max(src1p[n] - src2p[n], pixel_t(0.f));
        else if (MODE == MergeMode::Difference)
            dstp[n] = lol::fabs(src1p[n] - src2p[n]);
    }

    src1.unlock(src1p);
    src2.unlock(src2p);
    dst.unlock(dstp);

    return dst;
}

template<MergeMode MODE>
static image generic_merge(image &src1, image &src2, float alpha)
{
    bool gray1 = src1.format() == PixelFormat::Y_8
                  || src1.format() == PixelFormat::Y_F32;
    bool gray2 = src2.format() == PixelFormat::Y_8
                  || src2.format() == PixelFormat::Y_F32;
    if (gray1 && gray2)
        return generic_merge<PixelFormat::Y_F32, MODE>(src1, src2, alpha);
    else
        return generic_merge<PixelFormat::RGBA_F32, MODE>(src1, src2, alpha);

}

image image::Merge(image &src1, image &src2, float alpha)
{
    return generic_merge<MergeMode::Mix>(src1, src2, alpha);
}

image image::Mean(image &src1, image &src2)
{
    return generic_merge<MergeMode::Mix>(src1, src2, 0.5f);
}

image image::Min(image &src1, image &src2)
{
    return generic_merge<MergeMode::Min>(src1, src2, 0.0f);
}

image image::Max(image &src1, image &src2)
{
    return generic_merge<MergeMode::Max>(src1, src2, 0.0f);
}

image image::Overlay(image &src1, image &src2)
{
    return generic_merge<MergeMode::Overlay>(src1, src2, 0.0f);
}

image image::Screen(image &src1, image &src2)
{
    return generic_merge<MergeMode::Screen>(src1, src2, 0.0f);
}

image image::Divide(image &src1, image &src2)
{
    return generic_merge<MergeMode::Divide>(src1, src2, 0.0f);
}

image image::Multiply(image &src1, image &src2)
{
    return generic_merge<MergeMode::Multiply>(src1, src2, 0.0f);
}

image image::Add(image &src1, image &src2)
{
    return generic_merge<MergeMode::Add>(src1, src2, 0.0f);
}

image image::Sub(image &src1, image &src2)
{
    return generic_merge<MergeMode::Sub>(src1, src2, 0.0f);
}

image image::Difference(image &src1, image &src2)
{
    return generic_merge<MergeMode::Difference>(src1, src2, 0.0f);
}

} /* namespace lol */

