//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_CORE_TYPES_H__
#define __LOL_CORE_TYPES_H__

namespace lol
{

/* There are many reasons for wanting single-word type names, the most
 * important one being compilation speedups in our vector.h: we can hide
 * some global methods in namespaces that contain the name of the type,
 * but namespaces cannot have spaces in their names. */
typedef long double ldouble;

/* The “real” type used for real numbers. It’s a specialisation of the
 * “Real” template class. */
template<int N> class Real;
typedef Real<16> real;

/* The “half” type used for 16-bit floating point numbers. */
class half;

} /* namespace lol */

#endif // __LOL_CORE_TYPES_H__

