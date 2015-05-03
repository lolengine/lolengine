//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Extra includes that aren't proper part of the engine yet
// --------------------------------------------------------
//

#include <lol/../utils.h>
#include <lol/../numeric.h>

// Lua
#include <lol/../lolua/baselua.h>

// Static classes
#include <lol/../platform.h>
#include <lol/../video.h>
#include <lol/../audio.h>
#include <lol/../scene.h>
#include <lol/../profiler.h>
#include <lol/../messageservice.h>

// Input
#include <lol/../input/input.h>
#include <lol/../input/controller.h>

// Engine
#include <lol/../engine/ticker.h>
#include <lol/../engine/world.h>
#include <lol/../engine/entity.h>
#include <lol/../engine/worldentity.h>

// Entities
#include <lol/../camera.h>
#include <lol/../light.h>
#include <lol/../emitter.h>
#include <lol/../font.h>
#include <lol/../gradient.h>
#include <lol/../sample.h>
#include <lol/../sprite.h>
#include <lol/../text.h>
#include <lol/../textureimage.h>
#include <lol/../tileset.h>
#include <lol/../lolimgui.h>

// Other objects
#include <lol/../dict.h>
#include <lol/../mesh/mesh.h>
#include <lol/../mesh/primitivemesh.h>
#include <lol/../application/application.h>
#include <lol/../easymesh/csgbsp.h>
#include <lol/../easymesh/easymesh.h>
#include <lol/../easymesh/easymeshlua.h>

// Managers
#include <lol/../forge.h>
#include <lol/../tiler.h>
#include <lol/../sampler.h>

