//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// Bullet Physics integration in Lol
// ------------------
//

#if !defined __LOLBTPHYSICSINTEGRATION_H__
#define __LOLBTPHYSICSINTEGRATION_H__

namespace lol
{

#ifdef HAVE_PHYS_USE_BULLET

#define LOL2BT_UNIT				1.0f
#define BT2LOL_UNIT				1.0f

#define LOL2BT_SIZE				0.5f
#define BT2LOL_SIZE				2.0f

#define LOL2BT_VEC3(ELEMENT)	btVector3((ELEMENT).x, (ELEMENT).y, (ELEMENT).z)
#define BT2LOL_VEC3(ELEMENT)	(*(lol::vec3*)(&(ELEMENT)))

#define LOL2BT_QUAT(ELEMENT)	btQuaternion((ELEMENT).x, (ELEMENT).y, (ELEMENT).z, (ELEMENT).w)
#define BT2LOL_QUAT(ELEMENT)	lol::quat((ELEMENT).getW(), BT2LOL_VEC3((ELEMENT).getAxis())

#endif // HAVE_PHYS_USE_BULLET

} /* namespace lol */

#endif /* __LOLBTPHYSICSINTEGRATION_H__ */

