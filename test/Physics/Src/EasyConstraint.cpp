//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2012 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#ifdef HAVE_PHYS_USE_BULLET
#include "../Include/EasyConstraint.h"
#endif //HAVE_PHYS_USE_BULLET

namespace lol
{

namespace phys
{

#ifdef HAVE_PHYS_USE_BULLET

//-------------------------------------------------------------------------
//EASY_CONSTRAINT
//--

void EasyConstraint::AddToSimulation(class Simulation* current_simulation)
{
	btDiscreteDynamicsWorld* dynamics_world = current_simulation->GetWorld();
	if (dynamics_world && m_typed_constraint)
	{
		dynamics_world->addConstraint(m_typed_constraint, m_disable_a2b_collision);
		current_simulation->AddToConstraint(this);
	}
}

void EasyConstraint::RemoveFromSimulation(class Simulation* current_simulation)
{
	btDiscreteDynamicsWorld* dynamics_world = current_simulation->GetWorld();
	if (dynamics_world, m_typed_constraint)
		dynamics_world->removeConstraint(m_typed_constraint);
}

#endif // HAVE_PHYS_USE_BULLET

} /* namespace phys */

} /* namespace lol */
