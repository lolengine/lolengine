//
// Lol Engine
//
// Copyright: (c) 2004-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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
static Image GenericMerge(Image &src1, Image &src2, float alpha)
{
    typedef typename PixelType<FORMAT>::type pixel_t;

    ASSERT(src1.GetSize() == src2.GetSize());
    int const count = src1.GetSize().x * src2.GetSize().y;

    Image dst(src1.GetSize());

    pixel_t const *src1p = src1.Lock<FORMAT>();
    pixel_t const *src2p = src2.Lock<FORMAT>();
    pixel_t *dstp = dst.Lock<FORMAT>();

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
            dstp[n] = lol::abs(src1p[n] - src2p[n]);
    }

    src1.Unlock(src1p);
    src2.Unlock(src2p);
    dst.Unlock(dstp);

    return dst;
}

template<MergeMode MODE>
static Image GenericMerge(Image &src1, Image &src2, float alpha)
{
    bool gray1 = src1.GetFormat() == PixelFormat::Y_8
                  || src1.GetFormat() == PixelFormat::Y_F32;
    bool gray2 = src2.GetFormat() == PixelFormat::Y_8
                  || src2.GetFormat() == PixelFormat::Y_F32;
    if (gray1 && gray2)
        return GenericMerge<PixelFormat::Y_F32, MODE>(src1, src2, alpha);
    else
        return GenericMerge<PixelFormat::RGBA_F32, MODE>(src1, src2, alpha);

}

Image Image::Merge(Image &src1, Image &src2, float alpha)
{
    return GenericMerge<MergeMode::Mix>(src1, src2, alpha);
}

Image Image::Mean(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Mix>(src1, src2, 0.5f);
}

Image Image::Min(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Min>(src1, src2, 0.0f);
}

Image Image::Max(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Max>(src1, src2, 0.0f);
}

Image Image::Overlay(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Overlay>(src1, src2, 0.0f);
}

Image Image::Screen(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Screen>(src1, src2, 0.0f);
}

Image Image::Divide(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Divide>(src1, src2, 0.0f);
}

Image Image::Multiply(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Multiply>(src1, src2, 0.0f);
}

Image Image::Add(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Add>(src1, src2, 0.0f);
}

Image Image::Sub(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Sub>(src1, src2, 0.0f);
}

Image Image::Difference(Image &src1, Image &src2)
{
    return GenericMerge<MergeMode::Difference>(src1, src2, 0.0f);
}

} /* namespace lol */

