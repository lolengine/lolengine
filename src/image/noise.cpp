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
 * Noise rendering functions
 */

namespace lol
{

bool Image::RenderRandom(ivec2 size)
{
    SetSize(size);
    vec4 *pixels = Lock<PixelFormat::RGBA_F32>();

    for (int n = 0; n < size.x * size.y; ++n)
        pixels[n] = vec4(lol::rand(1.f),
                         lol::rand(1.f),
                         lol::rand(1.f),
                         1.f);

    Unlock(pixels);

    return true;
}

} /* namespace lol */

