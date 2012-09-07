
#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "../Include/LolBtPhysicsIntegration.h"
#include "../Include/LolPhysics.h"
#include "../Include/EasyConstraint.h"

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
        current_simulation->ObjectRegistration(true, this);
    }
}

void EasyConstraint::RemoveFromSimulation(class Simulation* current_simulation)
{
    btDiscreteDynamicsWorld* dynamics_world = current_simulation->GetWorld();
    if (dynamics_world && m_typed_constraint)
    {
        dynamics_world->removeConstraint(m_typed_constraint);
        current_simulation->ObjectRegistration(false, this);
    }
}

#endif // HAVE_PHYS_USE_BULLET

} /* namespace phys */

} /* namespace lol */
