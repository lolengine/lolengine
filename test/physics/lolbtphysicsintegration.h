//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// Bullet Physics integration in Lol
// ------------------
//

#if !defined __LOLBTPHYSICSINTEGRATION_H__
#define __LOLBTPHYSICSINTEGRATION_H__

namespace lol
{
    //Override Gamegroups names for Physic-useage
    //"_ENT_" means that this is a group for Entities that use EasyPhysic primitives.
    //"_EZP_" means that this is a group for EasyPhysic primitives.
#define GAMEGROUP_ENT_INPUT            GAMEGROUP_BEFORE
#define GAMEGROUP_ENT_PLATFORM        GAMEGROUP_DEFAULT
#define GAMEGROUP_ENT_MAIN            GAMEGROUP_AFTER
#define GAMEGROUP_EZP_CHAR_CTRLR    GAMEGROUP_AFTER_0
#define GAMEGROUP_SIMULATION        GAMEGROUP_AFTER_1

#define LOL2BT_UNIT                1.0f
#define BT2LOL_UNIT                1.0f

#define LOL2BT_SIZE                0.5f
#define BT2LOL_SIZE                2.0f

#define LOL2BT_VEC3(ELEMENT)    btVector3((ELEMENT).x, (ELEMENT).y, (ELEMENT).z)
#define BT2LOL_VEC3(ELEMENT)    (*(lol::vec3*)(&(ELEMENT)))

//Same as above with Unit taken into account
#define LOL2BTU_VEC3(ELEMENT)    btVector3((ELEMENT).x * LOL2BT_UNIT, (ELEMENT).y * LOL2BT_UNIT, (ELEMENT).z * LOL2BT_UNIT)
#define BT2LOLU_VEC3(ELEMENT)    (*(lol::vec3*)(&(ELEMENT))) * BT2LOL_UNIT

#define LOL2BT_QUAT(ELEMENT)    btQuaternion((ELEMENT).x, (ELEMENT).y, (ELEMENT).z, (ELEMENT).w)
#define BT2LOL_QUAT(ELEMENT)    lol::quat((ELEMENT).getW(), BT2LOL_VEC3((ELEMENT).getAxis())

} /* namespace lol */

#endif /* __LOLBTPHYSICSINTEGRATION_H__ */

