//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The main header
// ---------------
//

#if !defined __LOL_CORE_H__
#define __LOL_CORE_H__

// System and CPU features
#undef LOL_FEATURE_THREADS
#undef LOL_FEATURE_CHEAP_BRANCHES
#undef LOL_FEATURE_VERY_CHEAP_BRANCHES

#if !defined EMSCRIPTEN
#   define LOL_FEATURE_THREADS 1
#endif

#if !defined __CELLOS_LV2__
#   define LOL_FEATURE_CHEAP_BRANCHES 1
#endif

// Optimisation helpers
#if defined __GNUC__
#   define __likely(x)   __builtin_expect(!!(x), 1)
#   define __unlikely(x) __builtin_expect(!!(x), 0)
#   define INLINEATTR __attribute__((always_inline))
#   if defined __CELLOS_LV2__ && !defined __SNC__
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


/* Ensure we have nullptr */
#if defined nullptr
    /* do nothing */
#elif defined __GNUC__
#   define nullptr __null
#else
#   include <cstddef>
#   define nullptr NULL
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

/* External declaration for LolFx files. */
#define LOLFX_RESOURCE_DECLARE(name) \
    extern "C" char const *LOLFX_RESOURCE_NAME(name)
#define LOLFX_RESOURCE_NAME(name) lolfx_resource_##name

/* If using NaCl or Android, override main() with our version */
#if defined __native_client__
#   define main lol_nacl_main
#elif defined __ANDROID__
#   define main lol_android_main
#endif

/* If using SDL on Windows or OS X, let it override main() */
#if defined USE_SDL && (defined _WIN32 || defined __APPLE__)
#   include <SDL_main.h>
#endif

// Base types
#include <lol/base/all.h>
#include <lol/math/all.h>
#include <lol/sys/all.h>
#include <lol/gpu/all.h>
#include <lol/image/all.h>
#include <lol/debug/all.h>

#include "numeric.h"

// Static classes
#include "platform.h"
#include "video.h"
#include "audio.h"
#include "scene.h"
#include "input/input.h"
#include "input/keyboard.h"
#include "input/stick.h"
#include "profiler.h"

// Entities
#include "entity.h"
#include "worldentity.h"

#include "camera.h"
#include "light.h"
#include "emitter.h"
#include "font.h"
#include "gradient.h"
#include "sample.h"
#include "sprite.h"
#include "text.h"
#include "tileset.h"
#include "world.h"

// Other objects
#include "dict.h"
#include "map.h"
#include "layer.h"
#include "mesh/mesh.h"
#include "application/application.h"
#include "easymesh/csgbsp.h"
#include "easymesh/easymesh.h"

// Managers
#include "ticker.h"
#include "forge.h"
#include "tiler.h"
#include "sampler.h"

#endif // __LOL_CORE_H__

