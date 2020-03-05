//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdlib>
#include <functional>
#include <stdint.h>

namespace lol
{

//auto p = tickable::create<tickable>();

tickable::tickable()
{
}

tickable::~tickable()
{
}

void tickable::init()
{
}

void tickable::fini()
{
}

void tickable::tick_game(float seconds)
{
}

void tickable::tick_draw(float seconds, class Scene &scene)
{
}

} /* namespace lol */

