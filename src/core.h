//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The main header
// ---------------
//

#if !defined __LOL_CORE_H__
#define __LOL_CORE_H__

// CPU features
#undef LOL_FEATURE_CHEAP_BRANCHES
#undef LOL_FEATURE_VERY_CHEAP_BRANCHES

#if !defined __CELLOS_LV2__
#   define LOL_FEATURE_CHEAP_BRANCHES
#endif

// Optimisation helpers
#if defined __GNUC__
#   define __likely(x)   __builtin_expect(!!(x), 1)
#   define __unlikely(x) __builtin_expect(!!(x), 0)
#   define INLINEATTR __attribute__((always_inline))
#   if defined __CELLOS_LV2__
#      define FP_USE(x) __asm__("" : "+f" (x))
#   elif defined __x86_64__
#      define FP_USE(x) __asm__("" : "+x" (x))
#   elif defined __i386__ /* FIXME: this isn't good */
#      define FP_USE(x) __asm__("" : "+m" (x))
#   else
#      define FP_USE(x) (void)(x)
#   endif
#else
#   define __likely(x)   x
#   define __unlikely(x) x
#   define INLINEATTR
#   define FP_USE(x) (void)(x)
#endif

/* Ensure isnan() is present even on systems that don't define it, or
 * when -ffast-math is being used. */
#include <cmath>
#if defined __FAST_MATH__
#   undef isnan
#endif
#if !defined isnan
#   define isnan isnan
#   include <stdint.h>
static inline int isnan(float f)
{
    union { float f; uint32_t x; } u = { f };
    return (u.x << 1) > 0xff000000u;
}
#endif

// Base types
#include "trig.h"
#include "half.h"
#include "lol/math/real.h"
#include "lol/math/matrix.h"
#include "numeric.h"
#include "timer.h"
#include "thread/thread.h"

// Static classes
#include "log.h"
#include "platform.h"
#include "video.h"
#include "audio.h"
#include "scene.h"
#include "input.h"
#include "profiler.h"

// Entities
#include "entity.h"
#include "worldentity.h"

#include "emitter.h"
#include "font.h"
#include "gradient.h"
#include "sample.h"
#include "sprite.h"
#include "text.h"
#include "tileset.h"
#include "world.h"

// Other objects
#include "hash.h"
#include "dict.h"
#include "map.h"
#include "layer.h"
#include "gpu/vbo.h"
#include "shader/shader.h"
#include "image/image.h"
#include "application/application.h"

// Managers
#include "ticker.h"
#include "forge.h"
#include "tiler.h"
#include "sampler.h"

#endif // __LOL_CORE_H__

