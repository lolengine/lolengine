//
//  Lol Engine
//
//  Copyright © 2009—2019 Sam Hocevar <sam@hocevar.net>
//            © 2010—2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Bullet Physics integration in Lol
// ---------------------------------
//

namespace lol
{

// Override Gamegroups names for Physic-usage
// "_ENT_" means that this is a group for Entities that use EasyPhysic primitives.
// "_EZP_" means that this is a group for EasyPhysic primitives.
#define GAMEGROUP_EZP_CHAR_CTRLR    tickable::group::game::other_2
#define GAMEGROUP_SIMULATION        tickable::group::game::other_3

#define LOL2BT_UNIT 1.0f
#define BT2LOL_UNIT 1.0f

#define LOL2BT_SIZE 0.5f
#define BT2LOL_SIZE 2.0f

#define LOL2BT_VEC3(v) btVector3((v).x, (v).y, (v).z)
#define BT2LOL_VEC3(v) lol::vec3(v.getX(), v.getY(), v.getZ())

// Same as above with Unit taken into account
#define LOL2BTU_VEC3(v) (LOL2BT_VEC3(v) * LOL2BT_UNIT)
#define BT2LOLU_VEC3(v) (BT2LOL_VEC3(v) * BT2LOL_UNIT)

#define LOL2BT_QUAT(q) btQuaternion((q).x, (q).y, (q).z, (q).w)
#define BT2LOL_QUAT(q) lol::quat((q).getW(), BT2LOL_VEC3((q).getAxis())

} /* namespace lol */

