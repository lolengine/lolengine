//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Various maths constants
// ———————————————————————
//

#include <lol/base/types.h>

namespace lol
{

#define LOL_MATH_CONSTANT(name, value) \
    static ldouble const LD_##name = value; \
    static double const D_##name = (double)LD_##name; \
    static float const F_##name = (float)LD_##name;

/* These values are truncated, not rounded, because I don’t care. */

LOL_MATH_CONSTANT(TAU,  6.28318530717958647692528676655900576839433879875L)

LOL_MATH_CONSTANT(PI,   3.14159265358979323846264338327950288419716939937L)
LOL_MATH_CONSTANT(PI_2, 1.57079632679489661923132169163975144209858469968L)
LOL_MATH_CONSTANT(PI_3, 1.04719755119659774615421446109316762806572313312L)
LOL_MATH_CONSTANT(PI_4, 0.78539816339744830961566084581987572104929234984L)

LOL_MATH_CONSTANT(1_PI, 0.31830988618379067153776752674502872406891929148L)
LOL_MATH_CONSTANT(2_PI, 0.63661977236758134307553505349005744813783858296L)

LOL_MATH_CONSTANT(SQRT_2,   1.41421356237309504880168872420969807856967187537L)
LOL_MATH_CONSTANT(SQRT_3,   1.73205080756887729352744634150587236694280525381L)
LOL_MATH_CONSTANT(SQRT_1_2, 0.70710678118654752440084436210484903928483593768L)

#undef LOL_MATH_CONSTANT

} /* namespace lol */

