//
//  Lol Engine
//
//  Copyright © 2004—2017 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

/*
 * Noise rendering functions
 */

namespace lol
{

bool image::RenderRandom(ivec2 size)
{
    resize(size);
    vec4 *pixels = lock<PixelFormat::RGBA_F32>();

    for (int n = 0; n < size.x * size.y; ++n)
        pixels[n] = vec4(lol::rand(1.f),
                         lol::rand(1.f),
                         lol::rand(1.f),
                         1.f);

    unlock(pixels);

    return true;
}

} /* namespace lol */

