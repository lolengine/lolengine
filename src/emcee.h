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
// The Emcee class
// ---------------
// The Emcee manages tasks and their dependencies
//

#if !defined __LOL_EMCEE_H__
#define __LOL_EMCEE_H__

#include <stdint.h>

#include "entity.h"

namespace lol
{

class Emcee
{
public:
    static void Setup();
    static void Shutdown();

    static void SetState(Entity *entity, uint32_t state);
    static void SetStateWhenMatch(Entity *entity, uint32_t state,
                                  Entity *other_entity, uint32_t other_state);
};

} /* namespace lol */

#endif // __LOL_EMCEE_H__

