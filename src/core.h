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

// Base types
#include "trig.h"
#include "half.h"
#include "matrix.h"
#include "numeric.h"
#include "timer.h"

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
#include "shader/shader.h"
#include "image/image.h"

// Managers
#include "ticker.h"
#include "forge.h"
#include "tiler.h"
#include "sampler.h"

#endif // __LOL_CORE_H__

